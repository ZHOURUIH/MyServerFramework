#pragma once

#ifdef _MYSQL
#include "GameCommand.h"

class MICRO_LEGEND_FRAME_API CmdMySQLDataBaseInsertCombine : public GameCommand
{
	BASE(CmdMySQLDataBaseInsertCombine, GameCommand);
public:
	void reset() override
	{
		mTable0 = nullptr;
		mTable1 = nullptr;
		mData0 = nullptr;
		mData1 = nullptr;
	}
	void execute() override;
public:
	MySQLTable* mTable0 = nullptr;
	MySQLTable* mTable1 = nullptr;
	MySQLData* mData0 = nullptr;
	MySQLData* mData1 = nullptr;
};
#endif