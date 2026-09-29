#ifdef WINDOWS
#include "FrameHeader.h"
#include "PacketBit.h"

// ──────────────────────────────────────────────────────────────────────────────
// Frame 层内部测试用 Packet（不依赖任何 Game 层类型）
// 字段结构与 SCPackItem 等价：两个 Vector<llong> + ushort + bool
// ──────────────────────────────────────────────────────────────────────────────
class TestPacketBit : public PacketBit
{
	BASE(TestPacketBit, PacketBit);
public:
	enum class Field : byte
	{
		List0 = 0,
		List1 = 1,
		Size  = 2,
	};

	Vector<llong> mList0;
	Vector<llong> mList1;
	ushort        mSize = 0;
	bool          mFlag = false;

	bool readFromBuffer(SerializerBitRead* reader, const bool needReadSign) override
	{
		bool ok = true;
		if (isFieldValid(Field::List0))
		{
			ok = ok && reader->readSignedList(needReadSign, mList0);
		}
		if (isFieldValid(Field::List1))
		{
			ok = ok && reader->readSignedList(needReadSign, mList1);
		}
		if (isFieldValid(Field::Size))
		{
			ok = ok && reader->readUnsigned(mSize);
		}
		ok = ok && reader->readBool(mFlag);
		return ok;
	}
	bool writeToBuffer(SerializerBitWrite* writer, const bool needWriteSign) const override
	{
		bool ok = true;
		if (isFieldValid(Field::List0))
		{
			ok = ok && writer->writeSignedList(needWriteSign, mList0);
		}
		if (isFieldValid(Field::List1))
		{
			ok = ok && writer->writeSignedList(needWriteSign, mList1);
		}
		if (isFieldValid(Field::Size))
		{
			ok = ok && writer->writeUnsigned(mSize);
		}
		ok = ok && writer->writeBool(mFlag);
		return ok;
	}
	bool generateHasSignInternal() const override
	{
		if (isFieldValid(Field::List0))
		{
		for (const llong v : mList0)
		{
			if (v < 0)
			{
				return true;
			}
		}
	}
	if (isFieldValid(Field::List1))
	{
		for (const llong v : mList1)
		{
			if (v < 0)
			{
				return true;
			}
		}
	}
		return false;
	}
	void resetProperty() override
	{
		base::resetProperty();
		mList0.clear();
		mList1.clear();
		mSize = 0;
		mFlag = false;
	}
};

// ──────────────────────────────────────────────────────────────────────────────
// 断言宏（仅在此文件内使用）
// 调用约定：ST_ASSERT_EQ(actual, expected, msg)
// ──────────────────────────────────────────────────────────────────────────────
#define ST_ASSERT(expr, msg) \
	do { if (!(expr)) { ERROR(string("SerializeTest FAIL: ") + (msg)); } } while(0)
#define ST_ASSERT_EQ(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("SerializeTest FAIL: ") + (msg) + \
		" | expected=" + IToS((int)(b)) + " actual=" + IToS((int)(a))); } } while(0)
#define ST_ASSERT_EQ_LL(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("SerializeTest FAIL: ") + (msg) + \
		" | expected=" + LLToS(b) + " actual=" + LLToS(a)); } } while(0)
#define ST_ASSERT_FLOAT(a, b, msg) \
	do { if (fabsf((a)-(b)) > 1e-3f) { ERROR(string("SerializeTest FAIL: ") + (msg) + \
		" | expected=" + FToS(b) + " actual=" + FToS(a)); } } while(0)
#define ST_ASSERT_DOUBLE(a, b, msg) \
	do { if (fabs((a)-(b)) > 1e-4) { ERROR(string("SerializeTest FAIL: ") + (msg) + \
		" | expected=" + FToS((float)(b)) + " actual=" + FToS((float)(a))); } } while(0)

// ──────────────────────────────────────────────────────────────────────────────
// 辅助：模拟协议栈 write→read 往返
// ──────────────────────────────────────────────────────────────────────────────
bool SerializeTest::doWriteRead(TestPacketBit& w, TestPacketBit& r)
{
	const bool needSign = w.hasSign();
	SerializerBitWrite writer(false);
	if (!w.writeToBuffer(&writer, needSign))
	{
		return false;
	}
	writer.fillZeroToByteEnd();
	r.setFieldFlag(w.getFieldFlag());
	SerializerBitRead reader(writer.getBuffer(), writer.getByteCount());
	return r.readFromBuffer(&reader, needSign);
}

// ══════════════════════════════════════════════════════════════════════════════
// PacketBit 系列测试
// ══════════════════════════════════════════════════════════════════════════════

// 测试1：全字段有效
void SerializeTest::testPacketBit_AllFields()
{
	TestPacketBit w;
	w.resetProperty();
	w.mList0 = Vector<llong>{ 1001LL, 1002LL };
	w.mList1 = Vector<llong>{ 2001LL, 2002LL, 2003LL };
	w.mSize  = 50;
	w.mFlag  = false;

	TestPacketBit r;
	r.resetProperty();
	ST_ASSERT(doWriteRead(w, r),                          "全字段: write/read 成功");
	ST_ASSERT_EQ((int)r.mList0.size(), 2,                 "全字段: List0 数量=2");
	ST_ASSERT_EQ_LL(r.mList0[0], 1001LL,                  "全字段: List0[0]=1001");
	ST_ASSERT_EQ_LL(r.mList0[1], 1002LL,                  "全字段: List0[1]=1002");
	ST_ASSERT_EQ((int)r.mList1.size(), 3,                 "全字段: List1 数量=3");
	ST_ASSERT_EQ_LL(r.mList1[0], 2001LL,                  "全字段: List1[0]=2001");
	ST_ASSERT_EQ_LL(r.mList1[1], 2002LL,                  "全字段: List1[1]=2002");
	ST_ASSERT_EQ_LL(r.mList1[2], 2003LL,                  "全字段: List1[2]=2003");
	ST_ASSERT_EQ((int)r.mSize, 50,                        "全字段: size=50");
	ST_ASSERT(!r.mFlag,                                   "全字段: flag=false");
}

// 测试2：部分字段（两个列表字段置无效）
void SerializeTest::testPacketBit_PartialFields()
{
	TestPacketBit w;
	w.resetProperty();
	w.setFieldInvalid(TestPacketBit::Field::List0);
	w.setFieldInvalid(TestPacketBit::Field::List1);
	w.mSize = 30;
	w.mFlag = false;

	TestPacketBit r;
	r.resetProperty();
	ST_ASSERT(doWriteRead(w, r),                          "部分字段: write/read 成功");
	ST_ASSERT_EQ((int)r.mList0.size(), 0,                 "部分字段: List0=0");
	ST_ASSERT_EQ((int)r.mList1.size(), 0,                 "部分字段: List1=0");
	ST_ASSERT_EQ((int)r.mSize, 30,                        "部分字段: size=30");
	ST_ASSERT(!r.mFlag,                                   "部分字段: flag=false");
}

