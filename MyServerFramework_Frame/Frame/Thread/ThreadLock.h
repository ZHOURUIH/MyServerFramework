#pragma once

#include "FrameDefine.h"

class MICRO_LEGEND_FRAME_API ThreadLock
{
public:
	ThreadLock():
		mEnable(true)
	{
		mLock.clear(std::memory_order_release);
	}
	virtual ~ThreadLock() = default;
	void waitForUnlock(const char* file, int line);
	void unlock();
	void setEnable(const bool enable)	{ mEnable = enable; }
	bool isEnable() const				{ return mEnable; }
public:
	atomic_flag mLock;				// true表示锁定,false表示未锁定
	atomic<bool> mEnable;			// true表示线程锁启用,false表示线程锁被禁用
	int mLine = 0;					// 加锁的行号
	int mThreadID = 0;				// 加锁线程的ID
#ifdef WINDOWS
	string mFile;					// 加锁的文件名
#endif
};