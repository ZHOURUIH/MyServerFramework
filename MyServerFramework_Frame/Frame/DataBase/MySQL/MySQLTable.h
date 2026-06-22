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
	virtual void lateInit() { checkTableStructure(); }
	const char* getTableName() const { return mTableName; }

	// 查询
	MySQLDataListScope queryAll(MySQLDataListScope& scope)
	{
		return MySQLDataListScope(queryAll());
	}
	MySQLDataScope queryByIDPtr(llong instanceID)
	{
		return MySQLDataScope(queryByID(instanceID));
	}
	MySQLDataListScope queryListByFullParamPtr(const QueryParam& param)
	{
		return MySQLDataListScope(queryListByFullParam(param));
	}
	bool queryExistByString(int column, const string& value);
	bool queryExistByLLongStringAnd(int column0, llong value0, int column1, const string& value1);
	// 查询指定ID的数据是否存在
	bool queryExistByID(llong instanceID);
	// 这里的Ptr后缀是为了与模板的T后缀和不带Scope的版本做区分
	MySQLDataListScope queryListByIntPtr(int column, int value, int maxCount)
	{
		return MySQLDataListScope(queryListByInt(column, value, maxCount));
	}
	MySQLDataListScope queryListByIntGreaterPtr(int column, int value)
	{
		return MySQLDataListScope(queryListByIntGreater(column, value));
	}
	MySQLDataListScope queryListByLLongPtr(int column, llong value, int maxCount)
	{
		return MySQLDataListScope(queryListByLLong(column, value, maxCount));
	}
	MySQLDataListScope queryListByLLongListPtr(int column, const Vector<llong>& idList)
	{
		return MySQLDataListScope(queryListByLLongList(column, idList));
	}
	MySQLDataListScope queryListByLLong2OrPtr(int column0, llong value0, int column1, llong value1, int maxCount)
	{
		return MySQLDataListScope(queryListByLLong2Or(column0, value0, column1, value1, maxCount));
	}
	MySQLDataListScope queryListByStringPtr(int column, const string& value)
	{
		return MySQLDataListScope(queryListByString(column, value));
	}
	MySQLDataListScope queryListByStringListPtr(int column, const Vector<string>& value)
	{
		return MySQLDataListScope(queryListByStringList(column, value));
	}
	MySQLDataListScope queryListByIDRangePtr(llong minID, llong maxID)
	{
		return MySQLDataListScope(queryListByIDRange(minID, maxID));
	}
	MySQLDataListScope queryListByIDListPtr(const Vector<llong>& idList)
	{
		return MySQLDataListScope(queryListByIDList(idList));
	}
	MySQLDataListScope queryListByIDListPtr(Vector<llong>&& idList)
	{
		return MySQLDataListScope(queryListByIDList(move(idList)));
	}
	llong queryCount(const char* condition);
	int queryMaxInt(int column);
	llong queryMaxLLong(int column);
	void queryAllID(Vector<llong>& idList);

	// 删除
	bool deleteByID(llong instanceID) const;
	bool deleteByIDList(const Vector<llong>& idList) const { return deleteByIDList(idList.data(), idList.size()); }
	template<int Length>
	bool deleteByIDList(const ArrayList<Length, llong>& idList) const { return deleteByIDList(idList.data(), idList.size()); }
	bool deleteByIDList(const llong* idList, int idCount) const;
	bool deleteByLLong(int column, llong value, bool onlyOnce) const;
	bool deleteByInt(int column, int value, bool onlyOnce) const;
	bool deleteInIDRange(llong minID, llong maxID) const;
	bool deleteByString(int column, const string& value, bool onlyOnce) const;
	bool deleteByLLong2And(int column0, llong value0, int column1, llong value1, bool onlyOnce) const;
	bool deleteByLLongIntAnd(int column0, llong value0, int column1, int value1, bool onlyOnce) const;
	bool deleteByLLong2Or(int column0, llong value0, int column1, llong value1, bool onlyOnce) const;
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
	HashMap<llong, int> queryIntList(const Vector<llong>& id, int col);
	HashMap<llong, int> queryIntList(int col);
	// 查询单条数据的一个浮点数字段
	float queryFloat(llong id, int col);
	HashMap<llong, float> queryFloatList(const Vector<llong>& id, int col);
	HashMap<llong, float> queryFloatList(int col);
	// 查询单条数据的一个长整型字段
	llong queryLLong(llong id, int col);
	HashMap<llong, llong> queryLLongList(const Vector<llong>& id, int col);
	HashMap<llong, llong> queryLLongList(int col);
	// 查询单条数据的一个字符串字段
	string queryString(llong id, int col);
	HashMap<llong, string> queryStringList(const Vector<llong>& id, int col);
	HashMap<llong, string> queryStringList(int col);

	void addColName(const string& name) 
	{
		mColumnNameList.add(name);
		mColumnIndexList.add(name, mColumnNameList.size() - 1); 
	}
	int getColCount() const { return mColumnNameList.size(); }
	const string& getColName(const int colIndex) const { return mColumnNameList[colIndex]; }
	int getColIndex(const string& name) const { return mColumnIndexList.get(name, -1); }
	ullong generateColumnFlag(int colIndex);
	ullong generateColumnFlag(short* colIndex, int colCount);
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
	void endQuery(MYSQL_RES* result) const;
