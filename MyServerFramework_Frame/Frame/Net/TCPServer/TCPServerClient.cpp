#include "FrameHeader.h"

TCPServerClient::TCPServerClient(const int clientGUID, const MY_SOCKET s, const string& ip) :
	mIP(ip),
	mSendBuffer(new StreamBuffer(FrameDefine::CLIENT_SEND_BUFFER)),
	mRecvBuffer(new StreamBuffer(FrameDefine::CLIENT_RECV_BUFFER)),
	mRecvReader(nullptr, 0),
	mSendWriter(new SerializerBitWrite(false)),
	mPacketTempBuffer1(new StreamBuffer(FrameDefine::CLIENT_SEND_BUFFER)),
	mPacketTempBuffer2(new StreamBuffer(FrameDefine::CLIENT_SEND_BUFFER)),
	mSocket(s),
	mClientGUID(clientGUID),
	mIsDeadClient(false)
{
	memset(&mUDPAddress, 0, sizeof(mUDPAddress));
	// 构造就设置一个最大容量,避免后续还会在子线程中扩容的情况
	mSendWriter->initCapacity(FrameDefine::CLIENT_MAX_PACKET_SIZE);
}

void TCPServerClient::init()
{
	mHeartBeatTime = mTCPServerSystem->getHeartBeatTimeOut();
}

TCPServerClient::~TCPServerClient()
{
	destroyAllPacket();
	DELETE(mSendBuffer);
	DELETE(mRecvBuffer);
	DELETE(mSendWriter);
	DELETE(mPacketTempBuffer1);
	DELETE(mPacketTempBuffer2);
	// 关闭客户端套接字,并从列表移除
	CLOSE_SOCKET(mSocket);
	mSequenceNumber = 0;
}

void TCPServerClient::destroyAllPacket()
{
	DoubleBufferReadScope<PacketTCP*> readScope(mExecutePacketList);
	if (readScope.mReadList != nullptr && !readScope.mReadList->isEmpty())
	{
		mPacketTCPSCThreadPool->destroyClassList(*readScope.mReadList);
	}
}

void TCPServerClient::update(const float elapsedTime)
{
	if (mIsDeadClient)
	{
		destroyAllPacket();
		return;
	}


	// 执行所有消息
	executeAllPacket();
	mConnectTime += elapsedTime;
	if (mServerPingWaitTime >= 0.0f)
	{
		mServerPingWaitTime += elapsedTime;
	}
	if (tickTimerLoop(mCurServerPingTime, elapsedTime, mServerPingTime))
	{
		// 每隔固定时间向客户端发送一次主动ping,检查客户端的延迟情况
		CALL(mTCPServerSystem->getServerCheckPingCallback(), this, ++mServerPingIndex);
		// 如果没有正在等待,则开始计时等待
		// 正在等待中,说明已经累计多次未回复,则不改变当前等待时间,继续等待
		if (mServerPingWaitTime < 0.0f)
		{
			mServerPingWaitTime = 0.0f;
		}
	}
	if (tickTimerOnce(mHeartBeatTime, elapsedTime))
	{
		setDeadClient("客户端心跳超时", DEAD_TYPE::SERVER_KICK_OUT);
	}
}


bool TCPServerClient::submitSendData()
{
	// Buffer1和延迟队列都只在主线程访问。
	// Buffer2由主线程和Send线程共同访问,每个Client每帧最多在这里锁一次。
	if (mIsDeadClient || (mPacketTempBuffer1->isEmpty() && mPendingSendQueue.empty()))
	{
		return false;
	}

	int moveCount = 0;
	{
		THREAD_LOCK(mPacketTempBuffer2Lock);

		// 必须先搬Buffer1。Buffer1中的数据一定早于PendingQueue中的数据。
		moveCount += mPacketTempBuffer1->moveTo(mPacketTempBuffer2);

		// 只有Buffer1已经完全提交后才能继续提交PendingQueue,否则会打乱TCP字节流顺序。
		if (mPacketTempBuffer1->isEmpty() && !mPendingSendQueue.empty())
		{
			moveCount += flushPendingSendData(mPacketTempBuffer2);
		}
	}

	// Buffer2暂时没有空间不是异常。没有搬动时保留原数据,下一帧继续提交。
	return moveCount > 0;
}

