#include "FrameHeader.h"

#ifdef _MYSQL
QueryScope::QueryScope(const MySQLTable* table, const char* queryStr)
{
	mTable = table;
	query(queryStr);
}

QueryScope::QueryScope(const MySQLTable* table)
{
	mTable = table;
}

QueryScope::~QueryScope()
{
	if (mResult != nullptr)
	{
		mTable->endQuery(mResult);
	}
}

MYSQL_RES* QueryScope::query(const char* queryStr)
{
	const llong time0 = readTSC();
	mResult = mTable->executeQuery(queryStr);
	const llong time1 = readTSC();
	const double timeMS = Profiler::ticksToMS(time1 - time0);
	if (mResult && timeMS >= 5)
	{
		LOG("查询数据的耗时较长:" + FToS((float)timeMS) + "毫秒, sql:" + queryStr);
	}
	return mResult;
}
#endif