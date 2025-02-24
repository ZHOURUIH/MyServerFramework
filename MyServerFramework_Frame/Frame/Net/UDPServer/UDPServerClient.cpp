#include "FrameHeader.h"

UDPServerClient::UDPServerClient() :
	mSendWriter(new SerializerBitWrite()),
	mEncryptBuffer(new char[FrameDefine::UDP_MAX_PACKET_SIZE])
{}

UDPServerClient::~UDPServerClient()
{
	delete mSendWriter;
	mSendWriter = nullptr;
	delete[] mEncryptBuffer;
	mEncryptBuffer = nullptr;
}

void UDPServerClient::sendPacket(PacketTCP* packet, const llong token, const sockaddr_in& addr)
{
	checkMainThread();
	if (addr.sin_port == 0)
	{
		return;
	}
	const SerializerBitWrite* packetDataBuffer = mUDPServerSystem->getPacketDataBuffer();
	const int bodySize = packetDataBuffer->getByteCount();
	// 判断临时缓冲区大小是否足够
	if (sizeof(byte) + sizeof(int) + FrameDefine::PACKET_TYPE_SIZE + bodySize + FrameDefine::PACKET_CRC_SIZE > FrameDefine::UDP_MAX_PACKET_SIZE)
	{
		return;
	}

	// 将需要加密的数据拷贝到临时缓冲区中,避免加密影响原本消息包中的数据
	MEMCPY(mEncryptBuffer, FrameDefine::UDP_MAX_PACKET_SIZE, packetDataBuffer->getBuffer(), bodySize);
	// 加密消息
	TCPServerSystem::encrypt(mEncryptBuffer, bodySize, FrameDefine::ENCRYPT_KEY, FrameDefine::ENCRYPT_KEY_LENGTH, 0);

	mSendWriter->clear();
	mSendWriter->writeSigned(bodySize);
	mSendWriter->writeUnsigned(generateCRC16(bodySize));
	mSendWriter->writeUnsigned(packet->getType());
	const ullong fieldFlag = packet->getFieldFlag();
	mSendWriter->writeBool(fieldFlag != FrameDefine::FULL_FIELD_FLAG);
	if (fieldFlag != FrameDefine::FULL_FIELD_FLAG)
	{
		mSendWriter->writeUnsigned(fieldFlag);
	}
	mSendWriter->writeBuffer(mEncryptBuffer, bodySize);
	// 下面要计算crc,需要完整的字节信息,所以将剩下的位都填充为0
	mSendWriter->fillZeroToByteEnd();
	// 添加CRC校验码,因为要连包头一起校验,所以只能在这里写
	mSendWriter->writeUnsigned(generateCRC16(mSendWriter->getBuffer(), mSendWriter->getByteCount()));
	{
		// 放入到缓存列表中
		THREAD_LOCK(mSendListLock);
		UDPClientInfo* info = mSendList.tryGet(token);
		if (info == nullptr)
		{
			info = new UDPClientInfo();
			info->setToken(token);
			mSendList.insert(token, info);
		}
		// 虽然一般来说一个客户端的地址不会改变,但是为了保证一定正确,每次发送时都要设置一次地址
		info->setAddress(addr);
		const int sendSize = mSendWriter->getByteCount();
		const int dataBufferSize = getGreaterPower2(sendSize);
		// 由于申请和释放不在同一个线程,所以不使用对象池
		char* packetDataBuffer = new char[dataBufferSize];
		MEMCPY(packetDataBuffer, dataBufferSize, mSendWriter->getBuffer(), mSendWriter->getByteCount());
		info->addSendData(packetDataBuffer, sendSize);
	}
}

void UDPServerClient::processSend(MY_SOCKET socket)
{
	static HashMap<llong, pair<sockaddr_in, Vector<pair<char*, int>>>> tempSendList;
	if (mSendList.size() == 0)
	{
		return;
	}
	{
		tempSendList.clear();
		// 将待发送消息移动到临时列表,避免发送过程中一直锁住mSendList
		THREAD_LOCK(mSendListLock);
		for (const auto& iter : mSendList)
		{
			UDPClientInfo* clientInfo = iter.second;
			if (clientInfo->getSendList().size() == 0)
			{
				continue;
			}
			auto& list = tempSendList.insertOrGet(iter.first);
			list.first = clientInfo->getAddress();
			clientInfo->moveSendList(list.second);
			if (clientInfo->getSendList().size() > 0)
			{
				ERROR("数据移动失败");
			}
		}
	}
	for (const auto& iter : tempSendList)
	{
		const sockaddr_in& addr = iter.second.first;
		for (const auto& item : iter.second.second)
		{
			sendto(socket, item.first, item.second, 0, (sockaddr*)&addr, sizeof(addr));
			delete[] item.first;
		}
	}
}

