#include "FrameHeader.h"

TCPServerSystem::TCPServerSystem() :
	mPacketDataBuffer(new SerializerBitWrite()),
	mRecvBuffer(new char[FrameDefine::CLIENT_RECV_BUFFER]),
	mReceiveBytesCount(0),
	mReceivePacketCount(0)
{
	mPacketSendCountList.resize(1 << 16);
	mPacketSendActiveTypeList.reserve(256);
}

void TCPServerSystem::quit()
{
	// 先停止所有子线程
	mThreadManager->destroyThread(mSendThread);
	mThreadManager->destroyThread(mReceiveThread);
	mThreadManager->destroyThread(mAcceptThread);
#ifdef LINUX
	if (mRecvEpollFD >= 0)
	{
		close(mRecvEpollFD);
		mRecvEpollFD = -1;
	}
#endif
	// 销毁所有客户端
	DELETE_MAP(mClientList);
	CLOSE_SOCKET(mSocket);
#ifdef WINDOWS
	// 需要在CLOSE_SOCKET后面调用
	if (mWinsockInitialized)
	{
		mWinsockInitialized = false;
		WSACleanup();
	}
#endif
	DELETE(mPacketDataBuffer);
	DELETE_ARRAY(mRecvBuffer);
}

void TCPServerSystem::init()
{
	if (isAvailable())
	{
		return;
	}

	mPort = (ushort)mFrameConfigSystem->getTCPPort();
	mHeartBeatTimeOut = mFrameConfigSystem->getHeartBeatTimeout();
	mOutputLog = mFrameConfigSystem->getOutputNetLog();
	if (mPort == 0)
	{
		return;
	}

	// 初始化Socket环境
#ifdef WINDOWS
	if (!mWinsockInitialized)
	{
		WSADATA wsd;
		if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
		{
			ERROR("WSAStartup failed!");
			return;
		}
		mWinsockInitialized = true;
	}
#endif

	// 创建监听Socket
	mSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (mSocket == INVALID_SOCKET)
	{
#ifdef WINDOWS
		const int errorCode = WSAGetLastError();
#else
		const int errorCode = errno;
#endif

		ERROR("socket failed! error:" + IToS(errorCode));
		return;
	}

#ifdef LINUX
	// 允许服务器重启后尽快重新绑定相同端口
	// 避免旧连接处于TIME_WAIT等状态时导致bind失败
	int reuseAddr = 1;
	if (setsockopt(mSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&reuseAddr, sizeof(reuseAddr)) != 0)
	{
		const int errorCode = errno;
		ERROR("设置SO_REUSEADDR失败,error:" + IToS(errorCode));
		CLOSE_SOCKET(mSocket);
		return;
	}
#endif

	// 设置服务器Socket地址
	sockaddr_in addrServ{};
	makeSockAddr(addrServ, INADDR_ANY, mPort);

	// 绑定Socket
	if (bind(mSocket, (const struct sockaddr*)&addrServ, sizeof(sockaddr_in)) != 0)
	{
#ifdef WINDOWS
		const int errorCode = WSAGetLastError();
#else
		const int errorCode = errno;
#endif
		ERROR("bind failed! port:" + IToS(mPort) + ",error:" + IToS(errorCode));
		CLOSE_SOCKET(mSocket);
#ifdef LINUX
		// 服务器监听端口都无法绑定,继续运行没有意义
		exit(EXIT_FAILURE);
#endif
		return;
	}

	// 开始监听
	if (listen(mSocket, mFrameConfigSystem->getBacklog()) != 0)
	{
#ifdef WINDOWS
		const int errorCode = WSAGetLastError();
#else
		const int errorCode = errno;
#endif
		ERROR("listen failed! port:" + IToS(mPort) + ",error:" + IToS(errorCode));
		CLOSE_SOCKET(mSocket);
#ifdef LINUX
		exit(EXIT_FAILURE);
#endif
		return;
	}

	// 因为可能会出现一直卡在send的情况
	// 所以设置Socket超时时间
#ifdef WINDOWS
	int timeout = 2000;
	int ret = setsockopt(mSocket, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout));
	ret |= setsockopt(mSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
#elif defined LINUX
	struct timeval timeout = { 2, 0 };
	int ret = setsockopt(mSocket, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(struct timeval));
#endif
	if (ret != 0)
	{
#ifdef WINDOWS
		const int errorCode = WSAGetLastError();
#else
		const int errorCode = errno;
#endif
		ERROR("设置socket选项失败,error:" + IToS(errorCode));
		CLOSE_SOCKET(mSocket);
		return;
	}

#ifdef LINUX
	// 创建接收epoll
	mRecvEpollFD = epoll_create1(EPOLL_CLOEXEC);
	if (mRecvEpollFD < 0)
	{
		const int errorCode = errno;
		ERROR("epoll_create1 failed! errno:" + IToS(errorCode));
		CLOSE_SOCKET(mSocket);
		return;
	}
#endif

	// 创建事件驱动发送线程,没有数据时完全休眠,endFrame提交数据后立即wake。
	mSendThread = mThreadManager->createEventThread("SendSocketTCP", [this](){ sendThread(); });
	// 创建接收线程
	mReceiveThread = mThreadManager->createThread("ReceiveSocketTCP", [this](){ recvThread(); });
	mReceiveThread->setTime(10);
	mReceiveThread->setPrintFPS(true);
	// 创建Accept线程
	mAcceptThread = mThreadManager->createThread("AcceptSocket", [this]() { acceptThread(); });
}

