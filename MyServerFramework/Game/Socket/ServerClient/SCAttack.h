#pragma once

#include "PacketTCP.h"
#include "GamePacketDefine.h"

// auto generate start
// 回复客户端攻击结果
class SCAttack : public PacketTCP
{
	BASE(SCAttack, PacketTCP);
public:
private:
	static SCAttack mStaticObject;
	static string mPacketName;
public:
	SCAttack()
	{
		mType = PACKET_TYPE::SCAttack;
		mShowInfo = true;
	}
	static SCAttack& get()
	{
		mStaticObject.resetProperty();
		return mStaticObject;
	}
	const string& getPacketName() override { return mPacketName; }
	static const string& getStaticPacketName() { return mPacketName; }
	static constexpr ushort getStaticType() { return PACKET_TYPE::SCAttack; }
	static constexpr bool hasMember() { return false; }
	bool readFromBuffer(SerializerBitRead* reader) override { return true; }
	bool writeToBuffer(SerializerBitWrite* serializer) const override { return true; }
	void resetProperty() override
	{
		base::resetProperty();
	}
	// auto generate end
	void debugInfo(MyString<1024>& buffer) override
	{
		debug(buffer, "");
	}
	static void send(CharacterPlayer* player);
};