void UDPServerClient::removeClientToken(const llong token)
{
	THREAD_LOCK(mSendListLock);
	UDPClientInfo* info = nullptr;
	mSendList.erase(token, info);
	delete info;
}

void UDPServerClient::recvData(char* data, const int dataCount, const sockaddr_in& addr)
{
	CmdNetServerReceiveUDPPacket* cmd = nullptr;
	int bitIndex = 0;
	while(true)
	{
		PacketTCP* packet = nullptr;
		llong token = 0;
		// udp基于包的传输,解析出现错误或者未接收完全时都不会再解析
		if (packetRead(data, dataCount, bitIndex, packet, token) != PARSE_RESULT::SUCCESS)
		{
			break;
		}
		// udp消息不支持在子线程执行,需要耗费较多性能去查找对应的客户端
		if (cmd == nullptr)
		{
			CMD_THREAD_DELAY(CmdNetServerReceiveUDPPacket, cmdTemp);
			cmd = cmdTemp;
			cmd->mAddress = addr;
		}
		cmd->mPacketList.insertOrGet(token).push_back(packet);
	}
	if (cmd != nullptr)
	{
		mCommandSystem->pushCommandDelay(cmd, mTCPServerSystem);
	}
}

PARSE_RESULT UDPServerClient::packetRead(char* buffer, const int dataLength, int& bitIndex, PacketTCP*& packet, llong& token)
{
	SerializerBitRead reader(buffer, dataLength);
	reader.setBitIndex(bitIndex);
	packet = nullptr;
	int bodySize = 0;
	ushort bodySizeCRC = 0;
	ushort packetType = 0;
	const char* bodyBuffer = nullptr;
	ushort readCrc = 0;
	bool useFlag = false;
	ullong fieldFlag = FrameDefine::FULL_FIELD_FLAG;
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
	if (!reader.readSigned(token))
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
	// 此处也可能是bodySize是一个非常大的值导致读取失败
	if (bodySize > 0 && !reader.readBufferNoCopy(bodyBuffer, bodySize))
	{
		return PARSE_RESULT::NOT_ENOUGH;
	}
	reader.skipToByteEnd();
	// 在解析包体和读CRC前就计算CRC16
	const ushort crcCode = generateCRC16(buffer, reader.getReadByteCount());
	if (!reader.readUnsigned(readCrc))
	{
		return PARSE_RESULT::NOT_ENOUGH;
	}
	// 解密包体数据
	if (bodyBuffer != nullptr)
	{
		// bodyBuffer指向的是buffer,此buffer允许修改,所以此处强转为char*
		TCPServerSystem::decrypt((char*)bodyBuffer, bodySize, FrameDefine::ENCRYPT_KEY, FrameDefine::ENCRYPT_KEY_LENGTH, 0);
	}

	// 如果是消息解析数不足,并且收到无效消息时,不会报错
	if (mPacketTCPFactoryManager->getFactory(packetType) == nullptr)
	{
		return PARSE_RESULT::PACKET_PARSE_FAILED;
	}
	// 创建消息对象并解析包体
	packet = mPacketTCPThreadPool->newClass(packetType);
	if (packet == nullptr)
	{
		return PARSE_RESULT::PACKET_PARSE_FAILED;
	}
	packet->setFieldFlag(fieldFlag);
	if (bodyBuffer != nullptr)
	{
		SerializerBitRead bodyReader(bodyBuffer, bodySize);
		if (!packet->readFromBuffer(&bodyReader))
		{
			mPacketTCPThreadPool->destroyClass(packet);
			return PARSE_RESULT::PACKET_PARSE_FAILED;
		}
	}

	// CRC校验
	if (crcCode != readCrc)
	{
		mPacketTCPThreadPool->destroyClass(packet);
		packet = nullptr;
		return PARSE_RESULT::CRC_ERROR;
	}
	bitIndex = reader.getBitIndex();
	return PARSE_RESULT::SUCCESS;
}