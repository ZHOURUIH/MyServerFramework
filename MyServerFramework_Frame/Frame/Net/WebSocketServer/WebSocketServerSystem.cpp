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
	// 销毁所有客户端
	for (const auto& iterClient : mClientList)
	{
		delete iterClient.second;
	}
	mClientList.clear();
#ifdef WINDOWS
	WSACleanup();
#endif
	if (mSocket != INVALID_SOCKET)
	{
		CLOSE_SOCKET(mSocket);
		mSocket = INVALID_SOCKET;
	}
	if (mPacketDataBuffer != nullptr)
	{
		delete mPacketDataBuffer;
		mPacketDataBuffer = nullptr;
	}
	if (mRecvBuffer != nullptr)
	{
		delete[] mRecvBuffer;
		mRecvBuffer = nullptr;
	}
}

void WebSocketServerSystem::init()
{
	mPort = (ushort)mFrameConfigSystem->getWebSocketPort();
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
#endif
	//创建监听的Socket
	mSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (mSocket == INVALID_SOCKET)
	{
		ERROR("socket failed!");
		return;
	}

	mMaxSocket = (int)mSocket;

	//设置服务器Socket地址
	sockaddr_in addrServ;
	makeSockAddr(addrServ, INADDR_ANY, mPort);
	//绑定Sockets Server
	if (bind(mSocket, (const struct sockaddr*)&addrServ, sizeof(sockaddr_in)) != 0)
	{
		mSocket = INVALID_SOCKET;
		ERROR("bind failed!");
		// linux下只要绑定端口失败就退出,否则会一直无法连接此服务器
#ifdef LINUX
		exit(0);
#endif
		return;
	}
	//在Sockets Server上进行监听
	if (listen(mSocket, mFrameConfigSystem->getBacklog()) != 0)
	{
		mSocket = INVALID_SOCKET;
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
	int enable = 1;
	ret |= setsockopt(mSocket, SOL_SOCKET, TCP_NODELAY, (const char*)&enable, sizeof(enable));
#elif defined LINUX
	struct timeval timeout = { 2, 0 };
	int enable = 1;
	int ret = setsockopt(mSocket, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(struct timeval));
	ret |= setsockopt(mSocket, SOL_SOCKET, TCP_NODELAY, (char*)&enable, sizeof(enable));
#endif
	if (ret != 0)
	{
		ERROR("设置socket选项失败");
		return;
	}
	mSendThread = mThreadManager->createThread("SendSocketTCP", sendThread, this);
	mSendThread->setTime(10);
	mSendThread->setPrintFPS(true);
	mReceiveThread = mThreadManager->createThread("ReceiveSocketTCP", receiveThread, this);
	mReceiveThread->setTime(10);
	mReceiveThread->setPrintFPS(true);
	mAcceptThread = mThreadManager->createThread("AcceptSocket", acceptThread, this);
}

void WebSocketServerSystem::acceptThread(CustomThread* thread)
{
	auto* netManager = static_cast<This*>(thread->getArgs());
	sockaddr_in addr;
#ifdef WINDOWS
	int nLen = sizeof(addr);
#elif defined LINUX
	socklen_t nLen = sizeof(addr);
#endif
	MY_SOCKET sClient = accept(netManager->mSocket, (struct sockaddr*)&addr, &nLen);
	// 因为设置了2秒超时,所以accept每2秒就会返回一次INVALID_SOCKET
	if (sClient == (MY_SOCKET)INVALID_SOCKET)
	{
		return;
	}
	MyString<16> ip;
	// 获得客户端IP,然后通知已经接收到一个客户端的连接
	if (netManager->mOutputLog)
	{
#ifdef WINDOWS
		SPRINTF(ip.toBuffer(), 16, "%d.%d.%d.%d", addr.sin_addr.S_un.S_un_b.s_b1, addr.sin_addr.S_un.S_un_b.s_b2, addr.sin_addr.S_un.S_un_b.s_b3, addr.sin_addr.S_un.S_un_b.s_b4);
#elif defined LINUX
		int ip0 = getByte(addr.sin_addr.s_addr, 0);
		int ip1 = getByte(addr.sin_addr.s_addr, 1);
		int ip2 = getByte(addr.sin_addr.s_addr, 2);
		int ip3 = getByte(addr.sin_addr.s_addr, 3);
		SPRINTF(ip.toBuffer(), 16, "%d.%d.%d.%d", ip0, ip1, ip2, ip3);
#endif
	}
	int enable = 1;
#ifdef WINDOWS
	int ret = setsockopt(sClient, SOL_SOCKET, TCP_NODELAY, (const char*)&enable, sizeof(enable));
#elif defined LINUX
	int ret = setsockopt(sClient, SOL_SOCKET, TCP_NODELAY, (char*)&enable, sizeof(enable));
#endif
	if (ret != 0)
	{
		ERROR("设置socket选项失败");
		return;
	}
	THREAD_LOCK(netManager->mAcceptLock);
	netManager->mAcceptBuffer.emplace_back(sClient, ip.str());
}

void WebSocketServerSystem::processSend()
{
	if (mSendClientList.size() == 0)
	{
		return;
	}
	THREAD_LOCK(mSendLock);
	try
	{
		thread_local static Array<FD_SETSIZE, WebSocketServerClient*> selectedClient;
		// select查看后立即返回
		thread_local static timeval tv{ 0, 0 };
		thread_local static fd_set fdwrite;
		const int clientCount = mSendClientList.size();
		for (int j = 0; j < clientCount; )
		{
			FD_ZERO(&fdwrite);
			int selectedClientCount = 0;
			// 不能超过最大并发连接数,采用FD_SETSIZE一组的轮询方式
			FOR_I(FD_SETSIZE)
			{
				if (j >= clientCount)
				{
					break;
				}
				WebSocketServerClient* client = mSendClientList[j++];
				if (client->isDeadClient())
				{
					continue;
				}
				// 确认将所有待发送的数据写入到发送缓冲区
				client->writeToSendBuffer();
				// 只检查有数据需要写的客户端
				if (client->getSendBuffer().size() > 0)
				{
					selectedClient[selectedClientCount++] = client;
					FD_SET(client->getSocket(), &fdwrite);
				}
			}
			// select第一个参数在windows中可以为0,但是在其他系统中需要设置
			// 如果有客户端可写
			if (select(mMaxSocket + 1, nullptr, &fdwrite, nullptr, &tv) <= 0)
			{
				continue;
			}
			FOR_I(selectedClientCount)
			{
				WebSocketServerClient* client = selectedClient[i];
				if (!FD_ISSET(client->getSocket(), &fdwrite))
				{
					continue;
				}
				// 因为send可能无法一次将所有数据都发送出去,所以需要一直检测是否发送完
				const auto& sendBuffer = client->getSendBuffer();
				if (sendBuffer.size() > 0)
				{
					for (const auto& item : sendBuffer)
					{
						if (client->isDeadClient())
						{
							break;
						}
						char* data = item.first;
						const int dataLength = item.second.x;
						int flags = 0;
#ifdef LINUX
						flags = MSG_NOSIGNAL;
#endif
						const int thisSendCount = send(client->getSocket(), data, dataLength, flags);
						// 检查是否send有错误
						if (thisSendCount <= 0)
						{
							checkSendRecvError(client, thisSendCount);
						}
					}
				}
				client->clearSendBuffer();
			}
		}
	}
	catch (const exception& e)
	{
		LOG("exception : " + string(e.what()));
	}
}

void WebSocketServerSystem::processRecv()
{
	if (mRecvClientList.size() == 0)
	{
		return;
	}
	THREAD_LOCK(mRecvLock);
	try
	{
		thread_local static Array<FD_SETSIZE, WebSocketServerClient*> selectedClient;
		// select查看后立即返回
		thread_local static timeval tv{ 0, 0 };
		thread_local static fd_set fdread;
		const int clientCount = mRecvClientList.size();
		for (int j = 0; j < clientCount; )
		{
			FD_ZERO(&fdread);
			int selectedClientCount = 0;
			// 不能超过最大并发连接数,采用FD_SETSIZE一组的轮询方式
			FOR_I(FD_SETSIZE)
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
				selectedClient[selectedClientCount++] = client;
				FD_SET(client->getSocket(), &fdread);
			}
			// select第一个参数在windows中可以为0,但是在其他系统中需要设置
			if (select(mMaxSocket + 1, &fdread, nullptr, nullptr, &tv) <= 0)
			{
				continue;
			}
			FOR_I(selectedClientCount)
			{
				WebSocketServerClient* client = selectedClient[i];
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
	catch (const exception& e)
	{
		LOG("exception : " + string(e.what()));
	}
}

void WebSocketServerSystem::checkSendRecvError(WebSocketServerClient* client, const int successLength) const
{
	// 客户端可能已经与服务器断开了连接,先立即标记该客户端已断开,然后再移除
	if (successLength == 0)
	{
		client->setDeadClient("客户端主动关闭连接");
	}
	else
	{
		client->setDeadClient("recv或send返回值小于0");
	}
	const int errorCode = errno;
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
		client->setDeadClient("客户端已经关闭了Socket");
	}
}

void WebSocketServerSystem::update(const float elapsedTime)
{
	if (!isAvailable())
	{
		return;
	}
	// 将接收缓存列表同步到客户端列表,double check避免不必要的等待
	if (mAcceptBuffer.size() > 0)
	{
		THREAD_LOCK(mAcceptLock);
		for (const auto& acceptInfo : mAcceptBuffer)
		{
			notifyAcceptClient(acceptInfo.first, acceptInfo.second);
		}
		mAcceptBuffer.clear();
	}

	// 更新客户端,找出是否有客户端需要断开连接
	thread_local static Vector<pair<int, string>> tempLogoutClientList;
	tempLogoutClientList.clear();
	for (const auto& item : mClientList)
	{
		WebSocketServerClient* client = item.second;
		client->update(elapsedTime);
		// 将已经死亡的客户端放入列表
		if (client->isDeadClient())
		{
			tempLogoutClientList.emplace_back(item.first, client->getDeadReason());
		}
	}

	// 断开死亡客户端,需要等待所有线程的当前帧都执行完毕,否则在此处直接销毁客户端会导致其他线程报错
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
	// 发送消息前需要先写入数据到缓冲区
	// 但是由于一次写入不一定对应一次发送,可能会一次写入多次发送,所以将写入和发送分为两个函数
	// 而如果让外边存储一个缓冲区对象,使用起来又非常麻烦,所以就在WebSocketServerSystem中存储一个公用的缓冲区
	// 并且不能是静态的,静态对象的析构不是在主线程执行,会在检测执行线程时报错
	mPacketDataBuffer->clear();
	packet->writeToBuffer(mPacketDataBuffer);
	mWritePacketBytes += mPacketDataBuffer->getDataSize();
}

int WebSocketServerSystem::notifyAcceptClient(const MY_SOCKET socket, const string& ip)
{
	// 加入主线程的客户端列表
	const int clientGUID = generateSocketGUID();
	if (mClientList.contains(clientGUID))
	{
		CLOSE_SOCKET(socket);
		LOG_NO_PRINT("客户端列表中已经包含了此客户端ID:" + UIToS(clientGUID));
		return 0;
	}

	WebSocketServerClient* client = new WebSocketServerClient(clientGUID, socket, ip);
	client->init();
	mClientList.insert(clientGUID, client);
	if (mOutputLog)
	{
		LOG("IP:" + string(ip) + " 连接服务器! 当前连接数:" + IToS(mClientList.size()));
	}
	mMaxSocket = getMax(mMaxSocket, (int)socket);

	// 加入客户端列表,三个列表都要加入
	{
		THREAD_LOCK(mSendLock);
		mSendClientList.push_back(client);
	}
	{
		THREAD_LOCK(mRecvLock);
		mRecvClientList.push_back(client);
	}
	return clientGUID;
}

void WebSocketServerSystem::disconnectSocket(const int clientGUID, const string& reason)
{
	const auto iterClient = mClientList.find(clientGUID);
	if (iterClient == mClientList.end())
	{
		return;
	}
	WebSocketServerClient* client = iterClient->second;
	// 从客户端列表移除,三个列表都要移除
	{
		THREAD_LOCK(mSendLock);
		mSendClientList.eraseElement(client);
	}
	{
		THREAD_LOCK(mRecvLock);
		mRecvClientList.eraseElement(client);
	}
	// 从主线程的客户端列表中移除
	mClientList.erase(iterClient);
	if (mOutputLog)
	{
		PLAYER_LOG("客户端断开连接:角色ID:" + LLToS(client->getPlayerGUID()) + 
					",原因:" + reason + ", 剩余连接数:" + IToS(getClientCount()), client->getPlayerGUID());
	}
	// 退出账号的登录
	CmdNetServerLogoutAccount::execute(client);
	// 销毁客户端
	delete client;
}

void WebSocketServerSystem::notifyAccountLogin(WebSocketServerClient* client)
{
	mClientAccountIDList.insert(client->getAccountGUID(), client);
}

void WebSocketServerSystem::notifyAccountLogout(WebSocketServerClient* client)
{
	mClientAccountIDList.erase(client->getAccountGUID());
}

void WebSocketServerSystem::logoutAll()
{
	// 复制一份列表出来
	auto tempList = mClientList;
	for (const auto& iter : tempList)
	{
		CmdNetServerLogoutAccount::execute(iter.second);
	}
}

void WebSocketServerSystem::encrypt(char* data, const int length, const byte* key, const int keyLen, const byte param)
{
	if (length == 0)
	{
		return;
	}
	int keyIndex = (param ^ 223) & (keyLen - 1);
	FOR_I(length)
	{
		const byte keyChar = (byte)(key[keyIndex] ^ (byte)param);
		data[i] ^= keyChar;
		data[i] += keyChar >> 1;
		data[i] ^= (byte)(((keyChar * keyIndex) & (mKey0 * mKey1)) | ((mKey2 + mKey3) * keyIndex));
		keyIndex += i;
		if (keyIndex >= keyLen)
		{
			keyIndex &= keyLen - 1;
		}
	}
}

void WebSocketServerSystem::decrypt(char* data, const int length, const byte* key, const int keyLen, const byte param)
{
	if (length <= 0)
	{
		return;
	}
	int keyIndex = (param ^ 223) & (keyLen - 1);
	FOR_I(length)
	{
		const byte keyChar = (byte)(key[keyIndex] ^ (byte)param);
		data[i] ^= (byte)(((keyChar * keyIndex) & (mKey0 * mKey1)) | ((mKey2 + mKey3) * keyIndex));
		data[i] -= keyChar >> 1;
		data[i] ^= keyChar;
		keyIndex += i;
		if (keyIndex >= keyLen)
		{
			keyIndex &= keyLen - 1;
		}
	}
}