void TCPServerClient::sendPacket(PacketTCP* packet)
{
	checkMainThread();
	// 检查客户端是否还连接
	if (mIsDeadClient)
	{
		return;
	}
	const ushort packetType = packet->getType();
	mTCPServerSystem->increaseSendPacketCount(packetType, packet->getPacketName());
	// 保存发送数据
	const SerializerBitWrite* packetDataBuffer = mTCPServerSystem->getPacketDataBuffer();
	const int bodySize = packetDataBuffer->getByteCount();
	const int sendSize = sizeof(byte) + sizeof(int) + FrameDefine::PACKET_TYPE_SIZE + bodySize + FrameDefine::PACKET_CRC_SIZE;
	// 判断临时缓冲区大小是否足够
	if (sendSize > FrameDefine::CLIENT_MAX_PACKET_SIZE)
	{
		debugError("临时缓冲区太小! send size : " + IToS(sendSize) + ", temp buffer size : " + IToS(FrameDefine::CLIENT_MAX_PACKET_SIZE));
		return;
	}
	writeToSendBuffer1(packetType, packet->hasSign(), packet->getFieldFlag(), bodySize, packetDataBuffer->getBuffer());

#if !defined(VIRTUAL_CLIENT_TEST) && !defined(STRESS_TEST)
	if (mTCPServerSystem->getOutputLog() && packet->showInfo())
	{
		MyString<1024> packetInfo;
		packet->debugInfo(packetInfo);
		INT_STR(packetTypeStr, packetType);
		INT_STR(packetSizeStr, bodySize);
		MyString<2048> allInfo;
		allInfo.add("已发送: ", packetTypeStr.str(), ", ", packetInfo.str(), ", 字节数:", packetSizeStr.str());
		debugInfo(allInfo.str());
	}
#endif
}

// 发送的消息只有消息类型,没有消息参数
void TCPServerClient::sendPacket(const ushort packetType, const string& name)
{
	checkMainThread();
	mTCPServerSystem->increaseSendPacketCount(packetType, name);
	// 检查客户端是否还连接
	if (mIsDeadClient)
	{
		return;
	}
	// 保存发送数据
	constexpr int sendSize = sizeof(byte) + sizeof(int) + FrameDefine::PACKET_TYPE_SIZE + FrameDefine::PACKET_CRC_SIZE;
	// 判断临时缓冲区大小是否足够
	if (sendSize > FrameDefine::CLIENT_MAX_PACKET_SIZE)
	{
		debugError("临时缓冲区太小! send size : " + IToS(sendSize) + ", temp buffer size : " + IToS(FrameDefine::CLIENT_MAX_PACKET_SIZE));
		return;
	}

	writeToSendBuffer1(packetType, false, FrameDefine::FULL_FIELD_FLAG, 0, nullptr);
}

