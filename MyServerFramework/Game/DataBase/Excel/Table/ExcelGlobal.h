#pragma once

#include "EDGlobal.h"
#include "ExcelTable.h"

class ExcelGlobal : public ExcelTable<EDGlobal>
{
	BASE(ExcelGlobal, ExcelTable<EDGlobal>);
public:
	// auto generate start
	void checkAllDataDefault() override;
	// auto generate end
};