#ifdef _MYSQL
#include "FrameHeader.h"

HashMap<int, char*> MySQLTable::mTemp(10000);

MySQLTable::MySQLTable(const char* tableName):
	mTableName(tableName)
{
	mCacheTable = new MySQLCacheTable(this);
}

MySQLTable::~MySQLTable()
{
	mCacheTable->destroy();
	DELETE(mCacheTable);
}

void MySQLTable::clearCache(const llong id)
{
	mCacheTable->deleteData(id);
}

void MySQLTable::clearAllCache()
{
	mCacheTable->deleteAllData();
}

bool MySQLTable::queryExistByString(const int column, const string& value)
{
	// 先查询缓存中有没有
	if (mCacheTable->getDataByString(column, value) != nullptr)
	{
		return true;
	}

	string condition;
	sqlConditionString(condition, getColName(column), value);
	return queryExist(condition.c_str());
}

bool MySQLTable::queryExistByLLongStringAnd(const int column0, llong value0, const int column1, const string& value1)
{
	// 先查询缓存中有没有
	if (mCacheTable->getDataByLLongStringAnd(column0, value0, column1, value1) != nullptr)
	{
		return true;
	}

	string condition;
	sqlConditionLLong(condition, getColName(column0), value0);
	sqlConditionString(condition, getColName(column1), value1);
	return queryExist(condition.c_str());
}

// 查询指定ID的数据是否存在
bool MySQLTable::queryExistByID(const llong instanceID)
{
	// 先查询缓存中有没有
	if (mCacheTable->getCacheData(instanceID) != nullptr)
	{
		return true;
	}

	MyString<32> condition;
	sqlConditionLLong(condition, "ID", instanceID);
	return queryExist(condition.str());
}

Vector<MySQLData*> MySQLTable::queryAll(const bool createTempData)
{
	Vector<MySQLData*> dataList = queryAllNoCache(createTempData);
	mCacheTable->addOrUpdateDataList(dataList);
	return dataList;
}

Vector<MySQLData*> MySQLTable::queryAllNoCache(const bool createTempData)
{
	QueryParam param;
	param.mQueryAll = true;
	return queryList(param, createTempData);
}

Vector<MySQLData*> MySQLTable::queryAllPartial(const Vector<string>& columns, bool createTempData)
{
	// 仅查询部分字段的数据就不能进入缓存
	QueryParam param;
	param.mQueryAll = true;
	param.mColumns = new Vector<string>();
	param.mColumns->addRangeCopy(columns);
	return queryList(param, createTempData);
}

Vector<MySQLData*> MySQLTable::queryListByFullParam(const QueryParam& param)
{
	// 全表查询不走缓存
	Vector<MySQLData*> dataList = queryList(param, true);
	// 更新缓存,只有非部分字段查询时才会更新缓存
	if (param.mColumns == nullptr || param.mColumns->isEmpty())
	{
		mCacheTable->addOrUpdateDataList(dataList);
	}
	return dataList;
}

bool MySQLTable::queryExist(const char* condition)
{
	if (condition == nullptr || condition[0] == '\0')
	{
		return false;
	}
	const llong time0 = getRealTimeMS();
	MyString<512> queryStr;
	queryStr.add("SELECT 1 FROM ", mTableName, " WHERE ", condition, " LIMIT 1");
	// 获得查询结果
	const int count = parseColInt(QueryScope(this, queryStr.str()).mResult, 0);
	const llong time1 = getRealTimeMS();
	if (time1 - time0 >= 5)
	{
		LOG("查询数据是否存在耗时:" + IToS((int)(time1 - time0)) + ", sql:" + queryStr.str());
	}
	return count > 0;
}

Vector<MySQLData*> MySQLTable::queryList(const QueryParam& param, const bool createTempData)
{
	const llong time0 = readTSC();
	// 查询条件,如果查询条件较短,则使用栈内存,较长则使用堆内存
	string heapWhereStr;
	MyString<1024> stackWhereStr;
	if (!param.mConditionStack.isEmpty())
	{
		stackWhereStr.add(" WHERE ", param.mConditionStack.str());
	}
	else if (!param.mConditionHeap.empty())
	{
		if (param.mConditionHeap.length() < 512)
		{
			stackWhereStr.add(" WHERE ", param.mConditionHeap.c_str());
		}
		else
		{
			heapWhereStr.reserve(1024);
			heapWhereStr += " WHERE ";
			heapWhereStr += param.mConditionHeap;
		}
	}
	else if (param.mIDList != nullptr && !param.mIDList->isEmpty())
	{
		const int idCount = param.mIDList->size();
		constexpr int MAX_ID_COUNT = 64;
		if (idCount <= MAX_ID_COUNT)
		{
			LLONGS_STR(condition, param.mIDList->data(), MAX_ID_COUNT, idCount);
			stackWhereStr.add(" WHERE ID in (", condition.str(), ")");
		}
		else
		{
			heapWhereStr.reserve(1024);
			heapWhereStr += " WHERE ID in (";
			heapWhereStr += LLsToS(*param.mIDList);
			heapWhereStr += ")";
		}
	}
	// 既没有指定条件,也没有指定ID,则看是否确定要查询所有数据
	else if (!param.mQueryAll)
	{
		ERROR("未指定查询条件,且不是查询全部数据");
		return {};
	}

	// 限定查找数量
	MyString<64> limitStr;
	if (param.mLimitCount > 0)
	{
		LLONG_STR(limitStartStr, param.mLimitStart);
		LLONG_STR(limitCountStr, param.mLimitCount);
		limitStr.add(" LIMIT ", limitStartStr.str(), ",", limitCountStr.str());
	}
	// 如果只查一个ID,则限定只需要一个结果
	else if (param.mIDList != nullptr && param.mIDList->size() == 1)
	{
		limitStr.set(" LIMIT 1");
	}

	// 排序方式
	MyString<64> orderStr;
	if (!param.mOrderColumn.empty())
	{ 
		if (param.mOrder == MYSQL_ORDER::ASC)
		{
			orderStr.add(" ORDER BY ", param.mOrderColumn.c_str(), " ASC");
		}
		else if (param.mOrder == MYSQL_ORDER::DESC)
		{
			orderStr.add(" ORDER BY ", param.mOrderColumn.c_str(), " DESC");
		}
	}

	string selectContent = "*";
	if (param.mColumns != nullptr && !param.mColumns->isEmpty())
	{
		selectContent = "";
		for (const string& col : *param.mColumns)
		{
			selectContent += col + ",";
		}
		removeLastComma(selectContent);
	}

	// 生成最终的查询语句
	string heapQueryStr;
	MyString<2048> stackQueryStr;
	// 如果where用的是栈内存
	if (!stackWhereStr.isEmpty())
	{
		// 由于使用了自定义的缓存,为了避免条件删除时由于有要判断的列没有查询而导致未能将缓存中的数据删除,然后导致缓存与数据库不一致
		// 所以在查询时都是查询全部列的数据
		stackQueryStr.add("SELECT ", selectContent.c_str(), " FROM ", mTableName);
		stackQueryStr.add(stackWhereStr.str(), orderStr.str(), limitStr.str());
	}
	// 如果where使用的是堆内存
	else if (!heapWhereStr.empty())
	{
		heapQueryStr.reserve(2048);
		heapQueryStr += string("SELECT ");
		heapQueryStr += selectContent;
		heapQueryStr += string(" FROM ");
		heapQueryStr += mTableName;
		heapQueryStr += heapWhereStr;
		heapQueryStr += orderStr.str();
		heapQueryStr += limitStr.str();
	}
	// 没有where条件
	else
	{
		stackQueryStr.add("SELECT ", selectContent.c_str(), " FROM ", mTableName, orderStr.str(), limitStr.str());
	}

	// 获得查询结果
	QueryScope scope(this);
	MYSQL_RES* result = nullptr;
	if (!stackQueryStr.isEmpty())
	{
		result = scope.query(stackQueryStr.str());
	}
	else if (!heapQueryStr.empty())
	{
		result = scope.query(heapQueryStr.c_str());
	}
	Vector<MySQLData*> dataList;
	const bool ret = result != nullptr && result->row_count > 0;
	if (ret)
	{
		if (createTempData)
		{
			createDataList(dataList, (int)result->row_count);
		}
		else
		{
			createPersistDataList(dataList, (int)result->row_count);
		}
		mysqlToResultData(result, dataList);
	}
	const llong time1 = readTSC();
	const double timeMS = Profiler::ticksToMS(time1 - time0);
	if (ret && timeMS > 5)
	{
		LOG("查询数据的耗时较长:" + FToS((float)timeMS) + "毫秒, sql:" + (!stackQueryStr.isEmpty() ? stackQueryStr.str() : heapQueryStr.c_str()));
	}
	return dataList;
}

