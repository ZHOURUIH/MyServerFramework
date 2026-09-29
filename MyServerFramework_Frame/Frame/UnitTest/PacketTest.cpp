#ifdef WINDOWS
#include "FrameHeader.h"
#include "PacketBit.h"

// ─────────────────────────────────────────────────────────────────────────────
// Internal test packet for PacketTest
// Simple packet with 4 optional integer fields + 1 bool
// ─────────────────────────────────────────────────────────────────────────────
class TestSimplePacket : public PacketBit
{
	BASE(TestSimplePacket, PacketBit);
public:
	enum class Field : byte
	{
		A = 0,
		B = 1,
		C = 2,
		D = 3,
	};

	int  mA = 0;
	int  mB = 0;
	int  mC = 0;
	int  mD = 0;
	bool mFlag = false;

	bool readFromBuffer(SerializerBitRead* reader, const bool needReadSign) override
	{
		bool ok = true;
		if (isFieldValid(Field::A)) { ok = ok && reader->readSigned(needReadSign, mA); }
		if (isFieldValid(Field::B)) { ok = ok && reader->readSigned(needReadSign, mB); }
		if (isFieldValid(Field::C)) { ok = ok && reader->readSigned(needReadSign, mC); }
		if (isFieldValid(Field::D)) { ok = ok && reader->readSigned(needReadSign, mD); }
		ok = ok && reader->readBool(mFlag);
		return ok;
	}
	bool writeToBuffer(SerializerBitWrite* writer, const bool needWriteSign) const override
	{
		bool ok = true;
		if (isFieldValid(Field::A)) { ok = ok && writer->writeSigned(needWriteSign, mA); }
		if (isFieldValid(Field::B)) { ok = ok && writer->writeSigned(needWriteSign, mB); }
		if (isFieldValid(Field::C)) { ok = ok && writer->writeSigned(needWriteSign, mC); }
		if (isFieldValid(Field::D)) { ok = ok && writer->writeSigned(needWriteSign, mD); }
		ok = ok && writer->writeBool(mFlag);
		return ok;
	}
	bool generateHasSignInternal() const override
	{
		if (isFieldValid(Field::A) && mA < 0) { return true; }
		if (isFieldValid(Field::B) && mB < 0) { return true; }
		if (isFieldValid(Field::C) && mC < 0) { return true; }
		if (isFieldValid(Field::D) && mD < 0) { return true; }
		return false;
	}
	void resetProperty() override
	{
		base::resetProperty();
		mA = mB = mC = mD = 0;
		mFlag = false;
	}
};

// ─────────────────────────────────────────────────────────────────────────────
// Assertion macros
// ─────────────────────────────────────────────────────────────────────────────
#define PKT_ASSERT(expr, msg) \
	do { if (!(expr)) { ERROR(string("PacketTest FAIL: ") + (msg)); } } while(0)
#define PKT_ASSERT_EQ(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("PacketTest FAIL: ") + (msg) + \
		" | expected=" + IToS((int)(b)) + " actual=" + IToS((int)(a))); } } while(0)
#define PKT_ASSERT_EQ_LL(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("PacketTest FAIL: ") + (msg) + \
		" | expected=" + LLToS((llong)(b)) + " actual=" + LLToS((llong)(a))); } } while(0)

// ─────────────────────────────────────────────────────────────────────────────
// Helper: perform a write-then-read round-trip on TestSimplePacket
// ─────────────────────────────────────────────────────────────────────────────
bool PacketTest::packetRoundTrip(const TestSimplePacket& src, TestSimplePacket& dst)
{
	SerializerBitWrite writer;
	const bool ns = const_cast<TestSimplePacket&>(src).hasSign();
	if (!const_cast<TestSimplePacket&>(src).writeToBuffer(&writer, ns)) { return false; }

	dst.setFieldFlag(src.getFieldFlag());
	SerializerBitRead reader(writer.getBuffer(), writer.getByteCount());
	return dst.readFromBuffer(&reader, ns);
}

