#pragma once

#include "DTConditionGroup.h"

// 逻辑与的条件组
class MICRO_LEGEND_FRAME_API DTConditionGroupAnd : public DTConditionGroup
{
	BASE(DTConditionGroupAnd, DTConditionGroup);
public:
	TASK_STATE start() override;
	void reset() override
	{
		base::reset();
	}
	void resetProperty() override
	{
		base::resetProperty();
	}
protected:
	// 设置的参数
protected:
	// 运行时参数
};