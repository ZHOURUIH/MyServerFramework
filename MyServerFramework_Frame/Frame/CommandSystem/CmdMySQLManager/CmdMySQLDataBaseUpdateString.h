#pragma once

#ifdef _MYSQL
#include "GameCommand.h"
#include "MySQLTable.h"

// 简单地将一个字符串属性写入数据库
class MICRO_LEGEND_FRAME_API CmdMySQLDataBaseUpdateString : public GameCommand
{
	BASE(CmdMySQLDataBaseUpdateString, GameCommand);
public:
	void reset() override
	{
		mTable = nullptr;
		mID = 0;
		mColumn = -1;
		mValue.clear();
	}
	void execute() override
	{
		mTable->updateString(mID, mColumn, mValue);
	}
public:
	MySQLTable* mTable = nullptr;
	llong mID = 0;
	int mColumn = -1;
	string mValue;
};
#endif