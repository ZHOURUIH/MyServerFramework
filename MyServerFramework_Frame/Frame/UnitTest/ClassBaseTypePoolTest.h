#pragma once

#include "FrameMacro.h"

class MICRO_LEGEND_FRAME_API ClassBaseTypePoolTest
{
#ifdef WINDOWS
public:
	static void test();
private:
	// UnitTest methods
	static void testInitDefault();
	static void testNewClass();
	static void testNewClassReuseFromPool();
	static void testNewClassList();
	static void testNewClassListReuseFromPool();
	static void testDestroyClass();
	static void testDestroyClassNullSafe();
	static void testDestroyClassListVector();
	static void testDestroyClassListHashMap();
	static void testDestroyClassListArrayList();
	static void testMultipleTypes();
	static void testReuseByTypeHash();
	static void testQuit();
	static void testLargeScale();
	static void testDestroyThenNew();
	static void testMarkUsableAssignID();
	static void testInitDefaultThread();
	static void testNewClassThread();
	static void testNewClassReuse();
	static void testNewClassListThread();
	static void testDestroyClassThread();
	static void testDestroyClassNullSafeThread();
	static void testDestroyClassListVectorThread();
	static void testDestroyClassListHashMapThread();
	static void testDestroyClassListArrayListThread();
	static void testMultipleTypesThread();
	static void testQuitThread();
	static void testClearPool();
	static void testAssignID();
	static void testLargeScaleThread();
	static void testMixedNewDestroy();
	static void testNewClassListMultipleTypes();
	static void testDestroyAndReuseByType();
#endif
};