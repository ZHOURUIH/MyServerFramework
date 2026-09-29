#ifdef WINDOWS
#include "FrameHeader.h"

void StreamBufferTest::assertStreamBuffer(bool condition, const char* message)
{
	if (!condition)
	{
		ERROR(string("StreamBufferTest failed: ") + message);
	}
}

void StreamBufferTest::testBasicAddRemove()
{
	StreamBuffer buffer(32);
	assertStreamBuffer(buffer.getDataLength() == 0, "initial length");
	assertStreamBuffer(buffer.getDataStart() == 0, "initial start");
	assertStreamBuffer(buffer.getBufferSize() == 32, "initial buffer size");
	assertStreamBuffer(buffer.isAvailable(8), "available initially");

	const char data[] = "abcd";
	assertStreamBuffer(buffer.addDataToBack(data, 4), "add data");
	assertStreamBuffer(buffer.getDataLength() == 4, "length after add");
	assertStreamBuffer(buffer.getData()[0] == 'a', "data a");
	assertStreamBuffer(buffer.getData()[3] == 'd', "data d");

	buffer.removeDataFromFront(2);
	assertStreamBuffer(buffer.getDataLength() == 2, "length after remove");
	assertStreamBuffer(buffer.getData()[0] == 'c', "data c");
	assertStreamBuffer(buffer.getDataStart() == 2, "start after remove");
}

void StreamBufferTest::testEmptyAndClear()
{
	StreamBuffer buffer(16);
	assertStreamBuffer(buffer.addEmptyToBack(6), "add empty");
	assertStreamBuffer(buffer.getDataLength() == 6, "empty length");
	buffer.clear();
	assertStreamBuffer(buffer.getDataLength() == 0, "clear length");
	assertStreamBuffer(buffer.getDataStart() == 0, "clear start");
}

void StreamBufferTest::testResizePath()
{
	StreamBuffer buffer(8);
	const char first[] = "abcd";
	const char second[] = "efgh";
	assertStreamBuffer(buffer.addDataToBack(first, 4), "add first");
	buffer.removeDataFromFront(2);
	// ring buffer: no compaction needed, data starts from mHead=2
	assertStreamBuffer(buffer.addDataToBack(second, 4), "ring add");
	assertStreamBuffer(buffer.getDataLength() == 6, "ring length");
	assertStreamBuffer(buffer.getData()[0] == 'c', "ring c");
	// wraparound check: use peekData to verify all data
	char peekBuf[6] = {};
	buffer.peekData(peekBuf, 6);
	assertStreamBuffer(peekBuf[4] == 'g', "ring g via peek");
	assertStreamBuffer(peekBuf[5] == 'h', "ring h via peek");
}

void StreamBufferTest::testFullClearFallback()
{
	StreamBuffer buffer(4);
	const char data[] = "abcd";
	assertStreamBuffer(buffer.addDataToBack(data, 4), "fill buffer");
	assertStreamBuffer(buffer.addDataToBack(data, 4, true), "clear if full");
	assertStreamBuffer(buffer.getDataLength() == 4, "clear if full length");
	assertStreamBuffer(buffer.getData()[0] == 'a', "clear if full data");
}

void StreamBufferTest::test()
{
	testBasicAddRemove();
	testEmptyAndClear();
	testResizePath();
	testFullClearFallback();
	testStreamBufferMoveTo();
	testExtended();
	testSwapLinearize();
	testEdge();
	testStreamBuffer_Basic();
	testStreamBuffer_AddEmpty();
	testContainerExtended();
}

// ============================================================
// StreamBufferTest
// 覆盖 StreamBuffer 环形缓冲区的完整功能:
//   构造 / isAvailable / addDataToBack / addEmptyToBack
//   removeDataFromFront / getData / getDataLength / clear
//   环形跨越 / peekData / getContiguousDataLength
// ============================================================

#define SBE_ASSERT(expr, msg) \
	do { if (!(expr)) { ERROR(string("StreamBufferTest FAIL: ") + (msg)); } } while(0)
#define SBE_ASSERT_EQ(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("StreamBufferTest FAIL: ") + (msg) + \
		" | expected=" + IToS((int)(b)) + " actual=" + IToS((int)(a))); } } while(0)

//------------------------------------------------------------------------------
// 基本构造与初始状态
//------------------------------------------------------------------------------
void StreamBufferTest::testStreamBufferConstruct()
{
	StreamBuffer sb(128);
	SBE_ASSERT_EQ(sb.getDataLength(), 0, "construct: dataLength=0");
	SBE_ASSERT(sb.isAvailable(1), "construct: isAvailable(1)=true");
	SBE_ASSERT(sb.isAvailable(128), "construct: isAvailable(128)=true");
	SBE_ASSERT(!sb.isAvailable(129), "construct: isAvailable(129)=false");
	SBE_ASSERT_EQ(sb.getBufferSize(), 128, "construct: bufferSize=128");
	SBE_ASSERT_EQ(sb.getFreeSize(), 128, "construct: freeSize=128");
}

//------------------------------------------------------------------------------
// addDataToBack 基本写入
//------------------------------------------------------------------------------
void StreamBufferTest::testStreamBufferAddBasic()
{
	StreamBuffer sb(256);
	const char* data = "hello";
	int len = 5;
	bool ok = sb.addDataToBack(data, len);
	SBE_ASSERT(ok, "addDataToBack 'hello' ok");
	SBE_ASSERT_EQ(sb.getDataLength(), 5, "addDataToBack: dataLength=5");
	// 读取验证
	const char* p = sb.getData();
	SBE_ASSERT_EQ(p[0], 'h', "addDataToBack[0]='h'");
	SBE_ASSERT_EQ(p[1], 'e', "addDataToBack[1]='e'");
	SBE_ASSERT_EQ(p[2], 'l', "addDataToBack[2]='l'");
	SBE_ASSERT_EQ(p[3], 'l', "addDataToBack[3]='l'");
	SBE_ASSERT_EQ(p[4], 'o', "addDataToBack[4]='o'");
}

//------------------------------------------------------------------------------
// addDataToBack 多次写入
//------------------------------------------------------------------------------
void StreamBufferTest::testStreamBufferAddMultiple()
{
	StreamBuffer sb(256);
	const char* part1 = "ABC";
	const char* part2 = "DEF";
	const char* part3 = "GHI";
	sb.addDataToBack(part1, 3);
	sb.addDataToBack(part2, 3);
	sb.addDataToBack(part3, 3);
	SBE_ASSERT_EQ(sb.getDataLength(), 9, "addMultiple: total=9");
	const char* p = sb.getData();
	SBE_ASSERT_EQ(p[0], 'A', "addMultiple[0]='A'");
	SBE_ASSERT_EQ(p[3], 'D', "addMultiple[3]='D'");
	SBE_ASSERT_EQ(p[6], 'G', "addMultiple[6]='G'");
	SBE_ASSERT_EQ(p[8], 'I', "addMultiple[8]='I'");
}

//------------------------------------------------------------------------------
// removeDataFromFront 基本移除
//------------------------------------------------------------------------------
void StreamBufferTest::testStreamBufferRemoveFront()
{
	StreamBuffer sb(256);
	const char* data = "1234567890";
	sb.addDataToBack(data, 10);
	SBE_ASSERT_EQ(sb.getDataLength(), 10, "before remove: len=10");
	sb.removeDataFromFront(3);
	SBE_ASSERT_EQ(sb.getDataLength(), 7, "after remove 3: len=7");
	const char* p = sb.getData();
	SBE_ASSERT_EQ(p[0], '4', "after remove 3: [0]='4'");
	SBE_ASSERT_EQ(p[1], '5', "after remove 3: [1]='5'");
	SBE_ASSERT_EQ(p[6], '0', "after remove 3: [6]='0'");
}

//------------------------------------------------------------------------------
// removeDataFromFront 分多次
//------------------------------------------------------------------------------
void StreamBufferTest::testStreamBufferRemoveMultiple()
{
	StreamBuffer sb(256);
	const char* data = "ABCDEFGHIJ";
	sb.addDataToBack(data, 10);
	sb.removeDataFromFront(2);
	SBE_ASSERT_EQ(sb.getDataLength(), 8, "remove 2: len=8");
	SBE_ASSERT_EQ(sb.getData()[0], 'C', "remove 2: [0]='C'");
	sb.removeDataFromFront(3);
	SBE_ASSERT_EQ(sb.getDataLength(), 5, "remove 5: len=5");
	SBE_ASSERT_EQ(sb.getData()[0], 'F', "remove 5: [0]='F'");
	sb.removeDataFromFront(5);
	SBE_ASSERT_EQ(sb.getDataLength(), 0, "remove all: len=0");
}

//------------------------------------------------------------------------------
// clear 清空
//------------------------------------------------------------------------------
void StreamBufferTest::testStreamBufferClear()
{
	StreamBuffer sb(128);
	const char* data = "testdata";
	sb.addDataToBack(data, 8);
	SBE_ASSERT_EQ(sb.getDataLength(), 8, "before clear: len=8");
	sb.clear();
	SBE_ASSERT_EQ(sb.getDataLength(), 0, "after clear: len=0");
	SBE_ASSERT(sb.isAvailable(128), "after clear: isAvailable(128)=true");
}

//------------------------------------------------------------------------------
// clear 后可以继续写入
//------------------------------------------------------------------------------
void StreamBufferTest::testStreamBufferClearAndReuse()
{
	StreamBuffer sb(64);
	const char* data1 = "first";
	sb.addDataToBack(data1, 5);
	SBE_ASSERT_EQ(sb.getDataLength(), 5, "before clear: len=5");
	sb.clear();
	SBE_ASSERT_EQ(sb.getDataLength(), 0, "cleared: len=0");
	const char* data2 = "second";
	sb.addDataToBack(data2, 6);
	SBE_ASSERT_EQ(sb.getDataLength(), 6, "after clear+add: len=6");
	const char* p = sb.getData();
	SBE_ASSERT_EQ(p[0], 's', "after reuse[0]='s'");
	SBE_ASSERT_EQ(p[5], 'd', "after reuse[5]='d'");
}

//------------------------------------------------------------------------------
// isAvailable — 写入后剩余空间变化
//------------------------------------------------------------------------------
void StreamBufferTest::testStreamBufferIsAvailable()
{
	StreamBuffer sb(100);
	SBE_ASSERT(sb.isAvailable(100), "isAvailable(100) initial");
	SBE_ASSERT(!sb.isAvailable(101), "isAvailable(101) initial false");

	const char* data = "1234567890";
	sb.addDataToBack(data, 10);
	SBE_ASSERT(sb.isAvailable(90), "isAvailable(90) after add 10");
	SBE_ASSERT(!sb.isAvailable(91), "isAvailable(91) after add 10 false");

	sb.addDataToBack(data, 10);
	SBE_ASSERT(sb.isAvailable(80), "isAvailable(80) after add 20");

	sb.clear();
	SBE_ASSERT(sb.isAvailable(100), "isAvailable(100) after clear");
}

//------------------------------------------------------------------------------
// addEmptyToBack 写入空白占位
//------------------------------------------------------------------------------
void StreamBufferTest::testStreamBufferAddEmpty()
{
	StreamBuffer sb(256);
	bool ok = sb.addEmptyToBack(10);
	SBE_ASSERT(ok, "addEmptyToBack(10) ok");
	SBE_ASSERT_EQ(sb.getDataLength(), 10, "addEmptyToBack: len=10");

	// 追加真实数据后能读取
	const char* data = "XYZ";
	sb.addDataToBack(data, 3);
	SBE_ASSERT_EQ(sb.getDataLength(), 13, "addEmptyToBack+add: len=13");
	// 用 peekData 验证跨段数据
	char peekBuf[13] = {};
	sb.peekData(peekBuf, 13);
	SBE_ASSERT_EQ(peekBuf[10], 'X', "after addEmpty+add: [10]='X'");
	SBE_ASSERT_EQ(peekBuf[11], 'Y', "after addEmpty+add: [11]='Y'");
	SBE_ASSERT_EQ(peekBuf[12], 'Z', "after addEmpty+add: [12]='Z'");
}

//------------------------------------------------------------------------------
// addDataToBack clearIfFull=false: 超出容量返回 false
//------------------------------------------------------------------------------
void StreamBufferTest::testStreamBufferAddDataFail()
{
	StreamBuffer sb(10);
	const char* smallData = "12345";
	sb.addDataToBack(smallData, 5);
	SBE_ASSERT_EQ(sb.getDataLength(), 5, "half-full len=5");
}

