#pragma once

#include "SQLiteDataReader.h"
#include "FrameBase.h"
#include "ThreadLock.h"

class MICRO_LEGEND_FRAME_API SQLiteTableBase
{
public:
	SQLiteTableBase();
	void setTableName(const char* tableName) { mTableName = tableName; }
	const char* getTableName() const { return mTableName; }
	sqlite3* getSQLite3() const { return mSQLite3; }
	virtual void init(const string& fullPath);
	virtual ~SQLiteTableBase();
	virtual void checkAllData() {}
	void checkColName(const string& colName, int index);
	virtual void checkDataAllColName() = 0;
protected:
	bool doUpdate(const char* updateString, const char* conditionString) const;
	bool doInsert(const char* valueString) const;
	bool executeNonQuery(const char* queryString) const;
protected:
	ThreadLock mThreadLock;
	const char* mTableName = nullptr;
	sqlite3* mSQLite3 = nullptr;
};