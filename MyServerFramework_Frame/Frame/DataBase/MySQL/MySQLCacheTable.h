#pragma once

#ifdef _MYSQL
#include "FrameBase.h"
#include "MySQLCacheData.h"

class MICRO_LEGEND_FRAME_API MySQLCacheTable
{
public:
	MySQLCacheTable(MySQLTable* table);
	void destroy();
	ullong columnToFlag(int colName) const;
	ullong columnsToFlag(const Vector<int>& colNames) const;
	MySQLData* getCacheData(const llong id) const
	{
		MySQLCacheData* cacheData = mCacheDataList.tryGet(id);
		return cacheData != nullptr ? cacheData->getData() : nullptr;
	}
	MySQLData* getCacheDataByColumnLLong2Or(int column0, llong value0, int column1, llong value1);
	MySQLData* getCacheDataByColumnLLong2And(int column0, llong value0, int column1, llong value1);
	MySQLData* getCacheDataByColumnInt(int column, int value);
	MySQLData* getCacheDataByColumnLLong(int column, llong value);
	MySQLData* getCacheDataByColumnString(int column, const string& value);
	MySQLData* getCacheDataByColumnString2And(int column0, const string& value0, int column1, const string& value1);
	MySQLData* getCacheDataByColumnLLongIntAnd(int column0, llong value0, int column1, int value1);
	MySQLData* getCacheDataByColumnLLongStringAnd(int column0, llong value0, int column1, const string& value1);
	void getCacheDataListByColumnInt(int column0, int value0, Vector<MySQLData*>& dataList);
	void updateCacheBool(llong id, bool value, int colName) const;
	void updateCacheInt(llong id, int value, int colName) const;
	void updateCacheInt2(llong id, int value0, int colName0, int value1, int colName1) const;
	void updateCacheFloat(llong id, float value, int colName) const;
	void updateCacheLLong(llong id, llong value, int colName) const;
	void updateCacheLLongInt(llong id, llong value0, int colName0, int value1, int column1) const;
	void updateCacheLLongBool(llong id, llong value0, int colName0, bool value1, int column1) const;
	void updateCacheString(llong id, const string& value, int colName) const;
	void updateCacheString2(llong id, const string& value0, int colName0, const string& value1, int colName1) const;
	void updateCacheAllString(int colName, const string& value) const;
	void updateCacheAllInt(int colName, int value) const;
	void updateCacheAllLLong(int colName, llong value) const;
	void updateCacheAllFloat(int colName, float value) const;
	void updateCacheAllBool(int colName, bool value) const;
	void addCacheData(const MySQLData* data, ullong validFlag);
	void addCacheDataList(const Vector<MySQLData*>& dataList, ullong validFlag);
	void deleteAllCacheData();
	void deleteCacheData(llong id);
	void deleteCacheDataInIDRange(llong minID, llong maxID);
	// 因为需要支持根据部分字段的值来删除数据,所以在查询时需要查询所有字段的数据,否则可能会删除失败,导致缓存跟数据库不一致
	void deleteCacheDataByColumnBool(int column, bool value, bool onlyOnce);
	void deleteCacheDataByColumnInt(int column, int value, bool onlyOnce);
	void deleteCacheDataByColumnFloat(int column, float value, bool onlyOnce);
	void deleteCacheDataByColumnLLong(int column, llong value, bool onlyOnce);
	void deleteCacheDataByColumnString(int column, const string& value, bool onlyOnce);
	void deleteCacheDataByColumnLLong2And(int column0, llong value0, int column1, llong value1, bool onlyOnce);
	void deleteCacheDataByColumnLLong2Or(int column0, llong value0, int column1, llong value1, bool onlyOnce);
protected:
	HashMap<llong, MySQLCacheData*> mCacheDataList;
	MySQLTable* mTable = nullptr;		// 需要在子类进行赋值
};

#endif