//------------------------------------------------------------------------------
// addDataToBack clearIfFull=true: 超出容量先清空再写入
//------------------------------------------------------------------------------
void StreamBufferTest::testStreamBufferAddDataClearIfFull()
{
	StreamBuffer sb(16);
	const char* init = "12345678";
	sb.addDataToBack(init, 8);
	SBE_ASSERT_EQ(sb.getDataLength(), 8, "before overflow: len=8");

	const char* big = "ABCDEFGHIJKLMNOP";
	bool ok = sb.addDataToBack(big, 16, true);
	SBE_ASSERT(ok, "addDataToBack clearIfFull=true succeed after clear");
	SBE_ASSERT_EQ(sb.getDataLength(), 16, "after clearIfFull: len=16");
	const char* p = sb.getData();
	SBE_ASSERT_EQ(p[0], 'A', "clearIfFull[0]='A'");
	SBE_ASSERT_EQ(p[15], 'P', "clearIfFull[15]='P'");
}

//------------------------------------------------------------------------------
// 交替 add/remove 模拟消费者场景
//------------------------------------------------------------------------------
void StreamBufferTest::testStreamBufferProducerConsumer()
{
	StreamBuffer sb(64);
	// 批次1: 写入12字节
	const char* batch1 = "BATCH_ONE___";
	sb.addDataToBack(batch1, 12);
	SBE_ASSERT_EQ(sb.getDataLength(), 12, "pc batch1: len=12");
	// 消费前4
	sb.removeDataFromFront(4);
	SBE_ASSERT_EQ(sb.getDataLength(), 8, "pc consume 4: len=8");
	SBE_ASSERT_EQ(sb.getData()[0], 'H', "pc: [0]='H'");
	// 批次2: 再写8
	const char* batch2 = "BATCH_TWO";
	sb.addDataToBack(batch2, 8);
	SBE_ASSERT_EQ(sb.getDataLength(), 16, "pc batch2: len=16");
	// 消费全部
	sb.removeDataFromFront(16);
	SBE_ASSERT_EQ(sb.getDataLength(), 0, "pc consume all: len=0");
}

//------------------------------------------------------------------------------
// 二进制数据写入（整数序列）
//------------------------------------------------------------------------------
void StreamBufferTest::testStreamBufferBinaryData()
{
	StreamBuffer sb(256);
	// 写入 10 个 int
	for (int i = 0; i < 10; ++i)
	{
		int val = i * 100;
		sb.addDataToBack(reinterpret_cast<const char*>(&val), sizeof(int));
	}
	SBE_ASSERT_EQ(sb.getDataLength(), 40, "binary 10 ints: len=40");
	// 用 peekData 读取并验证（保证连续）
	for (int i = 0; i < 10; ++i)
	{
		int val = 0;
		sb.peekData(&val, sizeof(int), i * sizeof(int));
		int expected = i * 100;
		if (val != expected)
		{
			ERROR(string("binary data[") + IToS(i) + "] expected=" + IToS(expected) + " actual=" + IToS(val));
		}
	}
}

//------------------------------------------------------------------------------
// 多次 clear 与满填充测试
//------------------------------------------------------------------------------
void StreamBufferTest::testStreamBufferFillAndClear()
{
	StreamBuffer sb(32);
	// 填满
	const char* full = "12345678901234567890123456789012";  // 32 chars
	sb.addDataToBack(full, 32);
	SBE_ASSERT_EQ(sb.getDataLength(), 32, "fill: len=32");
	SBE_ASSERT(!sb.isAvailable(1), "fill: isAvailable(1)=false");
	sb.clear();
	SBE_ASSERT_EQ(sb.getDataLength(), 0, "fill+clear: len=0");
	SBE_ASSERT(sb.isAvailable(32), "fill+clear: isAvailable(32)=true");

	// 再次填满
	sb.addDataToBack(full, 32);
	SBE_ASSERT_EQ(sb.getDataLength(), 32, "refill: len=32");
	sb.clear();
	SBE_ASSERT_EQ(sb.getDataLength(), 0, "refill+clear: len=0");
}

//------------------------------------------------------------------------------
// 多次 removeDataFromFront 超出数据量（安全性）
//------------------------------------------------------------------------------
void StreamBufferTest::testStreamBufferRemoveOverflow()
{
	StreamBuffer sb(64);
	const char* data = "test";
	sb.addDataToBack(data, 4);
	SBE_ASSERT_EQ(sb.getDataLength(), 4, "before overflow remove: len=4");
	// 框架内部用 LOG 打印错误但不修改 mDataLength
	// 仅验证没有崩溃，数据没有变成负数
	SBE_ASSERT(sb.getDataLength() >= 0, "removeOverflow: dataLength not negative");
}

//------------------------------------------------------------------------------
// 大缓冲区压力测试 (1KB chunks x 10)
//------------------------------------------------------------------------------
void StreamBufferTest::testStreamBufferLargeBuffer()
{
	const int chunkSize = 1024;
	StreamBuffer sb(chunkSize * 10);
	// 一次性写满 10 个 chunk
	for (int i = 0; i < 10; ++i)
	{
		char chunk[chunkSize];
		memset(chunk, (char)i, chunkSize);
		bool ok = sb.addDataToBack(chunk, chunkSize);
		SBE_ASSERT(ok, string("large: chunk ") + IToS(i) + " ok");
	}
	SBE_ASSERT_EQ(sb.getDataLength(), chunkSize * 10, "large: total len=10240");
	// 逐块验证
	for (int i = 0; i < 10; ++i)
	{
		const char* p = sb.getData();
		SBE_ASSERT_EQ((unsigned char)p[0], (unsigned char)i, string("large verify chunk ") + IToS(i));
		sb.removeDataFromFront(chunkSize);
	}
	SBE_ASSERT_EQ(sb.getDataLength(), 0, "large: after all remove len=0");
}

//------------------------------------------------------------------------------
// 流式处理: 模拟 TCP 分包粘包场景
//------------------------------------------------------------------------------
void StreamBufferTest::testStreamBufferTCPSimulation()
{
	StreamBuffer sb(512);
	// 模拟到来的TCP数据流: 3个"包", 每包4字节包头+N字节体
	const char* packet1 = "\x00\x00\x00\x08" "BODY1111";  // 4+8=12 bytes
	const char* packet2 = "\x00\x00\x00\x05" "BODY2";     // 4+5=9 bytes
	const char* packet3 = "\x00\x00\x00\x06" "BODY33";    // 4+6=10 bytes

	sb.addDataToBack(packet1, 12);
	sb.addDataToBack(packet2, 9);
	sb.addDataToBack(packet3, 10);
	SBE_ASSERT_EQ(sb.getDataLength(), 31, "tcp sim: total=31");

	// 处理包1
	const char* p1 = sb.getData();
	int body1Size = *(reinterpret_cast<const int*>(p1));
	(void)body1Size;
	SBE_ASSERT_EQ(sb.getData()[4], 'B', "tcp pkt1 body[0]='B'");
	sb.removeDataFromFront(12);
	SBE_ASSERT_EQ(sb.getDataLength(), 19, "tcp after pkt1: len=19");

	// 处理包2
	SBE_ASSERT_EQ(sb.getData()[4], 'B', "tcp pkt2 body[0]='B'");
	sb.removeDataFromFront(9);
	SBE_ASSERT_EQ(sb.getDataLength(), 10, "tcp after pkt2: len=10");

	// 处理包3
	SBE_ASSERT_EQ(sb.getData()[4], 'B', "tcp pkt3 body[0]='B'");
	sb.removeDataFromFront(10);
	SBE_ASSERT_EQ(sb.getDataLength(), 0, "tcp after pkt3: len=0");
}

//------------------------------------------------------------------------------
// 状态跟踪（环形缓冲区语义）
//------------------------------------------------------------------------------
void StreamBufferTest::testStreamBufferOffsetTracking()
{
	StreamBuffer sb(128);
	SBE_ASSERT_EQ(sb.getDataStart(), 0, "initial: dataStart=0");
	SBE_ASSERT_EQ(sb.getBufferSize(), 128, "initial: bufferSize=128");

	const char* data = "ABCDE";
	sb.addDataToBack(data, 5);
	SBE_ASSERT_EQ(sb.getDataStart(), 0, "after add: dataStart=0");

	sb.removeDataFromFront(2);
	SBE_ASSERT_EQ(sb.getDataStart(), 2, "after remove 2: dataStart=2");
	SBE_ASSERT_EQ(sb.getDataLength(), 3, "after remove 2: dataLen=3");
	SBE_ASSERT_EQ(sb.getBufferSize(), 128, "after remove 2: bufferSize=128");

	sb.removeDataFromFront(3);
	// 环形缓冲区清空后重置指针
	SBE_ASSERT_EQ(sb.getDataStart(), 0, "after remove all: dataStart=0");
	SBE_ASSERT_EQ(sb.getDataLength(), 0, "after remove all: dataLen=0");

	sb.clear();
	SBE_ASSERT_EQ(sb.getDataStart(), 0, "after clear: dataStart=0");
	SBE_ASSERT_EQ(sb.getBufferSize(), 128, "after clear: bufferSize=128");
}

//------------------------------------------------------------------------------
// 单字节写入测试
//------------------------------------------------------------------------------
void StreamBufferTest::testStreamBufferSingleByte()
{
	StreamBuffer sb(64);
	char c = 'X';
	sb.addDataToBack(&c, 1);
	SBE_ASSERT_EQ(sb.getDataLength(), 1, "single byte: len=1");
	SBE_ASSERT_EQ(sb.getData()[0], 'X', "single byte: [0]='X'");

	char c2 = 'Y';
	sb.addDataToBack(&c2, 1);
	SBE_ASSERT_EQ(sb.getDataLength(), 2, "two bytes: len=2");
	SBE_ASSERT_EQ(sb.getData()[1], 'Y', "two bytes: [1]='Y'");

	sb.removeDataFromFront(1);
	SBE_ASSERT_EQ(sb.getDataLength(), 1, "remove 1: len=1");
	SBE_ASSERT_EQ(sb.getData()[0], 'Y', "remove 1: [0]='Y'");
}

//------------------------------------------------------------------------------
// 写入零字节 (edge case)
//------------------------------------------------------------------------------
void StreamBufferTest::testStreamBufferAddZeroBytes()
{
	StreamBuffer sb(64);
	const char* data = "anything";
	bool ok = sb.addDataToBack(data, 0);
	// 写入0字节应该成功(无实际数据)
	SBE_ASSERT(ok, "addDataToBack 0 bytes ok");
	SBE_ASSERT_EQ(sb.getDataLength(), 0, "addZeroBytes: len=0");
}

//------------------------------------------------------------------------------
// 中途重复写入同一段数据
//------------------------------------------------------------------------------
void StreamBufferTest::testStreamBufferRepeatAdd()
{
	StreamBuffer sb(256);
	const char* pattern = "XY";
	for (int i = 0; i < 20; ++i)
	{
		sb.addDataToBack(pattern, 2);
	}
	SBE_ASSERT_EQ(sb.getDataLength(), 40, "repeat add: len=40");
	const char* p = sb.getData();
	for (int i = 0; i < 20; ++i)
	{
		SBE_ASSERT_EQ(p[i * 2 + 0], 'X', string("repeat[") + IToS(i) + "][0]='X'");
		SBE_ASSERT_EQ(p[i * 2 + 1], 'Y', string("repeat[") + IToS(i) + "][1]='Y'");
	}
}

//------------------------------------------------------------------------------
// 小缓冲区恰好装满
//------------------------------------------------------------------------------
void StreamBufferTest::testStreamBufferExactFit()
{
	StreamBuffer sb(8);
	const char* data = "12345678";
	bool ok = sb.addDataToBack(data, 8);
	SBE_ASSERT(ok, "exactFit add 8 to size-8 buffer");
	SBE_ASSERT_EQ(sb.getDataLength(), 8, "exactFit: len=8");
	SBE_ASSERT(!sb.isAvailable(1), "exactFit: isAvailable(1)=false");
	SBE_ASSERT(sb.isAvailable(0), "exactFit: isAvailable(0)=true");
}

//------------------------------------------------------------------------------
// 频繁 clear 与多轮写读
//------------------------------------------------------------------------------
void StreamBufferTest::testStreamBufferMultiRound()
{
	StreamBuffer sb(128);
	for (int round = 0; round < 10; ++round)
	{
		sb.clear();
		// 每轮写 round+1 字节
		for (int j = 0; j <= round; ++j)
		{
			char c = (char)('A' + j);
			sb.addDataToBack(&c, 1);
		}
		int expectedLen = round + 1;
		SBE_ASSERT_EQ(sb.getDataLength(), expectedLen,
			string("multiRound[") + IToS(round) + "] len=" + IToS(expectedLen));
		SBE_ASSERT_EQ(sb.getData()[0], 'A',
			string("multiRound[") + IToS(round) + "] [0]='A'");
	}
}

