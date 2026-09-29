#pragma once

#include "FrameMacro.h"

class MICRO_LEGEND_FRAME_API ArrayListTest
{
#ifdef WINDOWS
public:
	static void test();
private:
	// UnitTest methods
	static void testALE_AddMulti_Two();
	static void testALE_AddMulti_Three();
	static void testALE_AddMulti_Four();
	static void testALE_AddMulti_Five();
	static void testALE_AddMulti_Six();
	static void testALE_AddMulti_Full();
	static void testALE_AddMulti_OverflowThree();
	static void testALE_AddNotEqual_AddWhenDiff();
	static void testALE_AddNotEqual_SkipWhenSame();
	static void testALE_AddNotEqual_Mixed();
	static void testALE_AddNotEqual_Full();
	static void testALE_AddUnique_Basic();
	static void testALE_AddUnique_Empty();
	static void testALE_AddUnique_AllUnique();
	static void testALE_AddUnique_Full();
	static void testALE_AddMove_String();
	static void testALE_AddMove_Int();
	static void testALE_EraseElement_Single();
	static void testALE_EraseElement_Multiple();
	static void testALE_EraseElement_NotExist();
	static void testALE_EraseElement_AllSame();
	static void testALE_EraseLastElement_Basic();
	static void testALE_EraseLastElement_NotExist();
	static void testALE_EraseLastElement_Single();
	static void testALE_EraseFirstElement_Basic();
	static void testALE_EraseFirstElement_NotExist();
	static void testALE_EraseFirstElement_OnlyOne();
	static void testALE_IsAll_True();
	static void testALE_IsAll_False();
	static void testALE_IsAll_Empty();
	static void testALE_IsAll_Single();
	static void testALE_AddRange_ArrayList();
	static void testALE_AddRange_ArrayList_Empty();
	static void testALE_AddRange_Ptr();
	static void testALE_AddRange_Ptr_Partial();
	static void testALE_AddRange_Ptr_AppendToExisting();
	static void testALE_SetRange_Basic();
	static void testALE_SetRange_ClearsFirst();
	static void testALE_Data_Basic();
	static void testALE_Data_Mutable();
	static void testALE_Data_Offset();
	static void testALE_Data_Offset_Zero();
	static void testALE_Get_Basic();
	static void testALE_Get_OutOfBounds();
	static void testALE_Get_Negative();
	static void testALE_Random_Single();
	static void testALE_Random_Multiple();
	static void testALE_Random_Empty();
	static void testALE_SetSize_Expand();
	static void testALE_SetSize_SameSize();
	static void testALE_SetSize_Smaller();
	static void testALE_Resize_Up();
	static void testALE_Resize_Down();
	static void testALE_Resize_Zero();
	static void testALE_Zero_Int();
	static void testALE_Zero_Float();
	static void testALE_FillArray_Int();
	static void testALE_FillArray_Float();
	static void testALE_FillArray_AfterAdd();
	static void testALE_FillArrayStart_Mid();
	static void testALE_FillArrayStart_Zero();
	static void testALE_MaxSize();
	static void testALE_CBeginCEnd();
	static void testALE_BeginEnd_Mutable();
	static void testALE_BatchProcess();
	static void testALE_SortLikePattern();
	static void testALE_AddRangeFromPtr_ThenSearch();
	static void testALE2_EraseAtBasic();
	static void testALE2_EraseAtFirst();
	static void testALE2_EraseAtLast();
	static void testALE2_ContainsTrue();
	static void testALE2_ContainsFalse();
	static void testALE2_ContainsEmpty();
	static void testALE2_MaxSize();
	static void testALE2_ClearAndReuse();
	static void testALE2_AddSixElements();
	static void testALE2_AddFiveElements();
	static void testALE2_AddFourElements();
	static void testALE2_AddThreeElements();
	static void testALE2_AddTwoElements();
	static void testALE2_AddNotEqualMixed();
	static void testALE2_AddUniqueAllSame();
	static void testALE2_AddUniqueAllDifferent();
	static void testALE2_GetOutOfBoundsDefault();
	static void testALE2_RandomSingleElem();
	static void testALE2_OperatorBracketWrite();
	static void testALE2_IsAllMixed();
	static void testALE2_AddRangeFromSelf();
	static void testALE2_SetRangeClearsFirst();
	static void testALE2_FillArrayPartial();
	static void testALE2_IteratorMutable();
	static void testSTLBasic();
	static void testSTLAdvanced();
#endif
};