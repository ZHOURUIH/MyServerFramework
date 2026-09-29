#include "FrameHeader.h"

UDPServerSystem::UDPServerSystem():
	mClient(new UDPServerClient()),
	mPacketDataBuffer(new SerializerBitWrite()),
	mTempRecvBuffer(new char[FrameDefine::CLIENT_RECV_BUFFER])
{}

void UDPServerSystem::quit()
{
	// 先停止所有子线程
	mThreadManager->destroyThread(mSendThread);
	mThreadManager->destroyThread(mReceiveThread);
	CLOSE_SOCKET(mSocket);
#ifdef WINDOWS
	// 需要在CLOSE_SOCKET后面调用
	if (mWinsockInitialized)
	{
		mWinsockInitialized = false;
		WSACleanup();
	}
#endif
	DELETE(mClient);
	DELETE(mPacketDataBuffer);
	DELETE_ARRAY(mTempRecvBuffer);
}

void UDPServerSystem::init()
{
	if (isAvailable())
	{
		return;
	}

	mPort = (ushort)mFrameConfigSystem->getUDPPort();
	// 未配置UDP端口,则不启动UDP服务器
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

	// 创建UDP Socket
	mSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (mSocket == INVALID_SOCKET)
	{
#ifdef WINDOWS
		const int errorCode = WSAGetLastError();
#else
		const int errorCode = errno;
#endif
		ERROR("UDP socket failed,error:" + IToS(errorCode));
		return;
	}

#ifdef LINUX
	// 服务器重启时允许快速重新绑定端口
	int reuseAddr = 1;
	if (setsockopt(mSocket, SOL_SOCKET, SO_REUSEADDR, (char*)&reuseAddr, sizeof(reuseAddr)) != 0)
	{
		const int errorCode = errno;
		ERROR("UDP设置SO_REUSEADDR失败,error:" + IToS(errorCode));
		CLOSE_SOCKET(mSocket);
		return;
	}
#endif

	// 设置服务器Socket地址
	sockaddr_in addrServ{};
	makeSockAddr(addrServ, INADDR_ANY, mPort);

	// 绑定UDP端口
	if (bind(mSocket, (const struct sockaddr*)&addrServ, sizeof(sockaddr_in)) != 0)
	{
#ifdef WINDOWS
		const int errorCode = WSAGetLastError();
#else
		const int errorCode = errno;
#endif
		ERROR("UDP bind failed,port:" + IToS(mPort) + ",error:" + IToS(errorCode));
		CLOSE_SOCKET(mSocket);
#ifdef LINUX
		exit(EXIT_FAILURE);
#endif
		return;
	}

	// UDP Socket设置为non-blocking
#ifdef WINDOWS
	u_long nonBlocking = 1;
	if (ioctlsocket(mSocket, FIONBIO, &nonBlocking) != 0)
	{
		const int errorCode = WSAGetLastError();
		ERROR("UDP设置non-blocking失败,error:" + IToS(errorCode));
		CLOSE_SOCKET(mSocket);
		return;
	}
#elif defined LINUX
	const int socketFlags = fcntl(mSocket, F_GETFL, 0);
	if (socketFlags < 0 || fcntl(mSocket, F_SETFL, socketFlags | O_NONBLOCK) != 0)
	{
		const int errorCode = errno;
		ERROR("UDP设置non-blocking失败,error:" + IToS(errorCode));
		CLOSE_SOCKET(mSocket);
		return;
	}
#endif
	mSendThread = mThreadManager->createThread("SendSocketUDP", [this]() { sendThread(); });
	mSendThread->setTime(16, 0);
	mReceiveThread = mThreadManager->createThread("ReceiveSocketUDP", [this]() { receiveThread(); });
	mReceiveThread->setTime(16, 0);
}

void UDPServerSystem::sendThread()
{
	// 发送消息
	mClient->processSend(mSocket);
}

void UDPServerSystem::receiveThread()
{
	// 限制单次循环最多处理的UDP包数量
	// 防止极端UDP包洪水导致Receive线程长时间不退出本轮
	constexpr int MAX_RECV_COUNT_PER_LOOP = 256;
	FOR(MAX_RECV_COUNT_PER_LOOP)
	{
		sockaddr_in fromAddr{};
#ifdef WINDOWS
		int fromLen = sizeof(fromAddr);
#elif defined LINUX
		socklen_t fromLen = sizeof(fromAddr);
#endif
		const int nRecv = recvfrom(mSocket, mTempRecvBuffer, FrameDefine::CLIENT_RECV_BUFFER, 0, (sockaddr*)&fromAddr, &fromLen);
		if (nRecv > 0)
		{
			mClient->recvData(mTempRecvBuffer, nRecv, fromAddr);
			continue;
		}
		if (nRecv == 0)
		{
			// UDP允许0字节数据报
			// 当前业务没有意义,直接忽略
			continue;
		}

#ifdef WINDOWS
		const int errorCode = WSAGetLastError();
		if (errorCode == WSAEWOULDBLOCK)
		{
			// 当前UDP接收队列已经读空
			break;
		}
		if (errorCode == WSAEINTR)
		{
			// 被中断,继续尝试
			continue;
		}
#else
		const int errorCode = errno;
		if (errorCode == EAGAIN || errorCode == EWOULDBLOCK)
		{
			// 当前UDP接收队列已经读空
			break;
		}
		if (errorCode == EINTR)
		{
			continue;
		}
#endif
		LOG("UDP recvfrom失败,error:" + IToS(errorCode));
		break;
	}
}

void UDPServerSystem::writePacket(PacketTCP* packet)
{
	checkMainThread();
	// 发送消息前需要先写入数据到缓冲区
	// 但是由于一次写入不一定对应一次发送,可能会一次写入多次发送,所以将写入和发送分为两个函数
	// 而如果让外边存储一个缓冲区对象,使用起来又非常麻烦,所以就在UDPServerSystem中存储一个公用的缓冲区
	// 并且不能是静态的,静态对象的析构不是在主线程执行,会在检测执行线程时报错
	mPacketDataBuffer->clear();
	packet->writeToBuffer(mPacketDataBuffer, packet->hasSign());
}