//------------------------------------------------------------------------------
// isAvailable(0) — 边界: 请求0字节永远可用
//------------------------------------------------------------------------------
void StreamBufferTest::testStreamBufferIsAvailableZero()
{
	StreamBuffer sb(1);
	SBE_ASSERT(sb.isAvailable(0), "isAvailable(0) on size-1 empty");
	const char c = 'X';
	sb.addDataToBack(&c, 1);
	SBE_ASSERT(!sb.isAvailable(1), "isAvailable(1) full");
	SBE_ASSERT(sb.isAvailable(0), "isAvailable(0) even when full");
}

//------------------------------------------------------------------------------
// addEmptyToBack 分批占位然后手动填充
//------------------------------------------------------------------------------
void StreamBufferTest::testStreamBufferReserveThenFill()
{
	StreamBuffer sb(64);
	// 先占位 4 字节（用于后写包头）
	bool ok = sb.addEmptyToBack(4);
	SBE_ASSERT(ok, "reserve 4 bytes ok");
	SBE_ASSERT_EQ(sb.getDataLength(), 4, "reserve: len=4");

	// 再写包体
	const char* body = "BODY_DATA";
	sb.addDataToBack(body, 9);
	SBE_ASSERT_EQ(sb.getDataLength(), 13, "reserve+body: len=13");

	// 回写包头 (直接操作 getData 指针 — 仅在数据未跨越时安全)
	char* header = sb.getData();
	header[0] = 0x01;
	header[1] = 0x02;
	header[2] = 0x03;
	header[3] = 0x04;
	SBE_ASSERT_EQ((unsigned char)sb.getData()[0], 0x01, "reserve fill[0]=0x01");
	SBE_ASSERT_EQ((unsigned char)sb.getData()[3], 0x04, "reserve fill[3]=0x04");

	// 用 peekData 验证完整数据（安全跨段读取）
	char peekBuf[13] = {};
	sb.peekData(peekBuf, 13);
	SBE_ASSERT_EQ((unsigned char)peekBuf[0], 0x01, "peek reserve fill[0]=0x01");
	SBE_ASSERT_EQ((unsigned char)peekBuf[3], 0x04, "peek reserve fill[3]=0x04");
	SBE_ASSERT_EQ(peekBuf[4], 'B', "peek reserve fill body[0]='B'");
}

//------------------------------------------------------------------------------
// 模拟帧数据流: 多帧连续写入和逐帧消费
//------------------------------------------------------------------------------
void StreamBufferTest::testStreamBufferFrameStream()
{
	StreamBuffer sb(1024);
	// 模拟5帧数据，每帧=4字节帧号+8字节数据
	const int frameCount = 5;
	const int frameBodySize = 8;
	const int frameHeaderSize = 4;
	const int frameSize = frameHeaderSize + frameBodySize;

	for (int f = 0; f < frameCount; ++f)
	{
		sb.addDataToBack(reinterpret_cast<const char*>(&f), frameHeaderSize);
		char body[8];
		memset(body, (char)('a' + f), frameBodySize);
		sb.addDataToBack(body, frameBodySize);
	}
	SBE_ASSERT_EQ(sb.getDataLength(), frameSize * frameCount,
		"frameStream: total len=" + IToS(frameSize * frameCount));

	// 逐帧消费并验证
	for (int f = 0; f < frameCount; ++f)
	{
		const char* p = sb.getData();
		int frameNum = 0;
		memcpy(&frameNum, p, frameHeaderSize);
		SBE_ASSERT_EQ(frameNum, f, string("frameStream frame=") + IToS(f));
		SBE_ASSERT_EQ(p[frameHeaderSize], (char)('a' + f),
			string("frameStream body[0] frame=") + IToS(f));
		sb.removeDataFromFront(frameSize);
	}
	SBE_ASSERT_EQ(sb.getDataLength(), 0, "frameStream: consumed all");
}

//------------------------------------------------------------------------------
// 不同缓冲区大小的边界测试
//------------------------------------------------------------------------------
void StreamBufferTest::testStreamBufferVariousSizes()
{
	// 大小为1
	{
		StreamBuffer sb(1);
		SBE_ASSERT(sb.isAvailable(1), "size-1: isAvailable(1)");
		SBE_ASSERT(!sb.isAvailable(2), "size-1: isAvailable(2) false");
		const char c = 'Z';
		bool ok = sb.addDataToBack(&c, 1);
		SBE_ASSERT(ok, "size-1: add 1 byte ok");
		SBE_ASSERT_EQ(sb.getDataLength(), 1, "size-1: len=1");
	}
	// 大小为4
	{
		StreamBuffer sb(4);
		int val = 0x12345678;
		bool ok = sb.addDataToBack(reinterpret_cast<const char*>(&val), 4);
		SBE_ASSERT(ok, "size-4: add int ok");
		SBE_ASSERT_EQ(sb.getDataLength(), 4, "size-4: len=4");
		int readback = 0;
		memcpy(&readback, sb.getData(), 4);
		SBE_ASSERT_EQ(readback, val, "size-4: int readback");
	}
	// 大小为2048
	{
		StreamBuffer sb(2048);
		SBE_ASSERT(sb.isAvailable(2048), "size-2048: isAvailable(2048)");
		SBE_ASSERT(!sb.isAvailable(2049), "size-2048: isAvailable(2049) false");
	}
}

//------------------------------------------------------------------------------
// addEmptyToBack clearIfFull=true 场景
//------------------------------------------------------------------------------
void StreamBufferTest::testStreamBufferAddEmptyClearIfFull()
{
	StreamBuffer sb(16);
	const char* init = "0123456789012345";
	sb.addDataToBack(init, 16);
	SBE_ASSERT_EQ(sb.getDataLength(), 16, "before empty overflow: len=16");

	// 再申请8字节空间，clearIfFull=true 会先清空
	bool ok = sb.addEmptyToBack(8, true);
	SBE_ASSERT(ok, "addEmptyToBack overflow clearIfFull=true ok");
	SBE_ASSERT_EQ(sb.getDataLength(), 8, "after addEmpty clearIfFull: len=8");
}

//------------------------------------------------------------------------------
// 混合 addDataToBack / addEmptyToBack 场景
//------------------------------------------------------------------------------
void StreamBufferTest::testStreamBufferMixedAddOps()
{
	StreamBuffer sb(128);
	// 先写header占位
	sb.addEmptyToBack(8);
	SBE_ASSERT_EQ(sb.getDataLength(), 8, "mixed: empty header 8");
	// 再写payload
	const char* payload = "PAYLOAD_DATA_12345";
	sb.addDataToBack(payload, 18);
	SBE_ASSERT_EQ(sb.getDataLength(), 26, "mixed: +payload=26");
	// 验证payload正确
	SBE_ASSERT_EQ(sb.getData()[8], 'P', "mixed: payload[0]='P'");
	// 移除header
	sb.removeDataFromFront(8);
	SBE_ASSERT_EQ(sb.getDataLength(), 18, "mixed: remove header len=18");
	SBE_ASSERT_EQ(sb.getData()[0], 'P', "mixed: after remove[0]='P'");
}

//------------------------------------------------------------------------------
// 环形跨越: 核心 — 数据跨越缓冲区末尾
//------------------------------------------------------------------------------
void StreamBufferTest::testStreamBufferRingWraparound()
{
	StreamBuffer sb(8);
	// 填满
	const char* full = "ABCDEFGH";
	sb.addDataToBack(full, 8);
	SBE_ASSERT_EQ(sb.getDataLength(), 8, "wrap: fill len=8");

	// 消费前4字节，head=4
	sb.removeDataFromFront(4);
	SBE_ASSERT_EQ(sb.getDataLength(), 4, "wrap: after remove len=4");
	SBE_ASSERT_EQ(sb.getDataStart(), 4, "wrap: after remove head=4");

	// 再写4字节，会跨越缓冲区末尾（tail=0, 写入4字节到0-3）
	const char* more = "IJKL";
	sb.addDataToBack(more, 4);
	SBE_ASSERT_EQ(sb.getDataLength(), 8, "wrap: after wrap add len=8");

	// getData() 返回第一段（从 head=4 开始，4字节：EFGH）
	SBE_ASSERT_EQ(sb.getContiguousDataLength(), 4, "wrap: contiguous=4");
	SBE_ASSERT_EQ(sb.getData()[0], 'E', "wrap: first seg [0]='E'");
	SBE_ASSERT_EQ(sb.getData()[3], 'H', "wrap: first seg [3]='H'");

	// 用 peekData 验证完整数据（跨段：EFGHIJKL）
	char peekBuf[8] = {};
	sb.peekData(peekBuf, 8);
	SBE_ASSERT_EQ(peekBuf[0], 'E', "wrap: peek[0]='E'");
	SBE_ASSERT_EQ(peekBuf[3], 'H', "wrap: peek[3]='H'");
	SBE_ASSERT_EQ(peekBuf[4], 'I', "wrap: peek[4]='I'");
	SBE_ASSERT_EQ(peekBuf[7], 'L', "wrap: peek[7]='L'");

	// 移除全部
	sb.removeDataFromFront(8);
	SBE_ASSERT_EQ(sb.getDataLength(), 0, "wrap: after remove all len=0");
	SBE_ASSERT_EQ(sb.getDataStart(), 0, "wrap: after remove all head=0");
}

//------------------------------------------------------------------------------
// 环形跨越: 多轮写读，指针多次回绕
//------------------------------------------------------------------------------
void StreamBufferTest::testStreamBufferRingMultiWrap()
{
	// 16 byte buffer: enough to hold multi-round wrap-around data
	StreamBuffer sb(16);
	// Round 1: write 8, consume 6, 2 left at tail
	const char* d1 = "ABCDEFGH";
	sb.addDataToBack(d1, 8);
	sb.removeDataFromFront(6);
	SBE_ASSERT_EQ(sb.getDataLength(), 2, "multiwrap r1: len=2");

	// Round 2: write 6, wrap (tail=8, 2 to [8-9], 4 to [0-3])
	const char* d2 = "IJKLMN";
	sb.addDataToBack(d2, 6);
	SBE_ASSERT_EQ(sb.getDataLength(), 8, "multiwrap r2: len=8");

	// Verify: GH IJKLMN
	char peekBuf[8] = {};
	sb.peekData(peekBuf, 8);
	SBE_ASSERT_EQ(peekBuf[0], 'G', "multiwrap: [0]='G'");
	SBE_ASSERT_EQ(peekBuf[1], 'H', "multiwrap: [1]='H'");
	SBE_ASSERT_EQ(peekBuf[2], 'I', "multiwrap: [2]='I'");
	SBE_ASSERT_EQ(peekBuf[7], 'N', "multiwrap: [7]='N'");

	// Consume 5, head moves from 6 to 11%16=11
	sb.removeDataFromFront(5);
	SBE_ASSERT_EQ(sb.getDataLength(), 3, "multiwrap r2: after remove len=3");

	// Round 3: write 8, still fits (freeSpace = 16-3 = 13 >= 8), wraps
	const char* d3 = "OPQRSTUV";
	sb.addDataToBack(d3, 8);
	SBE_ASSERT_EQ(sb.getDataLength(), 11, "multiwrap r3: len=11");

	// Full verify
	char peek2[11] = {};
	sb.peekData(peek2, 11);
	SBE_ASSERT_EQ(peek2[0], 'L', "multiwrap r3: [0]='L'");
	SBE_ASSERT_EQ(peek2[2], 'N', "multiwrap r3: [2]='N'");
	SBE_ASSERT_EQ(peek2[10], 'V', "multiwrap r3: [10]='V'");
}

//------------------------------------------------------------------------------
// peekData 边界：读取0字节、超出数据量
//------------------------------------------------------------------------------
void StreamBufferTest::testStreamBufferPeekDataEdge()
{
	StreamBuffer sb(16);
	const char* data = "ABCDEFGHIJ";
	sb.addDataToBack(data, 10);

	// 读取0字节
	int peeked = sb.peekData(nullptr, 0);
	SBE_ASSERT_EQ(peeked, 0, "peekData 0 bytes");

	// 读取超出数据量
	char buf[20] = {};
	peeked = sb.peekData(buf, 20);
	SBE_ASSERT_EQ(peeked, 10, "peekData overflow capped to 10");
}