void MySQLTable::queryAllID(Vector<llong>& idList)
{
	idList.clear();
	MyString<256> sql;
	sql.add("SELECT ID FROM ", mTableName);
	QueryScope scope(this, sql.str());
	MYSQL_RES* result = scope.mResult;
	if (result == nullptr)
	{
		return;
	}
	idList.reserve((int)result->row_count);
	while (true)
	{
		const MYSQL_ROW row = mysql_fetch_row(result);
		if (row == nullptr)
		{
			break;
		}
		if (row[0] != nullptr)
		{
			idList.add(SToLL(row[0]));
		}
	}
}

MySQLData* MySQLTable::queryByInt(const int column, const int value)
{
	if (MySQLData* cacheData = mCacheTable->getDataByInt(column, value))
	{
		MySQLData* data = createData();
		cacheData->cloneTo(data);
		return data;
	}

	// 缓存未命中
	QueryParam param;
	sqlConditionInt(param.mConditionStack, getColName(column), value);
	MySQLData* data = query(param);
	if (data != nullptr)
	{
		// 更新缓存
		mCacheTable->addOrUpdateData(data);
	}
	return data;
}

MySQLData* MySQLTable::queryByLLong(const int column, const llong value)
{
	if (MySQLData* cacheData = mCacheTable->getDataByLLong(column, value))
	{
		MySQLData* data = createData();
		cacheData->cloneTo(data);
		return data;
	}

	// 缓存未命中
	QueryParam param;
	sqlConditionLLong(param.mConditionStack, getColName(column), value);
	MySQLData* data = query(param);
	if (data != nullptr)
	{
		// 更新缓存
		mCacheTable->addOrUpdateData(data);
	}
	return data;
}

MySQLData* MySQLTable::queryByString(const int column, const string& value)
{
	if (MySQLData* cacheData = mCacheTable->getDataByString(column, value))
	{
		MySQLData* data = createData();
		cacheData->cloneTo(data);
		return data;
	}

	// 缓存未命中
	QueryParam param;
	sqlConditionString(param.mConditionHeap, getColName(column), value);
	MySQLData* data = query(param);
	if (data != nullptr)
	{
		// 更新缓存
		mCacheTable->addOrUpdateData(data);
	}
	return data;
}

MySQLData* MySQLTable::queryByString2And(const int column0, const string& value0, const int column1, const string& value1)
{
	if (MySQLData* cacheData = mCacheTable->getDataByString2And(column0, value0, column1, value1))
	{
		MySQLData* data = createData();
		cacheData->cloneTo(data);
		return data;
	}

	// 缓存未命中
	QueryParam param;
	string condition;
	sqlConditionString(param.mConditionHeap, getColName(column0), value0, "=", "&&");
	sqlConditionString(param.mConditionHeap, getColName(column1), value1);
	MySQLData* data = query(param);
	if (data != nullptr)
	{
		// 更新缓存
		mCacheTable->addOrUpdateData(data);
	}
	return data;
}

MySQLData* MySQLTable::queryByLLong2Or(const int column0, const llong value0, const int column1, const llong value1)
{
	// 先查缓存里面有没有
	if (MySQLData* cacheData = mCacheTable->getDataByLLong2Or(column0, value0, column1, value1))
	{
		MySQLData* data = createData();
		cacheData->cloneTo(data);
		return data;
	}

	QueryParam param;
	sqlConditionLLong(param.mConditionStack, getColName(column0), value0, "=", "||");
	sqlConditionLLong(param.mConditionStack, getColName(column1), value1);
	MySQLData* data = query(param);
	if (data != nullptr)
	{
		// 更新缓存
		mCacheTable->addOrUpdateData(data);
	}
	return data;
}

MySQLData* MySQLTable::queryByLLongIntAnd(const int column0, const llong value0, const int column1, const int value1)
{
	// 先查询缓存中有没有
	if (MySQLData* cacheData = mCacheTable->getDataByLLongIntAnd(column0, value0, column1, value1))
	{
		MySQLData* data = createData();
		cacheData->cloneTo(data);
		return data;
	}

	QueryParam param;
	sqlConditionLLong(param.mConditionStack, getColName(column0), value0, "=", "&&");
	sqlConditionInt(param.mConditionStack, getColName(column1), value1);
	MySQLData* data = query(param);
	if (data != nullptr)
	{
		// 更新缓存
		mCacheTable->addOrUpdateData(data);
	}
	return data;
}

MySQLData* MySQLTable::queryByLLongStringAnd(const int column0, const llong value0, const int column1, const string& value1)
{
	// 先查询缓存中有没有
	if (MySQLData* cacheData = mCacheTable->getDataByLLongStringAnd(column0, value0, column1, value1))
	{
		MySQLData* data = createData();
		cacheData->cloneTo(data);
		return data;
	}

	QueryParam param;
	sqlConditionLLong(param.mConditionHeap, getColName(column0), value0, "=", "&&");
	sqlConditionString(param.mConditionHeap, getColName(column1), value1);
	MySQLData* data = query(param);
	if (data != nullptr)
	{
		// 更新缓存
		mCacheTable->addOrUpdateData(data);
	}
	return data;
}

Vector<MySQLData*> MySQLTable::queryListByInt(const int column, const int value, const int maxCount)
{
	// 全表查询就不再查缓存了
	QueryParam param;
	sqlConditionInt(param.mConditionStack, getColName(column), value);
	param.mLimitCount = maxCount;
	Vector<MySQLData*> dataList = queryList(param, true);
	// 更新缓存
	if (!dataList.isEmpty())
	{
		mCacheTable->addOrUpdateDataList(dataList);
	}
	return dataList;
}

Vector<MySQLData*> MySQLTable::queryListByIntGreater(const int column, const int value)
{
	// 全表查询就不再查缓存了
	QueryParam param;
	sqlConditionInt(param.mConditionStack, getColName(column), value, ">");
	Vector<MySQLData*> dataList = queryList(param, true);
	// 更新缓存
	if (!dataList.isEmpty())
	{
		mCacheTable->addOrUpdateDataList(dataList);
	}
	return dataList;
}

Vector<MySQLData*> MySQLTable::queryListByLLong(const int column, const llong value, const int maxCount)
{
	// 全表查询就不再查缓存了
	QueryParam param;
	sqlConditionLLong(param.mConditionStack, getColName(column), value);
	param.mLimitCount = maxCount;
	Vector<MySQLData*> dataList = queryList(param, true);
	// 更新缓存
	if (!dataList.isEmpty())
	{
		mCacheTable->addOrUpdateDataList(dataList);
	}
	return dataList;
}

Vector<MySQLData*> MySQLTable::queryListByLLongList(const int column, const Vector<llong>& idList)
{
	// 全表查询不走缓存
	QueryParam param;
	const int count = idList.size();
	FOR(count)
	{
		if (i != count - 1)
		{
			sqlConditionLLong(param.mConditionHeap, getColName(column), idList[i], "=", "||");
		}
		else
		{
			sqlConditionLLong(param.mConditionHeap, getColName(column), idList[i]);
		}
	}
	Vector<MySQLData*> dataList = queryList(param, true);
	if (!dataList.isEmpty())
	{
		// 更新缓存
		mCacheTable->addOrUpdateDataList(dataList);
	}
	return dataList;
}

