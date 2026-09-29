#pragma once

#include "FrameMacro.h"

class MICRO_LEGEND_FRAME_API PodPoolThreadTest
{
#ifdef WINDOWS
public:
	static void test();
private:
	// UnitTest methods
	static void testNewPodInt();
	static void testNewPodLlong();
	static void testNewPodShort();
	static void testNewPodUshort();
	static void testNewPodByte();
	static void testNewPodUint();
	static void testNewPodNotNull();
	static void testDestroyPodNullAfter();
	static void testDestroyNullSafe();
	static void testReuseAfterDestroy();
	static void testReuseValue();
	static void testInitDefault();
	static void testInitDefaultThenNew();
	static void testInitDefaultExhaust();
	static void testClearPool();
	static void testClearPoolAfterNew();
	static void testMultipleNewSameType();
	static void testAlternateNewDestroy();
	static void testBatchNewDestroy();
	static void testManyRoundsInt();
	static void testManyRoundsLlong();
	static void testDestroyTwiceSafe();
	static void testLargePool();
	static void testNewAfterClear();
	static void testInitDefaultMultiple();
	static void testPointerUniqueness();
	static void testIntWriteRead();
	static void testLlongWriteRead();
	static void testShortWriteRead();
	static void testByteWriteRead();
	static void testNewPodIntExt();
	static void testNewPodLlongExt();
	static void testNewPodUshortExt();
	static void testNewPodByteExt();
	static void testDestroyPodSetsNull();
	static void testDestroyPodNullSafe();
	static void testDestroyPodDestroiedFlag();
	static void testReuseAfterDestroyExt();
	static void testInitDefaultPreAllocates();
	static void testExhaustPoolDynamic();
	static void testMultipleDestroyAndNew();
	static void testClearPoolExt();
	static void testClearPoolAfterDestroy();
	static void testTotalCountTracking();
	static void testIntPoolThread();
	static void testPoolNewValueIsValid();
	static void testPoolReuseOrder();
	static void testLargePoolExt();
	static void testMixedNewDestroyCycles();
	static void testQuitFreesMemory();
	static void testNewAfterClearPool();
	static void testClearPoolMultipleTimes();
	static void testDestroyManyThenNew();
	static void testInitDefaultThenExhaust();
	static void testNewReturnsUniquePointers();
	static void testPodPoolLlongBoundary();
	static void testPodPoolUshortBoundary();
	static void testPodPoolIntReuseInSequence();
	static void testPodPoolByteAllOnes();
	static void testPodPoolIntAlternateOddEven();
	static void runExtraPodPoolTests();
	static void testIntBoundaryMax();
	static void testIntBoundaryMin();
	static void testIntZero();
	static void testLlongBoundaryMax();
	static void testLlongBoundaryMin();
	static void testLlongZero();
	static void testShortBoundaryMax();
	static void testShortBoundaryMin();
	static void testUshortBoundaryMax();
	static void testUshortZero();
	static void testByteMax();
	static void testByteZero();
	static void testUintMax();
	static void testUintZero();
	static void testReusePreservesAddress();
	static void testReuseResetsValue();
	static void testMultipleReuseChain();
	static void testBatchReuseInt();
	static void testBatchReuseLlong();
	static void testInitDefaultInt();
	static void testInitDefaultLlong();
	static void testInitDefaultShort();
	static void testInitDefaultByte();
	static void testInitDefaultThenReuse();
	static void testInitDefaultMultipleValues();
	static void testClearPoolInt();
	static void testClearPoolLlong();
	static void testClearPoolThenNew();
	static void testClearPoolPreservesType();
	static void testStressInt();
	static void testStressLlong();
	static void testStressUint();
	static void testStressAlternateClear();
#endif
};