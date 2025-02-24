#pragma once

#include "FrameDefine.h"
#include "ThreadLock.h"

// 自动加锁解锁的类,搭配宏THREAD_LOCK更方便使用
// 用法:
// ThreadLock lock;
// THREAD_LOCK(lock)
class MICRO_LEGEND_FRAME_API ThreadLockScope
{
public:
	ThreadLockScope(ThreadLock& lock, const char* file, int line)
	{
		mLock = &lock;
		mLock->waitForUnlock(file, line);
	}
	~ThreadLockScope()
	{
		mLock->unlock();
	}
protected:
	ThreadLock* mLock = nullptr;
};