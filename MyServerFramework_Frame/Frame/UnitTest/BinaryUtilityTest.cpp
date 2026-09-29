#ifdef WINDOWS
#include "FrameHeader.h"
#include <climits>

namespace BinaryUtilityExtTestSupport
{
#define BinaryUtilityExtTestSupport_BUE_ASSERT(expr) \
	if (!(expr)) { ERROR("BinaryUtilityExtTest FAILED: " #expr); }
}

void BinaryUtilityTest::testGetSetByte()
{
	using namespace BinaryUtilityExtTestSupport;
	// int 的字节操作
	int val = 0x04030201;  // [0]=0x01 [1]=0x02 [2]=0x03 [3]=0x04
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getByte(val, 0) == 0x01);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getByte(val, 1) == 0x02);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getByte(val, 2) == 0x03);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getByte(val, 3) == 0x04);

	// setByte
	int v = 0;
	BinaryUtility::setByte(v, (byte)0xAB, 0);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getByte(v, 0) == 0xAB);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getByte(v, 1) == 0x00);

	BinaryUtility::setByte(v, (byte)0xCD, 2);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getByte(v, 2) == 0xCD);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getByte(v, 0) == 0xAB);  // 其他位不变

	// short
	short sv = 0x1234;
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getByte(sv, 0) == 0x34);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getByte(sv, 1) == 0x12);

	// ushort
	ushort usv = 0xABCD;
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getByte(usv, 0) == 0xCD);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getByte(usv, 1) == 0xAB);

	// llong
	llong lv = 0x0807060504030201LL;
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getByte(lv, 0) == 0x01);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getByte(lv, 7) == 0x08);

	// setByte on llong
	llong lv2 = 0;
	BinaryUtility::setByte(lv2, (byte)0xFF, 3);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getByte(lv2, 3) == 0xFF);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getByte(lv2, 0) == 0x00);
}

void BinaryUtilityTest::testBitGetSet()
{
	using namespace BinaryUtilityExtTestSupport;
	int v = 0;
	BinaryUtilityExtTestSupport_BUE_ASSERT(!BinaryUtility::hasBit(v, 0));

	BinaryUtility::setBit(v, 0, 1);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::hasBit(v, 0));
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getBit(v, 0) == 1);

	BinaryUtility::setBit(v, 0, 0);
	BinaryUtilityExtTestSupport_BUE_ASSERT(!BinaryUtility::hasBit(v, 0));
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getBit(v, 0) == 0);

	// 多位
	int v2 = 0;
	BinaryUtility::setBit(v2, 3, 1);
	BinaryUtility::setBit(v2, 7, 1);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::hasBit(v2, 3));
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::hasBit(v2, 7));
	BinaryUtilityExtTestSupport_BUE_ASSERT(!BinaryUtility::hasBit(v2, 0));
	BinaryUtilityExtTestSupport_BUE_ASSERT(!BinaryUtility::hasBit(v2, 1));
	BinaryUtilityExtTestSupport_BUE_ASSERT(v2 == ((1 << 3) | (1 << 7)));

	// setBitOne / setBitZero
	int v3 = 0;
	BinaryUtility::setBitOne(v3, 5);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::hasBit(v3, 5));
	BinaryUtility::setBitZero(v3, 5);
	BinaryUtilityExtTestSupport_BUE_ASSERT(!BinaryUtility::hasBit(v3, 5));

	// 所有 int 位
	int v4 = 0;
	for (int i = 0; i < 32; ++i)
	{
		BinaryUtility::setBitOne(v4, i);
		BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::hasBit(v4, i));
		BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getBit(v4, i) == 1);
	}
	BinaryUtilityExtTestSupport_BUE_ASSERT(v4 == -1);  // 所有位都是1

	for (int i = 0; i < 32; ++i)
	{
		BinaryUtility::setBitZero(v4, i);
		BinaryUtilityExtTestSupport_BUE_ASSERT(!BinaryUtility::hasBit(v4, i));
	}
	BinaryUtilityExtTestSupport_BUE_ASSERT(v4 == 0);
}

void BinaryUtilityTest::testBitTypeVariants()
{
	using namespace BinaryUtilityExtTestSupport;
	// byte
	byte bv = 0;
	BinaryUtility::setBitOne(bv, 0);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::hasBit(bv, 0));
	BinaryUtility::setBitZero(bv, 0);
	BinaryUtilityExtTestSupport_BUE_ASSERT(!BinaryUtility::hasBit(bv, 0));
	BinaryUtility::setBitOne(bv, 7);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::hasBit(bv, 7));

	// short
	short sv = 0;
	BinaryUtility::setBitOne(sv, 15);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::hasBit(sv, 15));
	BinaryUtility::setBitZero(sv, 15);
	BinaryUtilityExtTestSupport_BUE_ASSERT(!BinaryUtility::hasBit(sv, 15));

	// ushort
	ushort usv = 0;
	BinaryUtility::setBitOne(usv, 8);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::hasBit(usv, 8));

	// llong
	llong lv = 0;
	BinaryUtility::setBitOne(lv, 63);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::hasBit(lv, 63));
	BinaryUtility::setBitZero(lv, 63);
	BinaryUtilityExtTestSupport_BUE_ASSERT(!BinaryUtility::hasBit(lv, 63));

	// ullong
	ullong ulv = 0;
	BinaryUtility::setBitOne(ulv, 63);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::hasBit(ulv, 63));
}

void BinaryUtilityTest::testHighestBit()
{
	using namespace BinaryUtilityExtTestSupport;
	// 正数最高位是0
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getHighestBit((int)0) == 0);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getHighestBit((int)1) == 0);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getHighestBit(INT_MAX) == 0);

	// 负数/最高位为1
	int neg = -1;
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getHighestBit(neg) == 1);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getHighestBit(INT_MIN) == 1);

	// setHighestBit 置1
	int v = 0;
	BinaryUtility::setHighestBit(v, true);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getHighestBit(v) == 1);
	BinaryUtility::setHighestBit(v, false);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getHighestBit(v) == 0);

	// byte 最高位
	byte bv = 0x80;
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getHighestBit(bv) == 1);
	bv = 0x7F;
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getHighestBit(bv) == 0);

	// llong 最高位
	llong lv = LLONG_MIN;
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getHighestBit(lv) == 1);
	lv = LLONG_MAX;
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getHighestBit(lv) == 0);
}

void BinaryUtilityTest::testLowestBit()
{
	using namespace BinaryUtilityExtTestSupport;
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getLowestBit(0) == 0);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getLowestBit(1) == 1);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getLowestBit(2) == 0);  // 10 最低位是0
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getLowestBit(3) == 1);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getLowestBit(-1) == 1);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getLowestBit(-2) == 0);  // -2 = ...11111110，最低位是0

	int v = 0;
	BinaryUtility::setLowestBit(v, true);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getLowestBit(v) == 1);
	BinaryUtility::setLowestBit(v, false);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getLowestBit(v) == 0);

	// byte
	byte bv = 0xFE;  // 11111110
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getLowestBit(bv) == 0);
	BinaryUtility::setLowestBit(bv, true);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getLowestBit(bv) == 1);
}

void BinaryUtilityTest::testGetBitsBitCount()
{
	using namespace BinaryUtilityExtTestSupport;
	// getBits(value, start, count)
	// 0b10110101 = 0xB5, 从bit0开始取3位 = 101 = 5
	byte v = 0b10110101;
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getBits(v, 0, 3) == 5);
	// 从bit4开始取3位: bit4=1,bit5=1,bit6=0 低到高 = 0b011 = 3
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getBits(v, 4, 3) == 0b011);
	// 从bit2开始取4位: bit2=1,bit3=0,bit4=1,bit5=1 低到高 = 0b1101 = 13
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getBits(v, 2, 4) == 0b1101);
	// 取1位
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getBits(v, 0, 1) == 1);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getBits(v, 1, 1) == 0);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getBits(v, 7, 1) == 1);

	// bitCountToByteCount
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::bitCountToByteCount(0) == 0);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::bitCountToByteCount(1) == 1);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::bitCountToByteCount(8) == 1);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::bitCountToByteCount(9) == 2);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::bitCountToByteCount(16) == 2);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::bitCountToByteCount(17) == 3);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::bitCountToByteCount(24) == 3);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::bitCountToByteCount(25) == 4);
}

void BinaryUtilityTest::testCopyBits()
{
	using namespace BinaryUtilityExtTestSupport;
	// copyBits(destValue, destStart, sourceValue, sourceStart, writeBitCount)
	byte src = 0b11010110;
	byte dst = 0;
	// 将 src 从 bit2 开始的 3 位（101）复制到 dst 从 bit0 开始
	BinaryUtility::copyBits(dst, 0, src, 2, 3);
	// 0b101 = 5
	BinaryUtilityExtTestSupport_BUE_ASSERT(dst == 5);

	byte dst2 = 0;
	// 将 src bit0 开始 2位（10）复制到 dst2 bit4 开始
	BinaryUtility::copyBits(dst2, 4, src, 0, 2);
	// 0b00100000 | (10 << 4) = 0b00100000 = 32
	// src bit0-1 = 10, 即 0b10 = 2，放到 dst2 bit4 = 0b00100000 = 32
	BinaryUtilityExtTestSupport_BUE_ASSERT(dst2 == 0b00100000);

	// 简化版 copyBits(byte&, destStart, sourceValue)
	byte dst3 = 0;
	BinaryUtility::copyBits(dst3, (byte)0, (byte)0b1010);
	BinaryUtilityExtTestSupport_BUE_ASSERT(dst3 == 0b1010);

	byte dst4 = 0;
	BinaryUtility::copyBits(dst4, (byte)4, (byte)0b0011);
	BinaryUtilityExtTestSupport_BUE_ASSERT(dst4 == 0b00110000);
}

void BinaryUtilityTest::testGenerateBitCount()
{
	using namespace BinaryUtilityExtTestSupport;
	// byte: generateBitCount(0) = 0 (no bits needed)
	// generateBitCount(1) = 1, generateBitCount(2) = 2, generateBitCount(3) = 2
	// generateBitCount(127) = 7, generateBitCount(128) = 8 (for byte: 0~255)
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::generateBitCount((byte)0) == 0);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::generateBitCount((byte)1) == 1);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::generateBitCount((byte)2) == 2);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::generateBitCount((byte)3) == 2);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::generateBitCount((byte)4) == 3);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::generateBitCount((byte)7) == 3);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::generateBitCount((byte)8) == 4);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::generateBitCount((byte)127) == 7);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::generateBitCount((byte)128) == 8);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::generateBitCount((byte)255) == 8);

	// ushort
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::generateBitCount((ushort)0) == 0);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::generateBitCount((ushort)1) == 1);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::generateBitCount((ushort)256) == 9);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::generateBitCount((ushort)65535) == 16);

	// uint
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::generateBitCount((uint)0) == 0);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::generateBitCount((uint)65536) == 17);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::generateBitCount((uint)0xFFFFFFFF) == 32);
}

void BinaryUtilityTest::testBufferBit()
{
	using namespace BinaryUtilityExtTestSupport;
	char buf[4] = { 0 };
	// bitIndex 0-31 对应 buf[0-3] 的各位
	BinaryUtilityExtTestSupport_BUE_ASSERT(!BinaryUtility::getBufferBit(buf, 0));

	BinaryUtility::setBufferBitOne(buf, 0);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getBufferBit(buf, 0));
	BinaryUtilityExtTestSupport_BUE_ASSERT(!BinaryUtility::getBufferBit(buf, 1));

	BinaryUtility::setBufferBitOne(buf, 7);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getBufferBit(buf, 7));
	BinaryUtilityExtTestSupport_BUE_ASSERT(!BinaryUtility::getBufferBit(buf, 8));

	BinaryUtility::setBufferBitOne(buf, 8);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getBufferBit(buf, 8));

	BinaryUtility::setBufferBitOne(buf, 31);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getBufferBit(buf, 31));

	// 跨字节
	char buf2[2] = { 0 };
	for (int i = 0; i < 16; ++i)
	{
		BinaryUtility::setBufferBitOne(buf2, i);
	}
	BinaryUtilityExtTestSupport_BUE_ASSERT(buf2[0] == (char)0xFF && buf2[1] == (char)0xFF);
}

void BinaryUtilityTest::testGetListMax()
{
	using namespace BinaryUtilityExtTestSupport;
	// getListMax（无符号整数）
	Vector<uint> ulist;
	ulist.add(3);
	ulist.add(7);
	ulist.add(1);
	ulist.add(5);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getListMax(ulist) == 7);

	// 单元素
	Vector<uint> single;
	single.add(42);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getListMax(single) == 42);

	// 数组重载
	uint arr[] = { 10, 3, 8, 1 };
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getListMax(arr, 4) == 10);

	// getListMaxAbs（有符号整数）
	Vector<int> slist;
	slist.add(-10);
	slist.add(5);
	slist.add(-3);
	slist.add(8);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getListMaxAbs(slist) == 10);

	// 数组重载
	int sarr[] = { -100, 50, -30 };
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getListMaxAbs(sarr, 3) == 100);

	// 全正
	Vector<int> allPos;
	allPos.add(1); allPos.add(2); allPos.add(3);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getListMaxAbs(allPos) == 3);

	// 全负
	Vector<int> allNeg;
	allNeg.add(-5); allNeg.add(-3); allNeg.add(-7);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getListMaxAbs(allNeg) == 7);
}

void BinaryUtilityTest::testBitFlipCycle()
{
	using namespace BinaryUtilityExtTestSupport;
	// 对 int 的每个 bit 进行 set/clear 循环验证
	for (int i = 0; i < 32; ++i)
	{
		int v = 0;
		BinaryUtility::setBit(v, i, 1);
		BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getBit(v, i) == 1);
		BinaryUtilityExtTestSupport_BUE_ASSERT(v == (1 << i));
		BinaryUtility::setBit(v, i, 0);
		BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getBit(v, i) == 0);
		BinaryUtilityExtTestSupport_BUE_ASSERT(v == 0);
	}

	// hasBit 批量
	int pattern = 0b10101010;
	for (int i = 0; i < 8; ++i)
	{
		if (i % 2 == 1)
		{
			BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::hasBit(pattern, i));
		}
		else
		{
			BinaryUtilityExtTestSupport_BUE_ASSERT(!BinaryUtility::hasBit(pattern, i));
		}
	}
}

void BinaryUtilityTest::testSetByteIsolation()
{
	using namespace BinaryUtilityExtTestSupport;
	// 确保 setByte 不影响其他字节
	int v = 0x01020304;
	// 修改 byte[0] 为 0xFF
	BinaryUtility::setByte(v, (byte)0xFF, 0);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getByte(v, 0) == 0xFF);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getByte(v, 1) == 0x03);  // 不变
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getByte(v, 2) == 0x02);  // 不变
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getByte(v, 3) == 0x01);  // 不变

	// 修改 byte[3]
	BinaryUtility::setByte(v, (byte)0x00, 3);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getByte(v, 3) == 0x00);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getByte(v, 0) == 0xFF);  // 不变
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getByte(v, 1) == 0x03);  // 不变
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getByte(v, 2) == 0x02);  // 不变
}

void BinaryUtilityTest::testBitComprehensive()
{
	using namespace BinaryUtilityExtTestSupport;
	// 验证 getHighestBit 对各种有符号类型
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getHighestBit((char)0) == 0);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getHighestBit((char)-1) == 1);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getHighestBit((short)0) == 0);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getHighestBit((short)-1) == 1);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getHighestBit((llong)0) == 0);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getHighestBit((llong)-1) == 1);

	// setHighestBit on llong
	llong lv = 0;
	BinaryUtility::setHighestBit(lv, true);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getHighestBit(lv) == 1);
	BinaryUtility::setHighestBit(lv, false);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getHighestBit(lv) == 0);

	// getLowestBit for llong
	llong lv2 = LLONG_MIN;  // 最低位是0
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getLowestBit(lv2) == 0);
	llong lv3 = -1LL;  // 所有位是1
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getLowestBit(lv3) == 1);
}

void BinaryUtilityTest::testGetBitsBoundary()
{
	using namespace BinaryUtilityExtTestSupport;
	// 获取全部8位
	byte v = 0b11110000;
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getBits(v, 0, 8) == 0b11110000);

	// 获取0位
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getBits(v, 0, 0) == 0);

	// 各种 start+count 组合
	// 0b11110000，bit4=1, bit5=1, bit6=1, bit7=1
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getBits(v, 4, 4) == 0b1111);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getBits(v, 0, 4) == 0);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getBits(v, 3, 2) == 0b10);  // bit3=0, bit4=1 → 从低到高 0b10
}

void BinaryUtilityTest::testBitConstArrays()
{
	using namespace BinaryUtilityExtTestSupport;
	// sizeof(char)=1 → 3, sizeof(short)=2 → 4, sizeof(int)=4 → 5, sizeof(llong)=8 → 6
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::SIGNED_LENGTH_MAX_BIT[1] == 3);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::SIGNED_LENGTH_MAX_BIT[2] == 4);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::SIGNED_LENGTH_MAX_BIT[4] == 5);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::SIGNED_LENGTH_MAX_BIT[8] == 6);

	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::UNSIGNED_LENGTH_MAX_BIT[1] == 3);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::UNSIGNED_LENGTH_MAX_BIT[2] == 4);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::UNSIGNED_LENGTH_MAX_BIT[4] == 5);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::UNSIGNED_LENGTH_MAX_BIT[8] == 6);
}

void BinaryUtilityTest::testGenerateBitCountStress()
{
	using namespace BinaryUtilityExtTestSupport;
	// 验证 2^n 需要 n+1 个bit
	for (int i = 0; i < 8; ++i)
	{
		byte v = (byte)(1 << i);
		byte expected = (byte)(i + 1);
		BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::generateBitCount(v) == expected);
	}

	// 验证 2^n - 1 需要 n 个bit（全1）
	for (int i = 1; i < 8; ++i)
	{
		byte v = (byte)((1 << i) - 1);
		BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::generateBitCount(v) == (byte)i);
	}

	// ushort 大值
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::generateBitCount((ushort)32768) == 16);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::generateBitCount((ushort)16384) == 15);
}

void BinaryUtilityTest::testHasBitStress()
{
	using namespace BinaryUtilityExtTestSupport;
	// 测试每个 bit 位的独立性
	for (int i = 0; i < 32; ++i)
	{
		int v = 1 << i;
		for (int j = 0; j < 32; ++j)
		{
			if (j == i)
			{
				BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::hasBit(v, j));
			}
			else
			{
				BinaryUtilityExtTestSupport_BUE_ASSERT(!BinaryUtility::hasBit(v, j));
			}
		}
	}

	// llong 63位
	llong lv = (llong)1 << 62;
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::hasBit(lv, 62));
	BinaryUtilityExtTestSupport_BUE_ASSERT(!BinaryUtility::hasBit(lv, 61));
	BinaryUtilityExtTestSupport_BUE_ASSERT(!BinaryUtility::hasBit(lv, 63));
}

void BinaryUtilityTest::testBitSetClearCycle()
{
	using namespace BinaryUtilityExtTestSupport;
	// 对 int 的每一位都做 set/clear/check
	for (int bit = 0; bit < 31; ++bit)
	{
		int v = 0;
		BinaryUtility::setBit(v, bit, true);
		BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getBit(v, bit) == 1);
		BinaryUtility::setBit(v, bit, false);
		BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getBit(v, bit) == 0);
	}
	// 同时设置多位
	int mv = 0;
	BinaryUtility::setBit(mv, 0, true);
	BinaryUtility::setBit(mv, 4, true);
	BinaryUtility::setBit(mv, 8, true);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getBit(mv, 0) == 1);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getBit(mv, 4) == 1);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getBit(mv, 8) == 1);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getBit(mv, 1) == 0);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getBit(mv, 5) == 0);
	// 清除一位后其他不变
	BinaryUtility::setBit(mv, 4, false);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getBit(mv, 0) == 1);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getBit(mv, 4) == 0);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getBit(mv, 8) == 1);
}

void BinaryUtilityTest::testHasBitConsistency()
{
	using namespace BinaryUtilityExtTestSupport;
	int v = 0b10110101;  // bits 0,2,4,5,7 set
	int setBits[] = { 0, 2, 4, 5, 7 };
	int clearBits[] = { 1, 3, 6 };
	for (int b : setBits)
	{
		BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::hasBit(v, b));
		BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getBit(v, b) == 1);
	}
	for (int b : clearBits)
	{
		BinaryUtilityExtTestSupport_BUE_ASSERT(!BinaryUtility::hasBit(v, b));
		BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getBit(v, b) == 0);
	}
	// llong 一致性
	llong lv = 0LL;
	for (int bit = 0; bit < 62; bit += 5)
	{
		BinaryUtility::setBit(lv, bit, true);
	}
	for (int bit = 0; bit < 62; bit += 5)
	{
		BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::hasBit(lv, bit));
		BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getBit(lv, bit) == 1);
	}
}

void BinaryUtilityTest::testHighestBitExtended()
{
	using namespace BinaryUtilityExtTestSupport;
	// 框架 getHighestBit 取的是"类型最高位的值(0或1)"，即符号位
	// uint: 最高位只有值 >= 0x80000000 时为 1
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getHighestBit(0u) == 0);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getHighestBit(1u) == 0);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getHighestBit(0x7FFFFFFFu) == 0);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getHighestBit(0x80000000u) == 1);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getHighestBit(0xFFFFFFFFu) == 1);
	// byte: 最高位为1 => >= 128
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getHighestBit((byte)0) == 0);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getHighestBit((byte)127) == 0);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getHighestBit((byte)128) == 1);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getHighestBit((byte)255) == 1);
	// ushort
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getHighestBit((ushort)0x7FFF) == 0);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getHighestBit((ushort)0x8000) == 1);
}

void BinaryUtilityTest::testLowestBitExtended()
{
	using namespace BinaryUtilityExtTestSupport;
	// 奇数：bit0 = 1
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getLowestBit(1u) == 1);    // 0b0001 → bit0=1
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getLowestBit(3u) == 1);    // 0b0011 → bit0=1
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getLowestBit(255u) == 1);  // 0xFF → bit0=1
	// 偶数：bit0 = 0
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getLowestBit(2u) == 0);    // 0b0010 → bit0=0
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getLowestBit(4u) == 0);    // 0b0100 → bit0=0
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getLowestBit(8u) == 0);    // 0b1000 → bit0=0
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getLowestBit(6u) == 0);    // 0b110  → bit0=0
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getLowestBit(12u) == 0);   // 0b1100 → bit0=0
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getLowestBit(24u) == 0);   // 0b11000 → bit0=0
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getLowestBit(0b10100u) == 0);   // bit0=0
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getLowestBit(0b1000000u) == 0); // bit0=0
}

void BinaryUtilityTest::testGenerateBitCountExtended()
{
	using namespace BinaryUtilityExtTestSupport;
	// 边界值
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::generateBitCount((uint)0) == 0u);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::generateBitCount((uint)1) == 1u);
	// 2的幂：恰好多用一位
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::generateBitCount((uint)2) == 2u);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::generateBitCount((uint)4) == 3u);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::generateBitCount((uint)8) == 4u);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::generateBitCount((uint)16) == 5u);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::generateBitCount((uint)32) == 6u);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::generateBitCount((uint)256) == 9u);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::generateBitCount((uint)65536) == 17u);
	// 2的幂-1：与上一个2的幂用相同位数
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::generateBitCount((uint)3) == 2u);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::generateBitCount((uint)7) == 3u);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::generateBitCount((uint)15) == 4u);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::generateBitCount((uint)31) == 5u);
	// 验证返回值就是"可表示该值的最小bit数"
	for (int bits = 1; bits <= 16; ++bits)
	{
		const uint value = (uint)bits;
		const byte bc = BinaryUtility::generateBitCount(value);
		// bc 位能表示 value：value < (1u << bc)
		BinaryUtilityExtTestSupport_BUE_ASSERT((1u << bc) > value);
		// bc-1 位不够（value >= 1u << (bc-1)，当 value 为2的幂时等号成立）
		if (bc > 0)
		{
			BinaryUtilityExtTestSupport_BUE_ASSERT((1u << (bc - 1)) <= value);
		}
	}
}

void BinaryUtilityTest::testGetSetByteExtended()
{
	using namespace BinaryUtilityExtTestSupport;
	// llong 字节操作
	llong lv = 0x0807060504030201LL;
	for (int i = 0; i < 8; ++i)
	{
		BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getByte(lv, i) == (byte)(i + 1));
	}

	// ushort
	ushort us = 0xABCD;
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getByte(us, 0) == 0xCD);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getByte(us, 1) == 0xAB);

	// setByte on llong
	llong lset = 0LL;
	BinaryUtility::setByte(lset, (byte)0xFF, 7);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getByte(lset, 7) == 0xFF);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getByte(lset, 0) == 0x00);

	// setByte 多次叠加
	int multi = 0;
	for (int i = 0; i < 4; ++i)
	{
		BinaryUtility::setByte(multi, (byte)(i * 0x11), i);
	}
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getByte(multi, 0) == 0x00);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getByte(multi, 1) == 0x11);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getByte(multi, 2) == 0x22);
	BinaryUtilityExtTestSupport_BUE_ASSERT(BinaryUtility::getByte(multi, 3) == 0x33);
}

namespace BinaryUtilityExt2TestSupport
{
	// ============================================================
	// BinaryUtilityExt2Test — BinaryUtility 扩展第二批测试
	// 覆盖：read/readInverse/write/writeInverse（基础类型）
	//        readArray/readArrayInverse/writeArray/writeArrayInverse
	//        readBuffer/writeBuffer
	//        readBoolBit/writeBoolBit
	//        readSignedIntegerBit/writeSignedIntegerBit（int/short/char）
	//        readUnsignedIntegerBit/writeUnsignedIntegerBit（uint/ushort/byte）
	//        readSignedIntegerListBit/writeSignedIntegerListBit（Vector<int>）
	//        readUnsignedIntegerListBit/writeUnsignedIntegerListBit（Vector<ushort>）
	//        readFloatBit/writeFloatListBit（Vector<float>）
	//        readDoubleBit/writeDoubleListBit（Vector<double>）
	//        readVector2/writeVector2/readVector2Inverse/writeVector2Inverse
	//        readVector2Int/writeVector2Int
	//        readVector3/writeVector3/readVector3Inverse/writeVector3Inverse
	//        readVector4/writeVector4
	//        readColor/writeColor/readColorInverse/writeColorInverse
	//        crc16/crc16Byte
	//        generateBitCount（byte/ushort/uint/ullong）
	//        getBits/bitCountToByteCount
	//        setBufferBitOne/getBufferBit
	//        copyBits
	//        getListMax/getListMaxAbs/getFloatListMaxAbs/getDoubleListMaxAbs
	//        inverseByte/swapByte
	//        zeroArray
	//        fillZeroToByteEnd
	// ============================================================

#define BinaryUtilityExt2TestSupport_BUE2_ASSERT(expr, msg) \
    if (!(expr)) { ERROR(string("BinaryUtilityExt2Test FAILED: ") + (msg)); }

#define BinaryUtilityExt2TestSupport_BUE2_FLOAT_EQ(a, b) (fabsf((a) - (b)) < 1e-4f)
#define BinaryUtilityExt2TestSupport_BUE2_DOUBLE_EQ(a, b) (fabs((a) - (b)) < 1e-3)

// ===========================================================
// read / write — 基础 POD 类型往返
// ===========================================================










	// ===========================================================
	// readInverse / writeInverse
	// ===========================================================



	// ===========================================================
	// readArray / readArrayInverse / writeArray / writeArrayInverse
	// ===========================================================


	// ===========================================================
	// readBuffer / writeBuffer
	// ===========================================================



	// ===========================================================
	// 读写时缓冲区不足返回 false
	// ===========================================================


	// ===========================================================
	// readBoolBit / writeBoolBit
	// ===========================================================


	// ===========================================================
	// writeSignedIntegerBit / readSignedIntegerBit（int）
	// ===========================================================



	// ===========================================================
	// writeUnsignedIntegerBit / readUnsignedIntegerBit
	// ===========================================================



	// ===========================================================
	// writeSignedIntegerListBit / readSignedIntegerListBit（Vector<int>）
	// ===========================================================

	// ===========================================================
	// writeUnsignedIntegerListBit / readUnsignedIntegerListBit（Vector<ushort>）
	// ===========================================================

	// ===========================================================
	// writeFloatListBit / readFloatListBit（Vector<float>）
	// ===========================================================

	// ===========================================================
	// readDoubleBit / writeDoubleListBit（Vector<double>）
	// ===========================================================

	// ===========================================================
	// writeVector2 / readVector2 / readVector2Inverse / writeVector2Inverse
	// ===========================================================


	// ===========================================================
	// writeVector3 / readVector3 / readVector3Inverse / writeVector3Inverse
	// ===========================================================


	// ===========================================================
	// writeVector4 / readVector4 / Inverse
	// ===========================================================

	// ===========================================================
	// writeColor / readColor / readColorInverse / writeColorInverse
	// Color 成员均为 byte (uint8)，write/read 以 byte 为单位
	// ===========================================================


	// ===========================================================
	// crc16 / crc16Byte
	// ===========================================================

	// ===========================================================
	// generateBitCount（byte/ushort/uint/ullong）
	// ===========================================================

	// ===========================================================
	// getBits / bitCountToByteCount
	// ===========================================================


	// ===========================================================
	// setBufferBitOne / getBufferBit
	// ===========================================================

	// ===========================================================
	// copyBits
	// ===========================================================

	// ===========================================================
	// getListMax / getListMaxAbs / getFloatListMaxAbs / getDoubleListMaxAbs
	// ===========================================================



	// ===========================================================
	// inverseByte / swapByte
	// ===========================================================

	// ===========================================================
	// zeroArray
	// ===========================================================

	// ===========================================================
	// fillZeroToByteEnd
	// ===========================================================

	// ===========================================================
	// 混合场景：write多种类型然后全部读回
	// ===========================================================

	// ===========================================================
	// 混合bit级读写场景
	// ===========================================================

	// ===========================================================
	// 连续多次读写不同起始偏移
	// ===========================================================

	// ===========================================================
	// 压力测试：大量读写
	// ===========================================================



	// ===========================================================
	// test()
	// ===========================================================
}

void BinaryUtilityTest::testBUE2_ReadWriteInt()
{
	using namespace BinaryUtilityExt2TestSupport;
	char buf[64];
	memset(buf, 0, sizeof(buf));
	int idx = 0;
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::write(buf, 64, idx, (int)12345678), "write int ok");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(idx == 4, "write int advances 4 bytes");

	int readVal = 0;
	idx = 0;
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::read(buf, 64, idx, readVal), "read int ok");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(readVal == 12345678, "read int value match");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(idx == 4, "read int advances 4 bytes");
}

void BinaryUtilityTest::testBUE2_ReadWriteShort()
{
	using namespace BinaryUtilityExt2TestSupport;
	char buf[16];
	memset(buf, 0, sizeof(buf));
	int idx = 0;
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::write(buf, 16, idx, (short)-500), "write short ok");

	short sv = 0;
	idx = 0;
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::read(buf, 16, idx, sv), "read short ok");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(sv == -500, "read short value match");
}

void BinaryUtilityTest::testBUE2_ReadWriteLlong()
{
	using namespace BinaryUtilityExt2TestSupport;
	char buf[32];
	memset(buf, 0, sizeof(buf));
	int idx = 0;
	llong writeVal = 0x0102030405060708LL;
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::write(buf, 32, idx, writeVal), "write llong ok");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(idx == 8, "write llong advances 8 bytes");

	llong readVal = 0;
	idx = 0;
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::read(buf, 32, idx, readVal), "read llong ok");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(readVal == writeVal, "read llong value match");
}

void BinaryUtilityTest::testBUE2_ReadWriteFloat()
{
	using namespace BinaryUtilityExt2TestSupport;
	char buf[16];
	memset(buf, 0, sizeof(buf));
	int idx = 0;
	float fv = 3.14159f;
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::write(buf, 16, idx, fv), "write float ok");

	float rfv = 0.0f;
	idx = 0;
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::read(buf, 16, idx, rfv), "read float ok");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtilityExt2TestSupport_BUE2_FLOAT_EQ(rfv, fv), "read float value match");
}

void BinaryUtilityTest::testBUE2_ReadWriteDouble()
{
	using namespace BinaryUtilityExt2TestSupport;
	char buf[16];
	memset(buf, 0, sizeof(buf));
	int idx = 0;
	double dv = 2.718281828;
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::write(buf, 16, idx, dv), "write double ok");

	double rdv = 0.0;
	idx = 0;
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::read(buf, 16, idx, rdv), "read double ok");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtilityExt2TestSupport_BUE2_DOUBLE_EQ(rdv, dv), "read double value match");
}

void BinaryUtilityTest::testBUE2_ReadWriteByte()
{
	using namespace BinaryUtilityExt2TestSupport;
	char buf[8];
	memset(buf, 0, sizeof(buf));
	int idx = 0;
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::write(buf, 8, idx, (byte)0xBE), "write byte ok");

	byte rb = 0;
	idx = 0;
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::read(buf, 8, idx, rb), "read byte ok");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(rb == 0xBE, "read byte value match");
}

void BinaryUtilityTest::testBUE2_ReadWriteChar()
{
	using namespace BinaryUtilityExt2TestSupport;
	char buf[8];
	memset(buf, 0, sizeof(buf));
	int idx = 0;
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::write(buf, 8, idx, (char)-100), "write char ok");

	char rc = 0;
	idx = 0;
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::read(buf, 8, idx, rc), "read char ok");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(rc == -100, "read char value match");
}

void BinaryUtilityTest::testBUE2_ReadWriteUInt()
{
	using namespace BinaryUtilityExt2TestSupport;
	char buf[8];
	memset(buf, 0, sizeof(buf));
	int idx = 0;
	uint uv = 0xDEADBEEFu;
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::write(buf, 8, idx, uv), "write uint ok");

	uint ruv = 0;
	idx = 0;
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::read(buf, 8, idx, ruv), "read uint ok");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(ruv == uv, "read uint value match");
}

void BinaryUtilityTest::testBUE2_ReadWriteUShort()
{
	using namespace BinaryUtilityExt2TestSupport;
	char buf[8];
	memset(buf, 0, sizeof(buf));
	int idx = 0;
	ushort usv = 60000u;
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::write(buf, 8, idx, usv), "write ushort ok");

	ushort rusv = 0;
	idx = 0;
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::read(buf, 8, idx, rusv), "read ushort ok");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(rusv == usv, "read ushort value match");
}

void BinaryUtilityTest::testBUE2_ReadWriteBool()
{
	using namespace BinaryUtilityExt2TestSupport;
	char buf[8];
	memset(buf, 0, sizeof(buf));
	int idx = 0;
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::write(buf, 8, idx, true), "write bool true ok");

	bool rb = false;
	idx = 0;
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::read(buf, 8, idx, rb), "read bool ok");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(rb == true, "read bool true match");

	memset(buf, 0, sizeof(buf));
	idx = 0;
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::write(buf, 8, idx, false), "write bool false ok");
	idx = 0;
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::read(buf, 8, idx, rb), "read bool false ok");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(rb == false, "read bool false match");
}

void BinaryUtilityTest::testBUE2_ReadWriteInverseInt()
{
	using namespace BinaryUtilityExt2TestSupport;
	char buf[16];
	memset(buf, 0, sizeof(buf));
	int idx = 0;
	// 正向写入
	int val = 0x01020304;
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeInverse(buf, 16, idx, val), "writeInverse int ok");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(idx == 4, "writeInverse advances 4");
	// writeInverse 用 FOR_INVERSE 从 ptr[size-1] 到 ptr[0] 顺序写入 buf
	// 小端机器 0x01020304 内存布局为 ptr={0x04,0x03,0x02,0x01}
	// 逆序（ptr[3]→ptr[2]→ptr[1]→ptr[0]）写入后 buf={0x01,0x02,0x03,0x04}
	BinaryUtilityExt2TestSupport_BUE2_ASSERT((byte)buf[0] == 0x01, "writeInverse byte[0]=0x01");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT((byte)buf[1] == 0x02, "writeInverse byte[1]=0x02");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT((byte)buf[2] == 0x03, "writeInverse byte[2]=0x03");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT((byte)buf[3] == 0x04, "writeInverse byte[3]=0x04");

	int rval = 0;
	idx = 0;
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readInverse(buf, 16, idx, rval), "readInverse int ok");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(rval == val, "readInverse int value match");
}

void BinaryUtilityTest::testBUE2_ReadWriteInverseFloat()
{
	using namespace BinaryUtilityExt2TestSupport;
	char buf[16];
	memset(buf, 0, sizeof(buf));
	int idx = 0;
	float fv = 1234.5f;
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeInverse(buf, 16, idx, fv), "writeInverse float ok");

	float rfv = 0.0f;
	idx = 0;
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readInverse(buf, 16, idx, rfv), "readInverse float ok");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtilityExt2TestSupport_BUE2_FLOAT_EQ(rfv, fv), "readInverse float value match");
}

void BinaryUtilityTest::testBUE2_ReadWriteInverseLlong()
{
	using namespace BinaryUtilityExt2TestSupport;
	char buf[16];
	memset(buf, 0, sizeof(buf));
	int idx = 0;
	llong val = 0x0102030405060708LL;
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeInverse(buf, 16, idx, val), "writeInverse llong ok");
	// 小端存储 ptr={0x08,0x07,0x06,0x05,0x04,0x03,0x02,0x01}
	// FOR_INVERSE 从 ptr[7]=0x01 到 ptr[0]=0x08 写入
	// buf[0]=0x01（最高字节），buf[7]=0x08（最低字节）
	BinaryUtilityExt2TestSupport_BUE2_ASSERT((byte)buf[0] == 0x01, "writeInverse llong byte[0]");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT((byte)buf[7] == 0x08, "writeInverse llong byte[7]");

	llong rv = 0;
	idx = 0;
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readInverse(buf, 16, idx, rv), "readInverse llong ok");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(rv == val, "readInverse llong value match");
}

void BinaryUtilityTest::testBUE2_ReadWriteArray()
{
	using namespace BinaryUtilityExt2TestSupport;
	char buf[64];
	memset(buf, 0, sizeof(buf));
	int idx = 0;
	int srcArr[5] = { 10, 20, 30, 40, 50 };
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeArray(buf, 64, idx, srcArr, 5), "writeArray ok");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(idx == 20, "writeArray advances 5*4=20 bytes");

	int dstArr[5] = { 0 };
	idx = 0;
	BinaryUtility::readArray(buf, 64, idx, dstArr, 5);
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(dstArr[0] == 10, "readArray [0]");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(dstArr[1] == 20, "readArray [1]");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(dstArr[2] == 30, "readArray [2]");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(dstArr[3] == 40, "readArray [3]");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(dstArr[4] == 50, "readArray [4]");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(idx == 20, "readArray advances 20 bytes");
}

