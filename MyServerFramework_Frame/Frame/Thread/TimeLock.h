#pragma once

#include "Utility.h"

class MICRO_LEGEND_FRAME_API TimeLock
{
public:
	explicit TimeLock(llong frameTimeMS, llong forceSleep = 5);
	virtual ~TimeLock();
	// 返回值表示上一帧经过的时间
	llong update();
	void setFrameTime(const llong frameTime)
	{
		mFrameTimeMS = frameTime;
		clampMinRef(mForceSleep, (llong)1);
	}
	void setForceSleepTime(const llong forceSleep) { mForceSleep = forceSleep; }
protected:
	llong mFrameTimeMS;	// 表示锁帧时每一帧的最小时间,15表示锁帧最高60帧
	llong mForceSleep;	// 每帧无暂停时间时强制暂停的毫秒数,避免线程单帧任务繁重时,导致单帧消耗时间大于设定的固定单帧时间时,CPU占用过高的问题
	llong mLastTime;	// 上一帧记录的时间点
	llong mCurTime;		// 当前记录的时间点
#ifdef WINDOWS
	// Windows默认Sleep粒度可能达到15.6ms。所有TimeLock共享1ms定时器精度,最后一个TimeLock销毁时恢复。
	static atomic<int> mTimerResolutionRefCount;
#endif
};