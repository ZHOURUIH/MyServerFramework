#pragma once

#ifdef _MYSQL
#include "FrameBase.h"
#include "MySQLData.h"
#include "QueryParam.h"
#include "MySQLDataScope.h"
#include "MySQLDataListScope.h"

class MICRO_LEGEND_FRAME_API MySQLTable
{
public:
	MySQLTable(const char* tableName);
	virtual ~MySQLTable();
	void setMySQL(MYSQL* mysql) { mMySQL = mysql; }
	virtual void init() {}
	virtual void lateInit() {}

	// 查询
	MySQLDataListScope queryAll(MySQLDataListScope& scope)
	{
		return MySQLDataListScope(queryAll());
	}
	MySQLDataScope queryDataByIDPtr(llong instanceID)
	{
		return MySQLDataScope(queryDataByID(instanceID));
	}
	MySQLDataListScope queryDataListByFullParamPtr(const QueryParam& param)
	{
		return MySQLDataListScope(queryDataListByFullParam(param));
	}
	bool queryDataExistByColumnString(int column, const string& value);
	bool queryDataExistByColumnLLongStringAnd(int column0, llong value0, int column1, const string& value1);
	// 查询指定ID的数据是否存在
	bool queryDataExistByID(llong instanceID);
	MySQLDataListScope queryDataListByColumnIntPtr(int column, int value, int maxCount)
	{
		return MySQLDataListScope(queryDataListByColumnInt(column, value, maxCount));
	}
	MySQLDataListScope queryDataListByColumnIntGreaterPtr(int column, int value)
	{
		return MySQLDataListScope(queryDataListByColumnIntGreater(column, value));
	}
	MySQLDataListScope queryDataListByColumnLLongPtr(int column, llong value, int maxCount)
	{
		return MySQLDataListScope(queryDataListByColumnLLong(column, value, maxCount));
	}
	MySQLDataListScope queryDataListByColumnLLongListPtr(int column, const Vector<llong>& idList)
	{
		return MySQLDataListScope(queryDataListByColumnLLongList(column, idList));
	}
	MySQLDataListScope queryDataListByColumnLLong2OrPtr(int column0, llong value0, int column1, llong value1, int maxCount)
	{
		return MySQLDataListScope(queryDataListByColumnLLong2Or(column0, value0, column1, value1, maxCount));
	}
	MySQLDataListScope queryDataListByColumnStringPtr(int column, const string& value)
	{
		return MySQLDataListScope(queryDataListByColumnString(column, value));
	}
	MySQLDataListScope queryDataListByColumnStringListPtr(int column, const Vector<string>& value)
	{
		return MySQLDataListScope(queryDataListByColumnStringList(column, value));
	}
	MySQLDataListScope queryDataListByIDRangePtr(llong minID, llong maxID)
	{
		return MySQLDataListScope(queryDataListByIDRange(minID, maxID));
	}
	MySQLDataListScope queryDataListByIDListPtr(const Vector<llong>& idList)
	{
		return MySQLDataListScope(queryDataListByIDList(idList));
	}
	MySQLDataListScope queryDataListByIDListPtr(Vector<llong>&& idList)
	{
		return MySQLDataListScope(queryDataListByIDList(move(idList)));
	}
	llong queryDataCount(const char* condition);

	// 删除
	bool deleteByID(llong instanceID) const;
	bool deleteByIDList(const Vector<llong>& idList) const { return deleteByIDList(idList.data(), idList.size()); }
	template<int Length>
	bool deleteByIDList(const ArrayList<Length, llong>& idList) const { return deleteByIDList(idList.data(), idList.size()); }
	bool deleteByIDList(const llong* idList, int idCount) const;
	bool deleteByColumnLLong(int column, llong value, bool onlyOnce) const;
	bool deleteByColumnInt(int column, int value, bool onlyOnce) const;
	bool deleteInIDRange(llong minID, llong maxID) const;
	bool deleteByColumnString(int column, const string& value, bool onlyOnce) const;
	bool deleteByColumnLLong2And(int column0, llong value0, int column1, llong value1, bool onlyOnce) const;
	bool deleteByColumnLLong2Or(int column0, llong value0, int column1, llong value1, bool onlyOnce) const;
	void deleteAll();
	
