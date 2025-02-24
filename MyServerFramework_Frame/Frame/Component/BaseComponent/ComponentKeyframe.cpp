#include "FrameHeader.h"

void ComponentKeyframe::onFrameTick(const float elapsedTime)
{
	if (mKeyframe == nullptr || mPlayState != PLAY_STATE::PLAY)
	{
		return;
	}
	mCurrentTime += elapsedTime;
	bool done = false;
	// 无限播放当前震动
	if (mPlayLength < 0.0f)
	{
		if (mCurrentTime > mOnceLength)
		{
			mCurrentTime = 0.0f;
		}
	}
	// 播放固定长度的震动
	else
	{
		// 超过时间则停止,暂时不播放最后一帧
		mPlayedTime += elapsedTime;
		if (mPlayedTime > mPlayLength)
		{
			done = true;
			mCurrentTime = mPlayLength;
		}
		else if (mCurrentTime > mOnceLength)
		{
			mCurrentTime = 0.0f;
		}
	}
	if (mKeyframe != nullptr)
	{
		const float value = mKeyframe->evaluate(mCurrentTime * mInverseOnceLength);
		applyTrembling(value);
		afterApplyTrembling(done);
	}
}

void ComponentKeyframe::play(const int keyframeID, const bool loop, const float onceLength)
{
	if (!isActive())
	{
		return;
	}
	setKeyframeID(keyframeID);
	mKeyframe = mKeyframeManager->getKeyFrame(mKeyframeID);
	if (onceLength < 0.0f)
	{
		ERROR("onceLength can not be negative!");
	}
	if (mKeyframe == nullptr || isZero(onceLength))
	{
		mStopValue = 0.0f;
		// 停止并禁用组件
		afterApplyTrembling(true);
		return;
	}
	mStopValue = mKeyframe->evaluate(mKeyframe->length());
	setOnceLength(onceLength);
	mPlayState = PLAY_STATE::PLAY;
	mLoop = loop;
	mCurrentTime = 0.0f;
	mPlayedTime = 0.0f;
	mPlayLength = mLoop  ? -1.0f : mOnceLength;
	onFrameTick(0.0f);
}

void ComponentKeyframe::stop(const bool force)
{
	// 如果已经是停止的状态,并且不是要强制停止,则不再执行
	if (mPlayState == PLAY_STATE::STOP && !force)
	{
		return;
	}
	mPlayState = PLAY_STATE::STOP;
	mKeyframe = nullptr;
	mCurrentTime = 0.0f;
	mPlayedTime = 0.0f;
}

void ComponentKeyframe::setState(const PLAY_STATE state)
{
	if (mPlayState == state)
	{
		return;
	}
	if (state == PLAY_STATE::PLAY)
	{
		play(mKeyframeID, mLoop, mOnceLength);
	}
	else if (state == PLAY_STATE::STOP)
	{
		stop();
	}
	else if (state == PLAY_STATE::PAUSE)
	{
		pause();
	}
}

void ComponentKeyframe::resetProperty()
{
	base::resetProperty();
	mDoingCallback = nullptr;
	mDoneCallback = nullptr;
	mDoingUserData = nullptr;
	mDoneUserData = nullptr;
	mKeyframe = nullptr;
	mCurrentTime = 0.0f;
	mPlayedTime = 0.0f;
	mOnceLength = 1.0f;
	mPlayLength = 0.0f;
	mStopValue = 0.0f;
	mKeyframeID = 0;
	mLoop = true;
	mPlayState = PLAY_STATE::STOP;
}

//-------------------------------------------------------------------------------------------------------------------------------
void ComponentKeyframe::clearCallback()
{
	base::clearCallback();
	mDoingCallback = nullptr;
	mDoneCallback = nullptr;
	mDoingUserData = nullptr;
	mDoneUserData = nullptr;
}

void ComponentKeyframe::afterApplyTrembling(bool done)
{
	CALL(mDoingCallback, this, false, mDoingUserData);
	if (done)
	{
		setActive(false);
		// 强制停止组件
		stop(true);
		doneCallback(mDoneCallback, mDoneUserData, this);
	}
}

void ComponentKeyframe::doneCallback(const KeyFrameCallback& curDoneCallback, void*& userData, ComponentKeyframe* component)
{
	// 先保存回调,然后再调用回调之前就清空回调,确保在回调函数执行时已经完全完成
	const KeyFrameCallback tempCallback = curDoneCallback;
	void* tempUserData = userData;
	component->clearCallback();
	CALL(tempCallback, component, false, tempUserData);
}

void ComponentKeyframe::setCallback(KeyFrameCallback callback, void* userData, KeyFrameCallback& curCallback, void*& curUserData, ComponentKeyframe* component)
{
	const KeyFrameCallback tempCallback = curCallback;
	void* tempUserData = curUserData;
	curCallback = nullptr;
	curUserData = nullptr;
	// 如果回调函数当前不为空,则是中断了正在进行的变化
	CALL(tempCallback, component, true, tempUserData);
	curCallback = callback;
	curUserData = userData;
}