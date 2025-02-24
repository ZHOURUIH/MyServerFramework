#pragma once

#ifdef _MYSQL
#include "GameCommand.h"

class MICRO_LEGEND_FRAME_API CmdMySQLDataBaseDeleteByColumnLLong : public GameCommand
{
	BASE(CmdMySQLDataBaseDeleteByColumnLLong, GameCommand);
public:
	void reset() override
	{
		mTable = nullptr;
		mColumn = -1;
		mValue = 0;
		mOnlyOnce = false;
	}
	void execute() override;
public:
	MySQLTable* mTable = nullptr;
	int mColumn = -1;
	llong mValue = 0;
	bool mOnlyOnce = false;
};
#endif