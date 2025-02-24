#pragma once

#include "MySQLData.h"

// 账号表
class MDAccount : public MySQLData
{
	BASE(MDAccount, MySQLData);
public:
	static const int ID;
	static const int Account;
	static const int Password;
	static const int CharacterGUID;
	static const string Name_ID;
	static const string Name_Account;
	static const string Name_Password;
	static const string Name_CharacterGUID;
public:
	string mAccount;							// 账号
	string mPassword;							// 密码
	llong mCharacterGUID = 0;					// 角色的ID
public:
	static void fillColName(MySQLTable* table);
	void parseResult(const HashMap<int, char*>& resultRow) override;
	void paramList(string& params) const override;
	void generateUpdate(string& params, ullong flag) const override;
	void clone(MySQLData* target) const override;
	void cloneWithFlag(MySQLData* target, ullong flag) const override;
	void resetProperty() override;
	bool updateBool(bool value, int index) override;
	bool updateInt(int value, int index) override;
	bool updateFloat(float value, int index) override;
	bool updateLLong(llong value, int index) override;
	bool updateString(const string& value, int index) override;
	bool hasBool(bool value, int index) override;
	bool hasInt(int value, int index) override;
	bool hasFloat(float value, int index) override;
	bool hasLLong(llong value, int index) override;
	bool hasString(const string& value, int index) override;
	bool getBool(int index) override;
	int getInt(int index) override;
	float getFloat(int index) override;
	llong getLLong(int index) override;
	const string& getString(int index) override;
};