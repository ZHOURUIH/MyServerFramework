#pragma once

#include "MySQLTableT.h"
#include "MDAccount.h"

class MySQLAccount : public MySQLTableT<MDAccount>
{
	BASE(MySQLAccount, MySQLTableT<MDAccount>);
public:
	explicit MySQLAccount(const char* tableName) :base(tableName) {}
	void lateInit() override;
};