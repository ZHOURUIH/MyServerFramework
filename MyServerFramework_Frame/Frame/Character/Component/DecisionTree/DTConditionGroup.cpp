#include "FrameHeader.h"

void DTConditionGroup::addChild(DTNode* child)
{
	if (!child->isCondition())
	{
		ERROR("条件组节点下不能添加非条件节点");
		return;
	}
	base::addChild(child);
}