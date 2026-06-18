#pragma once

#include "EDTest.h"
#include "ExcelTable.h"

class ExcelTest : public ExcelTable<EDTest>
{
	BASE(ExcelTest, ExcelTable<EDTest>);
public:
	// auto generate start
	void checkAllDataDefault() override;
	// auto generate end
};