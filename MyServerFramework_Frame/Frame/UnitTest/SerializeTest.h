#pragma once

#include "FrameBase.h"

class TestPacketBit;

class MICRO_LEGEND_FRAME_API SerializeTest
{
#ifdef WINDOWS
public:
	static void test();
private:
	static bool doWriteRead(TestPacketBit& w, TestPacketBit& r);
	static void testPacketBit_AllFields();
	static void testPacketBit_PartialFields();
	static void testPacketBit_OnlyList0();
	static void testPacketBit_NegativeList1();
	static void testPacketBit_NegativeList0();
	static void testPacketBit_EmptyLists();
	static void testPacketBit_SingleElement();
	static void testPacketBit_ClearFieldFlag();
	static void testPacketBit_ResetProperty();
	static void testPacketBit_LargeList();
	static void testPacketBit_SizeMaxValue();
	static void testPacketBit_TwoPacketsSequential();
	static void testPacketBit_FlagToggle();
	static void testPacketBit_LLongExtremes();
	static void testPacketBitSerialize();
	static void testBasicTypes();
	static void testSkillFramePacket();
#endif
};