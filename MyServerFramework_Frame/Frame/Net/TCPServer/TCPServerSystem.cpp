#include "FrameHeader.h"

TCPServerSystem::TCPServerSystem() :
	mPacketDataBuffer(new SerializerBitWrite()),
	mRecvBuffer(new char[FrameDefine::CLIENT_RECV_BUFFER]),
	mReceiveBytesCount(0),
	mReceivePacketCount(0)
{}

void TCPServerSystem::quit()
{
	// 先停止所有子线程
	mThreadManager->destroyThread(mSendThread);
	mThreadManager->destroyThread(mReceiveThread);
	mThreadManager->destroyThread(mAcceptThread);
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
	if (mWinsockInitialized)
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

	//初始化Socket环境
#ifdef WINDOWS
	WSADATA wsd;
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		ERROR("WSAStartup failed!");
		return;
	}
	mWinsockInitialized = true;
#endif
	//创建监听的Socket
	mSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (mSocket == INVALID_SOCKET)
	{
		ERROR("socket failed!");
		return;
	}

	//设置服务器Socket地址
	sockaddr_in addrServ;
	makeSockAddr(addrServ, INADDR_ANY, mPort);
	//绑定Sockets Server
	int bindCode = bind(mSocket, (const struct sockaddr*)&addrServ, sizeof(sockaddr_in));
	if (bindCode != 0)
	{
		CLOSE_SOCKET(mSocket);
		ERROR("bind failed! code:" + IToS(bindCode));
		// linux下只要绑定端口失败就退出,否则会一直无法连接此服务器
#ifdef LINUX
		exit(0);
#endif
		return;
	}
	//在Sockets Server上进行监听
	if (listen(mSocket, mFrameConfigSystem->getBacklog()) != 0)
	{
		CLOSE_SOCKET(mSocket);
		ERROR("listen failed!");
		// linux下只要绑定端口失败就退出,否则会一直无法连接此服务器
#ifdef LINUX
		exit(0);
#endif
		return;
	}
	// 因为可能会出现一直卡在send的情况,所以给send添加一个超时时间
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
		ERROR("设置socket选项失败");
		return;
	}
	mSendThread = mThreadManager->createThread("SendSocketTCP", [this]{sendThread(); });
	mSendThread->setTime(10);
	mSendThread->setPrintFPS(true);
	mReceiveThread = mThreadManager->createThread("ReceiveSocketTCP", [this]{recvThread(); });
	mReceiveThread->setTime(10);
	mReceiveThread->setPrintFPS(true);
	mAcceptThread = mThreadManager->createThread("AcceptSocket", [this]{acceptThread(); });
}

void TCPServerSystem::acceptThread()
{
	sockaddr_in addr;
#ifdef WINDOWS
	int nLen = sizeof(addr);
#elif defined LINUX
	socklen_t nLen = sizeof(addr);
#endif
	MY_SOCKET socket = accept(mSocket, (struct sockaddr*)&addr, &nLen);
	// 因为设置了2秒超时,所以accept每2秒就会返回一次INVALID_SOCKET
	if (socket == INVALID_SOCKET)
	{
		return;
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
		return;
	}
	THREAD_LOCK(mAcceptLock);
	mAcceptBuffer.emplace(socket, ip.str());
}

void TCPServerSystem::sendThread()
{
	if (mSendClientList.isEmpty())
	{
		return;
	}
	// 这里需要保证一定不会访问到被析构的客户端对象,所以需要整个一起加锁
	THREAD_LOCK(mSendLock);
	// 遍历筛选出需要发送数据的客户端列表
	thread_local static Vector<TCPServerClient*> needSendClient;
	needSendClient.clear();
	for (TCPServerClient* client : mSendClientList)
	{
		if (client->isDeadClient())
		{
			continue;
		}
		// 确认将所有待发送的数据写入到发送缓冲区
		client->writeToSendBuffer();
		// 只检查有数据需要写的客户端
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
#ifdef LINUX
			// 因为在linux上会根据socket的值放入到指定位置,所以值不能超过最大长度,否则放不进去
			if (client->getSocket() >= FD_SETSIZE)
			{
				continue;
			}
#endif
			selectedClient.add(client);
			FD_SET(client->getSocket(), &fdwrite);
			maxSocket = getMax(maxSocket, (int)client->getSocket());
		}
		// select第一个参数在windows中可以为0,但是在其他系统中需要设置
		if (select(maxSocket + 1, nullptr, &fdwrite, nullptr, &tv) <= 0)
		{
			continue;
		}
		for (TCPServerClient* client : selectedClient)
		{
			if (!FD_ISSET(client->getSocket(), &fdwrite))
			{
				continue;
			}
			// 因为send可能无法一次将所有数据都发送出去,所以需要一直检测是否发送完,而且sendBuffer中数据本身就是分段存储的,是一个环形列表
			StreamBuffer* sendBuffer = client->getSendBuffer();
			while (sendBuffer->getDataLength() > 0 && !client->isDeadClient())
			{
				int flags = 0;
#ifdef LINUX
				flags = MSG_NOSIGNAL;
#endif
				const int thisSendCount = send(client->getSocket(), sendBuffer->getData(), sendBuffer->getContiguousDataLength(), flags);
				// 检查是否send有错误
				if (thisSendCount > 0)
				{
					mSendByteCount += thisSendCount;
					sendBuffer->removeDataFromFront(thisSendCount);
				}
				else
				{
					// 出现异常就会断开此客户端,不再发送这个客户端后续的消息
					checkSendRecvError(client, thisSendCount);
					break;
				}
			}
		}
	}
}

