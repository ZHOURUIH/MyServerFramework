#pragma once

#include "FrameBase.h"

class MICRO_LEGEND_FRAME_API RandomSystemTest
{
#ifdef WINDOWS
public:
	static void test();
private:
	static void assertRandomSystem(bool condition, const char* message);
	static void testConstructionAndSize();
	static void testRandomSequenceRead();
	static void testRandomSequenceWrap();
	static void testUpdateTouchesOneSlot();
#endif
};
