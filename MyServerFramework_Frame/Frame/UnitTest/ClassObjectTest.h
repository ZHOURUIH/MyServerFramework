#pragma once

#include "FrameMacro.h"

class MICRO_LEGEND_FRAME_API ClassObjectTest
{
#ifdef WINDOWS
public:
	static void test();
private:
	// UnitTest methods
	static void testDefaultState();
	static void testMarkUsable();
	static void testMarkDispose();
	static void testMarkUsableTwice();
	static void testMarkDisposeWrongPool();
	static void testMarkDisposeAfterAlreadyDisposed();
	static void testGetAssignID();
	static void testGetPool();
	static void testSetPendingDestroy();
	static void testIsPendingDestroy();
	static void testIsDestroy();
	static void testResetProperty();
	static void testMarkUsableResetsDestroy();
	static void testMarkUsableResetsAssignID();
	static void testMarkDisposeCallsResetProperty();
	static void testMultipleLifecycles();
	static void testAssignIDUniqueness();
	static void testMarkDisposeWithNullPool();
	static void testBaseTypeInfo();
	static void testIsSubClassOfVerify();
	static void testClassObjectPoolInteraction();
	static void testClassObjectMultipleReuses();
	static void testClassObjectDestroyFromDifferentPool();
	static void testClassObjectPendingDestroyBeforeDispose();
	static void testClassObjectResetClearsState();
	static void testClassObjectIsDestroyAfterMarkDispose();
	static void testClassObjectAssignIDAfterMarkUsable();
	static void testClassObjectMultipleMarkUsable();
	static void testClassObjectSafeVectorLifecycle();
	static void testClassObjectHashMapLifecycle();
	static void testClassObjectClearPool();
	static void testCOE2ObjectTagPersistenceAcrossReuse();
	static void testCOE2PoolBulkAllocVerify();
	static void testCOE2PoolSafeHashMapDestroy();
	static void testCOE2ExtraStress();
	static void runCOE2ExtraTests();
	static void testCOE2PoolSafeVectorBatch();
	static void testCOE2PoolIdGapAfterReuse();
	static void testCOE2PoolArrayListDestroy();
	static void runCOE2BonusTests();
#endif
};