void TCPServerSystem::acceptThread()
{
	// 一次线程循环尽量取完当前已经完成握手的连接,避免每轮只accept一个导致连接建立速度被线程帧率限制。
	// 这里仍然保留上限,避免异常连接风暴时AcceptSocket线程长时间占用CPU。
	constexpr int MAX_ACCEPT_COUNT_PER_LOOP = 256;
	FOR(MAX_ACCEPT_COUNT_PER_LOOP)
	{
		// 监听Socket仍然保持原来的阻塞模式。accept前先用零超时select确认当前确实有连接可取,
		// 这样可以连续drain backlog,又不会在取完最后一个连接后阻塞在下一次accept上。
		fd_set fdread;
		FD_ZERO(&fdread);
		FD_SET(mSocket, &fdread);
		timeval tv{ 0, 0 };
#ifdef WINDOWS
		const int selectRet = select(0, &fdread, nullptr, nullptr, &tv);
#elif defined LINUX
		const int selectRet = select((int)mSocket + 1, &fdread, nullptr, nullptr, &tv);
#endif
		if (selectRet <= 0 || !FD_ISSET(mSocket, &fdread))
		{
			break;
		}

		sockaddr_in addr{};
#ifdef WINDOWS
		int nLen = sizeof(addr);
#elif defined LINUX
		socklen_t nLen = sizeof(addr);
#endif
		MY_SOCKET socket = accept(mSocket, (struct sockaddr*)&addr, &nLen);
	// 因为设置了2秒超时,所以accept每2秒就会返回一次INVALID_SOCKET
		if (socket == INVALID_SOCKET)
		{
#ifdef LINUX
			if (errno == EINTR)
			{
				--i;
				continue;
			}
#endif
			break;
		}

		MyString<16> ip;
	// 获得客户端IP,然后通知已经接收到一个客户端的连接
#ifdef WINDOWS
		ip.sprintf("%d.%d.%d.%d", addr.sin_addr.S_un.S_un_b.s_b1, addr.sin_addr.S_un.S_un_b.s_b2, addr.sin_addr.S_un.S_un_b.s_b3, addr.sin_addr.S_un.S_un_b.s_b4);
#elif defined LINUX
		int ip0 = getByte(addr.sin_addr.s_addr, 0);
		int ip1 = getByte(addr.sin_addr.s_addr, 1);
		int ip2 = getByte(addr.sin_addr.s_addr, 2);
		int ip3 = getByte(addr.sin_addr.s_addr, 3);
		ip.sprintf("%d.%d.%d.%d", ip0, ip1, ip2, ip3);
#endif

		int enable = 1;
#ifdef WINDOWS
		int ret = setsockopt(socket, IPPROTO_TCP, TCP_NODELAY, (const char*)&enable, sizeof(enable));
#elif defined LINUX
		int ret = setsockopt(socket, IPPROTO_TCP, TCP_NODELAY, (char*)&enable, sizeof(enable));
#endif
		if (ret != 0)
		{
			ERROR("设置socket选项失败");
			CLOSE_SOCKET(socket);
			continue;
		}
#ifdef LINUX
		const int socketFlags = fcntl(socket, F_GETFL, 0);
		if (socketFlags < 0 || fcntl(socket, F_SETFL, socketFlags | O_NONBLOCK) != 0)
		{
			ERROR("设置客户端socket为non-blocking失败, errno:" + IToS(errno));
			CLOSE_SOCKET(socket);
			continue;
		}
#endif
		{
			THREAD_LOCK(mAcceptLock);
			mAcceptBuffer.emplace(socket, ip.str());
		}
	}
}

