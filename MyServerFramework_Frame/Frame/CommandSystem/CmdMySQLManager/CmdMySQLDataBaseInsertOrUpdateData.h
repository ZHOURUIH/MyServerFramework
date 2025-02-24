#pragma once

#ifdef _MYSQL
#include "GameCommand.h"

class MICRO_LEGEND_FRAME_API CmdMySQLDataBaseInsertOrUpdateData : public GameCommand
{
	BASE(CmdMySQLDataBaseInsertOrUpdateData, GameCommand);
public:
	void reset() override
	{
		mTable = nullptr;
		mData = nullptr;
	}
	void execute() override;
public:
	MySQLTable* mTable = nullptr;
	MySQLData* mData = nullptr;
};
#endif