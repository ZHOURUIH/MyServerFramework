#pragma once

#include "FrameBase.h"

class MICRO_LEGEND_FRAME_API TimePointTest
{
#ifdef WINDOWS
public:
	static void test();
private:
	static void testTimePointInDay();
	static void testTimePointResetProperty();
	static void testTimePointCallback();
#endif
};
