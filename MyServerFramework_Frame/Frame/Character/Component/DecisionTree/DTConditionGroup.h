#pragma once

#include "DTControl.h"

// 条件组,只能添加条件为子节点
class MICRO_LEGEND_FRAME_API DTConditionGroup : public DTControl
{
	BASE(DTConditionGroup, DTControl);
public:
	void reset() override
	{
		base::reset();
	}
	void resetProperty() override
	{
		base::resetProperty();
	}
	bool isCondition() override { return true; }
	void addChild(DTNode* child) override;
protected:
	// 设置的参数
protected:
	// 运行时参数
};