//------------------------------------------------------------------------------
// Packet::fieldFlag basic operations (via base class interface)
//------------------------------------------------------------------------------
void PacketTest::testPacketFieldFlag()
{
	TestSimplePacket p;

	// Default: all fields valid (FULL_FIELD_FLAG = 0xFFFFFFFFFFFFFFFF)
	PKT_ASSERT_EQ_LL((llong)p.getFieldFlag(), (llong)FrameDefine::FULL_FIELD_FLAG,
		"default fieldFlag=FULL");

	// isFieldValid returns true for all field indices by default
	PKT_ASSERT(p.isFieldValid(TestSimplePacket::Field::A), "default isFieldValid A");
	PKT_ASSERT(p.isFieldValid(TestSimplePacket::Field::B), "default isFieldValid B");
	PKT_ASSERT(p.isFieldValid(TestSimplePacket::Field::C), "default isFieldValid C");
	PKT_ASSERT(p.isFieldValid(TestSimplePacket::Field::D), "default isFieldValid D");

	// setFieldInvalid: mark field B as invalid
	p.setFieldInvalid(TestSimplePacket::Field::B);
	PKT_ASSERT( p.isFieldValid(TestSimplePacket::Field::A), "A still valid after mark B invalid");
	PKT_ASSERT(!p.isFieldValid(TestSimplePacket::Field::B), "B now invalid");
	PKT_ASSERT( p.isFieldValid(TestSimplePacket::Field::C), "C still valid");
	PKT_ASSERT( p.isFieldValid(TestSimplePacket::Field::D), "D still valid");

	// setFieldValid: bring B back
	p.setFieldValid(TestSimplePacket::Field::B);
	PKT_ASSERT(p.isFieldValid(TestSimplePacket::Field::B), "B valid again");

	// clearFieldFlag: all fields invalid
	p.clearFieldFlag();
	PKT_ASSERT_EQ_LL((llong)p.getFieldFlag(), 0LL, "clearFieldFlag -> 0");
	PKT_ASSERT(!p.isFieldValid(TestSimplePacket::Field::A), "A invalid after clear");
	PKT_ASSERT(!p.isFieldValid(TestSimplePacket::Field::B), "B invalid after clear");
	PKT_ASSERT(!p.isFieldValid(TestSimplePacket::Field::C), "C invalid after clear");
	PKT_ASSERT(!p.isFieldValid(TestSimplePacket::Field::D), "D invalid after clear");

	// setFieldFlag: manually set only A and C valid (bits 0 and 2)
	p.setFieldFlag((1ULL << 0) | (1ULL << 2));
	PKT_ASSERT( p.isFieldValid(TestSimplePacket::Field::A), "A valid via setFieldFlag");
	PKT_ASSERT(!p.isFieldValid(TestSimplePacket::Field::B), "B invalid via setFieldFlag");
	PKT_ASSERT( p.isFieldValid(TestSimplePacket::Field::C), "C valid via setFieldFlag");
	PKT_ASSERT(!p.isFieldValid(TestSimplePacket::Field::D), "D invalid via setFieldFlag");

	// resetProperty: restores FULL_FIELD_FLAG and resets data
	p.mA = 100; p.mB = 200; p.mC = 300; p.mD = 400; p.mFlag = true;
	p.resetProperty();
	PKT_ASSERT_EQ_LL((llong)p.getFieldFlag(), (llong)FrameDefine::FULL_FIELD_FLAG,
		"resetProperty restores FULL_FIELD_FLAG");
	PKT_ASSERT_EQ(p.mA, 0, "resetProperty mA=0");
	PKT_ASSERT_EQ(p.mB, 0, "resetProperty mB=0");
	PKT_ASSERT_EQ(p.mC, 0, "resetProperty mC=0");
	PKT_ASSERT_EQ(p.mD, 0, "resetProperty mD=0");
	PKT_ASSERT(!p.mFlag, "resetProperty mFlag=false");

	// type, sequenceNumber
	PKT_ASSERT_EQ(p.getType(), 0, "default type=0");
	PKT_ASSERT_EQ((int)p.getSequenceNumber(), 0, "default sequenceNumber=0");
	p.setType(42);
	p.setSequenceNumber(99);
	PKT_ASSERT_EQ(p.getType(), 42, "setType=42");
	PKT_ASSERT_EQ((int)p.getSequenceNumber(), 99, "setSequenceNumber=99");

	// showInfo default true
	PKT_ASSERT(p.showInfo(), "showInfo default true");
}

//------------------------------------------------------------------------------
// Full round-trip: all fields valid, positive values
//------------------------------------------------------------------------------
void PacketTest::testPacketRoundTripAllFields()
{
	TestSimplePacket src;
	src.mA = 111;
	src.mB = 222;
	src.mC = 333;
	src.mD = 444;
	src.mFlag = true;

	TestSimplePacket dst;
	PKT_ASSERT(packetRoundTrip(src, dst), "roundTrip all fields writeToBuffer ok");

	PKT_ASSERT_EQ(dst.mA, 111, "roundTrip all A=111");
	PKT_ASSERT_EQ(dst.mB, 222, "roundTrip all B=222");
	PKT_ASSERT_EQ(dst.mC, 333, "roundTrip all C=333");
	PKT_ASSERT_EQ(dst.mD, 444, "roundTrip all D=444");
	PKT_ASSERT(dst.mFlag, "roundTrip all flag=true");
}