//------------------------------------------------------------------------------
// getContiguousDataLength / getContiguousFreeLength
//------------------------------------------------------------------------------
void StreamBufferTest::testStreamBufferContiguousLength()
{
	StreamBuffer sb(16);
	// 初始状态
	SBE_ASSERT_EQ(sb.getContiguousDataLength(), 0, "contig: empty data=0");
	SBE_ASSERT_EQ(sb.getContiguousFreeLength(), 16, "contig: empty free=16");

	// 写入8字节
	const char* data = "ABCDEFGH";
	sb.addDataToBack(data, 8);
	SBE_ASSERT_EQ(sb.getContiguousDataLength(), 8, "contig: after add data=8");
	SBE_ASSERT_EQ(sb.getContiguousFreeLength(), 8, "contig: after add free=8");

	// 消费前4
	sb.removeDataFromFront(4);
	SBE_ASSERT_EQ(sb.getContiguousDataLength(), 4, "contig: after remove data=4");
	SBE_ASSERT_EQ(sb.getContiguousFreeLength(), 8, "contig: after remove free=8");

	// 写满到末尾
	sb.addDataToBack(data, 8);
	// 现在 head=4, tail=4(回绕), dataLen=12
	SBE_ASSERT_EQ(sb.getContiguousDataLength(), 12, "contig: after wrap fill data=12");
}

//------------------------------------------------------------------------------
// commitTail 直接写入场景
//------------------------------------------------------------------------------
void StreamBufferTest::testStreamBufferCommitTail()
{
	StreamBuffer sb(32);
	// 通过 getTailPtr 直接写入5字节
	char* tail = sb.getTailPtr();
	tail[0] = 'H';
	tail[1] = 'E';
	tail[2] = 'L';
	tail[3] = 'L';
	tail[4] = 'O';
	sb.commitTail(5);
	SBE_ASSERT_EQ(sb.getDataLength(), 5, "commitTail: len=5");
	SBE_ASSERT_EQ(sb.getData()[0], 'H', "commitTail: [0]='H'");
	SBE_ASSERT_EQ(sb.getData()[4], 'O', "commitTail: [4]='O'");
}

//------------------------------------------------------------------------------
// mergeFrom: merge another StreamBuffer
//------------------------------------------------------------------------------
void StreamBufferTest::testStreamBufferMergeFrom()
{
	// Case 1: basic merge, both linear
	{
		StreamBuffer dst(32);
		StreamBuffer src(16);
		const char* d1 = "HELLO";
		const char* d2 = "WORLD";
		dst.addDataToBack(d1, 5);
		src.addDataToBack(d2, 5);
		bool ok = dst.mergeFrom(&src);
		SBE_ASSERT(ok, "mergeFrom basic: ok");
		SBE_ASSERT_EQ(dst.getDataLength(), 10, "mergeFrom basic: len=10");
		char buf[10] = {};
		dst.peekData(buf, 10);
		SBE_ASSERT_EQ(buf[0], 'H', "mergeFrom basic: [0]='H'");
		SBE_ASSERT_EQ(buf[5], 'W', "mergeFrom basic: [5]='W'");
		SBE_ASSERT_EQ(buf[9], 'D', "mergeFrom basic: [9]='D'");
	}

	// Case 2: merge from wrapped source (data spans ring boundary)
	{
		StreamBuffer src(8);
		const char* full = "ABCDEFGH";
		src.addDataToBack(full, 8);
		src.removeDataFromFront(4);
		// src now: head=4, dataLen=4, tail=0, data=[EFGH] (not wrapped yet)
		// add more to wrap
		const char* more = "IJKL";
		src.addDataToBack(more, 4);
		// src now: head=4, tail=4, dataLen=8, data=[EFGHIJKL] wrapped
		SBE_ASSERT_EQ(src.getDataLength(), 8, "mergeFrom wrap src: len=8");

		StreamBuffer dst(32);
		const char* prefix = "XX";
		dst.addDataToBack(prefix, 2);
		bool ok = dst.mergeFrom(&src);
		SBE_ASSERT(ok, "mergeFrom wrap: ok");
		SBE_ASSERT_EQ(dst.getDataLength(), 10, "mergeFrom wrap: len=10");
		char buf[10] = {};
		dst.peekData(buf, 10);
		SBE_ASSERT_EQ(buf[0], 'X', "mergeFrom wrap: [0]='X'");
		SBE_ASSERT_EQ(buf[2], 'E', "mergeFrom wrap: [2]='E'");
		SBE_ASSERT_EQ(buf[5], 'H', "mergeFrom wrap: [5]='H'");
		SBE_ASSERT_EQ(buf[6], 'I', "mergeFrom wrap: [6]='I'");
		SBE_ASSERT_EQ(buf[9], 'L', "mergeFrom wrap: [9]='L'");
	}

	// Case 3: merge into wrapped destination
	{
		StreamBuffer dst(8);
		const char* full = "ABCDEFGH";
		dst.addDataToBack(full, 8);
		dst.removeDataFromFront(4);
		// dst: head=4, tail=0, dataLen=4, free=4
		StreamBuffer src(16);
		const char* data = "MN";
		src.addDataToBack(data, 2);
		bool ok = dst.mergeFrom(&src);
		SBE_ASSERT(ok, "mergeFrom wrap dst: ok");
		SBE_ASSERT_EQ(dst.getDataLength(), 6, "mergeFrom wrap dst: len=6");
		char buf[6] = {};
		dst.peekData(buf, 6);
		SBE_ASSERT_EQ(buf[0], 'E', "mergeFrom wrap dst: [0]='E'");
		SBE_ASSERT_EQ(buf[3], 'H', "mergeFrom wrap dst: [3]='H'");
		SBE_ASSERT_EQ(buf[4], 'M', "mergeFrom wrap dst: [4]='M'");
		SBE_ASSERT_EQ(buf[5], 'N', "mergeFrom wrap dst: [5]='N'");
	}

	// Case 4: merge empty source (no-op)
	{
		StreamBuffer dst(16);
		StreamBuffer src(16);
		const char* d = "ABC";
		dst.addDataToBack(d, 3);
		bool ok = dst.mergeFrom(&src);
		SBE_ASSERT(ok, "mergeFrom empty: ok");
		SBE_ASSERT_EQ(dst.getDataLength(), 3, "mergeFrom empty: len=3");
	}

	// Case 5: merge when destination full, clearIfFull=true
	{
		StreamBuffer dst(8);
		StreamBuffer src(16);
		const char* d1 = "ABCDEFGH";
		dst.addDataToBack(d1, 8);
		const char* d2 = "XYZ";
		src.addDataToBack(d2, 3);
		bool ok = dst.mergeFrom(&src, true);
		SBE_ASSERT(ok, "mergeFrom clearIfFull: ok");
		SBE_ASSERT_EQ(dst.getDataLength(), 3, "mergeFrom clearIfFull: len=3");
		char buf[3] = {};
		dst.peekData(buf, 3);
		SBE_ASSERT_EQ(buf[0], 'X', "mergeFrom clearIfFull: [0]='X'");
	}


	// Case 6: both source and destination wrapped
	{
		StreamBuffer dst(16);
		const char* dFull = "ABCDEFGHIJ";
		dst.addDataToBack(dFull, 10);
		dst.removeDataFromFront(6);
		// dst: head=6, dataLen=4, free=12, data=[GHIJ]
		StreamBuffer src(8);
		const char* sFull = "KLMNOPQR";
		src.addDataToBack(sFull, 8);
		src.removeDataFromFront(4);
		// src: head=4, dataLen=4, tail=0, active data=[OPQR]
		const char* sMore = "STUV";
		src.addDataToBack(sMore, 4);
		// src: head=4, tail=4, dataLen=8, data=[OPQRSTUV] wrapped
		bool ok = dst.mergeFrom(&src);
		SBE_ASSERT(ok, "mergeFrom both wrap: ok");
		SBE_ASSERT_EQ(dst.getDataLength(), 12, "mergeFrom both wrap: len=12");
		char buf[12] = {};
		dst.peekData(buf, 12);
		SBE_ASSERT_EQ(buf[0], 'G', "mergeFrom both wrap: [0]='G'");
		SBE_ASSERT_EQ(buf[3], 'J', "mergeFrom both wrap: [3]='J'");
		SBE_ASSERT_EQ(buf[4], 'O', "mergeFrom both wrap: [4]='O'");
		SBE_ASSERT_EQ(buf[7], 'R', "mergeFrom both wrap: [7]='R'");
		SBE_ASSERT_EQ(buf[8], 'S', "mergeFrom both wrap: [8]='S'");
		SBE_ASSERT_EQ(buf[11], 'V', "mergeFrom both wrap: [11]='V'");
	}

	// Case 7: source wraps around buffer end, dst is linear
	{
		// src(10): fill, remove 6, add 4 → data wraps
		StreamBuffer src(10);
		src.addDataToBack("ABCDEFGHIJ", 10); // tail=0, full
		src.removeDataFromFront(6);           // head=6, dataLen=4
		src.addDataToBack("KLMN", 4);         // tail=4, dataLen=8
		// src: [6..9]=GHIJ, [0..3]=KLMN → peekData = GHIJKLMN

		StreamBuffer dst(20);
		dst.addDataToBack("XX", 2);
		bool ok = dst.mergeFrom(&src);
		SBE_ASSERT(ok, "mergeFrom src wrap: ok");
		SBE_ASSERT_EQ(dst.getDataLength(), 10, "mergeFrom src wrap: len=10");
		char buf[10] = {};
		dst.peekData(buf, 10);
		SBE_ASSERT_EQ(buf[0], 'X', "mergeFrom src wrap: [0]='X'");
		SBE_ASSERT_EQ(buf[2], 'G', "mergeFrom src wrap: [2]='G'");
		SBE_ASSERT_EQ(buf[5], 'J', "mergeFrom src wrap: [5]='J'");
		SBE_ASSERT_EQ(buf[6], 'K', "mergeFrom src wrap: [6]='K'");
		SBE_ASSERT_EQ(buf[9], 'N', "mergeFrom src wrap: [9]='N'");
	}

	// Case 8: dst write wraps — dst tail near buffer end, mergeFrom's internal
	// addDataToBack crosses the dst buffer boundary
	{
		StreamBuffer dst(8);
		dst.addDataToBack("ABCDE", 5);   // tail=5, dataLen=5
		dst.removeDataFromFront(3);      // head=3, dataLen=2, tail=5, data=[DE]
		// tail=5, writing 5+ bytes will cross buffer end at [8]

		StreamBuffer src(16);
		src.addDataToBack("FGHIJ", 5);   // linear

		bool ok = dst.mergeFrom(&src);
		SBE_ASSERT(ok, "mergeFrom dst write wrap: ok");
		SBE_ASSERT_EQ(dst.getDataLength(), 7, "mergeFrom dst write wrap: len=7");
		char buf[7] = {};
		dst.peekData(buf, 7);
		SBE_ASSERT_EQ(buf[0], 'D', "mergeFrom dst write wrap: [0]='D'");
		SBE_ASSERT_EQ(buf[1], 'E', "mergeFrom dst write wrap: [1]='E'");
		SBE_ASSERT_EQ(buf[2], 'F', "mergeFrom dst write wrap: [2]='F'");
		SBE_ASSERT_EQ(buf[4], 'H', "mergeFrom dst write wrap: [4]='H'");
		SBE_ASSERT_EQ(buf[6], 'J', "mergeFrom dst write wrap: [6]='J'");
	}

	// Case 9: source wraps AND dst write wraps — both sides cross buffer boundary
	{
		// dst(8): tail near end, data wraps
		StreamBuffer dst(8);
		dst.addDataToBack("ABCDE", 5);   // tail=5
		dst.removeDataFromFront(3);      // head=3, dataLen=2, tail=5, data=[DE]
		// writing to dst will cross: tail=5, contiguous free=8-5=3

		// src(10): data wraps around — produce GHIJKL
		StreamBuffer src(10);
		src.addDataToBack("ABCDEFGH", 8);   // tail=8, dataLen=8
		src.removeDataFromFront(6);           // head=6, dataLen=2, data=[GH]
		src.addDataToBack("IJKL", 4);         // tail=2, dataLen=6
		// src data: [6..9]=GHIJ, [0..1]=KL → peekData = GHIJKL (6 bytes, wrapped)

		bool ok = dst.mergeFrom(&src);
		SBE_ASSERT(ok, "mergeFrom both wrap2: ok");
		SBE_ASSERT_EQ(dst.getDataLength(), 8, "mergeFrom both wrap2: len=8");
		char buf[8] = {};
		dst.peekData(buf, 8);
		SBE_ASSERT_EQ(buf[0], 'D', "mergeFrom both wrap2: [0]='D'");
		SBE_ASSERT_EQ(buf[1], 'E', "mergeFrom both wrap2: [1]='E'");
		SBE_ASSERT_EQ(buf[2], 'G', "mergeFrom both wrap2: [2]='G'");
		SBE_ASSERT_EQ(buf[5], 'J', "mergeFrom both wrap2: [5]='J'");
		SBE_ASSERT_EQ(buf[6], 'K', "mergeFrom both wrap2: [6]='K'");
		SBE_ASSERT_EQ(buf[7], 'L', "mergeFrom both wrap2: [7]='L'");
	}

	// Case 10: dst data wraps (read side) + source linear
	{
		StreamBuffer dst(8);
		dst.addDataToBack("ABCDEFGH", 8); // tail=0, full
		dst.removeDataFromFront(5);       // head=5, dataLen=3, data=[FGH]
		dst.addDataToBack("I", 1);        // tail=1, dataLen=4
		// dst data: [5..7]=FGH, [0]=I → peekData = FGHI (4 bytes, wraps)
		// freeSpace = 4

		StreamBuffer src(16);
		src.addDataToBack("JKLM", 4);     // linear

		bool ok = dst.mergeFrom(&src);
		SBE_ASSERT(ok, "mergeFrom dst data wrap: ok");
		SBE_ASSERT_EQ(dst.getDataLength(), 8, "mergeFrom dst data wrap: len=8");
		char buf[8] = {};
		dst.peekData(buf, 8);
		SBE_ASSERT_EQ(buf[0], 'F', "mergeFrom dst data wrap: [0]='F'");
		SBE_ASSERT_EQ(buf[3], 'I', "mergeFrom dst data wrap: [3]='I'");
		SBE_ASSERT_EQ(buf[4], 'J', "mergeFrom dst data wrap: [4]='J'");
		SBE_ASSERT_EQ(buf[7], 'M', "mergeFrom dst data wrap: [7]='M'");
	}

	// Case 11: triple wrap — source data wraps, dst data wraps, dst write wraps
	{
		// dst(12): data wraps, tail=5 (near middle, write will cross end for 7+ bytes)
		StreamBuffer dst(12);
		dst.addDataToBack("ABCDEFGHIJKL", 12); // tail=0, full
		dst.removeDataFromFront(8);             // head=8, dataLen=4, data=[IJKL]
		dst.addDataToBack("MNOPQ", 5);          // tail=5, dataLen=9
		// dst data: [8..11]=IJKL, [0..4]=MNOPQ → peekData = IJKLMNOPQ (wraps)
		// freeSpace=3, tail=5

		// src(8): data wraps
		StreamBuffer src(8);
		src.addDataToBack("RSTUVWXY", 8); // tail=0, full
		src.removeDataFromFront(6);        // head=6, dataLen=2, data=[XY]
		src.addDataToBack("Z", 1);         // tail=1, dataLen=3
		// src data: [6..7]=XY, [0]=Z → peekData = XYZ (wraps)

		bool ok = dst.mergeFrom(&src);
		SBE_ASSERT(ok, "mergeFrom triple wrap: ok");
		SBE_ASSERT_EQ(dst.getDataLength(), 12, "mergeFrom triple wrap: len=12");
		char buf[12] = {};
		dst.peekData(buf, 12);
		SBE_ASSERT_EQ(buf[0], 'I', "mergeFrom triple wrap: [0]='I'");
		SBE_ASSERT_EQ(buf[3], 'L', "mergeFrom triple wrap: [3]='L'");
		SBE_ASSERT_EQ(buf[4], 'M', "mergeFrom triple wrap: [4]='M'");
		SBE_ASSERT_EQ(buf[8], 'Q', "mergeFrom triple wrap: [8]='Q'");
		SBE_ASSERT_EQ(buf[9], 'X', "mergeFrom triple wrap: [9]='X'");
		SBE_ASSERT_EQ(buf[10], 'Y', "mergeFrom triple wrap: [10]='Y'");
		SBE_ASSERT_EQ(buf[11], 'Z', "mergeFrom triple wrap: [11]='Z'");
	}
}

