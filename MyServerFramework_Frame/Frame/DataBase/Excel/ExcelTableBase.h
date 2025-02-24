#pragma once

#include "FrameBase.h"

class MICRO_LEGEND_FRAME_API ExcelTableBase
{
public:
	virtual ~ExcelTableBase() = default;
	void setTableName(const string& tableName) { mTableName = tableName; }
	const string& getTableName() const { return mTableName; }
	void init(const string& fullPath) {}
	virtual void checkAllData() {}
protected:
	string mTableName;
};