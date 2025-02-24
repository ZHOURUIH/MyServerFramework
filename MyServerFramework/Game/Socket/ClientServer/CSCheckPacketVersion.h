#pragma once

#include "PacketTCP.h"
#include "GamePacketDefine.h"

// auto generate start
// 请求服务器检查网络消息版本号是否正确,此消息应该排第一个
class CSCheckPacketVersion : public PacketTCP
{
	BASE(CSCheckPacketVersion, PacketTCP);
public:
	string mPacketVersion;
	static string mPacketName;
public:
	CSCheckPacketVersion()
	{
		mType = PACKET_TYPE::CSCheckPacketVersion;
		mShowInfo = true;
	}
	static const string& getStaticPacketName() { return mPacketName; }
	const string& getPacketName() override { return mPacketName; }
	bool readFromBuffer(SerializerBitRead* reader) override
	{
		bool success = true;
		success = success && reader->readString(mPacketVersion);
		return success;
	}
	bool writeToBuffer(SerializerBitWrite* serializer) const override
	{
		bool success = true;
		success = success && serializer->writeString(mPacketVersion);
		return success;
	}
	void resetProperty() override
	{
		base::resetProperty();
		mPacketVersion.clear();
	}
	void execute() override;
	// auto generate end
	void debugInfo(MyString<1024>& buffer) override
	{
		debug(buffer, "");
	}
};