#include "FrameHeader.h"

TASK_STATE DTConditionGroupAnd::start()
{
	base::start();
	bool success = true;
	if (mChildList != nullptr)
	{
		for (DTNode* child : *mChildList)
		{
			if (child->start() != TASK_STATE::SUCCESS)
			{
				success = false;
				break;
			}
		}
	}
	mState = success ? TASK_STATE::SUCCESS : TASK_STATE::FAILED;
	return mState;
}