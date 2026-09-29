#pragma once
#include "FrameBase.h"
#include "FrameMacro.h"

class MICRO_LEGEND_FRAME_API ArrayPoolSingleTest
{
#ifdef WINDOWS
public:
	static void test();
private:
	static void testApsIntGetRelease();
	static void testApsIntReuse();
	static void testApsIntMultipleSizes();
	static void testApsIntZeroMemory();
	static void testApsIntZeroMemoryReuse();
	static void testApsIntReleaseNull();
	static void testApsIntReleaseZeroCount();
	static void testApsIntMultiplePools();
	static void testApsIntGetLargerThanPrevious();
	static void testApsCharGetRelease();
	static void testApsCharMultipleSizes();
	static void testApsCharReleaseNull();
	static void testApsDoubleGetRelease();
	static void testApsDoubleZeroMemory();
	static void testApsMixedTypes();
	static void testApsReuseAfterMixedRelease();
	static void testApsBatchAllocFree();
	static void testApsBatchSameSize();
	static void testApsAllocAllSizes();
	static void testApsSizeOne();
	static void testApsSizeLarge();
	static void testApsDestroy();
	static void testApsCharPoolGetRelease();
	static void testApsCharPoolGetZeroMemory();
	static void testApsCharPoolReuse();
	static void testApsCharPoolMultipleSizes();
	static void testApsCharPoolReleaseNull();
	static void testApsCharPoolBatch();
	static void testApsCharPoolIsNewAlloc();
	static void testApsCharPoolDestroy();
	static void testApsCharPoolConsecutiveReuse();
#endif
};