protected:
	// 查询全部数据,需要手动使用MySQLDataBase::destroyData销毁获取到的数据对象
	Vector<MySQLData*> queryAll(bool createTempData = true);
	Vector<MySQLData*> queryAllNoCache(bool createTempData = true);
	// 仅查询全表的指定字段
	Vector<MySQLData*> queryAllPartial(const Vector<string>& columns, bool createTempData = true);
	Vector<MySQLData*> queryListByFullParam(const QueryParam& param);
	MySQLData* queryByID(llong instanceID);
	MySQLData* queryByInt(int column, int value);
	MySQLData* queryByLLong(int column, llong value);
	MySQLData* queryByString(int column, const string& value);
	MySQLData* queryByString2And(int column0, const string& value0, int column1, const string& value1);
	MySQLData* queryByLLong2Or(int column, llong value, int column1, llong value1);
	MySQLData* queryByLLongIntAnd(int column0, llong value0, int column1, int value1);
	MySQLData* queryByLLongStringAnd(int column0, llong value0, int column1, const string& value1);
	Vector<MySQLData*> queryListByInt(int column, int value, int maxCount);
	Vector<MySQLData*> queryListByIntGreater(int column, int value);
	Vector<MySQLData*> queryListByLLong(int column, llong value, int maxCount);
	Vector<MySQLData*> queryListByLLongList(int column, const Vector<llong>& idList);
	Vector<MySQLData*> queryListByLLong2Or(int column0, llong value0, int column1, llong value1, int maxCount);
	Vector<MySQLData*> queryListByString(int column, const string& value);
	Vector<MySQLData*> queryListByStringList(int column, const Vector<string>& value);
	Vector<MySQLData*> queryListByIDRange(llong minID, llong maxID);
	// 不能保证查询结果与id列表顺序一致
	Vector<MySQLData*> queryListByIDList(const Vector<llong>& idList);
	// 不能保证查询结果与id列表顺序一致
	Vector<MySQLData*> queryListByIDList(Vector<llong>&& idList);
	// 查询
	bool queryExist(const char* condition);
	Vector<MySQLData*> queryList(const QueryParam& param, bool createTempData);
	MySQLData* query(const QueryParam& param);
	// 插入
	bool doInsert(const string& valueStr, bool showError = true) const;
	// 删除
	bool doDelete(const char* condition, bool onlyOnce) const;
	// 更新
	bool doUpdate(const char* updateStr, const char* condition, bool onlyOnce) const;
	bool doUpdate(const string& updateStr, const char* condition, bool onlyOnce) const;
	llong queryMaxGUID();
	// 检查数据库表格结构是否与代码注册的结构一致
	void checkTableStructure() const;
	MySQLData* mysqlToResultData(MYSQL_RES* result);
	void mysqlToResultData(MYSQL_RES* result, const Vector<MySQLData*>& dataList) const;
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
	atomic<llong> mMaxGUID = 0;					// 当前数据的最大ID,由于有时候会在mysql线程中修改此变量,所以使用原子类型
	static HashMap<int, char*> mTemp;			// 解析数据时临时使用的
};
#endif