//------------------------------------------------------------------------------
// Round-trip: only fields A and C valid
//------------------------------------------------------------------------------
void PacketTest::testPacketRoundTripPartialFields()
{
	TestSimplePacket src;
	src.clearFieldFlag();
	src.setFieldValid(TestSimplePacket::Field::A);
	src.setFieldValid(TestSimplePacket::Field::C);
	src.mA = 55;
	src.mB = 999; // should not be written/read
	src.mC = 77;
	src.mD = 888; // should not be written/read
	src.mFlag = true;

	TestSimplePacket dst;
	dst.mB = 11; // pre-existing, should not be overwritten
	dst.mD = 22;
	PKT_ASSERT(packetRoundTrip(src, dst), "roundTrip partial writeToBuffer ok");

	PKT_ASSERT_EQ(dst.mA, 55, "roundTrip partial A=55");
	PKT_ASSERT_EQ(dst.mB, 11, "roundTrip partial B unchanged=11");
	PKT_ASSERT_EQ(dst.mC, 77, "roundTrip partial C=77");
	PKT_ASSERT_EQ(dst.mD, 22, "roundTrip partial D unchanged=22");
	PKT_ASSERT(dst.mFlag, "roundTrip partial flag=true");
}

//------------------------------------------------------------------------------
// Round-trip with negative values (hasSign=true path)
//------------------------------------------------------------------------------
void PacketTest::testPacketRoundTripNegative()
{
	TestSimplePacket src;
	src.mA = -100;
	src.mB = -200;
	src.mC = 300;
	src.mD = -1;
	src.mFlag = false;

	TestSimplePacket dst;
	PKT_ASSERT(packetRoundTrip(src, dst), "roundTrip neg writeToBuffer ok");

	PKT_ASSERT_EQ(dst.mA, -100, "roundTrip neg A=-100");
	PKT_ASSERT_EQ(dst.mB, -200, "roundTrip neg B=-200");
	PKT_ASSERT_EQ(dst.mC,  300, "roundTrip neg C=300");
	PKT_ASSERT_EQ(dst.mD,   -1, "roundTrip neg D=-1");
	PKT_ASSERT(!dst.mFlag, "roundTrip neg flag=false");
}

//------------------------------------------------------------------------------
// Round-trip: zero values
//------------------------------------------------------------------------------
void PacketTest::testPacketRoundTripZero()
{
	TestSimplePacket src;
	src.mA = 0;
	src.mB = 0;
	src.mC = 0;
	src.mD = 0;
	src.mFlag = false;

	TestSimplePacket dst;
	dst.mA = 999; // pre-fill
	PKT_ASSERT(packetRoundTrip(src, dst), "roundTrip zero ok");

	PKT_ASSERT_EQ(dst.mA, 0, "roundTrip zero A=0");
	PKT_ASSERT_EQ(dst.mB, 0, "roundTrip zero B=0");
	PKT_ASSERT_EQ(dst.mC, 0, "roundTrip zero C=0");
	PKT_ASSERT_EQ(dst.mD, 0, "roundTrip zero D=0");
	PKT_ASSERT(!dst.mFlag, "roundTrip zero flag=false");
}

//------------------------------------------------------------------------------
// Round-trip: extreme values (max int, min+1)
//------------------------------------------------------------------------------
void PacketTest::testPacketRoundTripExtreme()
{
	TestSimplePacket src;
	src.mA = 2147483647;    // INT_MAX
	src.mB = -2147483647;   // INT_MIN + 1 (avoid UB from frame's -value)
	src.mC = 0;
	src.mD = 1;
	src.mFlag = true;

	TestSimplePacket dst;
	PKT_ASSERT(packetRoundTrip(src, dst), "roundTrip extreme ok");

	PKT_ASSERT_EQ(dst.mA, 2147483647,  "roundTrip extreme A=INT_MAX");
	PKT_ASSERT_EQ(dst.mB, -2147483647, "roundTrip extreme B=INT_MIN+1");
	PKT_ASSERT_EQ(dst.mC, 0,  "roundTrip extreme C=0");
	PKT_ASSERT_EQ(dst.mD, 1,  "roundTrip extreme D=1");
	PKT_ASSERT(dst.mFlag, "roundTrip extreme flag=true");
}