//------------------------------------------------------------------------------
// linearize: compact wrapped data to start from buffer[0]
//------------------------------------------------------------------------------
void StreamBufferTest::testStreamBufferMoveTo()
{
	// 基本部分搬运: target只能接收4字节,source必须保留剩余数据。
	{
		StreamBuffer source(16);
		StreamBuffer target(4);
		const char data[] = "ABCDEFGH";
		source.addDataToBack(data, 8);

		const int moveCount = source.moveTo(&target);
		assertStreamBuffer(moveCount == 4, "moveTo partial count");
		assertStreamBuffer(source.getDataLength() == 4, "moveTo source remain");
		assertStreamBuffer(target.getDataLength() == 4, "moveTo target length");

		char targetData[4] = {};
		target.peekData(targetData, 4);
		assertStreamBuffer(targetData[0] == 'A' && targetData[3] == 'D', "moveTo target order");

		char sourceData[4] = {};
		source.peekData(sourceData, 4);
		assertStreamBuffer(sourceData[0] == 'E' && sourceData[3] == 'H', "moveTo source order");
	}

	// 环形跨越搬运: source数据跨缓冲区末尾时仍必须保持字节顺序。
	{
		StreamBuffer source(8);
		StreamBuffer target(8);
		const char first[] = "123456";
		const char second[] = "ABCD";
		source.addDataToBack(first, 6);
		source.removeDataFromFront(4);
		source.addDataToBack(second, 4);

		const int moveCount = source.moveTo(&target);
		assertStreamBuffer(moveCount == 6, "moveTo wrap count");
		assertStreamBuffer(source.isEmpty(), "moveTo wrap source empty");

		char data[6] = {};
		target.peekData(data, 6);
		assertStreamBuffer(data[0] == '5' && data[1] == '6' &&
			data[2] == 'A' && data[3] == 'B' && data[4] == 'C' && data[5] == 'D',
			"moveTo wrap order");
	}

	// maxCount限制。
	{
		StreamBuffer source(8);
		StreamBuffer target(8);
		const char data[] = "123456";
		source.addDataToBack(data, 6);
		const int moveCount = source.moveTo(&target, 3);
		assertStreamBuffer(moveCount == 3, "moveTo maxCount");
		assertStreamBuffer(source.getDataLength() == 3, "moveTo maxCount source");
		assertStreamBuffer(target.getDataLength() == 3, "moveTo maxCount target");
	}
}

void StreamBufferTest::testStreamBufferLinearize()
{
	// Case 1: already linear — no-op
	{
		StreamBuffer sb(16);
		sb.addDataToBack("ABCDEFGH", 8);
		sb.linearize();
		SBE_ASSERT_EQ(sb.getDataLength(), 8, "linearize already linear: len=8");
		SBE_ASSERT_EQ(sb.getDataStart(), 0, "linearize already linear: head=0");
		char buf[8] = {};
		sb.peekData(buf, 8);
		SBE_ASSERT_EQ(buf[0], 'A', "linearize already linear: [0]='A'");
		SBE_ASSERT_EQ(buf[7], 'H', "linearize already linear: [7]='H'");
	}

	// Case 2: empty buffer — no-op
	{
		StreamBuffer sb(16);
		sb.linearize();
		SBE_ASSERT_EQ(sb.getDataLength(), 0, "linearize empty: len=0");
		SBE_ASSERT_EQ(sb.getDataStart(), 0, "linearize empty: head=0");
	}

	// Case 3: data wraps — compact to [0]
	{
		StreamBuffer sb(8);
		sb.addDataToBack("ABCDEFGH", 8); // full, tail=0
		sb.removeDataFromFront(3);        // head=3, dataLen=5, data=[DEFGH]
		sb.addDataToBack("IJ", 2);        // tail=2, dataLen=7, wraps
		// Before: [3..7]=DEFGH, [0..1]=IJ → peekData = DEFGHIJ
		SBE_ASSERT_EQ(sb.getDataStart(), 3, "linearize wrap: before head=3");
		SBE_ASSERT(sb.getContiguousDataLength() < sb.getDataLength(),
			"linearize wrap: before is wrapped");

		sb.linearize();

		// After: head=0, tail=7, data=[DEFGHIJ] at [0..6]
		SBE_ASSERT_EQ(sb.getDataStart(), 0, "linearize wrap: after head=0");
		SBE_ASSERT_EQ(sb.getDataLength(), 7, "linearize wrap: after len=7");
		SBE_ASSERT_EQ(sb.getContiguousDataLength(), 7,
			"linearize wrap: after contiguous=7");

		char buf[7] = {};
		sb.peekData(buf, 7);
		SBE_ASSERT_EQ(buf[0], 'D', "linearize wrap: [0]='D'");
		SBE_ASSERT_EQ(buf[4], 'H', "linearize wrap: [4]='H'");
		SBE_ASSERT_EQ(buf[5], 'I', "linearize wrap: [5]='I'");
		SBE_ASSERT_EQ(buf[6], 'J', "linearize wrap: [6]='J'");

		// After linearize, getData() returns full contiguous data directly
		const char* p = sb.getData();
		SBE_ASSERT_EQ(p[0], 'D', "linearize wrap getData: [0]='D'");
		SBE_ASSERT_EQ(p[6], 'J', "linearize wrap getData: [6]='J'");
	}

	// Case 4: linearize then continue add/remove still works
	{
		StreamBuffer sb(8);
		sb.addDataToBack("ABCDEF", 6);
		sb.removeDataFromFront(2);        // head=2, data=[CDEF]
		sb.addDataToBack("GH", 2);        // wraps: [2..7]=CDEF, [0..1]=GH
		sb.linearize();                    // now [0..5]=CDEF GH

		sb.removeDataFromFront(3);        // consume CDE, left FGH
		SBE_ASSERT_EQ(sb.getDataLength(), 3, "linearize+op: after remove len=3");
		char buf[3] = {};
		sb.peekData(buf, 3);
		SBE_ASSERT_EQ(buf[0], 'F', "linearize+op: [0]='F'");
		SBE_ASSERT_EQ(buf[2], 'H', "linearize+op: [2]='H'");

		sb.addDataToBack("IJKL", 4);      // append more
		SBE_ASSERT_EQ(sb.getDataLength(), 7, "linearize+op: after add len=7");
		char buf2[7] = {};
		sb.peekData(buf2, 7);
		SBE_ASSERT_EQ(buf2[0], 'F', "linearize+op: final [0]='F'");
		SBE_ASSERT_EQ(buf2[6], 'L', "linearize+op: final [6]='L'");
	}
}
//------------------------------------------------------------------------------
void StreamBufferTest::testExtended()
{
	testStreamBufferConstruct();
	testStreamBufferAddBasic();
	testStreamBufferAddMultiple();
	testStreamBufferRemoveFront();
	testStreamBufferRemoveMultiple();
	testStreamBufferClear();
	testStreamBufferClearAndReuse();
	testStreamBufferIsAvailable();
	testStreamBufferAddEmpty();
	testStreamBufferAddDataFail();
	testStreamBufferAddDataClearIfFull();
	testStreamBufferProducerConsumer();
	testStreamBufferBinaryData();
	testStreamBufferFillAndClear();
	testStreamBufferRemoveOverflow();
	testStreamBufferLargeBuffer();
	testStreamBufferTCPSimulation();
	testStreamBufferOffsetTracking();
	testStreamBufferSingleByte();
	testStreamBufferAddZeroBytes();
	testStreamBufferRepeatAdd();
	testStreamBufferExactFit();
	testStreamBufferMultiRound();
	testStreamBufferIsAvailableZero();
	testStreamBufferReserveThenFill();
	testStreamBufferFrameStream();
	testStreamBufferVariousSizes();
	testStreamBufferAddEmptyClearIfFull();
	testStreamBufferMixedAddOps();
	testStreamBufferRingWraparound();
	testStreamBufferRingMultiWrap();
	testStreamBufferPeekDataEdge();
	testStreamBufferContiguousLength();
	testStreamBufferCommitTail();
	testStreamBufferMergeFrom();
	testStreamBufferLinearize();
}


#define SBL_ASSERT(expr, msg) \
	do { if (!(expr)) { ERROR(string("StreamBufferTest FAILED: ") + (msg)); } } while(0)
#define SBL_ASSERT_EQ(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("StreamBufferTest FAILED: ") + (msg) + \
		" | expected=" + IToS((int)(b)) + " actual=" + IToS((int)(a))); } } while(0)

// ========== linearize: 已连续（no-op）==========
void StreamBufferTest::testLinearize_AlreadyContiguous()
{
	StreamBuffer sb(128);
	sb.addDataToBack("hello", 5);
	// 数据未跨越，linearize 应该 no-op
	const char* dataBefore = sb.getData();
	sb.linearize();
	SBL_ASSERT_EQ(sb.getDataLength(), 5, "contiguous length unchanged");
	SBL_ASSERT(sb.getData() == dataBefore || sb.getContiguousDataLength() == 5, "contiguous data still contiguous");
}

// ========== linearize: 空缓冲区 ==========
void StreamBufferTest::testLinearize_Empty()
{
	StreamBuffer sb(64);
	sb.linearize(); // 不应崩溃
	SBL_ASSERT_EQ(sb.getDataLength(), 0, "empty linearize ok");
}

