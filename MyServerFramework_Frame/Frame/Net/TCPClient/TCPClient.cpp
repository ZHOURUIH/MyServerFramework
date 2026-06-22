#include "FrameHeader.h"

TCPClient::TCPClient():
	mSendBuffer(new StreamBuffer(FrameDefine::CLIENT_SEND_BUFFER)),
	mWillSendBuffer0(new StreamBuffer(FrameDefine::CLIENT_SEND_BUFFER)),
	mWillSendBuffer1(new StreamBuffer(FrameDefine::CLIENT_SEND_BUFFER)),
	mRecvBuffer(new StreamBuffer(FrameDefine::CLIENT_RECV_BUFFER)),
	mSendWriter(new SerializerBitWrite()),
	mPacketDataBuffer(new SerializerBitWrite())
{}

TCPClient::~TCPClient() 
{
	disconnect();
	DELETE(mSendBuffer);
	DELETE(mWillSendBuffer0);
	DELETE(mWillSendBuffer1);
	DELETE(mRecvBuffer);
	DELETE(mSendWriter);
	DELETE(mPacketDataBuffer);
}

void TCPClient::disconnect()
{
	// 先停止所有子线程
#ifndef STRESS_TEST
	mThreadManager->destroyThread(mSendThread);
	mThreadManager->destroyThread(mReceiveThread);
#endif
	CLOSE_SOCKET(mSocket);
#ifdef WINDOWS
	// 需要在CLOSE_SOCKET后面调用
	if (mWinsockInitialized)
	{
		mWinsockInitialized = false;
		WSACleanup();
	}
#endif
	mCurServerHeartBeatTime = -1.0f;
	mSequenceNumber = 0;
	mLastReceiveSequenceNumber = 0;
	mExecutePacketList.clear();
	mWillSendBuffer1Lock.unlock();
	mPacketTCPCSThreadPool->notifyNewClassThreadDestroy();
}

void TCPClient::init(const string& ip, const ushort port)
{
	if (mWinsockInitialized)
	{
		return;
	}
	mServerIP = ip;
	mServerPort = port;
	// 初始化Socket环境
#ifdef WINDOWS
	WSADATA wsd;
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		ERROR("WSAStartup failed!");
		return;
	}
	mWinsockInitialized = true;
#endif
	// 创建监听的Socket
	mSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (mSocket == INVALID_SOCKET)
	{
		ERROR("socket failed!");
		return;
	}

	// 设置Socket地址
	SOCKADDR_IN addrServ{};
	addrServ.sin_family = AF_INET;
	addrServ.sin_port = htons(mServerPort);
#ifdef WINDOWS
	addrServ.sin_addr.S_un.S_addr = inet_addr(mServerIP.c_str());
#elif defined LINUX
	addrServ.sin_addr.s_addr = inet_addr(mServerIP.c_str());
#endif
	// 连接服务器
	if (connect(mSocket, (const struct sockaddr*)&addrServ, sizeof(SOCKADDR_IN)) != 0)
	{
		CLOSE_SOCKET(mSocket);
		LOG("connect failed!");
		return;
	}
	mWinsockInitialized = true;
	mCurServerHeartBeatTime = mHeartBeatTimeOut;
#ifndef STRESS_TEST
	mSendThread = mThreadManager->createThread("TCPClient_SendSocket", [this] { sendThread(); });
	mSendThread->setTime(100);
	mReceiveThread = mThreadManager->createThread("TCPClient_ReceiveSocket", [this] { recvThread(); });
	mReceiveThread->setTime(100);
#endif
}

void TCPClient::sendThread()
{
	// 真实情况下还是需要自己select,如果是模拟压力测试,则在外部select
#ifndef STRESS_TEST
	bool needSelect = true;
#else
	bool needSelect = false;
#endif
	// 发送消息
	if (!processSend(needSelect))
	{
#ifndef STRESS_TEST
		mSendThread->setRunning(false);
#endif
	}
}

void TCPClient::recvThread()
{
	// 真实情况下还是需要自己select,如果是模拟压力测试,则在外部select
#ifndef STRESS_TEST
	bool needSelect = true;
#else
	bool needSelect = false;
#endif
	// 接收消息
	if (!processRecv(needSelect))
	{
#ifndef STRESS_TEST
		mReceiveThread->setRunning(false);
#endif
	}
}

