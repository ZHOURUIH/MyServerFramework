#pragma once

#include "PacketTCP.h"
#include "GamePacketDefine.h"

// auto generate start
// 角色在Game层的完整数据
class SCCharacterFullGameData : public PacketTCP
{
	BASE(SCCharacterFullGameData, PacketTCP);
public:
	enum class Field : byte
	{
		HP = 0,
		MaxHP = 1,
		Name = 2,
	};
public:
	int mHP = 0;
	int mMaxHP = 0;
	string mName;
private:
	static SCCharacterFullGameData mStaticObject;
	static string mPacketName;
public:
	SCCharacterFullGameData()
	{
		mType = PACKET_TYPE::SCCharacterFullGameData;
		mShowInfo = false;
	}
	static SCCharacterFullGameData& get()
	{
		mStaticObject.resetProperty();
		return mStaticObject;
	}
	const string& getPacketName() override { return mPacketName; }
	static const string& getStaticPacketName() { return mPacketName; }
	static constexpr ushort getStaticType() { return PACKET_TYPE::SCCharacterFullGameData; }
	static constexpr bool hasMember() { return true; }
	bool readFromBuffer(SerializerBitRead* reader) override
	{
		bool success = true;
		if (isFieldValid(Field::HP))
		{
			success = success && reader->readSigned(mHP);
		}
		if (isFieldValid(Field::MaxHP))
		{
			success = success && reader->readSigned(mMaxHP);
		}
		if (isFieldValid(Field::Name))
		{
			success = success && reader->readString(mName);
		}
		return success;
	}
	bool writeToBuffer(SerializerBitWrite* serializer) const override
	{
		bool success = true;
		if (isFieldValid(Field::HP))
		{
			success = success && serializer->writeSigned(mHP);
		}
		if (isFieldValid(Field::MaxHP))
		{
			success = success && serializer->writeSigned(mMaxHP);
		}
		if (isFieldValid(Field::Name))
		{
			success = success && serializer->writeString(mName);
		}
		return success;
	}
	void resetProperty() override
	{
		base::resetProperty();
		mHP = 0;
		mMaxHP = 0;
		mName.clear();
	}
	// auto generate end
	void debugInfo(MyString<1024>& buffer) override
	{
		debug(buffer, "");
	}
	static void send(CharacterPlayer* player);
};