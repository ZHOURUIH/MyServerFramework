#pragma once

#include "PacketTCP.h"
#include "GamePacketDefine.h"

// auto generate start
// 登录角色
class CSLogin : public PacketTCP
{
	BASE(CSLogin, PacketTCP);
public:
	string mAccount;
	string mPassword;
	static string mPacketName;
public:
	CSLogin()
	{
		mType = PACKET_TYPE::CSLogin;
		mShowInfo = false;
	}
	static const string& getStaticPacketName() { return mPacketName; }
	const string& getPacketName() override { return mPacketName; }
	bool readFromBuffer(SerializerBitRead* reader) override
	{
		bool success = true;
		success = success && reader->readString(mAccount);
		success = success && reader->readString(mPassword);
		return success;
	}
	bool writeToBuffer(SerializerBitWrite* serializer) const override
	{
		bool success = true;
		success = success && serializer->writeString(mAccount);
		success = success && serializer->writeString(mPassword);
		return success;
	}
	void resetProperty() override
	{
		base::resetProperty();
		mAccount.clear();
		mPassword.clear();
	}
	void execute() override;
	// auto generate end
	void debugInfo(MyString<1024>& buffer) override
	{
		debug(buffer, "");
	}
};