//------------------------------------------------------------------------------
// hasSign caching: generates once, cached on repeated calls
//------------------------------------------------------------------------------
void PacketTest::testPacketHasSign()
{
	TestSimplePacket p;
	p.mA = 10;
	p.mB = 20;
	p.mC = 30;
	p.mD = 40;

	// All positive -> hasSign=false
	PKT_ASSERT(!p.hasSign(), "hasSign=false (all positive)");
	// Cached result: same on 2nd call
	PKT_ASSERT(!p.hasSign(), "hasSign cached=false");

	// resetProperty clears cache
	p.resetProperty();
	p.mA = -5;
	// Now has negative -> hasSign=true
	PKT_ASSERT(p.hasSign(), "hasSign=true after reset with negative");
	PKT_ASSERT(p.hasSign(), "hasSign cached=true");

	// After another reset with all positive
	p.resetProperty();
	p.mA = 1;
	PKT_ASSERT(!p.hasSign(), "hasSign=false after 2nd reset");
}

//------------------------------------------------------------------------------
// Multiple setFieldInvalid / setFieldValid cycles
//------------------------------------------------------------------------------
void PacketTest::testPacketFieldCycles()
{
	TestSimplePacket p;

	// Cycle A on/off multiple times
	FOR(200)
	{
		p.setFieldInvalid(TestSimplePacket::Field::A);
		PKT_ASSERT(!p.isFieldValid(TestSimplePacket::Field::A), "cycle A invalid");
		p.setFieldValid(TestSimplePacket::Field::A);
		PKT_ASSERT( p.isFieldValid(TestSimplePacket::Field::A), "cycle A valid");
	}

	// Individually mark all invalid
	p.setFieldInvalid(TestSimplePacket::Field::A);
	p.setFieldInvalid(TestSimplePacket::Field::B);
	p.setFieldInvalid(TestSimplePacket::Field::C);
	p.setFieldInvalid(TestSimplePacket::Field::D);
	PKT_ASSERT(!p.isFieldValid(TestSimplePacket::Field::A), "all invalid A");
	PKT_ASSERT(!p.isFieldValid(TestSimplePacket::Field::B), "all invalid B");
	PKT_ASSERT(!p.isFieldValid(TestSimplePacket::Field::C), "all invalid C");
	PKT_ASSERT(!p.isFieldValid(TestSimplePacket::Field::D), "all invalid D");

	// getFieldFlag should be 0 after all invalid
	// (FULL_FIELD_FLAG with bits 0-3 cleared)
	const ullong ff = p.getFieldFlag();
	PKT_ASSERT(!(ff & 0x1ULL), "fieldFlag bit0 cleared");
	PKT_ASSERT(!(ff & 0x2ULL), "fieldFlag bit1 cleared");
	PKT_ASSERT(!(ff & 0x4ULL), "fieldFlag bit2 cleared");
	PKT_ASSERT(!(ff & 0x8ULL), "fieldFlag bit3 cleared");

	// Re-enable all via setFieldValid
	p.setFieldValid(TestSimplePacket::Field::A);
	p.setFieldValid(TestSimplePacket::Field::B);
	p.setFieldValid(TestSimplePacket::Field::C);
	p.setFieldValid(TestSimplePacket::Field::D);
	PKT_ASSERT(p.isFieldValid(TestSimplePacket::Field::A), "re-enable A");
	PKT_ASSERT(p.isFieldValid(TestSimplePacket::Field::B), "re-enable B");
	PKT_ASSERT(p.isFieldValid(TestSimplePacket::Field::C), "re-enable C");
	PKT_ASSERT(p.isFieldValid(TestSimplePacket::Field::D), "re-enable D");
}

//------------------------------------------------------------------------------
// Consecutive round-trips reusing same packet
//------------------------------------------------------------------------------
void PacketTest::testPacketConsecutiveRoundTrip()
{
	TestSimplePacket src;

	FOR(200)
	{
		src.resetProperty();
		src.mA = i * 10;
		src.mB = i * 20;
		src.mC = -(i * 5);
		src.mD = i;
		src.mFlag = (i % 2 == 0);

		TestSimplePacket dst;
		PKT_ASSERT(packetRoundTrip(src, dst), "consecutive roundTrip ok");
		PKT_ASSERT_EQ(dst.mA, i * 10,    "consecutive A");
		PKT_ASSERT_EQ(dst.mB, i * 20,    "consecutive B");
		PKT_ASSERT_EQ(dst.mC, -(i * 5),  "consecutive C");
		PKT_ASSERT_EQ(dst.mD, i,         "consecutive D");
		PKT_ASSERT(dst.mFlag == (i % 2 == 0), "consecutive flag");
	}
}

