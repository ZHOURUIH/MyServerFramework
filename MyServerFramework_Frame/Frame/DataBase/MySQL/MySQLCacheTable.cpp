#ifdef _MYSQL
#include "FrameHeader.h"

MySQLCacheTable::MySQLCacheTable(MySQLTable* table) :
	mTable(table)
{}

void MySQLCacheTable::destroy()
{
	deleteAllCacheData();
}

ullong MySQLCacheTable::columnToFlag(const int colName) const
{
	ullong flag = 0;
	setBitOne(flag, colName);
	return flag;
}

ullong MySQLCacheTable::columnsToFlag(const Vector<int>& colNames) const
{
	ullong flag = 0;
	for (const int col : colNames)
	{
		setBitOne(flag, col);
	}
	return flag;
}

MySQLData* MySQLCacheTable::getCacheDataByColumnLLong2Or(const int column0, const llong value0, const int column1, const llong value1)
{
	for (const auto& item : mCacheDataList)
	{
		if (item.second->hasLLong(column0, value0) || item.second->hasLLong(column1, value1))
		{
			return item.second->getData();
		}
	}
	return nullptr;
}

MySQLData* MySQLCacheTable::getCacheDataByColumnLLong2And(const int column0, const llong value0, const int column1, const llong value1)
{
	for (const auto& item : mCacheDataList)
	{
		if (item.second->hasLLong(column0, value0) && item.second->hasLLong(column1, value1))
		{
			return item.second->getData();
		}
	}
	return nullptr;
}

MySQLData* MySQLCacheTable::getCacheDataByColumnInt(const int column, const int value)
{
	for (const auto& item : mCacheDataList)
	{
		if (item.second->hasInt(column, value))
		{
			return item.second->getData();
		}
	}
	return nullptr;
}

MySQLData* MySQLCacheTable::getCacheDataByColumnLLong(const int column, const llong value)
{
	for (const auto& item : mCacheDataList)
	{
		if (item.second->hasLLong(column, value))
		{
			return item.second->getData();
		}
	}
	return nullptr;
}

MySQLData* MySQLCacheTable::getCacheDataByColumnString(const int column, const string& value)
{
	for (const auto& item : mCacheDataList)
	{
		if (item.second->hasString(column, value))
		{
			return item.second->getData();
		}
	}
	return nullptr;
}

MySQLData* MySQLCacheTable::getCacheDataByColumnString2And(const int column0, const string& value0, const int column1, const string& value1)
{
	for (const auto& item : mCacheDataList)
	{
		if (item.second->hasString(column0, value0) && item.second->hasString(column1, value1))
		{
			return item.second->getData();
		}
	}
	return nullptr;
}

MySQLData* MySQLCacheTable::getCacheDataByColumnLLongIntAnd(const int column0, const llong value0, const int column1, const int value1)
{
	for (const auto& item : mCacheDataList)
	{
		if (item.second->hasLLong(column0, value0) && item.second->hasInt(column1, value1))
		{
			return item.second->getData();
		}
	}
	return nullptr;
}

MySQLData* MySQLCacheTable::getCacheDataByColumnLLongStringAnd(const int column0, const llong value0, const int column1, const string& value1)
{
	for (const auto& item : mCacheDataList)
	{
		if (item.second->hasLLong(column0, value0) && item.second->hasString(column1, value1))
		{
			return item.second->getData();
		}
	}
	return nullptr;
}

void MySQLCacheTable::getCacheDataListByColumnInt(const int column0, int value0, Vector<MySQLData*>& dataList)
{
	dataList.clear();
	for (const auto& item : mCacheDataList)
	{
		if (item.second->hasInt(column0, value0))
		{
			dataList.push_back(item.second->getData());
		}
	}
}

void MySQLCacheTable::updateCacheInt(const llong id, const int value, const int colName) const
{
	if (MySQLCacheData* curData = mCacheDataList.tryGet(id))
	{
		curData->updateInt(colName, value);
	}
}

void MySQLCacheTable::updateCacheBool(const llong id, const bool value, const int colName) const
{
	if (MySQLCacheData* curData = mCacheDataList.tryGet(id))
	{
		curData->updateBool(colName, value);
	}
}

void MySQLCacheTable::updateCacheInt2(const llong id, const int value0, const int colName0, const int value1, const int colName1) const
{
	if (MySQLCacheData* curData = mCacheDataList.tryGet(id))
	{
		curData->updateInt(colName0, value0);
		curData->updateInt(colName1, value1);
	}
}

