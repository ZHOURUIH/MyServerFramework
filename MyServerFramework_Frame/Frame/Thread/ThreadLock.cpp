#include "FrameHeader.h"

void ThreadLock::waitForUnlock(const char* file, const int line)
{
	if (!mEnable)
	{
		return;
	}
#ifdef WINDOWS
	llong time0 = getRealTimeMS();
#endif
	// 原子自旋操作
	// 如果mLock当前为true,则设置为true,并继续等待,如果mLock为false,则设置为true,不再循环等待
	while (mLock.test_and_set(std::memory_order_acquire))
	{
		SystemUtility::sleep(0);
	}
#ifdef WINDOWS
	llong time1 = getRealTimeMS();
	if (time1 - time0 > 1)
	{
		LOG_NO_WRITE("锁等待时间:" + IToS(int(time1 - time0)) +
			"毫秒,file:" + getFileName(file) + ":" + IToS(line) + 
			", 当前锁文件:" + getFileName(mFile) + ":" + IToS(mLine));
	}
	mFile = file;
#endif
	mLine = line;
	mThreadID = getThreadID();
}

void ThreadLock::unlock()
{
	if (!mEnable)
	{
		return;
	}
	mLock.clear(std::memory_order_release);
}