Vector<MySQLData*> MySQLTable::queryListByLLong2Or(const int column0, const llong value0, const int column1, const llong value1, const int maxCount)
{
	// 全表查询就不再查缓存了
	QueryParam param;
	sqlConditionLLong(param.mConditionStack, getColName(column0), value0, "=", "||");
	sqlConditionLLong(param.mConditionStack, getColName(column1), value1);
	param.mLimitCount = maxCount;
	Vector<MySQLData*> dataList = queryList(param, true);
	// 更新缓存
	if (!dataList.isEmpty())
	{
		mCacheTable->addOrUpdateDataList(dataList);
	}
	return dataList;
}

Vector<MySQLData*> MySQLTable::queryListByString(const int column, const string& value)
{
	// 全表查询就不再查缓存了
	QueryParam param;
	sqlConditionString(param.mConditionHeap, getColName(column), value);
	Vector<MySQLData*> dataList = queryList(param, true);
	// 更新缓存
	if (!dataList.isEmpty())
	{
		mCacheTable->addOrUpdateDataList(dataList);
	}
	return dataList;
}

Vector<MySQLData*> MySQLTable::queryListByStringList(const int column, const Vector<string>& idList)
{
	// 全表查询不走缓存
	QueryParam param;
	const int count = idList.size();
	FOR(count)
	{
		if (i != count - 1)
		{
			sqlConditionString(param.mConditionHeap, getColName(column), idList[i], "=", "||");
		}
		else
		{
			sqlConditionString(param.mConditionHeap, getColName(column), idList[i]);
		}
	}

	Vector<MySQLData*> dataList = queryList(param, true);
	if (!dataList.isEmpty())
	{
		// 更新缓存
		mCacheTable->addOrUpdateDataList(dataList);
	}
	return dataList;
}

Vector<MySQLData*> MySQLTable::queryListByIDRange(const llong minID, const llong maxID)
{
	// 全表查询就不再查缓存了
	QueryParam param;
	sqlConditionLLong(param.mConditionStack, "ID", minID, ">=", "&&");
	sqlConditionLLong(param.mConditionStack, "ID", maxID, "<=");
	Vector<MySQLData*> dataList = queryList(param, true);
	// 更新缓存
	if (!dataList.isEmpty())
	{
		mCacheTable->addOrUpdateDataList(dataList);
	}
	return dataList;
}

Vector<MySQLData*> MySQLTable::queryListByIDList(const Vector<llong>& idList)
{
	Vector<MySQLData*> dataList;
	Vector<llong> tempIDList;
	// 显式调用克隆,避免移动构造检查的误提示
	idList.cloneTo(tempIDList);
	// 先从缓存中查询
	FOR_VECTOR_INVERSE(tempIDList)
	{
		if (MySQLData* cacheData = mCacheTable->getCacheData(tempIDList[i]))
		{
			MySQLData* data = createData();
			cacheData->cloneTo(data);
			dataList.add(data);
			tempIDList.removeAt(i);
		}
	}
	if (tempIDList.isEmpty())
	{
		return dataList;
	}

	// 还有在缓存中查不到的,则从数据库查询
	QueryParam param;
	param.mIDList = new Vector<llong>();
	*param.mIDList = move(tempIDList);
	Vector<MySQLData*> newList = queryList(param, true);
	if (!newList.isEmpty())
	{
		// 更新缓存
		mCacheTable->addOrUpdateDataList(newList);
		dataList.addRange(move(newList));
	}
	return dataList;
}

Vector<MySQLData*> MySQLTable::queryListByIDList(Vector<llong>&& idList)
{
	Vector<MySQLData*> dataList;
	Vector<llong> tempIDList = move(idList);
	// 先从缓存中查询
	FOR_VECTOR_INVERSE(tempIDList)
	{
		if (MySQLData* cacheData = mCacheTable->getCacheData(tempIDList[i]))
		{
			MySQLData* data = createData();
			cacheData->cloneTo(data);
			dataList.add(data);
			tempIDList.removeAt(i);
		}
	}
	if (tempIDList.isEmpty())
	{
		return dataList;
	}

	// 还有在缓存中查不到的,则从数据库查询
	QueryParam param;
	param.mIDList = new Vector<llong>();
	*param.mIDList = move(tempIDList);
	Vector<MySQLData*> newList = queryList(param, true);
	if (!newList.isEmpty())
	{
		// 更新缓存
		mCacheTable->addOrUpdateDataList(newList);
		dataList.addRange(move(newList));
	}
	return dataList;
}

MySQLData* MySQLTable::queryByID(const llong instanceID)
{
	if (MySQLData* cacheData = mCacheTable->getCacheData(instanceID))
	{
		MySQLData* data = createData();
		cacheData->cloneTo(data);
		return data;
	}

	// 缓存未命中
	QueryParam param;
	param.mIDList = new Vector<llong>();
	param.mIDList->add(instanceID);
	// 查询失败
	MySQLData* data = query(param);
	if (data != nullptr)
	{
		// 查询成功,更新缓存
		mCacheTable->addOrUpdateData(data);
	}
	return data;
}

MySQLData* MySQLTable::query(const QueryParam& param)
{
	// 查询条件,如果查询条件较短,则使用栈内存,较长则使用堆内存
	string heapWhereStr;
	MyString<1024> stackWhereStr;
	if (!param.mConditionStack.isEmpty())
	{
		stackWhereStr.add(" WHERE ", param.mConditionStack.str());
	}
	else if (!param.mConditionHeap.empty())
	{
		if (param.mConditionHeap.length() < 512)
		{
			stackWhereStr.add(" WHERE ", param.mConditionHeap.c_str());
		}
		else
		{
			heapWhereStr.reserve(1024);
			heapWhereStr += " WHERE ";
			heapWhereStr += param.mConditionHeap;
		}
	}
	else if (param.mIDList != nullptr && !param.mIDList->isEmpty())
	{
		const int idCount = param.mIDList->size();
		constexpr int MAX_ID_COUNT = 64;
		if (idCount <= MAX_ID_COUNT)
		{
			LLONGS_STR(condition, param.mIDList->data(), MAX_ID_COUNT, idCount);
			stackWhereStr.add(" WHERE ID in (", condition.str(), ")");
		}
		else
		{
			heapWhereStr += " WHERE ID in (";
			heapWhereStr += LLsToS(*param.mIDList);
			heapWhereStr += ")";
		}
	}
	// 既没有指定条件,也没有指定ID,则看是否确定要查询所有数据
	else if (!param.mQueryAll)
	{
		ERROR("未指定查询条件,且不是查询全部数据");
		return nullptr;
	}

	// 生成最终的查询语句
	string heapQueryStr;
	MyString<2048> stackQueryStr;
	// 如果where用的是栈内存
	if (!stackWhereStr.isEmpty())
	{
		stackQueryStr.add("SELECT * FROM ", mTableName, stackWhereStr.str(), " LIMIT 1");
	}
	// 如果where使用的是堆内存
	else if (!heapWhereStr.empty())
	{
		heapQueryStr += "SELECT * FROM ";
		heapQueryStr += mTableName;
		heapQueryStr += heapWhereStr;
		heapQueryStr += " LIMIT 1";
	}
	// 没有where条件
	else
	{
		stackQueryStr.add("SELECT * FROM ", mTableName, " LIMIT 1");
	}

	// 获得查询结果
	MySQLData* data = nullptr;
	if (!stackQueryStr.isEmpty())
	{
		data = mysqlToResultData(QueryScope(this, stackQueryStr.str()).mResult);
	}
	else if (!heapQueryStr.empty())
	{
		data = mysqlToResultData(QueryScope(this, heapQueryStr.c_str()).mResult);
	}
	return data;
}