void TCPServerSystem::sendThread()
{
	// 这里需要保证一定不会访问到被析构的客户端对象,所以需要整个一起加锁
	THREAD_LOCK(mSendLock);
	if (mSendClientList.isEmpty())
	{
		mSendThread->setEventWaitTime(-1);
		return;
	}
	bool needRetry = false;
#ifdef LINUX
	// Linux客户端socket使用non-blocking,正常直接send。
	// Buffer2和SendBuffer都采用“能搬多少搬多少”,SendBuffer发送完后立即再次从Buffer2补充。
	// 遇到EAGAIN时由CustomThread设置2ms超时等待,即使没有新业务包也会自动重试。
	for (TCPServerClient* client : mSendClientList)
	{
		if (client->isDeadClient())
		{
			continue;
		}

		StreamBuffer* sendBuffer = client->getSendBuffer();
		bool stopThisClient = false;
		while (!client->isDeadClient() && !stopThisClient)
		{
			// 每次SendBuffer出现空间后都继续从Buffer2补充,直到Buffer2也被耗尽。
			client->writeToSendBuffer();
			if (sendBuffer->getDataLength() == 0)
			{
				break;
			}

			while (sendBuffer->getDataLength() > 0 && !client->isDeadClient())
			{
				const int thisSendCount = send(client->getSocket(), sendBuffer->getData(), sendBuffer->getContiguousDataLength(), MSG_NOSIGNAL);
				if (thisSendCount > 0)
				{
					mSendByteCount += thisSendCount;
					sendBuffer->removeDataFromFront(thisSendCount);
					continue;
				}
				if (thisSendCount < 0)
				{
					const int errorCode = errno;
					if (errorCode == EAGAIN || errorCode == EWOULDBLOCK)
					{
						needRetry = true;
						stopThisClient = true;
						break;
					}
					if (errorCode == EINTR)
					{
						continue;
					}
				}
				checkSendRecvError(client, thisSendCount);
				stopThisClient = true;
				break;
			}
		}
	}
#elif defined WINDOWS
	// Windows保持select发送路径,事件线程只负责避免空闲轮询。
	thread_local static Vector<TCPServerClient*> needSendClient;
	needSendClient.clear();
	for (TCPServerClient* client : mSendClientList)
	{
		if (client->isDeadClient())
		{
			continue;
		}
		// 先尽可能将Buffer2中的数据搬入SendBuffer。
		client->writeToSendBuffer();
		if (client->getSendBuffer()->getDataLength() > 0)
		{
			needSendClient.add(client);
		}
	}
	thread_local static ArrayList<FD_SETSIZE, TCPServerClient*> selectedClient;
	// select查看后立即返回
	thread_local static timeval tv{ 0, 0 };
	thread_local static fd_set fdwrite;
	const int clientCount = needSendClient.size();
	for (int j = 0; j < clientCount; )
	{
		FD_ZERO(&fdwrite);
		selectedClient.clear();
		int maxSocket = 0;
		// 不能超过最大并发连接数,采用FD_SETSIZE一组的轮询方式
		FOR(FD_SETSIZE)
		{
			if (j >= clientCount)
			{
				break;
			}
			TCPServerClient* client = needSendClient[j++];
			if (client->isDeadClient())
			{
				continue;
			}
			selectedClient.add(client);
			FD_SET(client->getSocket(), &fdwrite);
			maxSocket = getMax(maxSocket, (int)client->getSocket());
		}
		// select第一个参数在windows中可以为0,但是在其他系统中需要设置
		if (select(maxSocket + 1, nullptr, &fdwrite, nullptr, &tv) <= 0)
		{
			// 当前没有可写Socket,但待发送数据仍在mSendBuffer中,必须定时重试。
			needRetry = true;
			continue;
		}
		for (TCPServerClient* client : selectedClient)
		{
			if (!FD_ISSET(client->getSocket(), &fdwrite))
			{
				needRetry = true;
				continue;
			}
			// 因为send可能无法一次将所有数据都发送出去,所以需要一直检测是否发送完,而且sendBuffer中数据本身就是分段存储的,是一个环形列表
			StreamBuffer* sendBuffer = client->getSendBuffer();
			bool stopThisClient = false;
			while (!client->isDeadClient() && !stopThisClient)
			{
				// SendBuffer发送出空间以后立即继续从Buffer2补数据,避免Buffer2剩余数据等待下一次业务唤醒。
				client->writeToSendBuffer();
				if (sendBuffer->getDataLength() == 0)
				{
					break;
				}

				while (sendBuffer->getDataLength() > 0 && !client->isDeadClient())
				{
					const int thisSendCount = send(client->getSocket(), sendBuffer->getData(), sendBuffer->getContiguousDataLength(), 0);
					if (thisSendCount > 0)
					{
						mSendByteCount += thisSendCount;
						sendBuffer->removeDataFromFront(thisSendCount);
						continue;
					}
					if (thisSendCount < 0 && WSAGetLastError() == WSAEWOULDBLOCK)
					{
						needRetry = true;
						stopThisClient = true;
						break;
					}
					// 出现异常就会断开此客户端,不再发送这个客户端后续的消息
					checkSendRecvError(client, thisSendCount);
					stopThisClient = true;
					break;
				}
			}
		}
	}
#endif
	// 下一轮由CustomThread统一决定是无限等待新事件,还是2ms后自动重试。
	mSendThread->setEventWaitTime(needRetry ? 2 : -1);
}

