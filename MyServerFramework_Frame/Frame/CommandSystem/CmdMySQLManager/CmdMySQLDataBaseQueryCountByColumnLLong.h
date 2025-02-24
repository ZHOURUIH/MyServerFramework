#pragma once

#ifdef _MYSQL
#include "GameCommand.h"
#include "FrameCallback.h"

class MICRO_LEGEND_FRAME_API CmdMySQLDataBaseQueryCountByColumnLLong : public GameCommand
{
	BASE(CmdMySQLDataBaseQueryCountByColumnLLong, GameCommand);
public:
	void reset() override
	{
		mTable = nullptr;
		mColumn = -1;
		mValue = 0;
		mCallback = nullptr;
	}
	void execute() override;
public:
	MySQLTable* mTable = nullptr;
	int mColumn = -1;
	llong mValue = 0;
	LLongCallback mCallback = nullptr;
};
#endif