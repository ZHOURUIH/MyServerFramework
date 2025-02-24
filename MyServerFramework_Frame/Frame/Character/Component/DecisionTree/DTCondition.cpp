#include "FrameHeader.h"

void DTCondition::addChild(DTNode* child)
{
	ERROR("条件节点无法添加子节点");
}

TASK_STATE DTCondition::update(const float elapsedTime)
{
	base::update(elapsedTime);
	ERROR("条件节点不执行update,只执行start");
	return TASK_STATE::FAILED;
}