void BinaryUtilityTest::testBUE2_ReadWriteArrayInverse()
{
	using namespace BinaryUtilityExt2TestSupport;
	char buf[64];
	memset(buf, 0, sizeof(buf));
	int idx = 0;
	int srcArr[3] = { 0x01020304, 0x05060708, 0x090A0B0C };
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeArrayInverse(buf, 64, idx, srcArr, 3), "writeArrayInverse ok");

	int dstArr[3] = { 0 };
	idx = 0;
	BinaryUtility::readArrayInverse(buf, 64, idx, dstArr, 3);
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(dstArr[0] == srcArr[0], "readArrayInverse [0]");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(dstArr[1] == srcArr[1], "readArrayInverse [1]");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(dstArr[2] == srcArr[2], "readArrayInverse [2]");
}

void BinaryUtilityTest::testBUE2_ReadWriteBuffer()
{
	using namespace BinaryUtilityExt2TestSupport;
	char src[16] = "HelloWorld12345";
	char writeDst[32];
	memset(writeDst, 0, sizeof(writeDst));
	int writeIdx = 4;
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeBuffer(writeDst, 32, writeIdx, src, 10), "writeBuffer ok");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(writeIdx == 14, "writeBuffer advances 10 bytes");

	char readDst[16];
	memset(readDst, 0, sizeof(readDst));
	int readIdx = 4;
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readBuffer(writeDst, 32, readIdx, readDst, 16, 10), "readBuffer ok");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(readIdx == 14, "readBuffer advances 10 bytes");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(memcmp(readDst, src, 10) == 0, "readBuffer data match");
}

void BinaryUtilityTest::testBUE2_ReadWriteBufferBoundary()
{
	using namespace BinaryUtilityExt2TestSupport;
	char buf[8];
	memset(buf, 0, sizeof(buf));
	int idx = 0;
	char src[4] = { 1, 2, 3, 4 };
	// 写到缓冲区末尾
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeBuffer(buf, 8, idx, src, 4), "writeBuffer boundary ok");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(idx == 4, "writeBuffer idx=4");
	// 再写4字节刚好满
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeBuffer(buf, 8, idx, src, 4), "writeBuffer fill to end ok");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(idx == 8, "writeBuffer idx=8");
	// 写入失败——超出缓冲区
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(!BinaryUtility::writeBuffer(buf, 8, idx, src, 1), "writeBuffer overflow false");
}

void BinaryUtilityTest::testBUE2_ReadBufferBoundary()
{
	using namespace BinaryUtilityExt2TestSupport;
	char buf[4] = { 0x0A, 0x0B, 0x0C, 0x0D };
	char dst[4];
	int idx = 0;
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readBuffer(buf, 4, idx, dst, 4, 4), "readBuffer full ok");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(idx == 4, "readBuffer full idx=4");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT((byte)dst[0] == 0x0A, "readBuffer dst[0]");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT((byte)dst[3] == 0x0D, "readBuffer dst[3]");

	// 超出范围
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(!BinaryUtility::readBuffer(buf, 4, idx, dst, 4, 1), "readBuffer overflow false");
}

void BinaryUtilityTest::testBUE2_ReadWriteOverflow()
{
	using namespace BinaryUtilityExt2TestSupport;
	char buf[3];
	memset(buf, 0, sizeof(buf));
	int idx = 0;
	// int 需要4字节，缓冲区只有3字节，写入应失败
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(!BinaryUtility::write(buf, 3, idx, (int)100), "write int overflow false");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(idx == 0, "write int overflow idx unchanged");

	int rv = 0;
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(!BinaryUtility::read(buf, 3, idx, rv), "read int overflow false");
}

void BinaryUtilityTest::testBUE2_MultipleReadWrite()
{
	using namespace BinaryUtilityExt2TestSupport;
	char buf[64];
	memset(buf, 0, sizeof(buf));
	int idx = 0;
	BinaryUtility::write(buf, 64, idx, (int)1001);
	BinaryUtility::write(buf, 64, idx, (short)-200);
	BinaryUtility::write(buf, 64, idx, (float)9.99f);
	BinaryUtility::write(buf, 64, idx, (byte)0xFF);
	BinaryUtility::write(buf, 64, idx, (llong)987654321LL);

	int total = idx;
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(total == 4 + 2 + 4 + 1 + 8, "multiple write total bytes");

	idx = 0;
	int ri = 0;
	short rs = 0;
	float rf = 0.0f;
	byte rb = 0;
	llong rl = 0;
	BinaryUtility::read(buf, 64, idx, ri);
	BinaryUtility::read(buf, 64, idx, rs);
	BinaryUtility::read(buf, 64, idx, rf);
	BinaryUtility::read(buf, 64, idx, rb);
	BinaryUtility::read(buf, 64, idx, rl);

	BinaryUtilityExt2TestSupport_BUE2_ASSERT(ri == 1001, "multi read int");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(rs == -200, "multi read short");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtilityExt2TestSupport_BUE2_FLOAT_EQ(rf, 9.99f), "multi read float");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(rb == 0xFF, "multi read byte");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(rl == 987654321LL, "multi read llong");
}

void BinaryUtilityTest::testBUE2_BoolBit()
{
	using namespace BinaryUtilityExt2TestSupport;
	char buf[8];
	memset(buf, 0, sizeof(buf));
	int bitIdx = 0;

	// 写入 true
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeBoolBit(buf, 8, bitIdx, true), "writeBoolBit true ok");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(bitIdx == 1, "writeBoolBit bitIdx=1");

	// 写入 false
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeBoolBit(buf, 8, bitIdx, false), "writeBoolBit false ok");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(bitIdx == 2, "writeBoolBit bitIdx=2");

	// 写入 true
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeBoolBit(buf, 8, bitIdx, true), "writeBoolBit true2 ok");

	// 读取
	bool rb = false;
	bitIdx = 0;
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readBoolBit(buf, 8, bitIdx, rb), "readBoolBit ok");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(rb == true, "readBoolBit [0]=true");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(bitIdx == 1, "readBoolBit bitIdx=1");

	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readBoolBit(buf, 8, bitIdx, rb), "readBoolBit [1] ok");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(rb == false, "readBoolBit [1]=false");

	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readBoolBit(buf, 8, bitIdx, rb), "readBoolBit [2] ok");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(rb == true, "readBoolBit [2]=true");
}

void BinaryUtilityTest::testBUE2_BoolBitMultiple()
{
	using namespace BinaryUtilityExt2TestSupport;
	char buf[4];
	memset(buf, 0, sizeof(buf));
	int bitIdx = 0;
	// 写入8个交替的 true/false
	for (int i = 0; i < 8; i++)
	{
		BinaryUtility::writeBoolBit(buf, 4, bitIdx, (i % 2 == 0));
	}
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(bitIdx == 8, "8 boolBit write bitIdx=8");

	bitIdx = 0;
	for (int i = 0; i < 8; i++)
	{
		bool rb = false;
		BinaryUtility::readBoolBit(buf, 4, bitIdx, rb);
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(rb == (i % 2 == 0), "boolBit alternating read");
	}
}

void BinaryUtilityTest::testBUE2_SignedIntBit_Int()
{
	using namespace BinaryUtilityExt2TestSupport;
	// 场景1：正数
	{
		char buf[32];
		memset(buf, 0, sizeof(buf));
		int bitIdx = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeSignedIntegerBit(buf, 32, bitIdx, (int)12345, true), "write int 12345 ok");
		int rv = 0;
		bitIdx = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readSignedIntegerBit(buf, 32, bitIdx, rv, true), "read int 12345 ok");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(rv == 12345, "int 12345 roundtrip");
	}
	// 场景2：负数
	{
		char buf[32];
		memset(buf, 0, sizeof(buf));
		int bitIdx = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeSignedIntegerBit(buf, 32, bitIdx, (int)-9876, true), "write int -9876 ok");
		int rv = 0;
		bitIdx = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readSignedIntegerBit(buf, 32, bitIdx, rv, true), "read int -9876 ok");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(rv == -9876, "int -9876 roundtrip");
	}
	// 场景3：零
	{
		char buf[16];
		memset(buf, 0, sizeof(buf));
		int bitIdx = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeSignedIntegerBit(buf, 16, bitIdx, (int)0, true), "write int 0 ok");
		int rv = -1;
		bitIdx = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readSignedIntegerBit(buf, 16, bitIdx, rv, true), "read int 0 ok");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(rv == 0, "int 0 roundtrip");
	}
	// 场景4：大正值
	{
		char buf[32];
		memset(buf, 0, sizeof(buf));
		int bitIdx = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeSignedIntegerBit(buf, 32, bitIdx, (int)2000000000, true), "write int 2e9 ok");
		int rv = 0;
		bitIdx = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readSignedIntegerBit(buf, 32, bitIdx, rv, true), "read int 2e9 ok");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(rv == 2000000000, "int 2e9 roundtrip");
	}
	// 场景5：大负值（注意不能用 INT_MIN，用 -2147483647）
	{
		char buf[32];
		memset(buf, 0, sizeof(buf));
		int bitIdx = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeSignedIntegerBit(buf, 32, bitIdx, (int)-2147483647, true), "write int INT_MIN+1 ok");
		int rv = 0;
		bitIdx = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readSignedIntegerBit(buf, 32, bitIdx, rv, true), "read int INT_MIN+1 ok");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(rv == -2147483647, "int INT_MIN+1 roundtrip");
	}
	// 场景6：无符号模式（needWriteSign=false），负值会被当作正值取反
	{
		char buf[32];
		memset(buf, 0, sizeof(buf));
		int bitIdx = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeSignedIntegerBit(buf, 32, bitIdx, (int)777, false), "write int 777 no-sign ok");
		int rv = 0;
		bitIdx = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readSignedIntegerBit(buf, 32, bitIdx, rv, false), "read int 777 no-sign ok");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(rv == 777, "int 777 no-sign roundtrip");
	}
}

void BinaryUtilityTest::testBUE2_SignedIntBit_Short()
{
	using namespace BinaryUtilityExt2TestSupport;
	// 正数
	{
		char buf[16];
		memset(buf, 0, sizeof(buf));
		int bitIdx = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeSignedIntegerBit(buf, 16, bitIdx, (short)1000, true), "write short 1000 ok");
		short rv = 0;
		bitIdx = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readSignedIntegerBit(buf, 16, bitIdx, rv, true), "read short 1000 ok");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(rv == 1000, "short 1000 roundtrip");
	}
	// 负数
	{
		char buf[16];
		memset(buf, 0, sizeof(buf));
		int bitIdx = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeSignedIntegerBit(buf, 16, bitIdx, (short)-32767, true), "write short -32767 ok");
		short rv = 0;
		bitIdx = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readSignedIntegerBit(buf, 16, bitIdx, rv, true), "read short -32767 ok");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(rv == -32767, "short -32767 roundtrip");
	}
	// 零
	{
		char buf[8];
		memset(buf, 0, sizeof(buf));
		int bitIdx = 0;
		short rv = -1;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeSignedIntegerBit(buf, 8, bitIdx, (short)0, true), "write short 0 ok");
		bitIdx = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readSignedIntegerBit(buf, 8, bitIdx, rv, true), "read short 0 ok");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(rv == 0, "short 0 roundtrip");
	}
	// 中间值
	{
		char buf[16];
		memset(buf, 0, sizeof(buf));
		int bitIdx = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeSignedIntegerBit(buf, 16, bitIdx, (short)-128, true), "write short -128 ok");
		short rv = 0;
		bitIdx = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readSignedIntegerBit(buf, 16, bitIdx, rv, true), "read short -128 ok");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(rv == -128, "short -128 roundtrip");
	}
}

void BinaryUtilityTest::testBUE2_SignedIntBit_Char()
{
	using namespace BinaryUtilityExt2TestSupport;
	// 正数
	{
		char buf[8];
		memset(buf, 0, sizeof(buf));
		int bitIdx = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeSignedIntegerBit(buf, 8, bitIdx, (char)100, true), "write char 100 ok");
		char rv = 0;
		bitIdx = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readSignedIntegerBit(buf, 8, bitIdx, rv, true), "read char 100 ok");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(rv == 100, "char 100 roundtrip");
	}
	// 负数
	{
		char buf[8];
		memset(buf, 0, sizeof(buf));
		int bitIdx = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeSignedIntegerBit(buf, 8, bitIdx, (char)-127, true), "write char -127 ok");
		char rv = 0;
		bitIdx = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readSignedIntegerBit(buf, 8, bitIdx, rv, true), "read char -127 ok");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(rv == -127, "char -127 roundtrip");
	}
	// 零
	{
		char buf[8];
		memset(buf, 0, sizeof(buf));
		int bitIdx = 0;
		char rv = -1;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeSignedIntegerBit(buf, 8, bitIdx, (char)0, true), "write char 0 ok");
		bitIdx = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readSignedIntegerBit(buf, 8, bitIdx, rv, true), "read char 0 ok");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(rv == 0, "char 0 roundtrip");
	}
	// 1 和 -1
	{
		char buf[8];
		memset(buf, 0, sizeof(buf));
		int bitIdx = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeSignedIntegerBit(buf, 8, bitIdx, (char)1, true), "write char 1 ok");
		char rv = 0;
		bitIdx = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readSignedIntegerBit(buf, 8, bitIdx, rv, true), "read char 1 ok");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(rv == 1, "char 1 roundtrip");
	}
	{
		char buf[8];
		memset(buf, 0, sizeof(buf));
		int bitIdx = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeSignedIntegerBit(buf, 8, bitIdx, (char)-1, true), "write char -1 ok");
		char rv = 0;
		bitIdx = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readSignedIntegerBit(buf, 8, bitIdx, rv, true), "read char -1 ok");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(rv == -1, "char -1 roundtrip");
	}
}

void BinaryUtilityTest::testBUE2_UnsignedIntBit_UInt()
{
	using namespace BinaryUtilityExt2TestSupport;
	// 零
	{
		char buf[16];
		memset(buf, 0, sizeof(buf));
		int bitIdx = 0;
		uint rv = 1;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeUnsignedIntegerBit(buf, 16, bitIdx, (uint)0), "write uint 0 ok");
		bitIdx = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readUnsignedIntegerBit(buf, 16, bitIdx, rv), "read uint 0 ok");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(rv == 0u, "uint 0 roundtrip");
	}
	// 小值
	{
		char buf[16];
		memset(buf, 0, sizeof(buf));
		int bitIdx = 0;
		uint rv = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeUnsignedIntegerBit(buf, 16, bitIdx, (uint)7), "write uint 7 ok");
		bitIdx = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readUnsignedIntegerBit(buf, 16, bitIdx, rv), "read uint 7 ok");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(rv == 7u, "uint 7 roundtrip");
	}
	// 大值
	{
		char buf[16];
		memset(buf, 0, sizeof(buf));
		int bitIdx = 0;
		uint rv = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeUnsignedIntegerBit(buf, 16, bitIdx, (uint)4000000000u), "write uint 4e9 ok");
		bitIdx = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readUnsignedIntegerBit(buf, 16, bitIdx, rv), "read uint 4e9 ok");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(rv == 4000000000u, "uint 4e9 roundtrip");
	}
	// 255
	{
		char buf[8];
		memset(buf, 0, sizeof(buf));
		int bitIdx = 0;
		uint rv = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeUnsignedIntegerBit(buf, 8, bitIdx, (uint)255), "write uint 255 ok");
		bitIdx = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readUnsignedIntegerBit(buf, 8, bitIdx, rv), "read uint 255 ok");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(rv == 255u, "uint 255 roundtrip");
	}
}

void BinaryUtilityTest::testBUE2_UnsignedIntBit_UShort()
{
	using namespace BinaryUtilityExt2TestSupport;
	// 零
	{
		char buf[8];
		memset(buf, 0, sizeof(buf));
		int bitIdx = 0;
		ushort rv = 1;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeUnsignedIntegerBit(buf, 8, bitIdx, (ushort)0), "write ushort 0 ok");
		bitIdx = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readUnsignedIntegerBit(buf, 8, bitIdx, rv), "read ushort 0 ok");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(rv == 0u, "ushort 0 roundtrip");
	}
	// 最大值
	{
		char buf[8];
		memset(buf, 0, sizeof(buf));
		int bitIdx = 0;
		ushort rv = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeUnsignedIntegerBit(buf, 8, bitIdx, (ushort)65535), "write ushort 65535 ok");
		bitIdx = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readUnsignedIntegerBit(buf, 8, bitIdx, rv), "read ushort 65535 ok");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(rv == 65535u, "ushort 65535 roundtrip");
	}
	// 中间值
	{
		char buf[8];
		memset(buf, 0, sizeof(buf));
		int bitIdx = 0;
		ushort rv = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeUnsignedIntegerBit(buf, 8, bitIdx, (ushort)1000), "write ushort 1000 ok");
		bitIdx = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readUnsignedIntegerBit(buf, 8, bitIdx, rv), "read ushort 1000 ok");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(rv == 1000u, "ushort 1000 roundtrip");
	}
}

void BinaryUtilityTest::testBUE2_UnsignedIntBit_Byte()
{
	using namespace BinaryUtilityExt2TestSupport;
	// 零
	{
		char buf[4];
		memset(buf, 0, sizeof(buf));
		int bitIdx = 0;
		byte rv = 1;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeUnsignedIntegerBit(buf, 4, bitIdx, (byte)0), "write byte 0 ok");
		bitIdx = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readUnsignedIntegerBit(buf, 4, bitIdx, rv), "read byte 0 ok");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(rv == 0u, "byte 0 roundtrip");
	}
	// 255（最大）
	{
		char buf[4];
		memset(buf, 0, sizeof(buf));
		int bitIdx = 0;
		byte rv = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeUnsignedIntegerBit(buf, 4, bitIdx, (byte)255), "write byte 255 ok");
		bitIdx = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readUnsignedIntegerBit(buf, 4, bitIdx, rv), "read byte 255 ok");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(rv == 255u, "byte 255 roundtrip");
	}
	// 1
	{
		char buf[4];
		memset(buf, 0, sizeof(buf));
		int bitIdx = 0;
		byte rv = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeUnsignedIntegerBit(buf, 4, bitIdx, (byte)1), "write byte 1 ok");
		bitIdx = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readUnsignedIntegerBit(buf, 4, bitIdx, rv), "read byte 1 ok");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(rv == 1u, "byte 1 roundtrip");
	}
	// 128
	{
		char buf[4];
		memset(buf, 0, sizeof(buf));
		int bitIdx = 0;
		byte rv = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeUnsignedIntegerBit(buf, 4, bitIdx, (byte)128), "write byte 128 ok");
		bitIdx = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readUnsignedIntegerBit(buf, 4, bitIdx, rv), "read byte 128 ok");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(rv == 128u, "byte 128 roundtrip");
	}
}

void BinaryUtilityTest::testBUE2_SignedIntListBit()
{
	using namespace BinaryUtilityExt2TestSupport;
	// 场景1：多个正数
	{
		char buf[128];
		memset(buf, 0, sizeof(buf));
		int bitIdx = 0;
		Vector<int> src;
		src.add(100);
		src.add(200);
		src.add(300);
		src.add(400);
		src.add(500);
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeSignedIntegerListBit(buf, 128, bitIdx, src, true), "writeSignedIntListBit positive ok");
		Vector<int> dst;
		bitIdx = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readSignedIntegerListBit(buf, 128, bitIdx, dst, true), "readSignedIntListBit positive ok");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(dst.size() == 5, "signed int list size=5");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(dst[0] == 100 && dst[4] == 500, "signed int list values");
	}
	// 场景2：混合正负数
	{
		char buf[128];
		memset(buf, 0, sizeof(buf));
		int bitIdx = 0;
		Vector<int> src;
		src.add(-100);
		src.add(200);
		src.add(-300);
		src.add(400);
		src.add(-500);
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeSignedIntegerListBit(buf, 128, bitIdx, src, true), "writeSignedIntListBit mixed ok");
		Vector<int> dst;
		bitIdx = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readSignedIntegerListBit(buf, 128, bitIdx, dst, true), "readSignedIntListBit mixed ok");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(dst.size() == 5, "signed int list mixed size=5");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(dst[0] == -100, "signed int list [0]=-100");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(dst[1] == 200, "signed int list [1]=200");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(dst[2] == -300, "signed int list [2]=-300");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(dst[3] == 400, "signed int list [3]=400");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(dst[4] == -500, "signed int list [4]=-500");
	}
	// 场景3：空列表
	{
		char buf[32];
		memset(buf, 0, sizeof(buf));
		int bitIdx = 0;
		Vector<int> src;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeSignedIntegerListBit(buf, 32, bitIdx, src, true), "writeSignedIntListBit empty ok");
		Vector<int> dst;
		bitIdx = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readSignedIntegerListBit(buf, 32, bitIdx, dst, true), "readSignedIntListBit empty ok");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(dst.size() == 0, "signed int list empty size=0");
	}
	// 场景4：全零
	{
		char buf[64];
		memset(buf, 0, sizeof(buf));
		int bitIdx = 0;
		Vector<int> src;
		src.add(0);
		src.add(0);
		src.add(0);
		src.add(0);
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeSignedIntegerListBit(buf, 64, bitIdx, src, true), "writeSignedIntListBit zeros ok");
		Vector<int> dst;
		bitIdx = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readSignedIntegerListBit(buf, 64, bitIdx, dst, true), "readSignedIntListBit zeros ok");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(dst.size() == 4, "signed int list zero size=4");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(dst[0] == 0 && dst[3] == 0, "signed int list zero values");
	}
	// 场景5：单元素
	{
		char buf[32];
		memset(buf, 0, sizeof(buf));
		int bitIdx = 0;
		Vector<int> src;
		src.add(-7777);
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeSignedIntegerListBit(buf, 32, bitIdx, src, true), "writeSignedIntListBit single ok");
		Vector<int> dst;
		bitIdx = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readSignedIntegerListBit(buf, 32, bitIdx, dst, true), "readSignedIntListBit single ok");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(dst.size() == 1, "signed int list single size=1");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(dst[0] == -7777, "signed int list single value");
	}
	// 场景6：较大数量（20个）
	{
		char buf[512];
		memset(buf, 0, sizeof(buf));
		int bitIdx = 0;
		Vector<int> src;
		for (int i = 0; i < 20; i++)
		{
			src.add(i % 2 == 0 ? i * 1000 : -(i * 1000));
		}
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeSignedIntegerListBit(buf, 512, bitIdx, src, true), "writeSignedIntListBit 20 ok");
		Vector<int> dst;
		bitIdx = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readSignedIntegerListBit(buf, 512, bitIdx, dst, true), "readSignedIntListBit 20 ok");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(dst.size() == 20, "signed int list 20 size");
		for (int i = 0; i < 20; i++)
		{
			BinaryUtilityExt2TestSupport_BUE2_ASSERT(dst[i] == src[i], "signed int list 20 value match");
		}
	}
}

void BinaryUtilityTest::testBUE2_UnsignedShortListBit()
{
	using namespace BinaryUtilityExt2TestSupport;
	// 场景1：多个值
	{
		char buf[128];
		memset(buf, 0, sizeof(buf));
		int bitIdx = 0;
		Vector<ushort> src;
		src.add(10);
		src.add(20);
		src.add(30);
		src.add(40);
		src.add(50);
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeUnsignedIntegerListBit(buf, 128, bitIdx, src), "writeUnsignedShortListBit ok");
		Vector<ushort> dst;
		bitIdx = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readUnsignedIntegerListBit(buf, 128, bitIdx, dst), "readUnsignedShortListBit ok");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(dst.size() == 5, "ushort list size=5");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(dst[0] == 10 && dst[4] == 50, "ushort list values");
	}
	// 场景2：空列表
	{
		char buf[16];
		memset(buf, 0, sizeof(buf));
		int bitIdx = 0;
		Vector<ushort> src;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeUnsignedIntegerListBit(buf, 16, bitIdx, src), "writeUnsignedShortListBit empty ok");
		Vector<ushort> dst;
		bitIdx = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readUnsignedIntegerListBit(buf, 16, bitIdx, dst), "readUnsignedShortListBit empty ok");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(dst.size() == 0, "ushort list empty");
	}
	// 场景3：含零
	{
		char buf[64];
		memset(buf, 0, sizeof(buf));
		int bitIdx = 0;
		Vector<ushort> src;
		src.add(0);
		src.add(100);
		src.add(0);
		src.add(200);
		src.add(0);
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeUnsignedIntegerListBit(buf, 64, bitIdx, src), "writeUnsignedShortListBit withZero ok");
		Vector<ushort> dst;
		bitIdx = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readUnsignedIntegerListBit(buf, 64, bitIdx, dst), "readUnsignedShortListBit withZero ok");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(dst.size() == 5, "ushort list with-zero size=5");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(dst[0] == 0 && dst[1] == 100 && dst[3] == 200, "ushort list with-zero values");
	}
	// 场景4：最大值
	{
		char buf[64];
		memset(buf, 0, sizeof(buf));
		int bitIdx = 0;
		Vector<ushort> src;
		src.add(65535);
		src.add(65535);
		src.add(65535);
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeUnsignedIntegerListBit(buf, 64, bitIdx, src), "writeUnsignedShortListBit max ok");
		Vector<ushort> dst;
		bitIdx = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readUnsignedIntegerListBit(buf, 64, bitIdx, dst), "readUnsignedShortListBit max ok");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(dst.size() == 3, "ushort list max size=3");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(dst[0] == 65535 && dst[2] == 65535, "ushort list max values");
	}
}

void BinaryUtilityTest::testBUE2_FloatListBit()
{
	using namespace BinaryUtilityExt2TestSupport;
	// 场景1：正数列表
	{
		char buf[256];
		memset(buf, 0, sizeof(buf));
		int bitIdx = 0;
		Vector<float> src;
		src.add(1.5f);
		src.add(2.5f);
		src.add(3.5f);
		src.add(4.5f);
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeFloatListBit(buf, 256, bitIdx, src, true, 3), "writeFloatListBit positive ok");
		Vector<float> dst;
		bitIdx = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readFloatListBit(buf, 256, bitIdx, dst, true, 3), "readFloatListBit positive ok");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(dst.size() == 4, "float list size=4");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtilityExt2TestSupport_BUE2_FLOAT_EQ(dst[0], 1.5f), "float list [0]=1.5");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtilityExt2TestSupport_BUE2_FLOAT_EQ(dst[1], 2.5f), "float list [1]=2.5");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtilityExt2TestSupport_BUE2_FLOAT_EQ(dst[2], 3.5f), "float list [2]=3.5");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtilityExt2TestSupport_BUE2_FLOAT_EQ(dst[3], 4.5f), "float list [3]=4.5");
	}
	// 场景2：含负数
	{
		char buf[256];
		memset(buf, 0, sizeof(buf));
		int bitIdx = 0;
		Vector<float> src;
		src.add(-1.0f);
		src.add(2.0f);
		src.add(-3.0f);
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeFloatListBit(buf, 256, bitIdx, src, true, 3), "writeFloatListBit neg ok");
		Vector<float> dst;
		bitIdx = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readFloatListBit(buf, 256, bitIdx, dst, true, 3), "readFloatListBit neg ok");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(dst.size() == 3, "float neg list size=3");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtilityExt2TestSupport_BUE2_FLOAT_EQ(dst[0], -1.0f), "float neg [0]");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtilityExt2TestSupport_BUE2_FLOAT_EQ(dst[1], 2.0f), "float neg [1]");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtilityExt2TestSupport_BUE2_FLOAT_EQ(dst[2], -3.0f), "float neg [2]");
	}
	// 场景3：空列表
	{
		char buf[32];
		memset(buf, 0, sizeof(buf));
		int bitIdx = 0;
		Vector<float> src;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeFloatListBit(buf, 32, bitIdx, src, true, 3), "writeFloatListBit empty ok");
		Vector<float> dst;
		bitIdx = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readFloatListBit(buf, 32, bitIdx, dst, true, 3), "readFloatListBit empty ok");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(dst.size() == 0, "float list empty size=0");
	}
}

void BinaryUtilityTest::testBUE2_DoubleListBit()
{
	using namespace BinaryUtilityExt2TestSupport;
	// 场景1：正数列表
	{
		char buf[512];
		memset(buf, 0, sizeof(buf));
		int bitIdx = 0;
		Vector<double> src;
		src.add(1.23456);
		src.add(2.34567);
		src.add(3.45678);
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeDoubleListBit(buf, 512, bitIdx, src, true, 4), "writeDoubleListBit ok");
		Vector<double> dst;
		bitIdx = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readDoubleListBit(buf, 512, bitIdx, dst, true, 4), "readDoubleListBit ok");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(dst.size() == 3, "double list size=3");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtilityExt2TestSupport_BUE2_DOUBLE_EQ(dst[0], 1.23456), "double list [0]");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtilityExt2TestSupport_BUE2_DOUBLE_EQ(dst[1], 2.34567), "double list [1]");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtilityExt2TestSupport_BUE2_DOUBLE_EQ(dst[2], 3.45678), "double list [2]");
	}
	// 场景2：空列表
	{
		char buf[32];
		memset(buf, 0, sizeof(buf));
		int bitIdx = 0;
		Vector<double> src;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeDoubleListBit(buf, 32, bitIdx, src, true, 4), "writeDoubleListBit empty ok");
		Vector<double> dst;
		bitIdx = 0;
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readDoubleListBit(buf, 32, bitIdx, dst, true, 4), "readDoubleListBit empty ok");
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(dst.size() == 0, "double list empty size=0");
	}
}

void BinaryUtilityTest::testBUE2_Vector2ReadWrite()
{
	using namespace BinaryUtilityExt2TestSupport;
	char buf[64];
	memset(buf, 0, sizeof(buf));
	int idx = 0;
	Vector2 v = { 1.5f, -2.5f };
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeVector2(buf, 64, idx, v), "writeVector2 ok");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(idx == 8, "writeVector2 advances 8 bytes");

	Vector2 rv = { 0.0f, 0.0f };
	idx = 0;
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readVector2(buf, 64, idx, rv), "readVector2 ok");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtilityExt2TestSupport_BUE2_FLOAT_EQ(rv.x, 1.5f), "readVector2 x");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtilityExt2TestSupport_BUE2_FLOAT_EQ(rv.y, -2.5f), "readVector2 y");

	// Inverse
	memset(buf, 0, sizeof(buf));
	idx = 0;
	Vector2 vi = { 3.14f, 2.71f };
	// writeInverse for vector2 via writeVector2Inverse not defined, use manual
	BinaryUtility::writeInverse(buf, 64, idx, vi.x);
	BinaryUtility::writeInverse(buf, 64, idx, vi.y);
	Vector2 rvi = { 0.0f, 0.0f };
	idx = 0;
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readVector2Inverse(buf, 64, idx, rvi), "readVector2Inverse ok");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtilityExt2TestSupport_BUE2_FLOAT_EQ(rvi.x, vi.x), "readVector2Inverse x");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtilityExt2TestSupport_BUE2_FLOAT_EQ(rvi.y, vi.y), "readVector2Inverse y");
}

void BinaryUtilityTest::testBUE2_Vector2IntReadWrite()
{
	using namespace BinaryUtilityExt2TestSupport;
	char buf[32];
	memset(buf, 0, sizeof(buf));
	int idx = 0;
	Vector2Int v = { 100, -200 };
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeVector2Int(buf, 32, idx, v), "writeVector2Int ok");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(idx == 8, "writeVector2Int advances 8 bytes");

	Vector2Int rv = { 0, 0 };
	idx = 0;
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readVector2Int(buf, 32, idx, rv), "readVector2Int ok");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(rv.x == 100, "readVector2Int x");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(rv.y == -200, "readVector2Int y");

	// InverseWrite/Read
	memset(buf, 0, sizeof(buf));
	idx = 0;
	Vector2Int vi = { 300, -400 };
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeVector2IntInverse(buf, 32, idx, vi), "writeVector2IntInverse ok");
	Vector2Int rvi = { 0, 0 };
	idx = 0;
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readVector2IntInverse(buf, 32, idx, rvi), "readVector2IntInverse ok");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(rvi.x == 300, "readVector2IntInverse x");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(rvi.y == -400, "readVector2IntInverse y");
}

void BinaryUtilityTest::testBUE2_Vector3ReadWrite()
{
	using namespace BinaryUtilityExt2TestSupport;
	char buf[64];
	memset(buf, 0, sizeof(buf));
	int idx = 0;
	Vector3 v = { 1.0f, 2.0f, 3.0f };
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeVector3(buf, 64, idx, v), "writeVector3 ok");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(idx == 12, "writeVector3 advances 12 bytes");

	Vector3 rv = { 0.0f, 0.0f, 0.0f };
	idx = 0;
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readVector3(buf, 64, idx, rv), "readVector3 ok");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtilityExt2TestSupport_BUE2_FLOAT_EQ(rv.x, 1.0f), "readVector3 x");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtilityExt2TestSupport_BUE2_FLOAT_EQ(rv.y, 2.0f), "readVector3 y");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtilityExt2TestSupport_BUE2_FLOAT_EQ(rv.z, 3.0f), "readVector3 z");

	// Inverse
	memset(buf, 0, sizeof(buf));
	idx = 0;
	Vector3 vi = { -1.0f, -2.0f, -3.0f };
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeVector3Inverse(buf, 64, idx, vi), "writeVector3Inverse ok");
	Vector3 rvi = { 0.0f, 0.0f, 0.0f };
	idx = 0;
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readVector3Inverse(buf, 64, idx, rvi), "readVector3Inverse ok");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtilityExt2TestSupport_BUE2_FLOAT_EQ(rvi.x, -1.0f), "readVector3Inverse x");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtilityExt2TestSupport_BUE2_FLOAT_EQ(rvi.y, -2.0f), "readVector3Inverse y");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtilityExt2TestSupport_BUE2_FLOAT_EQ(rvi.z, -3.0f), "readVector3Inverse z");
}

void BinaryUtilityTest::testBUE2_Vector3IntReadWrite()
{
	using namespace BinaryUtilityExt2TestSupport;
	char buf[32];
	memset(buf, 0, sizeof(buf));
	int idx = 0;
	Vector3Int vi3 = { 10, 20, 30 };
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::write(buf, 32, idx, vi3.x), "writeVector3Int.x ok");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::write(buf, 32, idx, vi3.y), "writeVector3Int.y ok");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::write(buf, 32, idx, vi3.z), "writeVector3Int.z ok");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(idx == 12, "write Vector3Int 12 bytes");

	Vector3Int rv = { 0, 0, 0 };
	idx = 0;
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readVector3Int(buf, 32, idx, rv), "readVector3Int ok");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(rv.x == 10, "readVector3Int x");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(rv.y == 20, "readVector3Int y");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(rv.z == 30, "readVector3Int z");
}

void BinaryUtilityTest::testBUE2_Vector4ReadWrite()
{
	using namespace BinaryUtilityExt2TestSupport;
	char buf[64];
	memset(buf, 0, sizeof(buf));
	int idx = 0;
	Vector4 v = { 1.0f, 2.0f, 3.0f, 4.0f };
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeVector4(buf, 64, idx, v), "writeVector4 ok");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(idx == 16, "writeVector4 advances 16 bytes");

	Vector4 rv = { 0.0f, 0.0f, 0.0f, 0.0f };
	idx = 0;
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readVector4(buf, 64, idx, rv), "readVector4 ok");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtilityExt2TestSupport_BUE2_FLOAT_EQ(rv.x, 1.0f), "readVector4 x");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtilityExt2TestSupport_BUE2_FLOAT_EQ(rv.y, 2.0f), "readVector4 y");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtilityExt2TestSupport_BUE2_FLOAT_EQ(rv.z, 3.0f), "readVector4 z");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtilityExt2TestSupport_BUE2_FLOAT_EQ(rv.w, 4.0f), "readVector4 w");

	// Inverse
	memset(buf, 0, sizeof(buf));
	idx = 0;
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeVector4Inverse(buf, 64, idx, v), "writeVector4Inverse ok");
	Vector4 rvi = { 0.0f, 0.0f, 0.0f, 0.0f };
	idx = 0;
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readVector4Inverse(buf, 64, idx, rvi), "readVector4Inverse ok");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtilityExt2TestSupport_BUE2_FLOAT_EQ(rvi.x, 1.0f), "readVector4Inverse x");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtilityExt2TestSupport_BUE2_FLOAT_EQ(rvi.w, 4.0f), "readVector4Inverse w");
}

void BinaryUtilityTest::testBUE2_ColorReadWrite()
{
	using namespace BinaryUtilityExt2TestSupport;
	char buf[64];
	memset(buf, 0, sizeof(buf));
	int idx = 0;
	Color c(25, 127, 229, 255);
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeColor(buf, 64, idx, c), "writeColor ok");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(idx == 4, "writeColor advances 4 bytes (4 bytes)");

	Color rc;
	idx = 0;
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readColor(buf, 64, idx, rc), "readColor ok");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(rc.r == 25, "readColor r==25");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(rc.g == 127, "readColor g==127");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(rc.b == 229, "readColor b==229");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(rc.a == 255, "readColor a==255");

	// Inverse（字节序反转后读出应相等）
	memset(buf, 0, sizeof(buf));
	idx = 0;
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::writeColorInverse(buf, 64, idx, c), "writeColorInverse ok");
	Color rci;
	idx = 0;
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::readColorInverse(buf, 64, idx, rci), "readColorInverse ok");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(rci.r == c.r, "readColorInverse r match");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(rci.g == c.g, "readColorInverse g match");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(rci.b == c.b, "readColorInverse b match");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(rci.a == c.a, "readColorInverse a match");
}

void BinaryUtilityTest::testBUE2_ColorEdge()
{
	using namespace BinaryUtilityExt2TestSupport;
	char buf[64];
	memset(buf, 0, sizeof(buf));
	int idx = 0;
	// 边界颜色：全黑
	Color black(0, 0, 0, 0);
	BinaryUtility::writeColor(buf, 64, idx, black);
	Color rb(255, 255, 255, 255);
	idx = 0;
	BinaryUtility::readColor(buf, 64, idx, rb);
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(rb.r == 0, "black r==0");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(rb.a == 0, "black a==0");

	// 全白
	memset(buf, 0, sizeof(buf));
	idx = 0;
	Color white(255, 255, 255, 255);
	BinaryUtility::writeColor(buf, 64, idx, white);
	Color rw(0, 0, 0, 0);
	idx = 0;
	BinaryUtility::readColor(buf, 64, idx, rw);
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(rw.r == 255, "white r==255");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(rw.a == 255, "white a==255");
}