llong MySQLTable::queryMaxGUID()
{
	// 首先查询表中的最大GUID,用来生成物品的GUID
	MyString<64> queryStr;
	queryStr.add("SELECT max(ID) FROM ", mTableName);
	// 获得查询结果
	return clampZero(parseColLLong(QueryScope(this, queryStr.str()).mResult, 0));
}

void MySQLTable::checkTableStructure() const
{
	MyString<64> queryStr;
	queryStr.add("DESCRIBE ", mTableName);
	// 获得查询结果
	QueryScope scope(this, queryStr.str());

	Vector<string> colsInTable;
	while (true)
	{
		const MYSQL_ROW sql_row = mysql_fetch_row(scope.mResult);
		if (sql_row == nullptr)
		{
			break;
		}
		// 重置列的下标
		mysql_field_seek(scope.mResult, 0);
		FOR(mysql_num_fields(scope.mResult))
		{
			if (strcmp(mysql_fetch_field(scope.mResult)->name, "Field") == 0)
			{
				colsInTable.add(sql_row[i]);
				break;
			}
		}
	}
	if (mColumnNameList.size() != colsInTable.size())
	{
		ERROR("代码中已经注册的表格字段数量与数据库中的字段数量不一致,已注册:" + IToS(mColumnNameList.size()) + "个,查询到:" + IToS(colsInTable.size()) + ", 表格:" + mTableName);
		return;
	}
	for (const string& col : colsInTable)
	{
		if (!mColumnNameList.contains(col))
		{
			ERROR("数据库字段未注册:" + col + ", 表格:" + mTableName);
		}
	}
	for (const string& colName : mColumnNameList)
	{
		if (!colsInTable.remove(colName))
		{
			ERROR("已注册的表格字段在数据库中找不到:" + colName + ", 表格:" + mTableName);
		}
	}
}

ullong MySQLTable::generateColumnFlag(const int colIndex)
{
	if (colIndex < 0)
	{
		return 0;
	}
	ullong flag = 0;
	setBitOne(flag, colIndex);
	return flag;
}

ullong MySQLTable::generateColumnFlag(short* colIndex, const int colCount)
{
	ullong flag = 0;
	FOR(colCount)
	{
		if (colIndex[i] < 0)
		{
			continue;
		}
		setBitOne(flag, colIndex[i]);
	}
	return flag;
}

bool MySQLTable::deleteByID(const llong instanceID) const
{
	// 先从缓存里删除
	mCacheTable->deleteData(instanceID);

	MyString<32> condition;
	sqlConditionLLong(condition, "ID", instanceID);
	return doDelete(condition.str(), true);
}

bool MySQLTable::deleteByIDList(const llong* idList, const int idCount) const
{
	if (idCount <= 0)
	{
		return true;
	}
	// 从缓存里删除
	FOR(idCount)
	{
		mCacheTable->deleteData(idList[i]);
	}

	if (idCount < 16)
	{
		MyString<512> idListStr;
		LLsToS(idListStr, idList, idCount);
		MyString<512> condition;
		condition.add("ID in (", idListStr.str(), ")");
		return doDelete(condition.str(), false);
	}
	else
	{
		bool result = true;
		// 如果超过了5000个,也要分批次删除
		constexpr int BATCH_SIZE = 5000;
		FOR(generateBatchCount(idCount, BATCH_SIZE))
		{
			const int batchSize = generateBatchSize(idCount, BATCH_SIZE, i);
			string condition;
			condition.reserve(32 + batchSize * 10);
			condition += "ID in (";
			condition += LLsToS(idList + i * BATCH_SIZE, batchSize);
			condition += ")";
			result = doDelete(condition.c_str(), false) && result;
		}
		return result;
	}
}

bool MySQLTable::deleteByLLong(const int column, const llong value, const bool onlyOnce) const
{
	// 先从缓存删除
	mCacheTable->deleteDataByColumnLLong(column, value, onlyOnce);

	MyString<64> condition;
	sqlConditionLLong(condition, getColName(column), value);
	return doDelete(condition.str(), onlyOnce);
}

bool MySQLTable::deleteByInt(const int column, const int value, const bool onlyOnce) const
{
	// 先从缓存删除
	mCacheTable->deleteDataByColumnInt(column, value, onlyOnce);

	MyString<64> condition;
	sqlConditionInt(condition, getColName(column), value);
	return doDelete(condition.str(), onlyOnce);
}

bool MySQLTable::deleteInIDRange(const llong minID, const llong maxID) const
{
	// 先从缓存删除
	mCacheTable->deleteDataInIDRange(minID, maxID);

	MyString<64> condition;
	sqlConditionLLong(condition, "ID", minID, ">=", "&&");
	sqlConditionLLong(condition, "ID", maxID, "<=");
	return doDelete(condition.str(), false);
}

bool MySQLTable::deleteByString(const int column, const string& value, const bool onlyOnce) const
{
	// 从缓存删除
	mCacheTable->deleteDataByColumnString(column, value, onlyOnce);

	string condition;
	sqlConditionString(condition, getColName(column), value);
	return doDelete(condition.c_str(), onlyOnce);
}

bool MySQLTable::deleteByLLong2And(const int column0, const llong value0, const int column1, const llong value1, const bool onlyOnce) const
{
	// 先从缓存删除
	mCacheTable->deleteDataByColumnLLong2And(column0, value0, column1, value1, onlyOnce);

	MyString<128> condition;
	sqlConditionLLong(condition, getColName(column0), value0, "=", "&&");
	sqlConditionLLong(condition, getColName(column1), value1);
	return doDelete(condition.str(), onlyOnce);
}

bool MySQLTable::deleteByLLongIntAnd(const int column0, const llong value0, const int column1, const int value1, const bool onlyOnce) const
{
	// 先从缓存删除
	mCacheTable->deleteDataByColumnLLongIntAnd(column0, value0, column1, value1, onlyOnce);

	MyString<128> condition;
	sqlConditionLLong(condition, getColName(column0), value0, "=", "&&");
	sqlConditionInt(condition, getColName(column1), value1);
	return doDelete(condition.str(), onlyOnce);
}

bool MySQLTable::deleteByLLong2Or(const int column0, const llong value0, const int column1, const llong value1, const bool onlyOnce) const
{
	// 先从缓存删除
	mCacheTable->deleteDataByColumnLLong2Or(column0, value0, column1, value1, onlyOnce);

	MyString<128> condition;
	sqlConditionLLong(condition, getColName(column0), value0, "=", "||");
	sqlConditionLLong(condition, getColName(column1), value1);
	return doDelete(condition.str(), onlyOnce);
}

void MySQLTable::deleteAll()
{
	// 从缓存中删除
	mCacheTable->deleteAllData();

	executeNonQuery((string("DELETE FROM ") + mTableName).c_str());
}

bool MySQLTable::doDelete(const char* condition, const bool onlyOnce) const
{
	if (condition == nullptr || condition[0] == '\0')
	{
		return false;
	}
	string queryStr;
	queryStr.reserve(64 + strlength(condition));
	queryStr += "DELETE FROM ";
	queryStr += mTableName;
	queryStr += " WHERE ";
	queryStr += condition;
	if (onlyOnce)
	{
		queryStr += " LIMIT 1";
	}
	return executeNonQuery(queryStr.c_str());
}

bool MySQLTable::insertData(const MySQLData* data, const bool showError) const
{
	// 添加到缓存
	mCacheTable->addOrUpdateData(data);

	string valueStr;
	data->paramList(valueStr);
	return doInsert(valueStr, showError);
}

