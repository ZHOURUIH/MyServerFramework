#pragma once
#include "FrameBase.h"

class MICRO_LEGEND_FRAME_API MapTest 
{
#ifdef WINDOWS
public: 
	static void test(); 
private:
	static void testMap_InsertOrUpdate_New();
	static void testMap_InsertOrUpdate_Existing();
	static void testMap_InsertOrUpdate_Move();
	static void testMap_InsertOrUpdate_Multiple();
	static void testMap_InsertOrUpdate_StringKey();
	static void testMap_InsertOrGet_New();
	static void testMap_InsertOrGet_Existing();
	static void testMap_InsertOrGet_Chain();
	static void testMap_InsertOrGet_LargeCount();
	static void testMap_TrySet_Existing();
	static void testMap_TrySet_NotExist();
	static void testMap_TrySet_Multiple();
	static void testMap_TrySet_StringKey();
	static void testMap_GetPtrConst_Found();
	static void testMap_GetPtrConst_Missing();
	static void testMap_GetPtrConst_ModifyViaGetPtr();
	static void testMap_FindIterator();
	static void testMap_FindIteratorConst();
	static void testMap_EraseByIterator();
	static void testMap_EraseByIterator_AllElements();
	static void testMap_EraseGetValue();
	static void testMap_EraseGetValue_Missing();
	static void testMap_EraseGetValue_StringValue();
	static void testMap_InsertKeyOnly();
	static void testMap_InsertKeyOnly_StringValue();
	static void testMap_InsertMoveValue();
	static void testMap_Merge_NoConflict();
	static void testMap_Merge_WithConflict();
	static void testMap_Merge_Empty();
	static void testMap_Merge_IntoEmpty();
	static void testMap_Merge_LargeNoConflict();
	static void testMap_Merge_AllConflict();
	static void testMap_CloneTo_Basic();
	static void testMap_CloneTo_OverwriteExisting();
	static void testMap_CloneTo_EmptySource();
	static void testMap_CloneTo_LargeMap();
	static void testMap_Dispose();
	static void testMap_ClearDisposeMemory();
	static void testMap_ReverseIterator();
	static void testMap_ReverseIterator_Sum();
	static void testMap_GetAtIndex_Ordered();
	static void testMap_GetAtIndex_AfterErase();
	static void testMap_MoveCtor();
	static void testMap_MoveAssign();
	static void testMap_InitializerList();
	static void testMap_InitializerList_StringKey();
	static void testMap_ConstIterator();
	static void testMap_ConstBeginEnd();
	static void testMap_LargeVolume();
	static void testMap_InsertUpdateErase();
	static void testMap_MergeAndClone();
	static void testMap_EraseAllThenReinsert();
	static void testMap_LLongKey();
	static void testMap_StringKeyLargeVolume();
	static void testMap_FindAfterInsertOrUpdate();
	static void testMap_GetPtrModifyThenVerify();
	static void testMap_ClearAndReinsertion();
	static void testMap_IterationOrderedByKey();
	static void testMap_TryGetDefault();
	static void testMap_TryGetNoDefault();
	static void testMap_OperatorBracket_AutoInsert();
	static void testMap_OperatorBracket_Update();
	static void testMap_ContainsMany();
	static void testMap_IsEmpty_AfterAllOps();
	static void testMap_SizeTracking();
	static void testSTLBasic();
	static void testSTLOrdering();
#endif
};