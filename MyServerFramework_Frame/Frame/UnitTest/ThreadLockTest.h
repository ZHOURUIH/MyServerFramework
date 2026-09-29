#pragma once
#include "FrameMacro.h"

class MICRO_LEGEND_FRAME_API ThreadLockTest
{
#ifdef WINDOWS
public:
    static void test();
private:
    static void testLockUnlock();
    static void testSetEnable();
    static void testDisabledLockDoesNotBlock();
    static void testTimeLockBasic();
    static void testTimeLockSetFrameTime();
    static void testTimeLockSetForceSleepTime();
    static void testTimeLockMultipleUpdates();
    static void testTimeLockReturnPositive();
    static void testThreadLockScopeRAII();
    static void testThreadLockScopeEnableDisable();
    static void testTimeLockLowFPS();
    static void testTimeLockHighFPS();
    static void testTimeLock30FPS();
    static void testTimeLock60FPS();
#endif
};
