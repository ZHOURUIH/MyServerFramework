#pragma once
#include "FrameBase.h"
#include "FrameMacro.h"

class MICRO_LEGEND_FRAME_API CRC16Test
{
#ifdef WINDOWS
public:
	static void test();
private:
	static void testCRC16_Empty();
	static void testCRC16_SingleByte();
	static void testCRC16_KnownVectors();
	static void testCRC16_Incremental();
	static void testCRC16_ThreeSegments();
	static void testCRC16_BufferOffset();
	static void testCRC16_LargeBuffer();
	static void testCRC16_ByteByByte();
	static void testCRC16_AllZeros();
	static ushort crc16Reference(ushort crc, const char* buffer, const int len, const int bufferOffset = 0);
	static void testCRC16_SlicingBy4Regression();
	static void testCRC16_DifferentInitValues();
#endif
};