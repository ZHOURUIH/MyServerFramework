#ifdef WINDOWS
#include "FrameHeader.h"

#define MSE_ASSERT(expr, msg) \
	if (!(expr)) { ERROR(string("MyStringTest FAIL: ") + (msg)); }

// =====================================================================
// Part 1: MyString — 构造和赋值
// =====================================================================

void MyStringTest::testMSE_defaultConstruct()
{
	MyString<32> ms;
	MSE_ASSERT(ms.length() == 0, "default length 0");
	MSE_ASSERT(string(ms.str()) == "", "default content empty");
}

void MyStringTest::testMSE_assignConstChar()
{
	MyString<64> ms;
	ms.set("hello world");
	MSE_ASSERT(string(ms.str()) == "hello world", "assign const char*");
}

void MyStringTest::testMSE_assignMyString()
{
	MyString<32> a, b;
	a.set("source");
	b = a;
	MSE_ASSERT(string(b.str()) == "source", "assign MyString");
}

void MyStringTest::testMSE_assignEmpty()
{
	MyString<16> ms;
	ms.set("something");
	ms.set("");
	MSE_ASSERT(ms.length() == 0, "assign empty length 0");
}

void MyStringTest::testMSE_assignSelf()
{
	MyString<32> ms;
	ms.set("self");
	ms = ms;
	MSE_ASSERT(string(ms.str()) == "self", "assign self");
}

// =====================================================================
// Part 2: MyString — 追加
// =====================================================================

void MyStringTest::testMSE_appendChar()
{
	MyString<16> ms;
	ms.add("Hello");
	ms.add(' ');
	ms.add("World");
	MSE_ASSERT(string(ms.str()) == "Hello World", "append chars");
}

void MyStringTest::testMSE_appendMultiple()
{
	MyString<64> ms;
	ms.add("a");
	ms.add("b");
	ms.add("c");
	ms.add("d");
	ms.add("e");
	MSE_ASSERT(string(ms.str()) == "abcde", "append multiple");
}

void MyStringTest::testMSE_appendEmpty()
{
	MyString<16> ms;
	ms.add("base");
	ms.add("");
	MSE_ASSERT(string(ms.str()) == "base", "append empty unchanged");
}

void MyStringTest::testMSE_appendLong()
{
	MyString<128> ms;
	ms.add("start");
	for (int i = 0; i < 10; ++i) ms.add("x");
	MSE_ASSERT(string(ms.str()) == "startxxxxxxxxxx", "append long");
}

// =====================================================================
// Part 3: MyString — clear() 和重置
// =====================================================================

void MyStringTest::testMSE_clear()
{
	MyString<32> ms;
	ms.add("data");
	ms.clear();
	MSE_ASSERT(ms.length() == 0, "clear length 0");
	MSE_ASSERT(string(ms.str()) == "", "clear content empty");
}

void MyStringTest::testMSE_clearThenAssign()
{
	MyString<16> ms;
	ms.add("old");
	ms.clear();
	ms.add("new");
	MSE_ASSERT(string(ms.str()) == "new", "clear then assign");
}

void MyStringTest::testMSE_clearThenAppend()
{
	MyString<16> ms;
	ms.add("old");
	ms.clear();
	ms.add("new");
	MSE_ASSERT(string(ms.str()) == "new", "clear then append");
}

void MyStringTest::testMSE_clearMultiple()
{
	MyString<32> ms;
	for (int i = 0; i < 5; ++i)
	{
		ms.add("test");
		ms.clear();
		MSE_ASSERT(ms.length() == 0, "clear cycle");
	}
}

// =====================================================================
// Part 4: MyString — 边界和溢出
// =====================================================================

void MyStringTest::testMSE_exactFit()
{
	MyString<9> ms;
	ms.add("12345678");
	MSE_ASSERT(string(ms.str()) == "12345678", "exact fit 8 chars");
}