	// 插入
	bool insertData(const MySQLData* data, bool showError = true) const;

	// 更新
	bool updateData(const MySQLData* data, ullong flag) const;
	// 更新指定ID的数据的一个字符串字段
	bool updateString(llong id, int col, const char* paramValue) const;
	bool updateString(llong id, int col, const string& paramValue) const;
	bool updateString2(llong id, int col0, const string& paramValue0, int col1, const string& paramValue1) const;
	// 更新指定ID的数据的一个浮点数字段
	bool updateFloat(llong id, int col, float paramValue) const;
	// 更新bool
	bool updateBool(llong id, int col, bool paramValue) const;
	// 更新指定ID的数据的一个整数字段
	bool updateInt(llong id, int col, int paramValue) const;
	bool updateIntFloat(llong id, int col0, int paramValue0, int col1, float paramValue1) const;
	bool updateInt2(llong id, int column0, int value0, int column1, int value1) const;
	// 更新指定ID的数据的一个整数字段
	bool updateLLong(llong id, int col, llong paramValue) const;
	// 更新指定ID的数据的一个整数字段
	bool updateLLongInt(llong id, int column0, llong value0, int column1, int value1) const;
	bool updateLLongBool(llong id, int column0, llong value0, int column1, bool value1) const;
	// 更新整个表的一个整数字段
	bool updateAllString(int col, const string& paramValue) const;
	bool updateAllInt(int col, int paramValue) const;
	bool updateAllLLong(int col, llong paramValue) const;
	bool updateAllFloat(int col, float paramValue) const;
	bool updateAllBool(int col, bool paramValue) const;
	// 查询单条数据的一个整数字段
	int queryInt(llong id, int col);
	// 查询单条数据的一个浮点数字段
	float queryFloat(llong id, int col);
	// 查询单条数据的一个长整型字段
	llong queryULLong(llong id, int col);
	// 查询单条数据的一个字符串字段
	string queryString(llong id, int col);

