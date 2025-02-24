#include "FrameHeader.h"

void SQLiteManager::quit()
{
	for (const auto& iter : mSQLiteList)
	{
		delete iter.second;
	}
	mSQLiteList.clear();
}

void SQLiteManager::addSQLiteTable(SQLiteTableBase* table, const char* tableName)
{
	table->setTableName(tableName);
	table->init(string(FrameDefine::SQLITE_PATH) + tableName + ".db");
	mSQLiteList.insert(table->getTableName(), table);
}

void SQLiteManager::checkAll()
{
	for (const auto& iter : mSQLiteList)
	{
		iter.second->checkAllData();
		iter.second->checkDataAllColName();
	}
}