// auto generate start
#pragma once

#include "ExcelData.h"
#include "GameEnum.h"

// 全局变量表格
class EDGlobal : public ExcelData
{
	BASE(EDGlobal, ExcelData);
public:
	string mParamName;											// 参数名
	string mParamType;											// 参数类型
	string mParamValue;											// 参数值
public:
	void cloneTo(ExcelData* target) override;
	void read(SerializerRead* reader) override;
	static void postLoadAll(ExcelTableBase* tableBase){}
};
// auto generate end