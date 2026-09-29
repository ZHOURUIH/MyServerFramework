#pragma once
#include "FrameBase.h"
#include "FrameMacro.h"

class MICRO_LEGEND_FRAME_API QuickSortTest
{
#ifdef WINDOWS
public:
	static void test();
private:
	template<typename T>
	static bool isSortedAscend(const T* arr, int count);
	template<typename T>
	static bool isSortedDescend(const T* arr, int count);
	static void testQuickSortAscend_Basic();
	static void testQuickSortAscend_AlreadySorted();
	static void testQuickSortAscend_Reverse();
	static void testQuickSortAscend_AllSame();
	static void testQuickSortAscend_TrivialSize();
	static void testQuickSortAscend_Negative();
	static void testQuickSortAscend_Float();
	static void testQuickSortDescend_Basic();
	static void testQuickSortDescend_Negative();
	static void testQuickSortAscendPtr_Basic();
	static void testQuickSortAscendPtr_DuplicateValues();
	static void testQuickSortDescendPtr_Basic();
	static int compareInt(const int& a, const int& b);
	static void testQuickSortAscend_Compare();
	static void testQuickSortDescend_Compare();
	static void testQuickSort_CompareNullptr();
	static void testQuickSort_LargeDataset();
	static void testQuickSortNonRecursive_PivotReturn();
#endif
};