#pragma once

#include "FrameBase.h"

class MICRO_LEGEND_FRAME_API SafeSetTest
{
#ifdef WINDOWS
public:
	static void test();
private:
	static void testSafeSetBasicInsert();
	static void testSafeSetBasicErase();
	static void testSafeSetContains();
	static void testSafeSetClear();
	static void testSafeSetInsertDuringForeach();
	static void testSafeSetEraseDuringForeach();
	static void testSafeSetSizeAndEmpty();
	static void testSafeSetInsertAndEraseDuringForeach();
	static void testSafeSetScopeBasicInsert();
	static void testSafeSetScopeBasicErase();
	static void testSafeSetScopeReadOnly();
	static void testSafeSetScopeInsertDuringForeach();
	static void testSafeSetScopeEraseDuringForeach();
	static void testSafeSetScopeClearAfterForeach();
	static void testSafeSetScopeContains();
	static void testSafeSetScopeSizeAndEmpty();
#endif
};