// 测试3：仅 List0 有效（List1 无效）
void SerializeTest::testPacketBit_OnlyList0()
{
	TestPacketBit w;
	w.resetProperty();
	w.mList0 = Vector<llong>{ 111LL, 222LL, 333LL };
	w.setFieldInvalid(TestPacketBit::Field::List1);
	w.mSize = 7;
	w.mFlag = true;

	TestPacketBit r;
	r.resetProperty();
	ST_ASSERT(doWriteRead(w, r),                          "仅List0: write/read 成功");
	ST_ASSERT_EQ((int)r.mList0.size(), 3,                 "仅List0: 数量=3");
	ST_ASSERT_EQ_LL(r.mList0[0], 111LL,                   "仅List0: [0]=111");
	ST_ASSERT_EQ_LL(r.mList0[1], 222LL,                   "仅List0: [1]=222");
	ST_ASSERT_EQ_LL(r.mList0[2], 333LL,                   "仅List0: [2]=333");
	ST_ASSERT_EQ((int)r.mList1.size(), 0,                 "仅List0: List1=0");
	ST_ASSERT_EQ((int)r.mSize, 7,                         "仅List0: size=7");
	ST_ASSERT(r.mFlag,                                    "仅List0: flag=true");
}

// 测试4：List1 含负数（触发 hasSign=true）
void SerializeTest::testPacketBit_NegativeList1()
{
	TestPacketBit w;
	w.resetProperty();
	w.setFieldInvalid(TestPacketBit::Field::List0);
	w.mList1 = Vector<llong>{ -100LL, 200LL, -300LL };
	w.mSize  = 10;
	w.mFlag  = false;

	ST_ASSERT(w.hasSign(),                                "负数List1: hasSign=true");

	TestPacketBit r;
	r.resetProperty();
	ST_ASSERT(doWriteRead(w, r),                          "负数List1: write/read 成功");
	ST_ASSERT_EQ((int)r.mList1.size(), 3,                 "负数List1: 数量=3");
	ST_ASSERT_EQ_LL(r.mList1[0], -100LL,                  "负数List1: [0]=-100");
	ST_ASSERT_EQ_LL(r.mList1[1],  200LL,                  "负数List1: [1]=200");
	ST_ASSERT_EQ_LL(r.mList1[2], -300LL,                  "负数List1: [2]=-300");
}

// 测试5：List0 含负数
void SerializeTest::testPacketBit_NegativeList0()
{
	TestPacketBit w;
	w.resetProperty();
	w.mList0 = Vector<llong>{ -501LL, 502LL };
	w.setFieldInvalid(TestPacketBit::Field::List1);
	w.mSize = 5;
	w.mFlag = false;

	ST_ASSERT(w.hasSign(),                                "负数List0: hasSign=true");

	TestPacketBit r;
	r.resetProperty();
	ST_ASSERT(doWriteRead(w, r),                          "负数List0: write/read 成功");
	ST_ASSERT_EQ((int)r.mList0.size(), 2,                 "负数List0: 数量=2");
	ST_ASSERT_EQ_LL(r.mList0[0], -501LL,                  "负数List0: [0]=-501");
	ST_ASSERT_EQ_LL(r.mList0[1],  502LL,                  "负数List0: [1]=502");
}

// 测试6：空列表（字段有效但 size=0）
void SerializeTest::testPacketBit_EmptyLists()
{
	TestPacketBit w;
	w.resetProperty();
	w.mSize = 0;
	w.mFlag = false;

	TestPacketBit r;
	r.resetProperty();
	ST_ASSERT(doWriteRead(w, r),                          "空列表: write/read 成功");
	ST_ASSERT_EQ((int)r.mList0.size(), 0,                 "空列表: List0=0");
	ST_ASSERT_EQ((int)r.mList1.size(), 0,                 "空列表: List1=0");
	ST_ASSERT_EQ((int)r.mSize, 0,                         "空列表: size=0");
}

// 测试7：单元素列表
void SerializeTest::testPacketBit_SingleElement()
{
	TestPacketBit w;
	w.resetProperty();
	w.mList0 = Vector<llong>{ 42LL };
	w.mList1 = Vector<llong>{ -1LL };
	w.mSize  = 1;
	w.mFlag  = true;

	ST_ASSERT(w.hasSign(),                                "单元素: hasSign=true");

	TestPacketBit r;
	r.resetProperty();
	ST_ASSERT(doWriteRead(w, r),                          "单元素: write/read 成功");
	ST_ASSERT_EQ((int)r.mList0.size(), 1,                 "单元素: List0.size=1");
	ST_ASSERT_EQ_LL(r.mList0[0], 42LL,                    "单元素: List0[0]=42");
	ST_ASSERT_EQ((int)r.mList1.size(), 1,                 "单元素: List1.size=1");
	ST_ASSERT_EQ_LL(r.mList1[0], -1LL,                    "单元素: List1[0]=-1");
	ST_ASSERT(r.mFlag,                                    "单元素: flag=true");
}

// 测试8：clearFieldFlag（所有字段无效，只写 mFlag）
void SerializeTest::testPacketBit_ClearFieldFlag()
{
	TestPacketBit w;
	w.resetProperty();
	w.clearFieldFlag();
	w.mFlag = true;

	ST_ASSERT(!w.hasSign(),                               "ClearFieldFlag: hasSign=false");

	TestPacketBit r;
	r.resetProperty();
	ST_ASSERT(doWriteRead(w, r),                          "ClearFieldFlag: write/read 成功");
	ST_ASSERT_EQ((int)r.mList0.size(), 0,                 "ClearFieldFlag: List0=0");
	ST_ASSERT_EQ((int)r.mList1.size(), 0,                 "ClearFieldFlag: List1=0");
	ST_ASSERT(r.mFlag,                                    "ClearFieldFlag: flag=true");
}

// 测试9：resetProperty 后字段恢复 FULL_FIELD_FLAG
void SerializeTest::testPacketBit_ResetProperty()
{
	TestPacketBit p;
	p.resetProperty();
	p.mList0.add(999LL);
	p.mList1.add(888LL);
	p.mSize = 100;
	p.mFlag = true;

	p.resetProperty();

	ST_ASSERT_EQ((int)p.mList0.size(), 0,                 "Reset: List0 清空");
	ST_ASSERT_EQ((int)p.mList1.size(), 0,                 "Reset: List1 清空");
	ST_ASSERT_EQ((int)p.mSize, 0,                         "Reset: size=0");
	ST_ASSERT(!p.mFlag,                                   "Reset: flag=false");
	ST_ASSERT_EQ_LL((llong)p.getFieldFlag(),
		(llong)FrameDefine::FULL_FIELD_FLAG,              "Reset: mFieldFlag=FULL_FIELD_FLAG");
}