// 将消息数据写入到mPacketTempBuffer1
void TCPServerClient::writeToSendBuffer1(const ushort packetType, bool hasSign, ullong fieldFlag, uint bodySize, const char* bodyBuffer)
{
	const uint sequence = ++mSequenceNumber;
	const byte encryptParam = (byte)(packetType + bodySize + (sequence ^ 123 ^ (int)packetType));
	mSendWriter->clear();
	// 帧头最多约24字节,一次性保证容量后连续写入,避免6~7次Serializer::writeCheck。
	mSendWriter->reserveWrite(32);
	mSendWriter->writeUnsignedReserved(bodySize);
	mSendWriter->writeUnsignedReserved(generateCRC16(bodySize));
	mSendWriter->writeUnsignedReserved(packetType);
	mSendWriter->writeUnsignedReserved(sequence);
	mSendWriter->writeBoolReserved(hasSign);
	mSendWriter->writeBoolReserved(fieldFlag != FrameDefine::FULL_FIELD_FLAG);
	if (fieldFlag != FrameDefine::FULL_FIELD_FLAG)
	{
		mSendWriter->writeUnsignedReserved(fieldFlag);
	}
	if (bodySize > 0)
	{
		// writeBuffer会先将位偏移补齐到字节边界,所以这里提前计算出的就是包体起始字节。
		// 先复制到每个客户端自己的mSendWriter,再原地加密这份私有副本,避免修改TCPServerSystem共享的序列化包体。
		const int bodyOffset = bitCountToByteCount(mSendWriter->getBitCount());
		mSendWriter->writeBuffer(bodyBuffer, bodySize);
		TCPServerSystem::encrypt(mSendWriter->getMutableBuffer() + bodyOffset, bodySize, encryptParam);
	}
	// 下面要计算crc,需要完整的字节信息,所以将剩下的位都填充为0
	mSendWriter->fillZeroToByteEnd();
	// 添加CRC校验码,因为要连包头一起校验,所以只能在这里写
	mSendWriter->writeUnsigned(generateCRC16(mSendWriter->getBuffer(), mSendWriter->getByteCount()));

	const char* sendData = mSendWriter->getBuffer();
	const int sendDataCount = mSendWriter->getByteCount();

	// 正常路径仍然直接写Buffer1,不增加锁也不增加动态分配。
	// 一旦已经有延迟数据,后续消息必须继续进入PendingQueue,否则新消息可能越过旧消息导致协议顺序错乱。
	if (mPendingSendQueue.empty() && mPacketTempBuffer1->isAvailable(sendDataCount))
	{
		mPacketTempBuffer1->addDataToBack(sendData, sendDataCount);
		return;
	}

	addPendingSendData(sendData, sendDataCount);
}

bool TCPServerClient::addPendingSendData(const char* data, const int dataCount)
{
	if (data == nullptr || dataCount <= 0 || mIsDeadClient)
	{
		return false;
	}

	// 延迟队列是突发流量保护,不能无限增长。达到上限说明该客户端长期无法消费服务器下行数据。
	if (dataCount > FrameDefine::CLIENT_MAX_PENDING_SEND_SIZE ||
		mPendingSendBytes > FrameDefine::CLIENT_MAX_PENDING_SEND_SIZE - dataCount)
	{
		debugError("客户端待发送队列超过上限,当前积压:" + IToS(mPendingSendBytes) +
			",新增:" + IToS(dataCount) +
			",上限:" + IToS(FrameDefine::CLIENT_MAX_PENDING_SEND_SIZE));
		setDeadClient("客户端待发送队列超过上限", DEAD_TYPE::SERVER_KICK_OUT);
		return false;
	}

	mPendingSendQueue.emplace_back(data, data + dataCount);
	mPendingSendBytes += dataCount;
	return true;
}

int TCPServerClient::flushPendingSendData(StreamBuffer* target)
{
	if (target == nullptr || mPendingSendQueue.empty() || target->getFreeSize() <= 0)
	{
		return 0;
	}

	int totalWriteCount = 0;
	while (!mPendingSendQueue.empty() && target->getFreeSize() > 0)
	{
		std::vector<char>& frontData = mPendingSendQueue.front();
		const int dataSize = (int)frontData.size();
		const int remainCount = dataSize - mPendingSendFrontOffset;
		if (remainCount <= 0)
		{
			mPendingSendQueue.pop_front();
			mPendingSendFrontOffset = 0;
			continue;
		}

		const int writeCount = getMin(remainCount, target->getFreeSize());
		if (writeCount <= 0 ||
			!target->addDataToBack(frontData.data() + mPendingSendFrontOffset, writeCount))
		{
			break;
		}

		mPendingSendFrontOffset += writeCount;
		mPendingSendBytes -= writeCount;
		totalWriteCount += writeCount;

		if (mPendingSendFrontOffset >= dataSize)
		{
			mPendingSendQueue.pop_front();
			mPendingSendFrontOffset = 0;
		}
	}

	return totalWriteCount;
}