//------------------------------------------------------------------------------
// setFieldFlag: bit manipulation correctness
//------------------------------------------------------------------------------
void PacketTest::testPacketSetFieldFlagManual()
{
	TestSimplePacket p;

	// Set only D (bit 3)
	p.setFieldFlag(1ULL << 3);
	PKT_ASSERT(!p.isFieldValid(TestSimplePacket::Field::A), "manual A invalid");
	PKT_ASSERT(!p.isFieldValid(TestSimplePacket::Field::B), "manual B invalid");
	PKT_ASSERT(!p.isFieldValid(TestSimplePacket::Field::C), "manual C invalid");
	PKT_ASSERT( p.isFieldValid(TestSimplePacket::Field::D), "manual D valid");

	// Set A and D (bits 0 and 3)
	p.setFieldFlag((1ULL << 0) | (1ULL << 3));
	PKT_ASSERT( p.isFieldValid(TestSimplePacket::Field::A), "bits A valid");
	PKT_ASSERT(!p.isFieldValid(TestSimplePacket::Field::B), "bits B invalid");
	PKT_ASSERT(!p.isFieldValid(TestSimplePacket::Field::C), "bits C invalid");
	PKT_ASSERT( p.isFieldValid(TestSimplePacket::Field::D), "bits D valid");

	// Set all bits (FULL_FIELD_FLAG)
	p.setFieldFlag(FrameDefine::FULL_FIELD_FLAG);
	PKT_ASSERT(p.isFieldValid(TestSimplePacket::Field::A), "FULL A valid");
	PKT_ASSERT(p.isFieldValid(TestSimplePacket::Field::B), "FULL B valid");
	PKT_ASSERT(p.isFieldValid(TestSimplePacket::Field::C), "FULL C valid");
	PKT_ASSERT(p.isFieldValid(TestSimplePacket::Field::D), "FULL D valid");
}

//------------------------------------------------------------------------------
// Extended: multi-round-trip consistency
//------------------------------------------------------------------------------
void PacketTest::testPacketMultiRoundTrip()
{
	// Do 20 round trips with varying values, verify each recovers correctly
	for (int i = 0; i < 20; ++i)
	{
		TestSimplePacket src;
		src.setFieldFlag(FrameDefine::FULL_FIELD_FLAG);
		src.mA = i * 7 + 1;
		src.mB = i * 13 + 2;
		src.mC = i * 17 + 3;
		src.mD = i * 19 + 4;
		src.mFlag = (i % 2 == 0);

		TestSimplePacket dst;
		PKT_ASSERT(packetRoundTrip(src, dst), "multiRoundTrip ok");
		PKT_ASSERT_EQ(dst.mA, src.mA, "multiRoundTrip mA");
		PKT_ASSERT_EQ(dst.mB, src.mB, "multiRoundTrip mB");
		PKT_ASSERT_EQ(dst.mC, src.mC, "multiRoundTrip mC");
		PKT_ASSERT_EQ(dst.mD, src.mD, "multiRoundTrip mD");
		PKT_ASSERT(dst.mFlag == src.mFlag, "multiRoundTrip mFlag");
	}
}

//------------------------------------------------------------------------------
// Extended: negative values in all combinations
//------------------------------------------------------------------------------
void PacketTest::testPacketNegativeCombinations()
{
	// A negative, others positive
	{
		TestSimplePacket src;
		src.setFieldFlag(FrameDefine::FULL_FIELD_FLAG);
		src.mA = -42; src.mB = 100; src.mC = 200; src.mD = 300; src.mFlag = false;
		TestSimplePacket dst;
		PKT_ASSERT(packetRoundTrip(src, dst), "negCombo A only");
		PKT_ASSERT_EQ(dst.mA, -42, "negCombo A=-42");
		PKT_ASSERT_EQ(dst.mB, 100, "negCombo B=100");
	}
	// B negative
	{
		TestSimplePacket src;
		src.setFieldFlag(FrameDefine::FULL_FIELD_FLAG);
		src.mA = 100; src.mB = -99; src.mC = 50; src.mD = 75; src.mFlag = true;
		TestSimplePacket dst;
		PKT_ASSERT(packetRoundTrip(src, dst), "negCombo B only");
		PKT_ASSERT_EQ(dst.mB, -99, "negCombo B=-99");
	}
	// C negative
	{
		TestSimplePacket src;
		src.setFieldFlag(FrameDefine::FULL_FIELD_FLAG);
		src.mA = 1; src.mB = 2; src.mC = -333; src.mD = 4; src.mFlag = false;
		TestSimplePacket dst;
		PKT_ASSERT(packetRoundTrip(src, dst), "negCombo C only");
		PKT_ASSERT_EQ(dst.mC, -333, "negCombo C=-333");
	}
	// D negative
	{
		TestSimplePacket src;
		src.setFieldFlag(FrameDefine::FULL_FIELD_FLAG);
		src.mA = 5; src.mB = 6; src.mC = 7; src.mD = -1000; src.mFlag = true;
		TestSimplePacket dst;
		PKT_ASSERT(packetRoundTrip(src, dst), "negCombo D only");
		PKT_ASSERT_EQ(dst.mD, -1000, "negCombo D=-1000");
	}
	// All negative
	{
		TestSimplePacket src;
		src.setFieldFlag(FrameDefine::FULL_FIELD_FLAG);
		src.mA = -1; src.mB = -2; src.mC = -3; src.mD = -4; src.mFlag = false;
		TestSimplePacket dst;
		PKT_ASSERT(packetRoundTrip(src, dst), "negCombo all neg");
		PKT_ASSERT_EQ(dst.mA, -1, "negCombo all A=-1");
		PKT_ASSERT_EQ(dst.mB, -2, "negCombo all B=-2");
		PKT_ASSERT_EQ(dst.mC, -3, "negCombo all C=-3");
		PKT_ASSERT_EQ(dst.mD, -4, "negCombo all D=-4");
	}
}