bool TCPClient::processSend(bool needSelect)
{
	writeToSendBuffer();
	if (mSendBuffer->getDataLength() == 0)
	{
		return true;
	}
	// select查看后立即返回
	thread_local static timeval tv{ 0, 0 };
	thread_local static fd_set fdwrite;
	if (needSelect)
	{
		FD_ZERO(&fdwrite);
		FD_SET(mSocket, &fdwrite);
		// select第一个参数在windows中可以为0,但是在其他系统中需要设置
		// 如果有客户端可写
		if (select((int)mSocket + 1, nullptr, &fdwrite, nullptr, &tv) <= 0)
		{
			return true;
		}
		if (!FD_ISSET(mSocket, &fdwrite))
		{
			return true;
		}
	}
	// 因为send可能无法一次将所有数据都发送出去,所以需要一直检测是否发送完
	while (mSendBuffer->getDataLength() > 0)
	{
		const int thisSendCount = send(mSocket, mSendBuffer->getData(), mSendBuffer->getContiguousDataLength(), 0);
		// 检查是否send有错误
		if (thisSendCount > 0)
		{
#ifdef STRESS_TEST
			mTCPServerSystem->mClientSendBytes += thisSendCount;
#endif
			mSendBuffer->removeDataFromFront(thisSendCount);
		}
		else
		{
			checkSendRecvError(thisSendCount);
			return false;
		}
	}
	return true;
}

bool TCPClient::processRecv(bool needSelect)
{
	// select查看后立即返回
	if (needSelect)
	{
		thread_local static timeval tv{ 0, 0 };
		thread_local static fd_set fdread;
		FD_ZERO(&fdread);
		FD_SET(mSocket, &fdread);
		// select第一个参数在windows中可以为0,但是在其他系统中需要设置
		if (select((int)mSocket + 1, &fdread, nullptr, nullptr, &tv) <= 0)
		{
			return true;
		}
		if (!FD_ISSET(mSocket, &fdread))
		{
			return true;
		}
	}
	// 使用thread_local确保每个线程都有自己的静态变量,否则可能会出现冲突
	thread_local static char recvBuffer[FrameDefine::CLIENT_RECV_BUFFER];
	const int nRecv = recv(mSocket, recvBuffer, FrameDefine::CLIENT_RECV_BUFFER, 0);
	if (nRecv <= 0)
	{
		checkSendRecvError(nRecv);
		return false;
	}
#ifdef STRESS_TEST
	mTCPServerSystem->mClientRecvBytes += nRecv;
#endif
	mRecvBuffer->addDataToBack(recvBuffer, nRecv);

	MyString<256> reason;
	// 解析接收到的数据
	mTempPacketList.clear();
	for (;;)
	{
		if (mRecvBuffer->getDataLength() == 0)
		{
			break;
		}
		PacketTCP* packet = nullptr;
		int bitIndex = 0;
		const PARSE_RESULT parseResult = packetRead(bitIndex, packet, reason);
		// 未接收完全或者解析失败,继续等待接收
		if (parseResult != PARSE_RESULT::SUCCESS)
		{
			break;
		}
		mTempPacketList.add(packet);
		// 将已经解析的数据移除
		mRecvBuffer->removeDataFromFront(bitCountToByteCount(bitIndex));
		++mParsedCount;
	}
	// 将解析后的消息列表同步到列表中
	mExecutePacketList.add(mTempPacketList);
	return true;
}

void TCPClient::checkSendRecvError(const int successLength)
{
	// 客户端可能已经与服务器断开了连接,先立即标记该客户端已断开,然后再移除
	if (successLength == 0)
	{
		LOG(mServerName + "服务器主动关闭连接");
	}
	else
	{
		LOG(mServerName + "服务器recv或send返回值小于0");
	}
	delayCall([this]() { disconnect(); });
	
	const int errorCode = errno;
	if (errorCode == 0)
	{
		return;
	}
	LOG("错误码:" + IToS(errorCode));
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
		LOG("对端关闭了socket");
	}
}

