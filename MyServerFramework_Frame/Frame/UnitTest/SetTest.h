#pragma once
#include "FrameBase.h"

class MICRO_LEGEND_FRAME_API SetTest
{
#ifdef WINDOWS
 
public: 
	static void test(); 
private:
	static void testSet_BasicInsert();
	static void testSet_InsertMany();
	static void testSet_InsertStringType();
	static void testSet_EraseByValue();
	static void testSet_EraseByIterator();
	static void testSet_EraseAll();
	static void testSet_Clear();
	static void testSet_ClearEmpty();
	static void testSet_ClearThenInsert();
	static void testSet_PopFront();
	static void testSet_PopFrontMultiple();
	static void testSet_PopFrontRemovesElement();
	static void testSet_SetDataFromRawPtr();
	static void testSet_SetDataFromRawPtr_WithDups();
	static void testSet_SetDataFromVector();
	static void testSet_SetDataFromVector_Empty();
	static void testSet_SetDataFromVector_Merges();
	static void testSet_SetDataFromArrayList();
	static void testSet_SetDataFromArray();
	static void testSet_SetDataFromArray_PartialCount();
	static void testSet_InsertFromSet();
	static void testSet_InsertFromSet_Empty();
	static void testSet_InsertFromVector();
	static void testSet_InsertFromVector_Empty();
	static void testSet_CloneTo_Basic();
	static void testSet_CloneTo_Independent();
	static void testSet_CloneTo_Overwrite();
	static void testSet_MoveCtor();
	static void testSet_MoveAssign();
	static void testSet_FindFound();
	static void testSet_FindNotFound();
	static void testSet_Iterator_Sum();
	static void testSet_ConstIterator();
	static void testSet_ConstBeginEnd();
	static void testSet_LargeVolume();
	static void testSet_InsertEraseReinsert();
	static void testSet_SizeAfterOps();
	static void testSet_LLongType();
	static void testSet_SetDataCumulativeInsert();
	static void testSet_PopFrontUntilEmpty();
	static void testSet_CloneThenModifyOriginal();
	static void testSTLBasic();
#endif
};