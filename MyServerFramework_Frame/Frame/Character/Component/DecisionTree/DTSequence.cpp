#include "FrameHeader.h"

TASK_STATE DTSequence::start()
{
	base::start();
	mCurChildIndex = 0;
	if (mChildList == nullptr || mChildList->size() == 0)
	{
		mState = TASK_STATE::SUCCESS;
		return mState;
	}
	DTNode* child = (*mChildList)[mCurChildIndex];
	if (child->getState() == TASK_STATE::NONE)
	{
		child->start();
	}
	return checkState();
}

TASK_STATE DTSequence::update(const float elapsedTime)
{
	base::update(elapsedTime);

	// 执行当前节点
	while (mChildList != nullptr)
	{
		DTNode* child = (*mChildList)[mCurChildIndex];
		if (child->getState() == TASK_STATE::NONE)
		{
			child->start();
		}
		if (child->getState() == TASK_STATE::RUNNING)
		{
			child->update(elapsedTime);
		}
		// 如果执行成功,则立即执行下一个节点,不用等到下一帧
		if (child->getState() == TASK_STATE::SUCCESS)
		{
			if (mCurChildIndex == mChildList->size() - 1)
			{
				break;
			}
			++mCurChildIndex;
		}
		else
		{
			break;
		}
	}

	return checkState();
}

TASK_STATE DTSequence::startExecute(DTNode* startChild)
{
	base::startExecute(startChild);
	if (mChildList == nullptr)
	{
		mState = TASK_STATE::FAILED;
		return mState;
	}
	mCurChildIndex = mChildList->findFirstIndex(startChild);
	startChild->start();
	if (mCurChildIndex < 0)
	{
		mState = TASK_STATE::FAILED;
		return mState;
	}
	return checkState();
}

void DTSequence::notifyChildStart(DTNode* child)
{
	base::notifyChildStart(child);
	if (mChildList == nullptr)
	{
		mState = TASK_STATE::FAILED;
		return;
	}
	mCurChildIndex = mChildList->findFirstIndex(child);
	if (mCurChildIndex < 0)
	{
		ERROR("在子节点列表中找不到指定子节点");
		mState = TASK_STATE::FAILED;
		return;
	}
	// 中断其他正在运行的节点
	for (DTNode* curChild : *mChildList)
	{
		if (curChild != child && curChild->getState() == TASK_STATE::RUNNING)
		{
			curChild->abort();
		}
	}
}

void DTSequence::clearChild()
{
	base::clearChild();
	mCurChildIndex = 0;
}
void DTSequence::reset()
{
	base::reset();
	mCurChildIndex = 0;
}
void DTSequence::resetProperty()
{
	base::resetProperty();
	mCurChildIndex = 0;
}

TASK_STATE DTSequence::checkState()
{
	if (mChildList == nullptr)
	{
		mState = TASK_STATE::FAILED;
		return mState;
	}
	const TASK_STATE curChildState = (*mChildList)[mCurChildIndex]->getState();
	// 当前子节点执行失败,则返回失败
	if (curChildState == TASK_STATE::FAILED)
	{
		mState = TASK_STATE::FAILED;
	}
	// 当前子节点执行成功
	else if (curChildState == TASK_STATE::SUCCESS)
	{
		// 开始执行下一个子节点
		if (mCurChildIndex != mChildList->size() - 1)
		{
			++mCurChildIndex;
			mState = TASK_STATE::RUNNING;
		}
		// 所有子节点执行成功,返回成功
		else
		{
			mState = TASK_STATE::SUCCESS;
		}
	}
	// 有子节点正在执行
	else
	{
		mState = TASK_STATE::RUNNING;
	}

	// 如果整个节点已经执行完毕,则重置所有节点
	if (mState != TASK_STATE::RUNNING)
	{
		for (DTNode* curChild : *mChildList)
		{
			curChild->reset();
		}
	}
	return mState;
}