void BinaryUtilityTest::testBUE2_CRC16()
{
	using namespace BinaryUtilityExt2TestSupport;
	// 场景1：空数据
	ushort crc0 = BinaryUtility::crc16(0, nullptr, 0);
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(crc0 == 0, "crc16 empty = 0");

	// 场景2：已知数据验证
	const char data1[] = { 0x31, 0x32, 0x33, 0x34, 0x35 };
	ushort crc1a = BinaryUtility::crc16(0, data1, 5);
	ushort crc1b = BinaryUtility::crc16(0, data1, 5);
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(crc1a == crc1b, "crc16 deterministic");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(crc1a != 0, "crc16 non-empty != 0");

	// 场景3：不同数据产生不同CRC
	const char data2[] = { 0x41, 0x42, 0x43, 0x44, 0x45 };
	ushort crc2 = BinaryUtility::crc16(0, data2, 5);
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(crc1a != crc2, "crc16 different data different result");

	// 场景4：crc16Byte 逐字节计算
	ushort crcByByte = 0;
	for (int i = 0; i < 5; i++)
	{
		crcByByte = BinaryUtility::crc16Byte(crcByByte, (byte)data1[i]);
	}
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(crcByByte == crc1a, "crc16Byte same as crc16");

	// 场景5：单字节
	const char single[1] = { 0x01 };
	ushort crcSingle = BinaryUtility::crc16(0, single, 1);
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(crcSingle != 0, "crc16 single byte != 0");

	// 场景6：bufferOffset
	const char padded[] = { 0x00, 0x00, 0x31, 0x32, 0x33, 0x34, 0x35 };
	ushort crcOffset = BinaryUtility::crc16(0, padded, 5, 2);
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(crcOffset == crc1a, "crc16 with offset same as without");

	// 场景7：累积CRC
	ushort crcPart1 = BinaryUtility::crc16(0, data1, 2);
	ushort crcFull = BinaryUtility::crc16(crcPart1, data1, 3, 2);
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(crcFull == crc1a, "crc16 incremental same as full");
}

void BinaryUtilityTest::testBUE2_GenerateBitCount()
{
	using namespace BinaryUtilityExt2TestSupport;
	// byte
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::generateBitCount((byte)0) == 0, "generateBitCount byte 0 = 0");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::generateBitCount((byte)1) == 1, "generateBitCount byte 1 = 1");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::generateBitCount((byte)2) == 2, "generateBitCount byte 2 = 2");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::generateBitCount((byte)3) == 2, "generateBitCount byte 3 = 2");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::generateBitCount((byte)4) == 3, "generateBitCount byte 4 = 3");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::generateBitCount((byte)7) == 3, "generateBitCount byte 7 = 3");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::generateBitCount((byte)8) == 4, "generateBitCount byte 8 = 4");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::generateBitCount((byte)127) == 7, "generateBitCount byte 127 = 7");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::generateBitCount((byte)128) == 8, "generateBitCount byte 128 = 8");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::generateBitCount((byte)255) == 8, "generateBitCount byte 255 = 8");

	// ushort
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::generateBitCount((ushort)0) == 0, "generateBitCount ushort 0 = 0");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::generateBitCount((ushort)1) == 1, "generateBitCount ushort 1 = 1");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::generateBitCount((ushort)255) == 8, "generateBitCount ushort 255 = 8");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::generateBitCount((ushort)256) == 9, "generateBitCount ushort 256 = 9");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::generateBitCount((ushort)1000) == 10, "generateBitCount ushort 1000 = 10");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::generateBitCount((ushort)65535) == 16, "generateBitCount ushort 65535 = 16");

	// uint
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::generateBitCount((uint)0) == 0, "generateBitCount uint 0 = 0");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::generateBitCount((uint)1) == 1, "generateBitCount uint 1 = 1");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::generateBitCount((uint)65535) == 16, "generateBitCount uint 65535 = 16");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::generateBitCount((uint)65536) == 17, "generateBitCount uint 65536 = 17");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::generateBitCount((uint)0xFFFFFFFFu) == 32, "generateBitCount uint max = 32");

	// ullong
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::generateBitCount((ullong)0) == 0, "generateBitCount ullong 0 = 0");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::generateBitCount((ullong)1) == 1, "generateBitCount ullong 1 = 1");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::generateBitCount((ullong)0xFFFFFFFFull) == 32, "generateBitCount ullong 32bit max = 32");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::generateBitCount((ullong)0x100000000ull) == 33, "generateBitCount ullong 2^32 = 33");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::generateBitCount((ullong)0xFFFFFFFFFFFFFFFFull) == 64, "generateBitCount ullong max = 64");
}

void BinaryUtilityTest::testBUE2_GetBits()
{
	using namespace BinaryUtilityExt2TestSupport;
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::getBits(0b11111111, 0, 4) == 0b00001111, "getBits [0,4) of 0xFF");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::getBits(0b11111111, 4, 4) == 0b00001111, "getBits [4,4) of 0xFF");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::getBits(0b10101010, 1, 4) == 0b00000101, "getBits [1,4) of 0xAA");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::getBits(0b00000001, 0, 1) == 1, "getBits single bit=1");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::getBits(0b00000000, 0, 8) == 0, "getBits all zero");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::getBits(0b11110000, 4, 4) == 0b00001111, "getBits upper nibble");
}

void BinaryUtilityTest::testBUE2_BitCountToByteCount()
{
	using namespace BinaryUtilityExt2TestSupport;
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::bitCountToByteCount(0) == 0, "bitCountToByteCount 0 = 0");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::bitCountToByteCount(1) == 1, "bitCountToByteCount 1 = 1");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::bitCountToByteCount(8) == 1, "bitCountToByteCount 8 = 1");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::bitCountToByteCount(9) == 2, "bitCountToByteCount 9 = 2");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::bitCountToByteCount(16) == 2, "bitCountToByteCount 16 = 2");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::bitCountToByteCount(17) == 3, "bitCountToByteCount 17 = 3");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::bitCountToByteCount(24) == 3, "bitCountToByteCount 24 = 3");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::bitCountToByteCount(25) == 4, "bitCountToByteCount 25 = 4");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::bitCountToByteCount(32) == 4, "bitCountToByteCount 32 = 4");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::bitCountToByteCount(64) == 8, "bitCountToByteCount 64 = 8");
}

void BinaryUtilityTest::testBUE2_BufferBit()
{
	using namespace BinaryUtilityExt2TestSupport;
	char buf[8];
	memset(buf, 0, sizeof(buf));

	// 设置第0位
	BinaryUtility::setBufferBitOne(buf, 0);
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::getBufferBit(buf, 0) == true, "bit[0] = 1");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::getBufferBit(buf, 1) == false, "bit[1] = 0");

	// 设置第7位
	BinaryUtility::setBufferBitOne(buf, 7);
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::getBufferBit(buf, 7) == true, "bit[7] = 1");

	// 设置第8位（第二字节的第0位）
	BinaryUtility::setBufferBitOne(buf, 8);
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::getBufferBit(buf, 8) == true, "bit[8] = 1");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::getBufferBit(buf, 9) == false, "bit[9] = 0");

	// 设置第15位
	BinaryUtility::setBufferBitOne(buf, 15);
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::getBufferBit(buf, 15) == true, "bit[15] = 1");

	// 检查第1~6位仍然为0
	for (int i = 1; i < 7; i++)
	{
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::getBufferBit(buf, i) == false, "bit[1~6] = 0");
	}

	// 验证连续设置
	memset(buf, 0, sizeof(buf));
	for (int i = 0; i < 32; i += 2)
	{
		BinaryUtility::setBufferBitOne(buf, i);
	}
	for (int i = 0; i < 32; i++)
	{
		bool expected = (i % 2 == 0);
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::getBufferBit(buf, i) == expected, "alternating bits");
	}
}

void BinaryUtilityTest::testBUE2_CopyBits()
{
	using namespace BinaryUtilityExt2TestSupport;
	// 基本测试：将source的低3位复制到dest的高3位（从位5开始）
	byte dest = 0;
	byte src = 0b00000101;
	BinaryUtility::copyBits(dest, (byte)5, src, (byte)0, (byte)3);
	// 期望：dest的[5..7]被设置为101
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::getBits(dest, 5, 3) == 0b101, "copyBits dest[5..7]=101");

	// 复制到位0
	byte dest2 = 0;
	BinaryUtility::copyBits(dest2, (byte)0, (byte)0b11001010, (byte)2, (byte)4);
	// 从src的位2开始取4位 = 0b0010
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::getBits(dest2, 0, 4) == 0b0010, "copyBits to pos 0");

	// 覆盖测试：从1开始复制2位
	// copyBits 是 or 操作，先用0初始化再设置
	byte dest3clean = 0;
	BinaryUtility::copyBits(dest3clean, (byte)1, (byte)0b10101010, (byte)1, (byte)2);
	// 从src位1取2位 = 0b01
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::getBits(dest3clean, 1, 2) == 0b01, "copyBits 2 bits from pos 1");

	// char 版本
	char cdest = 0;
	BinaryUtility::copyBits(cdest, (byte)0, (byte)0b11110000, (byte)4, (byte)4);
	// 从src位4取4位 = 0b1111
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::getBits((byte)cdest, 0, 4) == 0b1111, "copyBits char version");
}

void BinaryUtilityTest::testBUE2_GetListMax()
{
	using namespace BinaryUtilityExt2TestSupport;
	// getListMax（无符号整数）
	Vector<uint> ulist;
	ulist.add(10u);
	ulist.add(50u);
	ulist.add(30u);
	ulist.add(20u);
	ulist.add(40u);
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::getListMax(ulist) == 50u, "getListMax uint vector");

	uint uArr[5] = { 5u, 15u, 3u, 100u, 7u };
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::getListMax(uArr, 5) == 100u, "getListMax uint array");

	// 单元素
	Vector<ushort> single;
	single.add((ushort)42);
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::getListMax(single) == 42u, "getListMax single element");

	// 空列表返回0
	Vector<uint> empty;
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::getListMax(empty) == 0u, "getListMax empty = 0");
}

void BinaryUtilityTest::testBUE2_GetListMaxAbs()
{
	using namespace BinaryUtilityExt2TestSupport;
	// getListMaxAbs（有符号整数）
	Vector<int> ilist;
	ilist.add(-100);
	ilist.add(50);
	ilist.add(-30);
	ilist.add(200);
	ilist.add(-10);
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::getListMaxAbs(ilist) == 200, "getListMaxAbs int vector = 200");

	int iArr[4] = { -500, 100, -300, 200 };
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::getListMaxAbs(iArr, 4) == 500, "getListMaxAbs int array = 500");

	// 全正
	Vector<int> allPos;
	allPos.add(1);
	allPos.add(2);
	allPos.add(3);
	allPos.add(4);
	allPos.add(5);
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::getListMaxAbs(allPos) == 5, "getListMaxAbs all positive");

	// 全负
	Vector<int> allNeg;
	allNeg.add(-1);
	allNeg.add(-2);
	allNeg.add(-3);
	allNeg.add(-4);
	allNeg.add(-5);
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::getListMaxAbs(allNeg) == 5, "getListMaxAbs all negative");

	// 空列表
	Vector<int> empty;
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::getListMaxAbs(empty) == 0, "getListMaxAbs empty = 0");
}

void BinaryUtilityTest::testBUE2_GetFloatDoubleListMaxAbs()
{
	using namespace BinaryUtilityExt2TestSupport;
	// getFloatListMaxAbs
	float farr[5] = { 1.5f, -3.5f, 2.5f, -5.5f, 4.5f };
	float fmax = BinaryUtility::getFloatListMaxAbs(farr, 5);
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtilityExt2TestSupport_BUE2_FLOAT_EQ(fmax, 5.5f), "getFloatListMaxAbs = 5.5");

	float farr2[3] = { 0.0f, 0.0f, 0.0f };
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtilityExt2TestSupport_BUE2_FLOAT_EQ(BinaryUtility::getFloatListMaxAbs(farr2, 3), 0.0f), "getFloatListMaxAbs zero");

	// getDoubleListMaxAbs
	double darr[4] = { 1.1, -2.2, 3.3, -4.4 };
	double dmax = BinaryUtility::getDoubleListMaxAbs(darr, 4);
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtilityExt2TestSupport_BUE2_DOUBLE_EQ(dmax, 4.4), "getDoubleListMaxAbs = 4.4");
}

void BinaryUtilityTest::testBUE2_InverseSwapByte()
{
	using namespace BinaryUtilityExt2TestSupport;
	// inverseByte — int
	int v = 0x01020304;
	BinaryUtility::inverseByte(v);
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(v == 0x04030201, "inverseByte int");

	// inverseByte — short
	short sv = (short)0x0102;
	BinaryUtility::inverseByte(sv);
	BinaryUtilityExt2TestSupport_BUE2_ASSERT((ushort)sv == (ushort)0x0201, "inverseByte short");

	// inverseByte — llong
	llong lv = 0x0102030405060708LL;
	BinaryUtility::inverseByte(lv);
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(lv == (llong)0x0807060504030201LL, "inverseByte llong");

	// swapByte
	// 0x01020304 小端内存布局: byte[0]=0x04, byte[1]=0x03, byte[2]=0x02, byte[3]=0x01
	// swapByte(0, 3) 后: byte[0]=0x01, byte[3]=0x04, byte[1]/byte[2] 不变(分别是 0x03/0x02)
	int sv2 = 0x01020304;
	BinaryUtility::swapByte(sv2, 0, 3);
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::getByte(sv2, 0) == 0x01, "swapByte: [0] should be 0x01");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::getByte(sv2, 3) == 0x04, "swapByte: [3] should be 0x04");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::getByte(sv2, 1) == 0x03, "swapByte: [1] unchanged");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtility::getByte(sv2, 2) == 0x02, "swapByte: [2] unchanged");
}

void BinaryUtilityTest::testBUE2_ZeroArray()
{
	using namespace BinaryUtilityExt2TestSupport;
	int arr[10];
	for (int i = 0; i < 10; i++)
	{
		arr[i] = i + 1;
	}
	BinaryUtility::zeroArray(arr, 10);
	for (int i = 0; i < 10; i++)
	{
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(arr[i] == 0, "zeroArray int[i] = 0");
	}

	float farr[5] = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f };
	BinaryUtility::zeroArray(farr, 5);
	for (int i = 0; i < 5; i++)
	{
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtilityExt2TestSupport_BUE2_FLOAT_EQ(farr[i], 0.0f), "zeroArray float[i] = 0");
	}

	llong larr[4] = { 100LL, 200LL, 300LL, 400LL };
	BinaryUtility::zeroArray(larr, 4);
	for (int i = 0; i < 4; i++)
	{
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(larr[i] == 0LL, "zeroArray llong[i] = 0");
	}
}

void BinaryUtilityTest::testBUE2_FillZeroToByteEnd()
{
	using namespace BinaryUtilityExt2TestSupport;
	char buf[16];
	memset(buf, (char)0xFF, sizeof(buf));

	// bitIdx = 5 → 当前字节的[5..7]应被清零
	int bitIdx = 5;
	BinaryUtility::fillZeroToByteEnd(buf, bitIdx);
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(bitIdx == 8, "fillZeroToByteEnd bitIdx advances to 8");
	// 验证buf[0]的高3位已清零
	byte firstByte = (byte)buf[0];
	BinaryUtilityExt2TestSupport_BUE2_ASSERT((firstByte & 0b11100000) == 0, "fillZeroToByteEnd clears high bits");
	// 低5位不变（仍为0xFF的低5位 = 0b11111）
	BinaryUtilityExt2TestSupport_BUE2_ASSERT((firstByte & 0b00011111) == 0b00011111, "fillZeroToByteEnd keeps low bits");

	// bitIdx已对齐时不做任何事
	memset(buf, (char)0xFF, sizeof(buf));
	bitIdx = 8;
	BinaryUtility::fillZeroToByteEnd(buf, bitIdx);
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(bitIdx == 8, "fillZeroToByteEnd aligned bitIdx unchanged");

	// bitIdx = 0时
	memset(buf, (char)0xFF, sizeof(buf));
	bitIdx = 0;
	BinaryUtility::fillZeroToByteEnd(buf, bitIdx);
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(bitIdx == 0, "fillZeroToByteEnd bitIdx=0 unchanged");
}

void BinaryUtilityTest::testBUE2_MixedWriteRead()
{
	using namespace BinaryUtilityExt2TestSupport;
	char buf[256];
	memset(buf, 0, sizeof(buf));
	int idx = 0;

	// 写入各种类型
	BinaryUtility::write(buf, 256, idx, (int)1000);
	BinaryUtility::write(buf, 256, idx, (short)-500);
	BinaryUtility::write(buf, 256, idx, (float)3.14f);
	BinaryUtility::write(buf, 256, idx, (double)2.718281828);
	BinaryUtility::write(buf, 256, idx, (byte)0xAB);
	BinaryUtility::write(buf, 256, idx, (llong)-9876543210LL);
	BinaryUtility::write(buf, 256, idx, (bool)true);
	BinaryUtility::write(buf, 256, idx, (uint)3000000000u);
	BinaryUtility::write(buf, 256, idx, (ushort)50000u);

	int expectedSize = 4 + 2 + 4 + 8 + 1 + 8 + 1 + 4 + 2;
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(idx == expectedSize, "mixed write total size");

	// 全部读回
	idx = 0;
	int ri = 0;
	short rs = 0;
	float rf = 0.0f;
	double rd = 0.0;
	byte rb = 0;
	llong rl = 0LL;
	bool rbool = false;
	uint rui = 0;
	ushort rus = 0;

	BinaryUtility::read(buf, 256, idx, ri);
	BinaryUtility::read(buf, 256, idx, rs);
	BinaryUtility::read(buf, 256, idx, rf);
	BinaryUtility::read(buf, 256, idx, rd);
	BinaryUtility::read(buf, 256, idx, rb);
	BinaryUtility::read(buf, 256, idx, rl);
	BinaryUtility::read(buf, 256, idx, rbool);
	BinaryUtility::read(buf, 256, idx, rui);
	BinaryUtility::read(buf, 256, idx, rus);

	BinaryUtilityExt2TestSupport_BUE2_ASSERT(ri == 1000, "mixed: int");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(rs == -500, "mixed: short");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtilityExt2TestSupport_BUE2_FLOAT_EQ(rf, 3.14f), "mixed: float");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(BinaryUtilityExt2TestSupport_BUE2_DOUBLE_EQ(rd, 2.718281828), "mixed: double");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(rb == 0xAB, "mixed: byte");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(rl == -9876543210LL, "mixed: llong");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(rbool == true, "mixed: bool");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(rui == 3000000000u, "mixed: uint");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(rus == 50000u, "mixed: ushort");
}

void BinaryUtilityTest::testBUE2_MixedBitReadWrite()
{
	using namespace BinaryUtilityExt2TestSupport;
	char buf[256];
	memset(buf, 0, sizeof(buf));
	int bitIdx = 0;

	// 写入多种bit类型
	BinaryUtility::writeBoolBit(buf, 256, bitIdx, true);
	BinaryUtility::writeSignedIntegerBit(buf, 256, bitIdx, (int)12345, true);
	BinaryUtility::writeBoolBit(buf, 256, bitIdx, false);
	BinaryUtility::writeUnsignedIntegerBit(buf, 256, bitIdx, (ushort)9999);
	BinaryUtility::writeSignedIntegerBit(buf, 256, bitIdx, (char)-50, true);
	BinaryUtility::writeBoolBit(buf, 256, bitIdx, true);
	BinaryUtility::writeUnsignedIntegerBit(buf, 256, bitIdx, (byte)200);

	// 读回
	bool rb1 = false;
	int ri = 0;
	bool rb2 = true;
	ushort rus = 0;
	char rc = 0;
	bool rb3 = false;
	byte rby = 0;

	bitIdx = 0;
	BinaryUtility::readBoolBit(buf, 256, bitIdx, rb1);
	BinaryUtility::readSignedIntegerBit(buf, 256, bitIdx, ri, true);
	BinaryUtility::readBoolBit(buf, 256, bitIdx, rb2);
	BinaryUtility::readUnsignedIntegerBit(buf, 256, bitIdx, rus);
	BinaryUtility::readSignedIntegerBit(buf, 256, bitIdx, rc, true);
	BinaryUtility::readBoolBit(buf, 256, bitIdx, rb3);
	BinaryUtility::readUnsignedIntegerBit(buf, 256, bitIdx, rby);

	BinaryUtilityExt2TestSupport_BUE2_ASSERT(rb1 == true, "mixed bit: bool1");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(ri == 12345, "mixed bit: int");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(rb2 == false, "mixed bit: bool2");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(rus == 9999, "mixed bit: ushort");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(rc == -50, "mixed bit: char");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(rb3 == true, "mixed bit: bool3");
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(rby == 200, "mixed bit: byte");
}

void BinaryUtilityTest::testBUE2_OffsetReadWrite()
{
	using namespace BinaryUtilityExt2TestSupport;
	char buf[128];
	memset(buf, 0, sizeof(buf));
	int idx = 0;

	// 在偏移0写入int
	BinaryUtility::write(buf, 128, idx, (int)111);
	// 在偏移4写入float
	BinaryUtility::write(buf, 128, idx, (float)2.2f);
	// 在偏移8写入short
	BinaryUtility::write(buf, 128, idx, (short)-99);
	// 在偏移10写入llong
	BinaryUtility::write(buf, 128, idx, (llong)999999999LL);

	// 非顺序读取（跳过float直接读short）
	idx = 8;
	short rs = 0;
	BinaryUtility::read(buf, 128, idx, rs);
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(rs == -99, "offset read short at 8");

	// 读取第一个int
	idx = 0;
	int ri = 0;
	BinaryUtility::read(buf, 128, idx, ri);
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(ri == 111, "offset read int at 0");

	// 读取llong
	idx = 10;
	llong rl = 0;
	BinaryUtility::read(buf, 128, idx, rl);
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(rl == 999999999LL, "offset read llong at 10");
}

void BinaryUtilityTest::testBUE2_StressReadWrite()
{
	using namespace BinaryUtilityExt2TestSupport;
	const int count = 100;
	char buf[count * 4 + 16];
	memset(buf, 0, sizeof(buf));
	int idx = 0;

	// 写入100个int
	for (int i = 0; i < count; i++)
	{
		BinaryUtility::write(buf, (int)sizeof(buf), idx, (int)(i * 13 - 500));
	}
	BinaryUtilityExt2TestSupport_BUE2_ASSERT(idx == count * 4, "stress write 100 ints total bytes");

	// 读回
	idx = 0;
	for (int i = 0; i < count; i++)
	{
		int rv = 0;
		BinaryUtility::read(buf, (int)sizeof(buf), idx, rv);
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(rv == i * 13 - 500, "stress read int value");
	}
}

void BinaryUtilityTest::testBUE2_StressBitReadWrite()
{
	using namespace BinaryUtilityExt2TestSupport;
	// 压力测试：写入50个int的bit编码，再全部读回
	const int count = 50;
	char buf[512];
	memset(buf, 0, sizeof(buf));
	int bitIdx = 0;

	for (int i = 0; i < count; i++)
	{
		int val = (i % 2 == 0) ? i * 100 : -(i * 100);
		BinaryUtility::writeSignedIntegerBit(buf, 512, bitIdx, val, true);
	}

	bitIdx = 0;
	for (int i = 0; i < count; i++)
	{
		int rv = 0;
		BinaryUtility::readSignedIntegerBit(buf, 512, bitIdx, rv, true);
		int expected = (i % 2 == 0) ? i * 100 : -(i * 100);
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(rv == expected, "stress bit read int value");
	}
}

void BinaryUtilityTest::testBUE2_StressUnsignedBit()
{
	using namespace BinaryUtilityExt2TestSupport;
	// 写入30个ushort的bit编码
	const int count = 30;
	char buf[256];
	memset(buf, 0, sizeof(buf));
	int bitIdx = 0;

	for (int i = 0; i < count; i++)
	{
		BinaryUtility::writeUnsignedIntegerBit(buf, 256, bitIdx, (ushort)(i * 300));
	}

	bitIdx = 0;
	for (int i = 0; i < count; i++)
	{
		ushort rv = 0;
		BinaryUtility::readUnsignedIntegerBit(buf, 256, bitIdx, rv);
		BinaryUtilityExt2TestSupport_BUE2_ASSERT(rv == (ushort)(i * 300), "stress unsigned bit ushort value");
	}
}

namespace BinaryUtilityExt3TestSupport
{
#define BinaryUtilityExt3TestSupport_BUE3_ASSERT(condition) if (!(condition)) { ERROR("BinaryUtilityExt3Test failed: " #condition); }
}

void BinaryUtilityTest::testGetByteBasic()
{
	using namespace BinaryUtilityExt3TestSupport;
	int v = 0x04030201;
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::getByte(v, 0) == 0x01);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::getByte(v, 1) == 0x02);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::getByte(v, 2) == 0x03);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::getByte(v, 3) == 0x04);
}

void BinaryUtilityTest::testGetByteAllPositions()
{
	using namespace BinaryUtilityExt3TestSupport;
	llong v = 0x0807060504030201LL;
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::getByte(v, 0) == 0x01);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::getByte(v, 7) == 0x08);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::getByte(v, 3) == 0x04);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::getByte(v, 4) == 0x05);
}

void BinaryUtilityTest::testSetByteBasic()
{
	using namespace BinaryUtilityExt3TestSupport;
	int v = 0;
	BinaryUtility::setByte(v, (byte)0xAB, 0);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::getByte(v, 0) == 0xAB);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::getByte(v, 1) == 0x00);
}

void BinaryUtilityTest::testSetByteAllPositions()
{
	using namespace BinaryUtilityExt3TestSupport;
	int v = 0;
	BinaryUtility::setByte(v, (byte)0x11, 0);
	BinaryUtility::setByte(v, (byte)0x22, 1);
	BinaryUtility::setByte(v, (byte)0x33, 2);
	BinaryUtility::setByte(v, (byte)0x44, 3);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::getByte(v, 0) == 0x11);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::getByte(v, 1) == 0x22);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::getByte(v, 2) == 0x33);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::getByte(v, 3) == 0x44);
}

void BinaryUtilityTest::testHasBitTrue()
{
	using namespace BinaryUtilityExt3TestSupport;
	int v = 0b1010;
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::hasBit(v, 1));
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::hasBit(v, 3));
}

void BinaryUtilityTest::testHasBitFalse()
{
	using namespace BinaryUtilityExt3TestSupport;
	int v = 0b1010;
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(!BinaryUtility::hasBit(v, 0));
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(!BinaryUtility::hasBit(v, 2));
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(!BinaryUtility::hasBit(v, 4));
}

void BinaryUtilityTest::testGetBitInt()
{
	using namespace BinaryUtilityExt3TestSupport;
	int v = 0b1010;
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::getBit(v, 0) == 0);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::getBit(v, 1) == 1);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::getBit(v, 2) == 0);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::getBit(v, 3) == 1);

	int v2 = 0;
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::getBit(v2, 0) == 0);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::getBit(v2, 31) == 0);

	int v3 = ~0;
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::getBit(v3, 0) == 1);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::getBit(v3, 31) == 1);
}

void BinaryUtilityTest::testGetBitByte()
{
	using namespace BinaryUtilityExt3TestSupport;
	byte v = 0b11001010;
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::getBit(v, 0) == 0);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::getBit(v, 1) == 1);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::getBit(v, 6) == 1);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::getBit(v, 7) == 1);
}

void BinaryUtilityTest::testSetBitOne()
{
	using namespace BinaryUtilityExt3TestSupport;
	int v = 0;
	BinaryUtility::setBit(v, 0, 1);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::getBit(v, 0) == 1);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(v == 1);

	BinaryUtility::setBit(v, 3, 1);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::getBit(v, 3) == 1);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(v == 9);
}

void BinaryUtilityTest::testSetBitZero()
{
	using namespace BinaryUtilityExt3TestSupport;
	int v = 0b1111;
	BinaryUtility::setBit(v, 1, 0);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::getBit(v, 1) == 0);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(v == 0b1101);

	BinaryUtility::setBit(v, 0, 0);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::getBit(v, 0) == 0);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(v == 0b1100);
}

void BinaryUtilityTest::testSetBitOneFunc()
{
	using namespace BinaryUtilityExt3TestSupport;
	int v = 0;
	BinaryUtility::setBitOne(v, 5);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::hasBit(v, 5));
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(!BinaryUtility::hasBit(v, 4));
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(!BinaryUtility::hasBit(v, 6));

	BinaryUtility::setBitOne(v, 0);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::hasBit(v, 0));
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::hasBit(v, 5));
}

void BinaryUtilityTest::testSetBitZeroFunc()
{
	using namespace BinaryUtilityExt3TestSupport;
	int v = ~0;
	BinaryUtility::setBitZero(v, 4);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(!BinaryUtility::hasBit(v, 4));
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::hasBit(v, 3));
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::hasBit(v, 5));
}

void BinaryUtilityTest::testGetHighestBitInt()
{
	using namespace BinaryUtilityExt3TestSupport;
	int positive = 100;
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::getHighestBit(positive) == 0);

	int negative = -1;
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::getHighestBit(negative) == 1);

	int zero = 0;
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::getHighestBit(zero) == 0);
}

void BinaryUtilityTest::testGetHighestBitByte()
{
	using namespace BinaryUtilityExt3TestSupport;
	byte low = 0x7F;
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::getHighestBit(low) == 0);

	byte high = 0x80;
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::getHighestBit(high) == 1);

	byte full = 0xFF;
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::getHighestBit(full) == 1);
}

void BinaryUtilityTest::testSetHighestBit()
{
	using namespace BinaryUtilityExt3TestSupport;
	int v = 0;
	BinaryUtility::setHighestBit(v, true);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::getHighestBit(v) == 1);

	BinaryUtility::setHighestBit(v, false);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::getHighestBit(v) == 0);

	byte b = 0;
	BinaryUtility::setHighestBit(b, true);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::getHighestBit(b) == 1);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(b == 0x80);
}

void BinaryUtilityTest::testGetLowestBit()
{
	using namespace BinaryUtilityExt3TestSupport;
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::getLowestBit(0) == 0);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::getLowestBit(1) == 1);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::getLowestBit(2) == 0);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::getLowestBit(3) == 1);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::getLowestBit(0xFF) == 1);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::getLowestBit(0xFE) == 0);
}

void BinaryUtilityTest::testSetLowestBit()
{
	using namespace BinaryUtilityExt3TestSupport;
	int v = 0;
	BinaryUtility::setLowestBit(v, true);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::getLowestBit(v) == 1);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(v == 1);

	BinaryUtility::setLowestBit(v, false);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::getLowestBit(v) == 0);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(v == 0);

	int v2 = 0xFE;
	BinaryUtility::setLowestBit(v2, true);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(v2 == 0xFF);
}

void BinaryUtilityTest::testGetBits()
{
	using namespace BinaryUtilityExt3TestSupport;
	byte v = 0b11011010;
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::getBits(v, 0, 3) == 0b010);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::getBits(v, 3, 3) == 0b011);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::getBits(v, 0, 1) == 0);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::getBits(v, 1, 1) == 1);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::getBits(v, 7, 1) == 1);
}

void BinaryUtilityTest::testBitCountToByteCount()
{
	using namespace BinaryUtilityExt3TestSupport;
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::bitCountToByteCount(0) == 0);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::bitCountToByteCount(1) == 1);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::bitCountToByteCount(7) == 1);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::bitCountToByteCount(8) == 1);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::bitCountToByteCount(9) == 2);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::bitCountToByteCount(16) == 2);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::bitCountToByteCount(17) == 3);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::bitCountToByteCount(24) == 3);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::bitCountToByteCount(32) == 4);
}

void BinaryUtilityTest::testGetListMaxUnsigned()
{
	using namespace BinaryUtilityExt3TestSupport;
	Vector<uint> vec;
	vec.add(5u);
	vec.add(3u);
	vec.add(8u);
	vec.add(1u);
	uint maxVal = BinaryUtility::getListMax(vec);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(maxVal == 8u);

	Vector<uint> empty;
	uint emptyMax = BinaryUtility::getListMax(empty);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(emptyMax == 0u);
}

void BinaryUtilityTest::testGetListMaxAbs()
{
	using namespace BinaryUtilityExt3TestSupport;
	Vector<int> vec;
	vec.add(5);
	vec.add(-10);
	vec.add(3);
	vec.add(-2);
	int maxAbs = BinaryUtility::getListMaxAbs(vec);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(maxAbs == 10);

	Vector<int> empty;
	int emptyMaxAbs = BinaryUtility::getListMaxAbs(empty);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(emptyMaxAbs == 0);
}

void BinaryUtilityTest::testGetListMax_ptr()
{
	using namespace BinaryUtilityExt3TestSupport;
	uint arr[] = { 7u, 2u, 9u, 4u };
	uint maxVal = BinaryUtility::getListMax(arr, 4);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(maxVal == 9u);

	int arr2[] = { -3, -7, -1, -5 };
	int maxAbs = BinaryUtility::getListMaxAbs(arr2, 4);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(maxAbs == 7);
}

void BinaryUtilityTest::testGenerateBitCountExt3()
{
	using namespace BinaryUtilityExt3TestSupport;
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::generateBitCount((byte)0) == 0);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::generateBitCount((byte)1) == 1);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::generateBitCount((byte)2) == 2);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::generateBitCount((byte)3) == 2);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::generateBitCount((byte)4) == 3);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::generateBitCount((byte)7) == 3);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::generateBitCount((byte)8) == 4);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::generateBitCount((byte)127) == 7);
	BinaryUtilityExt3TestSupport_BUE3_ASSERT(BinaryUtility::generateBitCount((byte)255) == 8);
}

namespace BinaryUtilitySerializeTestSupport
{
	static constexpr int CORRECTNESS_RANDOM_COUNT = 20000;
	static constexpr int BENCHMARK_VALUE_COUNT = 200000;
	static constexpr int BENCHMARK_SAMPLE_COUNT = 9;
	static constexpr int BENCHMARK_WARMUP_COUNT = 2;
	static constexpr int BENCHMARK_MESSAGE_COUNT = 100000;
	static uint nextRandom(uint& state)
	{
		state ^= state << 13;
		state ^= state >> 17;
		state ^= state << 5;
		return state;
	}
	template<typename Prepare, typename Test>
	static llong measureMedianNS(Prepare&& prepare, Test&& test)
	{
		llong samples[BENCHMARK_SAMPLE_COUNT]{};
		const int totalCount = BENCHMARK_WARMUP_COUNT + BENCHMARK_SAMPLE_COUNT;
		for (int i = 0; i < totalCount; ++i)
		{
			prepare();
			const auto start = steady_clock::now();
			test();
			const auto end = steady_clock::now();
			if (i >= BENCHMARK_WARMUP_COUNT)
			{
				samples[i - BENCHMARK_WARMUP_COUNT] = duration_cast<nanoseconds>(end - start).count();
			}
		}
		for (int i = 1; i < BENCHMARK_SAMPLE_COUNT; ++i)
		{
			const llong value = samples[i];
			int j = i - 1;
			while (j >= 0 && samples[j] > value)
			{
				samples[j + 1] = samples[j];
				--j;
			}
			samples[j + 1] = value;
		}
		return samples[BENCHMARK_SAMPLE_COUNT >> 1];
	}
	static string formatMS(const llong ns) { return FToS((float)ns / 1000000.0f, 3); }
	static string formatNSPerValue(const llong ns) { return FToS((float)ns / BENCHMARK_VALUE_COUNT, 3); }
	static string formatRatio(const llong value0, const llong value1) { return value1 > 0 ? FToS((float)value0 / (float)value1, 3) : "0"; }
	static bool readUIntCacheV28Reference(const char* buffer, const int bufferSize, BinaryUtility::IntegerBitReadCache& cache, uint& value)
	{
		constexpr byte TYPE_LENGTH_MAX_BIT = BinaryUtility::UNSIGNED_LENGTH_MAX_BIT[sizeof(uint)];
		constexpr byte LENGTH_MAX_VALUE = (1 << TYPE_LENGTH_MAX_BIT) - 1;
		constexpr byte REMOVE_HIGH_BIT_LIMIT = (1 << sizeof(uint)) - 1;
		if (cache.mRemainBitCount < TYPE_LENGTH_MAX_BIT && !BinaryUtility::refillIntegerBitReadCache(buffer, bufferSize, cache))
		{
			return false;
		}
		ullong window = cache.mWindow;
		byte bitCount = (byte)(window & LENGTH_MAX_VALUE);
		if (bitCount == LENGTH_MAX_VALUE)
		{
			++bitCount;
		}
		const bool removeHighBit = bitCount != 0 && bitCount < REMOVE_HIGH_BIT_LIMIT;
		const byte dataBitCount = bitCount == 0 ? 0 : removeHighBit ? bitCount - 1 : bitCount;
		const byte totalBitCount = TYPE_LENGTH_MAX_BIT + dataBitCount;
		if (cache.mRemainBitCount < totalBitCount)
		{
			if (!BinaryUtility::refillIntegerBitReadCache(buffer, bufferSize, cache) || cache.mRemainBitCount < totalBitCount)
			{
				return false;
			}
			window = cache.mWindow;
		}
		ullong unsignedValue = dataBitCount > 0 ? (window >> TYPE_LENGTH_MAX_BIT) & ((1ULL << dataBitCount) - 1ULL) : 0;
		if (removeHighBit)
		{
			unsignedValue |= 1ULL << dataBitCount;
		}
		value = (uint)unsignedValue;
		BinaryUtility::consumeIntegerBitReadCache(cache, totalBitCount);
		return true;
	}
	static bool readUIntList6V28Reference(const char* buffer, const int bufferSize, int& bitIndex, Array<6, uint*>& list)
	{
		bool useUnityBitCount = false;
		byte bitCount = 0;
		if (!BinaryUtility::readUnsignedIntegerListHeaderBit<uint>(buffer, bufferSize, bitIndex, useUnityBitCount, bitCount) || useUnityBitCount)
		{
			return false;
		}
		BinaryUtility::IntegerBitReadCache cache;
		cache.mBitIndex = bitIndex;
		FOR(6)
		{
			if (!readUIntCacheV28Reference(buffer, bufferSize, cache, *list[i]))
			{
				bitIndex = cache.mBitIndex;
				return false;
			}
		}
		bitIndex = cache.mBitIndex;
		return true;
	}
	static bool readUnsignedIntVectorV35Reference(const char* buffer, const int bufferSize, int& bitIndex, Vector<uint>& list)
	{
		ushort count = 0;
		if (!BinaryUtility::readUnsignedIntegerBit(buffer, bufferSize, bitIndex, count))
		{
			return false;
		}
		if (count == 0)
		{
			return true;
		}
		list.clear();
		list.resize(count);
		bool useUnityBitCount = false;
		byte bitCount = 0;
		if (!BinaryUtility::readUnsignedIntegerListHeaderBit<uint>(buffer, bufferSize, bitIndex, useUnityBitCount, bitCount))
		{
			return false;
		}
		if (useUnityBitCount)
		{
			if (bitCount == 0)
			{
				return true;
			}
			if (BinaryUtility::bitCountToByteCount(bitIndex + bitCount * count) > bufferSize)
			{
				return false;
			}
			FOR(count)
			{
				list[i] = (uint)BinaryUtility::readIntegerBits(buffer, bufferSize, bitIndex, bitCount);
			}
		}
		else
		{
			BinaryUtility::IntegerBitReadCache cache;
			cache.mBitIndex = bitIndex;
			FOR(count)
			{
				if (!BinaryUtility::readUnsignedIntegerCacheBit(buffer, bufferSize, cache, list[i]))
				{
					bitIndex = cache.mBitIndex;
					return false;
				}
			}
			bitIndex = cache.mBitIndex;
		}
		return true;
	}
	static bool readUnsignedIntVectorV36Reference(const char* buffer, const int bufferSize, int& bitIndex, Vector<uint>& list)
	{
		ushort count = 0;
		if (!BinaryUtility::readUnsignedIntegerBit(buffer, bufferSize, bitIndex, count))
		{
			return false;
		}
		if (count == 0)
		{
			return true;
		}
		list.clear();
		list.resize(count);
		uint* output = list.data();
		bool useUnityBitCount = false;
		byte bitCount = 0;
		if (!BinaryUtility::readUnsignedIntegerListHeaderBit<uint>(buffer, bufferSize, bitIndex, useUnityBitCount, bitCount))
		{
			return false;
		}
		if (useUnityBitCount)
		{
			if (bitCount == 0)
			{
				return true;
			}
			if (BinaryUtility::bitCountToByteCount(bitIndex + bitCount * count) > bufferSize)
			{
				return false;
			}
			FOR(count)
			{
				output[i] = (uint)BinaryUtility::readIntegerBits(buffer, bufferSize, bitIndex, bitCount);
			}
		}
		else
		{
			BinaryUtility::IntegerBitReadCache cache;
			cache.mBitIndex = bitIndex;
			FOR(count)
			{
				if (!BinaryUtility::readUnsignedIntegerCacheBit(buffer, bufferSize, cache, output[i]))
				{
					bitIndex = cache.mBitIndex;
					return false;
				}
			}
			bitIndex = cache.mBitIndex;
		}
		return true;
	}

