#include "FrameHeader.h"

QueryScope::QueryScope(MySQLTable* table, const char* queryStr)
{
	mTable = table;
	const llong time0 = getRealTimeMS();
	mResult = mTable->executeQuery(queryStr);
	const llong time1 = getRealTimeMS();
	if (mResult && time1 - time0 >= 5)
	{
		LOG("查询数据的耗时较长:" + IToS((int)(time1 - time0)) + "毫秒, sql:" + queryStr);
	}
}

QueryScope::~QueryScope()
{
	if (mResult != nullptr)
	{
		mTable->endQuery(mResult);
	}
}