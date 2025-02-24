#pragma once

#ifdef _MYSQL
#include "GameCommand.h"

// 删除多个数据库中满足指定条件的数据
class MICRO_LEGEND_FRAME_API CmdMySQLDataBaseDeleteByColumnString : public GameCommand
{
	BASE(CmdMySQLDataBaseDeleteByColumnString, GameCommand);
public:
	void reset() override
	{
		mTable = nullptr;
		mColumn = -1;
		mValue.clear();
		mOnlyOnce = false;
	}
	void execute() override;
public:
	MySQLTable* mTable = nullptr;
	int mColumn = -1;
	string mValue;
	bool mOnlyOnce = false;
};
#endif