//------------------------------------------------------------------------------
// Extended: only flag field (no integer fields)
//------------------------------------------------------------------------------
void PacketTest::testPacketFlagOnly()
{
	// Mark all integer fields invalid, only mFlag serialized
	TestSimplePacket src;
	src.clearFieldFlag();
	src.mA = 999; src.mB = 999; src.mC = 999; src.mD = 999;
	src.mFlag = true;

	SerializerBitWrite writer;
	const bool ns = src.hasSign();
	PKT_ASSERT(src.writeToBuffer(&writer, ns), "flagOnly write ok");

	TestSimplePacket dst;
	dst.setFieldFlag(src.getFieldFlag());
	SerializerBitRead reader(writer.getBuffer(), writer.getByteCount());
	PKT_ASSERT(dst.readFromBuffer(&reader, ns), "flagOnly read ok");

	// integer fields were not written/read, dst keeps default 0
	PKT_ASSERT_EQ(dst.mA, 0, "flagOnly dst.mA=0 (default)");
	PKT_ASSERT(dst.mFlag, "flagOnly dst.mFlag=true");
}

//------------------------------------------------------------------------------
// Extended: partial field combinations - all 2-field combos
//------------------------------------------------------------------------------
void PacketTest::testPacketPartialCombos()
{
	using F = TestSimplePacket::Field;

	struct Combo { F f0; F f1; int v0; int v1; };
	Combo combos[] = {
		{F::A, F::B, 10, 20},
		{F::A, F::C, 30, 40},
		{F::A, F::D, 50, 60},
		{F::B, F::C, 70, 80},
		{F::B, F::D, 90, 100},
		{F::C, F::D, 110, 120},
	};

	for (auto& co : combos)
	{
		TestSimplePacket src;
		src.clearFieldFlag();
		src.setFieldValid(co.f0);
		src.setFieldValid(co.f1);
		src.mFlag = false;

		// Assign values to both fields
		if (co.f0 == F::A) { src.mA = co.v0; }
		else if (co.f0 == F::B) { src.mB = co.v0; }
		else if (co.f0 == F::C) { src.mC = co.v0; }
		else { src.mD = co.v0; }

		if (co.f1 == F::A) { src.mA = co.v1; }
		else if (co.f1 == F::B) { src.mB = co.v1; }
		else if (co.f1 == F::C) { src.mC = co.v1; }
		else { src.mD = co.v1; }

		TestSimplePacket dst;
		PKT_ASSERT(packetRoundTrip(src, dst), "partialCombo roundtrip");

		int got0 = 0, got1 = 0, exp0 = co.v0, exp1 = co.v1;
		if (co.f0 == F::A) { got0 = dst.mA; }
		else if (co.f0 == F::B) { got0 = dst.mB; }
		else if (co.f0 == F::C) { got0 = dst.mC; }
		else { got0 = dst.mD; }

		if (co.f1 == F::A) { got1 = dst.mA; }
		else if (co.f1 == F::B) { got1 = dst.mB; }
		else if (co.f1 == F::C) { got1 = dst.mC; }
		else { got1 = dst.mD; }

		PKT_ASSERT_EQ(got0, exp0, "partialCombo field0 value");
		PKT_ASSERT_EQ(got1, exp1, "partialCombo field1 value");
	}
}

