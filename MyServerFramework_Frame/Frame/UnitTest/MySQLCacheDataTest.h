#pragma once

#include "FrameBase.h"
#include "FrameMacro.h"

class MICRO_LEGEND_FRAME_API MySQLCacheDataTest
{
#ifdef WINDOWS
public:
	static void test();
private:
#ifdef _MYSQL
	static void testResetProperty();
	static void testAddValidFlag();
	static void testSetAndCheckColumnValid();
	static void testIsColumnNameValid();
	static void testIsColumnNamesValid_NonEmpty();
	static void testValidFlagConsistency();
#endif
#endif
};