// 执行更新语句
bool MySQLTable::doUpdate(const char* updateStr, const char* condition, const bool onlyOnce) const
{
	if (updateStr == nullptr)
	{
		return false;
	}
	int reserveSize = 64 + strlength(updateStr);
	if (condition != nullptr && condition[0] != '\0')
	{
		reserveSize += strlength(condition);
	}
	string queryStr;
	queryStr.reserve(reserveSize);
	queryStr += "UPDATE ";
	queryStr += mTableName;
	queryStr += " SET ";
	queryStr += updateStr;
	if (condition != nullptr && condition[0] != '\0')
	{
		queryStr += " WHERE ";
		queryStr += condition;
	}
	if (onlyOnce)
	{
		queryStr += " LIMIT 1";
	}
	return executeNonQuery(queryStr.c_str());
}

bool MySQLTable::doUpdate(const string& updateStr, const char* condition, const bool onlyOnce) const
{
	if (updateStr.length() == 0)
	{
		return false;
	}
	int reserveSize = 64 + (int)updateStr.length();
	if (condition != nullptr && condition[0] != '\0')
	{
		reserveSize += strlength(condition);
	}
	string queryStr;
	queryStr.reserve(reserveSize);
	queryStr += "UPDATE ";
	queryStr += mTableName;
	queryStr += " SET ";
	queryStr += updateStr;
	if (condition != nullptr && condition[0] != '\0')
	{
		queryStr += " WHERE ";
		queryStr += condition;
	}
	if (onlyOnce)
	{
		queryStr += " LIMIT 1";
	}
	return executeNonQuery(queryStr.c_str());
}

bool MySQLTable::updateData(const MySQLData* data, const ullong flag) const
{
	// 更新缓存
	mCacheTable->addOrUpdateData(data, flag);

	string str;
	data->generateUpdate(str, flag);
	string queryStr;
	queryStr.reserve(64 + str.length());
	queryStr += "UPDATE ";
	queryStr += mTableName;
	queryStr += " SET ";
	queryStr += str;
	queryStr += " WHERE ID = ";
	LLONG_STR(idStr, data->mID);
	queryStr += idStr.str();
	return executeNonQuery(queryStr.c_str());
}

bool MySQLTable::doInsert(const string& valueStr, const bool showError) const
{
	string queryStr;
	queryStr.reserve(64 + valueStr.size());
	queryStr += "INSERT INTO ";
	queryStr += mTableName;
	queryStr += " VALUES(";
	queryStr += valueStr;
	queryStr += ")";
	return executeNonQuery(queryStr.c_str(), showError);
}

bool MySQLTable::updateString(const llong id, const int col, const char* paramValue) const
{
	// 更新缓存
	mCacheTable->updateString(id, paramValue, col);

	MyString<32> condition;
	sqlConditionLLong(condition, "ID", id);
	string updateStr;
	sqlUpdateString(updateStr, getColName(col), paramValue, false);
	return doUpdate(updateStr, condition.str(), true);
}

bool MySQLTable::updateString(const llong id, const int col, const string& paramValue) const
{
	// 更新缓存
	mCacheTable->updateString(id, paramValue, col);

	MyString<32> condition;
	sqlConditionLLong(condition, "ID", id);
	string updateStr;
	sqlUpdateString(updateStr, getColName(col), paramValue, false);
	return doUpdate(updateStr, condition.str(), true);
}

bool MySQLTable::updateString2(const llong id, const int col0, const string& paramValue0, const int col1, const string& paramValue1) const
{
	// 更新缓存
	mCacheTable->updateString2(id, paramValue0, col0, paramValue1, col1);

	MyString<32> condition;
	sqlConditionLLong(condition, "ID", id);
	string updateStr;
	sqlUpdateString(updateStr, getColName(col0), paramValue0);
	sqlUpdateString(updateStr, getColName(col1), paramValue1, false);
	return doUpdate(updateStr, condition.str(), true);
}

bool MySQLTable::updateFloat(const llong id, const int col, const float paramValue) const
{
	// 更新缓存
	mCacheTable->updateFloat(id, paramValue, col);

	MyString<32> condition;
	sqlConditionLLong(condition, "ID", id);
	MyString<64> updateStr;
	sqlUpdateFloat(updateStr, getColName(col), paramValue, false);
	return doUpdate(updateStr.str(), condition.str(), true);
}

bool MySQLTable::updateBool(const llong id, const int col, const bool paramValue) const
{
	// 更新缓存
	mCacheTable->updateBool(id, paramValue, col);

	MyString<32> condition;
	sqlConditionLLong(condition, "ID", id);
	MyString<64> updateStr;
	sqlUpdateBool(updateStr, getColName(col), paramValue, false);
	return doUpdate(updateStr.str(), condition.str(), true);
}

bool MySQLTable::updateInt(const llong id, const int col, const int paramValue) const
{
	// 更新缓存
	mCacheTable->updateInt(id, paramValue, col);

	MyString<32> condition;
	sqlConditionLLong(condition, "ID", id);
	MyString<64> updateStr;
	sqlUpdateInt(updateStr, getColName(col), paramValue, false);
	return doUpdate(updateStr.str(), condition.str(), true);
}

bool MySQLTable::updateIntFloat(const llong id, const int col0, const int paramValue0, const int col1, const float paramValue1) const
{
	// 更新缓存
	mCacheTable->updateInt(id, paramValue0, col0);
	mCacheTable->updateFloat(id, paramValue1, col1);

	MyString<32> condition;
	sqlConditionLLong(condition, "ID", id);
	MyString<64> updateStr;
	sqlUpdateInt(updateStr, getColName(col0), paramValue0);
	sqlUpdateFloat(updateStr, getColName(col1), paramValue1, false);
	return doUpdate(updateStr.str(), condition.str(), true);
}

bool MySQLTable::updateInt2(const llong id, const int column0, const int value0, const int column1, const int value1) const
{
	// 更新缓存
	mCacheTable->updateInt(id, value0, column0);
	mCacheTable->updateInt(id, value1, column1);

	MyString<32> condition;
	sqlConditionLLong(condition, "ID", id);
	MyString<64> updateStr;
	sqlUpdateInt(updateStr, getColName(column0), value0);
	sqlUpdateInt(updateStr, getColName(column1), value1, false);
	return doUpdate(updateStr.str(), condition.str(), true);
}

bool MySQLTable::updateLLong(const llong id, const int col, const llong paramValue) const
{
	// 更新缓存
	mCacheTable->updateLLong(id, paramValue, col);

	MyString<32> condition;
	sqlConditionLLong(condition, "ID", id);
	MyString<64> updateStr;
	sqlUpdateLLong(updateStr, getColName(col), paramValue, false);
	return doUpdate(updateStr.str(), condition.str(), true);
}

bool MySQLTable::updateLLongInt(const llong id, const int column0, const llong value0, const int column1, const int value1) const
{
	// 更新缓存
	mCacheTable->updateLLongInt(id, value0, column0, value1, column1);

	MyString<32> condition;
	sqlConditionLLong(condition, "ID", id);
	MyString<64> updateStr;
	sqlUpdateLLong(updateStr, getColName(column0), value0);
	sqlUpdateInt(updateStr, getColName(column1), value1, false);
	return doUpdate(updateStr.str(), condition.str(), true);
}

bool MySQLTable::updateLLongBool(const llong id, const int column0, const llong value0, const int column1, const bool value1) const
{
	// 更新缓存
	mCacheTable->updateLLongBool(id, value0, column0, value1, column1);

	MyString<32> condition;
	sqlConditionLLong(condition, "ID", id);
	MyString<64> updateStr;
	sqlUpdateLLong(updateStr, getColName(column0), value0);
	sqlUpdateBool(updateStr, getColName(column1), value1, false);
	return doUpdate(updateStr.str(), condition.str(), true);
}

bool MySQLTable::updateAllString(const int col, const string& paramValue) const
{
	// 更新缓存
	mCacheTable->updateAllString(col, paramValue);

	string updateStr;
	sqlUpdateString(updateStr, getColName(col), paramValue, false);
	return doUpdate(updateStr, nullptr, false);
}