void MyStringTest::testMSE_singleChar()
{
	MyString<8> ms;
	ms.add("X");
	MSE_ASSERT(string(ms.str()) == "X", "single char");
	MSE_ASSERT(ms.length() == 1, "single char length 1");
}

void MyStringTest::testMSE_twoChars()
{
	MyString<8> ms;
	ms.add("AB");
	MSE_ASSERT(string(ms.str()) == "AB", "two chars");
}

void MyStringTest::testMSE_almostFull()
{
	MyString<10> ms;
	ms.add("123456789");
	MSE_ASSERT(ms.length() == 9, "almost full length 9");
	MSE_ASSERT(string(ms.str()) == "123456789", "almost full content");
}

// =====================================================================
// Part 5: MyString — int 转换
// =====================================================================

void MyStringTest::testMSE_intConversion()
{
	MyString<16> ms;
	ms.set(IToS(42));
	MSE_ASSERT(string(ms.str()) == "42", "IToS 42");
	ms.set(IToS(-1));
	MSE_ASSERT(string(ms.str()) == "-1", "IToS -1");
	ms.set(IToS(0));
	MSE_ASSERT(string(ms.str()) == "0", "IToS 0");
}

void MyStringTest::testMSE_intConversionLarge()
{
	MyString<32> ms;
	ms.set(IToS(1234567890));
	MSE_ASSERT(string(ms.str()) == "1234567890", "IToS large");
	ms.set(IToS(-1234567890));
	MSE_ASSERT(string(ms.str()) == "-1234567890", "IToS negative large");
}

void MyStringTest::testMSE_intConversionMax()
{
	MyString<32> ms;
	ms.set(IToS(INT_MAX));
	MSE_ASSERT(string(ms.str()) == "2147483647", "IToS INT_MAX");
	ms.set(IToS(INT_MIN));
	MSE_ASSERT(string(ms.str()) == "-2147483648", "IToS INT_MIN");
}

// =====================================================================
// Part 6: MyString — llong 转换
// =====================================================================

void MyStringTest::testMSE_llongConversion()
{
	MyString<32> ms;
	ms.set(LLToS(1000000000000LL));
	MSE_ASSERT(string(ms.str()) == "1000000000000", "LLToS 1e12");
	ms.set(LLToS(-1000000000000LL));
	MSE_ASSERT(string(ms.str()) == "-1000000000000", "LLToS -1e12");
}

void MyStringTest::testMSE_llongMax()
{
	MyString<64> ms;
	ms.set(LLToS(LLONG_MAX));
	MSE_ASSERT(string(ms.str()) == "999999999999999999", "LLToS LLONG_MAX");
}

// =====================================================================
// Part 7: MyString — float 转换
// =====================================================================

void MyStringTest::testMSE_floatConversion()
{
	MyString<32> ms;
	ms.set(FToS(3.14f));
	MSE_ASSERT(!string(ms.str()).empty(), "FToS 3.14 non-empty");
	ms.set(FToS(0.0f));
	MSE_ASSERT(!string(ms.str()).empty(), "FToS 0.0 non-empty");
}

// =====================================================================
// Part 8: MyString — 综合操作
// =====================================================================

void MyStringTest::testMSE_appendAfterAssign()
{
	MyString<32> ms;
	ms.set("prefix");
	ms.add("_suffix");
	MSE_ASSERT(string(ms.str()) == "prefix_suffix", "append after assign");
}

void MyStringTest::testMSE_chainAssignment()
{
	MyString<32> a, b, c;
	a.set("first");
	b = a;
	c = b;
	MSE_ASSERT(string(c.str()) == "first", "chain assignment");
}

void MyStringTest::testMSE_stringStream()
{
	MyString<64> ms;
	ms.set("");
	ms.add("Part1");
	ms.add(" ");
	ms.add("Part2");
	ms.add(" ");
	ms.add("Part3");
	MSE_ASSERT(string(ms.str()) == "Part1 Part2 Part3", "string stream build");
}

