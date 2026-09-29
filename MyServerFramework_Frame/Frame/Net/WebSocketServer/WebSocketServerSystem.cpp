#include "FrameHeader.h"

WebSocketServerSystem::WebSocketServerSystem() :
	mPacketDataBuffer(new SerializerWrite()),
	mRecvBuffer(new char[FrameDefine::CLIENT_RECV_BUFFER]),
	mReceiveBytesCount(0),
	mReceivePacketCount(0)
{}

void WebSocketServerSystem::quit()
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

void WebSocketServerSystem::init()
{
	if (isAvailable())
	{
		return;
	}

	mPort = (ushort)mFrameConfigSystem->getWebSocketPort();
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
		ERROR("WebSocket socket failed,error:" + IToS(errorCode));
		return;
	}

#ifdef LINUX
	// 服务器重启时允许快速重新绑定端口
	int reuseAddr = 1;
	if (setsockopt(mSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&reuseAddr, sizeof(reuseAddr)) != 0)
	{
		const int errorCode = errno;
		ERROR("WebSocket设置SO_REUSEADDR失败,error:" + IToS(errorCode));
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
		ERROR("WebSocket bind failed,port:" + IToS(mPort) + ",error:" + IToS(errorCode));
		CLOSE_SOCKET(mSocket);

#ifdef LINUX
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
		ERROR("WebSocket listen failed,port:" + IToS(mPort) + ",error:" + IToS(errorCode));
		CLOSE_SOCKET(mSocket);

#ifdef LINUX
		exit(EXIT_FAILURE);
#endif
		return;
	}

#ifdef LINUX
	// Linux接收使用epoll
	mRecvEpollFD = epoll_create1(EPOLL_CLOEXEC);
	if (mRecvEpollFD < 0)
	{
		const int errorCode = errno;
		ERROR("WebSocket epoll_create1 failed,error:" + IToS(errorCode));
		CLOSE_SOCKET(mSocket);
		return;
	}
#endif

	mSendThread = mThreadManager->createThread("SendSocketWebSocket", [this] { sendThread(); });
	mSendThread->setTime(10);
	mSendThread->setPrintFPS(true);
	mReceiveThread = mThreadManager->createThread("ReceiveSocketWebSocket", [this] { recvThread(); });
	mReceiveThread->setTime(10);
	mReceiveThread->setPrintFPS(true);
	mAcceptThread = mThreadManager->createThread("AcceptSocketWebSocket", [this] { acceptThread(); });
}

