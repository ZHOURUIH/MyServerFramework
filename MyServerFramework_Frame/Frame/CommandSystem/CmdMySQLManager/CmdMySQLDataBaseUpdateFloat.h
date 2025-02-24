#pragma once

#ifdef _MYSQL
#include "GameCommand.h"
#include "MySQLTable.h"

// 简单地将一个浮点数数属性写入数据库
class MICRO_LEGEND_FRAME_API CmdMySQLDataBaseUpdateFloat : public GameCommand
{
	BASE(CmdMySQLDataBaseUpdateFloat, GameCommand);
public:
	void reset() override
	{
		mTable = nullptr;
		mID = 0;
		mColumn = -1;
		mValue = 0.0f;
	}
	void execute() override 
	{
		mTable->updateFloat(mID, mColumn, mValue); 
	}
public:
	MySQLTable* mTable = nullptr;
	llong mID = 0;
	int mColumn = -1;
	float mValue = 0.0f;
};
#endif