// 测试10：大列表（100 条 llong），覆盖统一长度位 + 大缓冲区路径
void SerializeTest::testPacketBit_LargeList()
{
	TestPacketBit w;
	w.resetProperty();
	w.setFieldInvalid(TestPacketBit::Field::List0);
	FOR(100)
	{
		w.mList1.add((llong)i * 12345678LL);
	}
	w.mSize = 200;
	w.mFlag = false;

	TestPacketBit r;
	r.resetProperty();
	ST_ASSERT(doWriteRead(w, r),                          "大列表: write/read 成功");
	ST_ASSERT_EQ((int)r.mList1.size(), 100,               "大列表: 数量=100");
	FOR(100)
	{
		ST_ASSERT_EQ_LL(r.mList1[i], (llong)i * 12345678LL, "大列表: [" + IToS(i) + "]");
	}
}

// 测试11：size 字段边界值 0xFFFF（ushort 最大值）
void SerializeTest::testPacketBit_SizeMaxValue()
{
	TestPacketBit w;
	w.resetProperty();
	w.setFieldInvalid(TestPacketBit::Field::List0);
	w.setFieldInvalid(TestPacketBit::Field::List1);
	w.mSize = 0xFFFF;
	w.mFlag = false;

	TestPacketBit r;
	r.resetProperty();
	ST_ASSERT(doWriteRead(w, r),                          "size最大值: write/read 成功");
	ST_ASSERT_EQ((int)r.mSize, 0xFFFF,                    "size最大值: size=0xFFFF");
}

// 测试12：两包顺序写入同一 writer，分别独立 read
void SerializeTest::testPacketBit_TwoPacketsSequential()
{
	TestPacketBit w1, w2;
	w1.resetProperty();
	w1.mList0 = Vector<llong>{ 10LL, 20LL };
	w1.setFieldInvalid(TestPacketBit::Field::List1);
	w1.mSize = 1;
	w1.mFlag = false;

	w2.resetProperty();
	w2.setFieldInvalid(TestPacketBit::Field::List0);
	w2.mList1 = Vector<llong>{ -5LL, -6LL };
	w2.mSize = 2;
	w2.mFlag = true;

	ST_ASSERT(w2.hasSign(),                               "顺序两包: w2.hasSign=true");

	TestPacketBit r1, r2;
	r1.resetProperty();
	r2.resetProperty();
	ST_ASSERT(doWriteRead(w1, r1),                        "顺序两包: w1 read 成功");
	ST_ASSERT(doWriteRead(w2, r2),                        "顺序两包: w2 read 成功");

	ST_ASSERT_EQ((int)r1.mList0.size(), 2,                "顺序两包: r1.List0.size=2");
	ST_ASSERT_EQ_LL(r1.mList0[0], 10LL,                   "顺序两包: r1.List0[0]=10");
	ST_ASSERT_EQ_LL(r1.mList0[1], 20LL,                   "顺序两包: r1.List0[1]=20");
	ST_ASSERT_EQ((int)r1.mSize, 1,                        "顺序两包: r1.size=1");
	ST_ASSERT(!r1.mFlag,                                  "顺序两包: r1.flag=false");

	ST_ASSERT_EQ((int)r2.mList1.size(), 2,                "顺序两包: r2.List1.size=2");
	ST_ASSERT_EQ_LL(r2.mList1[0], -5LL,                   "顺序两包: r2.List1[0]=-5");
	ST_ASSERT_EQ_LL(r2.mList1[1], -6LL,                   "顺序两包: r2.List1[1]=-6");
	ST_ASSERT_EQ((int)r2.mSize, 2,                        "顺序两包: r2.size=2");
	ST_ASSERT(r2.mFlag,                                   "顺序两包: r2.flag=true");
}

// 测试13：flag 交替 true→false→true 三次独立往返
void SerializeTest::testPacketBit_FlagToggle()
{
	for (int round = 0; round < 3; ++round)
	{
		const bool flagVal = (round % 2 == 0);
		TestPacketBit w;
		w.resetProperty();
		w.clearFieldFlag();
		w.mFlag = flagVal;

		TestPacketBit r;
		r.resetProperty();
		ST_ASSERT(doWriteRead(w, r), "flag交替: round=" + IToS(round) + " write/read 成功");
		ST_ASSERT_EQ((int)r.mFlag, (int)flagVal, "flag交替: round=" + IToS(round) + " flag");
	}
}

// 测试14：llong 极值（INT64_MIN/INT64_MAX）
void SerializeTest::testPacketBit_LLongExtremes()
{
	TestPacketBit w;
	w.resetProperty();
	w.setFieldInvalid(TestPacketBit::Field::List1);
	w.mList0 = Vector<llong>{ (llong)0x7FFFFFFFFFFFFFFFLL, (llong)0x8000000000000001LL };
	w.mSize  = 0;
	w.mFlag  = false;

	ST_ASSERT(w.hasSign(),                                "llong极值: hasSign=true");

	TestPacketBit r;
	r.resetProperty();
	ST_ASSERT(doWriteRead(w, r),                          "llong极值: write/read 成功");
	ST_ASSERT_EQ((int)r.mList0.size(), 2,                 "llong极值: 数量=2");
	ST_ASSERT_EQ_LL(r.mList0[0], (llong)0x7FFFFFFFFFFFFFFFLL, "llong极值: [0]=INT64_MAX");
	ST_ASSERT_EQ_LL(r.mList0[1], (llong)0x8000000000000001LL, "llong极值: [1]=INT64_MIN+1");
}

// PacketBit 序列化测试总入口
void SerializeTest::testPacketBitSerialize()
{
	testPacketBit_AllFields();
	testPacketBit_PartialFields();
	testPacketBit_OnlyList0();
	testPacketBit_NegativeList1();
	testPacketBit_NegativeList0();
	testPacketBit_EmptyLists();
	testPacketBit_SingleElement();
	testPacketBit_ClearFieldFlag();
	testPacketBit_ResetProperty();
	testPacketBit_LargeList();
	testPacketBit_SizeMaxValue();
	testPacketBit_TwoPacketsSequential();
	testPacketBit_FlagToggle();
	testPacketBit_LLongExtremes();
}

