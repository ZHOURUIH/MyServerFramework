#pragma once

#include "DTNode.h"

class MICRO_LEGEND_FRAME_API DTCondition : public DTNode
{
	BASE(DTCondition, DTNode);
public:
	bool isCondition() override { return true; }
	void addChild(DTNode* child) override;
	TASK_STATE update(const float elapsedTime) override;
protected:
};