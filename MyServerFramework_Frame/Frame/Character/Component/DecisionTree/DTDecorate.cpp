#include "FrameHeader.h"

TASK_STATE DTDecorate::start()
{
	base::start();
	if (mChildList == nullptr || mChildList->size() == 0)
	{
		mState = TASK_STATE::SUCCESS;
		return mState;
	}
	mChild = (*mChildList)[0];
	return mState;
}

void DTDecorate::clearChild()
{
	base::clearChild();
	mChild = nullptr;
}
void DTDecorate::reset()
{
	base::reset();
	mChild = nullptr;
}
void DTDecorate::resetProperty()
{
	base::resetProperty();
	mChild = nullptr;
}

void DTDecorate::addChild(DTNode* child)
{
	if (mChildList != nullptr && mChildList->size() > 0)
	{
		ERROR("装饰节点最多只能加一个子节点");
		return;
	}
	base::addChild(child);
}