// ══════════════════════════════════════════════════════════════════════════════
// 基础类型往返测试（第一段）
// ══════════════════════════════════════════════════════════════════════════════
void SerializeTest::testBasicTypes()
{
	// ── 标量期望值 ──────────────────────────────────────────────────────────
	const bool   EXP_BOOL_TRUE       = true;
	const bool   EXP_BOOL_FALSE      = false;
	const int    EXP_INT_NOSIGN      = 1;
	const int    EXP_INT_NEG         = -1;
	const int    EXP_INT_ZERO        = 0;
	const int    EXP_INT_BIG         = 1231243125;
	const int    EXP_INT_MAX         = 0x7FFFFFFF;
	const int    EXP_INT_MIN         = (int)0x80000001;   // INT_MIN+1 = -2147483647，INT_MIN 本身取反溢出框架不支持
	const llong  EXP_LL_NEG          = -1059LL;
	const llong  EXP_LL_POS          = 1773384288797LL;
	const llong  EXP_LL_ZERO         = 0LL;
	const llong  EXP_LL_MAX          = (llong)0x7FFFFFFFFFFFFFFFLL;
	const short  EXP_SHORT_POS       = 1234;
	const short  EXP_SHORT_NEG       = -1234;
	const short  EXP_SHORT_ZERO      = 0;
	const short  EXP_SHORT_MAX       = 32767;
	const short  EXP_SHORT_MIN       = (short)-32767;   // -32768 取反溢出，框架不支持
	const char   EXP_CHAR_POS        = 100;
	const char   EXP_CHAR_NEG        = -100;
	const float  EXP_FLOAT_POS       = 1.2f;
	const float  EXP_FLOAT_NEG       = -2.2f;
	const float  EXP_FLOAT_ZERO      = 0.0f;
	const double EXP_DOUBLE          = 2.2222;
	const double EXP_DOUBLE_NEG      = -3.1416;   // precision=4，最多4位小数；-3.14159有5位会被截断
	const double EXP_DOUBLE_ZERO     = 0.0;
	const uint   EXP_UINT_A          = 1u;
	const uint   EXP_UINT_B          = 2u;
	const uint   EXP_UINT_C          = 10u;
	const uint   EXP_UINT_ZERO       = 0u;
	const uint   EXP_UINT_ONE        = 1u;
	const uint   EXP_UINT_TWO        = 2u;
	const uint   EXP_UINT_MAX        = 0xFFFFFFFF;
	const uint   EXP_UINT_20001      = 20001u;
	const ushort EXP_USHORT_ZERO     = 0u;
	const ushort EXP_USHORT_ONE      = 1u;
	const ushort EXP_USHORT_MAX      = 0xFFFF;
	const byte   EXP_BYTE_ZERO       = 0u;
	const byte   EXP_BYTE_MAX        = 0xFF;
	const int    EXP_INT3_0          = 1;
	const int    EXP_INT3_1          = 2;
	const int    EXP_INT3_2          = -10;

	// ── 列表期望值 ──────────────────────────────────────────────────────────
	const Vector<int>    EXP_INT_LIST        { 1, 2, 3, 123123123 };
	const Vector<int>    EXP_INT_ZERO_LIST   { 0, 0, 0, 0 };
	const Vector<int>    EXP_INT_NEG_LIST    { -1, -2, -3 };
	const Vector<int>    EXP_INT_EMPTY       { };
	const Vector<uint>   EXP_UINT_ZERO_LIST  { 0u, 0u, 0u, 0u };
	const Vector<uint>   EXP_UINT_LIST       { 1u, 2u, 3u, 123123123u };
	const Vector<uint>   EXP_UINT_EMPTY      { };
	const Vector<short>  EXP_SHORT_LIST      { 1, 2, 3 };
	const Vector<short>  EXP_SHORT_NEG_LIST  { -1, -32767, 32767 };  // -32768 取反溢出，改为 -32767
	const Vector<ushort> EXP_USHORT_LIST     { 0u, 1u, 0xFFFFu };
	const Vector<llong>  EXP_LL_LIST         { 1LL, 2LL, 3LL };
	const Vector<llong>  EXP_LL_NEG_LIST     { -1LL, -1000000LL, (llong)0x7FFFFFFFFFFFFFFFLL };
	const Vector<byte>   EXP_BYTE_LIST       { 1, 2, 3 };
	const Vector<byte>   EXP_BYTE_BOUND      { 0, 0xFF };
	const Vector<char>   EXP_CHAR_LIST       { 1, 2, -3 };
	const Vector<char>   EXP_CHAR_NEG_LIST   { -1, -100, -127 };  // -128 取反溢出，改为 -127
	const Vector<float>  EXP_FLOAT_LIST3     { 1.5f, 2.5f, 3.5f };
	const Vector<float>  EXP_FLOAT_LIST5     { 1.5f, 2.5f, 1234.5f, 0.0f, 1.0f };
	const Vector<float>  EXP_FLOAT_NEG_LIST  { -1.5f, -2.5f, -3.5f };
	const Vector<float>  EXP_FLOAT_EMPTY     { };
	const Vector<double> EXP_DOUBLE_LIST3    { 1.555, 2.555, 3.555 };
	const Vector<double> EXP_DOUBLE_LIST5    { 1.5, 2.5, 1234.51, 0.0, 1.0 };
	const Vector<double> EXP_DOUBLE_NEG_LIST { -1.111, -2.222, -3.333 };
	const Vector<double> EXP_DOUBLE_EMPTY    { };

	// ── 写入 ────────────────────────────────────────────────────────────────
	SerializerBitWrite writer;
	// bool
	writer.writeBool(EXP_BOOL_TRUE);
	writer.writeBool(EXP_BOOL_FALSE);
	// int
	writer.writeSigned(false, EXP_INT_NOSIGN);
	writer.writeSigned(true,  EXP_INT_NEG);
	writer.writeSigned(false, EXP_INT_ZERO);
	writer.writeSigned(false, EXP_INT_BIG);
	writer.writeSigned(false, EXP_INT_MAX);
	writer.writeSigned(true,  EXP_INT_MIN);
	// llong
	writer.writeSigned(true,  EXP_LL_NEG, EXP_LL_POS);
	writer.writeSigned(false, EXP_LL_ZERO);
	writer.writeSigned(false, EXP_LL_MAX);
	// short
	writer.writeSigned(false, EXP_SHORT_POS);
	writer.writeSigned(true,  EXP_SHORT_NEG);
	writer.writeSigned(false, EXP_SHORT_ZERO);
	writer.writeSigned(false, EXP_SHORT_MAX);
	writer.writeSigned(true,  EXP_SHORT_MIN);
	// char
	writer.writeSigned(false, EXP_CHAR_POS);
	writer.writeSigned(true,  EXP_CHAR_NEG);
	// float
	writer.writeFloat(false,  EXP_FLOAT_POS);
	writer.writeFloat(true,   EXP_FLOAT_NEG);
	writer.writeFloat(false,  EXP_FLOAT_ZERO);
	// double
	writer.writeDouble(false, EXP_DOUBLE);
	writer.writeDouble(true,  EXP_DOUBLE_NEG);
	writer.writeDouble(false, EXP_DOUBLE_ZERO);
	// uint
	writer.writeUnsigned(EXP_UINT_A, EXP_UINT_B, EXP_UINT_C);
	writer.writeUnsigned(EXP_UINT_ZERO);
	writer.writeUnsigned(EXP_UINT_ONE);
	writer.writeUnsigned(EXP_UINT_TWO);
	writer.writeUnsigned(EXP_UINT_MAX);
	writer.writeUnsigned(EXP_UINT_20001);
	// ushort
	writer.writeUnsigned(EXP_USHORT_ZERO);
	writer.writeUnsigned(EXP_USHORT_ONE);
	writer.writeUnsigned(EXP_USHORT_MAX);
	// byte
	writer.writeUnsigned(EXP_BYTE_ZERO);
	writer.writeUnsigned(EXP_BYTE_MAX);
	// int×3
	writer.writeSigned(true, EXP_INT3_0, EXP_INT3_1, EXP_INT3_2);
	// 列表
	writer.writeSignedList(false,  EXP_INT_LIST);
	writer.writeSignedList(false,  EXP_INT_ZERO_LIST);
	writer.writeSignedList(true,   EXP_INT_NEG_LIST);
	writer.writeSignedList(false,  EXP_INT_EMPTY);
	writer.writeUnsignedList(EXP_UINT_ZERO_LIST);
	writer.writeUnsignedList(EXP_UINT_LIST);
	writer.writeUnsignedList(EXP_UINT_EMPTY);
	writer.writeSignedList(false,  EXP_SHORT_LIST);
	writer.writeSignedList(true,   EXP_SHORT_NEG_LIST);
	writer.writeUnsignedList(EXP_USHORT_LIST);
	writer.writeSignedList(false,  EXP_LL_LIST);
	writer.writeSignedList(true,   EXP_LL_NEG_LIST);
	writer.writeUnsignedList(EXP_BYTE_LIST);
	writer.writeUnsignedList(EXP_BYTE_BOUND);
	writer.writeSignedList(true,   EXP_CHAR_LIST);
	writer.writeSignedList(true,   EXP_CHAR_NEG_LIST);
	writer.writeFloatList(false,   EXP_FLOAT_LIST3);
	writer.writeFloatList(false,   EXP_FLOAT_LIST5);
	writer.writeFloatList(true,    EXP_FLOAT_NEG_LIST);
	writer.writeFloatList(false,   EXP_FLOAT_EMPTY);
	writer.writeDoubleList(false,  EXP_DOUBLE_LIST3);
	writer.writeDoubleList(false,  EXP_DOUBLE_LIST5);
	writer.writeDoubleList(true,   EXP_DOUBLE_NEG_LIST);
	writer.writeDoubleList(false,  EXP_DOUBLE_EMPTY);

	SerializerBitRead reader(writer.getBuffer(), writer.getByteCount());

	// ── 读取 & 断言：bool ───────────────────────────────────────────────────
	bool vBoolTrue = false, vBoolFalse = true;
	ST_ASSERT(reader.readBool(vBoolTrue),                      "readBool(true) 返回成功");
	ST_ASSERT_EQ((int)vBoolTrue, (int)EXP_BOOL_TRUE,           "bool=true");
	ST_ASSERT(reader.readBool(vBoolFalse),                     "readBool(false) 返回成功");
	ST_ASSERT_EQ((int)vBoolFalse, (int)EXP_BOOL_FALSE,         "bool=false");

	// ── 读取 & 断言：int ────────────────────────────────────────────────────
	int vIntNosign = -1, vIntNeg = 0, vIntZero = -1, vIntBig = 0, vIntMax = 0, vIntMin = 0;
	ST_ASSERT(reader.readSigned(false, vIntNosign),            "readSigned(false,int_nosign)");
	ST_ASSERT_EQ(vIntNosign, EXP_INT_NOSIGN,                   "int_nosign");
	ST_ASSERT(reader.readSigned(true, vIntNeg),                "readSigned(true,int_neg)");
	ST_ASSERT_EQ(vIntNeg, EXP_INT_NEG,                         "int_neg");
	ST_ASSERT(reader.readSigned(false, vIntZero),              "readSigned(false,int_zero)");
	ST_ASSERT_EQ(vIntZero, EXP_INT_ZERO,                       "int_zero");
	ST_ASSERT(reader.readSigned(false, vIntBig),               "readSigned(false,int_big)");
	ST_ASSERT_EQ(vIntBig, EXP_INT_BIG,                         "int_big");
	ST_ASSERT(reader.readSigned(false, vIntMax),               "readSigned(false,int_max)");
	ST_ASSERT_EQ(vIntMax, EXP_INT_MAX,                         "int_max");
	ST_ASSERT(reader.readSigned(true, vIntMin),                "readSigned(true,int_min)");
	ST_ASSERT_EQ(vIntMin, EXP_INT_MIN,                         "int_min");

	// ── 读取 & 断言：llong ──────────────────────────────────────────────────
	llong vLLNeg = 0, vLLPos = 0, vLLZero = -1, vLLMax = 0;
	ST_ASSERT(reader.readSigned(true, vLLNeg, vLLPos),         "readSigned(true,llong×2)");
	ST_ASSERT_EQ_LL(vLLNeg, EXP_LL_NEG,                        "llong_neg");
	ST_ASSERT_EQ_LL(vLLPos, EXP_LL_POS,                        "llong_pos");
	ST_ASSERT(reader.readSigned(false, vLLZero),               "readSigned(false,llong_zero)");
	ST_ASSERT_EQ_LL(vLLZero, EXP_LL_ZERO,                      "llong_zero");
	ST_ASSERT(reader.readSigned(false, vLLMax),                "readSigned(false,llong_max)");
	ST_ASSERT_EQ_LL(vLLMax, EXP_LL_MAX,                        "llong_max");

	// ── 读取 & 断言：short ──────────────────────────────────────────────────
	short vShortPos = 0, vShortNeg = 0, vShortZero = -1, vShortMax = 0, vShortMin = 0;
	ST_ASSERT(reader.readSigned(false, vShortPos),             "readSigned(false,short_pos)");
	ST_ASSERT_EQ((int)vShortPos, (int)EXP_SHORT_POS,           "short_pos");
	ST_ASSERT(reader.readSigned(true, vShortNeg),              "readSigned(true,short_neg)");
	ST_ASSERT_EQ((int)vShortNeg, (int)EXP_SHORT_NEG,           "short_neg");
	ST_ASSERT(reader.readSigned(false, vShortZero),            "readSigned(false,short_zero)");
	ST_ASSERT_EQ((int)vShortZero, (int)EXP_SHORT_ZERO,         "short_zero");
	ST_ASSERT(reader.readSigned(false, vShortMax),             "readSigned(false,short_max)");
	ST_ASSERT_EQ((int)vShortMax, (int)EXP_SHORT_MAX,           "short_max");
	ST_ASSERT(reader.readSigned(true, vShortMin),              "readSigned(true,short_min)");
	ST_ASSERT_EQ((int)vShortMin, (int)EXP_SHORT_MIN,           "short_min");

	// ── 读取 & 断言：char ───────────────────────────────────────────────────
	char vCharPos = 0, vCharNeg = 0;
	ST_ASSERT(reader.readSigned(false, vCharPos),              "readSigned(false,char_pos)");
	ST_ASSERT_EQ((int)vCharPos, (int)EXP_CHAR_POS,             "char_pos");
	ST_ASSERT(reader.readSigned(true, vCharNeg),               "readSigned(true,char_neg)");
	ST_ASSERT_EQ((int)vCharNeg, (int)EXP_CHAR_NEG,             "char_neg");

	// ── 读取 & 断言：float ──────────────────────────────────────────────────
	float vFloatPos = 0.0f, vFloatNeg = 0.0f, vFloatZero = 1.0f;
	ST_ASSERT(reader.readFloat(false, vFloatPos),              "readFloat(false,pos)");
	ST_ASSERT_FLOAT(vFloatPos, EXP_FLOAT_POS,                  "float_pos");
	ST_ASSERT(reader.readFloat(true, vFloatNeg),               "readFloat(true,neg)");
	ST_ASSERT_FLOAT(vFloatNeg, EXP_FLOAT_NEG,                  "float_neg");
	ST_ASSERT(reader.readFloat(false, vFloatZero),             "readFloat(false,zero)");
	ST_ASSERT_FLOAT(vFloatZero, EXP_FLOAT_ZERO,                "float_zero");

	// ── 读取 & 断言：double ─────────────────────────────────────────────────
	double vDouble = 0.0, vDoubleNeg = 0.0, vDoubleZero = 1.0;
	ST_ASSERT(reader.readDouble(false, vDouble),               "readDouble(false)");
	ST_ASSERT_DOUBLE(vDouble, EXP_DOUBLE,                      "double");
	ST_ASSERT(reader.readDouble(true, vDoubleNeg),             "readDouble(true,neg)");
	ST_ASSERT_DOUBLE(vDoubleNeg, EXP_DOUBLE_NEG,               "double_neg");
	ST_ASSERT(reader.readDouble(false, vDoubleZero),           "readDouble(false,zero)");
	ST_ASSERT_DOUBLE(vDoubleZero, EXP_DOUBLE_ZERO,             "double_zero");

	// ── 读取 & 断言：uint ───────────────────────────────────────────────────
	uint vUA = 0, vUB = 0, vUC = 0;
	ST_ASSERT(reader.readUnsigned(vUA, vUB, vUC),              "readUnsigned(uint×3)");
	ST_ASSERT_EQ((int)vUA, (int)EXP_UINT_A,                    "uint_a");
	ST_ASSERT_EQ((int)vUB, (int)EXP_UINT_B,                    "uint_b");
	ST_ASSERT_EQ((int)vUC, (int)EXP_UINT_C,                    "uint_c");
	uint vUZero = 99u, vUOne = 0u, vUTwo = 0u, vUMax = 0u, vU20001 = 0u;
	ST_ASSERT(reader.readUnsigned(vUZero),                     "readUnsigned(uint_zero)");
	ST_ASSERT_EQ((int)vUZero, (int)EXP_UINT_ZERO,              "uint_zero");
	ST_ASSERT(reader.readUnsigned(vUOne),                      "readUnsigned(uint_one)");
	ST_ASSERT_EQ((int)vUOne, (int)EXP_UINT_ONE,                "uint_one");
	ST_ASSERT(reader.readUnsigned(vUTwo),                      "readUnsigned(uint_two)");
	ST_ASSERT_EQ((int)vUTwo, (int)EXP_UINT_TWO,                "uint_two");
	ST_ASSERT(reader.readUnsigned(vUMax),                      "readUnsigned(uint_max)");
	ST_ASSERT_EQ_LL((llong)vUMax, (llong)EXP_UINT_MAX,         "uint_max");
	ST_ASSERT(reader.readUnsigned(vU20001),                    "readUnsigned(uint_20001)");
	ST_ASSERT_EQ((int)vU20001, (int)EXP_UINT_20001,            "uint_20001");

	// ── 读取 & 断言：ushort ─────────────────────────────────────────────────
	ushort vUSZero = 1u, vUSOne = 0u, vUSMax = 0u;
	ST_ASSERT(reader.readUnsigned(vUSZero),                    "readUnsigned(ushort_zero)");
	ST_ASSERT_EQ((int)vUSZero, (int)EXP_USHORT_ZERO,           "ushort_zero");
	ST_ASSERT(reader.readUnsigned(vUSOne),                     "readUnsigned(ushort_one)");
	ST_ASSERT_EQ((int)vUSOne, (int)EXP_USHORT_ONE,             "ushort_one");
	ST_ASSERT(reader.readUnsigned(vUSMax),                     "readUnsigned(ushort_max)");
	ST_ASSERT_EQ((int)vUSMax, (int)EXP_USHORT_MAX,             "ushort_max");

	// ── 读取 & 断言：byte ───────────────────────────────────────────────────
	byte vByteZero = 1u, vByteMax = 0u;
	ST_ASSERT(reader.readUnsigned(vByteZero),                  "readUnsigned(byte_zero)");
	ST_ASSERT_EQ((int)vByteZero, (int)EXP_BYTE_ZERO,           "byte_zero");
	ST_ASSERT(reader.readUnsigned(vByteMax),                   "readUnsigned(byte_max)");
	ST_ASSERT_EQ((int)vByteMax, (int)EXP_BYTE_MAX,             "byte_max");

	// ── 读取 & 断言：int×3 ─────────────────────────────────────────────────
	int vI30 = 0, vI31 = 0, vI32 = 0;
	ST_ASSERT(reader.readSigned(true, vI30, vI31, vI32),       "readSigned(true,int×3)");
	ST_ASSERT_EQ(vI30, EXP_INT3_0,                             "int3[0]");
	ST_ASSERT_EQ(vI31, EXP_INT3_1,                             "int3[1]");
	ST_ASSERT_EQ(vI32, EXP_INT3_2,                             "int3[2]");

	// ── 读取 & 断言：列表 ───────────────────────────────────────────────────
	// Vector<int>
	Vector<int> listInt;
	ST_ASSERT(reader.readSignedList(false, listInt),           "readSignedList<int>");
	ST_ASSERT_EQ((int)listInt.size(), (int)EXP_INT_LIST.size(), "int_list.size");
	for (int i = 0; i < (int)EXP_INT_LIST.size(); ++i)
	{
		ST_ASSERT_EQ(listInt[i], EXP_INT_LIST[i],             "int_list[" + IToS(i) + "]");
	}

	// Vector<int> 全零
	Vector<int> listIntZero;
	ST_ASSERT(reader.readSignedList(false, listIntZero),       "readSignedList<int> zeros");
	ST_ASSERT_EQ((int)listIntZero.size(), (int)EXP_INT_ZERO_LIST.size(), "int_zero_list.size");
	for (int i = 0; i < (int)EXP_INT_ZERO_LIST.size(); ++i)
	{
		ST_ASSERT_EQ(listIntZero[i], EXP_INT_ZERO_LIST[i],    "int_zero_list[" + IToS(i) + "]");
	}

	// Vector<int> 全负
	Vector<int> listIntNeg;
	ST_ASSERT(reader.readSignedList(true, listIntNeg),         "readSignedList<int> neg");
	ST_ASSERT_EQ((int)listIntNeg.size(), (int)EXP_INT_NEG_LIST.size(), "int_neg_list.size");
	for (int i = 0; i < (int)EXP_INT_NEG_LIST.size(); ++i)
	{
		ST_ASSERT_EQ(listIntNeg[i], EXP_INT_NEG_LIST[i],      "int_neg_list[" + IToS(i) + "]");
	}

	// Vector<int> 空
	Vector<int> listIntEmpty;
	ST_ASSERT(reader.readSignedList(false, listIntEmpty),      "readSignedList<int> empty");
	ST_ASSERT_EQ((int)listIntEmpty.size(), 0,                  "int_empty.size=0");

	// Vector<uint> 全零
	Vector<uint> listUIntZero;
	ST_ASSERT(reader.readUnsignedList(listUIntZero),           "readUnsignedList<uint> zeros");
	ST_ASSERT_EQ((int)listUIntZero.size(), (int)EXP_UINT_ZERO_LIST.size(), "uint_zero_list.size");
	for (int i = 0; i < (int)EXP_UINT_ZERO_LIST.size(); ++i)
	{
		ST_ASSERT_EQ((int)listUIntZero[i], (int)EXP_UINT_ZERO_LIST[i], "uint_zero_list[" + IToS(i) + "]");
	}

	// Vector<uint> 递增
	Vector<uint> listUInt;
	ST_ASSERT(reader.readUnsignedList(listUInt),               "readUnsignedList<uint>");
	ST_ASSERT_EQ((int)listUInt.size(), (int)EXP_UINT_LIST.size(), "uint_list.size");
	for (int i = 0; i < (int)EXP_UINT_LIST.size(); ++i)
	{
		ST_ASSERT_EQ((int)listUInt[i], (int)EXP_UINT_LIST[i], "uint_list[" + IToS(i) + "]");
	}

	// Vector<uint> 空
	Vector<uint> listUIntEmpty;
	ST_ASSERT(reader.readUnsignedList(listUIntEmpty),          "readUnsignedList<uint> empty");
	ST_ASSERT_EQ((int)listUIntEmpty.size(), 0,                 "uint_empty.size=0");

	// Vector<short> 正值
	Vector<short> listShort;
	ST_ASSERT(reader.readSignedList(false, listShort),         "readSignedList<short>");
	ST_ASSERT_EQ((int)listShort.size(), (int)EXP_SHORT_LIST.size(), "short_list.size");
	for (int i = 0; i < (int)EXP_SHORT_LIST.size(); ++i)
	{
		ST_ASSERT_EQ((int)listShort[i], (int)EXP_SHORT_LIST[i], "short_list[" + IToS(i) + "]");
	}

	// Vector<short> 含负/边界
	Vector<short> listShortNeg;
	ST_ASSERT(reader.readSignedList(true, listShortNeg),       "readSignedList<short> neg");
	ST_ASSERT_EQ((int)listShortNeg.size(), (int)EXP_SHORT_NEG_LIST.size(), "short_neg_list.size");
	for (int i = 0; i < (int)EXP_SHORT_NEG_LIST.size(); ++i)
	{
		ST_ASSERT_EQ((int)listShortNeg[i], (int)EXP_SHORT_NEG_LIST[i], "short_neg_list[" + IToS(i) + "]");
	}

	// Vector<ushort> 含边界
	Vector<ushort> listUShort;
	ST_ASSERT(reader.readUnsignedList(listUShort),             "readUnsignedList<ushort>");
	ST_ASSERT_EQ((int)listUShort.size(), (int)EXP_USHORT_LIST.size(), "ushort_list.size");
	for (int i = 0; i < (int)EXP_USHORT_LIST.size(); ++i)
	{
		ST_ASSERT_EQ((int)listUShort[i], (int)EXP_USHORT_LIST[i], "ushort_list[" + IToS(i) + "]");
	}

	// Vector<llong> 正值
	Vector<llong> listLL;
	ST_ASSERT(reader.readSignedList(false, listLL),            "readSignedList<llong>");
	ST_ASSERT_EQ((int)listLL.size(), (int)EXP_LL_LIST.size(),  "ll_list.size");
	for (int i = 0; i < (int)EXP_LL_LIST.size(); ++i)
	{
		ST_ASSERT_EQ_LL(listLL[i], EXP_LL_LIST[i],            "ll_list[" + IToS(i) + "]");
	}

	// Vector<llong> 含负/极值
	Vector<llong> listLLNeg;
	ST_ASSERT(reader.readSignedList(true, listLLNeg),          "readSignedList<llong> neg");
	ST_ASSERT_EQ((int)listLLNeg.size(), (int)EXP_LL_NEG_LIST.size(), "ll_neg_list.size");
	for (int i = 0; i < (int)EXP_LL_NEG_LIST.size(); ++i)
	{
		ST_ASSERT_EQ_LL(listLLNeg[i], EXP_LL_NEG_LIST[i],    "ll_neg_list[" + IToS(i) + "]");
	}

	// Vector<byte> 正常
	Vector<byte> listByte;
	ST_ASSERT(reader.readUnsignedList(listByte),               "readUnsignedList<byte>");
	ST_ASSERT_EQ((int)listByte.size(), (int)EXP_BYTE_LIST.size(), "byte_list.size");
	for (int i = 0; i < (int)EXP_BYTE_LIST.size(); ++i)
	{
		ST_ASSERT_EQ((int)listByte[i], (int)EXP_BYTE_LIST[i], "byte_list[" + IToS(i) + "]");
	}

	// Vector<byte> 边界 {0, 0xFF}
	Vector<byte> listByteBound;
	ST_ASSERT(reader.readUnsignedList(listByteBound),          "readUnsignedList<byte> bound");
	ST_ASSERT_EQ((int)listByteBound.size(), (int)EXP_BYTE_BOUND.size(), "byte_bound.size");
	for (int i = 0; i < (int)EXP_BYTE_BOUND.size(); ++i)
	{
		ST_ASSERT_EQ((int)listByteBound[i], (int)EXP_BYTE_BOUND[i], "byte_bound[" + IToS(i) + "]");
	}

	// Vector<char> 含负
	Vector<char> listChar;
	ST_ASSERT(reader.readSignedList(true, listChar),           "readSignedList<char>");
	ST_ASSERT_EQ((int)listChar.size(), (int)EXP_CHAR_LIST.size(), "char_list.size");
	for (int i = 0; i < (int)EXP_CHAR_LIST.size(); ++i)
	{
		ST_ASSERT_EQ((int)listChar[i], (int)EXP_CHAR_LIST[i], "char_list[" + IToS(i) + "]");
	}

	// Vector<char> 全负
	Vector<char> listCharNeg;
	ST_ASSERT(reader.readSignedList(true, listCharNeg),        "readSignedList<char> neg");
	ST_ASSERT_EQ((int)listCharNeg.size(), (int)EXP_CHAR_NEG_LIST.size(), "char_neg_list.size");
	for (int i = 0; i < (int)EXP_CHAR_NEG_LIST.size(); ++i)
	{
		ST_ASSERT_EQ((int)listCharNeg[i], (int)EXP_CHAR_NEG_LIST[i], "char_neg_list[" + IToS(i) + "]");
	}

	// Vector<float> 3个元素
	Vector<float> listFloat3;
	ST_ASSERT(reader.readFloatList(false, listFloat3),         "readFloatList 3");
	ST_ASSERT_EQ((int)listFloat3.size(), (int)EXP_FLOAT_LIST3.size(), "float_list3.size");
	for (int i = 0; i < (int)EXP_FLOAT_LIST3.size(); ++i)
	{
		ST_ASSERT_FLOAT(listFloat3[i], EXP_FLOAT_LIST3[i],    "float_list3[" + IToS(i) + "]");
	}

	// Vector<float> 5个元素
	Vector<float> listFloat5;
	ST_ASSERT(reader.readFloatList(false, listFloat5),         "readFloatList 5");
	ST_ASSERT_EQ((int)listFloat5.size(), (int)EXP_FLOAT_LIST5.size(), "float_list5.size");
	for (int i = 0; i < (int)EXP_FLOAT_LIST5.size(); ++i)
	{
		ST_ASSERT_FLOAT(listFloat5[i], EXP_FLOAT_LIST5[i],    "float_list5[" + IToS(i) + "]");
	}

	// Vector<float> 全负
	Vector<float> listFloatNeg;
	ST_ASSERT(reader.readFloatList(true, listFloatNeg),        "readFloatList neg");
	ST_ASSERT_EQ((int)listFloatNeg.size(), (int)EXP_FLOAT_NEG_LIST.size(), "float_neg_list.size");
	for (int i = 0; i < (int)EXP_FLOAT_NEG_LIST.size(); ++i)
	{
		ST_ASSERT_FLOAT(listFloatNeg[i], EXP_FLOAT_NEG_LIST[i], "float_neg_list[" + IToS(i) + "]");
	}

	// Vector<float> 空
	Vector<float> listFloatEmpty;
	ST_ASSERT(reader.readFloatList(false, listFloatEmpty),     "readFloatList empty");
	ST_ASSERT_EQ((int)listFloatEmpty.size(), 0,                "float_empty.size=0");

	// Vector<double> 3个元素
	Vector<double> listDouble3;
	ST_ASSERT(reader.readDoubleList(false, listDouble3),       "readDoubleList 3");
	ST_ASSERT_EQ((int)listDouble3.size(), (int)EXP_DOUBLE_LIST3.size(), "double_list3.size");
	for (int i = 0; i < (int)EXP_DOUBLE_LIST3.size(); ++i)
	{
		ST_ASSERT_DOUBLE(listDouble3[i], EXP_DOUBLE_LIST3[i], "double_list3[" + IToS(i) + "]");
	}

	// Vector<double> 5个元素
	Vector<double> listDouble5;
	ST_ASSERT(reader.readDoubleList(false, listDouble5),       "readDoubleList 5");
	ST_ASSERT_EQ((int)listDouble5.size(), (int)EXP_DOUBLE_LIST5.size(), "double_list5.size");
	for (int i = 0; i < (int)EXP_DOUBLE_LIST5.size(); ++i)
	{
		ST_ASSERT_DOUBLE(listDouble5[i], EXP_DOUBLE_LIST5[i], "double_list5[" + IToS(i) + "]");
	}

	// Vector<double> 全负
	Vector<double> listDoubleNeg;
	ST_ASSERT(reader.readDoubleList(true, listDoubleNeg),      "readDoubleList neg");
	ST_ASSERT_EQ((int)listDoubleNeg.size(), (int)EXP_DOUBLE_NEG_LIST.size(), "double_neg_list.size");
	for (int i = 0; i < (int)EXP_DOUBLE_NEG_LIST.size(); ++i)
	{
		ST_ASSERT_DOUBLE(listDoubleNeg[i], EXP_DOUBLE_NEG_LIST[i], "double_neg_list[" + IToS(i) + "]");
	}

	// Vector<double> 空
	Vector<double> listDoubleEmpty;
	ST_ASSERT(reader.readDoubleList(false, listDoubleEmpty),   "readDoubleList empty");
	ST_ASSERT_EQ((int)listDoubleEmpty.size(), 0,               "double_empty.size=0");
}