void TCPServerSystem::recvThread()
{
#ifdef LINUX
	if (mRecvEpollFD < 0)
	{
		return;
	}
	// disconnectSocket会在同一把锁下先从epoll移除再销毁client,因此event.data.ptr在这里始终有效。
	THREAD_LOCK(mRecvLock);
	const int clientCount = mRecvClientList.size();
	if (clientCount == 0)
	{
		return;
	}
	thread_local static vector<epoll_event> eventList;
	if ((int)eventList.size() < clientCount)
	{
		eventList.resize(clientCount);
	}
	const int eventCount = epoll_wait(mRecvEpollFD, eventList.data(), clientCount, 0);
	if (eventCount < 0)
	{
		if (errno != EINTR)
		{
			LOG("epoll_wait recv failed, errno:" + IToS(errno));
		}
		return;
	}
	for (int i = 0; i < eventCount; ++i)
	{
		TCPServerClient* client = (TCPServerClient*)eventList[i].data.ptr;
		if (client == nullptr || client->isDeadClient())
		{
			continue;
		}
		const uint eventFlags = eventList[i].events;
		const int nRecv = recv(client->getSocket(), mRecvBuffer, FrameDefine::CLIENT_RECV_BUFFER, 0);
		if (nRecv > 0)
		{
			mReceiveBytesCount += nRecv;
			client->recvData(mRecvBuffer, nRecv);
			int opt = 1;
			setsockopt(client->getSocket(), IPPROTO_TCP, TCP_QUICKACK, (char*)&opt, sizeof(opt));
			continue;
		}
		if (nRecv == 0)
		{
			checkSendRecvError(client, 0);
			continue;
		}
		const int errorCode = errno;
		if (errorCode == EAGAIN || errorCode == EWOULDBLOCK)
		{
			if ((eventFlags & (EPOLLRDHUP | EPOLLHUP)) != 0)
			{
				client->setDeadClient("客户端主动关闭连接", DEAD_TYPE::MANUAL_QUIT);
			}
			else if ((eventFlags & EPOLLERR) != 0)
			{
				client->setDeadClient("epoll检测到socket错误", DEAD_TYPE::SERVER_KICK_OUT);
			}
			continue;
		}
		if (errorCode != EINTR)
		{
			checkSendRecvError(client, nRecv);
		}
	}
#elif defined WINDOWS
	if (mRecvClientList.isEmpty())
	{
		return;
	}
	// 这里需要保证一定不会访问到被析构的客户端对象,所以需要整个一起加锁
	THREAD_LOCK(mRecvLock);
	thread_local static ArrayList<FD_SETSIZE, TCPServerClient*> selectedClient;
	// select查看后立即返回
	thread_local static timeval tv{ 0, 0 };
	thread_local static fd_set fdread;
	const int clientCount = mRecvClientList.size();
	for (int j = 0; j < clientCount; )
	{
		FD_ZERO(&fdread);
		selectedClient.clear();
		int maxSocket = 0;
		// 不能超过最大并发连接数,采用FD_SETSIZE一组的轮询方式
		FOR(FD_SETSIZE)
		{
			if (j >= clientCount)
			{
				break;
			}
			TCPServerClient* client = mRecvClientList[j++];
			if (client->isDeadClient())
			{
				continue;
			}
			selectedClient.add(client);
			FD_SET(client->getSocket(), &fdread);
			maxSocket = getMax(maxSocket, (int)client->getSocket());
		}
		// select第一个参数在windows中可以为0,但是在其他系统中需要设置
		if (select(maxSocket + 1, &fdread, nullptr, nullptr, &tv) <= 0)
		{
			continue;
		}
		for (TCPServerClient* client : selectedClient)
		{
			if (client->isDeadClient() || !FD_ISSET(client->getSocket(), &fdread))
			{
				continue;
			}
			const int nRecv = recv(client->getSocket(), mRecvBuffer, FrameDefine::CLIENT_RECV_BUFFER, 0);
			if (nRecv > 0)
			{
				mReceiveBytesCount += nRecv;
				client->recvData(mRecvBuffer, nRecv);
			}
			else
			{
				checkSendRecvError(client, nRecv);
			}
		}
	}
#endif
}

