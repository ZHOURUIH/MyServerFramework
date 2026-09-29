#pragma once

#include "FrameBase.h"

class TestSimplePacket;

class MICRO_LEGEND_FRAME_API PacketTest
{
#ifdef WINDOWS
public:
	static void test();
private:
	static bool packetRoundTrip(const TestSimplePacket& src, TestSimplePacket& dst);
	static void testPacketFieldFlag();
	static void testPacketRoundTripAllFields();
	static void testPacketRoundTripPartialFields();
	static void testPacketRoundTripNegative();
	static void testPacketRoundTripZero();
	static void testPacketRoundTripExtreme();
	static void testPacketHasSign();
	static void testPacketFieldCycles();
	static void testPacketConsecutiveRoundTrip();
	static void testPacketSetFieldFlagManual();
	static void testPacketMultiRoundTrip();
	static void testPacketNegativeCombinations();
	static void testPacketFlagOnly();
	static void testPacketPartialCombos();
	static void testPacketResetPropertyFull();
	static void testPacketHasSignDetection();
	static void testPacketLargeValues();
	static void testPacketClearAndSetFields();
	static void testPacketSequentialBuffers();
#endif
};
