#pragma once

#include "FrameMacro.h"

class MICRO_LEGEND_FRAME_API ClassKeyPoolTest
{
#ifdef WINDOWS
public:
	static void test();
private:
	// UnitTest methods
	static void testNewClassSingleKey();
	static void testNewClassMultipleKeys();
	static void testDestroyClassNullAfter();
	static void testDestroyClassReuse();
	static void testDestroyNullSafe();
	static void testNewClassListSingleKey();
	static void testNewClassListMultipleKeys();
	static void testNewClassListPartialReuse();
	static void testNewClassListSizeExact();
	static void testNewClassListEmpty();
	static void testMultiRoundAllocFree();
	static void testNewClassSubType();
	static void testNewClassListSubType();
	static void testAllocManyKeys();
	static void testDestroyAllThenReallocate();
	static void testLargeAllocFreeStress();
	static void testKeyZero();
	static void testKeyNegative();
	static void testMixedKeyAccess();
	static void testDestroyFromDifferentKeys();
	static void testCountTracking();
	static void testIsEmpty();
	static void testNewClassListReuseExhaustFallback();
	static void testBatchAllocByKey();
	static void testNewAfterQuit();
	static void testNewClass();
	static void testNewClassTemplate();
	static void testNewClassReuse();
	static void testNewClassList();
	static void testDestroyClass();
	static void testDestroyClassNullSafe();
	static void testDestroyClassListVector();
	static void testDestroyClassListHashMap();
	static void testDestroyClassListArrayList();
	static void testQuit();
	static void testLargeScale();
	static void testAssignID();
	static void testReuseResetProperty();
	static void testMultipleKeys();
	static void testInterleavedNewDestroy();
#endif
};