#pragma once

#ifdef _MYSQL
#include "GameCommand.h"
#include "MySQLTable.h"

// 简单地执行一条更新语句写入数据库
class MICRO_LEGEND_FRAME_API CmdMySQLDataBaseUpdateData : public GameCommand
{
	BASE(CmdMySQLDataBaseUpdateData, GameCommand);
public:
	void reset() override
	{
		mTable = nullptr;
		mData = nullptr;
	}
	void execute() override 
	{
		mTable->updateData(mData, FrameDefine::FULL_MYSQL_FLAG);
		mMySQLDataPool->destroyClass(mData);
	}
public:
	MySQLTable* mTable = nullptr;
	MySQLData* mData = nullptr;
};
#endif