	static bool writeUnsignedIntFormulaReference(char* buffer, const int bufferSize, int& bitIndex, const uint value)
	{
		const byte bitCount = BinaryUtility::generateBitCount(value);
		if ((value != 0 && bitCount == 0) || bitCount > sizeof(value) * 8)
		{
			return false;
		}
		constexpr byte TYPE_LENGTH_MAX_BIT = BinaryUtility::UNSIGNED_LENGTH_MAX_BIT[sizeof(uint)];
		constexpr byte LENGTH_MAX_VALUE = (1 << TYPE_LENGTH_MAX_BIT) - 1;
		constexpr byte REMOVE_HIGH_BIT_LIMIT = (1 << sizeof(uint)) - 1;
		byte writeLength = bitCount;
		byte dataLength = bitCount;
		if (bitCount == 1 << TYPE_LENGTH_MAX_BIT)
		{
			writeLength = bitCount - 1;
		}
		else if (bitCount == LENGTH_MAX_VALUE)
		{
			++dataLength;
		}
		if (value == 0)
		{
			if (BinaryUtility::bitCountToByteCount(bitIndex + TYPE_LENGTH_MAX_BIT) > bufferSize)
			{
				return false;
			}
			BinaryUtility::writeIntegerBits(buffer, bufferSize, bitIndex, writeLength, TYPE_LENGTH_MAX_BIT);
			return true;
		}
		const bool removeHighBit = dataLength < REMOVE_HIGH_BIT_LIMIT;
		const byte dataBitCount = removeHighBit ? dataLength - 1 : dataLength;
		const byte totalBitCount = TYPE_LENGTH_MAX_BIT + dataBitCount;
		if (BinaryUtility::bitCountToByteCount(bitIndex + totalBitCount) > bufferSize)
		{
			return false;
		}
		const ullong dataValue = dataBitCount > 0 ? (ullong)value & ((1ULL << dataBitCount) - 1ULL) : 0;
		const ullong packedValue = (ullong)writeLength | (dataValue << TYPE_LENGTH_MAX_BIT);
		BinaryUtility::writeIntegerBits(buffer, bufferSize, bitIndex, packedValue, totalBitCount);
		return true;
	}
	static bool writeFloatListBitV23Reference(char* buffer, const int bufferSize, int& bitIndex, const float* list, const int count, const bool needWriteSign, const int precision = 3)
	{
		if (count == 0)
		{
			return true;
		}
		const int powValue = pow10(precision);
		constexpr int CACHE_COUNT = 64;
		int valueCache[CACHE_COUNT];
		const bool useValueCache = count <= CACHE_COUNT;
		byte maxBitCount = 0;
		constexpr byte TYPE_LENGTH_MAX_BIT = BinaryUtility::SIGNED_LENGTH_MAX_BIT[sizeof(int)];
		int bitCountSingle = TYPE_LENGTH_MAX_BIT * count;
		int bitCountUnity = 0;
		if (useValueCache)
		{
			FOR(count)
			{
				const int intValue = round_(list[i] * powValue);
				valueCache[i] = intValue;
				const uint rawValue = (uint)intValue;
				const uint absValue = intValue < 0 ? 0u - rawValue : rawValue;
				const byte thisBitCount = generateBitCount(absValue);
				if (thisBitCount > maxBitCount)
				{
					maxBitCount = thisBitCount;
				}
				if (absValue > 0)
				{
					bitCountSingle += thisBitCount;
				}
			}
			bitCountUnity = maxBitCount > 0 ? (maxBitCount + 1) * count + TYPE_LENGTH_MAX_BIT : TYPE_LENGTH_MAX_BIT;
		}
		else
		{
			maxBitCount = generateBitCount((uint)round_(BinaryUtility::getFloatListMaxAbs(list, count) * powValue));
			bitCountUnity = maxBitCount > 0 ? (maxBitCount + 1) * count + TYPE_LENGTH_MAX_BIT : TYPE_LENGTH_MAX_BIT;
			if (bitCountSingle < bitCountUnity)
			{
				FOR(count)
				{
					const int thisAbsValue = round_(abs(list[i]) * powValue);
					if (thisAbsValue > 0)
					{
						bitCountSingle += generateBitCount((uint)thisAbsValue);
						if (bitCountSingle > bitCountUnity)
						{
							break;
						}
					}
				}
			}
		}
		if (bitCountSingle >= bitCountUnity)
		{
			if (!BinaryUtility::writeSignedIntegerListHeaderBit<int>(buffer, bufferSize, bitIndex, maxBitCount))
			{
				return false;
			}
			if (maxBitCount == 0)
			{
				return true;
			}
			if (BinaryUtility::bitCountToByteCount(bitIndex + (1 + maxBitCount) * count) > bufferSize)
			{
				return false;
			}
			const byte signBitCount = needWriteSign ? 1 : 0;
			const byte elementBitCount = maxBitCount + signBitCount;
			FOR(count)
			{
				const int intValue = useValueCache ? valueCache[i] : round_(list[i] * powValue);
				const bool isNegative = intValue < 0;
				const uint rawValue = (uint)intValue;
				const uint unsignedValue = isNegative ? 0u - rawValue : rawValue;
				const ullong packedValue = ((ullong)unsignedValue << signBitCount) | (needWriteSign && isNegative ? 1ULL : 0ULL);
				BinaryUtility::writeIntegerBits(buffer, bufferSize, bitIndex, packedValue, elementBitCount);
			}
		}
		else
		{
			++bitIndex;
			FOR(count)
			{
				const int intValue = useValueCache ? valueCache[i] : round_(list[i] * powValue);
				if (!BinaryUtility::writeSignedIntegerBit(buffer, bufferSize, bitIndex, intValue, needWriteSign))
				{
					return false;
				}
			}
		}
		return true;
	}


	static string formatNSPerCount(const llong ns, const int count) { return count > 0 ? FToS((float)ns / (float)count, 3) : "0"; }
}

void BinaryUtilityTest::assertResult(const bool result, const string& info)
{
	using namespace BinaryUtilitySerializeTestSupport;
	if (!result)
	{
		ERROR("BinaryUtilitySerializeTest FAILED: " + info);
	}
}

void BinaryUtilityTest::testSignedCorrectness()
{
	using namespace BinaryUtilitySerializeTestSupport;
	Vector<int> values;
	values.reserve(CORRECTNESS_RANDOM_COUNT + 32);
	const int boundary[] = { 0, 1, -1, 2, -2, 3, -3, 7, -7, 8, -8, 15, -15, 16, -16, 127, -127, 128, -128, 255, -255, 256, -256, 32767, -32767, 32768, -32768, 65535, -65535, INT_MAX, INT_MIN + 1 };
	for (int value : boundary)
	{
		values.add(value);
	}
	uint randomState = 0x13572468u;
	FOR(CORRECTNESS_RANDOM_COUNT)
	{
		const uint randomValue = nextRandom(randomState);
		int value = (int)(randomValue & 0x7FFFFFFFu);
		if ((randomValue & 0x80000000u) != 0)
		{
			value = -value;
		}
		values.add(value);
	}
	const int count = values.size();
	vector<char> byteBuffer((size_t)count * sizeof(int));
	vector<char> bitBuffer((size_t)count * (sizeof(int) + 1) + 16, 0);
	int byteWriteIndex = 0;
	int bitWriteIndex = 0;
	FOR(count)
	{
		assertResult(BinaryUtility::write(byteBuffer.data(), (int)byteBuffer.size(), byteWriteIndex, values[i]), "signed byte write");
		assertResult(BinaryUtility::writeSignedIntegerBit(bitBuffer.data(), (int)bitBuffer.size(), bitWriteIndex, values[i], true), "signed bit write");
	}
	int byteReadIndex = 0;
	int bitReadIndex = 0;
	FOR(count)
	{
		int byteValue = 0;
		int bitValue = 0;
		assertResult(BinaryUtility::read(byteBuffer.data(), byteWriteIndex, byteReadIndex, byteValue), "signed byte read");
		assertResult(BinaryUtility::readSignedIntegerBit(bitBuffer.data(), BinaryUtility::bitCountToByteCount(bitWriteIndex), bitReadIndex, bitValue, true), "signed bit read");
		assertResult(byteValue == values[i], "signed byte round-trip index:" + IToS(i));
		assertResult(bitValue == values[i], "signed bit round-trip index:" + IToS(i));
		assertResult(byteValue == bitValue, "signed byte/bit mismatch index:" + IToS(i));
	}
	assertResult(byteReadIndex == byteWriteIndex, "signed byte final index");
	assertResult(bitReadIndex == bitWriteIndex, "signed bit final index");
}

void BinaryUtilityTest::testUnsignedCorrectness()
{
	using namespace BinaryUtilitySerializeTestSupport;
	Vector<uint> values;
	values.reserve(CORRECTNESS_RANDOM_COUNT + 40);
	const uint boundary[] = { 0u, 1u, 2u, 3u, 7u, 8u, 15u, 16u, 31u, 32u, 63u, 64u, 127u, 128u, 255u, 256u, 511u, 512u, 1023u, 1024u, 16383u, 16384u, 32767u, 32768u, 65535u, 65536u, 0x7FFFFFFFu, 0x80000000u, UINT_MAX };
	for (uint value : boundary)
	{
		values.add(value);
	}
	uint randomState = 0x24681357u;
	FOR(CORRECTNESS_RANDOM_COUNT)
	{
		values.add(nextRandom(randomState));
	}
	const int count = values.size();
	vector<char> byteBuffer((size_t)count * sizeof(uint));
	vector<char> bitBuffer((size_t)count * (sizeof(uint) + 1) + 16, 0);
	int byteWriteIndex = 0;
	int bitWriteIndex = 0;
	FOR(count)
	{
		assertResult(BinaryUtility::write(byteBuffer.data(), (int)byteBuffer.size(), byteWriteIndex, values[i]), "unsigned byte write");
		assertResult(BinaryUtility::writeUnsignedIntegerBit(bitBuffer.data(), (int)bitBuffer.size(), bitWriteIndex, values[i]), "unsigned bit write");
	}
	int byteReadIndex = 0;
	int bitReadIndex = 0;
	FOR(count)
	{
		uint byteValue = 0;
		uint bitValue = 0;
		assertResult(BinaryUtility::read(byteBuffer.data(), byteWriteIndex, byteReadIndex, byteValue), "unsigned byte read");
		assertResult(BinaryUtility::readUnsignedIntegerBit(bitBuffer.data(), BinaryUtility::bitCountToByteCount(bitWriteIndex), bitReadIndex, bitValue), "unsigned bit read");
		assertResult(byteValue == values[i], "unsigned byte round-trip index:" + IToS(i));
		assertResult(bitValue == values[i], "unsigned bit round-trip index:" + IToS(i));
		assertResult(byteValue == bitValue, "unsigned byte/bit mismatch index:" + IToS(i));
	}
	assertResult(byteReadIndex == byteWriteIndex, "unsigned byte final index");
	assertResult(bitReadIndex == bitWriteIndex, "unsigned bit final index");
}

void BinaryUtilityTest::testUnsignedTypeBoundaries()
{
	using namespace BinaryUtilitySerializeTestSupport;
	const byte byteValues[] = { 0u, 1u, 2u, 3u, 7u, 8u, 15u, 16u, 31u, 32u, 63u, 64u, 127u, 128u, 255u };
	const ushort ushortValues[] = { 0u, 1u, 7u, 8u, 15u, 16u, 127u, 128u, 255u, 256u, 1023u, 1024u, 32767u, 32768u, 65535u };
	const ullong ullongValues[] = { 0ULL, 1ULL, 7ULL, 8ULL, 31ULL, 32ULL, 255ULL, 256ULL, 65535ULL, 65536ULL, 0x7FFFFFFFULL, 0x80000000ULL, 0xFFFFFFFFULL, 0x100000000ULL, ULLONG_MAX };
	char buffer[64]{};
	for (byte value : byteValues)
	{
		memset(buffer, 0, sizeof(buffer));
		int writeBit = 3;
		BinaryUtility::writeBoolBit(buffer, (int)sizeof(buffer), writeBit, true);
		assertResult(BinaryUtility::writeUnsignedIntegerBit(buffer, (int)sizeof(buffer), writeBit, value), "byte boundary write");
		int readBit = 3;
		bool prefix = false;
		BinaryUtility::readBoolBit(buffer, (int)sizeof(buffer), readBit, prefix);
		byte result = 0;
		assertResult(prefix && BinaryUtility::readUnsignedIntegerBit(buffer, (int)sizeof(buffer), readBit, result) && result == value && readBit == writeBit, "byte boundary round-trip value:" + IToS((int)value));
	}
	for (ushort value : ushortValues)
	{
		memset(buffer, 0, sizeof(buffer));
		int writeBit = 5;
		BinaryUtility::writeBoolBit(buffer, (int)sizeof(buffer), writeBit, true);
		assertResult(BinaryUtility::writeUnsignedIntegerBit(buffer, (int)sizeof(buffer), writeBit, value), "ushort boundary write");
		int readBit = 5;
		bool prefix = false;
		BinaryUtility::readBoolBit(buffer, (int)sizeof(buffer), readBit, prefix);
		ushort result = 0;
		assertResult(prefix && BinaryUtility::readUnsignedIntegerBit(buffer, (int)sizeof(buffer), readBit, result) && result == value && readBit == writeBit, "ushort boundary round-trip value:" + IToS((int)value));
	}
	for (ullong value : ullongValues)
	{
		memset(buffer, 0, sizeof(buffer));
		int writeBit = 7;
		BinaryUtility::writeBoolBit(buffer, (int)sizeof(buffer), writeBit, true);
		assertResult(BinaryUtility::writeUnsignedIntegerBit(buffer, (int)sizeof(buffer), writeBit, value), "ullong boundary write");
		int readBit = 7;
		bool prefix = false;
		BinaryUtility::readBoolBit(buffer, (int)sizeof(buffer), readBit, prefix);
		ullong result = 0;
		assertResult(prefix && BinaryUtility::readUnsignedIntegerBit(buffer, (int)sizeof(buffer), readBit, result) && result == value && readBit == writeBit, "ullong boundary round-trip");
	}
}

void BinaryUtilityTest::testSerializerCorrectness()
{
	using namespace BinaryUtilitySerializeTestSupport;
	const llong guid0 = 123456789LL;
	const llong guid1 = -987654321LL;
	const uint maxHP = 8000u;
	const uint hp = 7234u;
	const uint monsterID = 10023u;
	const uint actionState = 5u;
	const uint posX = 1234u;
	const uint posY = 5678u;
	const uint direction = 3u;
	const float attackSpeed = 1.25f;
	const string name = "BinaryUtilityTest";
	SerializerWrite byteWriter;
	byteWriter.initCapacity(128);
	assertResult(byteWriter.write(guid0) && byteWriter.write(guid1) && byteWriter.write(maxHP) && byteWriter.write(hp) && byteWriter.write(monsterID) && byteWriter.write(actionState) && byteWriter.write(posX) && byteWriter.write(posY) && byteWriter.write(direction) && byteWriter.write(attackSpeed) && byteWriter.writeString(name), "serializer byte write");
	SerializerBitWrite bitWriter;
	bitWriter.initCapacity(128);
	assertResult(bitWriter.writeSigned(true, guid0, guid1) && bitWriter.writeUnsigned(maxHP, hp, monsterID, actionState, posX, posY) && bitWriter.writeUnsigned(direction) && bitWriter.writeFloat(true, attackSpeed) && bitWriter.writeString(name), "serializer bit write");
	llong byteGuid0 = 0;
	llong byteGuid1 = 0;
	uint byteMaxHP = 0;
	uint byteHP = 0;
	uint byteMonsterID = 0;
	uint byteActionState = 0;
	uint bytePosX = 0;
	uint bytePosY = 0;
	uint byteDirection = 0;
	float byteAttackSpeed = 0.0f;
	string byteName;
	SerializerRead byteReader(byteWriter.getBuffer(), byteWriter.getDataSize());
	assertResult(byteReader.read(byteGuid0) && byteReader.read(byteGuid1) && byteReader.read(byteMaxHP) && byteReader.read(byteHP) && byteReader.read(byteMonsterID) && byteReader.read(byteActionState) && byteReader.read(bytePosX) && byteReader.read(bytePosY) && byteReader.read(byteDirection) && byteReader.read(byteAttackSpeed) && byteReader.readString(byteName), "serializer byte read");
	llong bitGuid0 = 0;
	llong bitGuid1 = 0;
	uint bitMaxHP = 0;
	uint bitHP = 0;
	uint bitMonsterID = 0;
	uint bitActionState = 0;
	uint bitPosX = 0;
	uint bitPosY = 0;
	uint bitDirection = 0;
	float bitAttackSpeed = 0.0f;
	string bitName;
	SerializerBitRead bitReader(bitWriter.getBuffer(), bitWriter.getByteCount());
	assertResult(bitReader.readSigned(true, bitGuid0, bitGuid1) && bitReader.readUnsigned(bitMaxHP, bitHP, bitMonsterID, bitActionState, bitPosX, bitPosY) && bitReader.readUnsigned(bitDirection) && bitReader.readFloat(true, bitAttackSpeed) && bitReader.readString(bitName), "serializer bit read");
	assertResult(byteGuid0 == guid0 && bitGuid0 == guid0 && byteGuid0 == bitGuid0, "serializer guid0");
	assertResult(byteGuid1 == guid1 && bitGuid1 == guid1 && byteGuid1 == bitGuid1, "serializer guid1");
	assertResult(byteMaxHP == maxHP && bitMaxHP == maxHP && byteMaxHP == bitMaxHP, "serializer maxHP");
	assertResult(byteHP == hp && bitHP == hp && byteHP == bitHP, "serializer hp");
	assertResult(byteMonsterID == monsterID && bitMonsterID == monsterID && byteMonsterID == bitMonsterID, "serializer monsterID");
	assertResult(byteActionState == actionState && bitActionState == actionState && byteActionState == bitActionState, "serializer actionState");
	assertResult(bytePosX == posX && bitPosX == posX && bytePosX == bitPosX, "serializer posX");
	assertResult(bytePosY == posY && bitPosY == posY && bytePosY == bitPosY, "serializer posY");
	assertResult(byteDirection == direction && bitDirection == direction && byteDirection == bitDirection, "serializer direction");
	assertResult(fabsf(byteAttackSpeed - attackSpeed) < 0.0001f && fabsf(bitAttackSpeed - attackSpeed) < 0.0011f, "serializer attackSpeed");
	assertResult(byteName == name && bitName == name && byteName == bitName, "serializer string");
}

template<typename T>
void BinaryUtilityTest::testSignedBoundaryValues(const T* values, const int count, const string& typeName)
{
	using namespace BinaryUtilitySerializeTestSupport;
	char buffer[64]{};
	for (int offset = 0; offset < 8; ++offset)
	{
		for (int i = 0; i < count; ++i)
		{
			memset(buffer, 0, sizeof(buffer));
			int writeBit = 0;
			for (int j = 0; j < offset; ++j)
			{
				assertResult(BinaryUtility::writeBoolBit(buffer, (int)sizeof(buffer), writeBit, (j & 1) == 0), typeName + " prefix write");
			}
			assertResult(BinaryUtility::writeSignedIntegerBit(buffer, (int)sizeof(buffer), writeBit, values[i], true), typeName + " write offset:" + IToS(offset));
			assertResult(BinaryUtility::writeBoolBit(buffer, (int)sizeof(buffer), writeBit, true), typeName + " suffix write");
			int readBit = 0;
			for (int j = 0; j < offset; ++j)
			{
				bool prefix = false;
				assertResult(BinaryUtility::readBoolBit(buffer, (int)sizeof(buffer), readBit, prefix) && prefix == ((j & 1) == 0), typeName + " prefix read");
			}
			T result = 0;
			bool suffix = false;
			assertResult(BinaryUtility::readSignedIntegerBit(buffer, (int)sizeof(buffer), readBit, result, true) && result == values[i], typeName + " round-trip offset:" + IToS(offset) + " index:" + IToS(i));
			assertResult(BinaryUtility::readBoolBit(buffer, (int)sizeof(buffer), readBit, suffix) && suffix && readBit == writeBit, typeName + " suffix read");
		}
	}
}

template<typename T>
void BinaryUtilityTest::testUnsignedBoundaryValues(const T* values, const int count, const string& typeName)
{
	using namespace BinaryUtilitySerializeTestSupport;
	char buffer[64]{};
	for (int offset = 0; offset < 8; ++offset)
	{
		for (int i = 0; i < count; ++i)
		{
			memset(buffer, 0, sizeof(buffer));
			int writeBit = 0;
			for (int j = 0; j < offset; ++j)
			{
				assertResult(BinaryUtility::writeBoolBit(buffer, (int)sizeof(buffer), writeBit, (j & 1) != 0), typeName + " prefix write");
			}
			assertResult(BinaryUtility::writeUnsignedIntegerBit(buffer, (int)sizeof(buffer), writeBit, values[i]), typeName + " write offset:" + IToS(offset));
			assertResult(BinaryUtility::writeBoolBit(buffer, (int)sizeof(buffer), writeBit, true), typeName + " suffix write");
			int readBit = 0;
			for (int j = 0; j < offset; ++j)
			{
				bool prefix = false;
				assertResult(BinaryUtility::readBoolBit(buffer, (int)sizeof(buffer), readBit, prefix) && prefix == ((j & 1) != 0), typeName + " prefix read");
			}
			T result = 0;
			bool suffix = false;
			assertResult(BinaryUtility::readUnsignedIntegerBit(buffer, (int)sizeof(buffer), readBit, result) && result == values[i], typeName + " round-trip offset:" + IToS(offset) + " index:" + IToS(i));
			assertResult(BinaryUtility::readBoolBit(buffer, (int)sizeof(buffer), readBit, suffix) && suffix && readBit == writeBit, typeName + " suffix read");
		}
	}
}

void BinaryUtilityTest::testExtendedIntegerCorrectness()
{
	using namespace BinaryUtilitySerializeTestSupport;
	const char charValues[] = { 0, 1, -1, 2, -2, 7, -7, 63, -63, 127, -127 };
	const short shortValues[] = { 0, 1, -1, 127, -127, 128, -128, 255, -255, 32767, -32767 };
	const int intValues[] = { 0, 1, -1, 127, -127, 128, -128, 32767, -32767, 32768, -32768, 65535, -65535, INT_MAX, -INT_MAX };
	const llong llongValues[] = { 0LL, 1LL, -1LL, 127LL, -127LL, 32767LL, -32767LL, 2147483647LL, -2147483647LL, 0x0200000000000000LL, -0x0200000000000000LL, 0x0400000000000000LL, -0x0400000000000000LL, 0x7FFFFFFFFFFFFFFFLL, -0x7FFFFFFFFFFFFFFFLL };
	const byte byteValues[] = { 0u, 1u, 2u, 7u, 8u, 15u, 16u, 31u, 32u, 63u, 64u, 127u, 128u, 255u };
	const ushort ushortValues[] = { 0u, 1u, 7u, 8u, 15u, 16u, 127u, 128u, 255u, 256u, 32767u, 32768u, 65535u };
	const uint uintValues[] = { 0u, 1u, 7u, 8u, 15u, 16u, 31u, 32u, 127u, 128u, 255u, 256u, 32767u, 32768u, 65535u, 65536u, 0x7FFFFFFFu, 0x80000000u, UINT_MAX };
	const ullong ullongValues[] = { 0ULL, 1ULL, 7ULL, 8ULL, 31ULL, 32ULL, 127ULL, 128ULL, 255ULL, 256ULL, 65535ULL, 65536ULL, 0xFFFFFFFFULL, 0x100000000ULL, 0x0400000000000000ULL, 0x0800000000000000ULL, 0x7FFFFFFFFFFFFFFFULL, 0x8000000000000000ULL, ULLONG_MAX };
	testSignedBoundaryValues(charValues, (int)(sizeof(charValues) / sizeof(charValues[0])), "char");
	testSignedBoundaryValues(shortValues, (int)(sizeof(shortValues) / sizeof(shortValues[0])), "short");
	testSignedBoundaryValues(intValues, (int)(sizeof(intValues) / sizeof(intValues[0])), "int");
	testSignedBoundaryValues(llongValues, (int)(sizeof(llongValues) / sizeof(llongValues[0])), "llong");
	testUnsignedBoundaryValues(byteValues, (int)(sizeof(byteValues) / sizeof(byteValues[0])), "byte");
	testUnsignedBoundaryValues(ushortValues, (int)(sizeof(ushortValues) / sizeof(ushortValues[0])), "ushort");
	testUnsignedBoundaryValues(uintValues, (int)(sizeof(uintValues) / sizeof(uintValues[0])), "uint");
	testUnsignedBoundaryValues(ullongValues, (int)(sizeof(ullongValues) / sizeof(ullongValues[0])), "ullong");
	// 当前signed协议长度位最大只能表示7/15/31/63bit绝对值,各signed类型最小值没有可兼容的无歧义编码,必须明确拒绝而不是静默写坏
	char minBuffer[32]{};
	int minBitIndex = 0;
	assertResult(!BinaryUtility::writeSignedIntegerBit(minBuffer, (int)sizeof(minBuffer), minBitIndex, (char)-128, true) && minBitIndex == 0, "signed char min must reject");
	assertResult(!BinaryUtility::writeSignedIntegerBit(minBuffer, (int)sizeof(minBuffer), minBitIndex, (short)SHRT_MIN, true) && minBitIndex == 0, "signed short min must reject");
	assertResult(!BinaryUtility::writeSignedIntegerBit(minBuffer, (int)sizeof(minBuffer), minBitIndex, INT_MIN, true) && minBitIndex == 0, "signed int min must reject");
	assertResult(!BinaryUtility::writeSignedIntegerBit(minBuffer, (int)sizeof(minBuffer), minBitIndex, (llong)LLONG_MIN, true) && minBitIndex == 0, "signed llong min must reject");
	const Array<2, int> intMinList{ 1, INT_MIN };
	const Array<2, llong> llongMinList{ 1LL, (llong)LLONG_MIN };
	assertResult(!BinaryUtility::writeSignedIntegerListBit(minBuffer, (int)sizeof(minBuffer), minBitIndex, intMinList, true) && minBitIndex == 0, "signed int list min must reject");
	assertResult(!BinaryUtility::writeSignedIntegerListBit(minBuffer, (int)sizeof(minBuffer), minBitIndex, llongMinList, true) && minBitIndex == 0, "signed llong list min must reject");
}

void BinaryUtilityTest::testUIntWriteCacheCorrectness()
{
	using namespace BinaryUtilitySerializeTestSupport;
	BinaryUtility::initUIntWriteCache();
	const uint values[]
	{
		0u, 1u, 2u, 3u, 7u, 8u, 15u, 16u, 31u, 32u, 63u, 64u, 127u, 128u, 255u, 256u,
		16383u, 16384u, 32767u, 32768u, 65535u, 65536u, 262143u, 262144u,
		BinaryUtility::UINT_WRITE_CACHE_COUNT_1M - 1u, BinaryUtility::UINT_WRITE_CACHE_COUNT_1M,
		BinaryUtility::UINT_WRITE_CACHE_COUNT_4M - 1u, BinaryUtility::UINT_WRITE_CACHE_COUNT_4M, UINT_MAX
	};
	for (int offset = 0; offset < 8; ++offset)
	{
		for (int i = 0; i < (int)(sizeof(values) / sizeof(values[0])); ++i)
		{
			char formulaBuffer[32]{};
			char cacheBuffer[32]{};
			char productionBuffer[32]{};
			for (int bit = 0; bit < offset; ++bit)
			{
				BinaryUtility::setBufferBitOne(formulaBuffer, bit);
				BinaryUtility::setBufferBitOne(cacheBuffer, bit);
				BinaryUtility::setBufferBitOne(productionBuffer, bit);
			}
			int formulaBitIndex = offset;
			int cacheBitIndex = offset;
			int productionBitIndex = offset;
			assertResult(writeUnsignedIntFormulaReference(formulaBuffer, (int)sizeof(formulaBuffer), formulaBitIndex, values[i]), "uint write cache formula offset:" + IToS(offset) + " index:" + IToS(i));
			assertResult(BinaryUtility::writeUnsignedIntegerBit(productionBuffer, (int)sizeof(productionBuffer), productionBitIndex, values[i]), "uint write cache production offset:" + IToS(offset) + " index:" + IToS(i));
			assertResult(formulaBitIndex == productionBitIndex, "uint write cache production bitIndex offset:" + IToS(offset) + " index:" + IToS(i));
			const int byteCount = BinaryUtility::bitCountToByteCount(formulaBitIndex);
			assertResult(memcmp(formulaBuffer, productionBuffer, byteCount) == 0, "uint write cache production buffer offset:" + IToS(offset) + " index:" + IToS(i));
			if (values[i] < BinaryUtility::UINT_WRITE_CACHE_COUNT_4M)
			{
				const uint encodedCacheValue = BinaryUtility::encodeUIntWriteCacheValue(values[i]);
				assertResult(BinaryUtility::writeEncodedUIntCacheBit(cacheBuffer, (int)sizeof(cacheBuffer), cacheBitIndex, encodedCacheValue), "uint write cache direct offset:" + IToS(offset) + " index:" + IToS(i));
				assertResult(formulaBitIndex == cacheBitIndex, "uint write cache direct bitIndex offset:" + IToS(offset) + " index:" + IToS(i));
				assertResult(memcmp(formulaBuffer, cacheBuffer, byteCount) == 0, "uint write cache direct buffer offset:" + IToS(offset) + " index:" + IToS(i));
				const byte cacheBitCount = (byte)(encodedCacheValue >> BinaryUtility::UINT_WRITE_CACHE_BIT_COUNT_SHIFT);
				assertResult(cacheBitCount == formulaBitIndex - offset, "uint write cache stored bitCount offset:" + IToS(offset) + " index:" + IToS(i));
				if (values[i] < BinaryUtility::UINT_WRITE_CACHE_PRODUCTION_COUNT)
				{
					assertResult(BinaryUtility::mUIntWriteCache[values[i]] == encodedCacheValue, "uint write production cache entry index:" + IToS(i));
					char cache32Buffer[32]{};
					for (int bit = 0; bit < offset; ++bit)
					{
						BinaryUtility::setBufferBitOne(cache32Buffer, bit);
					}
					int cache32BitIndex = offset;
					assertResult(BinaryUtility::writeEncodedUIntCache32Bit(cache32Buffer, (int)sizeof(cache32Buffer), cache32BitIndex, encodedCacheValue), "uint write cache32 direct offset:" + IToS(offset) + " index:" + IToS(i));
					assertResult(formulaBitIndex == cache32BitIndex, "uint write cache32 bitIndex offset:" + IToS(offset) + " index:" + IToS(i));
					assertResult(memcmp(formulaBuffer, cache32Buffer, byteCount) == 0, "uint write cache32 buffer offset:" + IToS(offset) + " index:" + IToS(i));
				}
			}
			if (byteCount > 0)
			{
				char truncatedBuffer[32]{};
				int truncatedBitIndex = offset;
				assertResult(!BinaryUtility::writeUnsignedIntegerBit(truncatedBuffer, byteCount - 1, truncatedBitIndex, values[i]), "uint write cache truncated must fail offset:" + IToS(offset) + " index:" + IToS(i));
				assertResult(truncatedBitIndex == offset, "uint write cache truncated bitIndex offset:" + IToS(offset) + " index:" + IToS(i));
			}
		}
	}
}

void BinaryUtilityTest::testListModeCorrectness()
{
	using namespace BinaryUtilitySerializeTestSupport;
	const Array<6, uint> unsignedUnity{ 8000u, 7234u, 10023u, 5u, 1234u, 5678u };
	const Array<6, uint> unsignedSingle{ 0u, 1u, 2u, 3u, 0x40000000u, 0u };
	const Array<6, int> signedUnity{ 8000, -7234, 10023, -5000, 1234, -5678 };
	const Array<6, int> signedSingle{ 0, 1, -2, 3, -1073741824, 0 };
	SerializerBitWrite writer;
	writer.initCapacity(256);
	assertResult(writer.writeUnsigned<6, uint>(unsignedUnity), "unsigned uniform list write");
	assertResult(writer.writeUnsigned<6, uint>(unsignedSingle), "unsigned independent list write");
	assertResult(writer.writeSigned<6, int>(true, signedUnity), "signed uniform list write");
	assertResult(writer.writeSigned<6, int>(true, signedSingle), "signed independent list write");
	SerializerBitRead reader(writer.getBuffer(), writer.getByteCount());
	uint u0 = 0, u1 = 0, u2 = 0, u3 = 0, u4 = 0, u5 = 0;
	assertResult(reader.readUnsigned(u0, u1, u2, u3, u4, u5), "unsigned uniform list read");
	assertResult(u0 == unsignedUnity[0] && u1 == unsignedUnity[1] && u2 == unsignedUnity[2] && u3 == unsignedUnity[3] && u4 == unsignedUnity[4] && u5 == unsignedUnity[5], "unsigned uniform list values");
	assertResult(reader.readUnsigned(u0, u1, u2, u3, u4, u5), "unsigned independent list read");
	assertResult(u0 == unsignedSingle[0] && u1 == unsignedSingle[1] && u2 == unsignedSingle[2] && u3 == unsignedSingle[3] && u4 == unsignedSingle[4] && u5 == unsignedSingle[5], "unsigned independent list values");
	int s0 = 0, s1 = 0, s2 = 0, s3 = 0, s4 = 0, s5 = 0;
	assertResult(reader.readSigned(true, s0, s1, s2, s3, s4, s5), "signed uniform list read");
	assertResult(s0 == signedUnity[0] && s1 == signedUnity[1] && s2 == signedUnity[2] && s3 == signedUnity[3] && s4 == signedUnity[4] && s5 == signedUnity[5], "signed uniform list values");
	assertResult(reader.readSigned(true, s0, s1, s2, s3, s4, s5), "signed independent list read");
	assertResult(s0 == signedSingle[0] && s1 == signedSingle[1] && s2 == signedSingle[2] && s3 == signedSingle[3] && s4 == signedSingle[4] && s5 == signedSingle[5], "signed independent list values");
}

void BinaryUtilityTest::testIndependentListCacheCorrectness()
{
	using namespace BinaryUtilitySerializeTestSupport;
	const uint unsignedValues[12]{ 0u, 1u, 2u, 3u, 0x40000000u, 0u, 7u, 8u, 15u, 16u, 0x20000000u, 65535u };
	const int signedValues[12]{ 0, 1, -2, 3, -1073741824, 0, 7, -8, 15, -16, 536870912, -65535 };
	FOR(8)
	{
		const int offset = i;
		char buffer[256]{};
		FOR_J(offset) { BinaryUtility::setBufferBitOne(buffer, j); }
		int writeBitIndex = offset;
		FOR_J(12) { assertResult(BinaryUtility::writeUnsignedIntegerBit(buffer, (int)sizeof(buffer), writeBitIndex, unsignedValues[j]), "unsigned independent cache write offset:" + IToS(offset) + " index:" + IToS(j)); }
		BinaryUtility::IntegerBitReadCache unsignedCache;
		unsignedCache.mBitIndex = offset;
		FOR_J(12)
		{
			uint value = 0;
			assertResult(BinaryUtility::readUnsignedIntegerCacheBit(buffer, BinaryUtility::bitCountToByteCount(writeBitIndex), unsignedCache, value) && value == unsignedValues[j], "unsigned independent cache read offset:" + IToS(offset) + " index:" + IToS(j));
		}
		assertResult(unsignedCache.mBitIndex == writeBitIndex, "unsigned independent cache bitIndex offset:" + IToS(offset));
		memset(buffer, 0, sizeof(buffer));
		FOR_J(offset) { BinaryUtility::setBufferBitOne(buffer, j); }
		writeBitIndex = offset;
		FOR_J(12) { assertResult(BinaryUtility::writeSignedIntegerBit(buffer, (int)sizeof(buffer), writeBitIndex, signedValues[j], true), "signed independent cache write offset:" + IToS(offset) + " index:" + IToS(j)); }
		BinaryUtility::IntegerBitReadCache signedCache;
		signedCache.mBitIndex = offset;
		FOR_J(12)
		{
			int value = 0;
			assertResult(BinaryUtility::readSignedIntegerCacheBit(buffer, BinaryUtility::bitCountToByteCount(writeBitIndex), signedCache, value, true) && value == signedValues[j], "signed independent cache read offset:" + IToS(offset) + " index:" + IToS(j));
		}
		assertResult(signedCache.mBitIndex == writeBitIndex, "signed independent cache bitIndex offset:" + IToS(offset));
	}
}

