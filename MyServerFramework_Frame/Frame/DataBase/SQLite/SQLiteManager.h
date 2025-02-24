#pragma once

#include "FrameComponent.h"

// 一个sqlite文件只允许有一个表格,且名字与文件名相同
class MICRO_LEGEND_FRAME_API SQLiteManager : public FrameComponent
{
	BASE(SQLiteManager, FrameComponent);
public:
	void quit() override;
	SQLiteTableBase* getSQLite(const string& name) { return mSQLiteList.tryGet(name); }
	void addSQLiteTable(SQLiteTableBase* table, const char* tableName);
	void checkAll();
public:
	HashMap<string, SQLiteTableBase*> mSQLiteList;
};