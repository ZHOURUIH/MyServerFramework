#pragma once
#include "FrameBase.h"

class MICRO_LEGEND_FRAME_API HashMapTest
{
#ifdef WINDOWS
public:
	static void test();
private:
	static void testHashMap_BasicInsert();
	static void testHashMap_InsertKeyOnly();
	static void testHashMap_InsertOrUpdate_New();
	static void testHashMap_InsertOrUpdate_Existing();
	static void testHashMap_InsertOrUpdate_Move();
	static void testHashMap_InsertOrGet_New();
	static void testHashMap_InsertOrGet_Existing();
	static void testHashMap_InsertOrGetWithValue();
	static void testHashMap_TrySet_Existing();
	static void testHashMap_TrySet_NotExist();
	static void testHashMap_GetPtr();
	static void testHashMap_GetPtrConst();
	static void testHashMap_TryGet_Found();
	static void testHashMap_TryGetNoDefault();
	static void testHashMap_Get_Found();
	static void testHashMap_GetAtIndex();
	static void testHashMap_EraseByKey();
	static void testHashMap_EraseByKeyAndGetValue();
	static void testHashMap_EraseByIterator();
	static void testHashMap_Clear();
	static void testHashMap_ClearEmpty();
	static void testHashMap_ClearThenInsert();
	static void testHashMap_Merge_NoConflict();
	static void testHashMap_Merge_WithConflict();
	static void testHashMap_Merge_Empty();
	static void testHashMap_CloneTo();
	static void testHashMap_OperatorBracket();
	static void testHashMap_Iterator();
	static void testHashMap_IteratorEmpty();
	static void testHashMap_StringKey();
	static void testHashMap_LLongKey();
	static void testHashMap_IntToString();
	static void testHashMap_LargeVolume();
	static void testHashMap_InsertUpdateAndErase();
	static void testHashMap_MergeAndClone();
	static void testHashMap_EraseAllThenReinsert();
	static void testHashMap_FindIterator();
	static void testHashMap_OperatorBracketAutoInsert();
	static void testHashMap_TrySetMultiple();
	static void testHashMap_InsertOrGetChained();
	static void testHashMap_StringValueVector();
	static void testHashMap_EraseByValue();
	static void testHashMap_InsertMoveSemantics();
	static void testHashMap_CloneToOverwrite();
	static void testHashMap_DisposeAndReuse();
	static void testHashMap_ConstMethodsOnConst();
	static void testBoundary();
	static void testHM_BulkInsert();
	static void testHM_BulkInsertRemove();
	static void testHM_RemoveThenRehash();
	static void testHM_AddOrSet();
	static void testHM_Set();
	static void testHM_GetWithDefault();
	static void testHM_ContainsAndFind();
	static void testHM_ClearAndReuse();
	static void testHM_StringKey();
	static void testHM_CopyMove();
	static void testHM_Iteration();
	static void testHM_LLongKey();
	static void testHM_DuplicateAddFails();
	static void testHashMap_Basic();
	static void testHashMap_InsertOrGet();
	static void testHashMap_Merge();
	static void testContainerExtended();
#endif
};