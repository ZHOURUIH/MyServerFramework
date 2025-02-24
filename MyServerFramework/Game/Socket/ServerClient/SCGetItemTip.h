#pragma once

#include "PacketTCP.h"
#include "GamePacketDefine.h"
#include "NetStructItemInfo.h"

// auto generate start
// 通用的奖励物品提示
class SCGetItemTip : public PacketTCP
{
	BASE(SCGetItemTip, PacketTCP);
public:
	Vector<NetStructItemInfo> mIDCountList;
private:
	static SCGetItemTip mStaticObject;
	static string mPacketName;
public:
	SCGetItemTip()
	{
		mType = PACKET_TYPE::SCGetItemTip;
		mShowInfo = false;
	}
	static SCGetItemTip& get()
	{
		mStaticObject.resetProperty();
		return mStaticObject;
	}
	const string& getPacketName() override { return mPacketName; }
	static const string& getStaticPacketName() { return mPacketName; }
	static constexpr ushort getStaticType() { return PACKET_TYPE::SCGetItemTip; }
	static constexpr bool hasMember() { return true; }
	bool readFromBuffer(SerializerBitRead* reader) override
	{
		bool success = true;
		success = success && reader->readCustomList(mIDCountList);
		return success;
	}
	bool writeToBuffer(SerializerBitWrite* serializer) const override
	{
		bool success = true;
		success = success && serializer->writeCustomList(mIDCountList);
		return success;
	}
	void resetProperty() override
	{
		base::resetProperty();
		mIDCountList.clear();
	}
	// auto generate end
	void debugInfo(MyString<1024>& buffer) override
	{
		debug(buffer, "");
	}
	static void send(CharacterPlayer* player);
};