void BinaryUtilityTest::testFloatCachedBitCountCorrectness()
{
	using namespace BinaryUtilitySerializeTestSupport;
	const float profiles[2][6]
	{
		{ 0.125f, 1.25f, 12.345f, -0.75f, -23.456f, 100.001f },
		{ 8.0f, 7.234f, 10.023f, -5.0f, 1.234f, -5.678f },
	};
	FOR(2)
	{
		const int profile = i;
		FOR_J(8)
		{
			const int offset = j;
			char referenceBuffer[128]{};
			char productionBuffer[128]{};
			for (int k = 0; k < offset; ++k)
			{
				BinaryUtility::setBufferBitOne(referenceBuffer, k);
				BinaryUtility::setBufferBitOne(productionBuffer, k);
			}
			int referenceBitIndex = offset;
			int productionBitIndex = offset;
			assertResult(writeFloatListBitV23Reference(referenceBuffer, (int)sizeof(referenceBuffer), referenceBitIndex, profiles[profile], 6, true), "float cached reference write profile:" + IToS(profile) + " offset:" + IToS(offset));
			assertResult(BinaryUtility::writeFloatListBit(productionBuffer, (int)sizeof(productionBuffer), productionBitIndex, profiles[profile], 6, false, true, 3), "float cached production write profile:" + IToS(profile) + " offset:" + IToS(offset));
			assertResult(referenceBitIndex == productionBitIndex, "float cached bitIndex profile:" + IToS(profile) + " offset:" + IToS(offset));
			const int byteCount = BinaryUtility::bitCountToByteCount(referenceBitIndex);
			assertResult(memcmp(referenceBuffer, productionBuffer, byteCount) == 0, "float cached buffer profile:" + IToS(profile) + " offset:" + IToS(offset));
		}
	}
}

void BinaryUtilityTest::testFloatPackedBitCountCacheBoundaryCorrectness()
{
	using namespace BinaryUtilitySerializeTestSupport;
	const float values13[13]{ 0.001f, -0.002f, 0.004f, -0.008f, 0.016f, -0.032f, 0.064f, -0.128f, 0.256f, -0.512f, 1.024f, -10.023f, 100.001f };
	const int counts[2]{ 12, 13 };
	FOR(2)
	{
		const int count = counts[i];
		FOR_J(8)
		{
			const int offset = j;
			char referenceBuffer[256]{};
			char productionBuffer[256]{};
			for (int k = 0; k < offset; ++k)
			{
				BinaryUtility::setBufferBitOne(referenceBuffer, k);
				BinaryUtility::setBufferBitOne(productionBuffer, k);
			}
			int referenceBitIndex = offset;
			int productionBitIndex = offset;
			assertResult(writeFloatListBitV23Reference(referenceBuffer, (int)sizeof(referenceBuffer), referenceBitIndex, values13, count, true), "float packed reference count:" + IToS(count) + " offset:" + IToS(offset));
			assertResult(BinaryUtility::writeFloatListBit(productionBuffer, (int)sizeof(productionBuffer), productionBitIndex, values13, count, false, true, 3), "float packed production count:" + IToS(count) + " offset:" + IToS(offset));
			assertResult(referenceBitIndex == productionBitIndex, "float packed bitIndex count:" + IToS(count) + " offset:" + IToS(offset));
			const int byteCount = BinaryUtility::bitCountToByteCount(referenceBitIndex);
			assertResult(memcmp(referenceBuffer, productionBuffer, byteCount) == 0, "float packed buffer count:" + IToS(count) + " offset:" + IToS(offset));
		}
	}
}

void BinaryUtilityTest::testSignedIntPointerListCorrectness()
{
	using namespace BinaryUtilitySerializeTestSupport;
	const Array<6, int> uniformValues{ 8000, -7234, 10023, -5000, 1234, -5678 };
	const Array<6, int> independentValues{ 0, 1, -2, 3, -1073741824, 0 };
	const Array<6, int>* profiles[2]{ &uniformValues, &independentValues };
	FOR(2)
	{
		FOR_J(8)
		{
			const int offset = j;
			char buffer[128]{};
			for (int bit = 0; bit < offset; ++bit) { BinaryUtility::setBufferBitOne(buffer, bit); }
			int writeBitIndex = offset;
			assertResult(BinaryUtility::writeSignedIntegerListBit(buffer, (int)sizeof(buffer), writeBitIndex, *profiles[i], true), "signed int pointer write profile:" + IToS(i) + " offset:" + IToS(offset));
			int r0 = 0, r1 = 0, r2 = 0, r3 = 0, r4 = 0, r5 = 0;
			int p0 = 0, p1 = 0, p2 = 0, p3 = 0, p4 = 0, p5 = 0;
			Array<6, int*> referenceList{ &r0, &r1, &r2, &r3, &r4, &r5 };
			Array<6, int*> productionList{ &p0, &p1, &p2, &p3, &p4, &p5 };
			int referenceBitIndex = offset;
			int productionBitIndex = offset;
			const int bufferSize = BinaryUtility::bitCountToByteCount(writeBitIndex);
			assertResult(BinaryUtility::readSignedIntegerListBit<int, 6>(buffer, bufferSize, referenceBitIndex, referenceList, true), "signed int pointer reference profile:" + IToS(i) + " offset:" + IToS(offset));
			assertResult(BinaryUtility::readSignedIntegerListBit(buffer, bufferSize, productionBitIndex, productionList, true), "signed int pointer production profile:" + IToS(i) + " offset:" + IToS(offset));
			assertResult(referenceBitIndex == productionBitIndex && productionBitIndex == writeBitIndex, "signed int pointer bitIndex profile:" + IToS(i) + " offset:" + IToS(offset));
			assertResult(r0 == p0 && r1 == p1 && r2 == p2 && r3 == p3 && r4 == p4 && r5 == p5, "signed int pointer values profile:" + IToS(i) + " offset:" + IToS(offset));
		}
	}
}

void BinaryUtilityTest::testSignedLLongPointerListCorrectness()
{
	using namespace BinaryUtilitySerializeTestSupport;
	const Array<2, llong> uniformValues{ 123456789LL, -123456700LL };
	const Array<2, llong> independentValues{ 123LL, -987654321LL };
	const Array<2, llong>* profiles[2]{ &uniformValues, &independentValues };
	FOR(2)
	{
		FOR_J(8)
		{
			const int offset = j;
			char buffer[128]{};
			for (int bit = 0; bit < offset; ++bit) { BinaryUtility::setBufferBitOne(buffer, bit); }
			int writeBitIndex = offset;
			assertResult(BinaryUtility::writeSignedIntegerListBit(buffer, (int)sizeof(buffer), writeBitIndex, *profiles[i], true), "signed llong pointer write profile:" + IToS(i) + " offset:" + IToS(offset));
			llong r0 = 0, r1 = 0;
			llong p0 = 0, p1 = 0;
			Array<2, llong*> referenceList{ &r0, &r1 };
			Array<2, llong*> productionList{ &p0, &p1 };
			int referenceBitIndex = offset;
			int productionBitIndex = offset;
			const int bufferSize = BinaryUtility::bitCountToByteCount(writeBitIndex);
			assertResult(BinaryUtility::readSignedIntegerListBit<llong, 2>(buffer, bufferSize, referenceBitIndex, referenceList, true), "signed llong pointer reference profile:" + IToS(i) + " offset:" + IToS(offset));
			assertResult(BinaryUtility::readSignedIntegerListBit(buffer, bufferSize, productionBitIndex, productionList, true), "signed llong pointer production profile:" + IToS(i) + " offset:" + IToS(offset));
			assertResult(referenceBitIndex == productionBitIndex && productionBitIndex == writeBitIndex, "signed llong pointer bitIndex profile:" + IToS(i) + " offset:" + IToS(offset));
			assertResult(r0 == p0 && r1 == p1, "signed llong pointer values profile:" + IToS(i) + " offset:" + IToS(offset));
		}
	}
}

void BinaryUtilityTest::testUnsignedIntPointerListCorrectness()
{
	using namespace BinaryUtilitySerializeTestSupport;
	const Array<6, uint> uniformValues{ 8000u, 7234u, 10023u, 5u, 1234u, 5678u };
	const Array<6, uint> independentValues{ 0u, 1u, 2u, 3u, 0x40000000u, 0u };
	const Array<6, uint>* profiles[2]{ &uniformValues, &independentValues };
	FOR(2)
	{
		FOR_J(8)
		{
			const int offset = j;
			char buffer[128]{};
			for (int bit = 0; bit < offset; ++bit) { BinaryUtility::setBufferBitOne(buffer, bit); }
			int writeBitIndex = offset;
			assertResult(BinaryUtility::writeUnsignedIntegerListBit(buffer, (int)sizeof(buffer), writeBitIndex, *profiles[i]), "unsigned int pointer write profile:" + IToS(i) + " offset:" + IToS(offset));
			uint r0 = 0, r1 = 0, r2 = 0, r3 = 0, r4 = 0, r5 = 0;
			uint p0 = 0, p1 = 0, p2 = 0, p3 = 0, p4 = 0, p5 = 0;
			Array<6, uint*> referenceList{ &r0, &r1, &r2, &r3, &r4, &r5 };
			Array<6, uint*> productionList{ &p0, &p1, &p2, &p3, &p4, &p5 };
			int referenceBitIndex = offset;
			int productionBitIndex = offset;
			const int bufferSize = BinaryUtility::bitCountToByteCount(writeBitIndex);
			assertResult(BinaryUtility::readUnsignedIntegerListBit<uint, 6>(buffer, bufferSize, referenceBitIndex, referenceList), "unsigned int pointer reference profile:" + IToS(i) + " offset:" + IToS(offset));
			assertResult(BinaryUtility::readUnsignedIntegerListBit(buffer, bufferSize, productionBitIndex, productionList), "unsigned int pointer production profile:" + IToS(i) + " offset:" + IToS(offset));
			assertResult(referenceBitIndex == productionBitIndex && productionBitIndex == writeBitIndex, "unsigned int pointer bitIndex profile:" + IToS(i) + " offset:" + IToS(offset));
			assertResult(r0 == p0 && r1 == p1 && r2 == p2 && r3 == p3 && r4 == p4 && r5 == p5, "unsigned int pointer values profile:" + IToS(i) + " offset:" + IToS(offset));
		}
	}
}

void BinaryUtilityTest::testUnsignedIntVectorSpecializationCorrectness()
{
	using namespace BinaryUtilitySerializeTestSupport;
	const uint uniformSource[]{ 8000u, 7234u, 10023u, 5u, 1234u, 5678u };
	const uint independentSource[]{ 0u, 1u, 2u, 3u, 15u, 31u, 255u, 1024u, 8000u, 10023u, 65535u, 100000u };
	const uint* sources[2]{ uniformSource, independentSource };
	const int counts[2]{ 6, 12 };
	FOR(2)
	{
		Vector<uint> values;
		values.reserve(counts[i]);
		FOR_J(counts[i]) { values.add(sources[i][j]); }
		FOR_J(8)
		{
			const int offset = j;
			char buffer[256]{};
			for (int bit = 0; bit < offset; ++bit) { BinaryUtility::setBufferBitOne(buffer, bit); }
			int writeBitIndex = offset;
			assertResult(BinaryUtility::writeUnsignedIntegerListBit(buffer, (int)sizeof(buffer), writeBitIndex, values), "unsigned int vector write profile:" + IToS(i) + " offset:" + IToS(offset));
			Vector<uint> v34List;
			Vector<uint> v35List;
			Vector<uint> v36List;
			Vector<uint> v37List;
			v34List.reserve(counts[i]);
			v35List.reserve(counts[i]);
			v36List.reserve(counts[i]);
			v37List.reserve(counts[i]);
			int v34BitIndex = offset;
			int v35BitIndex = offset;
			int v36BitIndex = offset;
			int v37BitIndex = offset;
			const int bufferSize = BinaryUtility::bitCountToByteCount(writeBitIndex);
			assertResult(BinaryUtility::readUnsignedIntegerListBit<uint>(buffer, bufferSize, v34BitIndex, v34List), "unsigned int vector V34 reference profile:" + IToS(i) + " offset:" + IToS(offset));
			assertResult(readUnsignedIntVectorV35Reference(buffer, bufferSize, v35BitIndex, v35List), "unsigned int vector V35 reference profile:" + IToS(i) + " offset:" + IToS(offset));
			assertResult(readUnsignedIntVectorV36Reference(buffer, bufferSize, v36BitIndex, v36List), "unsigned int vector V36 reference profile:" + IToS(i) + " offset:" + IToS(offset));
			assertResult(BinaryUtility::readUnsignedIntegerListBit(buffer, bufferSize, v37BitIndex, v37List), "unsigned int vector V37 production profile:" + IToS(i) + " offset:" + IToS(offset));
			assertResult(v34BitIndex == v35BitIndex && v35BitIndex == v36BitIndex && v36BitIndex == v37BitIndex && v37BitIndex == writeBitIndex, "unsigned int vector bitIndex profile:" + IToS(i) + " offset:" + IToS(offset));
			assertResult(v34List.size() == v35List.size() && v35List.size() == v36List.size() && v36List.size() == v37List.size(), "unsigned int vector size profile:" + IToS(i) + " offset:" + IToS(offset));
			for (int k = 0; k < v34List.size(); ++k)
			{
				assertResult(v34List[k] == v35List[k] && v35List[k] == v36List[k] && v36List[k] == v37List[k], "unsigned int vector value profile:" + IToS(i) + " offset:" + IToS(offset) + " index:" + IToS(k));
			}
		}
	}
	// V37去掉clear后必须保持V36失败时的列表状态一致,这里用截断buffer覆盖count/header/element各阶段失败。
	Vector<uint> failureValues;
	const uint failureSource[]{ 0u, 1u, 2u, 3u, 15u, 31u, 255u, 1024u, 8000u, 10023u, 65535u, 100000u };
	for (uint value : failureSource) { failureValues.add(value); }
	char failureBuffer[256]{};
	int failureWriteBitIndex = 0;
	assertResult(BinaryUtility::writeUnsignedIntegerListBit(failureBuffer, (int)sizeof(failureBuffer), failureWriteBitIndex, failureValues), "unsigned int vector failure write");
	const int fullBufferSize = BinaryUtility::bitCountToByteCount(failureWriteBitIndex);
	for (int testBufferSize = 0; testBufferSize <= fullBufferSize; ++testBufferSize)
	{
		Vector<uint> v36List{ 111u, 222u, 333u, 444u };
		Vector<uint> v37List{ 111u, 222u, 333u, 444u };
		int v36BitIndex = 0;
		int v37BitIndex = 0;
		const bool v36Result = readUnsignedIntVectorV36Reference(failureBuffer, testBufferSize, v36BitIndex, v36List);
		const bool v37Result = BinaryUtility::readUnsignedIntegerListBit(failureBuffer, testBufferSize, v37BitIndex, v37List);
		assertResult(v36Result == v37Result, "unsigned int vector failure result size:" + IToS(testBufferSize));
		assertResult(v36BitIndex == v37BitIndex, "unsigned int vector failure bitIndex size:" + IToS(testBufferSize));
		assertResult(v36List.size() == v37List.size(), "unsigned int vector failure list size:" + IToS(testBufferSize));
		for (int i = 0; i < v36List.size(); ++i)
		{
			assertResult(v36List[i] == v37List[i], "unsigned int vector failure value size:" + IToS(testBufferSize) + " index:" + IToS(i));
		}
	}
}

void BinaryUtilityTest::testUIntReadShortCodeCorrectness()
{
	using namespace BinaryUtilitySerializeTestSupport;
	const Array<6, uint> values{ 0u, 1u, 2u, 3u, 0x40000000u, 0u };
	FOR(8)
	{
		const int offset = i;
		char listBuffer[64]{};
		for (int bit = 0; bit < offset; ++bit) { BinaryUtility::setBufferBitOne(listBuffer, bit); }
		int listWriteBitIndex = offset;
		assertResult(BinaryUtility::writeUnsignedIntegerListBit(listBuffer, (int)sizeof(listBuffer), listWriteBitIndex, values), "uint short list write offset:" + IToS(offset));
		uint r0 = 0, r1 = 0, r2 = 0, r3 = 0, r4 = 0, r5 = 0;
		uint p0 = 0, p1 = 0, p2 = 0, p3 = 0, p4 = 0, p5 = 0;
		Array<6, uint*> referenceList{ &r0, &r1, &r2, &r3, &r4, &r5 };
		Array<6, uint*> productionList{ &p0, &p1, &p2, &p3, &p4, &p5 };
		int referenceListBitIndex = offset;
		int productionListBitIndex = offset;
		const int listBufferSize = BinaryUtility::bitCountToByteCount(listWriteBitIndex);
		assertResult(readUIntList6V28Reference(listBuffer, listBufferSize, referenceListBitIndex, referenceList), "uint short list reference offset:" + IToS(offset));
		assertResult(BinaryUtility::readUnsignedIntegerListBit(listBuffer, listBufferSize, productionListBitIndex, productionList), "uint short list production offset:" + IToS(offset));
		assertResult(referenceListBitIndex == productionListBitIndex && productionListBitIndex == listWriteBitIndex, "uint short list bitIndex offset:" + IToS(offset));
		assertResult(r0 == p0 && r1 == p1 && r2 == p2 && r3 == p3 && r4 == p4 && r5 == p5, "uint short list values offset:" + IToS(offset));
	}
}

void BinaryUtilityTest::testFloatListModeCorrectness()
{
	using namespace BinaryUtilitySerializeTestSupport;
	const Array<6, float> uniformValues{ 8.0f, 7.234f, 10.023f, -5.0f, 1.234f, -5.678f };
	const Array<6, float> independentValues{ 0.125f, 1.25f, 12.345f, -0.75f, -23.456f, 100.001f };
	SerializerBitWrite writer;
	writer.initCapacity(128);
	assertResult(writer.writeFloat<6>(true, uniformValues), "float uniform list write");
	assertResult(BinaryUtility::getBufferBit(writer.getBuffer(), 0), "float uniform mode bit");
	assertResult(writer.getBitCount() == 96, "float uniform bit count:" + IToS(writer.getBitCount()));
	SerializerBitRead reader(writer.getBuffer(), writer.getByteCount());
	float u0 = 0, u1 = 0, u2 = 0, u3 = 0, u4 = 0, u5 = 0;
	assertResult(reader.readFloat(true, u0, u1, u2, u3, u4, u5), "float uniform list read");
	assertResult(fabsf(u0 - uniformValues[0]) < 0.0011f && fabsf(u1 - uniformValues[1]) < 0.0011f && fabsf(u2 - uniformValues[2]) < 0.0011f && fabsf(u3 - uniformValues[3]) < 0.0011f && fabsf(u4 - uniformValues[4]) < 0.0011f && fabsf(u5 - uniformValues[5]) < 0.0011f, "float uniform values");
	writer.clear();
	assertResult(writer.writeFloat<6>(true, independentValues), "float independent list write");
	assertResult(!BinaryUtility::getBufferBit(writer.getBuffer(), 0), "float independent mode bit");
	assertResult(writer.getBitCount() == 105, "float independent bit count:" + IToS(writer.getBitCount()));
	SerializerBitRead independentReader(writer.getBuffer(), writer.getByteCount());
	float i0 = 0, i1 = 0, i2 = 0, i3 = 0, i4 = 0, i5 = 0;
	assertResult(independentReader.readFloat(true, i0, i1, i2, i3, i4, i5), "float independent list read");
	assertResult(fabsf(i0 - independentValues[0]) < 0.0011f && fabsf(i1 - independentValues[1]) < 0.0011f && fabsf(i2 - independentValues[2]) < 0.0011f && fabsf(i3 - independentValues[3]) < 0.0011f && fabsf(i4 - independentValues[4]) < 0.0011f && fabsf(i5 - independentValues[5]) < 0.0011f, "float independent values");
}

void BinaryUtilityTest::testSerializerReuseCorrectness()
{
	using namespace BinaryUtilitySerializeTestSupport;
	SerializerBitWrite writer;
	writer.initCapacity(128);
	assertResult(writer.writeUnsigned(UINT_MAX, UINT_MAX, UINT_MAX, UINT_MAX), "reuse dirty unsigned write");
	assertResult(writer.writeSigned(true, INT_MAX, -INT_MAX), "reuse dirty signed write");
	writer.clear();
	assertResult(writer.writeBool(false), "reuse bool write");
	assertResult(writer.writeUnsigned(0u, 1u, 2u, 3u), "reuse small unsigned write");
	assertResult(writer.writeSigned(true, 0, -1), "reuse small signed write");
	assertResult(writer.writeFloat(true, -1.25f), "reuse float write");
	SerializerBitRead reader(writer.getBuffer(), writer.getByteCount());
	bool boolValue = true;
	uint u0 = 0, u1 = 0, u2 = 0, u3 = 0;
	int s0 = 0, s1 = 0;
	float f = 0.0f;
	assertResult(reader.readBool(boolValue) && !boolValue, "reuse bool value");
	assertResult(reader.readUnsigned(u0, u1, u2, u3) && u0 == 0u && u1 == 1u && u2 == 2u && u3 == 3u, "reuse unsigned values");
	assertResult(reader.readSigned(true, s0, s1) && s0 == 0 && s1 == -1, "reuse signed values");
	assertResult(reader.readFloat(true, f) && fabsf(f + 1.25f) < 0.0011f, "reuse float value");
}

void BinaryUtilityTest::testTruncatedBufferCorrectness()
{
	using namespace BinaryUtilitySerializeTestSupport;
	char buffer[64]{};
	int writeBit = 0;
	assertResult(BinaryUtility::writeUnsignedIntegerBit(buffer, (int)sizeof(buffer), writeBit, UINT_MAX), "truncated uint write");
	int readBit = 0;
	uint uintValue = 0;
	const int uintBytes = BinaryUtility::bitCountToByteCount(writeBit);
	assertResult(uintBytes > 1 && !BinaryUtility::readUnsignedIntegerBit(buffer, uintBytes - 1, readBit, uintValue), "truncated uint must fail");
	memset(buffer, 0, sizeof(buffer));
	writeBit = 0;
	assertResult(BinaryUtility::writeSignedIntegerBit(buffer, (int)sizeof(buffer), writeBit, LLONG_MAX, true), "truncated llong write");
	readBit = 0;
	llong llongValue = 0;
	const int llongBytes = BinaryUtility::bitCountToByteCount(writeBit);
	assertResult(llongBytes > 1 && !BinaryUtility::readSignedIntegerBit(buffer, llongBytes - 1, readBit, llongValue, true), "truncated llong must fail");
}

void BinaryUtilityTest::testProtocolGoldenBuffer()
{
	using namespace BinaryUtilitySerializeTestSupport;
	SerializerBitWrite writer;
	writer.initCapacity(128);
	assertResult(writer.writeSigned(true, 123456789LL, -987654321LL), "golden signed write");
	assertResult(writer.writeUnsigned(8000u, 7234u, 10023u, 5u, 1234u, 5678u), "golden unsigned write");
	assertResult(writer.writeUnsigned(3u), "golden direction write");
	assertResult(writer.writeFloat(true, 1.25f), "golden float write");
	static const byte expected[] = { 0x3D, 0x15, 0xCD, 0x5B, 0xC7, 0x58, 0x34, 0x6F, 0xBD, 0x03, 0xFA, 0x84, 0xB8, 0x93, 0xB3, 0x00, 0x90, 0x26, 0x5C, 0x2C, 0x71, 0x11, 0x07 };
	assertResult(writer.getBitCount() == 181, "golden bit count changed:" + IToS(writer.getBitCount()));
	assertResult(writer.getByteCount() == (int)sizeof(expected), "golden byte count changed:" + IToS(writer.getByteCount()));
	if (writer.getByteCount() == (int)sizeof(expected))
	{
		assertResult(memcmp(writer.getBuffer(), expected, sizeof(expected)) == 0, "golden buffer changed, protocol compatibility broken");
	}
	writer.clear();
	const Array<6, uint> independentList{ 0u, 1u, 2u, 3u, 0x40000000u, 0u };
	assertResult(writer.writeUnsigned<6, uint>(independentList), "golden independent list write");
	static const byte expectedIndependent[] = { 0x40, 0x10, 0xC4, 0x0F, 0x00, 0x00, 0x00, 0x04, 0x00 };
	assertResult(writer.getBitCount() == 65, "golden independent bit count changed:" + IToS(writer.getBitCount()));
	assertResult(writer.getByteCount() == (int)sizeof(expectedIndependent), "golden independent byte count changed");
	if (writer.getByteCount() == (int)sizeof(expectedIndependent))
	{
		assertResult(memcmp(writer.getBuffer(), expectedIndependent, sizeof(expectedIndependent)) == 0, "golden independent buffer changed");
	}
}

void BinaryUtilityTest::correctnessTest()
{
	using namespace BinaryUtilitySerializeTestSupport;
	LOG("开始BinaryUtility按位/按字节一致性测试...");
	testSignedCorrectness();
	testUnsignedCorrectness();
	testUnsignedTypeBoundaries();
	testExtendedIntegerCorrectness();
	testUIntWriteCacheCorrectness();
	testListModeCorrectness();
	testIndependentListCacheCorrectness();
	testSignedIntPointerListCorrectness();
	testSignedLLongPointerListCorrectness();
	testUnsignedIntPointerListCorrectness();
	testUnsignedIntVectorSpecializationCorrectness();
	testUIntReadShortCodeCorrectness();
	testFloatListModeCorrectness();
	testFloatCachedBitCountCorrectness();
	testFloatPackedBitCountCacheBoundaryCorrectness();
	testSerializerReuseCorrectness();
	testTruncatedBufferCorrectness();
	testSerializerCorrectness();
	testProtocolGoldenBuffer();
	LOG("结束BinaryUtility按位/按字节一致性测试...");
}

void BinaryUtilityTest::fillSignedValues(Vector<int>& values, const int profile)
{
	using namespace BinaryUtilitySerializeTestSupport;
	values.resize(BENCHMARK_VALUE_COUNT);
	uint randomState = 0x12345678u + (uint)profile;
	FOR(BENCHMARK_VALUE_COUNT)
	{
		const uint randomValue = nextRandom(randomState);
		if (profile == 0)
		{
			values[i] = (int)(randomValue % 255u) - 127;
		}
		else if (profile == 1)
		{
			values[i] = (int)(randomValue % 131071u) - 65535;
		}
		else
		{
			int value = (int)(randomValue & 0x7FFFFFFFu);
			values[i] = (randomValue & 0x80000000u) != 0 ? -value : value;
		}
	}
}

void BinaryUtilityTest::fillUnsignedValues(Vector<uint>& values, const int profile)
{
	using namespace BinaryUtilitySerializeTestSupport;
	values.resize(BENCHMARK_VALUE_COUNT);
	uint randomState = 0x87654321u + (uint)profile;
	FOR(BENCHMARK_VALUE_COUNT)
	{
		const uint randomValue = nextRandom(randomState);
		values[i] = profile == 0 ? randomValue & 0xFFu : profile == 1 ? randomValue & 0xFFFFu : randomValue;
	}
}

void BinaryUtilityTest::logBenchmarkResult(const string& profileName, const llong byteWriteNS, const llong bitWriteNS, const llong byteReadNS, const llong bitReadNS, const int byteSize, const int bitSize)
{
	using namespace BinaryUtilitySerializeTestSupport;
	LOG("================ " + profileName + " ================");
	LOG("Byte Write Median:" + formatMS(byteWriteNS) + " ms | " + formatNSPerValue(byteWriteNS) + " ns/value");
	LOG("Bit  Write Median:" + formatMS(bitWriteNS) + " ms | " + formatNSPerValue(bitWriteNS) + " ns/value | Bit/Byte:" + formatRatio(bitWriteNS, byteWriteNS) + "x");
	LOG("Byte Read  Median:" + formatMS(byteReadNS) + " ms | " + formatNSPerValue(byteReadNS) + " ns/value");
	LOG("Bit  Read  Median:" + formatMS(bitReadNS) + " ms | " + formatNSPerValue(bitReadNS) + " ns/value | Bit/Byte:" + formatRatio(bitReadNS, byteReadNS) + "x");
	LOG("Byte Size:" + IToS(byteSize) + " bytes | Bit Size:" + IToS(bitSize) + " bytes | Bit/Byte:" + FToS(byteSize > 0 ? (float)bitSize / (float)byteSize * 100.0f : 0.0f, 2) + "%");
}

void BinaryUtilityTest::benchmarkSigned(const string& profileName, const int profile)
{
	using namespace BinaryUtilitySerializeTestSupport;
	Vector<int> values;
	fillSignedValues(values, profile);
	vector<char> byteBuffer((size_t)BENCHMARK_VALUE_COUNT * sizeof(int));
	vector<char> bitBuffer((size_t)BENCHMARK_VALUE_COUNT * (sizeof(int) + 1) + 16, 0);
	int byteSize = 0;
	int bitCount = 0;
	FOR(BENCHMARK_VALUE_COUNT)
	{
		BinaryUtility::write(byteBuffer.data(), (int)byteBuffer.size(), byteSize, values[i]);
		BinaryUtility::writeSignedIntegerBit(bitBuffer.data(), (int)bitBuffer.size(), bitCount, values[i], true);
	}
	const int bitSize = BinaryUtility::bitCountToByteCount(bitCount);
	volatile llong readSink = 0;
	const llong byteWriteNS = measureMedianNS([]() {}, [&]()
		{
			int index = 0;
			FOR(BENCHMARK_VALUE_COUNT)
			{
				BinaryUtility::write(byteBuffer.data(), (int)byteBuffer.size(), index, values[i]);
			}
		});
	const llong bitWriteNS = measureMedianNS([&]() { memset(bitBuffer.data(), 0, bitSize); }, [&]()
		{
			int index = 0;
			FOR(BENCHMARK_VALUE_COUNT)
			{
				BinaryUtility::writeSignedIntegerBit(bitBuffer.data(), (int)bitBuffer.size(), index, values[i], true);
			}
		});
	const llong byteReadNS = measureMedianNS([]() {}, [&]()
		{
			int index = 0;
			llong sum = 0;
			FOR(BENCHMARK_VALUE_COUNT)
			{
				int value = 0;
				BinaryUtility::read(byteBuffer.data(), byteSize, index, value);
				sum += value;
			}
			readSink = sum;
		});
	const llong bitReadNS = measureMedianNS([]() {}, [&]()
		{
			int index = 0;
			llong sum = 0;
			FOR(BENCHMARK_VALUE_COUNT)
			{
				int value = 0;
				BinaryUtility::readSignedIntegerBit(bitBuffer.data(), bitSize, index, value, true);
				sum += value;
			}
			readSink = sum;
		});
	logBenchmarkResult(profileName, byteWriteNS, bitWriteNS, byteReadNS, bitReadNS, byteSize, bitSize);
	(void)readSink;
}

template<typename T>
void BinaryUtilityTest::benchmarkUnsignedType(const string& profileName, const uint mask)
{
	using namespace BinaryUtilitySerializeTestSupport;
	Vector<T> values;
	values.resize(BENCHMARK_VALUE_COUNT);
	uint randomState = 0xA5A5A5A5u + (uint)sizeof(T);
	FOR(BENCHMARK_VALUE_COUNT)
	{
		values[i] = (T)(nextRandom(randomState) & mask);
	}
	vector<char> byteBuffer((size_t)BENCHMARK_VALUE_COUNT * sizeof(T));
	vector<char> bitBuffer((size_t)BENCHMARK_VALUE_COUNT * (sizeof(T) + 1) + 16, 0);
	int byteSize = 0;
	int bitCount = 0;
	FOR(BENCHMARK_VALUE_COUNT)
	{
		BinaryUtility::write(byteBuffer.data(), (int)byteBuffer.size(), byteSize, values[i]);
		BinaryUtility::writeUnsignedIntegerBit(bitBuffer.data(), (int)bitBuffer.size(), bitCount, values[i]);
	}
	const int bitSize = BinaryUtility::bitCountToByteCount(bitCount);
	volatile ullong readSink = 0;
	const llong byteWriteNS = measureMedianNS([]() {}, [&]()
		{
			int index = 0;
			FOR(BENCHMARK_VALUE_COUNT)
			{
				BinaryUtility::write(byteBuffer.data(), (int)byteBuffer.size(), index, values[i]);
			}
		});
	const llong bitWriteNS = measureMedianNS([&]() { memset(bitBuffer.data(), 0, bitSize); }, [&]()
		{
			int index = 0;
			FOR(BENCHMARK_VALUE_COUNT)
			{
				BinaryUtility::writeUnsignedIntegerBit(bitBuffer.data(), (int)bitBuffer.size(), index, values[i]);
			}
		});
	const llong byteReadNS = measureMedianNS([]() {}, [&]()
		{
			int index = 0;
			ullong sum = 0;
			FOR(BENCHMARK_VALUE_COUNT)
			{
				T value = 0;
				BinaryUtility::read(byteBuffer.data(), byteSize, index, value);
				sum += value;
			}
			readSink = sum;
		});
	const llong bitReadNS = measureMedianNS([]() {}, [&]()
		{
			int index = 0;
			ullong sum = 0;
			FOR(BENCHMARK_VALUE_COUNT)
			{
				T value = 0;
				BinaryUtility::readUnsignedIntegerBit(bitBuffer.data(), bitSize, index, value);
				sum += value;
			}
			readSink = sum;
		});
	logBenchmarkResult(profileName, byteWriteNS, bitWriteNS, byteReadNS, bitReadNS, byteSize, bitSize);
	(void)readSink;
}

void BinaryUtilityTest::benchmarkUnsigned(const string& profileName, const int profile)
{
	using namespace BinaryUtilitySerializeTestSupport;
	Vector<uint> values;
	fillUnsignedValues(values, profile);
	vector<char> byteBuffer((size_t)BENCHMARK_VALUE_COUNT * sizeof(uint));
	vector<char> bitBuffer((size_t)BENCHMARK_VALUE_COUNT * (sizeof(uint) + 1) + 16, 0);
	int byteSize = 0;
	int bitCount = 0;
	FOR(BENCHMARK_VALUE_COUNT)
	{
		BinaryUtility::write(byteBuffer.data(), (int)byteBuffer.size(), byteSize, values[i]);
		BinaryUtility::writeUnsignedIntegerBit(bitBuffer.data(), (int)bitBuffer.size(), bitCount, values[i]);
	}
	const int bitSize = BinaryUtility::bitCountToByteCount(bitCount);
	volatile ullong readSink = 0;
	const llong byteWriteNS = measureMedianNS([]() {}, [&]()
		{
			int index = 0;
			FOR(BENCHMARK_VALUE_COUNT)
			{
				BinaryUtility::write(byteBuffer.data(), (int)byteBuffer.size(), index, values[i]);
			}
		});
	const llong bitWriteNS = measureMedianNS([&]() { memset(bitBuffer.data(), 0, bitSize); }, [&]()
		{
			int index = 0;
			FOR(BENCHMARK_VALUE_COUNT)
			{
				BinaryUtility::writeUnsignedIntegerBit(bitBuffer.data(), (int)bitBuffer.size(), index, values[i]);
			}
		});
	const llong byteReadNS = measureMedianNS([]() {}, [&]()
		{
			int index = 0;
			ullong sum = 0;
			FOR(BENCHMARK_VALUE_COUNT)
			{
				uint value = 0;
				BinaryUtility::read(byteBuffer.data(), byteSize, index, value);
				sum += value;
			}
			readSink = sum;
		});
	const llong bitReadNS = measureMedianNS([]() {}, [&]()
		{
			int index = 0;
			ullong sum = 0;
			FOR(BENCHMARK_VALUE_COUNT)
			{
				uint value = 0;
				BinaryUtility::readUnsignedIntegerBit(bitBuffer.data(), bitSize, index, value);
				sum += value;
			}
			readSink = sum;
		});
	logBenchmarkResult(profileName, byteWriteNS, bitWriteNS, byteReadNS, bitReadNS, byteSize, bitSize);
	(void)readSink;
}

