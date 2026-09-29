#pragma once
#include "FrameMacro.h"

class MICRO_LEGEND_FRAME_API TickerTest
{
#ifdef WINDOWS
public:
    static void test();
private:
    static void testTickerResetProperty();
    static void testTickerSetCallback();
    static void testTickerIsFinishDefault();
    static void testTickerUpdateCallbackTrue();
    static void testTickerUpdateCallbackFalse();
    static void testTickerUpdateNoCallback();
    static void testTickerUpdateMultipleTimes();
    static void testTickerIsFinishAfterFalseCallback();
    static void testTickerCallbackCountExact();
    static void testTickerResetAfterFinish();
    static void testTickerCallbackAccumulated();
    static void testTickerCallbackWithCapture();
    static void testTickerResetPropertyClearsCallback();
    static void testTickerMultipleCallbackSwap();
    static void testTickerCallbackReturnsTrueMany();
#endif
};
