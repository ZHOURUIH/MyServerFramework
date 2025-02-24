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
#ifdef WINDOWS
	WSACleanup();
#endif
	if (mSocket != INVALID_SOCKET)
	{
		CLOSE_SOCKET(mSocket);
		mSocket = INVALID_SOCKET;
	}
	if (mClient != nullptr)
	{
		delete mClient;
		mClient = nullptr;
	}
	if (mPacketDataBuffer != nullptr)
	{
		delete mPacketDataBuffer;
		mPacketDataBuffer = nullptr;
	}
	if (mTempRecvBuffer != nullptr)
	{
		delete[] mTempRecvBuffer;
		mTempRecvBuffer = nullptr;
	}
}

void UDPServerSystem::init()
{
	mPort = (ushort)mFrameConfigSystem->getUDPPort();
	// 未配置udp端口,则不启动udp服务器
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
	mSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (mSocket == INVALID_SOCKET)
	{
		ERROR("socket failed!");
		return;
	}

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
	// 因为可能会出现一直卡在send的情况,所以给send添加一个超时时间
#ifdef WINDOWS
	int timeout = 2000;
	int ret = setsockopt(mSocket, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeout, sizeof(timeout));
#elif defined LINUX
	struct timeval timeout = { 2, 0 };
	const int ret = setsockopt(mSocket, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(struct timeval));
#endif
	if (ret != 0)
	{
		ERROR("设置socket选项失败");
		return;
	}
	mSendThread = mThreadManager->createThread("SendSocketUDP", sendThread, this);
	mReceiveThread = mThreadManager->createThread("ReceiveSocketUDP", receiveThread, this);
	mSendThread->setTime(16, 0);
	mReceiveThread->setTime(16, 0);
}

void UDPServerSystem::sendThread(CustomThread* thread)
{
	const auto* netManager = static_cast<This*>(thread->getArgs());
	// 发送消息
	netManager->mClient->processSend(netManager->mSocket);
}

void UDPServerSystem::receiveThread(CustomThread* thread)
{
	const auto* netManager = static_cast<This*>(thread->getArgs());
	// 接收消息
	sockaddr_in fromAddr;
#ifdef WINDOWS
	int fromLen = sizeof(fromAddr);
#elif defined LINUX
	socklen_t fromLen = sizeof(fromAddr);
#endif
	const int nRecv = recvfrom(netManager->mSocket, netManager->mTempRecvBuffer, FrameDefine::CLIENT_RECV_BUFFER, 0, (sockaddr*)&fromAddr, &fromLen);
	if (nRecv > 0)
	{
		netManager->mClient->recvData(netManager->mTempRecvBuffer, nRecv, fromAddr);
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
	packet->writeToBuffer(mPacketDataBuffer);
}