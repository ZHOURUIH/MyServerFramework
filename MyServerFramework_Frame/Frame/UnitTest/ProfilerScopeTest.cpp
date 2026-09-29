#ifdef WINDOWS
#include "FrameHeader.h"

void ProfilerScopeTest::assertProfilerScope(bool condition, const char* message)
{
	if (!condition)
	{
		ERROR(string("ProfilerScopeTest failed: ") + message);
	}
}

void ProfilerScopeTest::testSingleScope()
{
	Profiler::resetFrame();
	{
		ProfilerScope scope(42, "ProfilerScopeTest.cpp", "testSingleScope");
		for (volatile int i = 0; i < 1000; ++i)
		{
		}
	}

	assertProfilerScope(Profiler::mData[42].mCount == 1, "single scope count");
	assertProfilerScope(Profiler::mData[42].mTotalTicks > 0, "single scope ticks");
	assertProfilerScope(Profiler::mData[42].mFileName != nullptr, "single scope file");
	assertProfilerScope(Profiler::mData[42].mFunction != nullptr, "single scope function");
}

void ProfilerScopeTest::testNestedScopes()
{
	Profiler::resetFrame();
	{
		ProfilerScope outer(7, "ProfilerScopeTest.cpp", "outer");
		{
			ProfilerScope inner(8, "ProfilerScopeTest.cpp", "inner");
			for (volatile int i = 0; i < 512; ++i)
			{
			}
		}
	}

	assertProfilerScope(Profiler::mData[7].mCount == 1, "outer count");
	assertProfilerScope(Profiler::mData[8].mCount == 1, "inner count");
	assertProfilerScope(Profiler::mData[7].mTotalTicks > 0, "outer ticks");
	assertProfilerScope(Profiler::mData[8].mTotalTicks > 0, "inner ticks");
}

void ProfilerScopeTest::testRepeatedScopes()
{
	Profiler::resetFrame();
	FOR(5)
	{
		ProfilerScope scope(13, "ProfilerScopeTest.cpp", "repeat");
		for (volatile int j = 0; j < 128; ++j)
		{
		}
	}

	assertProfilerScope(Profiler::mData[13].mCount == 5, "repeat count");
	assertProfilerScope(Profiler::mData[13].mTotalTicks > 0, "repeat ticks");
}

void ProfilerScopeTest::test()
{
	testSingleScope();
	testNestedScopes();
	testRepeatedScopes();
}
#endif