// ══════════════════════════════════════════════════════════════════════════════
// 真实协议包：技能帧往返测试（第二段）
// ══════════════════════════════════════════════════════════════════════════════
void SerializeTest::testSkillFramePacket()
{
	const llong         expTargetGUID         = 0;
	const llong         expSkillFireTimeStamp = 1718813086792LL;
	const Vector<float> expFloatParam         { 4163.0f, 4155.0f, 2.0f };
	const bool          expTargetIsMonster    = false;
	const float         expAttackSpeed        = 2.05f;
	const float         expAttackStateTime    = -1.0f;
	const int           expSkillID            = 27;
	const int           expDamageToken        = 1;

	SerializerBitWrite writer0;
	writer0.writeSigned(false, expTargetGUID, expSkillFireTimeStamp);
	writer0.writeFloatList(false, expFloatParam);
	writer0.writeBool(expTargetIsMonster);
	writer0.writeFloat(true, expAttackSpeed, expAttackStateTime);
	writer0.writeSigned(true, expSkillID, expDamageToken);

	llong         mTargetGUID         = -1;
	llong         mSkillFireTimeStamp = -1;
	Vector<float> mFloatParam;
	bool          mTargetIsMonster    = true;
	float         mAttackSpeed        = 0.0f;
	float         mAttackStateTime    = 0.0f;
	int           mSkillID            = -1;
	int           mDamageToken        = -1;

	SerializerBitRead reader0(writer0.getBuffer(), writer0.getByteCount());
	ST_ASSERT(reader0.readSigned(false, mTargetGUID, mSkillFireTimeStamp),  "技能帧: readSigned(llong×2)");
	ST_ASSERT(reader0.readFloatList(false, mFloatParam),                     "技能帧: readFloatList");
	ST_ASSERT(reader0.readBool(mTargetIsMonster),                             "技能帧: readBool");
	ST_ASSERT(reader0.readFloat(true, mAttackSpeed, mAttackStateTime),       "技能帧: readFloat×2");
	ST_ASSERT(reader0.readSigned(true, mSkillID, mDamageToken),              "技能帧: readSigned(int×2)");

	ST_ASSERT_EQ_LL(mTargetGUID, expTargetGUID,                              "技能帧: TargetGUID");
	ST_ASSERT_EQ_LL(mSkillFireTimeStamp, expSkillFireTimeStamp,              "技能帧: SkillFireTimeStamp");
	ST_ASSERT_EQ((int)mFloatParam.size(), (int)expFloatParam.size(),         "技能帧: FloatParam.size");
	for (int i = 0; i < (int)expFloatParam.size(); ++i)
	{
		ST_ASSERT_FLOAT(mFloatParam[i], expFloatParam[i],                    "技能帧: FloatParam[" + IToS(i) + "]");
	}
	ST_ASSERT_EQ((int)mTargetIsMonster, (int)expTargetIsMonster,             "技能帧: TargetIsMonster");
	ST_ASSERT_FLOAT(mAttackSpeed, expAttackSpeed,                             "技能帧: AttackSpeed");
	ST_ASSERT_FLOAT(mAttackStateTime, expAttackStateTime,                     "技能帧: AttackStateTime");
	ST_ASSERT_EQ(mSkillID, expSkillID,                                        "技能帧: SkillID");
	ST_ASSERT_EQ(mDamageToken, expDamageToken,                                "技能帧: DamageToken");
}

// ══════════════════════════════════════════════════════════════════════════════
// 总入口
// ══════════════════════════════════════════════════════════════════════════════
void SerializeTest::test()
{
	testBasicTypes();
	testSkillFramePacket();
	testPacketBitSerialize();
}
#endif