// 此函数在sendThread调用,将数据从mPacketTempBuffer2中转到mSendBuffer
void TCPServerClient::writeToSendBuffer()
{
	if (mPacketTempBuffer2->getDataLength() == 0)
	{
		return;
	}
	// Buffer2由主线程endFrame和Send线程共同访问,长度检查也必须在锁内。
	THREAD_LOCK(mPacketTempBuffer2Lock);
	if (mPacketTempBuffer2->getDataLength() == 0)
	{
		return;
	}

	// SendBuffer空间不足不是异常,能搬多少搬多少。剩余数据继续保留在Buffer2中等待下一轮发送。
	mPacketTempBuffer2->moveTo(mSendBuffer);
}

void TCPServerClient::recvData(const char* data, const int dataCount)
{
	if (!mRecvBuffer->addDataToBack(data, dataCount))
	{
		LOG("客户端接收缓冲区已满,IP:" + mIP + ",角色ID:" + LLToS(mPlayerGUID) + ",接收字节数:" + IToS(dataCount));
		setDeadClient("客户端接收缓冲区已满", DEAD_TYPE::SERVER_KICK_OUT);
		return;
	}
	if (mRecvBuffer->getDataLength() == 0)
	{
		return;
	}

	// 解析接收到的数据
	mTempPacketList.clear();
	PARSE_RESULT ret = PARSE_RESULT::SUCCESS;
	string reason;
	int parsedCount = 0;
	for (;;)
	{
		if (mRecvBuffer->getDataLength() == 0)
		{
			break;
		}
		PacketTCP* packet = nullptr;
		int bitIndex = 0;
		reason.clear();
		const PARSE_RESULT parseResult = packetRead(bitIndex, packet, reason);
		// 未接收完全,继续等待接收
		if (parseResult == PARSE_RESULT::NOT_ENOUGH)
		{
			break;
		}

		// 数据未接收完全
		if (parseResult == PARSE_RESULT::SUCCESS)
		{
			mTempPacketList.add(packet);
			// 将已经解析的数据移除
			mRecvBuffer->removeDataFromFront(bitCountToByteCount(bitIndex));
			++parsedCount;
			++mParsedCount;
		}
		// 解析出现错误
		else
		{
			if (mPlayerGUID > 0)
			{
				if (parseResult == PARSE_RESULT::SEQUENCE_NUMBER_ERROR)
				{
					PLAYER_LOG("消息序列号错误", mPlayerGUID);
					// 接收到序列号错误的消息时封号10分钟
					UNIFIED_UTF8(reason, 128, "消息序列号错误");
					mTCPServerSystem->freezeAccount(mAccountGUID, 60 * 10, reason.str());
				}
				else if (parseResult == PARSE_RESULT::CRC_ERROR)
				{
					PLAYER_LOG("消息CRC校验错误", mPlayerGUID);
					// 接收到CRC校验错误的消息时封号10分钟
					UNIFIED_UTF8(reason, 128, "消息CRC校验错误");
					mTCPServerSystem->freezeAccount(mAccountGUID, 60 * 10, reason.str());
				}
			}
			// 已成功解析的消息包太少,则认为是无效客户端
			if (mParsedCount < FrameDefine::MIN_PARSE_COUNT)
			{
				debugInfo("无效客户端!" + reason);
			}
			else
			{
				debugInfo("消息解析错误!" + reason);
			}
			ret = parseResult;
			break;
		}
	}
	// 如果消息解析失败则将该客户端断开
	if (ret != PARSE_RESULT::SUCCESS)
	{
		// 先设置为断开连接的客户端
		setDeadClient("消息解析错误:" + IToS((int)ret), DEAD_TYPE::SERVER_KICK_OUT);
	}
	// 将解析后的消息列表同步到列表中
	mExecutePacketList.add(mTempPacketList);
	if (parsedCount > 0)
	{
		mTCPServerSystem->increaseReceivePacketCount(parsedCount);
	}
}

