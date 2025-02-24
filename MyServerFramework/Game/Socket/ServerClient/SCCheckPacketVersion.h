#pragma once

#include "PacketTCP.h"
#include "GamePacketDefine.h"

// auto generate start
// 向客户端返回网络消息版本号检查结果,此消息应该排第一个
class SCCheckPacketVersion : public PacketTCP
{
	BASE(SCCheckPacketVersion, PacketTCP);
public:
	bool mResult = false;
private:
	static SCCheckPacketVersion mStaticObject;
	static string mPacketName;
public:
	SCCheckPacketVersion()
	{
		mType = PACKET_TYPE::SCCheckPacketVersion;
		mShowInfo = true;
	}
	static SCCheckPacketVersion& get()
	{
		mStaticObject.resetProperty();
		return mStaticObject;
	}
	const string& getPacketName() override { return mPacketName; }
	static const string& getStaticPacketName() { return mPacketName; }
	static constexpr ushort getStaticType() { return PACKET_TYPE::SCCheckPacketVersion; }
	static constexpr bool hasMember() { return true; }
	bool readFromBuffer(SerializerBitRead* reader) override
	{
		bool success = true;
		success = success && reader->readBool(mResult);
		return success;
	}
	bool writeToBuffer(SerializerBitWrite* serializer) const override
	{
		bool success = true;
		success = success && serializer->writeBool(mResult);
		return success;
	}
	void resetProperty() override
	{
		base::resetProperty();
		mResult = false;
	}
	// auto generate end
	void debugInfo(MyString<1024>& buffer) override
	{
		debug(buffer, "");
	}
	static void send(TCPServerClient* client);
};