void TCPServerSystem::recvThread()
{
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
#ifdef LINUX
			if (client->getSocket() >= FD_SETSIZE)
			{
				continue;
			}
#endif
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
#ifdef LINUX
			int opt = 1;
			setsockopt(client->getSocket(), IPPROTO_TCP, TCP_QUICKACK, (char*)&opt, sizeof(opt));
#endif
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
}

void TCPServerSystem::checkSendRecvError(TCPServerClient* client, const int successLength) const
{
	// 客户端可能已经与服务器断开了连接,先立即标记该客户端已断开,然后再移除
	if (successLength == 0)
	{
		client->setDeadClient("客户端主动关闭连接", DEAD_TYPE::MANUAL_QUIT);
	}
	else
	{
		client->setDeadClient("recv或send返回值小于0", DEAD_TYPE::SERVER_KICK_OUT);
	}
#ifdef WINDOWS
	const int errorCode = WSAGetLastError();
#else
	const int errorCode = errno;
#endif
	if (errorCode == 0)
	{
		return;
	}
	LOG("errno:" + IToS(errorCode));
	if (errorCode == EPIPE)
	{
		LOG("管道损坏错误信号，error : EPIPE");
	}
	else if (errorCode == EAGAIN)
	{
		LOG("重试错误信号，error : EAGAIN");
	}
	else if (errorCode == EBADMSG)
	{
		client->setDeadClient("客户端已经关闭了Socket", DEAD_TYPE::SERVER_KICK_OUT);
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
		for (const auto& item : mPacketSendCountMap)
		{
			temp.emplace(item.first, item.second);
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
		mPacketSendCountMap.clear();
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

	TCPServerClient* client = new TCPServerClient(clientGUID, socket, ip);
	client->init();
	mClientList.add(clientGUID, client);
	if (mOutputLog)
	{
		LOG("IP:" + string(ip) + " 连接服务器! 当前连接数:" + IToS(mClientList.size()));
	}

	// 加入客户端列表,三个列表都要加入
	{
		THREAD_LOCK(mSendLock);
		mSendClientList.add(client);
	}
	{
		THREAD_LOCK(mRecvLock);
		mRecvClientList.add(client);
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
	// 从客户端列表移除,三个列表都要移除
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
	// 复制一份列表出来
	HashMap<int, TCPServerClient*> tempList;
	mClientList.cloneTo(tempList);
	for (const auto& iter : tempList)
	{
		iter.second->setDeadClient("全部退出登录", DEAD_TYPE::SERVER_KICK_OUT);
		CmdNetServerLogoutAccount::execute(iter.second);
	}
}

void TCPServerSystem::encrypt(char* data, const int length, const byte param)
{
	if (data == nullptr || length <= 0)
	{
		return;
	}
	int keyIndex = (param ^ 223) & (FrameDefine::ENCRYPT_KEY_LENGTH - 1);
	FOR(length)
	{
		const byte keyChar = (byte)(FrameDefine::ENCRYPT_KEY[keyIndex] ^ (byte)param);
		data[i] ^= keyChar;
		data[i] += keyChar >> 1;
		data[i] ^= (byte)(((keyChar * keyIndex) & (mKey0 * mKey1)) | ((mKey2 + mKey3) * keyIndex));
		keyIndex += i;
		keyIndex &= FrameDefine::ENCRYPT_KEY_LENGTH - 1;
	}
}

void TCPServerSystem::decrypt(char* data, const int length, const byte param)
{
	if (data == nullptr || length <= 0)
	{
		return;
	}
	int keyIndex = (param ^ 223) & (FrameDefine::ENCRYPT_KEY_LENGTH - 1);
	FOR(length)
	{
		const byte keyChar = (byte)(FrameDefine::ENCRYPT_KEY[keyIndex] ^ (byte)param);
		data[i] ^= (byte)(((keyChar * keyIndex) & (mKey0 * mKey1)) | ((mKey2 + mKey3) * keyIndex));
		data[i] -= keyChar >> 1;
		data[i] ^= keyChar;
		keyIndex += i;
		keyIndex &= FrameDefine::ENCRYPT_KEY_LENGTH - 1;
	}
}