void BinaryUtilityTest::benchmarkSerializerMessage()
{
	using namespace BinaryUtilitySerializeTestSupport;
	const llong guid0 = 123456789LL;
	const llong guid1 = 987654321LL;
	const uint maxHP = 8000u;
	const uint hp = 7234u;
	const uint monsterID = 10023u;
	const uint actionState = 5u;
	const uint posX = 1234u;
	const uint posY = 5678u;
	const uint direction = 3u;
	const float attackSpeed = 1.25f;
	SerializerWrite byteWriter;
	byteWriter.initCapacity(128);
	byteWriter.write(guid0);
	byteWriter.write(guid1);
	byteWriter.write(maxHP);
	byteWriter.write(hp);
	byteWriter.write(monsterID);
	byteWriter.write(actionState);
	byteWriter.write(posX);
	byteWriter.write(posY);
	byteWriter.write(direction);
	byteWriter.write(attackSpeed);
	const int byteSize = byteWriter.getDataSize();
	SerializerBitWrite bitWriter;
	bitWriter.initCapacity(128);
	bitWriter.writeSigned(true, guid0, guid1);
	bitWriter.writeUnsigned(maxHP, hp, monsterID, actionState, posX, posY);
	bitWriter.writeUnsigned(direction);
	bitWriter.writeFloat(true, attackSpeed);
	const int bitSize = bitWriter.getByteCount();
	const llong byteWriteNS = measureMedianNS([]() {}, [&]()
		{
			FOR(BENCHMARK_MESSAGE_COUNT)
			{
				byteWriter.clear();
				byteWriter.write(guid0);
				byteWriter.write(guid1);
				byteWriter.write(maxHP);
				byteWriter.write(hp);
				byteWriter.write(monsterID + (uint)(i & 1023));
				byteWriter.write(actionState);
				byteWriter.write(posX);
				byteWriter.write(posY);
				byteWriter.write(direction);
				byteWriter.write(attackSpeed);
			}
		});
	const llong bitWriteNS = measureMedianNS([]() {}, [&]()
		{
			FOR(BENCHMARK_MESSAGE_COUNT)
			{
				bitWriter.clear();
				bitWriter.writeSigned(true, guid0, guid1);
				bitWriter.writeUnsigned(maxHP, hp, monsterID + (uint)(i & 1023), actionState, posX, posY);
				bitWriter.writeUnsigned(direction);
				bitWriter.writeFloat(true, attackSpeed);
			}
		});
	byteWriter.clear();
	byteWriter.write(guid0);
	byteWriter.write(guid1);
	byteWriter.write(maxHP);
	byteWriter.write(hp);
	byteWriter.write(monsterID);
	byteWriter.write(actionState);
	byteWriter.write(posX);
	byteWriter.write(posY);
	byteWriter.write(direction);
	byteWriter.write(attackSpeed);
	bitWriter.clear();
	bitWriter.writeSigned(true, guid0, guid1);
	bitWriter.writeUnsigned(maxHP, hp, monsterID, actionState, posX, posY);
	bitWriter.writeUnsigned(direction);
	bitWriter.writeFloat(true, attackSpeed);
	volatile ullong readSink = 0;
	const llong byteReadNS = measureMedianNS([]() {}, [&]()
		{
			ullong sum = 0;
			FOR(BENCHMARK_MESSAGE_COUNT)
			{
				SerializerRead reader(byteWriter.getBuffer(), byteWriter.getDataSize());
				llong outGuid0 = 0;
				llong outGuid1 = 0;
				uint outMaxHP = 0;
				uint outHP = 0;
				uint outMonsterID = 0;
				uint outActionState = 0;
				uint outPosX = 0;
				uint outPosY = 0;
				uint outDirection = 0;
				float outAttackSpeed = 0.0f;
				reader.read(outGuid0);
				reader.read(outGuid1);
				reader.read(outMaxHP);
				reader.read(outHP);
				reader.read(outMonsterID);
				reader.read(outActionState);
				reader.read(outPosX);
				reader.read(outPosY);
				reader.read(outDirection);
				reader.read(outAttackSpeed);
				sum += (ullong)outGuid0 + (ullong)outGuid1 + outMaxHP + outHP + outMonsterID + outActionState + outPosX + outPosY + outDirection + (uint)outAttackSpeed;
			}
			readSink = sum;
		});
	const llong bitReadNS = measureMedianNS([]() {}, [&]()
		{
			ullong sum = 0;
			FOR(BENCHMARK_MESSAGE_COUNT)
			{
				SerializerBitRead reader(bitWriter.getBuffer(), bitWriter.getByteCount());
				llong outGuid0 = 0;
				llong outGuid1 = 0;
				uint outMaxHP = 0;
				uint outHP = 0;
				uint outMonsterID = 0;
				uint outActionState = 0;
				uint outPosX = 0;
				uint outPosY = 0;
				uint outDirection = 0;
				float outAttackSpeed = 0.0f;
				reader.readSigned(true, outGuid0, outGuid1);
				reader.readUnsigned(outMaxHP, outHP, outMonsterID, outActionState, outPosX, outPosY);
				reader.readUnsigned(outDirection);
				reader.readFloat(true, outAttackSpeed);
				sum += (ullong)outGuid0 + (ullong)outGuid1 + outMaxHP + outHP + outMonsterID + outActionState + outPosX + outPosY + outDirection + (uint)outAttackSpeed;
			}
			readSink = sum;
		});
	LOG("================ Serializer Numeric Message ================");
	LOG("MessageCount:" + IToS(BENCHMARK_MESSAGE_COUNT));
	LOG("Byte Write Median:" + formatMS(byteWriteNS) + " ms | " + FToS((float)byteWriteNS / BENCHMARK_MESSAGE_COUNT, 3) + " ns/message");
	LOG("Bit  Write Median:" + formatMS(bitWriteNS) + " ms | " + FToS((float)bitWriteNS / BENCHMARK_MESSAGE_COUNT, 3) + " ns/message | Bit/Byte:" + formatRatio(bitWriteNS, byteWriteNS) + "x");
	LOG("Byte Read  Median:" + formatMS(byteReadNS) + " ms | " + FToS((float)byteReadNS / BENCHMARK_MESSAGE_COUNT, 3) + " ns/message");
	LOG("Bit  Read  Median:" + formatMS(bitReadNS) + " ms | " + FToS((float)bitReadNS / BENCHMARK_MESSAGE_COUNT, 3) + " ns/message | Bit/Byte:" + formatRatio(bitReadNS, byteReadNS) + "x");
	LOG("Byte Size:" + IToS(byteSize) + " bytes | Bit Size:" + IToS(bitSize) + " bytes | Bit/Byte:" + FToS((float)bitSize / (float)byteSize * 100.0f, 2) + "%");
	(void)readSink;
}

template<typename T>
void BinaryUtilityTest::benchmarkSignedValues(const string& profileName, const vector<T>& values, const bool needSign)
{
	using namespace BinaryUtilitySerializeTestSupport;
	const int count = (int)values.size();
	vector<char> byteBuffer((size_t)count * sizeof(T));
	vector<char> bitBuffer((size_t)count * (sizeof(T) + 1) + 16, 0);
	int byteSize = 0;
	int bitCount = 0;
	for (int i = 0; i < count; ++i)
	{
		BinaryUtility::write(byteBuffer.data(), (int)byteBuffer.size(), byteSize, values[i]);
		BinaryUtility::writeSignedIntegerBit(bitBuffer.data(), (int)bitBuffer.size(), bitCount, values[i], needSign);
	}
	const int bitSize = BinaryUtility::bitCountToByteCount(bitCount);
	volatile ullong sink = 0;
	const llong byteWriteNS = measureMedianNS([]() {}, [&]() { int index = 0; for (int i = 0; i < count; ++i) { BinaryUtility::write(byteBuffer.data(), (int)byteBuffer.size(), index, values[i]); } });
	const llong bitWriteNS = measureMedianNS([&]() { memset(bitBuffer.data(), 0, bitSize); }, [&]() { int index = 0; for (int i = 0; i < count; ++i) { BinaryUtility::writeSignedIntegerBit(bitBuffer.data(), (int)bitBuffer.size(), index, values[i], needSign); } });
	const llong byteReadNS = measureMedianNS([]() {}, [&]() { int index = 0; ullong sum = 0; for (int i = 0; i < count; ++i) { T value = 0; BinaryUtility::read(byteBuffer.data(), byteSize, index, value); sum += (ullong)value; } sink = sum; });
	const llong bitReadNS = measureMedianNS([]() {}, [&]() { int index = 0; ullong sum = 0; for (int i = 0; i < count; ++i) { T value = 0; BinaryUtility::readSignedIntegerBit(bitBuffer.data(), bitSize, index, value, needSign); sum += (ullong)value; } sink = sum; });
	LOG("================ " + profileName + " ================");
	LOG("Byte Write Median:" + formatMS(byteWriteNS) + " ms | " + formatNSPerCount(byteWriteNS, count) + " ns/value");
	LOG("Bit  Write Median:" + formatMS(bitWriteNS) + " ms | " + formatNSPerCount(bitWriteNS, count) + " ns/value | Bit/Byte:" + formatRatio(bitWriteNS, byteWriteNS) + "x");
	LOG("Byte Read  Median:" + formatMS(byteReadNS) + " ms | " + formatNSPerCount(byteReadNS, count) + " ns/value");
	LOG("Bit  Read  Median:" + formatMS(bitReadNS) + " ms | " + formatNSPerCount(bitReadNS, count) + " ns/value | Bit/Byte:" + formatRatio(bitReadNS, byteReadNS) + "x");
	LOG("Byte Size:" + IToS(byteSize) + " bytes | Bit Size:" + IToS(bitSize) + " bytes | Bit/Byte:" + FToS(byteSize > 0 ? (float)bitSize / (float)byteSize * 100.0f : 0.0f, 2) + "%");
	(void)sink;
}

template<typename T>
void BinaryUtilityTest::benchmarkUnsignedValues(const string& profileName, const vector<T>& values)
{
	using namespace BinaryUtilitySerializeTestSupport;
	const int count = (int)values.size();
	vector<char> byteBuffer((size_t)count * sizeof(T));
	vector<char> bitBuffer((size_t)count * (sizeof(T) + 1) + 16, 0);
	int byteSize = 0;
	int bitCount = 0;
	for (int i = 0; i < count; ++i)
	{
		BinaryUtility::write(byteBuffer.data(), (int)byteBuffer.size(), byteSize, values[i]);
		BinaryUtility::writeUnsignedIntegerBit(bitBuffer.data(), (int)bitBuffer.size(), bitCount, values[i]);
	}
	const int bitSize = BinaryUtility::bitCountToByteCount(bitCount);
	volatile ullong sink = 0;
	const llong byteWriteNS = measureMedianNS([]() {}, [&]() { int index = 0; for (int i = 0; i < count; ++i) { BinaryUtility::write(byteBuffer.data(), (int)byteBuffer.size(), index, values[i]); } });
	const llong bitWriteNS = measureMedianNS([&]() { memset(bitBuffer.data(), 0, bitSize); }, [&]() { int index = 0; for (int i = 0; i < count; ++i) { BinaryUtility::writeUnsignedIntegerBit(bitBuffer.data(), (int)bitBuffer.size(), index, values[i]); } });
	const llong byteReadNS = measureMedianNS([]() {}, [&]() { int index = 0; ullong sum = 0; for (int i = 0; i < count; ++i) { T value = 0; BinaryUtility::read(byteBuffer.data(), byteSize, index, value); sum += (ullong)value; } sink = sum; });
	const llong bitReadNS = measureMedianNS([]() {}, [&]() { int index = 0; ullong sum = 0; for (int i = 0; i < count; ++i) { T value = 0; BinaryUtility::readUnsignedIntegerBit(bitBuffer.data(), bitSize, index, value); sum += (ullong)value; } sink = sum; });
	LOG("================ " + profileName + " ================");
	LOG("Byte Write Median:" + formatMS(byteWriteNS) + " ms | " + formatNSPerCount(byteWriteNS, count) + " ns/value");
	LOG("Bit  Write Median:" + formatMS(bitWriteNS) + " ms | " + formatNSPerCount(bitWriteNS, count) + " ns/value | Bit/Byte:" + formatRatio(bitWriteNS, byteWriteNS) + "x");
	LOG("Byte Read  Median:" + formatMS(byteReadNS) + " ms | " + formatNSPerCount(byteReadNS, count) + " ns/value");
	LOG("Bit  Read  Median:" + formatMS(bitReadNS) + " ms | " + formatNSPerCount(bitReadNS, count) + " ns/value | Bit/Byte:" + formatRatio(bitReadNS, byteReadNS) + "x");
	LOG("Byte Size:" + IToS(byteSize) + " bytes | Bit Size:" + IToS(bitSize) + " bytes | Bit/Byte:" + FToS(byteSize > 0 ? (float)bitSize / (float)byteSize * 100.0f : 0.0f, 2) + "%");
	(void)sink;
}

void BinaryUtilityTest::benchmarkBool()
{
	using namespace BinaryUtilitySerializeTestSupport;
	vector<bool> values(BENCHMARK_VALUE_COUNT);
	uint state = 0xABCDEF01u;
	FOR(BENCHMARK_VALUE_COUNT) { values[i] = (nextRandom(state) & 1u) != 0; }
	vector<char> byteBuffer((size_t)BENCHMARK_VALUE_COUNT * sizeof(bool));
	vector<char> bitBuffer((size_t)BinaryUtility::bitCountToByteCount(BENCHMARK_VALUE_COUNT) + 16, 0);
	int byteSize = 0;
	int bitCount = 0;
	FOR(BENCHMARK_VALUE_COUNT) { BinaryUtility::write(byteBuffer.data(), (int)byteBuffer.size(), byteSize, (bool)values[i]); BinaryUtility::writeBoolBit(bitBuffer.data(), (int)bitBuffer.size(), bitCount, values[i]); }
	const int bitSize = BinaryUtility::bitCountToByteCount(bitCount);
	volatile ullong sink = 0;
	const llong byteWriteNS = measureMedianNS([]() {}, [&]() { int index = 0; FOR(BENCHMARK_VALUE_COUNT) { BinaryUtility::write(byteBuffer.data(), (int)byteBuffer.size(), index, (bool)values[i]); } });
	const llong bitWriteNS = measureMedianNS([&]() { memset(bitBuffer.data(), 0, bitSize); }, [&]() { int index = 0; FOR(BENCHMARK_VALUE_COUNT) { BinaryUtility::writeBoolBit(bitBuffer.data(), (int)bitBuffer.size(), index, values[i]); } });
	const llong byteReadNS = measureMedianNS([]() {}, [&]() { int index = 0; ullong sum = 0; FOR(BENCHMARK_VALUE_COUNT) { bool value = false; BinaryUtility::read(byteBuffer.data(), byteSize, index, value); sum += value ? 1 : 0; } sink = sum; });
	const llong bitReadNS = measureMedianNS([]() {}, [&]() { int index = 0; ullong sum = 0; FOR(BENCHMARK_VALUE_COUNT) { bool value = false; BinaryUtility::readBoolBit(bitBuffer.data(), bitSize, index, value); sum += value ? 1 : 0; } sink = sum; });
	logBenchmarkResult("Bool Random", byteWriteNS, bitWriteNS, byteReadNS, bitReadNS, byteSize, bitSize);
	(void)sink;
}

void BinaryUtilityTest::benchmarkExtendedIntegerTypes()
{
	using namespace BinaryUtilitySerializeTestSupport;
	vector<char> charValues(BENCHMARK_VALUE_COUNT);
	vector<short> shortValues(BENCHMARK_VALUE_COUNT);
	vector<int> intPositive(BENCHMARK_VALUE_COUNT);
	vector<llong> llongSmall(BENCHMARK_VALUE_COUNT);
	vector<llong> llongFull(BENCHMARK_VALUE_COUNT);
	vector<ullong> ullongSmall(BENCHMARK_VALUE_COUNT);
	vector<ullong> ullongFull(BENCHMARK_VALUE_COUNT);
	uint state = 0x31415926u;
	FOR(BENCHMARK_VALUE_COUNT)
	{
		charValues[i] = (char)(nextRandom(state) & 0x7Fu);
		if ((nextRandom(state) & 1u) != 0) { charValues[i] = (char)-charValues[i]; }
		shortValues[i] = (short)(nextRandom(state) & 0xFFFFu);
		intPositive[i] = (int)(nextRandom(state) & 0xFFFFu);
		ullong raw = ((ullong)nextRandom(state) << 32) | nextRandom(state);
		llongSmall[i] = (llong)(raw % 131071ULL) - 65535LL;
		llong value = (llong)(raw & 0x7FFFFFFFFFFFFFFFULL);
		llongFull[i] = (raw & 0x8000000000000000ULL) != 0 ? -value : value;
		raw = ((ullong)nextRandom(state) << 32) | nextRandom(state);
		ullongSmall[i] = raw & 0xFFFFULL;
		ullongFull[i] = raw;
	}
	benchmarkSignedValues("Signed Char FullRange", charValues);
	benchmarkSignedValues("Signed Short FullRange", shortValues);
	benchmarkSignedValues("Signed Int Positive[0,65535] WithSign", intPositive, true);
	benchmarkSignedValues("Signed Int Positive[0,65535] NoSign", intPositive, false);
	benchmarkSignedValues("Signed LLong Small[-65535,65535]", llongSmall);
	benchmarkSignedValues("Signed LLong FullRange", llongFull);
	benchmarkUnsignedValues("Unsigned ULLong Small[0,65535]", ullongSmall);
	benchmarkUnsignedValues("Unsigned ULLong FullRange", ullongFull);
}

void BinaryUtilityTest::benchmarkLLongFastPathBoundary()
{
	using namespace BinaryUtilitySerializeTestSupport;
	LOG("================ 64Bit FastPath Boundary Sweep ================");
	vector<llong> signedFast(BENCHMARK_VALUE_COUNT, 0x0200000000000000LL);
	vector<llong> signedFallback(BENCHMARK_VALUE_COUNT, 0x0400000000000000LL);
	vector<ullong> unsignedFast(BENCHMARK_VALUE_COUNT, 0x0400000000000000ULL);
	vector<ullong> unsignedFallback(BENCHMARK_VALUE_COUNT, 0x0800000000000000ULL);
	benchmarkSignedValues("Signed LLong FastPath Last[2^57]", signedFast);
	benchmarkSignedValues("Signed LLong Fallback First[2^58]", signedFallback);
	benchmarkUnsignedValues("Unsigned ULLong FastPath Last[2^58]", unsignedFast);
	benchmarkUnsignedValues("Unsigned ULLong Fallback First[2^59]", unsignedFallback);
}

void BinaryUtilityTest::benchmarkUIntBoundarySweep()
{
	using namespace BinaryUtilitySerializeTestSupport;
	static const uint testValues[] = { 0u, 1u, 127u, 128u, 32767u, 32768u, UINT_MAX };
	static const char* names[] = { "0", "1", "127", "128", "32767", "32768", "UINT_MAX" };
	LOG("================ UInt Boundary Sweep ================");
	for (int k = 0; k < (int)(sizeof(testValues) / sizeof(testValues[0])); ++k)
	{
		vector<uint> values(BENCHMARK_VALUE_COUNT, testValues[k]);
		benchmarkUnsignedValues("UInt Boundary Value " + string(names[k]), values);
	}
}

void BinaryUtilityTest::benchmarkUIntWriteCacheRange(const string& rangeName, const uint valueMask, const vector<uint>& cacheTable)
{
	using namespace BinaryUtilitySerializeTestSupport;
	Vector<uint> values;
	values.resize(BENCHMARK_VALUE_COUNT);
	uint randomState = 0x13579BDFu ^ valueMask;
	for (int i = 0; i < BENCHMARK_VALUE_COUNT; ++i)
	{
		values[i] = nextRandom(randomState) & valueMask;
	}
	vector<char> formulaBuffer((size_t)BENCHMARK_VALUE_COUNT * 5 + 32, 0);
	vector<char> cacheBuffer((size_t)BENCHMARK_VALUE_COUNT * 5 + 32, 0);
	volatile ullong sink = 0;
	const llong formulaNS = measureMedianNS([&]() { memset(formulaBuffer.data(), 0, formulaBuffer.size()); }, [&]()
		{
			int bitIndex = 3;
			for (int i = 0; i < BENCHMARK_VALUE_COUNT; ++i)
			{
				writeUnsignedIntFormulaReference(formulaBuffer.data(), (int)formulaBuffer.size(), bitIndex, values[i]);
			}
			const int byteCount = BinaryUtility::bitCountToByteCount(bitIndex);
			sink = (ullong)bitIndex + (byte)formulaBuffer[0] + (byte)formulaBuffer[byteCount >> 1] + (byte)formulaBuffer[byteCount - 1];
		});
	const llong cacheNS = measureMedianNS([&]() { memset(cacheBuffer.data(), 0, cacheBuffer.size()); }, [&]()
		{
			int bitIndex = 3;
			for (int i = 0; i < BENCHMARK_VALUE_COUNT; ++i)
			{
				BinaryUtility::writeEncodedUIntCacheBit(cacheBuffer.data(), (int)cacheBuffer.size(), bitIndex, cacheTable[values[i]]);
			}
			const int byteCount = BinaryUtility::bitCountToByteCount(bitIndex);
			sink = (ullong)bitIndex + (byte)cacheBuffer[0] + (byte)cacheBuffer[byteCount >> 1] + (byte)cacheBuffer[byteCount - 1];
		});
	LOG(rangeName + " Formula:" + formatMS(formulaNS) + " ms | " + formatNSPerValue(formulaNS) + " ns/value");
	LOG(rangeName + " Cache  :" + formatMS(cacheNS) + " ms | " + formatNSPerValue(cacheNS) + " ns/value | Formula/Cache:" + formatRatio(formulaNS, cacheNS) + "x");
	if (valueMask < BinaryUtility::UINT_WRITE_CACHE_PRODUCTION_COUNT)
	{
		vector<char> cache32Buffer((size_t)BENCHMARK_VALUE_COUNT * 5 + 32, 0);
		const llong cache32NS = measureMedianNS([&]() { memset(cache32Buffer.data(), 0, cache32Buffer.size()); }, [&]()
			{
				int bitIndex = 3;
				for (int i = 0; i < BENCHMARK_VALUE_COUNT; ++i)
				{
					BinaryUtility::writeEncodedUIntCache32Bit(cache32Buffer.data(), (int)cache32Buffer.size(), bitIndex, cacheTable[values[i]]);
				}
				const int byteCount = BinaryUtility::bitCountToByteCount(bitIndex);
				sink = (ullong)bitIndex + (byte)cache32Buffer[0] + (byte)cache32Buffer[byteCount >> 1] + (byte)cache32Buffer[byteCount - 1];
			});
		LOG(rangeName + " Cache32:" + formatMS(cache32NS) + " ms | " + formatNSPerValue(cache32NS) + " ns/value | Cache/Cache32:" + formatRatio(cacheNS, cache32NS) + "x");
		vector<char> productionBuffer((size_t)BENCHMARK_VALUE_COUNT * 5 + 32, 0);
		const llong productionNS = measureMedianNS([&]() { memset(productionBuffer.data(), 0, productionBuffer.size()); }, [&]()
			{
				int bitIndex = 3;
				for (int i = 0; i < BENCHMARK_VALUE_COUNT; ++i)
				{
					BinaryUtility::writeUnsignedIntegerBit(productionBuffer.data(), (int)productionBuffer.size(), bitIndex, values[i]);
				}
				const int byteCount = BinaryUtility::bitCountToByteCount(bitIndex);
				sink = (ullong)bitIndex + (byte)productionBuffer[0] + (byte)productionBuffer[byteCount >> 1] + (byte)productionBuffer[byteCount - 1];
			});
		LOG(rangeName + " Product:" + formatMS(productionNS) + " ms | " + formatNSPerValue(productionNS) + " ns/value | Formula/Product:" + formatRatio(formulaNS, productionNS) + "x");
	}
	(void)sink;
}

void BinaryUtilityTest::benchmarkUIntWriteCache()
{
	using namespace BinaryUtilitySerializeTestSupport;
	BinaryUtility::initUIntWriteCache();
	vector<uint> cacheTable(BinaryUtility::UINT_WRITE_CACHE_COUNT_4M);
	for (uint value = 0; value < BinaryUtility::UINT_WRITE_CACHE_COUNT_4M; ++value)
	{
		cacheTable[value] = BinaryUtility::encodeUIntWriteCacheValue(value);
	}
	LOG("================ UInt Write Encoded Cache Range Sweep ================");
	LOG("EntryBytes:4 ProductionBytes:" + IToS((int)(BinaryUtility::UINT_WRITE_CACHE_PRODUCTION_COUNT * sizeof(uint))) + " ProductionCount:" + IToS((int)BinaryUtility::UINT_WRITE_CACHE_PRODUCTION_COUNT));
	benchmarkUIntWriteCacheRange("64K ", BinaryUtility::UINT_WRITE_CACHE_COUNT_64K - 1u, cacheTable);
	benchmarkUIntWriteCacheRange("256K", BinaryUtility::UINT_WRITE_CACHE_COUNT_256K - 1u, cacheTable);
	benchmarkUIntWriteCacheRange("1M  ", BinaryUtility::UINT_WRITE_CACHE_COUNT_1M - 1u, cacheTable);
	benchmarkUIntWriteCacheRange("4M  ", BinaryUtility::UINT_WRITE_CACHE_COUNT_4M - 1u, cacheTable);
}

void BinaryUtilityTest::benchmarkUnsignedOffset(const int startBit)
{
	using namespace BinaryUtilitySerializeTestSupport;
	Vector<uint> values;
	fillUnsignedValues(values, 1);
	vector<char> byteBuffer((size_t)BENCHMARK_VALUE_COUNT * sizeof(uint));
	vector<char> bitBuffer((size_t)BENCHMARK_VALUE_COUNT * 5 + 32, 0);
	int byteSize = 0;
	FOR(BENCHMARK_VALUE_COUNT) { BinaryUtility::write(byteBuffer.data(), (int)byteBuffer.size(), byteSize, values[i]); }
	int bitCount = startBit;
	FOR(BENCHMARK_VALUE_COUNT) { BinaryUtility::writeUnsignedIntegerBit(bitBuffer.data(), (int)bitBuffer.size(), bitCount, values[i]); }
	const int bitSize = BinaryUtility::bitCountToByteCount(bitCount);
	volatile ullong sink = 0;
	const llong byteWriteNS = measureMedianNS([]() {}, [&]() { int index = 0; FOR(BENCHMARK_VALUE_COUNT) { BinaryUtility::write(byteBuffer.data(), (int)byteBuffer.size(), index, values[i]); } });
	const llong bitWriteNS = measureMedianNS([&]() { memset(bitBuffer.data(), 0, bitSize); }, [&]() { int index = startBit; FOR(BENCHMARK_VALUE_COUNT) { BinaryUtility::writeUnsignedIntegerBit(bitBuffer.data(), (int)bitBuffer.size(), index, values[i]); } });
	const llong byteReadNS = measureMedianNS([]() {}, [&]() { int index = 0; ullong sum = 0; FOR(BENCHMARK_VALUE_COUNT) { uint value = 0; BinaryUtility::read(byteBuffer.data(), byteSize, index, value); sum += value; } sink = sum; });
	const llong bitReadNS = measureMedianNS([]() {}, [&]() { int index = startBit; ullong sum = 0; FOR(BENCHMARK_VALUE_COUNT) { uint value = 0; BinaryUtility::readUnsignedIntegerBit(bitBuffer.data(), bitSize, index, value); sum += value; } sink = sum; });
	logBenchmarkResult("Unsigned Int Medium Offset" + IToS(startBit), byteWriteNS, bitWriteNS, byteReadNS, bitReadNS, byteSize, bitSize);
	(void)sink;
}

void BinaryUtilityTest::benchmarkBitOffsetSweep()
{
	using namespace BinaryUtilitySerializeTestSupport;
	LOG("================ Bit Offset Sweep ================");
	benchmarkUnsignedOffset(0);
	benchmarkUnsignedOffset(1);
	benchmarkUnsignedOffset(3);
	benchmarkUnsignedOffset(7);
}

void BinaryUtilityTest::benchmarkUIntList6(const string& profileName, const Array<6, uint>& values)
{
	using namespace BinaryUtilitySerializeTestSupport;
	SerializerWrite byteWriter;
	byteWriter.initCapacity(128);
	FOR(6) { byteWriter.write(values[i]); }
	SerializerBitWrite bitWriter;
	bitWriter.initCapacity(128);
	bitWriter.writeUnsigned<6, uint>(values);
	const int byteSize = byteWriter.getDataSize();
	const int bitSize = bitWriter.getByteCount();
	volatile ullong sink = 0;
	const llong byteWriteNS = measureMedianNS([]() {}, [&]() { FOR(BENCHMARK_MESSAGE_COUNT) { byteWriter.clear(); FOR_J(6) { byteWriter.write(values[j]); } } });
	const llong bitWriteNS = measureMedianNS([]() {}, [&]() { FOR(BENCHMARK_MESSAGE_COUNT) { bitWriter.clear(); bitWriter.writeUnsigned<6, uint>(values); } });
	const llong byteReadNS = measureMedianNS([]() {}, [&]() { ullong sum = 0; FOR(BENCHMARK_MESSAGE_COUNT) { SerializerRead reader(byteWriter.getBuffer(), byteWriter.getDataSize()); FOR_J(6) { uint value = 0; reader.read(value); sum += value; } } sink = sum; });
	const llong bitReadNS = measureMedianNS([]() {}, [&]() { ullong sum = 0; FOR(BENCHMARK_MESSAGE_COUNT) { SerializerBitRead reader(bitWriter.getBuffer(), bitWriter.getByteCount()); uint a = 0, b = 0, c = 0, d = 0, e = 0, f = 0; reader.readUnsigned(a, b, c, d, e, f); sum += a + b + c + d + e + f; } sink = sum; });
	LOG("================ " + profileName + " ================");
	LOG("MessageCount:" + IToS(BENCHMARK_MESSAGE_COUNT) + " FieldCount:6");
	LOG("Byte Write Median:" + formatMS(byteWriteNS) + " ms | " + formatNSPerCount(byteWriteNS, BENCHMARK_MESSAGE_COUNT) + " ns/message");
	LOG("Bit  Write Median:" + formatMS(bitWriteNS) + " ms | " + formatNSPerCount(bitWriteNS, BENCHMARK_MESSAGE_COUNT) + " ns/message | Bit/Byte:" + formatRatio(bitWriteNS, byteWriteNS) + "x");
	LOG("Byte Read  Median:" + formatMS(byteReadNS) + " ms | " + formatNSPerCount(byteReadNS, BENCHMARK_MESSAGE_COUNT) + " ns/message");
	LOG("Bit  Read  Median:" + formatMS(bitReadNS) + " ms | " + formatNSPerCount(bitReadNS, BENCHMARK_MESSAGE_COUNT) + " ns/message | Bit/Byte:" + formatRatio(bitReadNS, byteReadNS) + "x");
	LOG("Byte Size:" + IToS(byteSize) + " bytes | Bit Size:" + IToS(bitSize) + " bytes | Bit/Byte:" + FToS((float)bitSize / (float)byteSize * 100.0f, 2) + "%");
	(void)sink;
}

void BinaryUtilityTest::benchmarkListModes()
{
	using namespace BinaryUtilitySerializeTestSupport;
	const Array<6, uint> uniformValues{ 8000u, 7234u, 10023u, 5u, 1234u, 5678u };
	const Array<6, uint> independentValues{ 0u, 1u, 2u, 3u, 0x40000000u, 0u };
	benchmarkUIntList6("Serializer UInt List UniformLength", uniformValues);
	benchmarkUIntList6("Serializer UInt List IndependentLength", independentValues);
}

void BinaryUtilityTest::benchmarkDynamicUnsignedList()
{
	using namespace BinaryUtilitySerializeTestSupport;
	Vector<uint> values;
	const uint source[] = { 0u, 1u, 2u, 3u, 15u, 31u, 255u, 1024u, 8000u, 10023u, 65535u, 100000u };
	for (uint value : source) { values.add(value); }
	SerializerWrite byteWriter;
	byteWriter.initCapacity(256);
	byteWriter.writeList(values);
	SerializerBitWrite bitWriter;
	bitWriter.initCapacity(256);
	bitWriter.writeUnsignedList(values);
	const int byteSize = byteWriter.getDataSize();
	const int bitSize = bitWriter.getByteCount();
	volatile ullong sink = 0;
	const llong byteWriteNS = measureMedianNS([]() {}, [&]() { FOR(BENCHMARK_MESSAGE_COUNT) { byteWriter.clear(); byteWriter.writeList(values); } });
	const llong bitWriteNS = measureMedianNS([]() {}, [&]() { FOR(BENCHMARK_MESSAGE_COUNT) { bitWriter.clear(); bitWriter.writeUnsignedList(values); } });
	Vector<uint> byteOutput;
	Vector<uint> bitOutput;
	byteOutput.reserve(values.size());
	bitOutput.reserve(values.size());
	const llong byteReadNS = measureMedianNS([]() {}, [&]() { ullong sum = 0; FOR(BENCHMARK_MESSAGE_COUNT) { SerializerRead reader(byteWriter.getBuffer(), byteWriter.getDataSize()); reader.readList(byteOutput); FOR_J(byteOutput.size()) { sum += byteOutput[j]; } } sink = sum; });
	const llong bitReadNS = measureMedianNS([]() {}, [&]() { ullong sum = 0; FOR(BENCHMARK_MESSAGE_COUNT) { SerializerBitRead reader(bitWriter.getBuffer(), bitWriter.getByteCount()); reader.readUnsignedList(bitOutput); FOR_J(bitOutput.size()) { sum += bitOutput[j]; } } sink = sum; });
	LOG("================ Serializer Dynamic UInt List ================");
	LOG("MessageCount:" + IToS(BENCHMARK_MESSAGE_COUNT) + " FieldCount:" + IToS(values.size()));
	LOG("Byte Write Median:" + formatMS(byteWriteNS) + " ms | " + formatNSPerCount(byteWriteNS, BENCHMARK_MESSAGE_COUNT) + " ns/message");
	LOG("Bit  Write Median:" + formatMS(bitWriteNS) + " ms | " + formatNSPerCount(bitWriteNS, BENCHMARK_MESSAGE_COUNT) + " ns/message | Bit/Byte:" + formatRatio(bitWriteNS, byteWriteNS) + "x");
	LOG("Byte Read  Median:" + formatMS(byteReadNS) + " ms | " + formatNSPerCount(byteReadNS, BENCHMARK_MESSAGE_COUNT) + " ns/message");
	LOG("Bit  Read  Median:" + formatMS(bitReadNS) + " ms | " + formatNSPerCount(bitReadNS, BENCHMARK_MESSAGE_COUNT) + " ns/message | Bit/Byte:" + formatRatio(bitReadNS, byteReadNS) + "x");
	LOG("Byte Size:" + IToS(byteSize) + " bytes | Bit Size:" + IToS(bitSize) + " bytes | Bit/Byte:" + FToS((float)bitSize / (float)byteSize * 100.0f, 2) + "%");
	(void)sink;
}

void BinaryUtilityTest::benchmarkSerializerTinyMessage()
{
	using namespace BinaryUtilitySerializeTestSupport;
	SerializerWrite byteWriter;
	byteWriter.initCapacity(32);
	byteWriter.write((uint)3);
	byteWriter.write(true);
	SerializerBitWrite bitWriter;
	bitWriter.initCapacity(32);
	bitWriter.writeUnsigned((uint)3);
	bitWriter.writeBool(true);
	const int byteSize = byteWriter.getDataSize();
	const int bitSize = bitWriter.getByteCount();
	volatile ullong sink = 0;
	const llong byteWriteNS = measureMedianNS([]() {}, [&]() { FOR(BENCHMARK_MESSAGE_COUNT) { byteWriter.clear(); byteWriter.write((uint)3); byteWriter.write(true); } });
	const llong bitWriteNS = measureMedianNS([]() {}, [&]() { FOR(BENCHMARK_MESSAGE_COUNT) { bitWriter.clear(); bitWriter.writeUnsigned((uint)3); bitWriter.writeBool(true); } });
	const llong byteReadNS = measureMedianNS([]() {}, [&]() { ullong sum = 0; FOR(BENCHMARK_MESSAGE_COUNT) { SerializerRead reader(byteWriter.getBuffer(), byteWriter.getDataSize()); uint state = 0; bool active = false; reader.read(state); reader.read(active); sum += state + (active ? 1 : 0); } sink = sum; });
	const llong bitReadNS = measureMedianNS([]() {}, [&]() { ullong sum = 0; FOR(BENCHMARK_MESSAGE_COUNT) { SerializerBitRead reader(bitWriter.getBuffer(), bitWriter.getByteCount()); uint state = 0; bool active = false; reader.readUnsigned(state); reader.readBool(active); sum += state + (active ? 1 : 0); } sink = sum; });
	LOG("================ Serializer Tiny Message ================");
	LOG("Byte Write Median:" + formatMS(byteWriteNS) + " ms | " + formatNSPerCount(byteWriteNS, BENCHMARK_MESSAGE_COUNT) + " ns/message");
	LOG("Bit  Write Median:" + formatMS(bitWriteNS) + " ms | " + formatNSPerCount(bitWriteNS, BENCHMARK_MESSAGE_COUNT) + " ns/message | Bit/Byte:" + formatRatio(bitWriteNS, byteWriteNS) + "x");
	LOG("Byte Read  Median:" + formatMS(byteReadNS) + " ms | " + formatNSPerCount(byteReadNS, BENCHMARK_MESSAGE_COUNT) + " ns/message");
	LOG("Bit  Read  Median:" + formatMS(bitReadNS) + " ms | " + formatNSPerCount(bitReadNS, BENCHMARK_MESSAGE_COUNT) + " ns/message | Bit/Byte:" + formatRatio(bitReadNS, byteReadNS) + "x");
	LOG("Byte Size:" + IToS(byteSize) + " bytes | Bit Size:" + IToS(bitSize) + " bytes | Bit/Byte:" + FToS((float)bitSize / (float)byteSize * 100.0f, 2) + "%");
	(void)sink;
}

void BinaryUtilityTest::benchmarkFloatIndependentCachedBitCount()
{
	using namespace BinaryUtilitySerializeTestSupport;
	const float values[6]{ 0.125f, 1.25f, 12.345f, -0.75f, -23.456f, 100.001f };
	char referenceBuffer[128]{};
	char productionBuffer[128]{};
	volatile int sink = 0;
	const llong referenceNS = measureMedianNS([]() {}, [&]()
		{
			int sum = 0;
			FOR(BENCHMARK_MESSAGE_COUNT)
			{
				memset(referenceBuffer, 0, sizeof(referenceBuffer));
				int bitIndex = 0;
				writeFloatListBitV23Reference(referenceBuffer, (int)sizeof(referenceBuffer), bitIndex, values, 6, true, 3);
				sum += bitIndex;
			}
			sink = sum;
		});
	const llong productionNS = measureMedianNS([]() {}, [&]()
		{
			int sum = 0;
			FOR(BENCHMARK_MESSAGE_COUNT)
			{
				memset(productionBuffer, 0, sizeof(productionBuffer));
				int bitIndex = 0;
				BinaryUtility::writeFloatListBit(productionBuffer, (int)sizeof(productionBuffer), bitIndex, values, 6, false, true, 3);
				sum += bitIndex;
			}
			sink = sum;
		});
	LOG("================ Float Independent Cached BitCount ================");
	LOG("V23 Reference:" + formatMS(referenceNS) + " ms | " + formatNSPerCount(referenceNS, BENCHMARK_MESSAGE_COUNT) + " ns/message");
	LOG("V28 Production:" + formatMS(productionNS) + " ms | " + formatNSPerCount(productionNS, BENCHMARK_MESSAGE_COUNT) + " ns/message | Reference/Production:" + formatRatio(referenceNS, productionNS) + "x");
	(void)sink;
}

