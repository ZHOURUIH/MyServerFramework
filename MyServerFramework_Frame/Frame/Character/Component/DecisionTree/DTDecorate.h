#pragma once

#include "DTNode.h"

class MICRO_LEGEND_FRAME_API DTDecorate : public DTNode
{
	BASE(DTDecorate, DTNode);
public:
	TASK_STATE start() override;
	void clearChild() override;
	void reset() override;
	void resetProperty() override;
	void addChild(DTNode* child)override;
protected:
	DTNode* mChild = nullptr;
};