void TCPServerSystem::checkSendRecvError(TCPServerClient* client, const int successLength) const
{
	// recv返回0明确表示对端正常关闭TCP连接
	// 此时errno / WSAGetLastError没有意义
	if (successLength == 0)
	{
		client->setDeadClient("客户端主动关闭连接", DEAD_TYPE::MANUAL_QUIT);
		return;
	}
	// successLength < 0时才获取Socket错误码
#ifdef WINDOWS
	const int errorCode = WSAGetLastError();
#else
	const int errorCode = errno;
#endif
	client->setDeadClient("recv或send返回值小于0", DEAD_TYPE::SERVER_KICK_OUT);
	LOG("Socket错误码:" + IToS(errorCode));
#ifdef WINDOWS
	if (errorCode == WSAECONNRESET)
	{
		LOG("连接被对端重置,error:WSAECONNRESET");
	}
	else if (errorCode == WSAECONNABORTED)
	{
		LOG("连接异常终止,error:WSAECONNABORTED");
	}
	else if (errorCode == WSAENOTCONN)
	{
		LOG("Socket已经断开,error:WSAENOTCONN");
	}
	else if (errorCode == WSAESHUTDOWN)
	{
		LOG("Socket已经关闭发送,error:WSAESHUTDOWN");
	}
#elif defined LINUX
	if (errorCode == EPIPE)
	{
		LOG("管道损坏错误信号,error:EPIPE");
	}
	else if (errorCode == ECONNRESET)
	{
		LOG("连接被对端重置,error:ECONNRESET");
	}
	else if (errorCode == ECONNABORTED)
	{
		LOG("连接异常终止,error:ECONNABORTED");
	}
	else if (errorCode == ENOTCONN)
	{
		LOG("Socket已经断开,error:ENOTCONN");
	}
#endif
}

void TCPServerSystem::endFrame(const float elapsedTime)
{
	if (!isAvailable())
	{
		return;
	}
	bool hasSendData = false;
	for (const auto& item : mClientList)
	{
		TCPServerClient* client = item.second;
		if (client != nullptr && client->submitSendData())
		{
			hasSendData = true;
		}
	}
	// 无论本帧有多少Client、多少Packet,整个TCPServerSystem只唤醒发送线程一次。
	if (hasSendData && mSendThread != nullptr)
	{
		mSendThread->wake();
	}
}