bool MySQLTable::updateAllInt(const int col, const int paramValue) const
{
	// 更新缓存
	mCacheTable->updateAllInt(col, paramValue);

	string updateStr;
	sqlUpdateInt(updateStr, getColName(col), paramValue, false);
	return doUpdate(updateStr, nullptr, false);
}

bool MySQLTable::updateAllLLong(const int col, const llong paramValue) const
{
	// 更新缓存
	mCacheTable->updateAllLLong(col, paramValue);

	string updateStr;
	sqlUpdateLLong(updateStr, getColName(col), paramValue, false);
	return doUpdate(updateStr, nullptr, false);
}

bool MySQLTable::updateAllFloat(const int col, const float paramValue) const
{
	// 更新缓存
	mCacheTable->updateAllFloat(col, paramValue);

	string updateStr;
	sqlUpdateFloat(updateStr, getColName(col), paramValue, false);
	return doUpdate(updateStr, nullptr, false);
}

bool MySQLTable::updateAllBool(const int col, const bool paramValue) const
{
	// 更新缓存
	mCacheTable->updateAllBool(col, paramValue);

	string updateStr;
	sqlUpdateBool(updateStr, getColName(col), paramValue, false);
	return doUpdate(updateStr, nullptr, false);
}

int MySQLTable::queryInt(const llong id, const int colIndex)
{
	// 先查询缓存,因为数据从数据库查询出来都是包含全部字段的,所以只要数据存在,那指定字段的数据一定就是有效的
	if (MySQLData* cacheData = mCacheTable->getCacheData(id))
	{
		return cacheData->getInt(colIndex);
	}

	const llong time0 = getRealTimeMS();
	MyString<32> condition;
	sqlConditionLLong(condition, "ID", id);
	MyString<256> queryStr;
	queryStr.add("SELECT ", getColName(colIndex).c_str(), " FROM ", mTableName, " WHERE ", condition.str(), " LIMIT 1");
	const int value = parseColInt(QueryScope(this, queryStr.str()).mResult, 0);
	const llong time1 = getRealTimeMS();
	if (time1 - time0 >= 5)
	{
		LOG("查询Int数据耗时:" + IToS((int)(time1 - time0)) + ", sql:" + queryStr.str());
	}
	return value;
}

HashMap<llong, int> MySQLTable::queryIntList(const Vector<llong>& idList, const int colIndex)
{
	HashMap<llong, int> valueList;
	if (idList.isEmpty())
	{
		return valueList;
	}
	// 先从缓存中查询
	Vector<llong> tempList = idList;
	for (int i = 0; i < tempList.size(); ++i)
	{
		if (MySQLData* cacheData = mCacheTable->getCacheData(tempList[i]))
		{
			valueList.add(tempList[i], cacheData->getInt(colIndex));
			tempList.removeAt(i--);
		}
	}
	if (tempList.isEmpty())
	{
		return valueList;
	}

	// 剩下的再去查数据库
	const llong time0 = getRealTimeMS();
	string queryStr;
	queryStr += "SELECT ID,";
	queryStr += getColName(colIndex);
	queryStr += " FROM ";
	queryStr += mTableName;
	queryStr += " WHERE ID in (";
	queryStr += LLsToS(tempList);
	queryStr += ")";
	
	QueryScope scope(this, queryStr.c_str());
	MYSQL_RES* result = scope.mResult;
	if (result == nullptr)
	{
		return valueList;
	}
	while (true)
	{
		const MYSQL_ROW row = mysql_fetch_row(result);
		if (row == nullptr)
		{
			break;
		}
		if (row[0] == nullptr)
		{
			continue;
		}
		valueList.add(SToLL(row[0]), row[1] != nullptr ? SToI(row[1]) : 0);
	}

	const llong time1 = getRealTimeMS();
	if (time1 - time0 >= 5)
	{
		LOG("查询Int数据耗时:" + IToS((int)(time1 - time0)) + ", sql:" + queryStr);
	}
	return valueList;
}

HashMap<llong, int> MySQLTable::queryIntList(const int colIndex)
{
	HashMap<llong, int> valueList;
	const llong time0 = getRealTimeMS();
	MyString<128> queryStr;
	queryStr.add("SELECT ID,");
	queryStr.add(getColName(colIndex));
	queryStr.add(" FROM ");
	queryStr.add(mTableName);

	QueryScope scope(this, queryStr.str());
	MYSQL_RES* result = scope.mResult;
	if (result == nullptr)
	{
		return valueList;
	}
	while (true)
	{
		const MYSQL_ROW row = mysql_fetch_row(result);
		if (row == nullptr)
		{
			break;
		}
		if (row[0] == nullptr)
		{
			continue;
		}
		valueList.add(SToLL(row[0]), row[1] != nullptr ? SToI(row[1]) : 0);
	}

	const llong time1 = getRealTimeMS();
	if (time1 - time0 >= 5)
	{
		LOG("查询Int数据耗时:" + IToS((int)(time1 - time0)) + ", sql:" + queryStr.str());
	}
	return valueList;
}

float MySQLTable::queryFloat(const llong id, const int col)
{
	// 先查询缓存
	if (MySQLData* cacheData = mCacheTable->getCacheData(id))
	{
		return cacheData->getFloat(col);
	}

	const llong time0 = getRealTimeMS();
	MyString<32> condition;
	sqlConditionLLong(condition, "ID", id);
	MyString<256> queryStr;
	queryStr.add("SELECT ", getColName(col).c_str(), " FROM ", mTableName, " WHERE ", condition.str(), " LIMIT 1");
	const float value = parseColFloat(QueryScope(this, queryStr.str()).mResult, 0);
	const llong time1 = getRealTimeMS();
	if (time1 - time0 >= 5)
	{
		LOG("查询Float数据耗时:" + IToS((int)(time1 - time0)) + ", sql:" + queryStr.str());
	}
	return value;
}

HashMap<llong, float> MySQLTable::queryFloatList(const Vector<llong>& idList, const int colIndex)
{
	HashMap<llong, float> valueList;
	if (idList.isEmpty())
	{
		return valueList;
	}
	// 先从缓存中查询
	Vector<llong> tempList = idList;
	for (int i = 0; i < tempList.size(); ++i)
	{
		if (MySQLData* cacheData = mCacheTable->getCacheData(tempList[i]))
		{
			valueList.add(tempList[i], cacheData->getFloat(colIndex));
			tempList.removeAt(i--);
		}
	}
	if (tempList.isEmpty())
	{
		return valueList;
	}

	// 剩下的再去查数据库
	const llong time0 = getRealTimeMS();
	string queryStr;
	queryStr += "SELECT ID,";
	queryStr += getColName(colIndex);
	queryStr += " FROM ";
	queryStr += mTableName;
	queryStr += " WHERE ID in (";
	queryStr += LLsToS(tempList);
	queryStr += ")";

	QueryScope scope(this, queryStr.c_str());
	MYSQL_RES* result = scope.mResult;
	if (result == nullptr)
	{
		return valueList;
	}
	while (true)
	{
		const MYSQL_ROW row = mysql_fetch_row(result);
		if (row == nullptr)
		{
			break;
		}
		if (row[0] == nullptr)
		{
			continue;
		}
		valueList.add(SToLL(row[0]), row[1] != nullptr ? SToF(row[1]) : 0.0f);
	}

	const llong time1 = getRealTimeMS();
	if (time1 - time0 >= 5)
	{
		LOG("查询Float数据耗时:" + IToS((int)(time1 - time0)) + ", sql:" + queryStr);
	}
	return valueList;
}

