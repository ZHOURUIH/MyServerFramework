#include "FrameHeader.h"

TCPClient::TCPClient():
	mInternalPacketTCPCSThreadPool(new PacketTCPCSThreadPool()),
	mSendBuffer(new StreamBuffer(FrameDefine::CLIENT_SEND_BUFFER)),
	mWillSendBuffer0(new StreamBuffer(FrameDefine::CLIENT_SEND_BUFFER)),
	mWillSendBuffer1(new StreamBuffer(FrameDefine::CLIENT_SEND_BUFFER)),
	mRecvBuffer(new StreamBuffer(FrameDefine::CLIENT_RECV_BUFFER)),
	mRecvReader(nullptr, 0),
	mSendWriter(new SerializerBitWrite()),
	mPacketDataBuffer(new SerializerBitWrite())
{}

TCPClient::~TCPClient() 
{
	disconnect();
	mInternalPacketTCPCSThreadPool->quit();
	DELETE(mInternalPacketTCPCSThreadPool);
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
	mInternalPacketTCPCSThreadPool->notifyNewClassThreadDestroy();
	mParsedCount = 0;
	mSendBuffer->clear();
	mWillSendBuffer0->clear();
	mWillSendBuffer1->clear();
	mRecvBuffer->clear();
	mTempPacketList.clear();
}

void TCPClient::init(const string& ip, const ushort port)
{
	// 判断TCP连接状态
	if (isAvailable())
	{
		return;
	}
	mInternalPacketTCPCSThreadPool->init();
	mServerIP = ip;
	mServerPort = port;
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
#ifdef WINDOWS
		const int errorCode = WSAGetLastError();
#else
		const int errorCode = errno;
#endif
		LOG(mServerName + "连接失败,IP:" + mServerIP + ",Port:" + IToS(mServerPort) + ",error:" + IToS(errorCode));
		CLOSE_SOCKET(mSocket);
		return;
	}
	LOG(mServerName + "连接成功,IP:" + mServerIP + ",Port:" + IToS(mServerPort));
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
	// 将待发送数据转移到真正的发送缓冲区
	writeToSendBuffer();
	if (mSendBuffer->getDataLength() == 0)
	{
		return true;
	}

	// select查看Socket是否可写
	thread_local static timeval tv{ 0, 0 };
	thread_local static fd_set fdwrite;
	if (needSelect)
	{
		FD_ZERO(&fdwrite);
		FD_SET(mSocket, &fdwrite);
		// Windows第一个参数会被忽略
		// Linux下必须是最大fd + 1
		const int selectResult = select((int)mSocket + 1, nullptr, &fdwrite, nullptr, &tv);
		// 没有可写事件
		if (selectResult == 0)
		{
			return true;
		}
		// select自身发生错误
		if (selectResult < 0)
		{
#ifdef WINDOWS
			const int errorCode = WSAGetLastError();
			// 被信号中断,下次继续
			if (errorCode == WSAEINTR)
			{
				return true;
			}
#else
			const int errorCode = errno;
			// 被信号中断,下次继续
			if (errorCode == EINTR)
			{
				return true;
			}
#endif
			LOG(mServerName + "发送select失败,error:" + IToS(errorCode));
			delayCall([this]() { disconnect(); });
			return false;
		}
		// 理论上select > 0时应该已经在集合中
		if (!FD_ISSET(mSocket, &fdwrite))
		{
			return true;
		}
	}

	// send不一定一次发送完,持续发送直到缓冲区为空
	while (mSendBuffer->getDataLength() > 0)
	{
#ifdef LINUX
		// 防止向已断开的Socket发送数据时触发SIGPIPE
		const int flag = MSG_NOSIGNAL;
#else
		const int flag = 0;
#endif
		const int thisSendCount = send(mSocket, mSendBuffer->getData(), mSendBuffer->getContiguousDataLength(), flag);
		if (thisSendCount > 0)
		{
#ifdef STRESS_TEST
			mTCPServerSystem->mClientSendBytes += thisSendCount;
#endif
			mSendBuffer->removeDataFromFront(thisSendCount);
			continue;
		}
		// checkSendRecvError返回true表示连接真的断了,否则就是EAGAIN/EINTR等临时错误
		// 未断开就暂时不能继续发送
		// 剩余数据仍然保留在mSendBuffer中
		// 下一次线程Update继续发送
		return !checkSendRecvError(thisSendCount);
	}
	return true;
}

