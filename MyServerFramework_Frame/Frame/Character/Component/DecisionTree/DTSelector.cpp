#include "FrameHeader.h"

TASK_STATE DTSelector::start()
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

TASK_STATE DTSelector::update(const float elapsedTime)
{
	base::update(elapsedTime);

	// 执行当前节点
	if (mChildList != nullptr)
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
	}

	return checkState();
}

TASK_STATE DTSelector::startExecute(DTNode* startChild)
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

void DTSelector::notifyChildStart(DTNode* child)
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

void DTSelector::clearChild()
{
	base::clearChild();
	mCurChildIndex = 0;
}
void DTSelector::reset()
{
	base::reset();
	mCurChildIndex = 0;
}
void DTSelector::resetProperty()
{
	base::resetProperty();
	mCurChildIndex = 0;
}

TASK_STATE DTSelector::checkState()
{
	if (mChildList == nullptr)
	{
		mState = TASK_STATE::FAILED;
		return mState;
	}
	const DTNode* curChild = (*mChildList)[mCurChildIndex];
	const TASK_STATE curChildState = curChild->getState();
	// 当前子节点执行成功,则返回成功
	if (curChildState == TASK_STATE::SUCCESS)
	{
		mState = TASK_STATE::SUCCESS;
	}
	// 当前子节点执行失败
	else if (curChildState == TASK_STATE::FAILED)
	{
		// 开始执行下一个子节点
		if (mCurChildIndex != mChildList->size() - 1)
		{
			++mCurChildIndex;
			mState = TASK_STATE::RUNNING;
		}
		// 所有子节点执行失败,返回失败
		else
		{
			mState = TASK_STATE::FAILED;
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
		for (DTNode* child : *mChildList)
		{
			child->reset();
		}
	}
	return mState;
}