HashMap<llong, float> MySQLTable::queryFloatList(const int colIndex)
{
	HashMap<llong, float> valueList;
	const llong time0 = getRealTimeMS();
	MyString<128> queryStr;
	queryStr.add("SELECT ID,", getColName(colIndex).c_str(), " FROM ", mTableName);
	QueryScope scope(this, queryStr.str());
	MYSQL_RES* result = scope.mResult;
	if (result == nullptr)
	{
		return valueList;
	}
	while (true)
	{
		const MYSQL_ROW row = mysql_fetch_row(result);
		if (row == nullptr)
		{
			break;
		}
		if (row[0] == nullptr)
		{
			continue;
		}
		valueList.add(SToLL(row[0]), row[1] != nullptr ? SToF(row[1]) : 0.0f);
	}

	const llong time1 = getRealTimeMS();
	if (time1 - time0 >= 5)
	{
		LOG("查询Float数据耗时:" + IToS((int)(time1 - time0)) + ", sql:" + queryStr.str());
	}
	return valueList;
}

llong MySQLTable::queryLLong(const llong id, const int col)
{
	// 先查询缓存
	if (MySQLData* cacheData = mCacheTable->getCacheData(id))
	{
		return cacheData->getLLong(col);
	}

	const llong time0 = getRealTimeMS();
	MyString<32> condition;
	sqlConditionLLong(condition, "ID", id);
	MyString<256> queryStr;
	queryStr.add("SELECT ", getColName(col).c_str(), " FROM ", mTableName, " WHERE ", condition.str(), " LIMIT 1");
	const llong value = parseColLLong(QueryScope(this, queryStr.str()).mResult, 0);
	const llong time1 = getRealTimeMS();
	if (time1 - time0 >= 5)
	{
		LOG("查询ULLong数据耗时:" + IToS((int)(time1 - time0)) + ", sql:" + queryStr.str());
	}
	return value;
}

HashMap<llong, llong> MySQLTable::queryLLongList(const Vector<llong>& idList, const int colIndex)
{
	HashMap<llong, llong> valueList;
	if (idList.isEmpty())
	{
		return valueList;
	}
	// 先从缓存中查询
	Vector<llong> tempList = idList;
	for (int i = 0; i < tempList.size(); ++i)
	{
		if (MySQLData* cacheData = mCacheTable->getCacheData(tempList[i]))
		{
			valueList.add(tempList[i], cacheData->getLLong(colIndex));
			tempList.removeAt(i--);
		}
	}
	if (tempList.isEmpty())
	{
		return valueList;
	}

	// 剩下的再去查数据库
	const llong time0 = getRealTimeMS();
	string queryStr;
	queryStr += "SELECT ID,";
	queryStr += getColName(colIndex);
	queryStr += " FROM ";
	queryStr += mTableName;
	queryStr += " WHERE ID in (";
	queryStr += LLsToS(tempList);
	queryStr += ")";

	QueryScope scope(this, queryStr.c_str());
	MYSQL_RES* result = scope.mResult;
	if (result == nullptr)
	{
		return valueList;
	}
	while (true)
	{
		const MYSQL_ROW row = mysql_fetch_row(result);
		if (row == nullptr)
		{
			break;
		}
		if (row[0] == nullptr)
		{
			continue;
		}
		valueList.add(SToLL(row[0]), row[1] != nullptr ? SToLL(row[1]) : 0LL);
	}

	const llong time1 = getRealTimeMS();
	if (time1 - time0 >= 5)
	{
		LOG("查询LLong数据耗时:" + IToS((int)(time1 - time0)) + ", sql:" + queryStr);
	}
	return valueList;
}

HashMap<llong, llong> MySQLTable::queryLLongList(const int colIndex)
{
	HashMap<llong, llong> valueList;
	const llong time0 = getRealTimeMS();
	MyString<128> queryStr;
	queryStr.add("SELECT ID,", getColName(colIndex).c_str(), " FROM ", mTableName);
	QueryScope scope(this, queryStr.str());
	MYSQL_RES* result = scope.mResult;
	if (result == nullptr)
	{
		return valueList;
	}
	while (true)
	{
		const MYSQL_ROW row = mysql_fetch_row(result);
		if (row == nullptr)
		{
			break;
		}
		if (row[0] == nullptr)
		{
			continue;
		}
		valueList.add(SToLL(row[0]), row[1] != nullptr ? SToLL(row[1]) : 0LL);
	}

	const llong time1 = getRealTimeMS();
	if (time1 - time0 >= 5)
	{
		LOG("查询LLong数据耗时:" + IToS((int)(time1 - time0)) + ", sql:" + queryStr.str());
	}
	return valueList;
}

string MySQLTable::queryString(const llong id, const int col)
{
	// 先查询缓存
	if (MySQLData* cacheData = mCacheTable->getCacheData(id))
	{
		return cacheData->getString(col);
	}

	const llong time0 = getRealTimeMS();
	MyString<32> condition;
	sqlConditionLLong(condition, "ID", id);
	MyString<256> queryStr;
	queryStr.add("SELECT ", getColName(col).c_str(), " FROM ", mTableName, " WHERE ", condition.str(), " LIMIT 1");
	const string value = parseColString(QueryScope(this, queryStr.str()).mResult, 0);
	const llong time1 = getRealTimeMS();
	if (time1 - time0 >= 5)
	{
		LOG("查询MyString数据耗时:" + IToS((int)(time1 - time0)) + ", sql:" + queryStr.str());
	}
	return value;
}

HashMap<llong, string> MySQLTable::queryStringList(const Vector<llong>& idList, const int colIndex)
{
	HashMap<llong, string> valueList;
	if (idList.isEmpty())
	{
		return valueList;
	}
	// 先从缓存中查询
	Vector<llong> tempList = idList;
	for (int i = 0; i < tempList.size(); ++i)
	{
		if (MySQLData* cacheData = mCacheTable->getCacheData(tempList[i]))
		{
			valueList.add(tempList[i], cacheData->getString(colIndex));
			tempList.removeAt(i--);
		}
	}
	if (tempList.isEmpty())
	{
		return valueList;
	}

	// 剩下的再去查数据库
	const llong time0 = getRealTimeMS();
	string queryStr;
	queryStr += "SELECT ID,";
	queryStr += getColName(colIndex);
	queryStr += " FROM ";
	queryStr += mTableName;
	queryStr += " WHERE ID in (";
	queryStr += LLsToS(tempList);
	queryStr += ")";

	QueryScope scope(this, queryStr.c_str());
	MYSQL_RES* result = scope.mResult;
	if (result == nullptr)
	{
		return valueList;
	}
	while (true)
	{
		const MYSQL_ROW row = mysql_fetch_row(result);
		if (row == nullptr)
		{
			break;
		}
		if (row[0] == nullptr)
		{
			continue;
		}
		valueList.add(SToLL(row[0]), row[1] != nullptr ? row[1] : "");
	}

	const llong time1 = getRealTimeMS();
	if (time1 - time0 >= 5)
	{
		LOG("查询LLong数据耗时:" + IToS((int)(time1 - time0)) + ", sql:" + queryStr);
	}
	return valueList;
}

HashMap<llong, string> MySQLTable::queryStringList(const int colIndex)
{
	HashMap<llong, string> valueList;
	const llong time0 = getRealTimeMS();
	MyString<128> queryStr;
	queryStr.add("SELECT ID,", getColName(colIndex).c_str(), " FROM ", mTableName);
	QueryScope scope(this, queryStr.str());
	MYSQL_RES* result = scope.mResult;
	if (result == nullptr)
	{
		return valueList;
	}
	while (true)
	{
		const MYSQL_ROW row = mysql_fetch_row(result);
		if (row == nullptr)
		{
			break;
		}
		if (row[0] == nullptr)
		{
			continue;
		}
		valueList.add(SToLL(row[0]), row[1] != nullptr ? row[1] : "");
	}

	const llong time1 = getRealTimeMS();
	if (time1 - time0 >= 5)
	{
		LOG("查询LLong数据耗时:" + IToS((int)(time1 - time0)) + ", sql:" + queryStr.str());
	}
	return valueList;
}