//------------------------------------------------------------------------------
// Extended: resetProperty clears all state
//------------------------------------------------------------------------------
void PacketTest::testPacketResetPropertyFull()
{
	TestSimplePacket p;
	p.setFieldFlag(FrameDefine::FULL_FIELD_FLAG);
	p.mA = -100; p.mB = 200; p.mC = -300; p.mD = 400; p.mFlag = true;

	p.resetProperty();

	// After reset: all fields default 0
	PKT_ASSERT_EQ(p.mA, 0, "resetFull mA=0");
	PKT_ASSERT_EQ(p.mB, 0, "resetFull mB=0");
	PKT_ASSERT_EQ(p.mC, 0, "resetFull mC=0");
	PKT_ASSERT_EQ(p.mD, 0, "resetFull mD=0");
	PKT_ASSERT(!p.mFlag, "resetFull mFlag=false");
	// fieldFlag should be reset to FULL by default
	PKT_ASSERT_EQ_LL((llong)p.getFieldFlag(), (llong)FrameDefine::FULL_FIELD_FLAG, "resetFull fieldFlag=FULL");

	// Can write/read again after reset
	p.mA = 77; p.mB = 88;
	TestSimplePacket dst;
	PKT_ASSERT(packetRoundTrip(p, dst), "resetFull re-use write ok");
	PKT_ASSERT_EQ(dst.mA, 77, "resetFull re-use mA=77");
	PKT_ASSERT_EQ(dst.mB, 88, "resetFull re-use mB=88");
}

//------------------------------------------------------------------------------
// Extended: hasSign detection
//------------------------------------------------------------------------------
void PacketTest::testPacketHasSignDetection()
{
	TestSimplePacket p;
	p.setFieldFlag(FrameDefine::FULL_FIELD_FLAG);

	// All positive -> hasSign=false
	p.mA = 1; p.mB = 2; p.mC = 3; p.mD = 4;
	PKT_ASSERT(!p.hasSign(), "hasSign all positive=false");

	// mA negative -> hasSign=true (reset cache first)
	p.resetProperty();
	p.setFieldFlag(FrameDefine::FULL_FIELD_FLAG);
	p.mA = -1; p.mB = 2; p.mC = 3; p.mD = 4;
	PKT_ASSERT(p.hasSign(), "hasSign mA negative=true");

	// mB negative
	p.resetProperty();
	p.setFieldFlag(FrameDefine::FULL_FIELD_FLAG);
	p.mA = 1; p.mB = -1; p.mC = 3; p.mD = 4;
	PKT_ASSERT(p.hasSign(), "hasSign mB negative=true");

	// mC negative
	p.resetProperty();
	p.setFieldFlag(FrameDefine::FULL_FIELD_FLAG);
	p.mA = 1; p.mB = 2; p.mC = -5; p.mD = 4;
	PKT_ASSERT(p.hasSign(), "hasSign mC negative=true");

	// mD negative
	p.resetProperty();
	p.setFieldFlag(FrameDefine::FULL_FIELD_FLAG);
	p.mA = 1; p.mB = 2; p.mC = 3; p.mD = -99;
	PKT_ASSERT(p.hasSign(), "hasSign mD negative=true");

	// Restore all positive
	p.resetProperty();
	p.setFieldFlag(FrameDefine::FULL_FIELD_FLAG);
	p.mA = 1; p.mB = 2; p.mC = 3; p.mD = 4;
	PKT_ASSERT(!p.hasSign(), "hasSign restored positive=false");

	// Invalid field does not affect hasSign
	p.resetProperty();
	p.setFieldFlag(FrameDefine::FULL_FIELD_FLAG);
	p.mA = -500; p.mB = 2; p.mC = 3; p.mD = 4;
	p.setFieldInvalid(TestSimplePacket::Field::A);
	PKT_ASSERT(!p.hasSign(), "hasSign invalid field ignored=false");
}

//------------------------------------------------------------------------------
// Extended: large values (near INT_MAX/2)
//------------------------------------------------------------------------------
void PacketTest::testPacketLargeValues()
{
	const int BIG = 1073741823;  // 2^30 - 1
	TestSimplePacket src;
	src.setFieldFlag(FrameDefine::FULL_FIELD_FLAG);
	src.mA = BIG;
	src.mB = BIG - 1;
	src.mC = BIG - 2;
	src.mD = BIG - 3;
	src.mFlag = true;

	TestSimplePacket dst;
	PKT_ASSERT(packetRoundTrip(src, dst), "large values roundtrip");
	PKT_ASSERT_EQ(dst.mA, BIG, "large mA");
	PKT_ASSERT_EQ(dst.mB, BIG - 1, "large mB");
	PKT_ASSERT_EQ(dst.mC, BIG - 2, "large mC");
	PKT_ASSERT_EQ(dst.mD, BIG - 3, "large mD");
	PKT_ASSERT(dst.mFlag, "large mFlag");
}

