#include "FrameHeader.h"

atomic<uint64_t> ThreadLock::mLockTicks = 0;
atomic<uint64_t> ThreadLock::mLockTimes = 0;
uint64_t ThreadLock::mStartTimeTick = 0;

void ThreadLock::recordOwner(const char* file, const int line)
{
#ifdef WINDOWS
	mFile.store(file, memory_order_relaxed);
	mLine.store(line, memory_order_relaxed);
#endif
}

void ThreadLock::waitForUnlock(const char* file, const int line)
{
	if (!mEnable.load(memory_order_relaxed))
	{
		return;
	}
	mLockTimes.fetch_add(1, memory_order_relaxed);
	// 无竞争是绝大多数情况。成功后只记录两个原子标量,不复制string,也不查询线程ID。
	if (!mLock.test_and_set(memory_order_acquire))
	{
		recordOwner(file, line);
		return;
	}
#ifdef WINDOWS
	const uint64_t start = readTSC();
#endif
	// 只有第一次获取失败后才进入真正的竞争等待和等待时间统计。
	while (mLock.test_and_set(memory_order_acquire))
	{
		SystemUtility::sleep(0);
	}
#ifdef WINDOWS
	const uint64_t end = readTSC();
	const uint64_t ticks = end - start;
	mLockTicks.fetch_add(ticks, memory_order_relaxed);
	const double ms = Profiler::ticksToMS(ticks);
	if (ms > 5)
	{
		const char* ownerFile = mFile.load(memory_order_relaxed);
		const int ownerLine = mLine.load(memory_order_relaxed);
		LOG_NO_WRITE("锁等待时间:" + FToS((float)ms, 1) +
			"毫秒,file:" + getFileName(file) + ":" + IToS(line) +
			", 当前锁文件:" + getFileName(ownerFile != nullptr ? ownerFile : "") + ":" + IToS(ownerLine));
	}
#endif
	recordOwner(file, line);
}

void ThreadLock::unlock()
{
	if (!mEnable.load(memory_order_relaxed))
	{
		return;
	}
	mLock.clear(memory_order_release);
}

void ThreadLock::dump()
{
#ifdef STRESS_TEST
	const uint64_t ticks = mLockTicks.exchange(0, memory_order_relaxed);
	if (mStartTimeTick > 0)
	{
		const uint64_t end = readTSC();
		const double elapsedSecond = Profiler::ticksToMS(end - mStartTimeTick) / 1000.0;
		if (elapsedSecond > 1e-6)
		{
			const float lockTimePerSecond = (float)(Profiler::ticksToMS(ticks) / elapsedSecond);
			LOG("平均每秒锁等待总时间:" + FToS(lockTimePerSecond) + "毫秒");
		}
		LOG("平均每秒加锁次数:" + LLToS(mLockTimes.load(memory_order_relaxed)));
	}
#else
	mLockTicks.exchange(0, memory_order_relaxed);
#endif
	mLockTimes.store(0, memory_order_relaxed);
	mStartTimeTick = readTSC();
}
