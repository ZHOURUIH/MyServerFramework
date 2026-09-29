#pragma once
#include "FrameMacro.h"

class MICRO_LEGEND_FRAME_API IntPoolThreadTest
{
#ifdef WINDOWS
public:
	static void test();
private:
	static void testNewIntNotNull();
	static void testDestroyIntNullAfter();
	static void testDestroyNullSafe();
	static void testReuseAfterDestroy();
	static void testInitDefault();
	static void testInitDefaultExhaust();
	static void testClearPool();
	static void testWriteRead();
	static void testWriteReadMax();
	static void testWriteReadMin();
	static void testBatchAllocFree();
	static void testManyRounds();
	static void testPointerUniqueness();
	static void testLargePool();
	static void testInitDefaultMultipleCalls();
	static void testClearAfterInit();
	static void testAlternateAllocFree();
	static void testValuePersistBetweenAllocs();
	static void testZeroValueWrite();
	static void testNegativeValueWrite();
#endif
};