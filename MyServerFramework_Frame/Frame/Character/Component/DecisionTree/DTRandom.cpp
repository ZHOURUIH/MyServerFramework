#include "FrameHeader.h"

TASK_STATE DTRandom::start()
{
	base::start();
	// 随机选择一个子节点开始执行
	if (mChildList != nullptr)
	{
		mCurChildIndex = randomInt(0, mChildList->size() - 1);
		DTNode* child = (*mChildList)[mCurChildIndex];
		if (child->getState() == TASK_STATE::NONE)
		{
			child->start();
		}
	}
	
	return checkState();
}

TASK_STATE DTRandom::update(const float elapsedTime)
{
	base::update(elapsedTime);

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
	if (child->getState() == TASK_STATE::RUNNING)
	{
		child->update(elapsedTime);
	}
	return checkState();
}

TASK_STATE DTRandom::startExecute(DTNode* startChild)
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

void DTRandom::notifyChildStart(DTNode* child)
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

void DTRandom::clearChild()
{
	base::clearChild();
	mCurChildIndex = 0;
}

TASK_STATE DTRandom::checkState()
{
	if (mChildList == nullptr)
	{
		mState = TASK_STATE::FAILED;
		return mState;
	}
	mState = (*mChildList)[mCurChildIndex]->getState();
	// 如果节点已经执行完毕,则重置所有节点
	if (mState != TASK_STATE::RUNNING)
	{
		for (DTNode* child : *mChildList)
		{
			child->reset();
		}
	}
	return mState;
}