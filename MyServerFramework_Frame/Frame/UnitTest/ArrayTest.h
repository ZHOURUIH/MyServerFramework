#pragma once
#include "FrameBase.h"
class MICRO_LEGEND_FRAME_API ArrayTest 
{
#ifdef WINDOWS
public: 
    static void test();
private:
    static void testArrayIterators();
    static void testArrayData();
    static void testArraySize();
    static void testArrayZero();
    static void testArrayFillAll();
    static void testArrayFillFromStart();
    static void testArrayOperatorBracket();
    static void testArrayContains();
    static void testArrayEraseAt();
    static void testArrayEraseElement();
    static void testArrayEraseFirstElement();
    static void testArrayEraseLastElement();
    static void testArrayCopyFull();
    static void testArrayCopyWithOffset();
    static void testArrayCopyWithCount();
    static void testArrayCopyFromPointer();
    static void testArrayMixedOperations();
    static void testArrayLargeScale();
    static void testArrayCharBytes();
    static void testArrayEraseComparison();
    static void testArraySizeOne();
	static void testArray_Basic();
	static void testArray_FillAndZero();
	static void testContainerExtended();
#endif
};