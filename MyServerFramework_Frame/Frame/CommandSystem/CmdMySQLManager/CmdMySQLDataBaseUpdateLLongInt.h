#pragma once

#ifdef _MYSQL
#include "GameCommand.h"
#include "MySQLTable.h"

// 简单地将一个llong和int属性写入数据库
class MICRO_LEGEND_FRAME_API CmdMySQLDataBaseUpdateLLongInt : public GameCommand
{
	BASE(CmdMySQLDataBaseUpdateLLongInt, GameCommand);
public:
	void reset() override
	{
		mTable = nullptr;
		mID = 0;
		mColumn0 = -1;
		mColumn1 = -1;
		mValue0 = 0;
		mValue1 = 0;
	}
	void execute() override
	{
		mTable->updateLLongInt(mID, mColumn0, mValue0, mColumn1, mValue1);
	}
public:
	MySQLTable* mTable = nullptr;
	llong mID = 0;
	int mColumn0 = -1;
	int mColumn1 = -1;
	llong mValue0 = 0;
	int mValue1 = 0;
};
#endif