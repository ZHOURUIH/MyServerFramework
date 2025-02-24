#pragma once

#ifdef _MYSQL
#include "GameCommand.h"
#include "MySQLTable.h"

// 简单地执行一条更新语句写入数据库,并且是全表执行
class MICRO_LEGEND_FRAME_API CmdMySQLDataBaseUpdateAllInt : public GameCommand
{
	BASE(CmdMySQLDataBaseUpdateAllInt, GameCommand);
public:
	void reset() override
	{
		mTable = nullptr;
		mColumn = -1;
		mValue = 0;
	}
	void execute() override 
	{
		mTable->updateAllInt(mColumn, mValue); 
	}
public:
	MySQLTable* mTable = nullptr;
	int mColumn = -1;
	int mValue = 0;
};
#endif