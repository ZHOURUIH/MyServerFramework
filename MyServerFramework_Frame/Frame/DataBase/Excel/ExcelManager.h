#pragma once

#include "FrameComponent.h"

class MICRO_LEGEND_FRAME_API ExcelManager : public FrameComponent
{
	BASE(ExcelManager, FrameComponent);
public:
	void quit() override;
	ExcelTableBase* getTable(const string& name) const { return mTableList.tryGet(name); }
	template<typename T>
	T* registeExcel(const char* tableName)
	{
		T* table = new T();
		table->init(tableName);
		mTableList.insert(tableName, table);
		return table;
	}
	void checkAll();
public:
	HashMap<string, ExcelTableBase*> mTableList;
};