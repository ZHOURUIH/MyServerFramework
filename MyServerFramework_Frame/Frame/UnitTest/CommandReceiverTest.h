#pragma once
#include "FrameBase.h"
#include "FrameMacro.h"

class MICRO_LEGEND_FRAME_API CommandReceiverTest
{
#ifdef WINDOWS
public:
	static void test();
private:
	static void testCrNameDefault();
	static void testCrSetNameBasic();
	static void testCrSetNameWithUnderscore();
	static void testCrSetNameWithNumbers();
	static void testCrSetNameEmpty();
	static void testCrSetNameOverride();
	static void testCrSetNameLong();
	static void testCrSetNameVeryLong();
	static void testCrSetNameSingleChar();
	static void testCrSetNameSpecial();
	static void testCrSetNameMultipleCR();
	static void testCrPrintNameDefault();
	static void testCrPrintNameAfterReset();
	static void testCrResetPropertyName();
	static void testCrResetPropertyThenSet();
	static void testCrResetPropertyMultipleTimes();
	static void testCrResetPropertyEmptyName();
	static void testCrClassObjectResetChain();
#endif
};