bool TCPClient::processRecv(bool needSelect)
{
	// select查看Socket是否可读
	if (needSelect)
	{
		thread_local static timeval tv{ 0, 0 };
		thread_local static fd_set fdread;
		FD_ZERO(&fdread);
		FD_SET(mSocket, &fdread);
		const int selectResult = select((int)mSocket + 1, &fdread, nullptr, nullptr, &tv);
		// 没有数据
		if (selectResult == 0)
		{
			return true;
		}

		// select自身错误
		if (selectResult < 0)
		{
#ifdef WINDOWS
			const int errorCode = WSAGetLastError();
			if (errorCode == WSAEINTR)
			{
				return true;
			}
#else
			const int errorCode = errno;
			if (errorCode == EINTR)
			{
				return true;
			}
#endif

			LOG(mServerName + "接收select失败,error:" + IToS(errorCode));
			delayCall([this](){ disconnect(); });
			return false;
		}
		if (!FD_ISSET(mSocket, &fdread))
		{
			return true;
		}
	}

	// 使用thread_local确保不同TCPClient线程之间不会共用临时接收Buffer
	thread_local static char recvBuffer[FrameDefine::CLIENT_RECV_BUFFER];
	const int nRecv = recv(mSocket, recvBuffer, FrameDefine::CLIENT_RECV_BUFFER, 0);
	if (nRecv <= 0)
	{
		// checkSendRecvError的返回值
		// nRecv == 0时返回true,表示对端正常关闭连接
		// nRecv < 0 + EAGAIN/EINTR时返回false,表示临时错误
		// nRecv < 0 + ECONNRESET时返回true,表示真正断线
		return !checkSendRecvError(nRecv);
	}

#ifdef STRESS_TEST
	mTCPServerSystem->mClientRecvBytes += nRecv;
#endif
	// 添加到TCP接收缓存
	mRecvBuffer->addDataToBack(recvBuffer, nRecv);
	MyString<256> reason;
	// 本次recv解析出来的消息
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
		if (parseResult == PARSE_RESULT::SUCCESS)
		{
			mTempPacketList.add(packet);
			// 删除已经解析完成的数据
			mRecvBuffer->removeDataFromFront(bitCountToByteCount(bitIndex));
			++mParsedCount;
			continue;
		}
		// NOT_ENOUGH时表示TCP半包,保留现有数据等待下一次recv,其他错误也只是等待下一次接受数据
		break;
	}

	// 同步到主线程执行队列
	if (!mTempPacketList.isEmpty())
	{
		mExecutePacketList.add(mTempPacketList);
	}
	return true;
}

bool TCPClient::checkSendRecvError(const int successLength)
{
	// recv返回0明确表示对端正常关闭连接
	// 此时errno / WSAGetLastError没有意义
	if (successLength == 0)
	{
		LOG(mServerName + "服务器主动关闭连接");
		delayCall([this]() {disconnect(); });
		return true;
	}

	// successLength < 0时必须立即读取错误码
#ifdef WINDOWS
	const int errorCode = WSAGetLastError();
	// 临时错误,连接并没有断
	if (errorCode == WSAEINTR || errorCode == WSAEWOULDBLOCK)
	{
		return false;
	}
#else
	const int errorCode = errno;
	// 临时错误,连接并没有断
	if (errorCode == EINTR || errorCode == EAGAIN || errorCode == EWOULDBLOCK)
	{
		return false;
	}
#endif

	LOG(mServerName + "服务器recv或send失败,错误码:" + IToS(errorCode));

#ifdef LINUX
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
		LOG("Socket已断开,error:ENOTCONN");
	}
#endif
	delayCall([this]() { disconnect(); });
	return true;
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
	mRecvReader.reset(mRecvBuffer->getData(), mRecvBuffer->getDataLength());
	mRecvReader.setBitIndex(bitIndex);
	SerializerBitRead& reader = mRecvReader;
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
	const int packetEndBitIndex = reader.getBitIndex();

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
	packet = mInternalPacketTCPCSThreadPool->newClass(packetType);
	if (packet == nullptr)
	{
		reason.set("消息解析错误! 消息类型ID:" + IToS((int)packetType));
		return PARSE_RESULT::PACKET_PARSE_FAILED;
	}
	packet->setFieldFlag(fieldFlag);
	if (bodyBuffer != nullptr)
	{
		reader.reset(bodyBuffer, bodySize);
		if (!packet->readFromBuffer(&reader, hasSign))
		{
			// 解析错误
			// 延迟到主线程销毁消息包
			delayCall([this, packet]() { auto* temp = packet; mInternalPacketTCPCSThreadPool->destroyClass(temp); });
			packet = nullptr;
			reason.set("消息解析错误! 消息类型ID:" + IToS((int)packetType));
			return PARSE_RESULT::PACKET_PARSE_FAILED;
		}
	}

	// CRC校验
	if (crcCode != readCrc)
	{
		// 延迟到主线程销毁消息包
		delayCall([this, packet]() { auto* temp = packet; mInternalPacketTCPCSThreadPool->destroyClass(temp); });
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
		delayCall([this, packet]() { auto* temp = packet; mInternalPacketTCPCSThreadPool->destroyClass(temp); });
		packet = nullptr;
		reason.set("sequence number check error! lastNumber:" + IToS(mLastReceiveSequenceNumber) +
					", receiveNumber:" + IToS(sequenceNumber));
		return PARSE_RESULT::SEQUENCE_NUMBER_ERROR;
	}
	mLastReceiveSequenceNumber = sequenceNumber;
	bitIndex = packetEndBitIndex;
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
					mInternalPacketTCPCSThreadPool->destroyClassList(mWillDestroyPackets);
					mWillDestroyPackets.add(packetReply);
				}
			}
			if (!mWillDestroyPackets.isEmpty())
			{
				mInternalPacketTCPCSThreadPool->destroyClassList(mWillDestroyPackets);
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