PARSE_RESULT TCPServerClient::packetRead(int& bitIndex, PacketTCP*& packet, string& reason)
{
	mRecvBuffer->linearize();
	mRecvReader.reset(mRecvBuffer->getData(), mRecvBuffer->getDataLength());
	mRecvReader.setBitIndex(bitIndex);
	SerializerBitRead& reader = mRecvReader;
	packet = nullptr;
	uint bodySize = 0;
	ushort bodySizeCRC = 0;
	ushort packetType = 0;
	uint sequenceNumber = 0;
	bool hasSign = false;
	bool useFlag = false;
	ullong fieldFlag = FrameDefine::FULL_FIELD_FLAG;
	const char* bodyBuffer = nullptr;
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
	// 在解析包体和读CRC前就计算CRC16
	const ushort crcCode = generateCRC16(reader.getBuffer(), reader.getReadByteCount());

	ushort readCrc = 0;
	if (!reader.readUnsigned(readCrc))
	{
		return PARSE_RESULT::NOT_ENOUGH;
	}
	// CRC校验
	if (crcCode != readCrc)
	{
		reason = "crc check error";
		return PARSE_RESULT::CRC_ERROR;
	}
	const int packetEndBitIndex = reader.getBitIndex();

	// 解密包体数据
	// bodyBuffer指向的是mRecvBuffer中的buffer,此buffer允许修改,所以此处强转为char*
	TCPServerSystem::decrypt((char*)bodyBuffer, bodySize, (byte)((int)packetType + bodySize + (sequenceNumber ^ 123 ^ (int)packetType)));

	// 如果是消息解析数不足,并且收到无效消息时,不会报错
	if (mParsedCount < FrameDefine::MIN_PARSE_COUNT && mPacketTCPFactoryManager->getFactory(packetType) == nullptr)
	{
		reason = "无效客户端, 消息类型错误! 消息类型ID:" + IToS((int)packetType);
		return PARSE_RESULT::INVALID_PACKET_TYPE;
	}
	// 创建消息对象并解析包体
	packet = mPacketTCPSCThreadPool->newClass(packetType);
	if (packet == nullptr)
	{
		reason = "消息解析错误! 消息类型ID:" + IToS((int)packetType);
		return PARSE_RESULT::PACKET_PARSE_FAILED;
	}
	packet->setFieldFlag(fieldFlag);
	if (bodyBuffer != nullptr)
	{
		reader.reset(bodyBuffer, bodySize);
		if (!packet->readFromBuffer(&reader, hasSign))
		{
			// 延迟到主线程销毁消息包
			delayCall([packet]() 
			{
				PacketTCP* temp = packet; 
				mPacketTCPSCThreadPool->destroyClass(temp);
			});
			packet = nullptr;
			reason = "消息解析错误! 消息类型ID:" + IToS((int)packetType);
			return PARSE_RESULT::PACKET_PARSE_FAILED;
		}
	}

	// 校验序列号是否正确
	packet->setSequenceNumber(sequenceNumber);
	if (sequenceNumber != mLastReceiveNumber + 1 && mPlayerGUID > 0 && mLastReceiveNumber != 0xFFFFFFFF)
	{
		string info = "丢包:" + IToS(sequenceNumber - mLastReceiveNumber - 1) + 
							", 角色ID:" + LLToS(mPlayerGUID) + ", 已接收包数量:" + UIToS(mParsedCount) +
							", 当前包序列号:" + IToS(sequenceNumber);
		LOG(info);
		PLAYER_LOG_NO_PRINT(info, mPlayerGUID);
		// 延迟到主线程销毁消息包
		delayCall([packet]()
		{
			PacketTCP* temp = packet;
			mPacketTCPSCThreadPool->destroyClass(temp);
		});
		packet = nullptr;
		reason = "sequence number check error! lastNumber:" + IToS(mLastReceiveNumber) + 
						", receiveNumber:" + IToS(sequenceNumber);
		return PARSE_RESULT::SEQUENCE_NUMBER_ERROR;
	}
	mLastReceiveNumber = sequenceNumber;

	bitIndex = packetEndBitIndex;
	packet->setClient(this);
	return PARSE_RESULT::SUCCESS;
}

