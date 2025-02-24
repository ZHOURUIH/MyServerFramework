#pragma once

#include "PacketTCP.h"
#include "GamePacketDefine.h"

// auto generate start
// 通知服务器攻击
class CSAttack : public PacketTCP
{
	BASE(CSAttack, PacketTCP);
public:
	Vector<llong> mTargetGUID;
	int mSkillID = 0;
	llong mTimeStamp = 0;
	static string mPacketName;
public:
	CSAttack()
	{
		mType = PACKET_TYPE::CSAttack;
		mShowInfo = false;
	}
	static const string& getStaticPacketName() { return mPacketName; }
	const string& getPacketName() override { return mPacketName; }
	bool readFromBuffer(SerializerBitRead* reader) override
	{
		bool success = true;
		success = success && reader->readSignedList(mTargetGUID);
		success = success && reader->readSigned(mSkillID);
		success = success && reader->readSigned(mTimeStamp);
		return success;
	}
	bool writeToBuffer(SerializerBitWrite* serializer) const override
	{
		bool success = true;
		success = success && serializer->writeSignedList(mTargetGUID);
		success = success && serializer->writeSigned(mSkillID);
		success = success && serializer->writeSigned(mTimeStamp);
		return success;
	}
	void resetProperty() override
	{
		base::resetProperty();
		mTargetGUID.clear();
		mSkillID = 0;
		mTimeStamp = 0;
	}
	void execute() override;
	// auto generate end
	void debugInfo(MyString<1024>& buffer) override
	{
		debug(buffer, "");
	}
};