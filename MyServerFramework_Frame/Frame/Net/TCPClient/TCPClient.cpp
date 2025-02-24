#include "FrameHeader.h"

TCPClient::TCPClient():
	mSendBuffer(new StreamBuffer(FrameDefine::CLIENT_SEND_BUFFER)),
	mRecvBuffer(new StreamBuffer(FrameDefine::CLIENT_RECV_BUFFER)),
	mSendWriter(new SerializerBitWrite()),
	mPacketDataBuffer(new SerializerBitWrite())
{}

TCPClient::~TCPClient() 
{
	disconnect();
	delete mSendBuffer;
	mSendBuffer = nullptr;
	delete mRecvBuffer;
	mRecvBuffer = nullptr;
	delete mSendWriter;
	mSendWriter = nullptr;
	delete mPacketDataBuffer;
	mPacketDataBuffer = nullptr;
}

void TCPClient::disconnect()
{
	// 先停止所有子线程
	mThreadManager->destroyThread(mSendReceiveThread);
	mSendReceiveThread = nullptr;
#ifdef WINDOWS
	WSACleanup();
#endif
	CLOSE_SOCKET(mSocket);
	mSequenceNumber = 0;
	mLastReceiveSequenceNumber = 0;
	mExecutePacketList.clear();
}

void TCPClient::init(const string& ip, const ushort port)
{
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
#endif
	// 创建监听的Socket
	mSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (mSocket == INVALID_SOCKET)
	{
		ERROR("socket failed!");
		return;
	}

	// 设置Socket地址
	SOCKADDR_IN addrServ;
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
		mSocket = INVALID_SOCKET;
		LOG("connect failed!");
		return;
	}
	mCurServerHeartBeatTime = mHeartBeatTimeOut;
	mSendReceiveThread = mThreadManager->createThread("ReceiveSendSocket", receiveSendSocket, this);
	mSendReceiveThread->setTime(100);
}

void TCPClient::receiveSendSocket(CustomThread* thread)
{
	auto* tcpClient = static_cast<This*>(thread->getArgs());
	// 接收消息
	if (!tcpClient->processRecv())
	{
		thread->setRunning(false);
		return;
	}
	// 发送消息
	if (!tcpClient->processSend())
	{
		thread->setRunning(false);
		return;
	}
}

bool TCPClient::processSend()
{
	THREAD_LOCK(mSendLock);
	if (mSendBuffer->getDataLength() == 0)
	{
		return true;
	}
	try
	{
		// select查看后立即返回
		thread_local static timeval tv{ 0, 0 };
		thread_local static fd_set fdwrite;
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
		// 因为send可能无法一次将所有数据都发送出去,所以需要一直检测是否发送完
		while (mSendBuffer->getDataLength() > 0)
		{
			const int thisSendCount = send(mSocket, mSendBuffer->getData(), mSendBuffer->getDataLength(), 0);
			// 检查是否send有错误
			if (thisSendCount > 0)
			{
				mSendBuffer->removeDataFromFront(thisSendCount);
			}
			else
			{
				checkSendRecvError(thisSendCount);
				return false;
			}
		}
	}
	catch (const exception& e)
	{
		LOG("exception : " + string(e.what()));
	}
	return true;
}

bool TCPClient::processRecv()
{
	try
	{
		// select查看后立即返回
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
		// 使用thread_local确保每个线程都有自己的静态变量,否则可能会出现冲突
		thread_local static char recvBuffer[FrameDefine::CLIENT_RECV_BUFFER];
		const int nRecv = recv(mSocket, recvBuffer, FrameDefine::CLIENT_RECV_BUFFER, 0);
		if (nRecv <= 0)
		{
			checkSendRecvError(nRecv);
			return false;
		}
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
			mTempPacketList.push_back(packet);
			// 将已经解析的数据移除
			mRecvBuffer->removeDataFromFront(bitCountToByteCount(bitIndex));
			++mParsedCount;
		}
		// 将解析后的消息列表同步到列表中
		mExecutePacketList.add(mTempPacketList);
	}
	catch (const exception& e)
	{
		LOG("exception : " + string(e.what()));
	}
	return true;
}