void TCPServerClient::executeAllPacket()
{
	// 执行读缓冲区中的所有消息包
	int packetCount = 0;
	DoubleBufferReadScope<PacketTCP*> readScope(mExecutePacketList);
	if (readScope.mReadList != nullptr)
	{
		const llong time0 = getRealTimeMS();
		// 有接收到消息,则说明心跳正常,如果只由ping来判断心跳,可能会出现一直收不到ping,但是会收到其他消息
		// 导致心跳超时的错误,可能是由于客户端逻辑错误导致,但是不能判定为心跳超时
		packetCount = readScope.mReadList->size();
		if (packetCount > 0)
		{
			mHeartBeatTime = mTCPServerSystem->getHeartBeatTimeOut();
		}
		if (packetCount > 100)
		{
#ifndef STRESS_TEST
			LOG("单个客户端一帧执行的消息数量:" + IToS(packetCount) + ", PlayerGUID:" + LLToS(mPlayerGUID) + ", AccountGUID:" + LLToS(mAccountGUID));
#endif
		}
		mTempExecutePacketCountList.clear();
		mWillDestroyPackets.clear();
		for (PacketTCP* packetReply : *readScope.mReadList)
		{
			if (packetReply == nullptr)
			{
				continue;
			}
			if (packetReply->isDestroy())
			{
				ERROR_PROFILE((string("0消息对象已经被销毁:") + typeid(*packetReply).name()).c_str());
				continue;
			}
			if (mIsDeadClient)
			{
				if (!mWillDestroyPackets.add(packetReply))
				{
					mPacketTCPSCThreadPool->destroyClassList(mWillDestroyPackets);
					mWillDestroyPackets.add(packetReply);
				}
				continue;
			}
			packetReply->execute();
#if !defined(VIRTUAL_CLIENT_TEST) && !defined(STRESS_TEST)
			if (mTCPServerSystem->getOutputLog() && packetReply->showInfo())
			{
				INT_STR(packetTypeStr, (int)packetReply->getType());
				MyString<1024> packetInfo;
				packetReply->debugInfo(packetInfo);
				MyString<2048> allInfo;
				allInfo.add("已接收 : ", packetTypeStr.str(), ", ", packetInfo.str());
				debugInfo(allInfo.str());
			}
#endif
			mTempExecutePacketCountList.addOrGet(packetReply->getType()) += 1;
			if (!mWillDestroyPackets.add(packetReply))
			{
				mPacketTCPSCThreadPool->destroyClassList(mWillDestroyPackets);
				mWillDestroyPackets.add(packetReply);
			}
		}
		if (!mWillDestroyPackets.isEmpty())
		{
			mPacketTCPSCThreadPool->destroyClassList(mWillDestroyPackets);
		}
		const llong time1 = getRealTimeMS();
		if (time1 - time0 > 10)
		{
			ushort maxCountType = 0;
			int maxExecuteCount = 0;
			for (const auto& item : mTempExecutePacketCountList)
			{
				if (maxExecuteCount < item.second)
				{
					maxExecuteCount = item.second;
					maxCountType = item.first;
				}
			}
			LOG("执行消息包耗时:" + IToS((int)(time1 - time0)) +
				"毫秒,消息包数量:" + IToS(packetCount) +
				",数量最多的消息包:" + IToS((int)maxCountType) +
				",数量:" + IToS(maxExecuteCount) +
				",IP:" + mIP +
				",角色ID:" + LLToS(mPlayerGUID));
		}
	}
}

