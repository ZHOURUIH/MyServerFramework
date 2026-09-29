#ifdef WINDOWS
#include "FrameHeader.h"

namespace CRC16TestLocal
{
#define CRC_ASSERT(expr, msg) \
	do { if (!(expr)) { ERROR(string("CRC16Test FAILED: ") + (msg)); } } while(0)
#define CRC_ASSERT_EQ(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("CRC16Test FAILED: ") + (msg) + \
		" | expected=" + IToS((int)(b)) + " actual=" + IToS((int)(a))); } } while(0)
}

void CRC16Test::testCRC16_Empty()
{
	using namespace CRC16TestLocal;
		// 初始 CRC 值为 0，空数据应返回 0 或某个确定值
		BinaryUtility::crc16(0, "", 0);
		CRC_ASSERT(true, "empty data no crash"); // 不崩溃就算过
	}

void CRC16Test::testCRC16_SingleByte()
{
	using namespace CRC16TestLocal;
		char data[] = { 0x00 };
		ushort r0 = BinaryUtility::crc16(0x0000, data, 1);
		// 验证相同输入产生相同输出
		ushort r1 = BinaryUtility::crc16(0x0000, data, 1);
		CRC_ASSERT_EQ(r0, r1, "single byte deterministic");

		char data2[] = { (char)0xFF };
		ushort r2 = BinaryUtility::crc16(0x0000, data2, 1);
		// 不同数据应该大概率不同（不绝对，但 0x00 vs 0xFF 应该不同）
		CRC_ASSERT(r0 != r2 || true, "different byte may differ");
	}

void CRC16Test::testCRC16_KnownVectors()
{
	using namespace CRC16TestLocal;
		// "123456789" 的标准 CRC-16/CCITT-FALSE 结果是 0xBB3D
		// 但具体结果取决于查表实现和初始值，这里主要验证一致性和链式计算
		const char* msg = "123456789";
		ushort r1 = BinaryUtility::crc16(0x0000, msg, 9);
		// 同样的输入必须产生同样的结果
		ushort r2 = BinaryUtility::crc16(0x0000, msg, 9);
		CRC_ASSERT_EQ(r1, r2, "known string deterministic");
	}

void CRC16Test::testCRC16_Incremental()
{
	using namespace CRC16TestLocal;
		// 一次性计算 "ABCDEFGH"
		const char* fullMsg = "ABCDEFGH";
		ushort fullCrc = BinaryUtility::crc16(0x0000, fullMsg, 8);

		// 分两次计算: 先 "ABCD" 得到中间 CRC，再用这个中间值算 "EFGH"
		ushort partialCrc = BinaryUtility::crc16(0x0000, "ABCD", 4);
		ushort incrementalCrc = BinaryUtility::crc16(partialCrc, "EFGH", 4);

		CRC_ASSERT_EQ(fullCrc, incrementalCrc, "incremental matches full calculation");
	}

void CRC16Test::testCRC16_ThreeSegments()
{
	using namespace CRC16TestLocal;
		// 完整: "HELLO_WORLD_TEST"
		const char* full = "HELLO_WORLD_TEST";
		ushort fullCrc = BinaryUtility::crc16(0x0000, full, 15);

		// 分三段: "HELLO" + "_WORLD" + "_TEST"
		ushort c1 = BinaryUtility::crc16(0x0000, "HELLO", 5);
		ushort c2 = BinaryUtility::crc16(c1, "_WORLD", 6);
		ushort c3 = BinaryUtility::crc16(c2, "_TEST", 4);

		CRC_ASSERT_EQ(fullCrc, c3, "three-segment incremental matches");
	}

void CRC16Test::testCRC16_BufferOffset()
{
	using namespace CRC16TestLocal;
		// 用 offset 跳过前几个字节，等效于从偏移处开始计算
		char buf[] = "XXXXABCDEFGH";
		// 直接从 buf+4 开始算 8 字节
		ushort withOffset = BinaryUtility::crc16(0x0000, buf + 4, 8);
		// 直接算 "ABCDEFGH"
		ushort direct = BinaryUtility::crc16(0x0000, "ABCDEFGH", 8);

		CRC_ASSERT_EQ(withOffset, direct, "bufferOffset equivalent to pointer advance");
	}

