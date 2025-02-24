#pragma once

#include "SQLiteDataReader.h"

class MICRO_LEGEND_FRAME_API SelectCount
{
public:
	int mRowCount = 0;
public:
	void parse(SQLiteDataReader* reader)
	{
		mRowCount = reader->getInt(0);
	}
};