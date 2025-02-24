#include "FrameHeader.h"

TimeLock::TimeLock(const llong frameTimeMS, const llong forceSleep):
	mFrameTimeMS(frameTimeMS),
	mForceSleep(forceSleep),
	mLastTime(getRealTimeMS()),
	mCurTime(mLastTime)
{
	clampMinRef(mForceSleep, (llong)1);
}

llong TimeLock::update()
{
	const llong endTime = getRealTimeMS();
	sleep((ulong)clampMin((llong)mFrameTimeMS - (endTime - mLastTime), mForceSleep));
	mCurTime = getRealTimeMS();
	const llong frameTime = mCurTime - mLastTime;
	mLastTime = mCurTime;
	return frameTime;
}