void MySQLCacheTable::updateCacheFloat(const llong id, const float value, const int colName) const
{
	if (MySQLCacheData* curData = mCacheDataList.tryGet(id))
	{
		curData->updateFloat(colName, value);
	}
}

void MySQLCacheTable::updateCacheLLong(const llong id, const llong value, const int colName) const
{
	if (MySQLCacheData* curData = mCacheDataList.tryGet(id))
	{
		curData->updateLLong(colName, value);
	}
}

void MySQLCacheTable::updateCacheLLongInt(const llong id, const llong value0, const int colName0, const int value1, const int column1) const
{
	if (MySQLCacheData* curData = mCacheDataList.tryGet(id))
	{
		curData->updateLLong(colName0, value0);
		curData->updateInt(column1, value1);
	}
}

void MySQLCacheTable::updateCacheLLongBool(const llong id, const llong value0, const int colName0, const bool value1, const int column1) const
{
	if (MySQLCacheData* curData = mCacheDataList.tryGet(id))
	{
		curData->updateLLong(colName0, value0);
		curData->updateBool(column1, value1);
	}
}

void MySQLCacheTable::updateCacheString(const llong id, const string& value, const int colName) const
{
	if (MySQLCacheData* curData = mCacheDataList.tryGet(id))
	{
		curData->updateString(colName, value);
	}
}

void MySQLCacheTable::updateCacheString2(const llong id, const string& value0, const int colName0, const string& value1, const int colName1) const
{
	if (MySQLCacheData* curData = mCacheDataList.tryGet(id))
	{
		curData->updateString(colName0, value0);
		curData->updateString(colName1, value1);
	}
}

void MySQLCacheTable::updateCacheAllString(const int colName, const string& value) const
{
	for (const auto& item : mCacheDataList)
	{
		item.second->updateString(colName, value);
	}
}

void MySQLCacheTable::updateCacheAllInt(const int colName, const int value) const
{
	for (const auto& item : mCacheDataList)
	{
		item.second->updateInt(colName, value);
	}
}

void MySQLCacheTable::updateCacheAllLLong(const int colName, const llong value) const
{
	for (const auto& item : mCacheDataList)
	{
		item.second->updateLLong(colName, value);
	}
}

void MySQLCacheTable::updateCacheAllFloat(const int colName, const float value) const
{
	for (const auto& item : mCacheDataList)
	{
		item.second->updateFloat(colName, value);
	}
}

void MySQLCacheTable::updateCacheAllBool(const int colName, const bool value) const
{
	for (const auto& item : mCacheDataList)
	{
		item.second->updateBool(colName, value);
	}
}

void MySQLCacheTable::addCacheData(const MySQLData* data, const ullong validFlag)
{
	// 数据已经有了,则更新数据
	if (MySQLCacheData* curData = mCacheDataList.tryGet(data->mID))
	{
		data->cloneWithFlag(curData->getData(), validFlag);
		curData->addValidFlag(validFlag);
	}
	// 插入新的数据
	else
	{
		MySQLCacheData* cacheData = mMySQLCacheDataPool->newClass();
		cacheData->setTable(mTable);
		data->clone(cacheData->getData());
		cacheData->addValidFlag(validFlag);
		mCacheDataList.insert(cacheData->getID(), cacheData);
	}
}

void MySQLCacheTable::addCacheDataList(const Vector<MySQLData*>& dataList, const ullong validFlag)
{
	for (const MySQLData* data : dataList)
	{
		if (data == nullptr)
		{
			continue;
		}
		// 数据已经有了,则更新数据
		if (MySQLCacheData* curData = mCacheDataList.tryGet(data->mID))
		{
			data->cloneWithFlag(curData->getData(), validFlag);
			curData->addValidFlag(validFlag);
		}
		// 插入新的数据
		else
		{
			MySQLCacheData* cacheData = mMySQLCacheDataPool->newClass();
			cacheData->setTable(mTable);
			data->clone(cacheData->getData());
			cacheData->addValidFlag(validFlag);
			mCacheDataList.insert(cacheData->getID(), cacheData);
		}
	}
}

void MySQLCacheTable::deleteAllCacheData()
{
	for (const auto& item : mCacheDataList)
	{
		item.second->destroy();
	}
	mMySQLCacheDataPool->destroyClassList(mCacheDataList);
	mCacheDataList.clear();
}

void MySQLCacheTable::deleteCacheData(const llong id)
{
	MySQLCacheData* data = nullptr;
	if (mCacheDataList.erase(id, data))
	{
		data->destroy();
		mMySQLCacheDataPool->destroyClass(data);
	}
}

