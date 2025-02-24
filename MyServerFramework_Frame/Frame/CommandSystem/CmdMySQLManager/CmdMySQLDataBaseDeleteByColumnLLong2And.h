#pragma once

#ifdef _MYSQL
#include "GameCommand.h"

class MICRO_LEGEND_FRAME_API CmdMySQLDataBaseDeleteByColumnLLong2And : public GameCommand
{
	BASE(CmdMySQLDataBaseDeleteByColumnLLong2And, GameCommand);
public:
	void reset() override
	{
		mTable = nullptr;
		mColumn0 = -1;
		mColumn1 = -1;
		mValue0 = 0;
		mValue1 = 0;
		mOnlyOnce = false;
	}
	void execute() override;
public:
	MySQLTable* mTable = nullptr;
	int mColumn0 = -1;
	int mColumn1 = -1;
	llong mValue0 = 0;
	llong mValue1 = 0;
	bool mOnlyOnce = false;
};
#endif