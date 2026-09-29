#pragma once

#include "FrameMacro.h"

class MICRO_LEGEND_FRAME_API ClassTypePoolTest
{
#ifdef WINDOWS
public:
	static void test();
private:
	// UnitTest methods
	static void testInitDefault();
	static void testNewClassBasic();
	static void testNewClassReuseFromPool();
	static void testNewClassListBasic();
	static void testNewClassListReuseFromPool();
	static void testDestroyClassBasic();
	static void testDestroyClassNullSafe();
	static void testDestroyClassListVector();
	static void testDestroyClassListHashMap();
	static void testDestroyClassListArrayList();
	static void testGetTotalCount();
	static void testMarkUsable();
	static void testMarkDispose();
	static void testMultipleTypes();
	static void testReuseOrder();
	static void testQuit();
	static void testNewClassTemplate();
	static void testNewClassListTemplate();
	static void testLargeScale();
	static void testDestroyThenNew();
	static void testNewClassBasicThreadExt();
	static void testNewClassMultipleTypes();
	static void testDestroyClassNullAfter();
	static void testDestroyNullSafe();
	static void testDestroyClassReuse();
	static void testInitDefaultThreadExt();
	static void testInitDefaultExhaust();
	static void testNewClassList();
	static void testNewClassListSizeExact();
	static void testNewClassListEmpty();
	static void testNewClassListMultipleTypes();
	static void testNewClassListReuse();
	static void testNewClassListReuseAndGrow();
	static void testDestroyClassList();
	static void testManyRoundsAlloc();
	static void testLargeAllocFree();
	static void testDestroyThenReallocate();
	static void testInitDefaultMultiple();
	static void testPointerUniqueness();
	static void testResetPropertyOnReuse();
	static void testMixedTypeAlloc();
	static void testStressAllocFree();
	static void testIsEmpty();
	static void testNewClassAfterQuit();
#endif
};