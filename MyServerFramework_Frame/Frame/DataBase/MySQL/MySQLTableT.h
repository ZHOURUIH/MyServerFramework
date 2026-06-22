#pragma once

#ifdef _MYSQL
#include "MySQLTable.h"
#include "MySQLDataListScope.h"
#include "MySQLDataListScopeT.h"
#include "MySQLDataHashMapScopeT.h"
#include "MySQLDataPool.h"
#include "MySQLDataScopeT.h"

template<typename T>
class MySQLTableT : public MySQLTable
{
	BASE(MySQLTableT, MySQLTable);
public:
	MySQLTableT(const char* tableName) :base(tableName) {}
	void init() override
	{
		base::init();
		T::fillColName(this);
	}
	MySQLData* createData() override { return mMySQLDataPool->newClassTemp<T>(); }
	void createDataList(Vector<MySQLData*>& dataList, int count) override { return mMySQLDataPool->newClassListTemp<T>(dataList, count); }
	T* createDataT() { return mMySQLDataPool->newClassTemp<T>(); }
	void createDataListT(Vector<T*>& dataList, int count) { return mMySQLDataPool->newClassListTemp<T>(dataList, count); }
	MySQLData* createPersistData() override { return mMySQLDataPool->newClass<T>(); }
	void createPersistDataList(Vector<MySQLData*>& dataList, int count) override { return mMySQLDataPool->newClassList<T>(dataList, count); }
	T* createPersistDataT() { return mMySQLDataPool->newClass<T>(); }
	void createPersistDataListT(Vector<T*>& dataList, int count) { return mMySQLDataPool->newClassList<T>(dataList, count); }
	const string& getColNameT(typename T::Column colIndex) const { return getColName((int)colIndex); }
	int queryMaxIntT(typename T::Column colIndex)
	{
		return queryMaxInt((int)colIndex);
	}
	llong queryMaxLLongT(typename T::Column colIndex)
	{
		return queryMaxLLong((int)colIndex);
	}
	bool updateDataT(const T* data, ullong flag) const
	{
		return updateData(data, flag);
	}
	bool insertDataT(const T* data, bool showError = true) const
	{
		return insertData(data, showError);
	}
	ullong generateColumnFlagT(typename T::Column colIndex)
	{
		return generateColumnFlag((int)colIndex);
	}
	ullong generateColumnFlagT(typename T::Column* colIndex, int colCount)
	{
		return generateColumnFlag((short*)colIndex, colCount);
	}
	bool updateAllStringT(typename T::Column col, const string& paramValue) const
	{
		return updateAllString((int)col, paramValue);
	}
	bool updateAllIntT(typename T::Column col, int paramValue) const
	{
		return updateAllInt((int)col, paramValue);
	}
	bool updateAllLLongT(typename T::Column col, llong paramValue) const
	{
		return updateAllLLong((int)col, paramValue);
	}
	bool updateAllFloatT(typename T::Column col, float paramValue) const
	{
		return updateAllFloat((int)col, paramValue);
	}
	bool updateAllBoolT(typename T::Column col, bool paramValue) const
	{
		return updateAllBool((int)col, paramValue);
	}
	bool updateBoolT(llong id, typename T::Column col, bool paramValue) const
	{
		return updateBool(id, (int)col, paramValue);
	}
	bool updateIntT(llong id, typename T::Column col, int paramValue) const
	{
		return updateInt(id, (int)col, paramValue);
	}
	bool updateFloatT(llong id, typename T::Column col, float paramValue) const
	{
		return updateFloat(id, (int)col, paramValue);
	}
	bool updateStringT(llong id, typename T::Column col, const char* paramValue) const
	{
		return updateString(id, (int)col, paramValue);
	}
	bool updateStringT(llong id, typename T::Column col, const string& paramValue) const
	{
		return updateString(id, (int)col, paramValue);
	}
	bool updateString2T(llong id, typename T::Column col0, const string& paramValue0, typename T::Column col1, const string& paramValue1) const
	{
		return updateString2(id, (int)col0, paramValue0, (int)col1, paramValue1);
	}
	bool updateIntFloatT(llong id, typename T::Column col0, int paramValue0, typename T::Column col1, float paramValue1) const
	{
		return updateIntFloat(id, (int)col0, paramValue0, (int)col1, paramValue1);
	}
	bool updateInt2T(llong id, typename T::Column column0, int value0, typename T::Column column1, int value1) const
	{
		return updateInt2(id, (int)column0, value0, (int)column1, value1);
	}
	bool updateLLongT(llong id, typename T::Column col, llong paramValue) const
	{
		return updateLLong(id, (int)col, paramValue);
	}
	bool updateLLongIntT(llong id, typename T::Column column0, llong value0, typename T::Column column1, int value1) const
	{
		return updateLLongInt(id, (int)column0, value0, (int)column1, value1);
	}
	bool updateLLongBoolT(llong id, typename T::Column column0, llong value0, typename T::Column column1, bool value1) const
	{
		return updateLLongBool(id, (int)column0, value0, (int)column1, value1);
	}
	bool deleteByLLongT(typename T::Column column, llong value, bool onlyOnce) const
	{
		return deleteByLLong((int)column, value, onlyOnce);
	}
	bool deleteByIntT(typename T::Column column, int value, bool onlyOnce) const
	{
		return deleteByInt((int)column, value, onlyOnce);
	}
	bool deleteByStringT(typename T::Column column, const string& value, bool onlyOnce) const
	{
		return deleteByString((int)column, value, onlyOnce);
	}
	bool deleteByLLong2AndT(typename T::Column column0, llong value0, typename T::Column column1, llong value1, bool onlyOnce) const
	{
		return deleteByLLong2And((int)column0, value0, (int)column1, value1, onlyOnce);
	}
	bool deleteByLLongIntAndT(typename T::Column column0, llong value0, typename T::Column column1, int value1, bool onlyOnce) const
	{
		return deleteByLLongIntAnd((int)column0, value0, (int)column1, value1, onlyOnce);
	}
	bool deleteByLLong2OrT(typename T::Column column0, llong value0, typename T::Column column1, llong value1, bool onlyOnce) const
	{
		return deleteByLLong2Or((int)column0, value0, (int)column1, value1, onlyOnce);
	}
	bool queryExistByStringT(typename T::Column column, const string& value)
	{
		return queryExistByString((int)column, value);
	}
	bool queryExistByLLongStringAndT(typename T::Column column0, llong value0, typename T::Column column1, const string& value1)
	{
		return queryExistByLLongStringAnd((int)column0, value0, (int)column1, value1);
	}
	MySQLDataListScopeT<T> queryAllPartialT(const Vector<typename T::Column>& columns, bool createTempData = true)
	{
		Vector<string> columnInts;
		for (const T::Column col : columns)
		{
			columnInts.add(getColName((int)col));
		}
		return MySQLDataListScopeT<T>(queryAllPartial(columnInts, createTempData));
	}
	MySQLDataListScopeT<T> queryAllT(bool createTempData = true) 
	{
		return MySQLDataListScopeT<T>(queryAll(createTempData));
	}
	MySQLDataListScopeT<T> queryAllNoCacheT(bool createTempData = true)
	{
		return MySQLDataListScopeT<T>(queryAllNoCache(createTempData));
	}
	MySQLDataHashMapScopeT<T> queryAllMapT(bool createTempData = true)
	{
		HashMap<llong, T*> dataMap;
		for (MySQLData* data : queryAll(createTempData))
		{
			dataMap.add(data->mID, static_cast<T*>(data));
		}
		return MySQLDataHashMapScopeT<T>(move(dataMap));
	}
	MySQLDataListScopeT<T> queryListByFullParamT(const QueryParam& param)
	{
		return MySQLDataListScopeT<T>(queryListByFullParam(param));
	}
	MySQLDataListScopeT<T> queryListByLLongT(typename T::Column column, llong value, int maxCount)
	{
		return MySQLDataListScopeT<T>(queryListByLLong((int)column, value, maxCount));
	}
	MySQLDataListScopeT<T> queryListByLLong2OrT(typename T::Column column0, llong value0, typename T::Column column1, llong value1, int maxCount)
	{
		return MySQLDataListScopeT<T>(queryListByLLong2Or((int)column0, value0, (int)column1, value1, maxCount));
	}
	MySQLDataHashMapScopeT<T> queryMapByIDListT(const Vector<llong>& idList)
	{
		if (idList.isEmpty())
		{
			return MySQLDataHashMapScopeT<T>();
		}
		HashMap<llong, T*> dataMap;
		for (MySQLData* data : queryListByIDList(idList))
		{
			dataMap.add(data->mID, static_cast<T*>(data));
		}
		return MySQLDataHashMapScopeT<T>(move(dataMap));
	}
	MySQLDataHashMapScopeT<T> queryMapByIDListT(Vector<llong>&& idList)
	{
		if (idList.isEmpty())
		{
			return MySQLDataHashMapScopeT<T>();
		}
		HashMap<llong, T*> dataMap;
		for (MySQLData* data : queryListByIDList(move(idList)))
		{
			dataMap.add(data->mID, static_cast<T*>(data));
		}
		return MySQLDataHashMapScopeT<T>(move(dataMap));
	}
	MySQLDataListScopeT<T> queryListByIDListT(const Vector<llong>& idList)
	{
		if (idList.isEmpty())
		{
			return MySQLDataListScopeT<T>();
		}
		return MySQLDataListScopeT<T>(queryListByIDList(idList));
	}
	MySQLDataListScopeT<T> queryListByIDListT(Vector<llong>&& idList)
	{
		if (idList.isEmpty())
		{
			return MySQLDataListScopeT<T>();
		}
		return MySQLDataListScopeT<T>(queryListByIDList(move(idList)));
	}
	MySQLDataListScopeT<T> queryListByStringListT(typename T::Column column, const Vector<string>& value)
	{
		if (value.isEmpty())
		{
			return MySQLDataListScopeT<T>();
		}
		return MySQLDataListScopeT<T>(queryListByStringList((int)column, value));
	}
	MySQLDataListScopeT<T> queryListByIntGreaterT(typename T::Column column, int value)
	{
		return MySQLDataListScopeT<T>(queryListByIntGreater((int)column, value));
	}
	MySQLDataListScopeT<T> queryListByIntT(typename T::Column column, int value, int maxCount)
	{
		return MySQLDataListScopeT<T>(queryListByInt((int)column, value, maxCount));
	}
	MySQLDataListScopeT<T> queryListByStringT(typename T::Column column, const string& value)
	{
		return MySQLDataListScopeT<T>(queryListByString((int)column, value));
	}
	MySQLDataListScopeT<T> queryListByIDRangeT(llong minID, llong maxID)
	{
		return MySQLDataListScopeT<T>(queryListByIDRange(minID, maxID));
	}
	MySQLDataScopeT<T> queryByIDT(llong instanceID)
	{
		return MySQLDataScopeT<T>(static_cast<T*>(queryByID(instanceID)));
	}
	MySQLDataScopeT<T> queryByLLong2OrT(typename T::Column column0, llong value, typename T::Column column1, llong value1)
	{
		return MySQLDataScopeT<T>(static_cast<T*>(queryByLLong2Or((int)column0, value, (int)column1, value1)));
	}
	MySQLDataScopeT<T> queryByLLongIntAndT(typename T::Column column0, llong value0, typename T::Column column1, int value1)
	{
		return MySQLDataScopeT<T>(static_cast<T*>(queryByLLongIntAnd((int)column0, value0, (int)column1, value1)));
	}
	MySQLDataScopeT<T> queryByString2AndT(typename T::Column column0, const string& value0, typename T::Column column1, const string& value1)
	{
		return MySQLDataScopeT<T>(static_cast<T*>(queryByString2And((int)column0, value0, (int)column1, value1)));
	}
	MySQLDataScopeT<T> queryByLLongStringAndT(typename T::Column column0, llong value0, typename T::Column column1, const string& value1)
	{
		return MySQLDataScopeT<T>(static_cast<T*>(queryByLLongStringAnd((int)column0, value0, (int)column1, value1)));
	}
	MySQLDataScopeT<T> queryByIntT(typename T::Column column, int value)
	{
		return MySQLDataScopeT<T>(static_cast<T*>(queryByInt((int)column, value)));
	}
	MySQLDataScopeT<T> queryByStringT(typename T::Column column, const string& value)
	{
		return MySQLDataScopeT<T>(static_cast<T*>(queryByString((int)column, value)));
	}
	MySQLDataScopeT<T> queryByLLongT(typename T::Column column, llong value)
	{
		return MySQLDataScopeT<T>(static_cast<T*>(queryByLLong((int)column, value)));
	}
	// 查询单条数据的一个整数字段
	int queryIntT(llong id, typename T::Column column)
	{
		return queryInt(id, (int)column);
	}
	HashMap<llong, int> queryIntListT(const Vector<llong>& id, typename T::Column column)
	{
		return queryIntList(id, (int)column);
	}
	HashMap<llong, int> queryIntListT(typename T::Column column)
	{
		return queryIntList((int)column);
	}
	// 查询单条数据的一个浮点数字段
	float queryFloatT(llong id, typename T::Column column)
	{
		return queryFloat(id, (int)column);
	}
	HashMap<llong, float> queryFloatListT(const Vector<llong>& id, typename T::Column column)
	{
		return queryFloatList(id, (int)column);
	}
	HashMap<llong, float> queryFloatListT(typename T::Column column)
	{
		return queryFloatList((int)column);
	}
	// 查询单条数据的一个长整型字段
	llong queryLLongT(llong id, typename T::Column column)
	{
		return queryLLong(id, (int)column);
	}
	HashMap<llong, llong> queryLLongListT(const Vector<llong>& id, typename T::Column column)
	{
		return queryLLongList(id, (int)column);
	}
	HashMap<llong, llong> queryLLongListT(typename T::Column column)
	{
		return queryLLongList((int)column);
	}
	// 查询单条数据的一个字符串字段
	string queryStringT(llong id, typename T::Column column)
	{
		return queryString(id, (int)column);
	}
	HashMap<llong, string> queryStringListT(const Vector<llong>& id, typename T::Column column)
	{
		return queryStringList(id, (int)column);
	}
	HashMap<llong, string> queryStringListT(typename T::Column column)
	{
		return queryStringList((int)column);
	}
};
#endif