MySQLData* MySQLTable::mysqlToResultData(MYSQL_RES* result)
{
	if (result == nullptr || result->row_count == 0)
	{
		return nullptr;
	}
	string tempStr;
	tempStr.reserve(32);
	const MYSQL_ROW sql_row = mysql_fetch_row(result);
	if (sql_row == nullptr)
	{
		return nullptr;
	}
	// 重置列的下标
	mysql_field_seek(result, 0);
	mTemp.clear();
	FOR(mysql_num_fields(result))
	{
		// 将char*转换为string,然后查找相同名字列的真实列名,后续就可以一直使用const char*类型,效率比string高很多
		tempStr = mysql_fetch_field(result)->name;
		const int colIndex = getColIndex(tempStr);
		if (colIndex < 0)
		{
			ERROR("找不到列名的下标:" + tempStr);
		}
		mTemp.add(colIndex, sql_row[i]);
	}
	MySQLData* data = createData();
	data->parseResult(mTemp);
	return data;
}

void MySQLTable::mysqlToResultData(MYSQL_RES* result, const Vector<MySQLData*>& dataList) const
{
	if (result == nullptr)
	{
		return;
	}

	const int fieldCount = (int)mysql_num_fields(result);
	MYSQL_FIELD* fields = mysql_fetch_fields(result);
	if (fields == nullptr)
	{
		return;
	}

	// MySQL结果列下标 -> 数据类列下标
	ArrayList<256, int> fieldToColumnIndex;
	FOR(fieldCount)
	{
		const int colIndex = getColIndex(fields[i].name);
		if (colIndex < 0)
		{
			ERROR("找不到列名的下标:" + string(fields[i].name));
		}
		fieldToColumnIndex.add(colIndex);
	}

	int index = 0;
	while (true)
	{
		const MYSQL_ROW sql_row = mysql_fetch_row(result);
		if (sql_row == nullptr)
		{
			break;
		}
		mTemp.clear();
		FOR(fieldCount)
		{
			const int colIndex = fieldToColumnIndex[i];
			if (colIndex < 0)
			{
				continue;
			}
			mTemp.add(colIndex, sql_row[i]);
		}
		dataList[index++]->parseResult(mTemp);
	}
}

const char* MySQLTable::parseColPtr(MYSQL_RES* result, const int colIndex) const
{
	if (result == nullptr || (int)mysql_num_fields(result) <= colIndex)
	{
		return nullptr;
	}
	const MYSQL_ROW sql_row = mysql_fetch_row(result);
	return sql_row != nullptr ? sql_row[colIndex] : nullptr;
}

llong MySQLTable::queryCount(const char* condition)
{
	const llong time0 = getRealTimeMS();
	MyString<512> queryStr;
	if (condition != nullptr && condition[0] != '\0')
	{
		queryStr.add("SELECT count(*) FROM ", mTableName, " WHERE ", condition);
	}
	else
	{
		queryStr.add("SELECT count(*) FROM ", mTableName);
	}
	// 获得查询结果
	const llong count = parseColLLong(QueryScope(this, queryStr.str()).mResult, 0);
	const llong time1 = getRealTimeMS();
	if (time1 - time0 >= 5)
	{
		LOG("查询数据数量耗时:" + IToS((int)(time1 - time0)) + ", sql:" + queryStr.str());
	}
	return count;
}

int MySQLTable::queryMaxInt(int column)
{
	const llong time0 = getRealTimeMS();
	const string& colName = getColName(column);
	MyString<128> queryStr;
	queryStr.add("SELECT max(", colName.c_str(), ") FROM ", mTableName);
	// 获得查询结果
	const int maxValue = parseColInt(QueryScope(this, queryStr.str()).mResult, 0);
	const llong time1 = getRealTimeMS();
	if (time1 - time0 >= 5)
	{
		LOG("查询数据数量耗时:" + IToS((int)(time1 - time0)) + ", sql:" + queryStr.str());
	}
	return maxValue;
}

llong MySQLTable::queryMaxLLong(int column)
{
	const llong time0 = getRealTimeMS();
	const string& colName = getColName(column);
	MyString<128> queryStr;
	queryStr.add("SELECT max(", colName.c_str(), ") FROM ", mTableName);
	// 获得查询结果
	const llong maxValue = parseColLLong(QueryScope(this, queryStr.str()).mResult, 0);
	const llong time1 = getRealTimeMS();
	if (time1 - time0 >= 5)
	{
		LOG("查询数据数量耗时:" + IToS((int)(time1 - time0)) + ", sql:" + queryStr.str());
	}
	return maxValue;
}

bool MySQLTable::executeNonQuery(const char* str, const bool showError, const bool hideInfo, bool allowReconnect) const
{
	if (mMySQLManager != nullptr && mMySQLManager->getThread() != nullptr && getThreadID() != mMySQLManager->getThread()->getThisThreadID())
	{
		ERROR("禁止在mysql线程之外调用数据库操作,query:" + string(str));
		return false;
	}
	if (str == nullptr || mMySQL == nullptr)
	{
		return false;
	}
	if (strchar(str, '\\') >= 0)
	{
		LOG("输入的语句带转义字符:" + string(str));
		return false;
	}
	// 日志有点太多了,暂时不写入
	//LOG_NO_PRINT(string("mysql:") + str);
	if (mysql_query(mMySQL, str) != 0)
	{
		// 如果是字符串非法,则只能暂时忽略掉,此处的日志打出来意义不大,就不显示了
		const char* errorInfo = mysql_error(mMySQL);
		if (showError)
		{
			ERROR("query error!   " + string(errorInfo) + ", query : " + str);
		}
		else
		{
			if (findString(errorInfo, "Incorrect string value") || !hideInfo)
			{
				LOG("执行数据库语句失败:" + string(errorInfo) + ", query:" + str);
				LOG("str hex:" + bytesToHexString((byte*)str, strlength(str)));
			}
		}
		// 重连成功后再执行一次此语句
		if (allowReconnect && mMySQLManager != nullptr && mMySQLManager->checkReconnect(errorInfo))
		{
			return executeNonQuery(str, showError, hideInfo, false);
		}
		return false;
	}
	return true;
}

MYSQL_RES* MySQLTable::executeQuery(const char* str, bool allowReconnect) const
{
	if (mMySQLManager != nullptr && mMySQLManager->getThread() != nullptr && getThreadID() != mMySQLManager->getThread()->getThisThreadID())
	{
		ERROR("禁止在mysql线程之外调用数据库操作,query:" + string(str));
		return nullptr;
	}
	if (str == nullptr || mMySQL == nullptr)
	{
		return nullptr;
	}
	if (strchar(str, '\\') >= 0)
	{
		LOG("输入的语句带转义字符:" + string(str));
		return nullptr;
	}
	// 日志有点太多了,暂时不写入
	//LOG_NO_PRINT(string("mysql:") + str);
	// 查询
	const int ret = mysql_query(mMySQL, str);
	if (ret != 0)
	{
		// 如果是字符串非法,则只能暂时忽略掉
		const string errorInfo(mysql_error(mMySQL));
		if (findString(errorInfo, "Incorrect string value"))
		{
			LOG("执行数据库语句失败:" + errorInfo + ", query:" + str + ", 错误码:" + IToS(ret));
		}
		else
		{
			ERROR("query error!   " + errorInfo + ", query : " + str + ", 错误码:" + IToS(ret));
		}
		// 重连成功后再执行一次此语句
		if (allowReconnect && mMySQLManager != nullptr && mMySQLManager->checkReconnect(errorInfo.c_str()))
		{
			return executeQuery(str, false);
		}
		return nullptr;
	}
	return mysql_store_result(mMySQL);
}

void MySQLTable::endQuery(MYSQL_RES* result) const
{
	// 释放结果资源
	if (result != nullptr)
	{
		mysql_free_result(result);
	}
}

#endif