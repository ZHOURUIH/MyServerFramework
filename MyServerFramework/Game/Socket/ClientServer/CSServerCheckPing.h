#pragma once

#include "PacketTCP.h"
#include "GamePacketDefine.h"

// auto generate start
// 客户端回复主动发送延迟检测消息,需要排在前面
class CSServerCheckPing : public PacketTCP
{
	BASE(CSServerCheckPing, PacketTCP);
public:
	int mIndex = 0;
	static string mPacketName;
public:
	CSServerCheckPing()
	{
		mType = PACKET_TYPE::CSServerCheckPing;
		mShowInfo = false;
	}
	static const string& getStaticPacketName() { return mPacketName; }
	const string& getPacketName() override { return mPacketName; }
	bool readFromBuffer(SerializerBitRead* reader) override
	{
		bool success = true;
		success = success && reader->readSigned(mIndex);
		return success;
	}
	bool writeToBuffer(SerializerBitWrite* serializer) const override
	{
		bool success = true;
		success = success && serializer->writeSigned(mIndex);
		return success;
	}
	void resetProperty() override
	{
		base::resetProperty();
		mIndex = 0;
	}
	void execute() override;
	// auto generate end
	void debugInfo(MyString<1024>& buffer) override
	{
		debug(buffer, "");
	}
};