#pragma once

#include "PacketTCP.h"
#include "GamePacketDefine.h"

// auto generate start
// 主动向客户端发送延迟检测消息,也需要排在前面
class SCServerCheckPing : public PacketTCP
{
	BASE(SCServerCheckPing, PacketTCP);
public:
	int mIndex = 0;
private:
	static SCServerCheckPing mStaticObject;
	static string mPacketName;
public:
	SCServerCheckPing()
	{
		mType = PACKET_TYPE::SCServerCheckPing;
		mShowInfo = false;
	}
	static SCServerCheckPing& get()
	{
		mStaticObject.resetProperty();
		return mStaticObject;
	}
	const string& getPacketName() override { return mPacketName; }
	static const string& getStaticPacketName() { return mPacketName; }
	static constexpr ushort getStaticType() { return PACKET_TYPE::SCServerCheckPing; }
	static constexpr bool hasMember() { return true; }
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
	// auto generate end
	void debugInfo(MyString<1024>& buffer) override
	{
		debug(buffer, "");
	}
	static void send(TCPServerClient* client, int index);
};