#pragma once

#include "FrameBase.h"

class MICRO_LEGEND_FRAME_API ProfilerScopeTest
{
#ifdef WINDOWS
public:
	static void test();
private:
	static void assertProfilerScope(bool condition, const char* message);
	static void testSingleScope();
	static void testNestedScopes();
	static void testRepeatedScopes();
#endif
};