void TCPServerSystem::update(const float elapsedTime)
{
	if (!isAvailable())
	{
		return;
	}
	// 将接收缓存列表同步到客户端列表,double check避免不必要的等待
	if (!mAcceptBuffer.isEmpty())
	{
		{
			// 复制一份列表,减少锁的时间
			THREAD_LOCK(mAcceptLock);
			mTempAcceptBuffer = move(mAcceptBuffer);
		}
		for (const auto& acceptInfo : mTempAcceptBuffer)
		{
			notifyAcceptClient(acceptInfo.first, acceptInfo.second);
		}
		mTempAcceptBuffer.clear();
	}

	// 更新客户端,找出是否有客户端需要断开连接
	thread_local static Vector<TCPServerClient*> tempLogoutClientList;
	tempLogoutClientList.clear();
	for (const auto& item : mClientList)
	{
		TCPServerClient* client = item.second;
		client->update(elapsedTime);
		// 将已经死亡的客户端放入列表
		if (client->isDeadClient())
		{
			tempLogoutClientList.emplace(client);
		}
	}

	// 断开死亡客户端,需要等待所有线程的当前帧都执行完毕,否则在此处直接销毁客户端会导致其他线程报错
	for (TCPServerClient* client : tempLogoutClientList)
	{
		disconnectSocket(client);
	}

	// 服务器心跳
	TICK_LOOP(elapsedTime, mServerHeartBeatTimeOut)
	{
		LOG("服务器心跳 : " + IToS(mServerHeartBeat++));
	}

	// 调试信息
	if (mEnablePacketLog)
	{
		TICK_LOOP(elapsedTime, mDumpPacketTimeInternal)
		{
			const int writePacketBytes = (int)(mWritePacketBytes / mDumpPacketTimeInternal / 1024);
			const int recvByteCount = (int)(mReceiveBytesCount / mDumpPacketTimeInternal / 1024);
			const int sendByteCount = (int)(mSendByteCount / mDumpPacketTimeInternal / 1024);
#ifdef STRESS_TEST
			const int clientSendByteCount = (int)(mClientSendBytes / mDumpPacketTimeInternal / 1024);
			const int clientRecvByteCount = (int)(mClientRecvBytes / mDumpPacketTimeInternal / 1024);
			LOG("客户端每秒发送字节数:" + IToS(clientSendByteCount) + "KB, " +
				"每秒接收字节数:" + IToS(clientRecvByteCount) + "KB, " +
				"每秒发包数:" + IToS((int)(mClientSendPacket / mDumpPacketTimeInternal)) + ", " +
				"每秒收包数:" + IToS((int)(mClientRecvPacket / mDumpPacketTimeInternal)));
#endif
			LOG("服务器每秒发送字节数:" + IToS(sendByteCount) + "KB, " +
				"每秒写入字节数:" + IToS(writePacketBytes) + "KB, " +
				"每秒写包数:" + IToS((int)(mWritePacketCount / mDumpPacketTimeInternal)) + ", " +
				"每秒发包数:" + IToS((int)(mSendPacketCount / mDumpPacketTimeInternal)));
			LOG("服务器每秒接收字节数:" + IToS(recvByteCount) + "KB, " +
				"每秒接收包数:" + IToS((int)(mReceivePacketCount / mDumpPacketTimeInternal)));
#ifdef STRESS_TEST
			mClientSendBytes = 0;
			mClientRecvBytes = 0;
			mClientSendPacket = 0;
			mClientRecvPacket = 0;
#endif
			mWritePacketBytes = 0;
			mWritePacketCount = 0;
			mSendPacketCount = 0;
			mReceiveBytesCount = 0;
			mReceivePacketCount = 0;
			mSendByteCount = 0;

			// x是消息类型,y是数量
			Vector<Vector2Int> temp;
			const int activeTypeCount = mPacketSendActiveTypeList.size();
			temp.reserve(activeTypeCount);
			int* packetSendCountList = mPacketSendCountList.data();
			FOR(activeTypeCount)
			{
				const ushort packetType = mPacketSendActiveTypeList[i];
				temp.emplace(packetType, packetSendCountList[packetType]);
			}
			quickSort(temp, comparePacketTypeCount);

			LOG(FToS(mDumpPacketTimeInternal) + "秒内发送数量top10的消息: start");
			const int count = getMin(temp.size(), 10);
			FOR(count)
			{
				const Vector2Int item = temp[i];
				LOG("类型:" + IToS(item.x) + ":" + mPacketSendNameMap.get(item.x) + ",数量:" + IToS(item.y));
			}
			LOG(FToS(mDumpPacketTimeInternal) + "秒内发送数量top10的消息: end");
			FOR(activeTypeCount)
			{
				packetSendCountList[mPacketSendActiveTypeList[i]] = 0;
			}
			mPacketSendActiveTypeList.clear();
		}
	}
}

void TCPServerSystem::writePacket(PacketTCP* packet)
{
	checkMainThread();
	++mWritePacketCount;
	// 发送消息前需要先写入数据到缓冲区
	// 但是由于一次写入不一定对应一次发送,可能会一次写入多次发送,所以将写入和发送分为两个函数
	// 而如果让外边存储一个缓冲区对象,使用起来又非常麻烦,所以就在TCPServerSystem中存储一个公用的缓冲区
	// 并且不能是静态的,静态对象的析构不是在主线程执行,会在检测执行线程时报错
	mPacketDataBuffer->clear();
	packet->writeToBuffer(mPacketDataBuffer, packet->hasSign());
	mWritePacketBytes += mPacketDataBuffer->getByteCount();
}

