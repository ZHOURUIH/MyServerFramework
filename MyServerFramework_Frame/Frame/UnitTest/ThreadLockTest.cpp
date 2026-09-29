#ifdef WINDOWS
#include "FrameHeader.h"

#define TLT_ASSERT(expr, msg) \
    do { if (!(expr)) { ERROR(string("ThreadLockTest FAILED: ") + (msg)); } } while(0)

void ThreadLockTest::test()
{
    testLockUnlock();
    testSetEnable();
    testDisabledLockDoesNotBlock();
    testTimeLockBasic();
    testTimeLockSetFrameTime();
    testTimeLockSetForceSleepTime();
    testTimeLockMultipleUpdates();
    testTimeLockReturnPositive();
    testThreadLockScopeRAII();
    testThreadLockScopeEnableDisable();
    testTimeLockLowFPS();
    testTimeLockHighFPS();
    testTimeLock30FPS();
    testTimeLock60FPS();
}

void ThreadLockTest::testLockUnlock()
{
    ThreadLock lock;
    TLT_ASSERT(lock.isEnable(), "lock default enabled");
    lock.waitForUnlock(__FILE__, __LINE__);
    lock.unlock();
    lock.waitForUnlock(__FILE__, __LINE__);
    lock.unlock();
}

void ThreadLockTest::testSetEnable()
{
    ThreadLock lock;
    TLT_ASSERT(lock.isEnable(), "default enabled");
    lock.setEnable(false);
    TLT_ASSERT(!lock.isEnable(), "after setEnable(false)");
    lock.setEnable(true);
    TLT_ASSERT(lock.isEnable(), "after setEnable(true)");
}

void ThreadLockTest::testDisabledLockDoesNotBlock()
{
    ThreadLock lock;
    lock.setEnable(false);
    lock.waitForUnlock(__FILE__, __LINE__);
    lock.unlock();
    lock.waitForUnlock(__FILE__, __LINE__);
    lock.unlock();
    TLT_ASSERT(!lock.isEnable(), "still disabled after lock/unlock");
}

void ThreadLockTest::testTimeLockBasic()
{
    TimeLock tl(33, 1);
    llong elapsed = tl.update();
    TLT_ASSERT(elapsed >= 0, "TimeLock update >= 0");
}

void ThreadLockTest::testTimeLockSetFrameTime()
{
    TimeLock tl(33, 1);
    tl.setFrameTime(16);
    llong elapsed = tl.update();
    TLT_ASSERT(elapsed >= 0, "TimeLock after setFrameTime(16)");
}

void ThreadLockTest::testTimeLockSetForceSleepTime()
{
    TimeLock tl(33, 1);
    tl.setForceSleepTime(2);
    llong elapsed = tl.update();
    TLT_ASSERT(elapsed >= 0, "TimeLock after setForceSleepTime(2)");
}

void ThreadLockTest::testTimeLockMultipleUpdates()
{
    TimeLock tl(1, 1);
    FOR(200)
    {
        llong e = tl.update();
        TLT_ASSERT(e >= 0, "TimeLock multiple updates >= 0");
    }
}

void ThreadLockTest::testTimeLockReturnPositive()
{
    TimeLock tl(5, 1);
    tl.update();
    llong e = tl.update();
    TLT_ASSERT(e >= 0, "TimeLock second update >= 0");
}

void ThreadLockTest::testThreadLockScopeRAII()
{
    ThreadLock lock;
    {
        ThreadLockScope scope(lock, __FILE__, __LINE__);
    }
    lock.waitForUnlock(__FILE__, __LINE__);
    lock.unlock();
}

void ThreadLockTest::testThreadLockScopeEnableDisable()
{
    ThreadLock lock;
    lock.setEnable(false);
    {
        ThreadLockScope scope(lock, __FILE__, __LINE__);
    }
    TLT_ASSERT(!lock.isEnable(), "lock still disabled after scope");
    lock.setEnable(true);
    {
        ThreadLockScope scope(lock, __FILE__, __LINE__);
    }
    TLT_ASSERT(lock.isEnable(), "lock still enabled after scope");
}

void ThreadLockTest::testTimeLockLowFPS()
{
    TimeLock tl(100, 1);
    llong e = tl.update();
    TLT_ASSERT(e >= 0, "TimeLock 100ms frame >= 0");
}

void ThreadLockTest::testTimeLockHighFPS()
{
    TimeLock tl(1, 1);
    for (int i = 0; i < 3; ++i)
    {
        llong e = tl.update();
        TLT_ASSERT(e >= 0, "TimeLock 1ms frame >= 0");
    }
}

void ThreadLockTest::testTimeLock30FPS()
{
    TimeLock tl(33, 1);
    llong total = 0;
    FOR(4)
    {
        total += tl.update();
    }
    TLT_ASSERT(total >= 0, "TimeLock 30fps total >= 0");
}

void ThreadLockTest::testTimeLock60FPS()
{
    TimeLock tl(16, 1);
    llong total = 0;
    FOR(4)
    {
        total += tl.update();
    }
    TLT_ASSERT(total >= 0, "TimeLock 60fps total >= 0");
}
#endif
