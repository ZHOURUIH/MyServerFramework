#include "GameHeader.h"

const int MDAccount::ID = 0;
const int MDAccount::Account = 1;
const int MDAccount::Password = 2;
const int MDAccount::CharacterGUID = 3;
const string MDAccount::Name_ID = "ID";
const string MDAccount::Name_Account = "Account";
const string MDAccount::Name_Password = "Password";
const string MDAccount::Name_CharacterGUID = "CharacterGUID";

void MDAccount::fillColName(MySQLTable* table)
{
	table->addColName(Name_ID);
	table->addColName(Name_Account);
	table->addColName(Name_Password);
	table->addColName(Name_CharacterGUID);
}

void MDAccount::parseResult(const HashMap<int, char*>& resultRow)
{
	parseLLong(mID, resultRow.tryGet(ID));
	parseString(mAccount, resultRow.tryGet(Account));
	parseString(mPassword, resultRow.tryGet(Password));
	parseLLong(mCharacterGUID, resultRow.tryGet(CharacterGUID));
}

void MDAccount::paramList(string& params) const
{
	sqlAddLLong(params, mID, true);
	sqlAddString(params, mAccount, true);
	sqlAddString(params, mPassword, true);
	sqlAddLLong(params, mCharacterGUID, false);
}

void MDAccount::generateUpdate(string& params, const ullong flag) const
{
	if (hasBit(flag, Account))
	{
		sqlUpdateString(params, Name_Account, mAccount);
	}
	if (hasBit(flag, Password))
	{
		sqlUpdateString(params, Name_Password, mPassword);
	}
	if (hasBit(flag, CharacterGUID))
	{
		sqlUpdateLLong(params, Name_CharacterGUID, mCharacterGUID);
	}
	// 去除最后的逗号
	const int length = (int)params.length();
	if (length > 0)
	{
		params.erase(length - 1);
	}
}

void MDAccount::clone(MySQLData* target) const
{
	base::clone(target);
	auto* targetData = static_cast<This*>(target);
	targetData->mAccount = mAccount;
	targetData->mPassword = mPassword;
	targetData->mCharacterGUID = mCharacterGUID;
}

void MDAccount::cloneWithFlag(MySQLData* target, const ullong flag) const
{
	base::cloneWithFlag(target, flag);
	auto* targetData = static_cast<This*>(target);
	if (hasBit(flag, Account))
	{
		targetData->mAccount = mAccount;
	}
	if (hasBit(flag, Password))
	{
		targetData->mPassword = mPassword;
	}
	if (hasBit(flag, CharacterGUID))
	{
		targetData->mCharacterGUID = mCharacterGUID;
	}
}

void MDAccount::resetProperty()
{
	base::resetProperty();
	mAccount.clear();
	mPassword.clear();
	mCharacterGUID = 0;
}

bool MDAccount::updateBool(const bool value, const int index)
{
	base::updateBool(value, index);
	switch (index)
	{
	case Account: return false;
	case Password: return false;
	case CharacterGUID: return false;
	}
	return false;
}

bool MDAccount::updateInt(const int value, const int index)
{
	base::updateInt(value, index);
	switch (index)
	{
	case Account: return false;
	case Password: return false;
	case CharacterGUID: return false;
	}
	return false;
}

bool MDAccount::updateFloat(const float value, const int index)
{
	base::updateFloat(value, index);
	switch (index)
	{
	case Account: return false;
	case Password: return false;
	case CharacterGUID: return false;
	}
	return false;
}

bool MDAccount::updateLLong(const llong value, const int index)
{
	base::updateLLong(value, index);
	switch (index)
	{
	case Account: return false;
	case Password: return false;
	case CharacterGUID: mCharacterGUID = value; return true;
	}
	return false;
}

bool MDAccount::updateString(const string& value, const int index)
{
	base::updateString(value, index);
	switch (index)
	{
	case Account: mAccount = value; return true;
	case Password: mPassword = value; return true;
	case CharacterGUID: return false;
	}
	return false;
}
bool MDAccount::hasBool(const bool value, const int index)
{
	switch (index)
	{
	case Account: return false;
	case Password: return false;
	case CharacterGUID: return false;
	}
	return false;
}

bool MDAccount::hasInt(const int value, const int index)
{
	switch (index)
	{
	case Account: return false;
	case Password: return false;
	case CharacterGUID: return false;
	}
	return false;
}

bool MDAccount::hasFloat(const float value, const int index)
{
	switch (index)
	{
	case Account: return false;
	case Password: return false;
	case CharacterGUID: return false;
	}
	return false;
}

bool MDAccount::hasLLong(const llong value, const int index)
{
	switch (index)
	{
	case Account: return false;
	case Password: return false;
	case CharacterGUID: return mCharacterGUID == value;
	}
	return false;
}

bool MDAccount::hasString(const string& value, const int index)
{
	switch (index)
	{
	case Account: return mAccount == value;
	case Password: return mPassword == value;
	case CharacterGUID: return false;
	}
	return false;
}

bool MDAccount::getBool(const int index)
{
	switch (index)
	{
	case Account: return false;
	case Password: return false;
	case CharacterGUID: return false;
	}
	return false;
}

int MDAccount::getInt(const int index)
{
	switch (index)
	{
	case Account: return 0;
	case Password: return 0;
	case CharacterGUID: return 0;
	}
	return 0;
}

float MDAccount::getFloat(const int index)
{
	switch (index)
	{
	case Account: return 0.0f;
	case Password: return 0.0f;
	case CharacterGUID: return 0.0f;
	}
	return 0.0f;
}

llong MDAccount::getLLong(const int index)
{
	switch (index)
	{
	case Account: return 0;
	case Password: return 0;
	case CharacterGUID: return mCharacterGUID;
	}
	return 0;
}

const string& MDAccount::getString(const int index)
{
	switch (index)
	{
	case Account: return mAccount;
	case Password: return mPassword;
	case CharacterGUID: return FrameDefine::EMPTY;
	}
	return FrameDefine::EMPTY;
}