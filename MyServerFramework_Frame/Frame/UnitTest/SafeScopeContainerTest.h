#pragma once

#include "FrameMacro.h"

class MICRO_LEGEND_FRAME_API SafeScopeContainerTest
{
#ifdef WINDOWS
public:
	static void test();
private:
	// UnitTest methods
	static void testSafeVectorScopeEmpty();
	static void testSafeVectorScopeAddDuringForeach();
	static void testSafeVectorScopeRemoveDuringForeach();
	static void testSafeVectorScopeReadList();
	static void testSafeVectorScopeNestedScope();
	static void testSafeVectorScopeMultiAdd();
	static void testSafeVectorScopeIntType();
	static void testSafeVectorScopePtrType();
	static void testSafeVectorScopeModifyBetweenScopes();
	static void testSafeVectorScopeSize();
	static void testSafeSetScopeEmpty();
	static void testSafeSetScopeAddDuringForeach();
	static void testSafeSetScopeRemoveDuringForeach();
	static void testSafeSetScopeContains();
	static void testSafeSetScopeMultiple();
	static void testSafeHashMapScopeEmpty();
	static void testSafeHashMapScopeAddDuringForeach();
	static void testSafeHashMapScopeRemoveDuringForeach();
	static void testSafeHashMapScopeReadMap();
	static void testSafeHashMapScopeInt();
	static void testSafeHashMapScopeString();
	static void testSafeVectorScopeComplexType();
	static void testSafeVectorScopeLargeCount();
	static void testSafeSetScopeModifyAndVerify();
	static void testSafeHashMapScopeModifyBetweenScopes();
#endif
};
