#include "FrameHeader.h"

TASK_STATE DTParallel::start()
{
	base::start();
	// 开始执行所有节点
	if (mChildList != nullptr)
	{
		for (DTNode* child : *mChildList)
		{
			if (child->getState() == TASK_STATE::NONE)
			{
				child->start();
			}
		}
	}
	return checkState();
}

TASK_STATE DTParallel::update(const float elapsedTime)
{
	base::update(elapsedTime);

	if (mChildList != nullptr)
	{
		for (DTNode* child : *mChildList)
		{
			if (child->getState() == TASK_STATE::RUNNING)
			{
				child->update(elapsedTime);
			}
		}
	}
	return checkState();
}

TASK_STATE DTParallel::startExecute(DTNode* startChild)
{
	base::startExecute(startChild);

	// 因为是并行的,所以还是会同时执行所有节点
	if (mChildList != nullptr)
	{
		for (DTNode* child : *mChildList)
		{
			if (child->getState() == TASK_STATE::NONE)
			{
				child->start();
			}
		}
	}
	return checkState();
}

TASK_STATE DTParallel::checkState()
{
	// 是否有子节点执行失败
	bool hasFailed = false;
	if (mChildList != nullptr)
	{
		for (const DTNode* child : *mChildList)
		{
			if (child->getState() == TASK_STATE::FAILED)
			{
				hasFailed = true;
				break;
			}
		}
	}

	if (hasFailed)
	{
		mState = TASK_STATE::FAILED;
	}
	else
	{
		// 是否已经全部执行成功,子节点无状态也认为是执行成功,因为可能是并行节点执行完毕重置后,其中某个节点独自开始执行
		// 就会出现其他节点为无状态
		mState = TASK_STATE::SUCCESS;
		if (mChildList != nullptr)
		{
			for (const DTNode* child : *mChildList)
			{
				const TASK_STATE childState = child->getState();
				if (childState != TASK_STATE::SUCCESS && childState != TASK_STATE::NONE)
				{
					mState = TASK_STATE::RUNNING;
					break;
				}
			}
		}
	}

	// 如果节点已经执行完毕,则重置所有节点
	if (mState != TASK_STATE::RUNNING)
	{
		if (mChildList != nullptr)
		{
			for (DTNode* child : *mChildList)
			{
				child->reset();
			}
		}
	}
	return mState;
}