int TCPServerSystem::notifyAcceptClient(MY_SOCKET socket, const string& ip)
{
	// 加入主线程的客户端列表
	const int clientGUID = generateSocketGUID();
	if (mClientList.contains(clientGUID))
	{
		CLOSE_SOCKET(socket);
		LOG_NO_PRINT("客户端列表中已经包含了此客户端ID:" + UIToS(clientGUID));
		return 0;
	}

	// 加入客户端列表,三个列表都要加入
	TCPServerClient* client = new TCPServerClient(clientGUID, socket, ip);
	client->init();
#ifdef LINUX
	// Linux下先注册epoll再让发送/接收线程看到client。锁顺序与disconnectSocket保持一致。
	{
		THREAD_LOCK(mSendLock);
		THREAD_LOCK(mRecvLock);
		epoll_event event{};
		event.events = EPOLLIN | EPOLLRDHUP;
		event.data.ptr = client;
		if (epoll_ctl(mRecvEpollFD, EPOLL_CTL_ADD, client->getSocket(), &event) != 0)
		{
			ERROR("epoll_ctl ADD client failed, errno:" + IToS(errno));
			DELETE(client);
			return 0;
		}
		mSendClientList.add(client);
		mRecvClientList.add(client);
	}
#elif defined WINDOWS
	{
		THREAD_LOCK(mSendLock);
		mSendClientList.add(client);
	}
	{
		THREAD_LOCK(mRecvLock);
		mRecvClientList.add(client);
	}
#endif
	mClientList.add(clientGUID, client);
	if (mOutputLog)
	{
		LOG("IP:" + string(ip) + " 连接服务器! 当前连接数:" + IToS(mClientList.size()));
	}
	return clientGUID;
}

void TCPServerSystem::disconnectSocket(TCPServerClient* client)
{
	if (!isMainThread())
	{
		ERROR("只能在主线程中调用");
		return;
	}
	THREAD_LOCK(mSendLock);
	THREAD_LOCK(mRecvLock);
#ifdef LINUX
	// 必须在销毁client/socket前从epoll移除。recvThread使用同一把mRecvLock,不会拿到已经析构的event.data.ptr。
	if (mRecvEpollFD >= 0 && client->getSocket() != INVALID_SOCKET)
	{
		if (epoll_ctl(mRecvEpollFD, EPOLL_CTL_DEL, client->getSocket(), nullptr) != 0 && errno != ENOENT && errno != EBADF)
		{
			LOG("epoll_ctl DEL client failed, errno:" + IToS(errno));
		}
	}
#endif
	mSendClientList.remove(client);
	mRecvClientList.remove(client);
	mClientList.remove(client->getClientGUID());
	if (mOutputLog)
	{
		PLAYER_LOG("客户端断开连接:角色ID:" + LLToS(client->getPlayerGUID()) + 
					",原因:" + client->getDeadReason() + ", 剩余连接数:" + IToS(getClientCount()), client->getPlayerGUID());
	}
	// 退出账号的登录
	CmdNetServerLogoutAccount::execute(client);

	// 销毁客户端
	DELETE(client);
}

void TCPServerSystem::logoutAll()
{
	for (const auto& iter : mClientList)
	{
		iter.second->setDeadClient("全部退出登录", DEAD_TYPE::SERVER_KICK_OUT);
	}
}

