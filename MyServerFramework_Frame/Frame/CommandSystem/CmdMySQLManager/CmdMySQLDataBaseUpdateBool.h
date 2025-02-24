#pragma once

#ifdef _MYSQL
#include "GameCommand.h"
#include "MySQLTable.h"

// 简单地将一个整数属性写入数据库
class MICRO_LEGEND_FRAME_API CmdMySQLDataBaseUpdateBool : public GameCommand
{
	BASE(CmdMySQLDataBaseUpdateBool, GameCommand);
public:
	void reset() override
	{
		mTable = nullptr;
		mID = 0;
		mColumn = -1;
		mValue = 0;
	}
	void execute() override 
	{
		mTable->updateBool(mID, mColumn, mValue); 
	}
public:
	MySQLTable* mTable = nullptr;
	llong mID = 0;
	int mColumn = -1;
	int mValue = 0;
};
#endif