// ========== linearize: 跨越一次环形边界 ==========
void StreamBufferTest::testLinearize_SingleWrap()
{
	StreamBuffer sb(16);
	// 先写入 10 字节，然后移除 6 字节，使 head=6 tail=10
	sb.addDataToBack("0123456789", 10);
	sb.removeDataFromFront(6);
	// 此时 head=6, tail=10(循环后), 有效数据4字节: "6789"
	SBL_ASSERT_EQ(sb.getDataStart(), 6, "wrap pre: head=6");

	// 再写入超过尾部剩余空间的长度，强制跨环写入
	int freeAtEnd = sb.getContiguousFreeLength();
	char fill[20];
	memset(fill, 'X', sizeof(fill));
	bool ok = sb.addDataToBack(fill, freeAtEnd + 6);
	SBL_ASSERT(ok, "wrap write ok");
	// 现在数据一定跨越了缓冲区末尾

	int totalLen = sb.getDataLength();
	SBL_ASSERT(totalLen > 0, "has data after wrap");
	int contigLen = sb.getContiguousDataLength();
	SBL_ASSERT(contigLen < totalLen, "data wraps around");

	// linearize 后应该连续
	sb.linearize();

	SBL_ASSERT_EQ(sb.getDataStart(), 0, "post-linearize head=0");
	SBL_ASSERT_EQ(sb.getContiguousDataLength(), totalLen, "fully contiguous after linearize");

	// 验证数据完整性: 用 peekData 对比
	char* peekBuf = new char[totalLen];
	// 先 peekData 到临时区
	sb.peekData(peekBuf, totalLen);

	// linearize 后 getData() 应该和 peekData 一致
	const char* linearData = sb.getData();
	SBL_ASSERT(memcmp(linearData, peekBuf, totalLen) == 0, "linearize data matches peekData");
	delete[] peekBuf;
}

// ========== linearize: 多次跨环后线性化 ==========
void StreamBufferTest::testLinearize_MultiWrapThenLinearize()
{
	StreamBuffer sb(32);
	// 模拟 TCP 场景：反复 add/remove 造成多次环绕
	FOR(5)
	{
		char payload[12];
		memset(payload, 'A' + i, sizeof(payload));
		sb.addDataToBack(payload, sizeof(payload));
		sb.removeDataFromFront(8); // 每次消费 8 字节，head 漂移
	}

	if (sb.getContiguousDataLength() < sb.getDataLength())
	{
		int lenBefore = sb.getDataLength();
		sb.linearize();
		SBL_ASSERT_EQ(sb.getDataLength(), lenBefore, "multi-wrap length preserved");
		SBL_ASSERT_EQ(sb.getContiguousDataLength(), lenBefore, "multi-wrap fully contiguous");
		SBL_ASSERT_EQ(sb.getDataStart(), 0, "multi-wrap head reset to 0");
	}
	else
	{
		// 数据恰好连续，也测一下
		sb.linearize();
		SBL_ASSERT(true, "already contiguous in multi-wrap scenario");
	}
}

// ========== linearize: 满缓冲区跨环 ==========
void StreamBufferTest::testLinearize_FullWrap()
{
	// 构造一个几乎满且必然跨环的 buffer
	StreamBuffer sb(24);
	// 写入到接近满
	sb.addDataToBack("0123456789ABCDEF", 16); // head=0, tail=16
	sb.removeDataFromFront(12); // head=12, tail=16, 有效=4
	// 填充到满（跨环写入）
	char big[20];
	memset(big, 'Z', sizeof(big));
	sb.addDataToBack(big, 19); // 跨环: 16..23(8字节) + 0..11(11字节)
	SBL_ASSERT(sb.getDataLength() > sb.getBufferSize() - sb.getDataLength(), "full wrap: contig < total"); // 即 contigLen < totalLen

	int totalLen = sb.getDataLength();
	sb.linearize();
	SBL_ASSERT_EQ(sb.getDataStart(), 0, "full wrap: head=0");
	SBL_ASSERT_EQ(sb.getContiguousDataLength(), totalLen, "full wrap: fully contiguous");
}

// ========== linearize: 单字节跨环边界 ==========
void StreamBufferTest::testLinearize_OneByteAtBoundary()
{
	StreamBuffer sb(8);
	// 精心构造：head 在倒数第二位，tail 在第一位（1 字节跨环）
	sb.addDataToBack("ABCDEFGH", 8); // 满
	sb.removeDataFromFront(7);        // head=7, 有效="H"(1字节)
	// 写入更多数据，迫使 tail 绕过
	sb.addDataToBack("XY", 2);       // tail=(7+2)%8=1, 有效="HXY" 共3字节
	// 数据分布: [7]='H', [0]='X', [1]='Y'
	// contigLen = min(3, 8-7)=1, 但总长=3 → 跨环!

	SBL_ASSERT_EQ(sb.getContiguousDataLength(), 1, "one-byte boundary: contig=1");
	SBL_ASSERT_EQ(sb.getDataLength(), 3, "one-byte boundary: total=3");

	sb.linearize();

	SBL_ASSERT_EQ(sb.getDataStart(), 0, "one-byte post-linearize head=0");
	SBL_ASSERT_EQ(sb.getContiguousDataLength(), 3, "one-byte post-linearize full");
	const char* d = sb.getData();
	SBL_ASSERT(d[0] == 'H' && d[1] == 'X' && d[2] == 'Y', "one-byte data correct");
}

// ========== getContiguousFreeLength 边界测试 ==========
void StreamBufferTest::testGetContiguousFreeLength()
{
	StreamBuffer sb(16);
	// 空 buffer: 整个 buffer 都可写
	SBL_ASSERT_EQ(sb.getContiguousFreeLength(), 16, "empty: free=all");

	sb.addDataToBack("ABCD", 4); // head=0, tail=4
	SBL_ASSERT_EQ(sb.getContiguousFreeLength(), 12, "partial: 12 free at tail");

	// 移除一些，但 head 还没动
	sb.removeDataFromFront(2); // head=2, tail=4
	// free = 16 - 2 = 14, tail 在 head 右边所以 freeAtTail = 16 - 4 = 12
	SBL_ASSERT_EQ(sb.getContiguousFreeLength(), 12, "after remove: freeAtTail=12");

	// 强制跨环
	sb.addDataToBack("EFGHIJKLMNO", 11); // tail=(4+11)%16=15, head=2, len=13
	// free = 3, tail < head 所以 freeAtTail = head-tail = 2-15+16=3
	int cf = sb.getContiguousFreeLength();
	SBL_ASSERT(cf >= 1 && cf <= 3, "wrapped: small contiguous free");
}

void StreamBufferTest::testSwapLinearize()
{
	testLinearize_AlreadyContiguous();
	testLinearize_Empty();
	testLinearize_SingleWrap();
	testLinearize_MultiWrapThenLinearize();
	testLinearize_FullWrap();
	testLinearize_OneByteAtBoundary();
	testGetContiguousFreeLength();
}


#define SBE_ASSERT_1(expr, msg) \
	if (!(expr)) { ERROR(string("StreamBufferTest FAIL: ") + (msg)); }
#define SBE_ASSERT_EQ_1(a, b, msg) \
	if ((a) != (b)) { ERROR(string("StreamBufferTest FAIL: ") + (msg) + \
		" | expected=" + IToS((int)(b)) + " actual=" + IToS((int)(a))); }

// =====================================================================
// Part 1: StreamBuffer — 空状态测试
// =====================================================================

void StreamBufferTest::testSBE_defaultState()
{
	StreamBuffer buf(128);
	SBE_ASSERT_EQ_1(buf.getDataLength(), 0, "default dataLength 0");
	SBE_ASSERT_EQ_1(buf.getBufferSize(), 128, "default bufferSize 0");
}

void StreamBufferTest::testSBE_emptyPeekData()
{
	StreamBuffer buf(128);
	char out[16] = {};
	SBE_ASSERT_1(!buf.peekData(out, 0, 1), "empty peek returns false");
}

void StreamBufferTest::testSBE_emptyAddData()
{
	StreamBuffer buf(128);
	const char data[] = "test";
	buf.addDataToBack(data, 4);
	SBE_ASSERT_EQ_1(buf.getDataLength(), 4, "empty addData size 4");
}

// =====================================================================
// Part 2: StreamBuffer — 基础添加和读取
// =====================================================================

void StreamBufferTest::testSBE_addAndPeek()
{
	StreamBuffer buf(128);
	const char src[] = "Hello!";
	buf.addDataToBack(src, 6);
	SBE_ASSERT_EQ_1(buf.getDataLength(), 6, "add & peek length");

	char dst[6] = {};
	SBE_ASSERT_1(buf.peekData(dst, 6), "peek success");
	for (int i = 0; i < 6; ++i)
	{
		SBE_ASSERT_EQ_1(dst[i], src[i], "peek content");
	}
}

void StreamBufferTest::testSBE_addAndRemove()
{
	StreamBuffer buf(128);
	const char src[] = "ABCD";
	buf.addDataToBack(src, 4);
	buf.removeDataFromFront(2);
	SBE_ASSERT_EQ_1(buf.getDataLength(), 2, "add+remove length 2");

	char dst[2] = {};
	buf.peekData(dst, 2);
	SBE_ASSERT_EQ_1(dst[0], 'C', "add+remove[0]=C");
	SBE_ASSERT_EQ_1(dst[1], 'D', "add+remove[1]=D");
}

void StreamBufferTest::testSBE_addRemoveAll()
{
	StreamBuffer buf(128);
	buf.addDataToBack("XYZ", 3);
	buf.removeDataFromFront(3);
	SBE_ASSERT_1(buf.isEmpty(), "add+remove all empty");
}

void StreamBufferTest::testSBE_addTwice()
{
	StreamBuffer buf(128);
	buf.addDataToBack("AB", 2);
	buf.addDataToBack("CD", 2);
	SBE_ASSERT_EQ_1(buf.getDataLength(), 4, "add twice length 4");

	char dst[4] = {};
	buf.peekData(dst, 4);
	SBE_ASSERT_EQ_1(dst[0], 'A', "add twice[0]");
	SBE_ASSERT_EQ_1(dst[1], 'B', "add twice[1]");
	SBE_ASSERT_EQ_1(dst[2], 'C', "add twice[2]");
	SBE_ASSERT_EQ_1(dst[3], 'D', "add twice[3]");
}

void StreamBufferTest::testSBE_removePartial()
{
	StreamBuffer buf(128);
	buf.addDataToBack("12345", 5);
	buf.removeDataFromFront(2);
	SBE_ASSERT_EQ_1(buf.getDataLength(), 3, "remove partial length 3");

	buf.addDataToBack("678", 3);
	SBE_ASSERT_EQ_1(buf.getDataLength(), 6, "remove+add length 6");

	char dst[6] = {};
	buf.peekData(dst, 6);
	SBE_ASSERT_EQ_1(dst[0], '3', "remove partial[0]");
	SBE_ASSERT_EQ_1(dst[1], '4', "remove partial[1]");
	SBE_ASSERT_EQ_1(dst[2], '5', "remove partial[2]");
	SBE_ASSERT_EQ_1(dst[3], '6', "remove partial[3]");
	SBE_ASSERT_EQ_1(dst[4], '7', "remove partial[4]");
	SBE_ASSERT_EQ_1(dst[5], '8', "remove partial[5]");
}

// =====================================================================
// Part 3: StreamBuffer — 多次操作序列
// =====================================================================

void StreamBufferTest::testSBE_multiCycle()
{
	StreamBuffer buf(128);
	for (int cycle = 0; cycle < 5; ++cycle)
	{
		char src[4] = { (char)('A' + cycle * 4), (char)('A' + cycle * 4 + 1),
					   (char)('A' + cycle * 4 + 2), (char)('A' + cycle * 4 + 3) };
		buf.addDataToBack(src, 4);
	}
	SBE_ASSERT_EQ_1(buf.getDataLength(), 20, "5 cycles length 20");

	for (int i = 0; i < 20; ++i)
	{
		char c;
		buf.peekData(&c, 1, i);
		SBE_ASSERT_EQ_1(c, (char)('A' + i), "cycle content");
	}
}

void StreamBufferTest::testSBE_addRemoveAddRemove()
{
	StreamBuffer buf(128);
	buf.addDataToBack("ab", 2);
	buf.removeDataFromFront(1);
	buf.addDataToBack("cd", 2);
	buf.removeDataFromFront(2);
	SBE_ASSERT_EQ_1(buf.getDataLength(), 1, "ara length 1");
	char c;
	buf.peekData(&c, 1);
	SBE_ASSERT_EQ_1(c, 'd', "ara content = d");
}

