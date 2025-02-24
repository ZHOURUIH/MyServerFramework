#pragma once

#ifdef _MYSQL
#include "GameCommand.h"
#include "FrameCallback.h"

class MICRO_LEGEND_FRAME_API CmdMySQLDataBaseQueryCountByColumnLLong2Or : public GameCommand
{
	BASE(CmdMySQLDataBaseQueryCountByColumnLLong2Or, GameCommand);
public:
	void reset() override
	{
		mTable = nullptr;
		mColumn0 = -1;
		mColumn1 = -1;
		mValue0 = 0;
		mValue1 = 0;
		mCallback = nullptr;
	}
	void execute() override;
public:
	MySQLTable* mTable = nullptr;
	int mColumn0 = -1;
	int mColumn1 = -1;
	llong mValue0 = 0;
	llong mValue1 = 0;
	LLongCallback mCallback = nullptr;
};
#endif