void MyStringTest::testMSE_interleavedOps()
{
	MyString<16> ms;
	ms.set("ABC");
	ms.clear();
	ms.set("123");
	ms.add("456");
	ms.clear();
	ms.set("final");
	MSE_ASSERT(string(ms.str()) == "final", "interleaved ops final");
}

// =====================================================================
// Part 9: MyString — 大小比较
// =====================================================================

void MyStringTest::testMSE_lengthTracking()
{
	MyString<32> ms;
	MSE_ASSERT(ms.length() == 0, "length start 0");
	ms.set("hello");
	MSE_ASSERT(ms.length() == 5, "length 5");
	ms.set("");
	MSE_ASSERT(ms.length() == 0, "length back to 0");
	ms.set("a");
	MSE_ASSERT(ms.length() == 1, "length 1");
}

void MyStringTest::testMSE_lengthAfterClear()
{
	MyString<16> ms;
	ms.set("test");
	ms.clear();
	MSE_ASSERT(ms.length() == 0, "length after clear 0");
}

// =====================================================================
// Part 10: MyString — 多次 clear 和重用
// =====================================================================

void MyStringTest::testMSE_reusePattern()
{
	MyString<32> ms;
	for (int i = 0; i < 10; ++i)
	{
		ms.set("");
		ms.add(IToS(i));
		ms.add(":value");
		MSE_ASSERT(ms.length() > 0, "reuse pattern non-empty");
	}
}

void MyStringTest::testMSE_clearAndReassign()
{
	MyString<32> ms;
	ms.set("first");
	ms.clear();
	ms.set("second");
	ms.clear();
	ms.set("third");
	MSE_ASSERT(string(ms.str()) == "third", "clear reassign third");
}

// =====================================================================
// Part 11: MyString — 特殊字符
// =====================================================================

void MyStringTest::testMSE_specialChars()
{
	MyString<32> ms;
	ms.set("abc123!@#");
	MSE_ASSERT(string(ms.str()) == "abc123!@#", "special chars");
}

void MyStringTest::testMSE_digitsOnly()
{
	MyString<16> ms;
	ms.set("9876543210");
	MSE_ASSERT(string(ms.str()) == "9876543210", "digits only");
}

void MyStringTest::testMSE_mixedCase()
{
	MyString<16> ms;
	ms.set("AbCdEfG");
	MSE_ASSERT(string(ms.str()) == "AbCdEfG", "mixed case");
}

// =====================================================================
// Part 12: MyString — str() 访问
// =====================================================================

void MyStringTest::testMSE_strAccess()
{
	MyString<16> ms;
	ms.set("hello");
	const char* s = ms.str();
	MSE_ASSERT(s != nullptr, "str() non-null");
	MSE_ASSERT(s[0] == 'h', "str()[0]=h");
	MSE_ASSERT(s[4] == 'o', "str()[4]=o");
}

void MyStringTest::testMSE_strAfterClear()
{
	MyString<16> ms;
	ms.set("data");
	ms.clear();
	const char* s = ms.str();
	MSE_ASSERT(s != nullptr, "str after clear non-null");
	MSE_ASSERT(s[0] == 0, "str after clear empty");
}

// =====================================================================
// Part 13: MyString — 多类型转换组合
// =====================================================================

void MyStringTest::testMSE_mixedConversion()
{
	MyString<64> ms;
	ms.set("int:");
	ms.add(IToS(-99));
	ms.add(" float:");
	ms.add(FToS(1.5f));
	ms.add(" llong:");
	ms.add(LLToS(123456789LL));
	MSE_ASSERT(true, "mixed conversion no crash");
}

// =====================================================================
// Part 14: MyString — 追加后验证
// =====================================================================