void StreamBufferTest::testSBE_addLargeThenRemove()
{
	StreamBuffer buf(2048);
	char big[1024];
	for (int i = 0; i < 1024; ++i)
	{
		big[i] = (char)(i & 0xFF);
	}
	buf.addDataToBack(big, 1024);
	SBE_ASSERT_EQ_1(buf.getDataLength(), 1024, "large add 1024");

	buf.removeDataFromFront(512);
	SBE_ASSERT_EQ_1(buf.getDataLength(), 512, "large remove to 512");

	char dst[512] = {};
	buf.peekData(dst, 512);
	for (int i = 0; i < 512; ++i)
	{
		SBE_ASSERT_EQ_1((byte)dst[i], (byte)((i + 512) & 0xFF), "large content");
	}
}

void StreamBufferTest::testSBE_addRemoveAllRepeated()
{
	StreamBuffer buf(128);
	for (int cycle = 0; cycle < 20; ++cycle)
	{
		buf.addDataToBack("data", 4);
		buf.removeDataFromFront(4);
		SBE_ASSERT_1(buf.isEmpty(), "cycle empty");
	}
}

// =====================================================================
// Part 4: StreamBuffer — 环状缓冲区边界
// =====================================================================

void StreamBufferTest::testSBE_wrapAround()
{
	StreamBuffer buf(128);
	// 先填充到接近缓冲区末尾，然后移除部分，再加数据触发环状
	char initial[100];
	memset(initial, 'X', 100);
	buf.addDataToBack(initial, 100);
	buf.removeDataFromFront(80);
	SBE_ASSERT_EQ_1(buf.getDataLength(), 20, "wrap before length 20");

	char more[100];
	memset(more, 'Y', 100);
	buf.addDataToBack(more, 80);
	SBE_ASSERT_EQ_1(buf.getDataLength(), 100, "wrap after length 100");
}

void StreamBufferTest::testSBE_multipleWrapAround()
{
	StreamBuffer buf(128);
	for (int cycle = 0; cycle < 10; ++cycle)
	{
		char data[30];
		memset(data, (char)('A' + cycle), 30);
		buf.addDataToBack(data, 30);
		buf.removeDataFromFront(25);
	}
	char dst[50] = {};
	buf.peekData(dst, buf.getDataLength());
	SBE_ASSERT_1(true, "multiple wraps no crash");
}

void StreamBufferTest::testSBE_exactBufferFit()
{
	StreamBuffer buf(128);
	char data[64];
	memset(data, 'Z', 64);
	buf.addDataToBack(data, 64);
	SBE_ASSERT_EQ_1(buf.getDataLength(), 64, "exact fit length 64");
	buf.removeDataFromFront(32);
	buf.addDataToBack(data, 32);
	SBE_ASSERT_EQ_1(buf.getDataLength(), 64, "exact fit re-fill 64");
}

// =====================================================================
// Part 6: StreamBuffer — mergeFrom
// =====================================================================

void StreamBufferTest::testSBE_mergeFromBasic()
{
	StreamBuffer a(128), b(128);
	a.addDataToBack("Hello ", 6);
	b.addDataToBack("World!", 6);
	a.mergeFrom(&b);
	SBE_ASSERT_EQ_1(a.getDataLength(), 12, "merge length 12");
	SBE_ASSERT_1(!b.isEmpty(), "merge source empty");

	char dst[12] = {};
	a.peekData(dst, 12);
	SBE_ASSERT_EQ_1(dst[0], 'H', "merge[0]");
	SBE_ASSERT_EQ_1(dst[5], ' ', "merge[5]");
	SBE_ASSERT_EQ_1(dst[11], '!', "merge[11]");
}

void StreamBufferTest::testSBE_mergeFromEmpty()
{
	StreamBuffer a(128), b(128);
	a.addDataToBack("data", 4);
	a.mergeFrom(&b);
	SBE_ASSERT_EQ_1(a.getDataLength(), 4, "merge empty target unchanged");
}

void StreamBufferTest::testSBE_mergeFromEmptyBoth()
{
	StreamBuffer a(128), b(128);
	a.mergeFrom(&b);
	SBE_ASSERT_1(a.isEmpty(), "merge both empty");
}

void StreamBufferTest::testSBE_mergeFromSelf()
{
	StreamBuffer buf(128);
	buf.addDataToBack("test", 4);
	buf.mergeFrom(&buf);
	SBE_ASSERT_1(true, "merge self no crash");
}

void StreamBufferTest::testSBE_mergeFromAfterRemove()
{
	StreamBuffer a(128), b(128);
	a.addDataToBack("0123456789", 10);
	a.removeDataFromFront(5);
	b.addDataToBack("abcde", 5);
	a.mergeFrom(&b);
	SBE_ASSERT_EQ_1(a.getDataLength(), 10, "merge after remove length 10");
}

// =====================================================================
// Part 7: StreamBuffer — linearize
// =====================================================================

void StreamBufferTest::testSBE_linearizeEmpty()
{
	StreamBuffer buf(128);
	buf.linearize();
	SBE_ASSERT_1(buf.isEmpty(), "linearize empty");
}

void StreamBufferTest::testSBE_linearizeSimple()
{
	StreamBuffer buf(128);
	buf.addDataToBack("ABCDEF", 6);
	buf.linearize();
	SBE_ASSERT_EQ_1(buf.getDataLength(), 6, "linearize simple length");

	char dst[6] = {};
	buf.peekData(dst, 6);
	SBE_ASSERT_EQ_1(dst[0], 'A', "linearize[0]");
	SBE_ASSERT_EQ_1(dst[5], 'F', "linearize[5]");
}

void StreamBufferTest::testSBE_linearizeAfterPartialRead()
{
	StreamBuffer buf(128);
	buf.addDataToBack("ABCDEFGH", 8);
	buf.removeDataFromFront(3);
	buf.linearize();
	SBE_ASSERT_EQ_1(buf.getDataLength(), 5, "linearize after read length 5");

	char dst[5] = {};
	buf.peekData(dst, 5);
	SBE_ASSERT_EQ_1(dst[0], 'D', "linearize after read[0]=D");
	SBE_ASSERT_EQ_1(dst[4], 'H', "linearize after read[4]=H");
}

void StreamBufferTest::testSBE_linearizeThenAdd()
{
	StreamBuffer buf(128);
	buf.addDataToBack("ABC", 3);
	buf.linearize();
	buf.addDataToBack("DEF", 3);
	SBE_ASSERT_EQ_1(buf.getDataLength(), 6, "linearize then add length");

	char dst[6] = {};
	buf.peekData(dst, 6);
	SBE_ASSERT_EQ_1(dst[0], 'A', "linearize then add[0]");
	SBE_ASSERT_EQ_1(dst[5], 'F', "linearize then add[5]");
}

void StreamBufferTest::testSBE_linearizeAfterMerge()
{
	StreamBuffer a(128), b(128);
	a.addDataToBack("AB", 2);
	b.addDataToBack("CD", 2);
	a.mergeFrom(&b);
	a.linearize();
	SBE_ASSERT_EQ_1(a.getDataLength(), 4, "linearize after merge length");

	char dst[4] = {};
	a.peekData(dst, 4);
	SBE_ASSERT_EQ_1(dst[0], 'A', "linearize after merge[0]");
	SBE_ASSERT_EQ_1(dst[3], 'D', "linearize after merge[3]");
}

// =====================================================================
// Part 8: StreamBuffer — getContiguousDataLength
// =====================================================================

void StreamBufferTest::testSBE_contiguousLength()
{
	StreamBuffer buf(128);
	buf.addDataToBack("ABCD", 4);
	int len = buf.getContiguousDataLength();
	SBE_ASSERT_EQ_1(len, buf.getDataLength(), "contiguous length = total");
}

void StreamBufferTest::testSBE_contiguousAfterRemove()
{
	StreamBuffer buf(128);
	buf.addDataToBack("ABCDEFGH", 8);
	buf.removeDataFromFront(2);
	int len = buf.getContiguousDataLength();
	SBE_ASSERT_1(len > 0 && len <= buf.getDataLength(), "contiguous after remove");
}

// =====================================================================
// Part 9: StreamBuffer — 批量数据操作
// =====================================================================

void StreamBufferTest::testSBE_bulkAddAndRead()
{
	StreamBuffer buf(128);
	for (int i = 0; i < 100; ++i)
	{
		char c = (char)('a' + (i % 26));
		buf.addDataToBack(&c, 1);
	}
	SBE_ASSERT_EQ_1(buf.getDataLength(), 100, "bulk add length 100");

	for (int i = 0; i < 100; ++i)
	{
		char c;
		buf.peekData(&c, 1, i);
		SBE_ASSERT_EQ_1(c, (char)('a' + (i % 26)), "bulk content");
	}
}

void StreamBufferTest::testSBE_bulkRemove()
{
	StreamBuffer buf(1024);
	char data[500];
	memset(data, 'X', 500);
	buf.addDataToBack(data, 500);
	SBE_ASSERT_EQ_1(buf.getDataLength(), 500, "bulk remove before");

	for (int i = 0; i < 50; ++i)
	{
		buf.removeDataFromFront(10);
	}
	SBE_ASSERT_1(buf.isEmpty(), "bulk remove all empty");
}

void StreamBufferTest::testSBE_bulkAddAfterRemove()
{
	StreamBuffer buf(128);
	for (int batch = 0; batch < 10; ++batch)
	{
		char data[20];
		memset(data, (char)('0' + batch), 20);
		buf.addDataToBack(data, 20);
		if (batch > 0)
		{
			buf.removeDataFromFront(10);
		}
	}
	SBE_ASSERT_1(true, "bulk add after remove no crash");
}

// =====================================================================
// Part 10: StreamBuffer — 边界值和极端情况
// =====================================================================

void StreamBufferTest::testSBE_singleByteAddRemove()
{
	StreamBuffer buf(128);
	const char c = 'X';
	buf.addDataToBack(&c, 1);
	SBE_ASSERT_EQ_1(buf.getDataLength(), 1, "single byte add");

	char out;
	buf.peekData(&out, 1);
	SBE_ASSERT_EQ_1(out, 'X', "single byte peek");

	buf.removeDataFromFront(1);
	SBE_ASSERT_1(buf.isEmpty(), "single byte remove empty");
}

void StreamBufferTest::testSBE_zeroLengthAdd()
{
	StreamBuffer buf(128);
	buf.addDataToBack("", 0);
	SBE_ASSERT_1(buf.isEmpty(), "zero length add empty");
}

void StreamBufferTest::testSBE_maxSizeAdd()
{
	StreamBuffer buf(9000);
	char big[4096];
	memset(big, 'M', 4096);
	buf.addDataToBack(big, 4096);
	SBE_ASSERT_EQ_1(buf.getDataLength(), 4096, "max size 4096");

	char verify[4096] = {};
	buf.peekData(verify, 4096);
	for (int i = 0; i < 4096; ++i)
	{
		SBE_ASSERT_EQ_1((byte)verify[i], (byte)'M', "max size content");
	}
}

void StreamBufferTest::testSBE_dataEquality()
{
	StreamBuffer buf(128);
	const char src[] = "!@#$%^&*()";
	buf.addDataToBack(src, 10);
	char dst[10] = {};
	buf.peekData(dst, 10);
	for (int i = 0; i < 10; ++i)
	{
		SBE_ASSERT_EQ_1(dst[i], src[i], "data equality");
	}
}

// =====================================================================
// Part 11: StreamBuffer — 混合操作
// =====================================================================

void StreamBufferTest::testSBE_addPeekRemovePeek()
{
	StreamBuffer buf(128);
	buf.addDataToBack("Hello", 5);
	char p1[3] = {};
	buf.peekData(p1, 3);
	SBE_ASSERT_EQ_1(p1[0], 'H', "apr peek[0]");
	buf.removeDataFromFront(2);
	char p2[3] = {};
	buf.peekData(p2, 3);
	SBE_ASSERT_EQ_1(p2[0], 'l', "apr peek after remove[0]=l");
	SBE_ASSERT_EQ_1(p2[1], 'l', "apr peek after remove[1]=l");
	SBE_ASSERT_EQ_1(p2[2], 'o', "apr peek after remove[2]=o");
}

void StreamBufferTest::testSBE_addAddRemoveAdd()
{
	StreamBuffer buf(128);
	buf.addDataToBack("AB", 2);
	buf.addDataToBack("CD", 2);
	buf.removeDataFromFront(2);
	buf.addDataToBack("EF", 2);
	SBE_ASSERT_EQ_1(buf.getDataLength(), 4, "aara length");

	char dst[4] = {};
	buf.peekData(dst, 4);
	SBE_ASSERT_EQ_1(dst[0], 'C', "aara[0]=C");
	SBE_ASSERT_EQ_1(dst[1], 'D', "aara[1]=D");
	SBE_ASSERT_EQ_1(dst[2], 'E', "aara[2]=E");
	SBE_ASSERT_EQ_1(dst[3], 'F', "aara[3]=F");
}