void TCPClient::checkSendRecvError(const int successLength) const
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
	int bodySize = 0;
	ushort bodySizeCRC = 0;
	ushort packetType = 0;
	int sequenceNumber = 0;
	const char* bodyBuffer = nullptr;
	ushort readCrc = 0;
	bool useFlag = false;
	ullong fieldFlag = FrameDefine::FULL_FIELD_FLAG;
	SerializerBitRead reader(mRecvBuffer->getData(), mRecvBuffer->getDataLength());
	reader.setBitIndex(bitIndex);
	if (!reader.readSigned(bodySize))
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
	if (!reader.readSigned(sequenceNumber))
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
	if (bodyBuffer != nullptr)
	{
		// bodyBuffer指向的是mRecvBuffer中的buffer,此buffer允许修改,所以此处强转为char*
		TCPServerSystem::decrypt((char*)bodyBuffer, bodySize, FrameDefine::ENCRYPT_KEY, FrameDefine::ENCRYPT_KEY_LENGTH, (byte)((int)packetType + bodySize + (sequenceNumber ^ 123 ^ (int)packetType)));
	}

	// 如果是消息解析数不足,并且收到无效消息时,不会报错
	if (mParsedCount < FrameDefine::MIN_PARSE_COUNT && mPacketTCPFactoryManager->getFactory(packetType) == nullptr)
	{
		reason.setString("无效客户端, 消息类型错误! 消息类型ID:" + IToS((int)packetType));
		return PARSE_RESULT::INVALID_PACKET_TYPE;
	}
	// 创建消息对象,并解析包体数据
	packet = mPacketTCPThreadPool->newClass(packetType);
	if (packet == nullptr)
	{
		reason.setString("消息解析错误! 消息类型ID:" + IToS((int)packetType));
		return PARSE_RESULT::PACKET_PARSE_FAILED;
	}
	packet->setFieldFlag(fieldFlag);
	if (bodyBuffer != nullptr)
	{
		SerializerBitRead reader(bodyBuffer, bodySize);
		if (!packet->readFromBuffer(&reader))
		{
			// 解析错误
			mPacketTCPThreadPool->destroyClass(packet);
			reason.setString("消息解析错误! 消息类型ID:" + IToS((int)packetType));
			return PARSE_RESULT::PACKET_PARSE_FAILED;
		}
	}

	// CRC校验
	if (crcCode != readCrc)
	{
		mPacketTCPThreadPool->destroyClass(packet);
		packet = nullptr;
		reason.setString("crc check error");
		return PARSE_RESULT::CRC_ERROR;
	}

	// 校验序列号是否正确
	packet->setSequenceNumber(sequenceNumber);
	if (sequenceNumber != mLastReceiveSequenceNumber + 1 && mLastReceiveSequenceNumber != 0x7FFFFFFF)
	{
		const string info = "丢包:" + IToS(sequenceNumber - mLastReceiveSequenceNumber - 1) + 
							", 已接收包数量:" + UIToS(mParsedCount) + ", 当前包序列号:" + IToS(sequenceNumber);
		LOG(info);
		mPacketTCPThreadPool->destroyClass(packet);
		packet = nullptr;
		reason.setString("sequence number check error! lastNumber:" + IToS(mLastReceiveSequenceNumber) + 
						", receiveNumber:" + IToS(sequenceNumber));
		return PARSE_RESULT::SEQUENCE_NUMBER_ERROR;
	}
	mLastReceiveSequenceNumber = sequenceNumber;
	bitIndex = reader.getBitIndex();
	return PARSE_RESULT::SUCCESS;
}

void TCPClient::update(const float elapsedTime)
{
	{
		DoubleBufferReadScope<PacketTCP*> readScope(mExecutePacketList);
		// 执行读缓冲区中的所有消息包
		if (readScope.mReadList != nullptr)
		{
			for (PacketTCP* packetReply : *readScope.mReadList)
			{
				packetReply->execute();
				mPacketTCPThreadPool->destroyClass(packetReply);
			}
		}
	}

	// 心跳
	if (tickTimerLoop(mCurServerHeartBeatTime, elapsedTime, mHeartBeatTimeOut))
	{
		heartBeat();
	}
}

void TCPClient::sendPacket(PacketTCP* packet)
{
	checkMainThread();
	mPacketDataBuffer->clear();
	packet->writeToBuffer(mPacketDataBuffer);
	const int bodySize = mPacketDataBuffer->getByteCount();
	const ushort packetType = packet->getType();
	const ullong fieldFlag = packet->getFieldFlag();
	const int sequence = ++mSequenceNumber;
	// 当前程序为客户端时,一个消息只会被发送一次,所以不需要拷贝到另外一个缓冲区,直接加密即可
	TCPServerSystem::encrypt((char*)mPacketDataBuffer->getBuffer(), bodySize, FrameDefine::ENCRYPT_KEY, FrameDefine::ENCRYPT_KEY_LENGTH, (byte)(packetType + bodySize + (sequence ^ 123 ^ packetType)));

	mSendWriter->clear();
	mSendWriter->writeSigned(bodySize);
	// 为包体长度添加一个校验值,避免长度被意外修改,导致包体读取错误
	mSendWriter->writeUnsigned(generateCRC16(bodySize));
	mSendWriter->writeUnsigned(packetType);
	mSendWriter->writeSigned(sequence);
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

	// 放入到缓存列表中,因为要保证发送消息的顺序,所以只能都先放到二级缓冲区
	{
		THREAD_LOCK(mSendLock);
		mSendBuffer->addDataToBack(mSendWriter->getBuffer(), mSendWriter->getByteCount());
	}
}