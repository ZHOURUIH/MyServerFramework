#pragma once

#ifdef _MYSQL
#include "GameCommand.h"
#include "MySQLTable.h"

// 简单地将一个整数属性和一个浮点属性写入数据库
class MICRO_LEGEND_FRAME_API CmdMySQLDataBaseUpdateIntFloat : public GameCommand
{
	BASE(CmdMySQLDataBaseUpdateIntFloat, GameCommand);
public:
	void reset() override
	{
		mTable = nullptr;
		mID = 0;
		mColumn0 = -1;
		mValue0 = 0;
		mColumn1 = -1;
		mValue1 = 0.0f;
	}
	void execute() override
	{
		mTable->updateIntFloat(mID, mColumn0, mValue0, mColumn1, mValue1);
	}
public:
	MySQLTable* mTable = nullptr;
	llong mID = 0;
	int mColumn0 = -1;
	int mValue0 = 0;
	int mColumn1 = -1;
	float mValue1 = 0.0f;
};
#endif