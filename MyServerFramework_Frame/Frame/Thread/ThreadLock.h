#pragma once

#include "FrameDefine.h"

class MICRO_LEGEND_FRAME_API ThreadLock
{
public:
	ThreadLock():
		mEnable(true)
	{
		mLock.clear(memory_order_release);
	}
	virtual ~ThreadLock() = default;
	void waitForUnlock(const char* file, int line);
	void unlock();
	void setEnable(const bool enable)	{ mEnable.store(enable, memory_order_relaxed); }
	bool isEnable() const				{ return mEnable.load(memory_order_relaxed); }
	static void dump();
private:
	void recordOwner(const char* file, int line);
public:
	atomic_flag mLock;				// true表示锁定,false表示未锁定
	atomic<bool> mEnable;			// true表示线程锁启用,false表示线程锁被禁用
#ifdef WINDOWS
	// __FILE__是静态字符串,只保存指针即可,避免每次加锁都复制std::string。
	// 仅用于发生长时间锁竞争时打印当前持锁位置。
	atomic<const char*> mFile{ nullptr };
	atomic<int> mLine{ 0 };
#endif
	static atomic<uint64_t> mLockTicks;
	static atomic<uint64_t> mLockTimes;
	static uint64_t mStartTimeTick;
};