//------------------------------------------------------------------------------
// Extended: clearFieldFlag then set individual fields
//------------------------------------------------------------------------------
void PacketTest::testPacketClearAndSetFields()
{
	TestSimplePacket p;
	p.clearFieldFlag();

	// All fields invalid
	PKT_ASSERT(!p.isFieldValid(TestSimplePacket::Field::A), "clear A invalid");
	PKT_ASSERT(!p.isFieldValid(TestSimplePacket::Field::B), "clear B invalid");
	PKT_ASSERT(!p.isFieldValid(TestSimplePacket::Field::C), "clear C invalid");
	PKT_ASSERT(!p.isFieldValid(TestSimplePacket::Field::D), "clear D invalid");

	// Set A only
	p.setFieldValid(TestSimplePacket::Field::A);
	PKT_ASSERT( p.isFieldValid(TestSimplePacket::Field::A), "set A valid");
	PKT_ASSERT(!p.isFieldValid(TestSimplePacket::Field::B), "set A B still invalid");

	// Set B
	p.setFieldValid(TestSimplePacket::Field::B);
	PKT_ASSERT(p.isFieldValid(TestSimplePacket::Field::B), "set B valid");

	// Invalidate A
	p.setFieldInvalid(TestSimplePacket::Field::A);
	PKT_ASSERT(!p.isFieldValid(TestSimplePacket::Field::A), "re-invalidate A");
	PKT_ASSERT( p.isFieldValid(TestSimplePacket::Field::B), "B still valid after A invalidated");

	// Set all via FULL_FIELD_FLAG
	p.setFieldFlag(FrameDefine::FULL_FIELD_FLAG);
	PKT_ASSERT(p.isFieldValid(TestSimplePacket::Field::A), "FULL A");
	PKT_ASSERT(p.isFieldValid(TestSimplePacket::Field::B), "FULL B");
	PKT_ASSERT(p.isFieldValid(TestSimplePacket::Field::C), "FULL C");
	PKT_ASSERT(p.isFieldValid(TestSimplePacket::Field::D), "FULL D");
}

//------------------------------------------------------------------------------
// Extended: sequentially encode decode multiple packets sharing a buffer
//------------------------------------------------------------------------------
void PacketTest::testPacketSequentialBuffers()
{
	// Write two packets into separate buffers, read back each
	TestSimplePacket src1, src2;
	src1.setFieldFlag(FrameDefine::FULL_FIELD_FLAG);
	src1.mA = 111; src1.mB = 222; src1.mC = 333; src1.mD = 444; src1.mFlag = true;

	src2.setFieldFlag(FrameDefine::FULL_FIELD_FLAG);
	src2.mA = 555; src2.mB = 666; src2.mC = 777; src2.mD = 888; src2.mFlag = false;

	SerializerBitWrite w1, w2;
	const bool ns1 = src1.hasSign();
	const bool ns2 = src2.hasSign();
	PKT_ASSERT(src1.writeToBuffer(&w1, ns1), "seq write p1");
	PKT_ASSERT(src2.writeToBuffer(&w2, ns2), "seq write p2");

	TestSimplePacket dst1, dst2;
	dst1.setFieldFlag(src1.getFieldFlag());
	dst2.setFieldFlag(src2.getFieldFlag());

	SerializerBitRead r1(w1.getBuffer(), w1.getByteCount());
	SerializerBitRead r2(w2.getBuffer(), w2.getByteCount());

	PKT_ASSERT(dst1.readFromBuffer(&r1, ns1), "seq read p1");
	PKT_ASSERT(dst2.readFromBuffer(&r2, ns2), "seq read p2");

	PKT_ASSERT_EQ(dst1.mA, 111, "seq p1 mA");
	PKT_ASSERT_EQ(dst1.mD, 444, "seq p1 mD");
	PKT_ASSERT(dst1.mFlag, "seq p1 mFlag");
	PKT_ASSERT_EQ(dst2.mA, 555, "seq p2 mA");
	PKT_ASSERT_EQ(dst2.mD, 888, "seq p2 mD");
	PKT_ASSERT(!dst2.mFlag, "seq p2 mFlag");
}

//------------------------------------------------------------------------------
// PacketTest entry point
//------------------------------------------------------------------------------
void PacketTest::test()
{
	testPacketFieldFlag();
	testPacketRoundTripAllFields();
	testPacketRoundTripPartialFields();
	testPacketRoundTripNegative();
	testPacketRoundTripZero();
	testPacketRoundTripExtreme();
	testPacketHasSign();
	testPacketFieldCycles();
	testPacketConsecutiveRoundTrip();
	testPacketSetFieldFlagManual();
	testPacketMultiRoundTrip();
	testPacketNegativeCombinations();
	testPacketFlagOnly();
	testPacketPartialCombos();
	testPacketResetPropertyFull();
	testPacketHasSignDetection();
	testPacketLargeValues();
	testPacketClearAndSetFields();
	testPacketSequentialBuffers();
}
#endif