void TCPServerSystem::encrypt(char* data, const int length, const byte param)
{
	if (data == nullptr || length <= 0)
	{
		return;
	}
	const byte* encryptKey = mFrameConfigSystem->getEncryptKey();
	const int encryptKeyLength = mFrameConfigSystem->getEncryptKeyLength();
	if (length < ENCRYPT_LOOKUP_MIN_LENGTH)
	{
		int keyIndex = (param ^ 223) & (encryptKeyLength - 1);
		for (int i = 0; i < length; ++i)
		{
			const byte keyChar = (byte)(encryptKey[keyIndex] ^ param);
			data[i] ^= keyChar;
			data[i] += keyChar >> 1;
			data[i] ^= (byte)(((keyChar * keyIndex) & (mKey0 * mKey1)) | ((mKey2 + mKey3) * keyIndex));
			keyIndex = (keyIndex + i) & (encryptKeyLength - 1);
		}
		return;
	}
	const EncryptLookupTable& lookup = EncryptLookupTable::getEncryptLookupTable(encryptKey, encryptKeyLength, mKey0, mKey1, mKey2, mKey3);
	const int sequenceLength = lookup.getSequenceLength();
	const byte* keyCharSequence = lookup.getKeyCharSequence(param);
	const byte* addValueSequence = lookup.getAddValueSequence(param);
	const byte* xorValueSequence = lookup.getXorValueSequence(param);
	char* curData = data;
	int remain = length;
	while (remain > 0)
	{
		const int processCount = remain < sequenceLength ? remain : sequenceLength;
		int i = 0;
		for (; i + 16 <= processCount; i += 16)
		{
			__m128i value = _mm_loadu_si128((const __m128i*)(curData + i));
			const __m128i keyValue = _mm_loadu_si128((const __m128i*)(keyCharSequence + i));
			const __m128i addValue = _mm_loadu_si128((const __m128i*)(addValueSequence + i));
			const __m128i xorValue = _mm_loadu_si128((const __m128i*)(xorValueSequence + i));
			value = _mm_xor_si128(value, keyValue);
			value = _mm_add_epi8(value, addValue);
			value = _mm_xor_si128(value, xorValue);
			_mm_storeu_si128((__m128i*)(curData + i), value);
		}
		for (; i < processCount; ++i)
		{
			byte value = (byte)curData[i];
			value ^= keyCharSequence[i];
			value = (byte)(value + addValueSequence[i]);
			value ^= xorValueSequence[i];
			curData[i] = (char)value;
		}
		curData += processCount;
		remain -= processCount;
	}
}

void TCPServerSystem::decrypt(char* data, const int length, const byte param)
{
	if (data == nullptr || length <= 0)
	{
		return;
	}
	const byte* encryptKey = mFrameConfigSystem->getEncryptKey();
	const int encryptKeyLength = mFrameConfigSystem->getEncryptKeyLength();
	if (length < ENCRYPT_LOOKUP_MIN_LENGTH)
	{
		int keyIndex = (param ^ 223) & (encryptKeyLength - 1);
		for (int i = 0; i < length; ++i)
		{
			const byte keyChar = (byte)(encryptKey[keyIndex] ^ param);
			data[i] ^= (byte)(((keyChar * keyIndex) & (mKey0 * mKey1)) | ((mKey2 + mKey3) * keyIndex));
			data[i] -= keyChar >> 1;
			data[i] ^= keyChar;
			keyIndex = (keyIndex + i) & (encryptKeyLength - 1);
		}
		return;
	}
	const EncryptLookupTable& lookup = EncryptLookupTable::getEncryptLookupTable(encryptKey, encryptKeyLength, mKey0, mKey1, mKey2, mKey3);
	const int sequenceLength = lookup.getSequenceLength();
	const byte* keyCharSequence = lookup.getKeyCharSequence(param);
	const byte* addValueSequence = lookup.getAddValueSequence(param);
	const byte* xorValueSequence = lookup.getXorValueSequence(param);
	char* curData = data;
	int remain = length;
	while (remain > 0)
	{
		const int processCount = remain < sequenceLength ? remain : sequenceLength;
		int i = 0;
		for (; i + 16 <= processCount; i += 16)
		{
			__m128i value = _mm_loadu_si128((const __m128i*)(curData + i));
			const __m128i keyValue = _mm_loadu_si128((const __m128i*)(keyCharSequence + i));
			const __m128i addValue = _mm_loadu_si128((const __m128i*)(addValueSequence + i));
			const __m128i xorValue = _mm_loadu_si128((const __m128i*)(xorValueSequence + i));
			value = _mm_xor_si128(value, xorValue);
			value = _mm_sub_epi8(value, addValue);
			value = _mm_xor_si128(value, keyValue);
			_mm_storeu_si128((__m128i*)(curData + i), value);
		}
		for (; i < processCount; ++i)
		{
			byte value = (byte)curData[i];
			value ^= xorValueSequence[i];
			value = (byte)(value - addValueSequence[i]);
			value ^= keyCharSequence[i];
			curData[i] = (char)value;
		}
		curData += processCount;
		remain -= processCount;
	}
}