	void addColName(const string& name) { mColumnNameList.push_back(name); mColumnIndexList.insert(name, mColumnNameList.size() - 1); }
	int getColCount() const { return mColumnNameList.size(); }
	const string& getColName(const int colIndex) const { return mColumnNameList[colIndex]; }
	int getColIndex(const string& name) const { return mColumnIndexList.tryGet(name, -1); }
	void addColToFlag(ullong& flag, int colIndex);
	void addColToFlag(ullong& flag, int* colIndex, int colCount);
	void notifyDataBaseConnected() { mMaxGUID = queryMaxGUID(); }
	llong getMaxGUID() const { return mMaxGUID; }
	void setMaxGUID(const llong maxGUID) { mMaxGUID = maxGUID; }
	llong generateMaxGUID() { return ++mMaxGUID; }
	void clearCache(llong id);
	void clearAllCache();
	virtual MySQLData* createData() = 0;
	virtual void createDataList(Vector<MySQLData*>& dataList, int count) = 0;
	virtual MySQLData* createPersistData() = 0;
	virtual void createPersistDataList(Vector<MySQLData*>& dataList, const int count) = 0;
	// 执行一条不需要获取任何数据的查询语句
	bool executeNonQuery(const char* str, bool showError = true, bool hideInfo = false, bool allowReconnect = true) const;
	// 执行一条需要查询数据的语句,返回查询到的数据
	MYSQL_RES* executeQuery(const char* str, bool allowReconnect = true) const;
	// 销毁数据查询对象
	void endQuery(MYSQL_RES* result);
protected:
	// 查询全部数据,需要手动使用MySQLDataBase::destroyData销毁获取到的数据对象
	Vector<MySQLData*> queryAll(bool createTempData = true);
	Vector<MySQLData*> queryDataListByFullParam(const QueryParam& param);
	MySQLData* queryDataByID(llong instanceID);
	MySQLData* queryDataByColumnInt(int column, int value);
	MySQLData* queryDataByColumnLLong(int column, llong value);
	MySQLData* queryDataByColumnString(int column, const string& value);
	MySQLData* queryDataByColumnString2And(int column0, const string& value0, int column1, const string& value1);
	MySQLData* queryDataByColumnLLong2Or(int column, llong value, int column1, llong value1);
	MySQLData* queryDataByColumnLLongIntAnd(int column0, llong value0, int column1, int value1);
	MySQLData* queryDataByColumnLLongStringAnd(int column0, llong value0, int column1, const string& value1);
	Vector<MySQLData*> queryDataListByColumnInt(int column, int value, int maxCount);
	Vector<MySQLData*> queryDataListByColumnIntGreater(int column, int value);
	Vector<MySQLData*> queryDataListByColumnLLong(int column, llong value, int maxCount);
	Vector<MySQLData*> queryDataListByColumnLLongList(int column, const Vector<llong>& idList);
	Vector<MySQLData*> queryDataListByColumnLLong2Or(int column0, llong value0, int column1, llong value1, int maxCount);
	Vector<MySQLData*> queryDataListByColumnString(int column, const string& value);
	Vector<MySQLData*> queryDataListByColumnStringList(int column, const Vector<string>& value);
	Vector<MySQLData*> queryDataListByIDRange(llong minID, llong maxID);
	// 不能保证查询结果与id列表顺序一致
	Vector<MySQLData*> queryDataListByIDList(const Vector<llong>& idList);
	// 不能保证查询结果与id列表顺序一致
	Vector<MySQLData*> queryDataListByIDList(Vector<llong>&& idList);
	// 查询
	bool queryDataExist(const char* condition);
	Vector<MySQLData*> queryDataList(const QueryParam& param, bool createTempData);
	MySQLData* queryData(const QueryParam& param);
	// 插入
	bool doInsert(const string& valueStr, bool showError = true) const;
	// 删除
	bool doDelete(const char* condition, bool onlyOnce) const;
	// 更新
	bool doUpdate(const char* updateStr, const char* condition, bool onlyOnce) const;
	bool doUpdate(const string& updateStr, const char* condition, bool onlyOnce) const;
	llong queryMaxGUID();
	MySQLData* mysqlToResultData(MYSQL_RES* result);
	void mysqlToResultData(MYSQL_RES* result, const Vector<MySQLData*>& dataList);
	const char* parseColPtr(MYSQL_RES* result, int colIndex) const;
	int parseColInt(MYSQL_RES* result, const int colIndex) const
	{
		if (result == nullptr)
		{
			return 0;
		}
		const char* row = parseColPtr(result, colIndex);
		return row != nullptr ? SToI(row) : 0;
	}
	float parseColFloat(MYSQL_RES* result, const int colIndex) const
	{
		if (result == nullptr)
		{
			return 0.0f;
		}
		const char* row = parseColPtr(result, colIndex);
		return row != nullptr ? SToF(row) : 0.0f;
	}
	llong parseColLLong(MYSQL_RES* result, const int colIndex) const
	{
		if (result == nullptr)
		{
			return 0;
		}
		const char* row = parseColPtr(result, colIndex);
		return row != nullptr ? SToLL(row) : 0;
	}
	string parseColString(MYSQL_RES* result, const int colIndex) const
	{
		if (result == nullptr)
		{
			return FrameDefine::EMPTY;
		}
		const char* row = parseColPtr(result, colIndex);
		return row != nullptr ? row : FrameDefine::EMPTY;
	}
protected:
	Vector<string> mColumnNameList;				// 表格列名的列表
	HashMap<string, short> mColumnIndexList;	// 根据列名查找列下标
	MySQLCacheTable* mCacheTable = nullptr;		// 表格缓存对象
	const char* mTableName = nullptr;			// 表格名
	MYSQL* mMySQL = nullptr;					// 数据库对象
	llong mMaxGUID = 0;							// 当前数据的最大ID
	static HashMap<int, char*> mTemp;			// 解析数据时临时使用的
};
#endif