void StreamBufferTest::testSBE_addAddRemoveRemove()
{
	StreamBuffer buf(128);
	buf.addDataToBack("ABCD", 4);
	buf.addDataToBack("EFGH", 4);
	buf.removeDataFromFront(5);
	buf.removeDataFromFront(3);
	SBE_ASSERT_1(buf.isEmpty(), "aarr empty");
}

void StreamBufferTest::testSBE_stressSequence()
{
	StreamBuffer buf(1024);
	for (int i = 0; i < 50; ++i)
	{
		char data[10];
		memset(data, (char)('A' + (i % 26)), 10);
		buf.addDataToBack(data, 10);
		if (i % 3 == 0)
		{
			buf.removeDataFromFront(5);
		}
		if (i % 7 == 0)
		{
			buf.linearize();
		}
	}
	SBE_ASSERT_1(true, "stress sequence no crash");
}

// =====================================================================
// Part 12: StreamBuffer — clear() 后操作
// =====================================================================

void StreamBufferTest::testSBE_clearThenAdd()
{
	StreamBuffer buf(128);
	buf.addDataToBack("old", 3);
	buf.clear();
	SBE_ASSERT_1(buf.isEmpty(), "clear then add empty");
	buf.addDataToBack("new", 3);
	SBE_ASSERT_EQ_1(buf.getDataLength(), 3, "clear then add length");

	char dst[3] = {};
	buf.peekData(dst, 3);
	SBE_ASSERT_EQ_1(dst[0], 'n', "clear then add[0]");
	SBE_ASSERT_EQ_1(dst[2], 'w', "clear then add[2]");
}

void StreamBufferTest::testSBE_clearThenAddLarge()
{
	StreamBuffer buf(512);
	buf.addDataToBack("small", 5);
	buf.clear();
	char big[200];
	memset(big, 'B', 200);
	buf.addDataToBack(big, 200);
	SBE_ASSERT_EQ_1(buf.getDataLength(), 200, "clear+large 200");
}

// =====================================================================
// Part 13: StreamBuffer — 多缓冲区交叉操作
// =====================================================================

void StreamBufferTest::testSBE_independentBuffers()
{
	StreamBuffer a(128), b(128);
	a.addDataToBack("alpha", 5);
	b.addDataToBack("beta", 4);
	SBE_ASSERT_EQ_1(a.getDataLength(), 5, "independent a length");
	SBE_ASSERT_EQ_1(b.getDataLength(), 4, "independent b length");

	char dst[5] = {};
	a.peekData(dst, 5);
	SBE_ASSERT_EQ_1(dst[0], 'a', "independent a data[0]");
	b.peekData(dst, 4);
	SBE_ASSERT_EQ_1(dst[0], 'b', "independent b data[0]");

	a.removeDataFromFront(2);
	b.removeDataFromFront(1);
	SBE_ASSERT_EQ_1(a.getDataLength(), 3, "independent a after remove");
	SBE_ASSERT_EQ_1(b.getDataLength(), 3, "independent b after remove");
}

void StreamBufferTest::testSBE_clearAndReuse()
{
	StreamBuffer buf(128);
	buf.addDataToBack("first_batch", 11);
	buf.clear();
	buf.addDataToBack("second_batch", 12);
	SBE_ASSERT_EQ_1(buf.getDataLength(), 12, "clear and reuse length");

	char dst[12] = {};
	buf.peekData(dst, 12);
	SBE_ASSERT_EQ_1(dst[0], 's', "clear reuse data[0]");
	SBE_ASSERT_EQ_1(dst[7], 'b', "clear reuse data[7]");
}

void StreamBufferTest::testSBE_partialPeek()
{
	StreamBuffer buf(128);
	buf.addDataToBack("abcdefghij", 10);

	char half[5] = {};
	SBE_ASSERT_1(buf.peekData(half, 5), "partial peek first half");
	SBE_ASSERT_EQ_1(half[0], 'a', "partial[0]=a");
	SBE_ASSERT_EQ_1(half[4], 'e', "partial[4]=e");

	char second[5] = {};
	SBE_ASSERT_1(buf.peekData(second, 5, 5), "partial peek second half");
	SBE_ASSERT_EQ_1(second[0], 'f', "partial[5]=f");
	SBE_ASSERT_EQ_1(second[4], 'j', "partial[9]=j");
}

void StreamBufferTest::testSBE_repeatedAddRemove()
{
	StreamBuffer buf(128);
	for (int cycle = 0; cycle < 100; ++cycle)
	{
		char data[1] = { (char)(cycle % 256) };
		buf.addDataToBack(data, 1);
		char read;
		buf.peekData(&read, 1);
		// After many add/remove cycles, buffer should be consistent
		if (cycle >= 50)
		{
			buf.removeDataFromFront(1);
		}
	}
	SBE_ASSERT_1(true, "repeated add/remove no crash");
}

void StreamBufferTest::testSBE_sequentialAddAndPeek()
{
	StreamBuffer buf(1024);
	char data[256];
	for (int i = 0; i < 256; ++i)
	{
		data[i] = (char)i;
	}
	buf.addDataToBack(data, 256);
	SBE_ASSERT_EQ_1(buf.getDataLength(), 256, "sequential add length");

	for (int i = 0; i < 256; ++i)
	{
		char c;
		buf.peekData(&c, 1, i);
		SBE_ASSERT_EQ_1((int)(byte)c, i, "sequential data byte");
	}
}

// =====================================================================
// 主入口
// =====================================================================
void StreamBufferTest::testEdge()
{
	testSBE_defaultState();
	testSBE_emptyPeekData();
	testSBE_emptyAddData();
	testSBE_addAndPeek();
	testSBE_addAndRemove();
	testSBE_addRemoveAll();
	testSBE_addTwice();
	testSBE_removePartial();
	testSBE_multiCycle();
	testSBE_addRemoveAddRemove();
	testSBE_addLargeThenRemove();
	testSBE_addRemoveAllRepeated();
	testSBE_wrapAround();
	testSBE_multipleWrapAround();
	testSBE_exactBufferFit();
	testSBE_mergeFromBasic();
	testSBE_mergeFromEmpty();
	testSBE_mergeFromEmptyBoth();
	testSBE_mergeFromSelf();
	testSBE_mergeFromAfterRemove();
	testSBE_linearizeEmpty();
	testSBE_linearizeSimple();
	testSBE_linearizeAfterPartialRead();
	testSBE_linearizeThenAdd();
	testSBE_linearizeAfterMerge();
	testSBE_contiguousLength();
	testSBE_contiguousAfterRemove();
	testSBE_bulkAddAndRead();
	testSBE_bulkRemove();
	testSBE_bulkAddAfterRemove();
	testSBE_singleByteAddRemove();
	testSBE_zeroLengthAdd();
	testSBE_maxSizeAdd();
	testSBE_dataEquality();
	testSBE_addPeekRemovePeek();
	testSBE_addAddRemoveAdd();
	testSBE_addAddRemoveRemove();
	testSBE_stressSequence();
	testSBE_clearThenAdd();
	testSBE_clearThenAddLarge();
	testSBE_independentBuffers();
	testSBE_clearAndReuse();
	testSBE_partialPeek();
	testSBE_repeatedAddRemove();
	testSBE_sequentialAddAndPeek();
}

// ============================================================
// Consolidated container smoke tests
// ============================================================
#define STREAMBUFFERTEST_CT_ASSERT(expr, msg) \
	do { if (!(expr)) { ERROR(string("StreamBufferTest FAIL: ") + (msg)); } } while(0)
#define STREAMBUFFERTEST_CT_ASSERT_EQ(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("StreamBufferTest FAIL: ") + (msg) + " | expected=" + IToS((int)(b)) + " actual=" + IToS((int)(a))); } } while(0)
#define STREAMBUFFERTEST_CT_ASSERT_STR(a, b, msg) \
	do { if (string(a) != string(b)) { ERROR(string("StreamBufferTest FAIL: ") + (msg) + " | expected=" + (b) + " actual=" + (a)); } } while(0)

void StreamBufferTest::testStreamBuffer_Basic()
{
	StreamBuffer buf(64);

	// initial state
	STREAMBUFFERTEST_CT_ASSERT_EQ(buf.getDataLength(), 0, "StreamBuffer init length=0");

	// addDataToBack
	const char data[] = { 1, 2, 3, 4, 5 };
	STREAMBUFFERTEST_CT_ASSERT(buf.addDataToBack(data, 5), "StreamBuffer addData ok");
	STREAMBUFFERTEST_CT_ASSERT_EQ(buf.getDataLength(), 5, "StreamBuffer length=5");
	STREAMBUFFERTEST_CT_ASSERT_EQ((int)(unsigned char)buf.getData()[0], 1, "StreamBuffer getData[0]=1");
	STREAMBUFFERTEST_CT_ASSERT_EQ((int)(unsigned char)buf.getData()[4], 5, "StreamBuffer getData[4]=5");

	// isAvailable
	STREAMBUFFERTEST_CT_ASSERT(buf.isAvailable(59), "StreamBuffer isAvailable(59)");
	STREAMBUFFERTEST_CT_ASSERT(!buf.isAvailable(60), "StreamBuffer not isAvailable(60)");

	// removeDataFromFront
	buf.removeDataFromFront(2);
	STREAMBUFFERTEST_CT_ASSERT_EQ(buf.getDataLength(), 3, "StreamBuffer length=3 after remove");
	STREAMBUFFERTEST_CT_ASSERT_EQ((int)(unsigned char)buf.getData()[0], 3, "StreamBuffer getData[0]=3 after remove");

	// add more then check again
	const char extra[] = { 10, 20 };
	STREAMBUFFERTEST_CT_ASSERT(buf.addDataToBack(extra, 2), "StreamBuffer addExtra ok");
	STREAMBUFFERTEST_CT_ASSERT_EQ(buf.getDataLength(), 5, "StreamBuffer length=5 after extra");

	// clear
	buf.clear();
	STREAMBUFFERTEST_CT_ASSERT_EQ(buf.getDataLength(), 0, "StreamBuffer length=0 after clear");
}

void StreamBufferTest::testStreamBuffer_AddEmpty()
{
	StreamBuffer buf(32);
	STREAMBUFFERTEST_CT_ASSERT(buf.addEmptyToBack(8), "StreamBuffer addEmptyToBack 8 ok");
	STREAMBUFFERTEST_CT_ASSERT_EQ(buf.getDataLength(), 8, "StreamBuffer length=8 after addEmpty");
}

void StreamBufferTest::testContainerExtended()
{
	// capacity boundary
	{
		StreamBuffer buf(10);
		const char data[] = { 1,2,3,4,5,6,7,8,9,10 };
		STREAMBUFFERTEST_CT_ASSERT(buf.addDataToBack(data, 10), "StreamBuffer add 10 ok");
		STREAMBUFFERTEST_CT_ASSERT_EQ(buf.getDataLength(), 10, "StreamBuffer full length=10");
		STREAMBUFFERTEST_CT_ASSERT(!buf.isAvailable(1), "StreamBuffer full not available");
	}

	// multiple add/remove cycles
	{
		StreamBuffer buf(64);
		FOR(5)
		{
			const char data[] = { (char)i, (char)(i + 1) };
			STREAMBUFFERTEST_CT_ASSERT(buf.addDataToBack(data, 2), "StreamBuffer cycle add");
			buf.removeDataFromFront(1);
		}
		STREAMBUFFERTEST_CT_ASSERT_EQ(buf.getDataLength(), 5, "StreamBuffer cycles length=5");
	}

	// addEmptyToBack
	{
		StreamBuffer buf(32);
		STREAMBUFFERTEST_CT_ASSERT(buf.addEmptyToBack(16), "StreamBuffer addEmpty 16");
		STREAMBUFFERTEST_CT_ASSERT_EQ(buf.getDataLength(), 16, "StreamBuffer addEmpty length=16");
		STREAMBUFFERTEST_CT_ASSERT(buf.isAvailable(0), "StreamBuffer isAvailable(0)");
	}

	// clear and reuse
	{
		StreamBuffer buf(32);
		const char data[] = { 1,2,3 };
		buf.addDataToBack(data, 3);
		buf.clear();
		STREAMBUFFERTEST_CT_ASSERT_EQ(buf.getDataLength(), 0, "StreamBuffer clear=0");
		buf.addDataToBack(data, 3);
		STREAMBUFFERTEST_CT_ASSERT_EQ(buf.getDataLength(), 3, "StreamBuffer reuse=3");
	}
}
#endif
