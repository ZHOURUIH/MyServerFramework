#include "FrameHeader.h"

TASK_STATE DTRepeater::start()
{
	base::start();
	if (mChild == nullptr)
	{
		mState = TASK_STATE::FAILED;
		return mState;
	}
	mCurRepeat = 0;
	mChild->start();
	return checkState();
}

TASK_STATE DTRepeater::update(const float elapsedTime)
{
	base::update(elapsedTime);

	if (mChild == nullptr)
	{
		mState = TASK_STATE::SUCCESS;
		return mState;
	}
	TASK_STATE state = mChild->getState();
	if (state != TASK_STATE::RUNNING)
	{
		state = mChild->start();
	}
	if (state == TASK_STATE::RUNNING)
	{
		mChild->update(elapsedTime);
	}
	return checkState();
}

void DTRepeater::reset()
{
	base::reset();
	mCurRepeat = 0;
}
void DTRepeater::resetProperty()
{
	base::resetProperty();
	mMaxRepeat = 0;
	mCurRepeat = 0;
}

TASK_STATE DTRepeater::checkState()
{
	const TASK_STATE curChildState = mChild->getState();
	if (curChildState == TASK_STATE::RUNNING)
	{
		mState = TASK_STATE::RUNNING;
		return mState;
	}
	mChild->reset();
	// 子节点执行完以后循环计数
	if (mMaxRepeat <= 0)
	{
		mState = TASK_STATE::RUNNING;
		return mState;
	}
	++mCurRepeat;
	if (mCurRepeat >= mMaxRepeat)
	{
		mState = TASK_STATE::SUCCESS;
	}
	else
	{
		mState = TASK_STATE::RUNNING;
	}
	return mState;
}