PARSE_RESULT TCPClient::packetRead(int& bitIndex, PacketTCP*& packet, MyString<256>& reason)
{
	packet = nullptr;
	uint bodySize = 0;
	ushort bodySizeCRC = 0;
	ushort packetType = 0;
	uint sequenceNumber = 0;
	const char* bodyBuffer = nullptr;
	ushort readCrc = 0;
	bool hasSign = false;
	bool useFlag = false;
	ullong fieldFlag = FrameDefine::FULL_FIELD_FLAG;
	mRecvBuffer->linearize();
	SerializerBitRead reader(mRecvBuffer->getData(), mRecvBuffer->getDataLength());
	reader.setBitIndex(bitIndex);
	if (!reader.readUnsigned(bodySize))
	{
		return PARSE_RESULT::NOT_ENOUGH;
	}
	if (!reader.readUnsigned(bodySizeCRC))
	{
		return PARSE_RESULT::NOT_ENOUGH;
	}
	if (generateCRC16(bodySize) != bodySizeCRC)
	{
		return PARSE_RESULT::BODY_SIZE_CRC_ERROR;
	}
	if (!reader.readUnsigned(packetType))
	{
		return PARSE_RESULT::NOT_ENOUGH;
	}
	if (!reader.readUnsigned(sequenceNumber))
	{
		return PARSE_RESULT::NOT_ENOUGH;
	}
	if (!reader.readBool(hasSign))
	{
		return PARSE_RESULT::NOT_ENOUGH;
	}
	if (!reader.readBool(useFlag))
	{
		return PARSE_RESULT::NOT_ENOUGH;
	}
	if (useFlag && !reader.readUnsigned(fieldFlag))
	{
		return PARSE_RESULT::NOT_ENOUGH;
	}
	// 这里出现错误的原因不一定是未接收完,也可能是bodySize是一个非常大的值,导致无法读取完
	if (bodySize > 0 && !reader.readBufferNoCopy(bodyBuffer, bodySize))
	{
		return PARSE_RESULT::NOT_ENOUGH;
	}
	reader.skipToByteEnd();
	// 在解析包体前就计算CRC16
	const ushort crcCode = generateCRC16(reader.getBuffer(), reader.getReadByteCount());
	if (!reader.readUnsigned(readCrc))
	{
		return PARSE_RESULT::NOT_ENOUGH;
	}

	// 解密包体数据
	// bodyBuffer指向的是mRecvBuffer中的buffer,此buffer允许修改,所以此处强转为char*
	TCPServerSystem::decrypt((char*)bodyBuffer, bodySize, (byte)((int)packetType + bodySize + (sequenceNumber ^ 123 ^ (int)packetType)));

	// 如果是消息解析数不足,并且收到无效消息时,不会报错
	if (mParsedCount < FrameDefine::MIN_PARSE_COUNT && mPacketTCPFactoryManager->getFactory(packetType) == nullptr)
	{
		reason.set("无效客户端, 消息类型错误! 消息类型ID:" + IToS((int)packetType));
		return PARSE_RESULT::INVALID_PACKET_TYPE;
	}
	// 创建消息对象,并解析包体数据
	packet = mPacketTCPCSThreadPool->newClass(packetType);
	if (packet == nullptr)
	{
		reason.set("消息解析错误! 消息类型ID:" + IToS((int)packetType));
		return PARSE_RESULT::PACKET_PARSE_FAILED;
	}
	packet->setFieldFlag(fieldFlag);
	if (bodyBuffer != nullptr)
	{
		SerializerBitRead reader(bodyBuffer, bodySize);
		if (!packet->readFromBuffer(&reader, hasSign))
		{
			// 解析错误
			// 延迟到主线程销毁消息包
			delayCall([packet]() { auto* temp = packet; mPacketTCPCSThreadPool->destroyClass(temp); });
			packet = nullptr;
			reason.set("消息解析错误! 消息类型ID:" + IToS((int)packetType));
			return PARSE_RESULT::PACKET_PARSE_FAILED;
		}
	}

	// CRC校验
	if (crcCode != readCrc)
	{
		// 延迟到主线程销毁消息包
		delayCall([packet]() { auto* temp = packet; mPacketTCPCSThreadPool->destroyClass(temp); });
		packet = nullptr;
		reason.set("crc check error");
		return PARSE_RESULT::CRC_ERROR;
	}

	// 校验序列号是否正确
	packet->setSequenceNumber(sequenceNumber);
	if (sequenceNumber != mLastReceiveSequenceNumber + 1 && mLastReceiveSequenceNumber != 0xFFFFFFFF)
	{
		string info = "丢包:" + IToS(sequenceNumber - mLastReceiveSequenceNumber - 1) + 
							", 已接收包数量:" + UIToS(mParsedCount) + ", 当前包序列号:" + IToS(sequenceNumber);
		LOG(info);
		// 延迟到主线程销毁消息包
		delayCall([packet]() { auto* temp = packet; mPacketTCPCSThreadPool->destroyClass(temp); });
		packet = nullptr;
		reason.set("sequence number check error! lastNumber:" + IToS(mLastReceiveSequenceNumber) +
					", receiveNumber:" + IToS(sequenceNumber));
		return PARSE_RESULT::SEQUENCE_NUMBER_ERROR;
	}
	mLastReceiveSequenceNumber = sequenceNumber;
	bitIndex = reader.getBitIndex();
#ifdef STRESS_TEST
	++mTCPServerSystem->mClientRecvPacket;
#endif
	return PARSE_RESULT::SUCCESS;
}

