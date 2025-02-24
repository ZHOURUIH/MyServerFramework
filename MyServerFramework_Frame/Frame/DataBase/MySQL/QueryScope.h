#pragma once

#ifdef _MYSQL
#include "FrameBase.h"

class QueryScope
{
protected:
	MySQLTable* mTable = nullptr;
public:
	MYSQL_RES* mResult = nullptr;
public:
	QueryScope(MySQLTable* table, const char* queryStr);
	~QueryScope();
};

#endif