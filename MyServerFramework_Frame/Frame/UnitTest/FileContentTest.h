#pragma once

#include "FrameBase.h"

class MICRO_LEGEND_FRAME_API FileContentTest
{
#ifdef WINDOWS
public:
	static void test();
private:
	static void assertFileContent(bool condition, const char* message);
	static void testCreateBuffer();
	static void testBufferLifecycle();
	static void testBinaryFileLoad();
#endif
};