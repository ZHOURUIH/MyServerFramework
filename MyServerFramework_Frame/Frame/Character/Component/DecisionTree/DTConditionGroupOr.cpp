#include "FrameHeader.h"

TASK_STATE DTConditionGroupOr::start()
{
	base::start();
	bool success = false;
	if (mChildList != nullptr)
	{
		for (DTNode* child : *mChildList)
		{
			if (child->start() == TASK_STATE::SUCCESS)
			{
				success = true;
				break;
			}
		}
	}
	mState = success ? TASK_STATE::SUCCESS : TASK_STATE::FAILED;
	return mState;
}