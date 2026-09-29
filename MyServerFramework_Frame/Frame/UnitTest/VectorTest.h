#pragma once
#include "FrameBase.h"

class MICRO_LEGEND_FRAME_API VectorTest 
{
#ifdef WINDOWS
public: 
	static void test(); 
private:
	static void testVector_PushBackSingle();
	static void testVector_PushBack2();
	static void testVector_PushBack3();
	static void testVector_PushBack4();
	static void testVector_PushBack5();
	static void testVector_PushBack6();
	static void testVector_PushBackMove();
	static void testVector_AddUnique_VET();
	static void testVector_AddNotEqual();
	static void testVector_AddUniqueOrErase_Add();
	static void testVector_AddUniqueOrErase_Erase();
	static void testVector_AddRange();
	static void testVector_SetRange();
	static void testVector_AddRangeRawPtr();
	static void testVector_SetRangeRawPtr();
	static void testVector_AddRangeCopy();
	static void testVector_AddRangeMove();
	static void testVector_AddRangeFromArrayList();
	static void testVector_EraseAt();
	static void testVector_EraseAtOutOfRange();
	static void testVector_EraseAtCount();
	static void testVector_EraseElement();
	static void testVector_EraseAllElement_VET();
	static void testVector_EraseLast();
	static void testVector_EraseLastEmpty();
	static void testVector_PopBack();
	static void testVector_PopBackEmpty();
	static void testVector_Contains();
	static void testVector_IsAll();
	static void testVector_FindFirstIndex();
	static void testVector_FindElementCount();
	static void testVector_SwapIndex();
	static void testVector_Random();
	static void testVector_RandomEmpty();
	static void testVector_RandomErase();
	static void testVector_InsertAt();
	static void testVector_InsertAtBegin();
	static void testVector_InsertAtEnd();
	static void testVector_CloneTo();
	static void testVector_Sort();
	static void testVector_SortSingleElement();
	static void testVector_SortAlreadySorted();
	static void testVector_Resize();
	static void testVector_ResizeDown();
	static void testVector_Reserve();
	static void testVector_GetDefault();
	static void testVector_GetWithCustomDefault();
	static void testVector_EqualityOp();
	static void testVector_InequalityOp();
	static void testVector_EqualityDifferentSize();
	static void testVector_Clear();
	static void testVector_ClearEmpty();
	static void testVector_IsEmpty();
	static void testVector_LargeVolumePushBack();
	static void testVector_EraseAllThenAdd();
	static void testVector_MixedOperations();
	static void testVector_AddRangeThenSort();
	static void testVector_SwapIndexMultiple();
	static void testVector_FindFirstIndexMultipleOccurrences();
	static void testVector_EraseElementNotExist();
	static void testVector_PopBackUntilEmpty();
	static void testVector_CloneToOverwrite();
	static void testVector_StringVector();
	static void testVector_IntegrationSortAndSearch();
	static void testVector_AddUniqueMultiple();
	static void testVector_InsertInMiddle();
	static void testVector_EraseAtMultipleSegments();
	static void testVector_EmplaceBack();
	static void testVector_ClearAndReserve();
	static void testVector_SetRangeCopy_String();
	static void testVector_ResetElement();
	static void testVector_ResetElementAt();
	static void testVector_DataPointer();
	static void testVector_IteratorBasic();
	static void testVector_ReverseIterator();
	static void testVector_Basic();
	static void testVector_AddUnique();
	static void testVector_Initializer();
	static void testVector_EraseAllElement();
	static void testContainerExtended();
#endif
};