#pragma once

#ifdef _MYSQL
#include "GameCommand.h"
#include "MySQLTable.h"

// 简单地将一个长整数列表属性写入数据库
class MICRO_LEGEND_FRAME_API CmdMySQLDataBaseUpdateLLongList : public GameCommand
{
	BASE(CmdMySQLDataBaseUpdateLLongList, GameCommand);
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
		mTable->updateString(mID, mColumn, LLsToS(mValue));
	}
public:
	MySQLTable* mTable = nullptr;
	llong mID = 0;
	int mColumn = -1;
	Vector<llong> mValue;
};
#endif