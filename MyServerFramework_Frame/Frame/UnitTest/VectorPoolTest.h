#pragma once

#include "FrameMacro.h"

class MICRO_LEGEND_FRAME_API VectorPoolTest
{
#ifdef WINDOWS
public:
	static void test();
private:
	// UnitTest methods
	static void testVectorPoolNewDestroy();
	static void testVectorPoolReuse();
	static void testVectorPoolMultipleTypes();
	static void testVectorPoolLargeScale();
	static void testVectorPoolDestroyPool();
	static void testVectorPoolModifyContent();
	static void testVectorPoolSequential();
	static void testVectorPoolThreadNewDestroy();
	static void testVectorPoolThreadReuse();
	static void testVectorPoolThreadMultipleTypes();
	static void testVectorPoolThreadLargeScale();
	static void testVectorPoolThreadModifyContent();
	static void testVectorPoolThreadDestroy();
	static void testVectorPoolMixedIntFloat();
	static void testVectorPoolThreadNullSafe();
	static void testVectorPoolIntBasic();
	static void testVectorPoolIntReuse();
	static void testVectorPoolIntMultiAlloc();
	static void testVectorPoolFloatBasic();
	static void testVectorPoolStringBasic();
	static void testVectorPoolModifyAndReturn();
	static void testVectorPoolReuseAfterModify();
	static void testVectorPoolDestroyPoolExt2();
	static void testVectorPoolThreadIntBasic();
	static void testVectorPoolThreadIntReuse();
	static void testVectorPoolThreadFloatBasic();
	static void testVectorPoolThreadStringBasic();
	static void testVectorPoolThreadModifyAndReturn();
	static void testVectorPoolThreadReuseAfterModify();
	static void testVectorPoolThreadNullAfterDestroy();
	static void testVectorPoolIntLargeScale();
	static void testVectorPoolThreadIntLargeScale();
	static void testVectorPoolMultiType();
	static void testVectorPoolThreadDestroyPool();
	static void testVectorPoolReuseCount();
#endif
};