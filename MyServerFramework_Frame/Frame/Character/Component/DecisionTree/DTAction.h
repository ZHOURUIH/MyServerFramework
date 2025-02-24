#pragma once

#include "DTNode.h"

class MICRO_LEGEND_FRAME_API DTAction : public DTNode
{
	BASE(DTAction, DTNode);
public:
	void addChild(DTNode* child) override
	{
		ERROR("行为节点无法添加子节点");
	}
};