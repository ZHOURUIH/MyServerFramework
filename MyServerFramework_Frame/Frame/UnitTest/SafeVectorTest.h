#pragma once

#include "FrameMacro.h"

class MICRO_LEGEND_FRAME_API SafeVectorTest
{
#ifdef WINDOWS
public:
	static void test();
private:
	// UnitTest methods
	static void testSafeVectorBasic();
	static void testSafeVectorClear();
	static void testSafeVectorNonForeachSync();
	static void testSafeVectorForeachPush();
	static void testSafeVectorForeachErase();
	static void testSafeVectorForeachAddThenRemove();
	static void testSafeVectorForeachClear();
	static void testSafeVectorData();
	static void testSafeVectorStress();
	static void testSafeHashMapBasic();
	static void testSafeHashMapClear();
	static void testSafeHashMapForeachInsert();
	static void testSafeHashMapForeachErase();
	static void testSafeHashMapEraseWithValue();
	static void testSafeMapBasic();
	static void testSafeMapForeachModify();
	static void testSafeMapClear();
	static void testSafeSetBasic();
	static void testSafeSetErase();
	static void testSafeSetForeachInsert();
	static void testSafeSetForeachErase();
	static void testSafeSetClear();
	static void testSafeSetForeachSync();
	static void testSafeVectorForeachState();
	static void testSafeHashMapForeachState();
	static void testSafeVectorMultiForeach();
	static void testSafeHashMapStress();
	static void testSafeMapComplex();
	static void testBasicPushBack();
	static void testBasicEraseAt();
	static void testBasicEraseElement();
	static void testBasicClear();
	static void testSizeAndIsEmpty();
	static void testContains();
	static void testGetMainList();
	static void testForeachFlag();
	static void testPushBackDuringForeach();
	static void testEraseAtDuringForeach();
	static void testEraseElementDuringForeach();
	static void testClearDuringForeach();
	static void testMoveMainListToOutsideForeach();
	static void testMoveMainListToDuringForeach();
	static void testMultiplePushBackDuringForeach();
	static void testMultipleEraseAtDuringForeach();
	static void testMixedAddRemoveDuringForeach();
	static void testLargeModifyTriggersCloneTo();
	static void testEraseAtOutOfBounds();
	static void testEraseElementNotFound();
	static void testSyncAfterForeach();
	static void testUpdateListConsistency();
	static void testDataPointer();
	static void testDestructorCleansUp();
	static void testResetElementDuringForeach();
	static void testHasResetCountCleared();
	static void testMultipleForeachCycles();
	static void testEraseAllDuringForeach();
	static void testPushBackThenClearDuringForeach();
	static void testSafeVectorString();
	static void testSafeVectorStringForeach();
	static void testSafeVectorPointer();
	static void testSafeVectorLargeData();
	static void testSafeVectorRepeatedCycles();
	static void testSafeVectorEraseByValueAll();
	static void testSafeVectorSingleElement();
	static void testSafeVectorEmptyForeach();
	static void testSafeVectorMixedTypes();
	static void runExtraTests();
	static void testSVEX_MoveMainListTo_Basic();
	static void testSVEX_MoveMainListTo_Empty();
	static void testSVEX_MoveMainListTo_ThenReuse();
	static void testSVEX_MoveMainListTo_LargeList();
	static void testSVEX_MoveMainListTo_OverwriteTarget();
	static void testSVEX_MoveMainListTo_MultipleRounds();
	static void testSVEX_MoveMainListTo_DuringForeach();
	static void testSVEX_MoveMainListTo_DuringForeach_ThenPush();
	static void testSVEX_IsForeaching_InitFalse();
	static void testSVEX_IsForeaching_DuringForeach();
	static void testSVEX_IsForeaching_AfterModify();
	static void testSVEX_Data_Basic();
	static void testSVEX_Data_AfterPushBack();
	static void testSVEX_Data_AfterErase();
	static void testSVEX_FrameTransfer();
	static void testSVEX_ForeachWithMoveAndPush();
	static void testSVEX_ContainsAfterMove();
	static void testSVEX_SizeAfterMove();
	static void testSVEX_MoveEmpty_ToNonEmpty();
	static void testSVEX_IsEmpty_AfterPushAndMove();
	static void testSVEX_GetMainListAfterMove();
	static void testSVEX_PushAfterMove_GetsCorrectData();
#endif
};