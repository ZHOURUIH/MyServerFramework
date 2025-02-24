#pragma once

#ifdef _MYSQL
#include "GameCommand.h"
#include "MySQLTable.h"

// 简单地将两个整数属性写入数据库
class MICRO_LEGEND_FRAME_API CmdMySQLDataBaseUpdateInt2 : public GameCommand
{
	BASE(CmdMySQLDataBaseUpdateInt2, GameCommand);
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
		if (mColumn0 != -1)
		{
			mTable->updateInt(mID, mColumn0, mValue0);
		}
		if (mColumn1 != -1)
		{
			mTable->updateInt(mID, mColumn1, mValue1);
		}
	}
public:
	MySQLTable* mTable = nullptr;
	llong mID = 0;
	int mColumn0 = -1;
	int mColumn1 = -1;
	int mValue0 = 0;
	int mValue1 = 0;
};
#endif