void BinaryUtilityTest::benchmarkFloatUniformV28()
{
	using namespace BinaryUtilitySerializeTestSupport;
	const float values[6]{ 8.0f, 7.234f, 10.023f, -5.0f, 1.234f, -5.678f };
	char referenceBuffer[128]{};
	char productionBuffer[128]{};
	volatile int sink = 0;
	const llong referenceNS = measureMedianNS([]() {}, [&]()
		{
			int sum = 0;
			FOR(BENCHMARK_MESSAGE_COUNT)
			{
				memset(referenceBuffer, 0, sizeof(referenceBuffer));
				int bitIndex = 0;
				writeFloatListBitV23Reference(referenceBuffer, (int)sizeof(referenceBuffer), bitIndex, values, 6, true, 3);
				sum += bitIndex;
			}
			sink = sum;
		});
	const llong productionNS = measureMedianNS([]() {}, [&]()
		{
			int sum = 0;
			FOR(BENCHMARK_MESSAGE_COUNT)
			{
				memset(productionBuffer, 0, sizeof(productionBuffer));
				int bitIndex = 0;
				BinaryUtility::writeFloatListBit(productionBuffer, (int)sizeof(productionBuffer), bitIndex, values, 6, false, true, 3);
				sum += bitIndex;
			}
			sink = sum;
		});
	LOG("================ Float Uniform V23 vs V28 ================");
	LOG("V23 Reference:" + formatMS(referenceNS) + " ms | " + formatNSPerCount(referenceNS, BENCHMARK_MESSAGE_COUNT) + " ns/message");
	LOG("V28 Production:" + formatMS(productionNS) + " ms | " + formatNSPerCount(productionNS, BENCHMARK_MESSAGE_COUNT) + " ns/message | Reference/Production:" + formatRatio(referenceNS, productionNS) + "x");
	(void)sink;
}

void BinaryUtilityTest::benchmarkSerializerFloatMessage()
{
	using namespace BinaryUtilitySerializeTestSupport;
	const float values[6]{ 0.125f, 1.25f, 12.345f, -0.75f, -23.456f, 100.001f };
	SerializerWrite byteWriter;
	byteWriter.initCapacity(128);
	FOR(6) { byteWriter.write(values[i]); }
	SerializerBitWrite bitWriter;
	bitWriter.initCapacity(128);
	bitWriter.writeFloat(true, values[0], values[1], values[2], values[3], values[4], values[5]);
	const int byteSize = byteWriter.getDataSize();
	const int bitSize = bitWriter.getByteCount();
	volatile llong sink = 0;
	const llong byteWriteNS = measureMedianNS([]() {}, [&]() { FOR(BENCHMARK_MESSAGE_COUNT) { byteWriter.clear(); FOR_J(6) { byteWriter.write(values[j]); } } });
	const llong bitWriteNS = measureMedianNS([]() {}, [&]() { FOR(BENCHMARK_MESSAGE_COUNT) { bitWriter.clear(); bitWriter.writeFloat(true, values[0], values[1], values[2], values[3], values[4], values[5]); } });
	const llong byteReadNS = measureMedianNS([]() {}, [&]() { llong sum = 0; FOR(BENCHMARK_MESSAGE_COUNT) { SerializerRead reader(byteWriter.getBuffer(), byteWriter.getDataSize()); FOR_J(6) { float value = 0.0f; reader.read(value); sum += (llong)(value * 1000.0f); } } sink = sum; });
	const llong bitReadNS = measureMedianNS([]() {}, [&]() { llong sum = 0; FOR(BENCHMARK_MESSAGE_COUNT) { SerializerBitRead reader(bitWriter.getBuffer(), bitWriter.getByteCount()); float a = 0, b = 0, c = 0, d = 0, e = 0, f = 0; reader.readFloat(true, a, b, c, d, e, f); sum += (llong)(a * 1000.0f) + (llong)(b * 1000.0f) + (llong)(c * 1000.0f) + (llong)(d * 1000.0f) + (llong)(e * 1000.0f) + (llong)(f * 1000.0f); } sink = sum; });
	LOG("================ Serializer Float Message ================");
	LOG("Byte Write Median:" + formatMS(byteWriteNS) + " ms | " + formatNSPerCount(byteWriteNS, BENCHMARK_MESSAGE_COUNT) + " ns/message");
	LOG("Bit  Write Median:" + formatMS(bitWriteNS) + " ms | " + formatNSPerCount(bitWriteNS, BENCHMARK_MESSAGE_COUNT) + " ns/message | Bit/Byte:" + formatRatio(bitWriteNS, byteWriteNS) + "x");
	LOG("Byte Read  Median:" + formatMS(byteReadNS) + " ms | " + formatNSPerCount(byteReadNS, BENCHMARK_MESSAGE_COUNT) + " ns/message");
	LOG("Bit  Read  Median:" + formatMS(bitReadNS) + " ms | " + formatNSPerCount(bitReadNS, BENCHMARK_MESSAGE_COUNT) + " ns/message | Bit/Byte:" + formatRatio(bitReadNS, byteReadNS) + "x");
	LOG("Byte Size:" + IToS(byteSize) + " bytes | Bit Size:" + IToS(bitSize) + " bytes | Bit/Byte:" + FToS((float)bitSize / (float)byteSize * 100.0f, 2) + "%");
	(void)sink;
}

void BinaryUtilityTest::benchmarkSerializerFloatUniformMessage()
{
	using namespace BinaryUtilitySerializeTestSupport;
	const float values[6]{ 8.0f, 7.234f, 10.023f, -5.0f, 1.234f, -5.678f };
	SerializerWrite byteWriter;
	byteWriter.initCapacity(128);
	FOR(6) { byteWriter.write(values[i]); }
	SerializerBitWrite bitWriter;
	bitWriter.initCapacity(128);
	bitWriter.writeFloat(true, values[0], values[1], values[2], values[3], values[4], values[5]);
	const int byteSize = byteWriter.getDataSize();
	const int bitSize = bitWriter.getByteCount();
	volatile llong sink = 0;
	const llong byteWriteNS = measureMedianNS([]() {}, [&]() { FOR(BENCHMARK_MESSAGE_COUNT) { byteWriter.clear(); FOR_J(6) { byteWriter.write(values[j]); } } });
	const llong bitWriteNS = measureMedianNS([]() {}, [&]() { FOR(BENCHMARK_MESSAGE_COUNT) { bitWriter.clear(); bitWriter.writeFloat(true, values[0], values[1], values[2], values[3], values[4], values[5]); } });
	const llong byteReadNS = measureMedianNS([]() {}, [&]() { llong sum = 0; FOR(BENCHMARK_MESSAGE_COUNT) { SerializerRead reader(byteWriter.getBuffer(), byteWriter.getDataSize()); FOR_J(6) { float value = 0.0f; reader.read(value); sum += (llong)(value * 1000.0f); } } sink = sum; });
	const llong bitReadNS = measureMedianNS([]() {}, [&]() { llong sum = 0; FOR(BENCHMARK_MESSAGE_COUNT) { SerializerBitRead reader(bitWriter.getBuffer(), bitWriter.getByteCount()); float a = 0, b = 0, c = 0, d = 0, e = 0, f = 0; reader.readFloat(true, a, b, c, d, e, f); sum += (llong)(a * 1000.0f) + (llong)(b * 1000.0f) + (llong)(c * 1000.0f) + (llong)(d * 1000.0f) + (llong)(e * 1000.0f) + (llong)(f * 1000.0f); } sink = sum; });
	LOG("================ Serializer Float UniformLength ================");
	LOG("Byte Write Median:" + formatMS(byteWriteNS) + " ms | " + formatNSPerCount(byteWriteNS, BENCHMARK_MESSAGE_COUNT) + " ns/message");
	LOG("Bit  Write Median:" + formatMS(bitWriteNS) + " ms | " + formatNSPerCount(bitWriteNS, BENCHMARK_MESSAGE_COUNT) + " ns/message | Bit/Byte:" + formatRatio(bitWriteNS, byteWriteNS) + "x");
	LOG("Byte Read  Median:" + formatMS(byteReadNS) + " ms | " + formatNSPerCount(byteReadNS, BENCHMARK_MESSAGE_COUNT) + " ns/message");
	LOG("Bit  Read  Median:" + formatMS(bitReadNS) + " ms | " + formatNSPerCount(bitReadNS, BENCHMARK_MESSAGE_COUNT) + " ns/message | Bit/Byte:" + formatRatio(bitReadNS, byteReadNS) + "x");
	LOG("Byte Size:" + IToS(byteSize) + " bytes | Bit Size:" + IToS(bitSize) + " bytes | Bit/Byte:" + FToS((float)bitSize / (float)byteSize * 100.0f, 2) + "%");
	(void)sink;
}

void BinaryUtilityTest::benchmarkSerializerStringMessage()
{
	using namespace BinaryUtilitySerializeTestSupport;
	const string value = "BinaryUtility_Message_String_0123456789";
	SerializerWrite byteWriter;
	byteWriter.initCapacity(128);
	byteWriter.writeString(value);
	SerializerBitWrite bitWriter;
	bitWriter.initCapacity(128);
	bitWriter.writeString(value);
	const int byteSize = byteWriter.getDataSize();
	const int bitSize = bitWriter.getByteCount();
	volatile ullong sink = 0;
	const llong byteWriteNS = measureMedianNS([]() {}, [&]() { FOR(BENCHMARK_MESSAGE_COUNT) { byteWriter.clear(); byteWriter.writeString(value); } });
	const llong bitWriteNS = measureMedianNS([]() {}, [&]() { FOR(BENCHMARK_MESSAGE_COUNT) { bitWriter.clear(); bitWriter.writeString(value); } });
	string byteOutput;
	string bitOutput;
	byteOutput.reserve(value.length());
	bitOutput.reserve(value.length());
	const llong byteReadNS = measureMedianNS([]() {}, [&]() { ullong sum = 0; FOR(BENCHMARK_MESSAGE_COUNT) { SerializerRead reader(byteWriter.getBuffer(), byteWriter.getDataSize()); reader.readString(byteOutput); sum += byteOutput.length(); } sink = sum; });
	const llong bitReadNS = measureMedianNS([]() {}, [&]() { ullong sum = 0; FOR(BENCHMARK_MESSAGE_COUNT) { SerializerBitRead reader(bitWriter.getBuffer(), bitWriter.getByteCount()); reader.readString(bitOutput); sum += bitOutput.length(); } sink = sum; });
	LOG("================ Serializer String Message ================");
	LOG("Byte Write Median:" + formatMS(byteWriteNS) + " ms | " + formatNSPerCount(byteWriteNS, BENCHMARK_MESSAGE_COUNT) + " ns/message");
	LOG("Bit  Write Median:" + formatMS(bitWriteNS) + " ms | " + formatNSPerCount(bitWriteNS, BENCHMARK_MESSAGE_COUNT) + " ns/message | Bit/Byte:" + formatRatio(bitWriteNS, byteWriteNS) + "x");
	LOG("Byte Read  Median:" + formatMS(byteReadNS) + " ms | " + formatNSPerCount(byteReadNS, BENCHMARK_MESSAGE_COUNT) + " ns/message");
	LOG("Bit  Read  Median:" + formatMS(bitReadNS) + " ms | " + formatNSPerCount(bitReadNS, BENCHMARK_MESSAGE_COUNT) + " ns/message | Bit/Byte:" + formatRatio(bitReadNS, byteReadNS) + "x");
	LOG("Byte Size:" + IToS(byteSize) + " bytes | Bit Size:" + IToS(bitSize) + " bytes | Bit/Byte:" + FToS((float)bitSize / (float)byteSize * 100.0f, 2) + "%");
	(void)sink;
}

void BinaryUtilityTest::benchmarkUIntReadShortCodeFastPath()
{
	using namespace BinaryUtilitySerializeTestSupport;
	const Array<6, uint> listValues{ 0u, 1u, 2u, 3u, 0x40000000u, 0u };
	char listBuffer[64]{};
	int listWriteBitIndex = 0;
	BinaryUtility::writeUnsignedIntegerListBit(listBuffer, (int)sizeof(listBuffer), listWriteBitIndex, listValues);
	const int listBufferSize = BinaryUtility::bitCountToByteCount(listWriteBitIndex);
	volatile ullong sink = 0;
	uint r0 = 0, r1 = 0, r2 = 0, r3 = 0, r4 = 0, r5 = 0;
	uint p0 = 0, p1 = 0, p2 = 0, p3 = 0, p4 = 0, p5 = 0;
	Array<6, uint*> referenceList{ &r0, &r1, &r2, &r3, &r4, &r5 };
	Array<6, uint*> productionList{ &p0, &p1, &p2, &p3, &p4, &p5 };
	const llong listReferenceNS = measureMedianNS([]() {}, [&]()
		{
			ullong sum = 0;
			FOR(BENCHMARK_MESSAGE_COUNT)
			{
				int bitIndex = 0;
				readUIntList6V28Reference(listBuffer, listBufferSize, bitIndex, referenceList);
				sum += r0 + r1 + r2 + r3 + r4 + r5;
			}
			sink = sum;
		});
	const llong listProductionNS = measureMedianNS([]() {}, [&]()
		{
			ullong sum = 0;
			FOR(BENCHMARK_MESSAGE_COUNT)
			{
				int bitIndex = 0;
				BinaryUtility::readUnsignedIntegerListBit(listBuffer, listBufferSize, bitIndex, productionList);
				sum += p0 + p1 + p2 + p3 + p4 + p5;
			}
			sink = sum;
		});
	LOG("================ UInt Independent Read ShortCode FastPath ================");
	LOG("V28 Reference:" + formatMS(listReferenceNS) + " ms | " + formatNSPerCount(listReferenceNS, BENCHMARK_MESSAGE_COUNT) + " ns/message");
	LOG("V33 Product  :" + formatMS(listProductionNS) + " ms | " + formatNSPerCount(listProductionNS, BENCHMARK_MESSAGE_COUNT) + " ns/message | Reference/Product:" + formatRatio(listReferenceNS, listProductionNS) + "x");
	(void)sink;
}

void BinaryUtilityTest::benchmarkReadLayerOverheadSingleUInt()
{
	using namespace BinaryUtilitySerializeTestSupport;
	SerializerBitWrite writer;
	writer.initCapacity(128);
	writer.writeUnsigned(1234u);
	const char* buffer = writer.getBuffer();
	const int bufferSize = writer.getByteCount();
	volatile ullong sink = 0;
	const llong rawNS = measureMedianNS([]() {}, [&]() { ullong sum = 0; FOR(BENCHMARK_MESSAGE_COUNT) { int bitIndex = 0; uint value = 0; BinaryUtility::readUnsignedIntegerBit(buffer, bufferSize, bitIndex, value); sum += value; } sink = sum; });
	const llong serializerNS = measureMedianNS([]() {}, [&]() { ullong sum = 0; FOR(BENCHMARK_MESSAGE_COUNT) { SerializerBitRead reader(buffer, bufferSize); uint value = 0; reader.readUnsigned(value); sum += value; } sink = sum; });
	LOG("================ Read Layer Single UInt ================");
	LOG("Raw BinaryUtility:" + formatMS(rawNS) + " ms | " + formatNSPerCount(rawNS, BENCHMARK_MESSAGE_COUNT) + " ns/message");
	LOG("SerializerBitRead :" + formatMS(serializerNS) + " ms | " + formatNSPerCount(serializerNS, BENCHMARK_MESSAGE_COUNT) + " ns/message | Serializer/Raw:" + formatRatio(serializerNS, rawNS) + "x");
	(void)sink;
}

void BinaryUtilityTest::benchmarkReadLayerReaderReuse()
{
	using namespace BinaryUtilitySerializeTestSupport;
	SerializerBitWrite writer;
	writer.initCapacity(128);
	writer.writeUnsigned(1234u);
	const char* buffer = writer.getBuffer();
	const int bufferSize = writer.getByteCount();
	volatile ullong sink = 0;
	const llong reconstructNS = measureMedianNS([]() {}, [&]() { ullong sum = 0; FOR(BENCHMARK_MESSAGE_COUNT) { SerializerBitRead reader(buffer, bufferSize); uint value = 0; reader.readUnsigned(value); sum += value; } sink = sum; });
	SerializerBitRead reuseResetAllReader(buffer, bufferSize);
	const llong reuseResetAllNS = measureMedianNS([]() {}, [&]() { ullong sum = 0; FOR(BENCHMARK_MESSAGE_COUNT) { reuseResetAllReader.reset(buffer, bufferSize); uint value = 0; reuseResetAllReader.readUnsigned(value); sum += value; } sink = sum; });
	SerializerBitRead reuseIndexReader(buffer, bufferSize);
	const llong reuseIndexNS = measureMedianNS([]() {}, [&]() { ullong sum = 0; FOR(BENCHMARK_MESSAGE_COUNT) { reuseIndexReader.setBitIndex(0); uint value = 0; reuseIndexReader.readUnsigned(value); sum += value; } sink = sum; });
	LOG("================ Read Layer Reader Construction Fixed Buffer ================");
	LOG("Reconstruct Reader :" + formatMS(reconstructNS) + " ms | " + formatNSPerCount(reconstructNS, BENCHMARK_MESSAGE_COUNT) + " ns/message");
	LOG("Reuse + ResetAll   :" + formatMS(reuseResetAllNS) + " ms | " + formatNSPerCount(reuseResetAllNS, BENCHMARK_MESSAGE_COUNT) + " ns/message | Reconstruct/ResetAll:" + formatRatio(reconstructNS, reuseResetAllNS) + "x");
	LOG("Reuse + ResetIndex :" + formatMS(reuseIndexNS) + " ms | " + formatNSPerCount(reuseIndexNS, BENCHMARK_MESSAGE_COUNT) + " ns/message | Reconstruct/ResetIndex:" + formatRatio(reconstructNS, reuseIndexNS) + "x");
	constexpr int VARYING_BUFFER_COUNT = 16;
	const uint varyingValues[VARYING_BUFFER_COUNT]
	{
		0u, 1u, 7u, 8u, 31u, 32u, 127u, 128u,
		255u, 256u, 4095u, 4096u, 32767u, 32768u, 65535u, 0x7FFFFFFFu
	};
	char varyingBuffers[VARYING_BUFFER_COUNT][16]{};
	int varyingBufferSizes[VARYING_BUFFER_COUNT]{};
	FOR(VARYING_BUFFER_COUNT)
	{
		writer.clear();
		writer.writeUnsigned(varyingValues[i]);
		varyingBufferSizes[i] = writer.getByteCount();
		FOR_J(varyingBufferSizes[i]) { varyingBuffers[i][j] = writer.getBuffer()[j]; }
	}
	const llong varyingRawNS = measureMedianNS([]() {}, [&]() { ullong sum = 0; FOR(BENCHMARK_MESSAGE_COUNT) { const int index = i & (VARYING_BUFFER_COUNT - 1); int bitIndex = 0; uint value = 0; BinaryUtility::readUnsignedIntegerBit(varyingBuffers[index], varyingBufferSizes[index], bitIndex, value); sum += value; } sink = sum; });
	const llong varyingReconstructNS = measureMedianNS([]() {}, [&]() { ullong sum = 0; FOR(BENCHMARK_MESSAGE_COUNT) { const int index = i & (VARYING_BUFFER_COUNT - 1); SerializerBitRead reader(varyingBuffers[index], varyingBufferSizes[index]); uint value = 0; reader.readUnsigned(value); sum += value; } sink = sum; });
	SerializerBitRead varyingReuseReader(varyingBuffers[0], varyingBufferSizes[0]);
	const llong varyingResetAllNS = measureMedianNS([]() {}, [&]() { ullong sum = 0; FOR(BENCHMARK_MESSAGE_COUNT) { const int index = i & (VARYING_BUFFER_COUNT - 1); varyingReuseReader.reset(varyingBuffers[index], varyingBufferSizes[index]); uint value = 0; varyingReuseReader.readUnsigned(value); sum += value; } sink = sum; });
	LOG("================ Read Layer Reader Construction Varying Buffer ================");
	LOG("Raw BinaryUtility  :" + formatMS(varyingRawNS) + " ms | " + formatNSPerCount(varyingRawNS, BENCHMARK_MESSAGE_COUNT) + " ns/message");
	LOG("Reconstruct Reader :" + formatMS(varyingReconstructNS) + " ms | " + formatNSPerCount(varyingReconstructNS, BENCHMARK_MESSAGE_COUNT) + " ns/message | Reconstruct/Raw:" + formatRatio(varyingReconstructNS, varyingRawNS) + "x");
	LOG("Reuse + ResetAll   :" + formatMS(varyingResetAllNS) + " ms | " + formatNSPerCount(varyingResetAllNS, BENCHMARK_MESSAGE_COUNT) + " ns/message | ResetAll/Raw:" + formatRatio(varyingResetAllNS, varyingRawNS) + "x | Reconstruct/ResetAll:" + formatRatio(varyingReconstructNS, varyingResetAllNS) + "x");
	(void)sink;
}

void BinaryUtilityTest::benchmarkReadLayerPacketReaderReuse()
{
	using namespace BinaryUtilitySerializeTestSupport;
	constexpr int BUFFER_COUNT = 16;
	char headerBuffers[BUFFER_COUNT][32]{};
	char bodyBuffers[BUFFER_COUNT][16]{};
	int headerSizes[BUFFER_COUNT]{};
	int bodySizes[BUFFER_COUNT]{};
	SerializerBitWrite headerWriter;
	SerializerBitWrite bodyWriter;
	headerWriter.initCapacity(64);
	bodyWriter.initCapacity(32);
	FOR(BUFFER_COUNT)
	{
		headerWriter.clear();
		headerWriter.writeUnsigned((uint)(16 + i));
		headerWriter.writeUnsigned((ushort)(100 + i));
		headerWriter.writeUnsigned((ushort)(200 + i));
		headerWriter.writeUnsigned((uint)(1000 + i * 17));
		headerWriter.writeBool((i & 1) != 0);
		headerWriter.writeBool((i & 2) != 0);
		headerSizes[i] = headerWriter.getByteCount();
		FOR_J(headerSizes[i]) { headerBuffers[i][j] = headerWriter.getBuffer()[j]; }
		bodyWriter.clear();
		bodyWriter.writeUnsigned((uint)(3 + i * 257));
		bodyWriter.writeBool((i & 4) != 0);
		bodySizes[i] = bodyWriter.getByteCount();
		FOR_J(bodySizes[i]) { bodyBuffers[i][j] = bodyWriter.getBuffer()[j]; }
	}
	volatile ullong sink = 0;
	const llong twoConstructNS = measureMedianNS([]() {}, [&]()
		{
			ullong sum = 0;
			FOR(BENCHMARK_MESSAGE_COUNT)
			{
				const int index = i & (BUFFER_COUNT - 1);
				uint bodySize = 0, sequence = 0, bodyValue = 0;
				ushort bodyCRC = 0, packetType = 0;
				bool hasSign = false, useFlag = false, bodyFlag = false;
				SerializerBitRead headerReader(headerBuffers[index], headerSizes[index]);
				headerReader.readUnsigned(bodySize);
				headerReader.readUnsigned(bodyCRC);
				headerReader.readUnsigned(packetType);
				headerReader.readUnsigned(sequence);
				headerReader.readBool(hasSign);
				headerReader.readBool(useFlag);
				SerializerBitRead bodyReader(bodyBuffers[index], bodySizes[index]);
				bodyReader.readUnsigned(bodyValue);
				bodyReader.readBool(bodyFlag);
				sum += bodySize + bodyCRC + packetType + sequence + bodyValue + (hasSign ? 1u : 0u) + (useFlag ? 1u : 0u) + (bodyFlag ? 1u : 0u);
			}
			sink = sum;
		});
	const llong localReuseNS = measureMedianNS([]() {}, [&]()
		{
			ullong sum = 0;
			FOR(BENCHMARK_MESSAGE_COUNT)
			{
				const int index = i & (BUFFER_COUNT - 1);
				uint bodySize = 0, sequence = 0, bodyValue = 0;
				ushort bodyCRC = 0, packetType = 0;
				bool hasSign = false, useFlag = false, bodyFlag = false;
				SerializerBitRead reader(headerBuffers[index], headerSizes[index]);
				reader.readUnsigned(bodySize);
				reader.readUnsigned(bodyCRC);
				reader.readUnsigned(packetType);
				reader.readUnsigned(sequence);
				reader.readBool(hasSign);
				reader.readBool(useFlag);
				reader.reset(bodyBuffers[index], bodySizes[index]);
				reader.readUnsigned(bodyValue);
				reader.readBool(bodyFlag);
				sum += bodySize + bodyCRC + packetType + sequence + bodyValue + (hasSign ? 1u : 0u) + (useFlag ? 1u : 0u) + (bodyFlag ? 1u : 0u);
			}
			sink = sum;
		});
	SerializerBitRead persistentReader(headerBuffers[0], headerSizes[0]);
	const llong persistentReuseNS = measureMedianNS([]() {}, [&]()
		{
			ullong sum = 0;
			FOR(BENCHMARK_MESSAGE_COUNT)
			{
				const int index = i & (BUFFER_COUNT - 1);
				uint bodySize = 0, sequence = 0, bodyValue = 0;
				ushort bodyCRC = 0, packetType = 0;
				bool hasSign = false, useFlag = false, bodyFlag = false;
				persistentReader.reset(headerBuffers[index], headerSizes[index]);
				persistentReader.readUnsigned(bodySize);
				persistentReader.readUnsigned(bodyCRC);
				persistentReader.readUnsigned(packetType);
				persistentReader.readUnsigned(sequence);
				persistentReader.readBool(hasSign);
				persistentReader.readBool(useFlag);
				persistentReader.reset(bodyBuffers[index], bodySizes[index]);
				persistentReader.readUnsigned(bodyValue);
				persistentReader.readBool(bodyFlag);
				sum += bodySize + bodyCRC + packetType + sequence + bodyValue + (hasSign ? 1u : 0u) + (useFlag ? 1u : 0u) + (bodyFlag ? 1u : 0u);
			}
			sink = sum;
		});
	LOG("================ Read Layer Packet Reader Lifecycle ================");
	LOG("Two Construct        :" + formatMS(twoConstructNS) + " ms | " + formatNSPerCount(twoConstructNS, BENCHMARK_MESSAGE_COUNT) + " ns/message");
	LOG("Local Reuse Body     :" + formatMS(localReuseNS) + " ms | " + formatNSPerCount(localReuseNS, BENCHMARK_MESSAGE_COUNT) + " ns/message | Two/Local:" + formatRatio(twoConstructNS, localReuseNS) + "x");
	LOG("Persistent Reset Both:" + formatMS(persistentReuseNS) + " ms | " + formatNSPerCount(persistentReuseNS, BENCHMARK_MESSAGE_COUNT) + " ns/message | Two/Persistent:" + formatRatio(twoConstructNS, persistentReuseNS) + "x");
	(void)sink;
}

void BinaryUtilityTest::benchmarkReadLayerOverheadTiny()
{
	using namespace BinaryUtilitySerializeTestSupport;
	SerializerBitWrite writer;
	writer.initCapacity(128);
	writer.writeUnsigned(3u);
	writer.writeBool(true);
	const char* buffer = writer.getBuffer();
	const int bufferSize = writer.getByteCount();
	volatile ullong sink = 0;
	const llong rawNS = measureMedianNS([]() {}, [&]() { ullong sum = 0; FOR(BENCHMARK_MESSAGE_COUNT) { int bitIndex = 0; uint state = 0; bool active = false; BinaryUtility::readUnsignedIntegerBit(buffer, bufferSize, bitIndex, state); BinaryUtility::readBoolBit(buffer, bufferSize, bitIndex, active); sum += state + (active ? 1u : 0u); } sink = sum; });
	const llong serializerNS = measureMedianNS([]() {}, [&]() { ullong sum = 0; FOR(BENCHMARK_MESSAGE_COUNT) { SerializerBitRead reader(buffer, bufferSize); uint state = 0; bool active = false; reader.readUnsigned(state); reader.readBool(active); sum += state + (active ? 1u : 0u); } sink = sum; });
	LOG("================ Read Layer Tiny Message ================");
	LOG("Raw BinaryUtility:" + formatMS(rawNS) + " ms | " + formatNSPerCount(rawNS, BENCHMARK_MESSAGE_COUNT) + " ns/message");
	LOG("SerializerBitRead :" + formatMS(serializerNS) + " ms | " + formatNSPerCount(serializerNS, BENCHMARK_MESSAGE_COUNT) + " ns/message | Serializer/Raw:" + formatRatio(serializerNS, rawNS) + "x");
	(void)sink;
}

void BinaryUtilityTest::benchmarkReadLayerOverheadUIntList(const string& profileName, const Array<6, uint>& values)
{
	using namespace BinaryUtilitySerializeTestSupport;
	SerializerBitWrite writer;
	writer.initCapacity(128);
	writer.writeUnsigned<6, uint>(values);
	const char* buffer = writer.getBuffer();
	const int bufferSize = writer.getByteCount();
	volatile ullong sink = 0;
	uint a = 0, b = 0, c = 0, d = 0, e = 0, f = 0;
	Array<6, uint*> persistentOutput{ &a, &b, &c, &d, &e, &f };
	const llong templateReferenceNS = measureMedianNS([]() {}, [&]() { ullong sum = 0; FOR(BENCHMARK_MESSAGE_COUNT) { int bitIndex = 0; BinaryUtility::readUnsignedIntegerListBit<uint, 6>(buffer, bufferSize, bitIndex, persistentOutput); sum += a + b + c + d + e + f; } sink = sum; });
	const llong rawPersistentNS = measureMedianNS([]() {}, [&]() { ullong sum = 0; FOR(BENCHMARK_MESSAGE_COUNT) { int bitIndex = 0; BinaryUtility::readUnsignedIntegerListBit(buffer, bufferSize, bitIndex, persistentOutput); sum += a + b + c + d + e + f; } sink = sum; });
	const llong rawBuildArgsNS = measureMedianNS([]() {}, [&]() { ullong sum = 0; FOR(BENCHMARK_MESSAGE_COUNT) { int bitIndex = 0; uint v0 = 0, v1 = 0, v2 = 0, v3 = 0, v4 = 0, v5 = 0; Array<6, uint*> output{ &v0, &v1, &v2, &v3, &v4, &v5 }; BinaryUtility::readUnsignedIntegerListBit(buffer, bufferSize, bitIndex, output); sum += v0 + v1 + v2 + v3 + v4 + v5; } sink = sum; });
	const llong serializerNS = measureMedianNS([]() {}, [&]() { ullong sum = 0; FOR(BENCHMARK_MESSAGE_COUNT) { SerializerBitRead reader(buffer, bufferSize); uint v0 = 0, v1 = 0, v2 = 0, v3 = 0, v4 = 0, v5 = 0; reader.readUnsigned(v0, v1, v2, v3, v4, v5); sum += v0 + v1 + v2 + v3 + v4 + v5; } sink = sum; });
	LOG("================ Read Layer " + profileName + " ================");
	LOG("V32 Template Core  :" + formatMS(templateReferenceNS) + " ms | " + formatNSPerCount(templateReferenceNS, BENCHMARK_MESSAGE_COUNT) + " ns/message");
	LOG("Raw Persistent Args:" + formatMS(rawPersistentNS) + " ms | " + formatNSPerCount(rawPersistentNS, BENCHMARK_MESSAGE_COUNT) + " ns/message | V32/V33:" + formatRatio(templateReferenceNS, rawPersistentNS) + "x");
	LOG("Raw Build Args     :" + formatMS(rawBuildArgsNS) + " ms | " + formatNSPerCount(rawBuildArgsNS, BENCHMARK_MESSAGE_COUNT) + " ns/message | Build/Persistent:" + formatRatio(rawBuildArgsNS, rawPersistentNS) + "x");
	LOG("SerializerBitRead  :" + formatMS(serializerNS) + " ms | " + formatNSPerCount(serializerNS, BENCHMARK_MESSAGE_COUNT) + " ns/message | Serializer/Persistent:" + formatRatio(serializerNS, rawPersistentNS) + "x");
	(void)sink;
}

void BinaryUtilityTest::benchmarkReadLayerDynamicUIntList()
{
	using namespace BinaryUtilitySerializeTestSupport;
	Vector<uint> values;
	const uint source[]{ 0u, 1u, 2u, 3u, 15u, 31u, 255u, 1024u, 8000u, 10023u, 65535u, 100000u };
	for (uint value : source) { values.add(value); }
	char buffer[256]{};
	int writeBitIndex = 0;
	BinaryUtility::writeUnsignedIntegerListBit(buffer, (int)sizeof(buffer), writeBitIndex, values);
	const int bufferSize = BinaryUtility::bitCountToByteCount(writeBitIndex);
	volatile ullong sink = 0;
	Vector<uint> v34Output;
	Vector<uint> v35Output;
	Vector<uint> v36Output;
	Vector<uint> v37Output;
	Vector<uint> serializerOutput;
	v34Output.reserve(values.size());
	v35Output.reserve(values.size());
	v36Output.reserve(values.size());
	v37Output.reserve(values.size());
	serializerOutput.reserve(values.size());
	const llong templateReferenceNS = measureMedianNS([]() {}, [&]() { ullong sum = 0; FOR(BENCHMARK_MESSAGE_COUNT) { int bitIndex = 0; BinaryUtility::readUnsignedIntegerListBit<uint>(buffer, bufferSize, bitIndex, v34Output); FOR_J(v34Output.size()) { sum += v34Output[j]; } } sink = sum; });
	const llong v35ReferenceNS = measureMedianNS([]() {}, [&]() { ullong sum = 0; FOR(BENCHMARK_MESSAGE_COUNT) { int bitIndex = 0; readUnsignedIntVectorV35Reference(buffer, bufferSize, bitIndex, v35Output); FOR_J(v35Output.size()) { sum += v35Output[j]; } } sink = sum; });
	const llong v36ReferenceNS = measureMedianNS([]() {}, [&]() { ullong sum = 0; FOR(BENCHMARK_MESSAGE_COUNT) { int bitIndex = 0; readUnsignedIntVectorV36Reference(buffer, bufferSize, bitIndex, v36Output); FOR_J(v36Output.size()) { sum += v36Output[j]; } } sink = sum; });
	const llong v37ProductNS = measureMedianNS([]() {}, [&]() { ullong sum = 0; FOR(BENCHMARK_MESSAGE_COUNT) { int bitIndex = 0; BinaryUtility::readUnsignedIntegerListBit(buffer, bufferSize, bitIndex, v37Output); FOR_J(v37Output.size()) { sum += v37Output[j]; } } sink = sum; });
	const llong serializerNS = measureMedianNS([]() {}, [&]() { ullong sum = 0; FOR(BENCHMARK_MESSAGE_COUNT) { SerializerBitRead reader(buffer, bufferSize); reader.readUnsignedList(serializerOutput); FOR_J(serializerOutput.size()) { sum += serializerOutput[j]; } } sink = sum; });
	LOG("================ Read Layer Dynamic UInt Vector ================");
	LOG("V34 Template Vector:" + formatMS(templateReferenceNS) + " ms | " + formatNSPerCount(templateReferenceNS, BENCHMARK_MESSAGE_COUNT) + " ns/message");
	LOG("V35 Reference      :" + formatMS(v35ReferenceNS) + " ms | " + formatNSPerCount(v35ReferenceNS, BENCHMARK_MESSAGE_COUNT) + " ns/message | V34/V35:" + formatRatio(templateReferenceNS, v35ReferenceNS) + "x");
	LOG("V36 Reference      :" + formatMS(v36ReferenceNS) + " ms | " + formatNSPerCount(v36ReferenceNS, BENCHMARK_MESSAGE_COUNT) + " ns/message | V35/V36:" + formatRatio(v35ReferenceNS, v36ReferenceNS) + "x");
	LOG("V37 Product        :" + formatMS(v37ProductNS) + " ms | " + formatNSPerCount(v37ProductNS, BENCHMARK_MESSAGE_COUNT) + " ns/message | V36/V37:" + formatRatio(v36ReferenceNS, v37ProductNS) + "x");
	LOG("SerializerBitRead  :" + formatMS(serializerNS) + " ms | " + formatNSPerCount(serializerNS, BENCHMARK_MESSAGE_COUNT) + " ns/message | Serializer/Core:" + formatRatio(serializerNS, v37ProductNS) + "x");
	(void)sink;
}

void BinaryUtilityTest::benchmarkReadLayerDynamicUIntListVaryingCount()
{
	using namespace BinaryUtilitySerializeTestSupport;
	constexpr int BUFFER_COUNT = 4;
	const uint source0[]{ 0u, 1u, 2u };
	const uint source1[]{ 3u, 15u, 31u, 255u, 1024u };
	const uint source2[]{ 0u, 1u, 2u, 3u, 15u, 31u, 255u, 1024u, 8000u, 10023u, 65535u, 100000u };
	const uint source3[]{ 7u, 127u, 32768u, 65535u, 100000u, 0x40000000u, 0xFFFFFFFFu };
	const uint* sources[BUFFER_COUNT]{ source0, source1, source2, source3 };
	const int counts[BUFFER_COUNT]{ 3, 5, 12, 7 };
	char buffers[BUFFER_COUNT][256]{};
	int bufferSizes[BUFFER_COUNT]{};
	FOR(BUFFER_COUNT)
	{
		Vector<uint> values;
		values.reserve(counts[i]);
		FOR_J(counts[i]) { values.add(sources[i][j]); }
		int writeBitIndex = 0;
		BinaryUtility::writeUnsignedIntegerListBit(buffers[i], (int)sizeof(buffers[i]), writeBitIndex, values);
		bufferSizes[i] = BinaryUtility::bitCountToByteCount(writeBitIndex);
	}
	volatile ullong sink = 0;
	Vector<uint> v36Output;
	Vector<uint> v37Output;
	v36Output.reserve(12);
	v37Output.reserve(12);
	const llong v36ReferenceNS = measureMedianNS([]() {}, [&]() { ullong sum = 0; FOR(BENCHMARK_MESSAGE_COUNT) { const int index = i & (BUFFER_COUNT - 1); int bitIndex = 0; readUnsignedIntVectorV36Reference(buffers[index], bufferSizes[index], bitIndex, v36Output); const uint* output = v36Output.data(); FOR_J(v36Output.size()) { sum += output[j]; } } sink = sum; });
	const llong v37ProductNS = measureMedianNS([]() {}, [&]() { ullong sum = 0; FOR(BENCHMARK_MESSAGE_COUNT) { const int index = i & (BUFFER_COUNT - 1); int bitIndex = 0; BinaryUtility::readUnsignedIntegerListBit(buffers[index], bufferSizes[index], bitIndex, v37Output); const uint* output = v37Output.data(); FOR_J(v37Output.size()) { sum += output[j]; } } sink = sum; });
	LOG("================ Read Layer Dynamic UInt Vector Varying Count ================");
	LOG("V36 Reference:" + formatMS(v36ReferenceNS) + " ms | " + formatNSPerCount(v36ReferenceNS, BENCHMARK_MESSAGE_COUNT) + " ns/message");
	LOG("V37 Product  :" + formatMS(v37ProductNS) + " ms | " + formatNSPerCount(v37ProductNS, BENCHMARK_MESSAGE_COUNT) + " ns/message | V36/V37:" + formatRatio(v36ReferenceNS, v37ProductNS) + "x");
	(void)sink;
}

