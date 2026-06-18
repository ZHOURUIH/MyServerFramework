// auto generate start
#pragma once

#include "ExcelData.h"
#include "GameEnum.h"

// 成就表
class EDAchivement : public ExcelData
{
	BASE(EDAchivement, ExcelData);
public:
	int mTestList1 = 0;										// 完成成就所需的值,此值应该与Condition中的值一致,只是方便获取
	int mReward = 0;										// 完成成就可获得的奖励,索引到Reward表
public:
	void cloneTo(ExcelData* target) override;
	void read(SerializerRead* reader) override;
};
// auto generate end