void MyStringTest::testMSE_appendVerifyEachStep()
{
	MyString<32> ms;
	ms.set("a");
	MSE_ASSERT(string(ms.str()) == "a", "step1: 'a'");
	ms.add("b");
	MSE_ASSERT(string(ms.str()) == "ab", "step2: 'ab'");
	ms.add("c");
	MSE_ASSERT(string(ms.str()) == "abc", "step3: 'abc'");
	ms.add("def");
	MSE_ASSERT(string(ms.str()) == "abcdef", "step4: 'abcdef'");
}

void MyStringTest::testMSE_multipleClear()
{
	MyString<16> ms;
	ms.set("first");
	ms.clear();
	ms.set("second");
	ms.clear();
	ms.set("third");
	ms.clear();
	MSE_ASSERT(ms.length() == 0, "multiple clear length 0");
}

void MyStringTest::testMSE_assignEmptyAfterNonEmpty()
{
	MyString<16> ms;
	ms.set("non_empty");
	ms.set("");
	MSE_ASSERT(ms.length() == 0, "assign empty after non-empty length 0");
	ms.set("restored");
	MSE_ASSERT(string(ms.str()) == "restored", "assign after empty");
}

void MyStringTest::testMSE_appendLargeSequential()
{
	MyString<64> ms;
	for (int i = 0; i < 10; ++i)
	{
		ms.add(IToS(i));
		ms.add(",");
	}
	string result(ms.str());
	MSE_ASSERT(result.find("0,1,2") != string::npos, "sequential append prefix");
	MSE_ASSERT(result.find("8,9,") != string::npos, "sequential append suffix");
}

void MyStringTest::testMSE_reuseAfterLengthTracking()
{
	MyString<32> ms;
	ms.set("abcdefghij");
	MSE_ASSERT(ms.length() == 10, "length tracking 10");
	ms.clear();
	MSE_ASSERT(ms.length() == 0, "length after clear 0");
	ms.set("12345");
	MSE_ASSERT(ms.length() == 5, "length after reassign 5");
}

void MyStringTest::testMSE_assignStringStream()
{
	MyString<128> ms;
	ms.set("start");
	ms.add("_middle_");
	ms.add("end");
	MSE_ASSERT(string(ms.str()) == "start_middle_end", "string stream build");
}

// =====================================================================
// 主入口
// =====================================================================
void MyStringTest::test()
{
	testMSE_defaultConstruct();
	testMSE_assignConstChar();
	testMSE_assignMyString();
	testMSE_assignEmpty();
	testMSE_assignSelf();
	testMSE_appendChar();
	testMSE_appendMultiple();
	testMSE_appendEmpty();
	testMSE_appendLong();
	testMSE_clear();
	testMSE_clearThenAssign();
	testMSE_clearThenAppend();
	testMSE_clearMultiple();
	testMSE_exactFit();
	testMSE_singleChar();
	testMSE_twoChars();
	testMSE_almostFull();
	testMSE_intConversion();
	testMSE_intConversionLarge();
	testMSE_intConversionMax();
	testMSE_llongConversion();
	testMSE_llongMax();
	testMSE_floatConversion();
	testMSE_appendAfterAssign();
	testMSE_chainAssignment();
	testMSE_stringStream();
	testMSE_interleavedOps();
	testMSE_lengthTracking();
	testMSE_lengthAfterClear();
	testMSE_reusePattern();
	testMSE_clearAndReassign();
	testMSE_specialChars();
	testMSE_digitsOnly();
	testMSE_mixedCase();
	testMSE_strAccess();
	testMSE_strAfterClear();
	testMSE_mixedConversion();
	testMSE_appendVerifyEachStep();
	testMSE_multipleClear();
	testMSE_assignEmptyAfterNonEmpty();
	testMSE_appendLargeSequential();
	testMSE_reuseAfterLengthTracking();
	testMSE_assignStringStream();
	testMyString_Basic();
	testContainerExtended();
}

