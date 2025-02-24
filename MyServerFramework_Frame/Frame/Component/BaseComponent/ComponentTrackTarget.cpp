#include "FrameHeader.h"

void ComponentTrackTarget::onFrameTick(const float elapsedTime)
{
	if (mTarget == nullptr)
	{
		return;
	}
	const Vector3 targetPos = getTargetPosition();
	const Vector3 curPos = getPosition();
	const float moveDelta = mSpeed * elapsedTime;
	const Vector3 delta = targetPos - curPos;
	const bool done = !lengthGreater(delta, moveDelta);
	setPosition(done ? targetPos : normalize(delta) * moveDelta + curPos);
	CALL(mDoingCallback, this, false, mDoingUserData);
	if (done)
	{
		CALL(mDoneCallback, this, false, mDoneUserData);
	}
}

void ComponentTrackTarget::setMoveDoneTrack(TrackCallback doneCallback, void* userData)
{
	const TrackCallback tempCallback = mDoneCallback;
	void* tempUserData = mDoneUserData;
	mDoneCallback = nullptr;
	mDoneUserData = nullptr;
	// 如果回调函数当前不为空,则是中断了正在进行的变化
	CALL(tempCallback, this, true, tempUserData);
	mDoneCallback = doneCallback;
	mDoneUserData = userData;
}

void ComponentTrackTarget::resetProperty()
{
	base::resetProperty();
	mTarget = nullptr;
	mSpeed = 0.0f;
	mTargetOffset.clear();
	mDoneCallback = nullptr;
	mDoingCallback = nullptr;
	mDoneUserData = nullptr;
	mDoingUserData = nullptr;
}