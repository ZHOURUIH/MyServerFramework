#pragma once

#include "EDAchivement.h"
#include "ExcelTable.h"

class ExcelAchivement : public ExcelTable<EDAchivement>
{
	BASE(ExcelAchivement, ExcelTable<EDAchivement>);
public:
	// auto generate start
	void checkAllDataDefault() override;
	// auto generate end
};