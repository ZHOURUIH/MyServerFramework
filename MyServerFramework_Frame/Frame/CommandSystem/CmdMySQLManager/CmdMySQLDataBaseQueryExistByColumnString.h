#pragma once

#ifdef _MYSQL
#include "GameCommand.h"
#include "FrameCallback.h"

class MICRO_LEGEND_FRAME_API CmdMySQLDataBaseQueryExistByColumnString : public GameCommand
{
	BASE(CmdMySQLDataBaseQueryExistByColumnString, GameCommand);
public:
	void reset() override
	{
		mTable = nullptr;
		mColumn = -1;
		mValue.clear();
		mCallback = nullptr;
	}
	void execute() override;
public:
	MySQLTable* mTable = nullptr;
	int mColumn = -1;
	string mValue;
	BoolCallback mCallback = nullptr;
};
#endif