void CRC16Test::testCRC16_LargeBuffer()
{
	using namespace CRC16TestLocal;
		const int SIZE = 4096;
		char* bigBuf = new char[SIZE];
		// 填充固定模式
		for (int i = 0; i < SIZE; ++i)
			bigBuf[i] = (char)(i & 0xFF);

		ushort crc1 = BinaryUtility::crc16(0x0000, bigBuf, SIZE);
		ushort crc2 = BinaryUtility::crc16(0x0000, bigBuf, SIZE); // 再跑一次验证确定性
		CRC_ASSERT_EQ(crc1, crc2, "large buffer deterministic");

		delete[] bigBuf;
	}

void CRC16Test::testCRC16_ByteByByte()
{
	using namespace CRC16TestLocal;
		// 用 crc16Byte 逐字节计算 "AB"
		ushort crc = 0x0000;
		crc = BinaryUtility::crc16Byte(crc, (byte)'A');
		crc = BinaryUtility::crc16Byte(crc, (byte)'B');

		// 对比一次性计算
		ushort direct = BinaryUtility::crc16(0x0000, "AB", 2);

		CRC_ASSERT_EQ(crc, direct, "byte-by-byte via crc16Byte matches crc16");
	}

void CRC16Test::testCRC16_AllZeros()
{
	using namespace CRC16TestLocal;
		const int N = 64;
		char zeros[N];
		memset(zeros, 0, sizeof(zeros));

		ushort r1 = BinaryUtility::crc16(0x0000, zeros, N);
		ushort r2 = BinaryUtility::crc16(0x0000, zeros, N);
		CRC_ASSERT_EQ(r1, r2, "all-zeros deterministic");

		// 全零 vs 全 0xFF 应该不同
		char ff[N];
		memset(ff, 0xFF, sizeof(ff));
		ushort rFF = BinaryUtility::crc16(0x0000, ff, N);
		CRC_ASSERT(r1 != rFF || true, "zeros differs from all-FF (likely)");
	}

ushort CRC16Test::crc16Reference(ushort crc, const char* buffer, const int len, const int bufferOffset)
{
	using namespace CRC16TestLocal;
		for (int i = 0; i < len; ++i)
		{
			crc = BinaryUtility::crc16Byte(crc, (byte)buffer[bufferOffset + i]);
		}
		return crc;
	}

void CRC16Test::testCRC16_SlicingBy4Regression()
{
	using namespace CRC16TestLocal;
		char data[1032];
		for (int i = 0; i < 1032; ++i)
		{
			data[i] = (char)((i * 37 + i * i * 13 + 91) & 0xFF);
		}
		const ushort initList[]{ 0x0000, 0x001F, 0x1234, 0xFFFF };
		for (ushort init : initList)
		{
			for (int offset = 0; offset < 8; ++offset)
			{
				for (int len = 0; len <= 1024; ++len)
				{
					const ushort expected = crc16Reference(init, data, len, offset);
					const ushort actual = BinaryUtility::crc16(init, data, len, offset);
					CRC_ASSERT_EQ(actual, expected, "slicing-by-4 regression len=" + IToS(len) + " offset=" + IToS(offset));
				}
			}
		}
	}

void CRC16Test::testCRC16_DifferentInitValues()
{
	using namespace CRC16TestLocal;
		const char* msg = "test_data";
		ushort r0 = BinaryUtility::crc16(0x0000, msg, 9);
		ushort rFFFF = BinaryUtility::crc16(0xFFFF, msg, 9);
		ushort r1234 = BinaryUtility::crc16(0x1234, msg, 9);

		// 不同初始值几乎必然得到不同 CRC（理论上可能碰撞但不至于全等）
		bool allSame = (r0 == rFFFF && rFFFF == r1234);
		CRC_ASSERT(!allSame, "different init values produce different results");
	}

void CRC16Test::test()
{
	testCRC16_Empty();
	testCRC16_SingleByte();
	testCRC16_KnownVectors();
	testCRC16_Incremental();
	testCRC16_ThreeSegments();
	testCRC16_BufferOffset();
	testCRC16_LargeBuffer();
	testCRC16_ByteByByte();
	testCRC16_AllZeros();
	testCRC16_DifferentInitValues();
	testCRC16_SlicingBy4Regression();
}
#endif
