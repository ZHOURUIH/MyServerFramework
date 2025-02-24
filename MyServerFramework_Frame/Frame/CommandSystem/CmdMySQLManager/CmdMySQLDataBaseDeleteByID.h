#pragma once

#ifdef _MYSQL
#include "GameCommand.h"

// 根据ID删除多个数据库中多条数据
class MICRO_LEGEND_FRAME_API CmdMySQLDataBaseDeleteByID : public GameCommand
{
	BASE(CmdMySQLDataBaseDeleteByID, GameCommand);
public:
	void reset() override
	{
		mTableList.clear();
		mIDList.clear();
	}
	void execute() override;
public:
	Vector<MySQLTable*> mTableList;
	Vector<llong> mIDList;
};
#endif