void WebSocketServerSystem::acceptThread()
{
	// 一次尽量取完当前已经完成握手的TCP连接
	// 设置上限避免异常连接风暴时Accept线程一直占用CPU
	constexpr int MAX_ACCEPT_COUNT_PER_LOOP = 256;
	FOR(MAX_ACCEPT_COUNT_PER_LOOP)
	{
		fd_set fdread{};
		FD_ZERO(&fdread);
		FD_SET(mSocket, &fdread);
		timeval tv{ 0, 0 };
#ifdef WINDOWS
		const int selectRet = select(0, &fdread, nullptr, nullptr, &tv);
#elif defined LINUX
		const int selectRet = select((int)mSocket + 1, &fdread, nullptr, nullptr, &tv);
#endif
		if (selectRet == 0)
		{
			break;
		}
		if (selectRet < 0)
		{
#ifdef WINDOWS
			const int errorCode = WSAGetLastError();
			if (errorCode == WSAEINTR)
			{
				--i;
				continue;
			}
#else
			const int errorCode = errno;
			if (errorCode == EINTR)
			{
				--i;
				continue;
			}
#endif
			LOG("WebSocket accept select失败,error:" + IToS(errorCode));
			break;
		}
		if (!FD_ISSET(mSocket, &fdread))
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
		if (socket == INVALID_SOCKET)
		{
#ifdef WINDOWS
			const int errorCode = WSAGetLastError();
			if (errorCode == WSAEINTR)
			{
				--i;
				continue;
			}
#else
			const int errorCode = errno;
			if (errorCode == EINTR)
			{
				--i;
				continue;
			}
#endif
			break;
		}

		MyString<16> ip;
#ifdef WINDOWS
		ip.sprintf("%d.%d.%d.%d",
			addr.sin_addr.S_un.S_un_b.s_b1,
			addr.sin_addr.S_un.S_un_b.s_b2,
			addr.sin_addr.S_un.S_un_b.s_b3,
			addr.sin_addr.S_un.S_un_b.s_b4);
#elif defined LINUX
		const int ip0 = getByte(addr.sin_addr.s_addr, 0);
		const int ip1 = getByte(addr.sin_addr.s_addr, 1);
		const int ip2 = getByte(addr.sin_addr.s_addr, 2);
		const int ip3 = getByte(addr.sin_addr.s_addr, 3);
		ip.sprintf("%d.%d.%d.%d", ip0, ip1, ip2, ip3);
#endif

		// 禁用Nagle
		int enable = 1;
#ifdef WINDOWS
		int ret = setsockopt(socket, IPPROTO_TCP, TCP_NODELAY, (const char*)&enable, sizeof(enable));
#elif defined LINUX
		int ret = setsockopt(socket, IPPROTO_TCP, TCP_NODELAY, (char*)&enable, sizeof(enable));
#endif
		if (ret != 0)
		{
			ERROR("WebSocket设置TCP_NODELAY失败");
			CLOSE_SOCKET(socket);
			continue;
		}

#ifdef WINDOWS
		// Windows仍然是阻塞Socket + select
		// 防止极端情况下send卡住太久
		int timeout = 2000;
		ret = setsockopt(socket, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout));
		ret |= setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
		if (ret != 0)
		{
			const int errorCode = WSAGetLastError();
			ERROR("WebSocket设置Socket超时失败,error:" + IToS(errorCode));
			CLOSE_SOCKET(socket);
			continue;
		}
#elif defined LINUX
		// Linux客户端Socket使用non-blocking
		const int socketFlags = fcntl(socket, F_GETFL, 0);
		if (socketFlags < 0 || fcntl(socket, F_SETFL, socketFlags | O_NONBLOCK) != 0)
		{
			const int errorCode = errno;
			ERROR("WebSocket设置客户端socket为non-blocking失败,error:" + IToS(errorCode));
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

void WebSocketServerSystem::sendThread()
{
	if (mSendClientList.isEmpty())
	{
		return;
	}

	// 保证send过程中client不会被主线程析构
	THREAD_LOCK(mSendLock);

#ifdef LINUX
	// Linux使用non-blocking Socket
	// 直接尝试send,EAGAIN时留到下一轮继续
	for (WebSocketServerClient* client : mSendClientList)
	{
		if (client->isDeadClient())
		{
			continue;
		}
		client->writeToSendBuffer();
		const auto& sendBuffer = client->getSendBuffer();
		while (!sendBuffer.isEmpty() && !client->isDeadClient())
		{
			const auto& item = sendBuffer[0];
			const int sendOffset = client->getSendOffset();
			const int dataLength = item.second.x;
			const int remainLength = dataLength - sendOffset;
			if (remainLength <= 0)
			{
				client->consumeSendData(0);
				continue;
			}

			const int thisSendCount = send(client->getSocket(), item.first + sendOffset, remainLength, MSG_NOSIGNAL);
			if (thisSendCount > 0)
			{
				client->consumeSendData(thisSendCount);
				continue;
			}
			if (thisSendCount < 0)
			{
				const int errorCode = errno;
				if (errorCode == EAGAIN || errorCode == EWOULDBLOCK)
				{
					// 当前Socket发送缓冲区已满,下一轮继续
					break;
				}
				if (errorCode == EINTR)
				{
					continue;
				}
			}

			checkSendRecvError(client, thisSendCount);
			break;
		}
	}

#elif defined WINDOWS
	thread_local static Vector<WebSocketServerClient*> needSendClient;
	needSendClient.clear();
	for (WebSocketServerClient* client : mSendClientList)
	{
		if (client->isDeadClient())
		{
			continue;
		}
		client->writeToSendBuffer();
		if (!client->getSendBuffer().isEmpty())
		{
			needSendClient.add(client);
		}
	}

	thread_local static ArrayList<FD_SETSIZE, WebSocketServerClient*> selectedClient;
	thread_local static fd_set fdwrite;
	const int clientCount = needSendClient.size();
	for (int j = 0; j < clientCount; )
	{
		FD_ZERO(&fdwrite);
		selectedClient.clear();
		FOR(FD_SETSIZE)
		{
			if (j >= clientCount)
			{
				break;
			}
			WebSocketServerClient* client = needSendClient[j++];
			if (client->isDeadClient())
			{
				continue;
			}
			selectedClient.add(client);
			FD_SET(client->getSocket(), &fdwrite);
		}
		if (selectedClient.isEmpty())
		{
			continue;
		}

		timeval tv{ 0, 0 };
		const int selectResult = select(0, nullptr, &fdwrite, nullptr, &tv);
		if (selectResult == 0)
		{
			continue;
		}
		if (selectResult < 0)
		{
			const int errorCode = WSAGetLastError();
			if (errorCode != WSAEINTR)
			{
				LOG("WebSocket发送select失败,error:" + IToS(errorCode));
			}
			continue;
		}

		for (WebSocketServerClient* client : selectedClient)
		{
			if (client->isDeadClient() || !FD_ISSET(client->getSocket(), &fdwrite))
			{
				continue;
			}
			const auto& sendBuffer = client->getSendBuffer();
			while (!sendBuffer.isEmpty() && !client->isDeadClient())
			{
				const auto& item = sendBuffer[0];
				const int sendOffset = client->getSendOffset();
				const int dataLength = item.second.x;
				const int remainLength = dataLength - sendOffset;
				if (remainLength <= 0)
				{
					client->consumeSendData(0);
					continue;
				}
				const int thisSendCount = send(client->getSocket(), item.first + sendOffset, remainLength, 0);
				if (thisSendCount > 0)
				{
					client->consumeSendData(thisSendCount);
					continue;
				}

				if (thisSendCount < 0)
				{
					const int errorCode = WSAGetLastError();
					if (errorCode == WSAEINTR)
					{
						continue;
					}
					if (errorCode == WSAEWOULDBLOCK)
					{
						break;
					}
				}
				checkSendRecvError(client, thisSendCount);
				break;
			}
		}
	}
#endif
}

void WebSocketServerSystem::recvThread()
{
#ifdef LINUX
	if (mRecvEpollFD < 0)
	{
		return;
	}

	// disconnectSocket会持有相同的mRecvLock,
	// 所以event.data.ptr在本轮处理过程中不会变成野指针
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
		const int errorCode = errno;
		if (errorCode != EINTR)
		{
			LOG("WebSocket epoll_wait失败,error:" + IToS(errorCode));
		}
		return;
	}

	for (int i = 0; i < eventCount; ++i)
	{
		WebSocketServerClient* client = (WebSocketServerClient*)eventList[i].data.ptr;
		if (client == nullptr || client->isDeadClient())
		{
			continue;
		}

		const uint eventFlags = eventList[i].events;
		bool receivedData = false;
		// level-trigger模式下把当前Socket已经到达的数据尽量读完
		for (;;)
		{
			const int nRecv = recv(client->getSocket(), mRecvBuffer, FrameDefine::CLIENT_RECV_BUFFER, 0);
			if (nRecv > 0)
			{
				receivedData = true;
				mReceiveBytesCount += nRecv;
				client->recvData(mRecvBuffer, nRecv);
				if (client->isDeadClient())
				{
					break;
				}
				continue;
			}
			if (nRecv == 0)
			{
				checkSendRecvError(client, 0);
				break;
			}
			const int errorCode = errno;
			if (errorCode == EINTR)
			{
				continue;
			}
			if (errorCode == EAGAIN || errorCode == EWOULDBLOCK)
			{
				break;
			}
			checkSendRecvError(client, nRecv);
			break;
		}

		if (receivedData && !client->isDeadClient())
		{
			int opt = 1;
			setsockopt(client->getSocket(), IPPROTO_TCP, TCP_QUICKACK, (char*)&opt, sizeof(opt));
		}
		if (client->isDeadClient())
		{
			continue;
		}

		if ((eventFlags & EPOLLERR) != 0)
		{
			int socketError = 0;
			socklen_t errorLength = sizeof(socketError);
			getsockopt(client->getSocket(), SOL_SOCKET, SO_ERROR, (char*)&socketError, &errorLength);
			if (socketError != 0)
			{
				LOG("WebSocket epoll检测到socket错误,error:" + IToS(socketError));
			}
			client->setDeadClient("epoll检测到socket错误", DEAD_TYPE::SERVER_KICK_OUT);
			continue;
		}
		if ((eventFlags & (EPOLLRDHUP | EPOLLHUP)) != 0)
		{
			client->setDeadClient("客户端主动关闭连接", DEAD_TYPE::MANUAL_QUIT);
		}
	}

#elif defined WINDOWS
	if (mRecvClientList.isEmpty())
	{
		return;
	}

	THREAD_LOCK(mRecvLock);
	thread_local static ArrayList<FD_SETSIZE, WebSocketServerClient*> selectedClient;
	thread_local static fd_set fdread;
	const int clientCount = mRecvClientList.size();
	for (int j = 0; j < clientCount; )
	{
		FD_ZERO(&fdread);
		selectedClient.clear();
		FOR(FD_SETSIZE)
		{
			if (j >= clientCount)
			{
				break;
			}
			WebSocketServerClient* client = mRecvClientList[j++];
			if (client->isDeadClient())
			{
				continue;
			}
			selectedClient.add(client);
			FD_SET(client->getSocket(), &fdread);
		}
		if (selectedClient.isEmpty())
		{
			continue;
		}

		timeval tv{ 0, 0 };
		const int selectResult = select(0, &fdread, nullptr, nullptr, &tv);
		if (selectResult == 0)
		{
			continue;
		}
		if (selectResult < 0)
		{
			const int errorCode = WSAGetLastError();
			if (errorCode != WSAEINTR)
			{
				LOG("WebSocket接收select失败,error:" + IToS(errorCode));
			}
			continue;
		}

		for (WebSocketServerClient* client : selectedClient)
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

void WebSocketServerSystem::checkSendRecvError(WebSocketServerClient* client, const int successLength) const
{
	// recv == 0明确表示对端正常关闭
	// 此时errno / WSAGetLastError没有意义
	if (successLength == 0)
	{
		client->setDeadClient("客户端主动关闭连接", DEAD_TYPE::MANUAL_QUIT);
		return;
	}

#ifdef WINDOWS
	const int errorCode = WSAGetLastError();
	// 临时错误,连接并没有断开
	if (errorCode == WSAEINTR || errorCode == WSAEWOULDBLOCK)
	{
		return;
	}
	client->setDeadClient("recv或send返回值小于0", DEAD_TYPE::SERVER_KICK_OUT);
	LOG("WebSocket Socket错误码:" + IToS(errorCode));
	if (errorCode == WSAECONNRESET)
	{
		LOG("WebSocket连接被对端重置,error:WSAECONNRESET");
	}
	else if (errorCode == WSAECONNABORTED)
	{
		LOG("WebSocket连接异常终止,error:WSAECONNABORTED");
	}
	else if (errorCode == WSAENOTCONN)
	{
		LOG("WebSocket已经断开,error:WSAENOTCONN");
	}
	else if (errorCode == WSAESHUTDOWN)
	{
		LOG("WebSocket已经关闭发送,error:WSAESHUTDOWN");
	}
#elif defined LINUX
	const int errorCode = errno;
	// 临时错误,连接并没有断开
	if (errorCode == EINTR || errorCode == EAGAIN || errorCode == EWOULDBLOCK)
	{
		return;
	}

	client->setDeadClient("recv或send返回值小于0", DEAD_TYPE::SERVER_KICK_OUT);
	LOG("WebSocket Socket错误码:" + IToS(errorCode));
	if (errorCode == EPIPE)
	{
		LOG("WebSocket管道损坏,error:EPIPE");
	}
	else if (errorCode == ECONNRESET)
	{
		LOG("WebSocket连接被对端重置,error:ECONNRESET");
	}
	else if (errorCode == ECONNABORTED)
	{
		LOG("WebSocket连接异常终止,error:ECONNABORTED");
	}
	else if (errorCode == ENOTCONN)
	{
		LOG("WebSocket已经断开,error:ENOTCONN");
	}
#endif
}

void WebSocketServerSystem::update(const float elapsedTime)
{
	if (!isAvailable())
	{
		return;
	}

	// 将Accept线程产生的连接转移到主线程
	if (!mAcceptBuffer.isEmpty())
	{
		{
			THREAD_LOCK(mAcceptLock);
			mTempAcceptBuffer = move(mAcceptBuffer);
		}
		for (const auto& acceptInfo : mTempAcceptBuffer)
		{
			notifyAcceptClient(acceptInfo.first, acceptInfo.second);
		}
		mTempAcceptBuffer.clear();
	}

	// 更新客户端,找出需要断开的客户端
	thread_local static Vector<pair<int, string>> tempLogoutClientList;
	tempLogoutClientList.clear();

	for (const auto& item : mClientList)
	{
		WebSocketServerClient* client = item.second;
		client->update(elapsedTime);
		if (client->isDeadClient())
		{
			tempLogoutClientList.emplace(item.first, client->getDeadReason());
		}
	}

	// 主线程统一断开并销毁客户端
	for (const auto& logoutInfo : tempLogoutClientList)
	{
		disconnectSocket(logoutInfo.first, logoutInfo.second);
	}

	// 服务器心跳
	if (tickTimerLoop(mCurServerHeartBeatTime, elapsedTime, mServerHeartBeatTimeOut))
	{
		LOG("服务器心跳 : " + IToS(mServerHeartBeat++));
	}

	if (tickTimerLoop(mDumpPacketTimer, elapsedTime, mDumpPacketTimeInternal))
	{
		const int writePacketBytes = (int)(mWritePacketBytes / mDumpPacketTimeInternal);
		const int writePacketCount = (int)(mWritePacketCount / mDumpPacketTimeInternal);
		const int sendPacketCount = (int)(mSendPacketCount / mDumpPacketTimeInternal);
		const int recvByteCount = (int)(mReceiveBytesCount / mDumpPacketTimeInternal);
		const int recvPacketCount = (int)(mReceivePacketCount / mDumpPacketTimeInternal);
		LOG("每秒写入字节数:" + IToS(writePacketBytes) + ", 每秒写包数:" + IToS(writePacketCount) + ", 每秒发包数:" + IToS(sendPacketCount));
		LOG("每秒接收字节数:" + IToS(recvByteCount) + ", 每秒接收包数:" + IToS(recvPacketCount));
		mWritePacketBytes = 0;
		mWritePacketCount = 0;
		mSendPacketCount = 0;
		mReceiveBytesCount = 0;
		mReceivePacketCount = 0;
	}
}

void WebSocketServerSystem::writePacket(PacketWebSocket* packet)
{
	checkMainThread();
	++mWritePacketCount;
	// 发送消息前先序列化到公共缓冲区
	mPacketDataBuffer->clear();
	packet->writeToBuffer(mPacketDataBuffer);
	mWritePacketBytes += mPacketDataBuffer->getDataSize();
}

int WebSocketServerSystem::notifyAcceptClient(MY_SOCKET socket, const string& ip)
{
	const int clientGUID = generateSocketGUID();
	if (mClientList.contains(clientGUID))
	{
		CLOSE_SOCKET(socket);
		LOG_NO_PRINT("客户端列表中已经包含了此客户端ID:" + UIToS(clientGUID));
		return 0;
	}

	WebSocketServerClient* client = new WebSocketServerClient(clientGUID, socket, ip);
	client->init();
#ifdef LINUX
	// Linux先注册epoll,注册成功以后才让发送/接收线程看到client
	{
		THREAD_LOCK(mSendLock);
		THREAD_LOCK(mRecvLock);
		epoll_event event{};
		event.events = EPOLLIN | EPOLLRDHUP;
		event.data.ptr = client;
		if (epoll_ctl(mRecvEpollFD, EPOLL_CTL_ADD, client->getSocket(), &event) != 0)
		{
			const int errorCode = errno;
			ERROR("WebSocket epoll_ctl ADD client failed,error:" + IToS(errorCode));
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

	// 所有底层注册成功以后最后加入主线程列表
	mClientList.add(clientGUID, client);
	if (mOutputLog)
	{
		LOG("IP:" + string(ip) + " 连接WebSocket服务器! 当前连接数:" + IToS(mClientList.size()));
	}
	return clientGUID;
}

void WebSocketServerSystem::disconnectSocket(const int clientGUID, const string& reason)
{
	if (!isMainThread())
	{
		ERROR("只能在主线程中调用");
		return;
	}

	const auto iterClient = mClientList.find(clientGUID);
	if (iterClient == mClientList.end())
	{
		return;
	}
	WebSocketServerClient* client = iterClient->second;
	THREAD_LOCK(mSendLock);
	THREAD_LOCK(mRecvLock);
#ifdef LINUX
	// 必须先从epoll中移除再销毁client
	if (mRecvEpollFD >= 0 && client->getSocket() != INVALID_SOCKET)
	{
		if (epoll_ctl(mRecvEpollFD, EPOLL_CTL_DEL, client->getSocket(), nullptr) != 0 && errno != ENOENT && errno != EBADF)
		{
			LOG("WebSocket epoll_ctl DEL client failed,error:" + IToS(errno));
		}
	}
#endif
	mSendClientList.remove(client);
	mRecvClientList.remove(client);
	mClientList.remove(iterClient);

	if (mOutputLog)
	{
		PLAYER_LOG("WebSocket客户端断开连接:角色ID:" + LLToS(client->getPlayerGUID()) + ",原因:" + reason +
					", 剩余连接数:" + IToS(getClientCount()), client->getPlayerGUID());
	}
	// 退出账号登录
	CmdNetServerLogoutAccount::execute(client);
	DELETE(client);
}

void WebSocketServerSystem::logoutAll()
{
	for (const auto& iter : mClientList)
	{
		iter.second->setDeadClient("全部退出登录", DEAD_TYPE::SERVER_KICK_OUT);
	}
}