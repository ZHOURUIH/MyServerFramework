#pragma once
#include "FrameBase.h"
#include "FrameMacro.h"

class MICRO_LEGEND_FRAME_API MyStringTest
{
#ifdef WINDOWS
public:
	static void test();
private:
	static void testMSE_defaultConstruct();
	static void testMSE_assignConstChar();
	static void testMSE_assignMyString();
	static void testMSE_assignEmpty();
	static void testMSE_assignSelf();
	static void testMSE_appendChar();
	static void testMSE_appendMultiple();
	static void testMSE_appendEmpty();
	static void testMSE_appendLong();
	static void testMSE_clear();
	static void testMSE_clearThenAssign();
	static void testMSE_clearThenAppend();
	static void testMSE_clearMultiple();
	static void testMSE_exactFit();
	static void testMSE_singleChar();
	static void testMSE_twoChars();
	static void testMSE_almostFull();
	static void testMSE_intConversion();
	static void testMSE_intConversionLarge();
	static void testMSE_intConversionMax();
	static void testMSE_llongConversion();
	static void testMSE_llongMax();
	static void testMSE_floatConversion();
	static void testMSE_appendAfterAssign();
	static void testMSE_chainAssignment();
	static void testMSE_stringStream();
	static void testMSE_interleavedOps();
	static void testMSE_lengthTracking();
	static void testMSE_lengthAfterClear();
	static void testMSE_reusePattern();
	static void testMSE_clearAndReassign();
	static void testMSE_specialChars();
	static void testMSE_digitsOnly();
	static void testMSE_mixedCase();
	static void testMSE_strAccess();
	static void testMSE_strAfterClear();
	static void testMSE_mixedConversion();
	static void testMSE_appendVerifyEachStep();
	static void testMSE_multipleClear();
	static void testMSE_assignEmptyAfterNonEmpty();
	static void testMSE_appendLargeSequential();
	static void testMSE_reuseAfterLengthTracking();
	static void testMSE_assignStringStream();
	static void testMyString_Basic();
	static void testContainerExtended();
#endif
};