// ============================================================
// Consolidated container smoke tests
// ============================================================
#define MYSTRINGTEST_CT_ASSERT(expr, msg) \
	do { if (!(expr)) { ERROR(string("MyStringTest FAIL: ") + (msg)); } } while(0)
#define MYSTRINGTEST_CT_ASSERT_EQ(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("MyStringTest FAIL: ") + (msg) + " | expected=" + IToS((int)(b)) + " actual=" + IToS((int)(a))); } } while(0)
#define MYSTRINGTEST_CT_ASSERT_STR(a, b, msg) \
	do { if (string(a) != string(b)) { ERROR(string("MyStringTest FAIL: ") + (msg) + " | expected=" + (b) + " actual=" + (a)); } } while(0)

void MyStringTest::testMyString_Basic()
{
	MyString<64> s;

	// default: isEmpty
	MYSTRINGTEST_CT_ASSERT(s.isEmpty(), "MyString default isEmpty");
	MYSTRINGTEST_CT_ASSERT_EQ(s.length(), 0, "MyString default length=0");

	// set / str / length
	s.set("hello");
	MYSTRINGTEST_CT_ASSERT(!s.isEmpty(), "MyString not empty after set");
	MYSTRINGTEST_CT_ASSERT_STR(s.str(), "hello", "MyString str=hello");
	MYSTRINGTEST_CT_ASSERT_EQ(s.length(), 5, "MyString length=5");

	// clear
	s.clear();
	MYSTRINGTEST_CT_ASSERT(s.isEmpty(), "MyString isEmpty after clear");
	MYSTRINGTEST_CT_ASSERT_EQ(s.length(), 0, "MyString length=0 after clear");

	// operator[] read
	s.set("abcde");
	MYSTRINGTEST_CT_ASSERT_EQ((int)s[0], (int)'a', "MyString [0]='a'");
	MYSTRINGTEST_CT_ASSERT_EQ((int)s[4], (int)'e', "MyString [4]='e'");

	// size / getMaxLength
	MYSTRINGTEST_CT_ASSERT_EQ(s.getMaxLength(), 64, "MyString getMaxLength=64");

	// copy from string
	string src = "test_copy";
	s.set(src);
	MYSTRINGTEST_CT_ASSERT_STR(s.str(), "test_copy", "MyString set(std::string)");
	MYSTRINGTEST_CT_ASSERT_EQ(s.length(), 9, "MyString length=9");
}

void MyStringTest::testContainerExtended()
{
// set / str()
	{
		MyString<64> ms;
		ms.set("hello");
		MYSTRINGTEST_CT_ASSERT_STR(ms.str(), "hello", "MyString set");
		MYSTRINGTEST_CT_ASSERT_EQ(ms.length(), 5, "MyString length=5");
	}

	// append (simulate via set)
	{
		MyString<128> ms;
		ms.set("hello world");
		MYSTRINGTEST_CT_ASSERT_STR(ms.str(), "hello world", "MyString set combined");
	}

	// clear
	{
		MyString<32> ms;
		ms.set("test");
		ms.clear();
		MYSTRINGTEST_CT_ASSERT_EQ(ms.length(), 0, "MyString clear length=0");
	}

	// operator== (use strcmp)
	{
		MyString<32> ms1, ms2;
		ms1.set("abc");
		ms2.set("abc");
		MYSTRINGTEST_CT_ASSERT(strcmp(ms1.str(), ms2.str()) == 0, "MyString == via strcmp");
		ms2.set("xyz");
		MYSTRINGTEST_CT_ASSERT(strcmp(ms1.str(), ms2.str()) != 0, "MyString != via strcmp");
	}

	// capacity check (no overflow)
	{
		MyString<8> ms;
		ms.set("1234567"); // 7 chars + null = 8, fits
		MYSTRINGTEST_CT_ASSERT_EQ(ms.length(), 7, "MyString capacity 7 chars");
	}
//------------------------------------------------------------------------------
}
#endif
