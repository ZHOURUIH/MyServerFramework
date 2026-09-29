#pragma once

#include "FrameMacro.h"

class MICRO_LEGEND_FRAME_API SafeHashMapTest
{
#ifdef WINDOWS
public:
	static void test();
private:
	// UnitTest methods
	static void testSafeHashMapBasicInsertScope();
	static void testSafeHashMapBasicEraseScope();
	static void testSafeHashMapScopeReadOnly();
	static void testSafeHashMapInsertDuringForeachScope();
	static void testSafeHashMapEraseDuringForeachScope();
	static void testSafeHashMapClearAfterForeach();
	static void testSafeHashMapContainsAndGet();
	static void testSafeHashMapEraseWithValueScope();
	static void testSafeHashMapSizeAndEmptyScope();
	static void testSafeHashMapBasicInsert();
	static void testSafeHashMapBasicErase();
	static void testSafeHashMapContains();
	static void testSafeHashMapTryGet();
	static void testSafeHashMapGetPtr();
	static void testSafeHashMapClear();
	static void testSafeHashMapInsertDuringForeach();
	static void testSafeHashMapEraseDuringForeach();
	static void testSafeHashMapInsertAndEraseDuringForeach();
	static void testSafeHashMapLargeModifyDuringForeach();
	static void testSafeHashMapForeachEmpty();
	static void testSafeHashMapSizeAndEmpty();
	static void testSafeHashMapMainListAccess();
	static void testSafeHashMapEraseWithValue();
#endif
};