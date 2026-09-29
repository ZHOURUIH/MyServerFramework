#include "FrameHeader.h"

#ifdef WINDOWS
atomic<int> TimeLock::mTimerResolutionRefCount = 0;
#endif

TimeLock::TimeLock(const llong frameTimeMS, const llong forceSleep):
	mFrameTimeMS(frameTimeMS),
	mForceSleep(forceSleep),
	mLastTime(getRealTimeMS()),
	mCurTime(mLastTime)
{
	clampMinRef(mForceSleep, (llong)1);
#ifdef WINDOWS
	// 服务器的主循环和网络线程都依赖Sleep锁帧。Windows默认定时器粒度会把33ms/10ms
	// 放大到约46ms/15ms,直接增加应用层网络延迟。只在第一个TimeLock创建时申请1ms精度。
	if (mTimerResolutionRefCount.fetch_add(1, memory_order_acq_rel) == 0)
	{
		timeBeginPeriod(1);
	}
#endif
}

TimeLock::~TimeLock()
{
#ifdef WINDOWS
	if (mTimerResolutionRefCount.fetch_sub(1, memory_order_acq_rel) == 1)
	{
		timeEndPeriod(1);
	}
#endif
}

llong TimeLock::update()
{
	const llong endTime = getRealTimeMS();
	SystemUtility::sleep((ulong)clampMin((llong)mFrameTimeMS - (endTime - mLastTime), mForceSleep));
	mCurTime = getRealTimeMS();
	const llong frameTime = mCurTime - mLastTime;
	mLastTime = mCurTime;
	return frameTime;
}