void BinaryUtilityTest::benchmarkReadLayerSignedLLongPair()
{
	using namespace BinaryUtilitySerializeTestSupport;
	const Array<2, llong> values{ 123456789LL, 987654321LL };
	char buffer[128]{};
	int writeBitIndex = 0;
	BinaryUtility::writeSignedIntegerListBit(buffer, (int)sizeof(buffer), writeBitIndex, values, true);
	const int bufferSize = BinaryUtility::bitCountToByteCount(writeBitIndex);
	volatile ullong sink = 0;
	llong v0 = 0, v1 = 0;
	Array<2, llong*> output{ &v0, &v1 };
	const llong templateReferenceNS = measureMedianNS([]() {}, [&]()
		{
			ullong sum = 0;
			FOR(BENCHMARK_MESSAGE_COUNT)
			{
				int bitIndex = 0;
				BinaryUtility::readSignedIntegerListBit<llong, 2>(buffer, bufferSize, bitIndex, output, true);
				sum += (ullong)v0 + (ullong)v1;
			}
			sink = sum;
		});
	const llong specializedNS = measureMedianNS([]() {}, [&]()
		{
			ullong sum = 0;
			FOR(BENCHMARK_MESSAGE_COUNT)
			{
				int bitIndex = 0;
				BinaryUtility::readSignedIntegerListBit(buffer, bufferSize, bitIndex, output, true);
				sum += (ullong)v0 + (ullong)v1;
			}
			sink = sum;
		});
	const llong serializerNS = measureMedianNS([]() {}, [&]()
		{
			ullong sum = 0;
			FOR(BENCHMARK_MESSAGE_COUNT)
			{
				SerializerBitRead reader(buffer, bufferSize);
				llong a = 0, b = 0;
				reader.readSigned(true, a, b);
				sum += (ullong)a + (ullong)b;
			}
			sink = sum;
		});
	LOG("================ Read Layer Signed LLong Pair ================");
	LOG("V33 Template Core:" + formatMS(templateReferenceNS) + " ms | " + formatNSPerCount(templateReferenceNS, BENCHMARK_MESSAGE_COUNT) + " ns/message");
	LOG("V34 Specialized  :" + formatMS(specializedNS) + " ms | " + formatNSPerCount(specializedNS, BENCHMARK_MESSAGE_COUNT) + " ns/message | V33/V34:" + formatRatio(templateReferenceNS, specializedNS) + "x");
	LOG("SerializerBitRead:" + formatMS(serializerNS) + " ms | " + formatNSPerCount(serializerNS, BENCHMARK_MESSAGE_COUNT) + " ns/message | Serializer/Core:" + formatRatio(serializerNS, specializedNS) + "x");
	(void)sink;
}

void BinaryUtilityTest::benchmarkReadLayerFloatList(const string& profileName, const float* values)
{
	using namespace BinaryUtilitySerializeTestSupport;
	char buffer[128]{};
	int writeBitIndex = 0;
	BinaryUtility::writeFloatListBit(buffer, (int)sizeof(buffer), writeBitIndex, values, 6, false, true, 3);
	const int bufferSize = BinaryUtility::bitCountToByteCount(writeBitIndex);
	volatile llong sink = 0;
	int i0 = 0, i1 = 0, i2 = 0, i3 = 0, i4 = 0, i5 = 0;
	Array<6, int*> signedOutput{ &i0, &i1, &i2, &i3, &i4, &i5 };
	const llong signedTemplateReferenceNS = measureMedianNS([]() {}, [&]()
		{
			llong sum = 0;
			FOR(BENCHMARK_MESSAGE_COUNT)
			{
				int bitIndex = 0;
				BinaryUtility::readSignedIntegerListBit<int, 6>(buffer, bufferSize, bitIndex, signedOutput, true);
				sum += (llong)i0 + i1 + i2 + i3 + i4 + i5;
			}
			sink = sum;
		});
	const llong signedCoreNS = measureMedianNS([]() {}, [&]()
		{
			llong sum = 0;
			FOR(BENCHMARK_MESSAGE_COUNT)
			{
				int bitIndex = 0;
				BinaryUtility::readSignedIntegerListBit(buffer, bufferSize, bitIndex, signedOutput, true);
				sum += (llong)i0 + i1 + i2 + i3 + i4 + i5;
			}
			sink = sum;
		});
	float f0 = 0.0f, f1 = 0.0f, f2 = 0.0f, f3 = 0.0f, f4 = 0.0f, f5 = 0.0f;
	float* floatOutput[6]{ &f0, &f1, &f2, &f3, &f4, &f5 };
	const llong rawFloatNS = measureMedianNS([]() {}, [&]()
		{
			llong sum = 0;
			FOR(BENCHMARK_MESSAGE_COUNT)
			{
				int bitIndex = 0;
				BinaryUtility::readFloatListBit(buffer, bufferSize, bitIndex, floatOutput, 6, true, 3);
				sum += (llong)(f0 * 1000.0f) + (llong)(f1 * 1000.0f) + (llong)(f2 * 1000.0f) + (llong)(f3 * 1000.0f) + (llong)(f4 * 1000.0f) + (llong)(f5 * 1000.0f);
			}
			sink = sum;
		});
	const llong serializerNS = measureMedianNS([]() {}, [&]()
		{
			llong sum = 0;
			FOR(BENCHMARK_MESSAGE_COUNT)
			{
				SerializerBitRead reader(buffer, bufferSize);
				float v0 = 0.0f, v1 = 0.0f, v2 = 0.0f, v3 = 0.0f, v4 = 0.0f, v5 = 0.0f;
				reader.readFloat(true, v0, v1, v2, v3, v4, v5);
				sum += (llong)(v0 * 1000.0f) + (llong)(v1 * 1000.0f) + (llong)(v2 * 1000.0f) + (llong)(v3 * 1000.0f) + (llong)(v4 * 1000.0f) + (llong)(v5 * 1000.0f);
			}
			sink = sum;
		});
	LOG("================ Read Layer Float " + profileName + " ================");
	LOG("V31 Template Core  :" + formatMS(signedTemplateReferenceNS) + " ms | " + formatNSPerCount(signedTemplateReferenceNS, BENCHMARK_MESSAGE_COUNT) + " ns/message");
	LOG("Signed Integer Core:" + formatMS(signedCoreNS) + " ms | " + formatNSPerCount(signedCoreNS, BENCHMARK_MESSAGE_COUNT) + " ns/message | V31/V32:" + formatRatio(signedTemplateReferenceNS, signedCoreNS) + "x");
	LOG("Raw Float Decode   :" + formatMS(rawFloatNS) + " ms | " + formatNSPerCount(rawFloatNS, BENCHMARK_MESSAGE_COUNT) + " ns/message | Float/Signed:" + formatRatio(rawFloatNS, signedCoreNS) + "x");
	LOG("SerializerBitRead  :" + formatMS(serializerNS) + " ms | " + formatNSPerCount(serializerNS, BENCHMARK_MESSAGE_COUNT) + " ns/message | Serializer/Raw:" + formatRatio(serializerNS, rawFloatNS) + "x");
	(void)sink;
}

void BinaryUtilityTest::benchmarkReadLayerOverhead()
{
	using namespace BinaryUtilitySerializeTestSupport;
	benchmarkReadLayerOverheadSingleUInt();
	benchmarkReadLayerReaderReuse();
	benchmarkReadLayerPacketReaderReuse();
	benchmarkReadLayerOverheadTiny();
	benchmarkReadLayerSignedLLongPair();
	const Array<6, uint> uniformValues{ 8000u, 7234u, 10023u, 5u, 1234u, 5678u };
	const Array<6, uint> independentValues{ 0u, 1u, 2u, 3u, 0x40000000u, 0u };
	benchmarkReadLayerOverheadUIntList("UInt UniformLength", uniformValues);
	benchmarkReadLayerOverheadUIntList("UInt IndependentLength", independentValues);
	benchmarkReadLayerDynamicUIntList();
	benchmarkReadLayerDynamicUIntListVaryingCount();
	const float floatIndependentValues[6]{ 0.125f, 1.25f, 12.345f, -0.75f, -23.456f, 100.001f };
	const float floatUniformValues[6]{ 8.0f, 7.234f, 10.023f, -5.0f, 1.234f, -5.678f };
	benchmarkReadLayerFloatList("IndependentLength", floatIndependentValues);
	benchmarkReadLayerFloatList("UniformLength", floatUniformValues);
}

void BinaryUtilityTest::benchmarkBitReadBufferSizeSensitivity()
{
	using namespace BinaryUtilitySerializeTestSupport;
	SerializerBitWrite writer;
	writer.initCapacity(128);
	writer.writeSigned(true, 123456789LL, 987654321LL);
	writer.writeUnsigned(8000u, 7234u, 10023u, 5u, 1234u, 5678u);
	writer.writeUnsigned(3u);
	writer.writeFloat(true, 1.25f);
	const int exactSize = writer.getByteCount();
	const int paddedSize = writer.getBufferSize();
	volatile ullong sink = 0;
	auto readMessage = [&](const int bufferSize)
		{
			ullong sum = 0;
			FOR(BENCHMARK_MESSAGE_COUNT)
			{
				SerializerBitRead reader(writer.getBuffer(), bufferSize);
				llong guid0 = 0, guid1 = 0;
				uint maxHP = 0, hp = 0, monsterID = 0, actionState = 0, posX = 0, posY = 0, direction = 0;
				float attackSpeed = 0.0f;
				reader.readSigned(true, guid0, guid1);
				reader.readUnsigned(maxHP, hp, monsterID, actionState, posX, posY);
				reader.readUnsigned(direction);
				reader.readFloat(true, attackSpeed);
				sum += (ullong)guid0 + (ullong)guid1 + maxHP + hp + monsterID + actionState + posX + posY + direction + (uint)attackSpeed;
			}
			sink = sum;
		};
	const llong exactNS = measureMedianNS([]() {}, [&]() { readMessage(exactSize); });
	const llong paddedNS = measureMedianNS([]() {}, [&]() { readMessage(paddedSize); });
	LOG("================ Bit Read Buffer Size Sensitivity ================");
	LOG("Exact Size:" + IToS(exactSize) + " bytes | Read Median:" + formatMS(exactNS) + " ms | " + formatNSPerCount(exactNS, BENCHMARK_MESSAGE_COUNT) + " ns/message");
	LOG("Padded Size:" + IToS(paddedSize) + " bytes | Read Median:" + formatMS(paddedNS) + " ms | " + formatNSPerCount(paddedNS, BENCHMARK_MESSAGE_COUNT) + " ns/message | Exact/Padded:" + formatRatio(exactNS, paddedNS) + "x");
	(void)sink;
}

void BinaryUtilityTest::testUnsignedLLongMaxBitBoundary()
{
	using namespace BinaryUtilitySerializeTestSupport;
	constexpr ullong TEST_VALUE = ~0ULL;

	// ullong长度字段占6bit。
	// ulong最大值需要完整64bit数据,
	// 所以最终应该占用6 + 64 = 70bit。
	constexpr int EXPECTED_VALUE_BIT_COUNT = 70;

	FOR(8)
	{
		const int offset = i;

		char buffer[32]{};

		// 故意测试0~7的非字节对齐起始位置。
		for (int bit = 0; bit < offset; ++bit)
		{
			BinaryUtility::setBufferBitOne(buffer, bit);
		}

		int writeBitIndex = offset;

		assertResult(
			writeUnsignedIntegerBit(
				buffer,
				(int)sizeof(buffer),
				writeBitIndex,
				TEST_VALUE),
			"unsigned llong max write offset:" + IToS(offset));

		// ulong.MaxValue本身必须严格消耗70bit。
		assertResult(
			writeBitIndex == offset + EXPECTED_VALUE_BIT_COUNT,
			"unsigned llong max write bitIndex offset:" +
			IToS(offset) +
			",Expected:" +
			IToS(offset + EXPECTED_VALUE_BIT_COUNT) +
			",Actual:" +
			IToS(writeBitIndex));

		// 后面再写两个哨兵字段。
		// 专门用来检查读取64位整数后有没有少读/多读1bit。
		assertResult(
			write(
				buffer,
				(int)sizeof(buffer),
				writeBitIndex,
				false),
			"unsigned llong max write sentinel0 offset:" + IToS(offset));

		assertResult(
			write(
				buffer,
				(int)sizeof(buffer),
				writeBitIndex,
				true),
			"unsigned llong max write sentinel1 offset:" + IToS(offset));

		const int finalWriteBitIndex = writeBitIndex;
		const int bufferSize =
			BinaryUtility::bitCountToByteCount(finalWriteBitIndex);

		int readBitIndex = offset;
		ullong value = 0;

		assertResult(
			BinaryUtility::readUnsignedIntegerBit(
				buffer,
				bufferSize,
				readBitIndex,
				value),
			"unsigned llong max read offset:" + IToS(offset));

		assertResult(
			value == TEST_VALUE,
			"unsigned llong max value offset:" + IToS(offset));

		// 这里是本次问题最关键的断言。
		assertResult(
			readBitIndex == offset + EXPECTED_VALUE_BIT_COUNT,
			"unsigned llong max read bitIndex offset:" +
			IToS(offset) +
			",Expected:" +
			IToS(offset + EXPECTED_VALUE_BIT_COUNT) +
			",Actual:" +
			IToS(readBitIndex));

		bool sentinel0 = true;
		bool sentinel1 = false;

		assertResult(
			read(
				buffer,
				bufferSize,
				readBitIndex,
				sentinel0),
			"unsigned llong max read sentinel0 offset:" + IToS(offset));

		assertResult(
			!sentinel0,
			"unsigned llong max sentinel0 value offset:" + IToS(offset));

		assertResult(
			read(
				buffer,
				bufferSize,
				readBitIndex,
				sentinel1),
			"unsigned llong max read sentinel1 offset:" + IToS(offset));

		assertResult(
			sentinel1,
			"unsigned llong max sentinel1 value offset:" + IToS(offset));

		assertResult(
			readBitIndex == finalWriteBitIndex,
			"unsigned llong max final bitIndex offset:" +
			IToS(offset) +
			",Write:" +
			IToS(finalWriteBitIndex) +
			",Read:" +
			IToS(readBitIndex));
	}
}


void BinaryUtilityTest::test()
{
	testDeep();
	testExtreme();
	testBinaryUtility();
}

void BinaryUtilityTest::assertBinary(bool condition, const char* message)
{
	if (!condition)
	{
		ERROR(string("BinaryUtilityTest failed: ") + message);
	}
}

bool BinaryUtilityTest::approxEqualFloat(const float a, const float b, const float eps)
{
	return fabs(a - b) <= eps;
}

bool BinaryUtilityTest::approxEqualDouble(const double a, const double b, const double eps)
{
	return fabs(a - b) <= eps;
}

void BinaryUtilityTest::testBitCountAndFlags()
{
	assertBinary(generateBitCount((byte)0) == 0, "bitcount byte 0");
	assertBinary(generateBitCount((byte)1) == 1, "bitcount byte 1");
	assertBinary(generateBitCount((byte)2) == 2, "bitcount byte 2");
	assertBinary(generateBitCount((byte)3) == 2, "bitcount byte 3");
	assertBinary(generateBitCount((ushort)255) == 8, "bitcount ushort 255");
	assertBinary(generateBitCount((ushort)256) == 9, "bitcount ushort 256");
	assertBinary(generateBitCount((uint)65535) == 16, "bitcount uint 65535");
	assertBinary(generateBitCount((ullong)4294967296ull) == 33, "bitcount ullong");
	assertBinary(getBits((byte)0b11010110, (byte)1, (byte)3) == 0b011, "getBits");
	assertBinary(bitCountToByteCount(0) == 0, "bitcount bytes 0");
	assertBinary(bitCountToByteCount(1) == 1, "bitcount bytes 1");
	assertBinary(bitCountToByteCount(8) == 1, "bitcount bytes 8");
	assertBinary(bitCountToByteCount(9) == 2, "bitcount bytes 9");
}

void BinaryUtilityTest::testBitManipulation()
{
	uint value = 0;
	setByte(value, (byte)0x12, 0);
	setByte(value, (byte)0x34, 1);
	setByte(value, (byte)0x56, 2);
	assertBinary(getByte(value, 0) == 0x12, "getByte 0");
	assertBinary(getByte(value, 1) == 0x34, "getByte 1");
	assertBinary(getByte(value, 2) == 0x56, "getByte 2");

	setBit(value, 0, 1);
	setBitOne(value, 3);
	setBitZero(value, 3);
	assertBinary(hasBit(value, 0), "hasBit 0");
	assertBinary(!hasBit(value, 3), "hasBit 3");
	assertBinary(getBit(value, 0) == 1, "getBit 0");
	assertBinary(getHighestBit((byte)0x80) == 1, "highest bit");
	assertBinary(getLowestBit((byte)0x81) == 1, "lowest bit");

	byte copy = 0;
	copyBits(copy, 2, (byte)0b11110000, 4, 4);
	assertBinary(copy == 0b00001111 << 2, "copyBits");

	ushort swapValue = 0x1234;
	swapByte(swapValue, 0, 1);
	assertBinary(swapValue == 0x3412, "swapByte");
	uint inverseValue = 0x11223344;
	inverseByte(inverseValue);
	assertBinary(inverseValue == 0x44332211, "inverseByte");

	char buffer[32]{};
	setString(buffer, 32, string("abc"));
	assertBinary(string(buffer) == "abc", "setString");
}

void BinaryUtilityTest::testPrimitiveReadWrite()
{
	char buffer[64]{};
	int index = 0;
	assertBinary(write(buffer, 64, index, (int)123456), "write int");
	assertBinary(writeInverse(buffer, 64, index, (short)-321), "write inverse short");
	assertBinary(write(buffer, 64, index, (float)12.5f), "write float");
	index = 0;
	int intValue = 0;
	short shortValue = 0;
	float floatValue = 0.0f;
	assertBinary(read(buffer, 64, index, intValue), "read int");
	assertBinary(readInverse(buffer, 64, index, shortValue), "read inverse short");
	assertBinary(read(buffer, 64, index, floatValue), "read float");
	assertBinary(intValue == 123456, "int roundtrip");
	assertBinary(shortValue == -321, "short roundtrip");
	assertBinary(floatValue == 12.5f, "float roundtrip");
}

void BinaryUtilityTest::testVectorReadWrite()
{
	char buffer[128]{};
	int index = 0;
	assertBinary(writeVector2(buffer, 128, index, Vector2(1.5f, -2.5f)), "write vec2");
	assertBinary(writeVector3(buffer, 128, index, Vector3(3.25f, 4.5f, -5.75f)), "write vec3");
	assertBinary(writeVector4(buffer, 128, index, Vector4(6.0f, 7.0f, 8.0f, 9.0f)), "write vec4");
	index = 0;
	Vector2 v2;
	Vector3 v3;
	Vector4 v4;
	assertBinary(readVector2(buffer, 128, index, v2), "read vec2");
	assertBinary(readVector3(buffer, 128, index, v3), "read vec3");
	assertBinary(readVector4(buffer, 128, index, v4), "read vec4");
	assertBinary(approxEqualFloat(v2.x, 1.5f) && approxEqualFloat(v2.y, -2.5f), "vec2 roundtrip");
	assertBinary(approxEqualFloat(v3.z, -5.75f), "vec3 roundtrip");
	assertBinary(approxEqualFloat(v4.w, 9.0f), "vec4 roundtrip");
}

void BinaryUtilityTest::testColorAndBuffer()
{
	char buffer[128]{};
	int index = 0;
	Color color(11, 22, 33, 44);
	assertBinary(writeColor(buffer, 128, index, color), "write color");
	assertBinary(writeBuffer(buffer, 128, index, "XYZ", 3), "write buffer");
	index = 0;
	Color loaded;
	char text[4]{};
	assertBinary(readColor(buffer, 128, index, loaded), "read color");
	assertBinary(readBuffer(buffer, 128, index, text, 4, 3), "read buffer");
	assertBinary(loaded.r == 11, "color r");
	assertBinary(string(text, 3) == "XYZ", "buffer text");
}

void BinaryUtilityTest::testUnsignedIntegerBits()
{
	char buffer[64]{};
	int bitIndex = 0;
	assertBinary(writeBoolBit(buffer, 64, bitIndex, true), "write bool");
	assertBinary(writeUnsignedIntegerBit(buffer, 64, bitIndex, (ushort)511), "write ushort");
	assertBinary(writeUnsignedIntegerBit(buffer, 64, bitIndex, (uint)65535), "write uint");
	assertBinary(writeUnsignedIntegerBit(buffer, 64, bitIndex, (ullong)123456789ull), "write ullong");
	bitIndex = 0;
	bool boolValue = false;
	ushort us = 0;
	uint ui = 0;
	ullong ul = 0;
	assertBinary(readBoolBit(buffer, 64, bitIndex, boolValue), "read bool");
	assertBinary(readUnsignedIntegerBit(buffer, 64, bitIndex, us), "read ushort");
	assertBinary(readUnsignedIntegerBit(buffer, 64, bitIndex, ui), "read uint");
	assertBinary(readUnsignedIntegerBit(buffer, 64, bitIndex, ul), "read ullong");
	assertBinary(boolValue, "bool roundtrip");
	assertBinary(us == 511, "ushort roundtrip");
	assertBinary(ui == 65535, "uint roundtrip");
	assertBinary(ul == 123456789ull, "ullong roundtrip");
}

void BinaryUtilityTest::testSignedIntegerBits()
{
	char buffer[128]{};
	int bitIndex = 0;
	assertBinary(writeSignedIntegerBit(buffer, 128, bitIndex, (int)-12345, true), "write signed int");
	assertBinary(writeSignedIntegerBit(buffer, 128, bitIndex, (short)321, true), "write signed short");
	assertBinary(writeSignedIntegerBit(buffer, 128, bitIndex, (llong)-9876543210ll, true), "write signed llong");
	bitIndex = 0;
	int i = 0;
	short s = 0;
	llong l = 0;
	assertBinary(readSignedIntegerBit(buffer, 128, bitIndex, i, true), "read signed int");
	assertBinary(readSignedIntegerBit(buffer, 128, bitIndex, s, true), "read signed short");
	assertBinary(readSignedIntegerBit(buffer, 128, bitIndex, l, true), "read signed llong");
	assertBinary(i == -12345, "signed int roundtrip");
	assertBinary(s == 321, "signed short roundtrip");
	assertBinary(l == -9876543210ll, "signed llong roundtrip");
}

void BinaryUtilityTest::testListMaxAndBitLists()
{
	Vector<int> signedList;
	signedList.add(-3);
	signedList.add(7);
	signedList.add(-12);
	signedList.add(5);
	assertBinary(BinaryUtility::getListMaxAbs(signedList.data(), signedList.size()) == 12, "list max abs");

	Vector<uint> unsignedList;
	unsignedList.add(1u);
	unsignedList.add(255u);
	unsignedList.add(512u);
	assertBinary(BinaryUtility::getListMax(unsignedList.data(), unsignedList.size()) == 512, "list max");

	char buffer[256]{};
	int bitIndex = 0;
	assertBinary(writeUnsignedIntegerListBit(buffer, 256, bitIndex, unsignedList), "write unsigned list");
	bitIndex = 0;
	Vector<uint> unsignedRead;
	assertBinary(readUnsignedIntegerListBit(buffer, 256, bitIndex, unsignedRead), "read unsigned list");
	assertBinary(unsignedRead.size() == 3, "unsigned list size");
	assertBinary(unsignedRead[1] == 255u, "unsigned list value");

	memset(buffer, 0, 256);
	bitIndex = 0;
	Vector<int> signedRead;
	assertBinary(writeSignedIntegerListBit(buffer, 256, bitIndex, signedList, true), "write signed list");
	bitIndex = 0;
	assertBinary(readSignedIntegerListBit(buffer, 256, bitIndex, signedRead, true), "read signed list");
	assertBinary(signedRead.size() == 4, "signed list size");
	assertBinary(signedRead[2] == -12, "signed list value");
}

void BinaryUtilityTest::testFloatDoubleLists()
{
	Vector<float> floats;
	floats.add(1.25f);
	floats.add(-2.5f);
	floats.add(3.75f);
	Vector<double> doubles;
	doubles.add(10.125);
	doubles.add(-20.25);
	doubles.add(30.5);

	char buffer[512]{};
	int bitIndex = 0;
	assertBinary(writeFloatListBit(buffer, 512, bitIndex, floats, true, 2), "write float list");
	assertBinary(writeDoubleListBit(buffer, 512, bitIndex, doubles, true, 3), "write double list");
	bitIndex = 0;
	Vector<float> floatsRead;
	Vector<double> doublesRead;
	assertBinary(readFloatListBit(buffer, 512, bitIndex, floatsRead, true, 2), "read float list");
	assertBinary(readDoubleListBit(buffer, 512, bitIndex, doublesRead, true, 3), "read double list");
	assertBinary(floatsRead.size() == 3, "float list size");
	assertBinary(doublesRead.size() == 3, "double list size");
	assertBinary(approxEqualFloat(floatsRead[0], 1.25f, 0.05f), "float list value");
	assertBinary(approxEqualDouble(doublesRead[2], 30.5, 0.05), "double list value");
}

void BinaryUtilityTest::testDeep()
{
	testBitCountAndFlags();
	testBitManipulation();
	testPrimitiveReadWrite();
	testVectorReadWrite();
	testColorAndBuffer();
	testUnsignedIntegerBits();
	testSignedIntegerBits();
	testListMaxAndBitLists();
	testFloatDoubleLists();
}

void BinaryUtilityTest::check(const bool result, const string& info)
{
	if (!result)
	{
		ERROR("BinaryUtilityTest FAILED: " + info);
	}
}

void BinaryUtilityTest::testULongBoundary()
{
	const ullong values[]
	{
		(1ULL << 62) - 1ULL,
		1ULL << 62,
		(1ULL << 62) + 1ULL,
		(1ULL << 63) - 1ULL,
		1ULL << 63,
		ULLONG_MAX - 1ULL,
		ULLONG_MAX,
	};
	FOR((int)(sizeof(values) / sizeof(values[0])))
	{
		SerializerBitWrite writer;
		writer.initCapacity(32);
		check(writer.writeUnsigned(values[i]), "ullong boundary write,index:" + IToS(i));
		check(writer.writeBool(false), "ullong boundary sentinel0 write,index:" + IToS(i));
		check(writer.writeBool(true), "ullong boundary sentinel1 write,index:" + IToS(i));

		SerializerBitRead reader(writer.getBuffer(), writer.getByteCount());
		ullong result = 0;
		check(reader.readUnsigned(result), "ullong boundary read,index:" + IToS(i));
		check(result == values[i], "ullong boundary value,index:" + IToS(i));
		bool sentinel0 = true;
		bool sentinel1 = false;
		check(reader.readBool(sentinel0) && !sentinel0, "ullong boundary sentinel0,index:" + IToS(i));
		check(reader.readBool(sentinel1) && sentinel1, "ullong boundary sentinel1,index:" + IToS(i));
		check(reader.getBitIndex() == writer.getBitCount(),
			"ullong boundary bitIndex,index:" + IToS(i) +
			",Write:" + IToS(writer.getBitCount()) +
			",Read:" + IToS(reader.getBitIndex()));
	}
}

void BinaryUtilityTest::testListCountOverflow()
{
	constexpr int OVERFLOW_COUNT = (int)USHRT_MAX + 1;
	char buffer[16]{};

	Vector<uint> unsignedValues;
	unsignedValues.resize(OVERFLOW_COUNT);
	int bitIndex = 3;
	check(!BinaryUtility::writeUnsignedIntegerListBit(buffer, (int)sizeof(buffer), bitIndex, unsignedValues),
		"unsigned Vector.Count=65536必须拒绝");
	check(bitIndex == 3, "unsigned Vector.Count=65536失败后bitIndex不能变化,Actual:" + IToS(bitIndex));

	Vector<int> signedValues;
	signedValues.resize(OVERFLOW_COUNT);
	bitIndex = 3;
	check(!BinaryUtility::writeSignedIntegerListBit(buffer, (int)sizeof(buffer), bitIndex, signedValues, true),
		"signed Vector.Count=65536必须拒绝");
	check(bitIndex == 3, "signed Vector.Count=65536失败后bitIndex不能变化,Actual:" + IToS(bitIndex));

	Vector<float> floatValues;
	floatValues.resize(OVERFLOW_COUNT);
	bitIndex = 3;
	check(!BinaryUtility::writeFloatListBit(buffer, (int)sizeof(buffer), bitIndex, floatValues, true, 3),
		"float Vector.Count=65536必须拒绝");
	check(bitIndex == 3, "float Vector.Count=65536失败后bitIndex不能变化,Actual:" + IToS(bitIndex));

	Vector<double> doubleValues;
	doubleValues.resize(OVERFLOW_COUNT);
	bitIndex = 3;
	check(!BinaryUtility::writeDoubleListBit(buffer, (int)sizeof(buffer), bitIndex, doubleValues, true, 4),
		"double Vector.Count=65536必须拒绝");
	check(bitIndex == 3, "double Vector.Count=65536失败后bitIndex不能变化,Actual:" + IToS(bitIndex));
}

void BinaryUtilityTest::testTruncatedULong()
{
	SerializerBitWrite writer;
	writer.initCapacity(32);
	check(writer.writeUnsigned(ULLONG_MAX), "truncated ullong write");
	check(writer.getByteCount() > 1, "truncated ullong前置条件");

	SerializerBitRead reader(writer.getBuffer(), writer.getByteCount() - 1);
	ullong result = 0;
	check(!reader.readUnsigned(result), "截断ullong必须读取失败");
}

void BinaryUtilityTest::testExtreme()
{
	testULongBoundary();
	testListCountOverflow();
	testTruncatedULong();
}

// ============================================================
// Consolidated utility coverage
// ============================================================
#define BINARYUTILITYTEST_UT_ASSERT(expr, msg) \
	do { if (!(expr)) { ERROR(string("BinaryUtilityTest FAIL: ") + (msg)); } } while(0)
#define BINARYUTILITYTEST_UT_ASSERT_EQ(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("BinaryUtilityTest FAIL: ") + (msg) + " | expected=" + IToS((int)(b)) + " actual=" + IToS((int)(a))); } } while(0)
#define BINARYUTILITYTEST_UT_ASSERT_EQ_LL(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("BinaryUtilityTest FAIL: ") + (msg) + " | expected=" + LLToS(b) + " actual=" + LLToS(a)); } } while(0)
#define BINARYUTILITYTEST_UT_ASSERT_FLOAT(a, b, msg) \
	do { if (fabsf((a) - (b)) > 1e-3f) { ERROR(string("BinaryUtilityTest FAIL: ") + (msg) + " | expected=" + FToS(b) + " actual=" + FToS(a)); } } while(0)
#define BINARYUTILITYTEST_UT_ASSERT_STR(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("BinaryUtilityTest FAIL: ") + (msg) + " | expected=" + (b) + " actual=" + (a)); } } while(0)

void BinaryUtilityTest::testBinary_BitOps()
{
	// hasBit
	BINARYUTILITYTEST_UT_ASSERT(BinaryUtility::hasBit((int)0b1010, 1), "hasBit(1010,1)=true");
	BINARYUTILITYTEST_UT_ASSERT(BinaryUtility::hasBit((int)0b1010, 3), "hasBit(1010,3)=true");
	BINARYUTILITYTEST_UT_ASSERT(!BinaryUtility::hasBit((int)0b1010, 0), "hasBit(1010,0)=false");
	BINARYUTILITYTEST_UT_ASSERT(!BinaryUtility::hasBit((int)0b1010, 2), "hasBit(1010,2)=false");

	// getBit
	BINARYUTILITYTEST_UT_ASSERT_EQ(BinaryUtility::getBit((int)0b1010, 1), 1, "getBit(1010,1)=1");
	BINARYUTILITYTEST_UT_ASSERT_EQ(BinaryUtility::getBit((int)0b1010, 0), 0, "getBit(1010,0)=0");

	// setBitOne / setBitZero
	int v = 0b0000;
	BinaryUtility::setBitOne(v, 2);
	BINARYUTILITYTEST_UT_ASSERT_EQ(v, 0b0100, "setBitOne(2)->0b0100");
	BinaryUtility::setBitZero(v, 2);
	BINARYUTILITYTEST_UT_ASSERT_EQ(v, 0b0000, "setBitZero(2)->0b0000");

	// setBit
	BinaryUtility::setBit(v, 3, 1);
	BINARYUTILITYTEST_UT_ASSERT_EQ(v, 0b1000, "setBit(3,1)->0b1000");
	BinaryUtility::setBit(v, 3, 0);
	BINARYUTILITYTEST_UT_ASSERT_EQ(v, 0b0000, "setBit(3,0)->0b0000");
}

void BinaryUtilityTest::testBinary_HighLowBit()
{
	// getHighestBit on byte
	byte val8 = 0x80u;  // 1000 0000
	BINARYUTILITYTEST_UT_ASSERT_EQ((int)BinaryUtility::getHighestBit(val8), 1, "getHighestBit(0x80)=1");
	val8 = 0x7Fu;       // 0111 1111
	BINARYUTILITYTEST_UT_ASSERT_EQ((int)BinaryUtility::getHighestBit(val8), 0, "getHighestBit(0x7F)=0");

	// setHighestBit on int
	int iv = 0;
	BinaryUtility::setHighestBit(iv, true);
	BINARYUTILITYTEST_UT_ASSERT(BinaryUtility::getHighestBit(iv) == 1, "setHighestBit(true)->1");
	BinaryUtility::setHighestBit(iv, false);
	BINARYUTILITYTEST_UT_ASSERT(BinaryUtility::getHighestBit(iv) == 0, "setHighestBit(false)->0");

	// getLowestBit
	BINARYUTILITYTEST_UT_ASSERT_EQ(BinaryUtility::getLowestBit((int)0b101), 1, "getLowestBit(101)=1");
	BINARYUTILITYTEST_UT_ASSERT_EQ(BinaryUtility::getLowestBit((int)0b110), 0, "getLowestBit(110)=0");
}

void BinaryUtilityTest::testBinary_ByteOps()
{
	// little-endian layout: pos=0 is lowest byte
	int v = 0x01020304;
	BINARYUTILITYTEST_UT_ASSERT_EQ((int)BinaryUtility::getByte(v, 0), 0x04, "getByte(0x01020304,0)=0x04");
	BINARYUTILITYTEST_UT_ASSERT_EQ((int)BinaryUtility::getByte(v, 1), 0x03, "getByte(0x01020304,1)=0x03");
	BINARYUTILITYTEST_UT_ASSERT_EQ((int)BinaryUtility::getByte(v, 2), 0x02, "getByte(0x01020304,2)=0x02");
	BINARYUTILITYTEST_UT_ASSERT_EQ((int)BinaryUtility::getByte(v, 3), 0x01, "getByte(0x01020304,3)=0x01");

	// setByte
	int w = 0;
	BinaryUtility::setByte(w, (byte)0xABu, 1);
	BINARYUTILITYTEST_UT_ASSERT_EQ((int)BinaryUtility::getByte(w, 1), 0xAB, "setByte(0xAB,1)->getByte=0xAB");
	BINARYUTILITYTEST_UT_ASSERT_EQ((int)BinaryUtility::getByte(w, 0), 0x00, "setByte does not affect pos=0");
}

void BinaryUtilityTest::testBinary_GetBits()
{
	// byte 0b11011010: bits[1..3]=0b101=5
	BINARYUTILITYTEST_UT_ASSERT_EQ((int)BinaryUtility::getBits((byte)0b11011010u, 1, 3), 0b101, "getBits(11011010,1,3)=0b101");
	// bits[4..7]=0b1101=13
	BINARYUTILITYTEST_UT_ASSERT_EQ((int)BinaryUtility::getBits((byte)0b11011010u, 4, 4), 0b1101, "getBits(11011010,4,4)=0b1101");
	// all zeros
	BINARYUTILITYTEST_UT_ASSERT_EQ((int)BinaryUtility::getBits((byte)0u, 0, 8), 0, "getBits(0,0,8)=0");
}

void BinaryUtilityTest::testBinary_BitToByteCount()
{
	BINARYUTILITYTEST_UT_ASSERT_EQ(BinaryUtility::bitCountToByteCount(0), 0, "bitToByteCount(0)=0");
	BINARYUTILITYTEST_UT_ASSERT_EQ(BinaryUtility::bitCountToByteCount(1), 1, "bitToByteCount(1)=1");
	BINARYUTILITYTEST_UT_ASSERT_EQ(BinaryUtility::bitCountToByteCount(8), 1, "bitToByteCount(8)=1");
	BINARYUTILITYTEST_UT_ASSERT_EQ(BinaryUtility::bitCountToByteCount(9), 2, "bitToByteCount(9)=2");
	BINARYUTILITYTEST_UT_ASSERT_EQ(BinaryUtility::bitCountToByteCount(16), 2, "bitToByteCount(16)=2");
	BINARYUTILITYTEST_UT_ASSERT_EQ(BinaryUtility::bitCountToByteCount(17), 3, "bitToByteCount(17)=3");
}

void BinaryUtilityTest::testBinary_ListMax()
{
	Vector<uint> uList{ 5u, 3u, 9u, 1u };
	BINARYUTILITYTEST_UT_ASSERT_EQ((int)BinaryUtility::getListMax(uList), 9, "getListMax<uint>{5,3,9,1}=9");

	Vector<uint> uSingle{ 42u };
	BINARYUTILITYTEST_UT_ASSERT_EQ((int)BinaryUtility::getListMax(uSingle), 42, "getListMax<uint>{42}=42");

	Vector<int> sAbsList{ -10, 3, -7, 2 };
	BINARYUTILITYTEST_UT_ASSERT_EQ(BinaryUtility::getListMaxAbs(sAbsList), 10, "getListMaxAbs{-10,3,-7,2}=10");

	Vector<int> allPos{ 1, 5, 3 };
	BINARYUTILITYTEST_UT_ASSERT_EQ(BinaryUtility::getListMaxAbs(allPos), 5, "getListMaxAbs{1,5,3}=5");
}

void BinaryUtilityTest::testBinaryUtility()
{
	testBinary_BitOps();
	testBinary_HighLowBit();
	testBinary_ByteOps();
	testBinary_GetBits();
	testBinary_BitToByteCount();
	testBinary_ListMax();
}
#endif
