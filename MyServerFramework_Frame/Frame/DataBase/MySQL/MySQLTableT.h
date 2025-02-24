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
	MySQLData* createData() override												{ return mMySQLDataPool->newClassTemp<T>(); }
	void createDataList(Vector<MySQLData*>& dataList, int count) override			{ return mMySQLDataPool->newClassListTemp<T>(dataList, count); }
	T* createDataT()																{ return mMySQLDataPool->newClassTemp<T>(); }
	void createDataListT(Vector<T*>& dataList, int count)							{ return mMySQLDataPool->newClassListTemp<T>(dataList, count); }
	MySQLData* createPersistData() override											{ return mMySQLDataPool->newClass<T>(); }
	void createPersistDataList(Vector<MySQLData*>& dataList, int count) override	{ return mMySQLDataPool->newClassList<T>(dataList, count); }
	T* createPersistDataT()															{ return mMySQLDataPool->newClass<T>(); }
	void createPersistDataListT(Vector<T*>& dataList, int count)					{ return mMySQLDataPool->newClassList<T>(dataList, count); }
	// 查询,不要直接放到for中进行遍历,否则会由于对象析构而使列表被清空,需要定义临时变量再遍历临时变量
	MySQLDataListScopeT<T> queryAllT(const bool createTempData = true) 
	{
		return MySQLDataListScopeT<T>(queryAll(createTempData));
	}
	MySQLDataListScopeT<T> queryDataListByFullParamT(const QueryParam& param)
	{
		return MySQLDataListScopeT<T>(queryDataListByFullParam(param));
	}
	MySQLDataListScopeT<T> queryDataListByColumnLLongT(int column, llong value, int maxCount)
	{
		return MySQLDataListScopeT<T>(queryDataListByColumnLLong(column, value, maxCount));
	}
	MySQLDataListScopeT<T> queryDataListByColumnLLong2OrT(int column0, llong value0, int column1, llong value1, int maxCount)
	{
		return MySQLDataListScopeT<T>(queryDataListByColumnLLong2Or(column0, value0, column1, value1, maxCount));
	}
	MySQLDataHashMapScopeT<T> queryDataMapByIDListT(const Vector<llong>& idList)
	{
		if (idList.size() == 0)
		{
			return MySQLDataHashMapScopeT<T>();
		}
		HashMap<llong, T*> dataMap;
		for (MySQLData* data : queryDataListByIDList(idList))
		{
			dataMap.insert(data->mID, static_cast<T*>(data));
		}
		return MySQLDataHashMapScopeT<T>(move(dataMap));
	}
	MySQLDataHashMapScopeT<T> queryDataMapByIDListT(Vector<llong>&& idList)
	{
		if (idList.size() == 0)
		{
			return MySQLDataHashMapScopeT<T>();
		}
		HashMap<llong, T*> dataMap;
		for (MySQLData* data : queryDataListByIDList(move(idList)))
		{
			dataMap.insert(data->mID, static_cast<T*>(data));
		}
		return MySQLDataHashMapScopeT<T>(move(dataMap));
	}
	MySQLDataListScopeT<T> queryDataListByIDListT(const Vector<llong>& idList)
	{
		if (idList.size() == 0)
		{
			return MySQLDataListScopeT<T>();
		}
		return MySQLDataListScopeT<T>(queryDataListByIDList(idList));
	}
	MySQLDataListScopeT<T> queryDataListByIDListT(Vector<llong>&& idList)
	{
		if (idList.size() == 0)
		{
			return MySQLDataListScopeT<T>();
		}
		return MySQLDataListScopeT<T>(queryDataListByIDList(move(idList)));
	}
	MySQLDataListScopeT<T> queryDataListByColumnStringListT(const int column, const Vector<string>& value)
	{
		if (value.size() == 0)
		{
			return MySQLDataListScopeT<T>();
		}
		return MySQLDataListScopeT<T>(queryDataListByColumnStringList(column, value));
	}
	MySQLDataListScopeT<T> queryDataListByColumnIntGreaterT(const int column, const int value)
	{
		return MySQLDataListScopeT<T>(queryDataListByColumnIntGreater(column, value));
	}
	MySQLDataListScopeT<T> queryDataListByColumnIntT(const int column, const int value, int maxCount)
	{
		return MySQLDataListScopeT<T>(queryDataListByColumnInt(column, value, maxCount));
	}
	MySQLDataListScopeT<T> queryDataListByColumnStringT(const int column, const string& value)
	{
		return MySQLDataListScopeT<T>(queryDataListByColumnString(column, value));
	}
	MySQLDataListScopeT<T> queryDataListByIDRangeT(const llong minID, const llong maxID)
	{
		return MySQLDataListScopeT<T>(queryDataListByIDRange(minID, maxID));
	}
	MySQLDataScopeT<T> queryDataByIDT(llong instanceID)
	{
		return MySQLDataScopeT<T>(static_cast<T*>(queryDataByID(instanceID)));
	}
	MySQLDataScopeT<T> queryDataByColumnLLong2OrT(int column, llong value, int column1, llong value1)
	{
		return MySQLDataScopeT<T>(static_cast<T*>(queryDataByColumnLLong2Or(column, value, column1, value1)));
	}
	MySQLDataScopeT<T> queryDataByColumnLLongIntAndT(int column0, llong value0, int column1, int value1)
	{
		return MySQLDataScopeT<T>(static_cast<T*>(queryDataByColumnLLongIntAnd(column0, value0, column1, value1)));
	}
	MySQLDataScopeT<T> queryDataByColumnString2AndT(int column0, const string& value0, int column1, const string& value1)
	{
		return MySQLDataScopeT<T>(static_cast<T*>(queryDataByColumnString2And(column0, value0, column1, value1)));
	}
	MySQLDataScopeT<T> queryDataByColumnLLongStringAndT(int column0, llong value0, int column1, const string& value1)
	{
		return MySQLDataScopeT<T>(static_cast<T*>(queryDataByColumnLLongStringAnd(column0, value0, column1, value1)));
	}
	MySQLDataScopeT<T> queryDataByColumnIntT(int column, int value)
	{
		return MySQLDataScopeT<T>(static_cast<T*>(queryDataByColumnInt(column, value)));
	}
	MySQLDataScopeT<T> queryDataByColumnStringT(int column, const string& value)
	{
		return MySQLDataScopeT<T>(static_cast<T*>(queryDataByColumnString(column, value)));
	}
	MySQLDataScopeT<T> queryDataByColumnLLongT(int column, llong value)
	{
		return MySQLDataScopeT<T>(static_cast<T*>(queryDataByColumnLLong(column, value)));
	}
};
#endif