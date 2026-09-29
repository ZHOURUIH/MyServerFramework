#ifdef WINDOWS
#include "FrameHeader.h"

#define TICK_ASSERT(expr, msg) \
    do { if (!(expr)) { ERROR(string("TickerTest FAILED: ") + (msg)); } } while(0)

void TickerTest::test()
{
    testTickerResetProperty();
    testTickerSetCallback();
    testTickerIsFinishDefault();
    testTickerUpdateCallbackTrue();
    testTickerUpdateCallbackFalse();
    testTickerUpdateNoCallback();
    testTickerUpdateMultipleTimes();
    testTickerIsFinishAfterFalseCallback();
    testTickerCallbackCountExact();
    testTickerResetAfterFinish();
    testTickerCallbackAccumulated();
    testTickerCallbackWithCapture();
    testTickerResetPropertyClearsCallback();
    testTickerMultipleCallbackSwap();
    testTickerCallbackReturnsTrueMany();
}

void TickerTest::testTickerResetProperty()
{
    Ticker t;
    t.resetProperty();
    TICK_ASSERT(!t.isFinish(), "resetProperty: isFinish==false");
}

void TickerTest::testTickerSetCallback()
{
    Ticker t;
    int counter = 0;
    t.setCallback([&counter]() -> bool { ++counter; return true; });
    TICK_ASSERT(!t.isFinish(), "setCallback clears finish");
}

void TickerTest::testTickerIsFinishDefault()
{
    Ticker t;
    TICK_ASSERT(!t.isFinish(), "default isFinish==false");
}

void TickerTest::testTickerUpdateCallbackTrue()
{
    Ticker t;
    int callCount = 0;
    t.setCallback([&callCount]() -> bool { ++callCount; return true; });
    t.update(0.016f);
    TICK_ASSERT(callCount == 1, "update called callback once");
    TICK_ASSERT(!t.isFinish(), "callback returns true => not finish");
}

void TickerTest::testTickerUpdateCallbackFalse()
{
    Ticker t;
    int callCount = 0;
    t.setCallback([&callCount]() -> bool { ++callCount; return false; });
    t.update(0.016f);
    TICK_ASSERT(callCount == 1, "update called callback once");
    TICK_ASSERT(t.isFinish(), "callback returns false => finish");
}

void TickerTest::testTickerUpdateNoCallback()
{
    Ticker t;
    t.update(0.016f);
    TICK_ASSERT(!t.isFinish(), "update with no callback => not finish");
}

void TickerTest::testTickerUpdateMultipleTimes()
{
    Ticker t;
    int callCount = 0;
    t.setCallback([&callCount]() -> bool { ++callCount; return true; });
    t.update(0.016f);
    t.update(0.016f);
    t.update(0.016f);
    TICK_ASSERT(callCount == 3, "update 3 times callCount==3");
    TICK_ASSERT(!t.isFinish(), "all true => not finish");
}

void TickerTest::testTickerIsFinishAfterFalseCallback()
{
    Ticker t;
    t.setCallback([]() -> bool { return false; });
    t.update(0.1f);
    TICK_ASSERT(t.isFinish(), "false callback => finish");
    t.setCallback([]() -> bool { return true; });
    TICK_ASSERT(!t.isFinish(), "setCallback resets finish");
}

void TickerTest::testTickerCallbackCountExact()
{
    Ticker t;
    int n = 0;
    t.setCallback([&n]() -> bool
    {
        ++n;
        return n < 5;
    });
    FOR(10)
    {
        t.update(0.016f);
    }
    TICK_ASSERT(n == 5, "callback called exactly 5 times until false");
    TICK_ASSERT(t.isFinish(), "finish after false");
}

void TickerTest::testTickerResetAfterFinish()
{
    Ticker t;
    t.setCallback([]() -> bool { return false; });
    t.update(0.016f);
    TICK_ASSERT(t.isFinish(), "finished");
    t.resetProperty();
    TICK_ASSERT(!t.isFinish(), "resetProperty clears finish");
}

void TickerTest::testTickerCallbackAccumulated()
{
    Ticker t;
    float total = 0.0f;
    t.setCallback([&total]() -> bool
    {
        total += 0.016f;
        return true;
    });
    FOR(100)
    {
        t.update(0.016f);
    }
    TICK_ASSERT(total > 1.5f, "accumulated total > 1.5f");
}

void TickerTest::testTickerCallbackWithCapture()
{
    Ticker t;
    int sum = 0;
    int step = 3;
    t.setCallback([&sum, step]() -> bool
    {
        sum += step;
        return sum < 15;
    });
    FOR(20)
    {
        t.update(0.01f);
    }
    TICK_ASSERT(sum >= 15, "captured sum >= 15");
    TICK_ASSERT(t.isFinish(), "finish after sum >= 15");
}

void TickerTest::testTickerResetPropertyClearsCallback()
{
    Ticker t;
    int n = 0;
    t.setCallback([&n]() -> bool { ++n; return true; });
    t.update(0.01f);
    TICK_ASSERT(n == 1, "callback called");
    t.resetProperty();
    t.update(0.01f);
    TICK_ASSERT(n == 1, "after resetProperty callback cleared no more call");
}

void TickerTest::testTickerMultipleCallbackSwap()
{
    Ticker t;
    int n1 = 0;
    int n2 = 0;
    t.setCallback([&n1]() -> bool { ++n1; return true; });
    t.update(0.016f);
    t.update(0.016f);
    TICK_ASSERT(n1 == 2, "first callback called 2 times");
    t.setCallback([&n2]() -> bool { ++n2; return false; });
    t.update(0.016f);
    TICK_ASSERT(n2 == 1, "second callback called once");
    TICK_ASSERT(n1 == 2, "first callback not called after swap");
    TICK_ASSERT(t.isFinish(), "finish after second false");
}

void TickerTest::testTickerCallbackReturnsTrueMany()
{
    Ticker t;
    int n = 0;
    t.setCallback([&n]() -> bool { ++n; return true; });
    FOR(200)
    {
        t.update(0.016f);
    }
    TICK_ASSERT(n == 200, "callback called 200 times");
    TICK_ASSERT(!t.isFinish(), "all true => never finish");
}
#endif
