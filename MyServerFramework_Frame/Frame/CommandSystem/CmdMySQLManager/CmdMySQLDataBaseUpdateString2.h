#pragma once

#ifdef _MYSQL
#include "GameCommand.h"
#include "MySQLTable.h"

// 简单地将一个字符串属性写入数据库
class MICRO_LEGEND_FRAME_API CmdMySQLDataBaseUpdateString2 : public GameCommand
{
	BASE(CmdMySQLDataBaseUpdateString2, GameCommand);
public:
	void reset() override
	{
		mTable = nullptr;
		mID = 0;
		mColumn0 = -1;
		mValue0.clear();
		mColumn1 = -1;
		mValue1.clear();
	}
	void execute() override
	{
		mTable->updateString2(mID, mColumn0, mValue0, mColumn1, mValue1);
	}
public:
	MySQLTable* mTable = nullptr;
	llong mID = 0;
	int mColumn0 = -1;
	string mValue0;
	int mColumn1 = -1;
	string mValue1;
};
#endif