void TCPClient::update(const float elapsedTime)
{
	// 从mWillSendBuffer0中转到mWillSendBuffer1
	if (mWillSendBuffer0->getDataLength() > 0)
	{
		{
			THREAD_LOCK(mWillSendBuffer1Lock);
			mWillSendBuffer1->mergeFrom(mWillSendBuffer0);
		}
		mWillSendBuffer0->clear();
	}
	
	{
		DoubleBufferReadScope<PacketTCP*> readScope(mExecutePacketList);
		// 执行读缓冲区中的所有消息包
		if (readScope.mReadList != nullptr)
		{
			mWillDestroyPackets.clear();
			for (PacketTCP* packetReply : *readScope.mReadList)
			{
				packetReply->execute();
				if (!mWillDestroyPackets.add(packetReply))
				{
					mPacketTCPCSThreadPool->destroyClassList(mWillDestroyPackets);
					mWillDestroyPackets.add(packetReply);
				}
			}
			if (!mWillDestroyPackets.isEmpty())
			{
				mPacketTCPCSThreadPool->destroyClassList(mWillDestroyPackets);
			}
		}
	}

	// 心跳
	if (tickTimerLoop(mCurServerHeartBeatTime, elapsedTime, mHeartBeatTimeOut))
	{
		heartBeat();
	}
}

void TCPClient::writeToSendBuffer()
{
	// 从mWillSendBuffer1中转到mSendBuffer
	if (mWillSendBuffer1->getDataLength() > 0)
	{
		THREAD_LOCK(mWillSendBuffer1Lock);
		mSendBuffer->mergeFrom(mWillSendBuffer1);
		mWillSendBuffer1->clear();
	}
}

void TCPClient::sendPacket(PacketTCP* packet)
{
	checkMainThread();
	mPacketDataBuffer->clear();
	packet->writeToBuffer(mPacketDataBuffer, packet->hasSign());
	const uint bodySize = mPacketDataBuffer->getByteCount();
	const ushort packetType = packet->getType();
	const ullong fieldFlag = packet->getFieldFlag();
	const uint sequence = ++mSequenceNumber;
	// 当前程序为客户端时,一个消息只会被发送一次,所以不需要拷贝到另外一个缓冲区,直接加密即可
	TCPServerSystem::encrypt((char*)mPacketDataBuffer->getBuffer(), bodySize, (byte)(packetType + bodySize + (sequence ^ 123 ^ packetType)));

	mSendWriter->clear();
	mSendWriter->writeUnsigned(bodySize);
	// 为包体长度添加一个校验值,避免长度被意外修改,导致包体读取错误
	mSendWriter->writeUnsigned(generateCRC16(bodySize));
	mSendWriter->writeUnsigned(packetType);
	mSendWriter->writeUnsigned(sequence);
	mSendWriter->writeBool(packet->hasSign());
	mSendWriter->writeBool(fieldFlag != FrameDefine::FULL_FIELD_FLAG);
	if (fieldFlag != FrameDefine::FULL_FIELD_FLAG)
	{
		mSendWriter->writeUnsigned(fieldFlag);
	}
	mSendWriter->writeBuffer(mPacketDataBuffer->getBuffer(), bodySize);
	// 下面要计算crc,需要完整的字节信息,所以将剩下的位都填充为0
	mSendWriter->fillZeroToByteEnd();
	// 添加CRC校验码,要连包头一起校验
	mSendWriter->writeUnsigned(generateCRC16(mSendWriter->getBuffer(), mSendWriter->getByteCount()));

	// 放入到缓存列表中,因为要减少加锁的次数,所以不会直接放入mSendBuffer中
	mWillSendBuffer0->addDataToBack(mSendWriter->getBuffer(), mSendWriter->getByteCount());
#ifdef STRESS_TEST
	++mTCPServerSystem->mClientSendPacket;
#endif
}