void TCPServerClient::debugInfo(const string& info) const
{
	Character* player = mCharacterManager->getCharacter(mPlayerGUID);
	const string& name = player != nullptr ? player->getPrintName() : FrameDefine::EMPTY;
	LLONG_STR(charGUIDStr, mPlayerGUID);
	MyString<1024> fullInfo;
	if (mAccountGUID != 0)
	{
		LLONG_STR(accountGUIDStr, mAccountGUID);
		fullInfo.add("IP:", mIP.c_str(), ", 账号GUID:", accountGUIDStr.str());
		fullInfo.add(", 角色GUID:", charGUIDStr.str(), ", 名字:", name.c_str(), " ||\t", info.c_str());
	}
	else
	{
		fullInfo.add("IP:", mIP.c_str(), ", 角色GUID:", charGUIDStr.str());
		fullInfo.add(", 名字:", name.c_str(), " ||\t", info.c_str());
	}
	if (mPlayerGUID > 0)
	{
		PLAYER_LOG_NO_PRINT(fullInfo.str(), mPlayerGUID);
	}
}

void TCPServerClient::debugError(const string& info) const
{
	Character* player = mCharacterManager->getCharacter(mPlayerGUID);
	const string& name = player != nullptr ? player->getPrintName() : FrameDefine::EMPTY;
	LLONG_STR(charGUIDStr, mPlayerGUID);
	MyString<1024> fullInfo;
	if (mAccountGUID != 0)
	{
		LLONG_STR(accountGUIDStr, mAccountGUID);
		fullInfo.add("IP:", mIP.c_str(), ", 账号GUID:", accountGUIDStr.str());
		fullInfo.add(", 角色GUID:", charGUIDStr.str(), ", 名字:", name.c_str(), " ||\t", info.c_str());
	}
	else
	{
		fullInfo.add("IP:", mIP.c_str(), ", 角色GUID:", charGUIDStr.str());
		fullInfo.add(", 名字:", name.c_str(), " ||\t", info.c_str());
	}
	if (mPlayerGUID > 0)
	{
		PLAYER_ERROR(fullInfo.str(), mPlayerGUID);
	}
}

void TCPServerClient::notifyPing()
{
	mHeartBeatTime = mTCPServerSystem->getHeartBeatTimeOut();
#ifndef STRESS_TEST
	mLastPingList.add(getRealTimeMS());
	if (mLastPingList.size() > 10)
	{
		// 每次心跳是2秒,如果3次心跳的时间小于17秒,留15%的误差空间,则可认为是开了加速外挂,加速了客户端的全局时间
		if (mLastPingList[mLastPingList.size() - 1] - mLastPingList[0] < (mLastPingList.size() - 1) * (mClientPingTime * 1000 * 0.85f))
		{
			string info = "触发了异常心跳检测, 心跳时间列表:" + LLsToS(mLastPingList) +
				", 账号ID:" + LLToS(mAccountGUID) +
				", 角色ID:" + LLToS(mPlayerGUID);
			LOG(info);
			PLAYER_LOG_NO_PRINT(info, mPlayerGUID);
			// 时间校验不通过,则直接封号,为了避免误封而产生更大的问题,只封10分钟
			UNIFIED_UTF8(reason, 128, "客户端时间异常");
			mTCPServerSystem->freezeAccount(mAccountGUID, 60 * 10, reason.str());
		}
		mLastPingList.clear();
	}
#endif
}

void TCPServerClient::notifyServerPing(int index)
{
	if (mServerPingIndex != index)
	{
		return;
	}
	mServerPingWaitTime = -1.0f;
}