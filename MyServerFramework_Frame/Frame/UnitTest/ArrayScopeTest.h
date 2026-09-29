#pragma once

#include "FrameMacro.h"

class MICRO_LEGEND_FRAME_API ArrayScopeTest
{
#ifdef WINDOWS
public:
	static void test();
private:
	// UnitTest methods
	static void testArrayScopeIntBasic();
	static void testArrayScopeZeroMemory();
	static void testArrayScopeNoZeroMemory();
	static void testArrayScopeSize1();
	static void testArrayScopeSize128();
	static void testArrayScopeLargeAlloc();
	static void testArrayScopeWriteRead();
	static void testArrayScopeFloat();
	static void testArrayScopeDouble();
	static void testArrayScopeConsecutive();
	static void testArrayScopeShort();
	static void testArrayScopeByte();
	static void testArrayScopeMultipleScopes();
	static void testArrayScopeNestedScopes();
	static void testCharArrayScopeBasic();
	static void testCharArrayScopeZeroMemory();
	static void testCharArrayScopeWriteString();
	static void testCharArrayScopeSize1();
	static void testCharArrayScopeSize512();
	static void testCharArrayScopeNullTerminate();
	static void testCharArrayScopeConsecutive();
	static void testCharArrayScopeLargeAlloc();
	static void testArrayScopeThreadIntBasic();
	static void testArrayScopeThreadZeroMemory();
	static void testArrayScopeThreadWriteRead();
	static void testArrayScopeThreadFloat();
	static void testArrayScopeThreadSize256();
	static void testArrayScopeThreadConsecutive();
	static void testArrayDefaultConstruct();
	static void testArrayFillArray();
	static void testArrayFillArrayPartial();
	static void testArraySetElement();
	static void testArraySize();
	static void testArrayData();
	static void testArrayContains();
	static void testArrayIterator();
	static void testArrayCopyConstruct();
	static void testArrayAssign();
	static void testArrayEqualManual();
	static void testArrayNotEqualManual();
	static void testArrayOutOfBounds();
	static void testArrayLargeType();
	static void testArrayIntArray();
	static void testArrayFloatArray();
	static void testArrayStringArray();
	static void testArrayZero();
	static void testArraySortAscend();
	static void testArraySortDescend();
	static void testArraySortWithData();
	static void testArrayBinarySearch();
	static void testArrayFindAndModify();
#endif
};