void MySQLCacheTable::deleteCacheDataInIDRange(const llong minID, const llong maxID)
{
	for (auto iter = mCacheDataList.begin(); iter != mCacheDataList.end(); )
	{
		if (iter->first >= minID && iter->first <= maxID)
		{
			iter->second->destroy();
			mMySQLCacheDataPool->destroyClass(iter->second);
			iter = mCacheDataList.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}

void MySQLCacheTable::deleteCacheDataByColumnBool(const int column, const bool value, bool onlyOnce)
{
	for (auto iter = mCacheDataList.begin(); iter != mCacheDataList.end(); )
	{
		MySQLCacheData* cacheData = iter->second;
		if (cacheData->hasBool(column, value))
		{
			cacheData->destroy();
			mMySQLCacheDataPool->destroyClass(cacheData);
			iter = mCacheDataList.erase(iter);
			if (onlyOnce)
			{
				break;
			}
		}
		else
		{
			++iter;
		}
	}
}

void MySQLCacheTable::deleteCacheDataByColumnInt(const int column, const int value, bool onlyOnce)
{
	for (auto iter = mCacheDataList.begin(); iter != mCacheDataList.end(); )
	{
		MySQLCacheData* cacheData = iter->second;
		if (cacheData->hasInt(column, value))
		{
			cacheData->destroy();
			mMySQLCacheDataPool->destroyClass(cacheData);
			iter = mCacheDataList.erase(iter);
			if (onlyOnce)
			{
				break;
			}
		}
		else
		{
			++iter;
		}
	}
}

void MySQLCacheTable::deleteCacheDataByColumnFloat(const int column, const float value, bool onlyOnce)
{
	for (auto iter = mCacheDataList.begin(); iter != mCacheDataList.end(); )
	{
		MySQLCacheData* cacheData = iter->second;
		if (cacheData->hasFloat(column, value))
		{
			cacheData->destroy();
			mMySQLCacheDataPool->destroyClass(cacheData);
			iter = mCacheDataList.erase(iter);
			if (onlyOnce)
			{
				break;
			}
		}
		else
		{
			++iter;
		}
	}
}

void MySQLCacheTable::deleteCacheDataByColumnLLong(const int column, const llong value, bool onlyOnce)
{
	for (auto iter = mCacheDataList.begin(); iter != mCacheDataList.end(); )
	{
		MySQLCacheData* cacheData = iter->second;
		if (cacheData->hasLLong(column, value))
		{
			cacheData->destroy();
			mMySQLCacheDataPool->destroyClass(cacheData);
			iter = mCacheDataList.erase(iter);
			if (onlyOnce)
			{
				break;
			}
		}
		else
		{
			++iter;
		}
	}
}

void MySQLCacheTable::deleteCacheDataByColumnString(const int column, const string& value, bool onlyOnce)
{
	for (auto iter = mCacheDataList.begin(); iter != mCacheDataList.end(); )
	{
		MySQLCacheData* cacheData = iter->second;
		if (cacheData->hasString(column, value))
		{
			cacheData->destroy();
			mMySQLCacheDataPool->destroyClass(cacheData);
			iter = mCacheDataList.erase(iter);
			if (onlyOnce)
			{
				break;
			}
		}
		else
		{
			++iter;
		}
	}
}

void MySQLCacheTable::deleteCacheDataByColumnLLong2And(const int column0, const llong value0, const int column1, const llong value1, bool onlyOnce)
{
	for (auto iter = mCacheDataList.begin(); iter != mCacheDataList.end(); )
	{
		MySQLCacheData* cacheData = iter->second;
		if (cacheData->hasLLong(column0, value0) && cacheData->hasLLong(column1, value1))
		{
			cacheData->destroy();
			mMySQLCacheDataPool->destroyClass(cacheData);
			iter = mCacheDataList.erase(iter);
			if (onlyOnce)
			{
				break;
			}
		}
		else
		{
			++iter;
		}
	}
}

void MySQLCacheTable::deleteCacheDataByColumnLLong2Or(const int column0, const llong value0, const int column1, const llong value1, bool onlyOnce)
{
	for (auto iter = mCacheDataList.begin(); iter != mCacheDataList.end(); )
	{
		MySQLCacheData* cacheData = iter->second;
		if (cacheData->hasLLong(column0, value0) || cacheData->hasLLong(column1, value1))
		{
			cacheData->destroy();
			mMySQLCacheDataPool->destroyClass(cacheData);
			iter = mCacheDataList.erase(iter);
			if (onlyOnce)
			{
				break;
			}
		}
		else
		{
			++iter;
		}
	}
}
#endif