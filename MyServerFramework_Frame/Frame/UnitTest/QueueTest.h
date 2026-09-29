#pragma once

#include "FrameBase.h"

class MICRO_LEGEND_FRAME_API QueueTest
{
#ifdef WINDOWS
public:
	static void test();
private:
	static void testQueueBasicInt();
	static void testQueuePopEmpty();
	static void testQueuePopOnly();
	static void testQueueClear();
	static void testQueueAddRange();
	static void testQueueFIFOOrder();
	static void testQueueString();
	static void testQueueInterleavedAddPop();
	static void testQueueStress();
	static void testQueueWithModifyScenario();
#endif
};