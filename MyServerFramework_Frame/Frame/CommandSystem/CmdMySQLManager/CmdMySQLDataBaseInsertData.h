#pragma once

#ifdef _MYSQL
#include "GameCommand.h"
#include "FrameCallback.h"

class MICRO_LEGEND_FRAME_API CmdMySQLDataBaseInsertData : public GameCommand
{
	BASE(CmdMySQLDataBaseInsertData, GameCommand);
public:
	void reset() override
	{
		mTable = nullptr;
		mData = nullptr;
		mCallback = nullptr;
	}
	void execute() override;
public:
	MySQLTable* mTable = nullptr;
	MySQLData* mData = nullptr;
	BoolCallback mCallback = nullptr;
};
#endif