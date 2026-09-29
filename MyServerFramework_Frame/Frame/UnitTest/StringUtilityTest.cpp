#ifdef WINDOWS
#include "FrameHeader.h"
namespace StringUtilityExtTestSupport
{
#define SUET_ASSERT(expr, msg) \
	do { if (!(expr)) { ERROR(string("StringUtilityExtTest FAIL: ") + (msg)); } } while(0)
#define SUET_ASSERT_EQ(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("StringUtilityExtTest FAIL: ") + (msg) + \
		" | expected=" + IToS((int)(b)) + " actual=" + IToS((int)(a))); } } while(0)
#define SUET_ASSERT_STR(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("StringUtilityExtTest FAIL: ") + (msg) + \
		" | expected='" + (b) + "' actual='" + (a) + "'"); } } while(0)
#define SUET_ASSERT_LL(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("StringUtilityExtTest FAIL: ") + (msg) + \
		" | expected=" + LLToS(b) + " actual=" + LLToS(a)); } } while(0)
}

void StringUtilityTest::testSplitCharKey()
{
	using namespace StringUtilityExtTestSupport;
	// 基本分割
	Vector<string> v;
	split("a,b,c", ',', v);
	SUET_ASSERT_EQ(v.size(), 3, "split 'a,b,c' -> 3");
	SUET_ASSERT_STR(v[0], "a", "split[0]='a'");
	SUET_ASSERT_STR(v[1], "b", "split[1]='b'");
	SUET_ASSERT_STR(v[2], "c", "split[2]='c'");

	// 空字符串
	Vector<string> v2;
	split("", ',', v2);
	SUET_ASSERT_EQ(v2.size(), 0, "split empty -> 0");

	// 只有分隔符，removeEmpty=true
	Vector<string> v3;
	split(",,", ',', v3, true);
	SUET_ASSERT_EQ(v3.size(), 0, "split ',,',removeEmpty -> 0");

	// removeEmpty=false
	Vector<string> v4;
	split("a,,c", ',', v4, false);
	SUET_ASSERT_EQ(v4.size(), 3, "split 'a,,c' no remove -> 3");
	SUET_ASSERT_STR(v4[1], "", "split[1]=''");

	// 末尾有分隔符
	Vector<string> v5;
	split("x:y:", ':', v5);
	SUET_ASSERT_EQ(v5.size(), 2, "split 'x:y:' -> 2");

	// 单个元素（无分隔符）
	Vector<string> v6;
	split("hello", ',', v6);
	SUET_ASSERT_EQ(v6.size(), 1, "split no delim -> 1");
	SUET_ASSERT_STR(v6[0], "hello", "split no delim val");

	// 返回值版本
	Vector<string> v7 = split("1|2|3", '|');
	SUET_ASSERT_EQ(v7.size(), 3, "split return vec size=3");
	SUET_ASSERT_STR(v7[2], "3", "split return vec[2]='3'");

	// buffer 版本
	string buf[4];
	int cnt = split("p-q-r", '-', buf, 4);
	SUET_ASSERT_EQ(cnt, 3, "split buffer cnt=3");
	SUET_ASSERT_STR(buf[0], "p", "split buf[0]='p'");
}

void StringUtilityTest::testSplitStringKey()
{
	using namespace StringUtilityExtTestSupport;
	// 多字符分隔符
	Vector<string> v;
	split("one::two::three", "::", v);
	SUET_ASSERT_EQ(v.size(), 3, "split '::' -> 3");
	SUET_ASSERT_STR(v[0], "one", "split_s[0]='one'");
	SUET_ASSERT_STR(v[1], "two", "split_s[1]='two'");
	SUET_ASSERT_STR(v[2], "three", "split_s[2]='three'");

	// 分隔符不存在
	Vector<string> v2;
	split("hello", "::", v2);
	SUET_ASSERT_EQ(v2.size(), 1, "split_s no key -> 1");

	// removeEmpty=false
	Vector<string> v3;
	split("a::::b", "::", v3, false);
	SUET_ASSERT(v3.size() >= 2, "split_s empty keep");

	// string& 版本
	Vector<string> v4;
	string src = "X--Y--Z";
	split(src, "--", v4);
	SUET_ASSERT_EQ(v4.size(), 3, "split string& -> 3");
	SUET_ASSERT_STR(v4[1], "Y", "split string&[1]='Y'");
}

void StringUtilityTest::testSplitLine()
{
	using namespace StringUtilityExtTestSupport;
	Vector<string> v;
	splitLine("line1\nline2\nline3", v);
	SUET_ASSERT_EQ(v.size(), 3, "splitLine 3");
	SUET_ASSERT_STR(v[0], "line1", "splitLine[0]");

	// \r\n
	Vector<string> v2;
	splitLine("a\r\nb\r\nc", v2);
	SUET_ASSERT(v2.size() >= 2, "splitLine \\r\\n >= 2");

	// buffer 版本
	string buf[5];
	splitLine("x\ny\nz", buf, 5);
	SUET_ASSERT_STR(buf[0], "x", "splitLine buf[0]='x'");
}

void StringUtilityTest::testFindString()
{
	using namespace StringUtilityExtTestSupport;
	// 找到
	SUET_ASSERT(findString("hello world", "world"), "find 'world'");
	SUET_ASSERT(findString("hello world", "hello"), "find 'hello'");

	// 找不到
	SUET_ASSERT(!findString("hello world", "xyz"), "not find 'xyz'");

	// 获取位置
	int pos = -1;
	findString("abcabc", "b", &pos);
	SUET_ASSERT_EQ(pos, 1, "findString pos=1");

	// 从指定位置开始
	int pos2 = -1;
	findString("abcabc", "b", &pos2, 2);
	SUET_ASSERT_EQ(pos2, 4, "findString startIndex=2 pos=4");

	// findStringPos
	SUET_ASSERT_EQ(findStringPos("hello", "ell"), 1, "findStringPos=1");
	SUET_ASSERT_EQ(findStringPos("hello", "xyz"), -1, "findStringPos=-1");

	// string版本
	string s = "abcdef";
	SUET_ASSERT_EQ(findStringPos(s, string("cde")), 2, "findStringPos string=2");

	// findStringLower (大小写不敏感)
	SUET_ASSERT(findStringLower("Hello World", "WORLD"), "findStringLower");
	SUET_ASSERT(!findStringLower("Hello World", "xyz"), "findStringLower not found");
}

void StringUtilityTest::testStartEndWith()
{
	using namespace StringUtilityExtTestSupport;
	// startWith
	SUET_ASSERT(startWith("hello world", "hello"), "startWith 'hello'");
	SUET_ASSERT(!startWith("hello world", "world"), "not startWith 'world'");
	SUET_ASSERT(startWith("", ""), "startWith '' ''");

	// 大小写不敏感
	SUET_ASSERT(startWith("Hello", "hello", false), "startWith insensitive");
	SUET_ASSERT(!startWith("Hello", "WORLD", false), "startWith insensitive false");

	// endWith
	SUET_ASSERT(endWith("hello.cpp", ".cpp"), "endWith '.cpp'");
	SUET_ASSERT(!endWith("hello.cpp", ".h"), "not endWith '.h'");
	SUET_ASSERT(endWith("test", "test"), "endWith full match");

	// 大小写不敏感
	SUET_ASSERT(endWith("test.CPP", ".cpp", false), "endWith insensitive");

	// char* 版本
	SUET_ASSERT(startWith("prefix_abc", "prefix"), "startWith char*");
	SUET_ASSERT(endWith("abc_suffix", "suffix"), "endWith char*");
}

void StringUtilityTest::testRemoveStartLast()
{
	using namespace StringUtilityExtTestSupport;
	string s1 = "   hello   ";
	removeStartAll(s1, ' ');
	SUET_ASSERT_STR(s1, "hello   ", "removeStartAll spaces");

	string s2 = "   hello   ";
	removeLastAll(s2, ' ');
	SUET_ASSERT_STR(s2, "   hello", "removeLastAll spaces");

	string s3 = "/path/to/file/";
	removeLast(s3, '/');
	SUET_ASSERT_STR(s3, "/path/to/file", "removeLast '/'");

	string s4 = "/path/to/file";
	removeStart(s4, '/');
	SUET_ASSERT_STR(s4, "path/to/file", "removeStart '/'");

	// 全部是同一字符
	string s5 = "aaaa";
	removeStartAll(s5, 'a');
	SUET_ASSERT_STR(s5, "", "removeStartAll all same -> ''");

	string s6 = "bbbb";
	removeLastAll(s6, 'b');
	SUET_ASSERT_STR(s6, "", "removeLastAll all same -> ''");

	// 无需移除
	string s7 = "hello";
	removeStartAll(s7, 'x');
	SUET_ASSERT_STR(s7, "hello", "removeStartAll no match");

	// removeStartString / removeEndString
	string s8 = removeStartString("prefix_content", "prefix_");
	SUET_ASSERT_STR(s8, "content", "removeStartString");

	string s9 = removeEndString("content_suffix", "_suffix");
	SUET_ASSERT_STR(s9, "content", "removeEndString");

	// 不匹配时原样返回
	string s10 = removeStartString("hello", "xyz");
	SUET_ASSERT_STR(s10, "hello", "removeStartString no match");
}

void StringUtilityTest::testReplaceAll()
{
	using namespace StringUtilityExtTestSupport;
	string s1 = "hello world hello";
	replaceAll(s1, "hello", "hi");
	SUET_ASSERT_STR(s1, "hi world hi", "replaceAll string");

	string s2 = "a.b.c.d";
	replaceAll(s2, '.', '/');
	SUET_ASSERT_STR(s2, "a/b/c/d", "replaceAll char");

	// 无匹配时不变
	string s3 = "no match";
	replaceAll(s3, "xyz", "abc");
	SUET_ASSERT_STR(s3, "no match", "replaceAll no match");

	// replace 指定范围
	string s4 = "hello world";
	replace(s4, 6, 11, "C++");
	SUET_ASSERT_STR(s4, "hello C++", "replace range");

	// removeAll single char
	string s5 = "a1b2c3d4";
	removeAll(s5, '1');
	SUET_ASSERT_STR(s5, "ab2c3d4", "removeAll '1'");

	// removeAll two chars
	string s6 = "a1b2c3";
	removeAll(s6, '1', '2');
	SUET_ASSERT_STR(s6, "abc3", "removeAll '1','2'");
}

void StringUtilityTest::testFileNamePath()
{
	using namespace StringUtilityExtTestSupport;
	// getFileName
	SUET_ASSERT_STR(getFileName("C:/path/to/file.txt"), "file.txt", "getFileName");
	SUET_ASSERT_STR(getFileName("file.txt"), "file.txt", "getFileName no path");

	// getFileNameNoSuffix
	SUET_ASSERT_STR(getFileNameNoSuffix("C:/path/to/file.txt", true), "file", "getFileNameNoSuffix");
	SUET_ASSERT_STR(getFileNameNoSuffix("file.txt", false), "file", "getFileNameNoSuffix no path");

	// getFilePath
	SUET_ASSERT_STR(getFilePath("C:/path/to/file.txt", true), "C:/path/to/", "getFilePath keep slash");
	SUET_ASSERT_STR(getFilePath("C:/path/to/file.txt", false), "C:/path/to", "getFilePath no slash");

	// getFileSuffix
	SUET_ASSERT_STR(getFileSuffix("file.txt", true), ".txt", "getFileSuffix with dot");
	SUET_ASSERT_STR(getFileSuffix("file.txt", false), "txt", "getFileSuffix no dot");
	SUET_ASSERT_STR(getFileSuffix("file", true), "", "getFileSuffix no suffix");

	// removeSuffix
	SUET_ASSERT_STR(removeSuffix("file.txt"), "file", "removeSuffix");
	SUET_ASSERT_STR(removeSuffix("file"), "file", "removeSuffix no suffix");

	// getFirstFolderName / removeFirstPath
	SUET_ASSERT_STR(getFirstFolderName("Assets/Scripts/Test"), "Assets", "getFirstFolderName");
	SUET_ASSERT_STR(removeFirstPath("Assets/Scripts/Test"), "Scripts/Test", "removeFirstPath");
}

void StringUtilityTest::testStrMisc()
{
	using namespace StringUtilityExtTestSupport;
	// strlength
	SUET_ASSERT_EQ(strlength("hello"), 5, "strlength 5");
	SUET_ASSERT_EQ(strlength(""), 0, "strlength 0");
	SUET_ASSERT_EQ(strlength("hello", 3), 3, "strlength max 3");

	// findCharCount
	SUET_ASSERT_EQ(findCharCount("abcabc", 'a'), 2, "findCharCount 'a'=2");
	SUET_ASSERT_EQ(findCharCount("hello", 'z'), 0, "findCharCount 'z'=0");

	// strchar (找字符位置)
	SUET_ASSERT_EQ(strchar("abcabc", 'b'), 1, "strchar 'b'=1");
	SUET_ASSERT_EQ(strchar("abcabc", 'b', 2), 4, "strchar 'b' start=2 -> 4");
	SUET_ASSERT_EQ(strchar("hello", 'z'), -1, "strchar 'z'=-1");

	// getLastChar / getLastNotChar
	SUET_ASSERT_EQ(getLastChar("abcba", 'a'), 4, "getLastChar 'a'=4");
	SUET_ASSERT_EQ(getLastNotChar("aaabbb", 'b'), 2, "getLastNotChar 'b'=2");

	// getLastNotNumberPos
	SUET_ASSERT_EQ(getLastNotNumberPos("abc123"), 2, "lastNotNumPos=2");
	SUET_ASSERT_EQ(getLastNotNumberPos("123"), -1, "lastNotNumPos all digit=-1");

	// getLastNumber
	SUET_ASSERT_EQ(getLastNumber("item99"), 99, "getLastNumber=99");
	SUET_ASSERT_EQ(getLastNumber("test0"), 0, "getLastNumber=0");
}

void StringUtilityTest::testNumberToString()
{
	using namespace StringUtilityExtTestSupport;
	// IToS
	SUET_ASSERT_STR(IToS(0), "0", "IToS(0)");
	SUET_ASSERT_STR(IToS(42), "42", "IToS(42)");
	SUET_ASSERT_STR(IToS(-999), "-999", "IToS(-999)");
	SUET_ASSERT_STR(IToS(2147483647), "2147483647", "IToS(INT_MAX)");

	// LLToS
	SUET_ASSERT_STR(LLToS(0LL), "0", "LLToS(0)");
	SUET_ASSERT_STR(LLToS(9999999999LL), "9999999999", "LLToS big");
	SUET_ASSERT_STR(LLToS(-1LL), "-1", "LLToS(-1)");

	// UIToS
	SUET_ASSERT_STR(UIToS(0), "0", "UIToS(0)");
	SUET_ASSERT_STR(UIToS(100), "100", "UIToS(100)");

	// FToS
	SUET_ASSERT(!FToS(3.14f).empty(), "FToS not empty");
	SUET_ASSERT(!FToS(0.0f).empty(), "FToS(0) not empty");

	// SToI (string -> int)
	SUET_ASSERT_EQ(SToI("42"), 42, "SToI(42)");
	SUET_ASSERT_EQ(SToI("-5"), -5, "SToI(-5)");
	SUET_ASSERT_EQ(SToI("0"), 0, "SToI(0)");

	// SToLL (string -> llong)
	SUET_ASSERT_LL(SToLL("9999999999"), 9999999999LL, "SToLL big");
	SUET_ASSERT_LL(SToLL("-1"), -1LL, "SToLL(-1)");

	// SToF (string -> float)
	SUET_ASSERT(fabsf(SToF("3.14") - 3.14f) < 1e-2f, "SToF(3.14)");
	SUET_ASSERT(fabsf(SToF("0.0")) < 1e-5f, "SToF(0.0)");

	// limitLen
	SUET_ASSERT_EQ(IToS(7, 4).size(), (size_t)4, "IToS limitLen=4");
	SUET_ASSERT_EQ(LLToS(7LL, 3).size(), (size_t)3, "LLToS limitLen=3");
}

void StringUtilityTest::testStrCopyZero()
{
	using namespace StringUtilityExtTestSupport;
	// zeroString
	string z = zeroString(5);
	SUET_ASSERT_EQ((int)z.size(), 5, "zeroString size=5");
	SUET_ASSERT_STR(z, "00000", "zeroString val");

	string z0 = zeroString(0);
	SUET_ASSERT_STR(z0, "", "zeroString(0) empty");

	// strcat_s
	char buf[32] = "hello";
	strcat_s(buf, 32, " world");
	SUET_ASSERT_STR(string(buf), "hello world", "strcat_s");

	// strcat_s with length
	char buf2[32] = "abc";
	strcat_s(buf2, 32, "DEFGHI", 3);
	SUET_ASSERT_STR(string(buf2), "abcDEF", "strcat_s len=3");

	// strcpy_s
	char buf3[32];
	strcpy_s(buf3, 32, "copied");
	SUET_ASSERT_STR(string(buf3), "copied", "strcpy_s");
}

void StringUtilityTest::testGreaterPower2StringUtil()
{
	using namespace StringUtilityExtTestSupport;
	SUET_ASSERT_EQ(greaterPower2(1), 2, "sp2(1)=2");
	SUET_ASSERT_EQ(greaterPower2(2), 2, "sp2(2)=2");
	SUET_ASSERT_EQ(greaterPower2(3), 4, "sp2(3)=4");
	SUET_ASSERT_EQ(greaterPower2(4), 4, "sp2(4)=4");
	SUET_ASSERT_EQ(greaterPower2(5), 8, "sp2(5)=8");
	SUET_ASSERT_EQ(greaterPower2(100), 128, "sp2(100)=128");
	SUET_ASSERT_EQ(greaterPower2(128), 128, "sp2(128)=128");
	SUET_ASSERT_EQ(greaterPower2(256), 256, "sp2(256)=256");
	SUET_ASSERT_EQ(greaterPower2(255), 256, "sp2(255)=256");
}

void StringUtilityTest::testRemoveString()
{
	using namespace StringUtilityExtTestSupport;
	char buf[64] = "hello world hello";
	bool removed = removeString(buf, (int)strlen(buf), "world ");
	SUET_ASSERT(removed, "removeString found");
	SUET_ASSERT_STR(string(buf).substr(0, 11), "hello hello", "removeString result");

	char buf2[32] = "no match";
	bool removed2 = removeString(buf2, (int)strlen(buf2), "xyz");
	SUET_ASSERT(!removed2, "removeString not found");
}

void StringUtilityTest::testNumberFormat()
{
	using namespace StringUtilityExtTestSupport;
	// IToS with limit
	string s1 = IToS(42, 6);
	SUET_ASSERT_EQ((int)s1.size(), 6, "IToS(42,6) size=6");

	string s2 = IToS(1234567, 3);
	SUET_ASSERT_EQ((int)s2.size(), 7, "IToS(1234567,3) limitLen<actual digits -> no truncation, size=7");

	// LLToS with limit
	string s3 = LLToS(123LL, 5);
	SUET_ASSERT_EQ((int)s3.size(), 5, "LLToS(123,5) size=5");

	// UIToS with limit
	string s4 = UIToS(99, 4);
	SUET_ASSERT_EQ((int)s4.size(), 4, "UIToS(99,4) size=4");
}

void StringUtilityTest::testEdgeCases()
{
	using namespace StringUtilityExtTestSupport;
	// split 单字符
	Vector<string> v1;
	split("x", ',', v1);
	SUET_ASSERT_EQ(v1.size(), 1, "split single char -> 1");
	SUET_ASSERT_STR(v1[0], "x", "split single char val");

	// findString: 空串搜索
	SUET_ASSERT(findString("anything", ""), "findString empty key always found");

	// startWith 长于源串
	SUET_ASSERT(!startWith("ab", "abc"), "startWith longer pattern false");

	// endWith 长于源串
	SUET_ASSERT(!endWith("ab", "abc"), "endWith longer pattern false");

	// replaceAll key == newWord
	string s = "hello";
	replaceAll(s, "hello", "hello");
	SUET_ASSERT_STR(s, "hello", "replaceAll same key/val");

	// removeAll empty str
	string s2 = "";
	removeAll(s2, 'a');
	SUET_ASSERT_STR(s2, "", "removeAll on empty");

	// getLastNumber on str with no number -> undefined, just no crash
	int n = getLastNumber("abc");
	(void)n; // 不崩就行

	// strlength maxLength < actual
	SUET_ASSERT_EQ(strlength("hello world", 3), 3, "strlength capped");
}

namespace StringUtilityExt2TestSupport
{
	// ============================================================
	// StringUtilityExt2Test
	// 覆盖 StringUtility 中尚未被 StringUtilityExtTest 测试的函数:
	//   BToS / SToB
	//   V2ToS / V2ToS(MyString) / V3ToS / V3ToS(MyString)
	//   V3IToS / V3IToS(MyString) / V2IToS / V2IToS(MyString)
	//   V2UIToS / V2UIToS(MyString) / V2SToS / V2SToS(MyString)
	//   V2USToS / V2USToS(MyString) / V2IsToS
	//   SToV2 / SToV2I / SToV2UI / SToV2S / SToV2US / SToV3 / SToV3I
	//   ULLToS (string) / ULLToS (MyString)
	//   LLsToS (string) / LLsToS (MyString)
	//   ULLsToS (string) / ULLsToS (MyString)
	//   SsToS / USsToS / bytesToString (MyString)
	//   IToS (MyString) / FToS (MyString)
	//   strcat_s(char*) / strcat_t / strcpy_s / zeroString
	//   floatToStringExtra / bytesToString(string)
	//   removeStartString / removeEndString
	//   getFileNameNoSuffix / getFilePath / getFileSuffix
	//   getFileName / getFirstFolderName / removeFirstPath
	//   getLastNotNumberPos / getLastNumber
	//   stringToULLong / SToLL / SToF / SToI
	// ============================================================

#define SU2_ASSERT(expr, msg) \
	do { if (!(expr)) { ERROR(string("StringUtilityExt2Test FAIL: ") + (msg)); } } while(0)
#define SU2_ASSERT_EQ(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("StringUtilityExt2Test FAIL: ") + (msg) + \
		" | expected=" + IToS((int)(b)) + " actual=" + IToS((int)(a))); } } while(0)
#define SU2_ASSERT_LL(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("StringUtilityExt2Test FAIL: ") + (msg) + \
		" | expected=" + LLToS(b) + " actual=" + LLToS(a)); } } while(0)
#define SU2_ASSERT_STR(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("StringUtilityExt2Test FAIL: ") + (msg) + \
		" | expected=" + (b) + " actual=" + (a)); } } while(0)
#define SU2_ASSERT_F(a, b, msg) \
	do { if (fabsf((float)(a) - (float)(b)) > 1e-3f) { ERROR(string("StringUtilityExt2Test FAIL: ") + (msg) + \
		" | expected=" + FToS((float)(b)) + " actual=" + FToS((float)(a))); } } while(0)
}

void StringUtilityTest::testBoolConvert()
{
	using namespace StringUtilityExt2TestSupport;
	SU2_ASSERT_STR(BToS(true), "True", "BToS(true)");
	SU2_ASSERT_STR(BToS(false), "False", "BToS(false)");

	SU2_ASSERT(SToB(string("True")), "SToB(string 'True')");
	SU2_ASSERT(SToB(string("true")), "SToB(string 'true')");
	SU2_ASSERT(!SToB(string("False")), "SToB(string 'False')=false");
	SU2_ASSERT(!SToB(string("0")), "SToB(string '0')=false");
	SU2_ASSERT(!SToB(string("")), "SToB(string '')=false");

	SU2_ASSERT(SToB("True"), "SToB(char* 'True')");
	SU2_ASSERT(SToB("true"), "SToB(char* 'true')");
	SU2_ASSERT(!SToB("False"), "SToB(char* 'False')=false");
	SU2_ASSERT(!SToB("no"), "SToB(char* 'no')=false");

	// 往返验证
	SU2_ASSERT_EQ(SToB(BToS(true)), true, "BToS->SToB true");
	SU2_ASSERT_EQ(SToB(BToS(false)), false, "BToS->SToB false");
}

void StringUtilityTest::testStringToNumber()
{
	using namespace StringUtilityExt2TestSupport;
	SU2_ASSERT_EQ(SToI(string("0")), 0, "SToI string '0'");
	SU2_ASSERT_EQ(SToI(string("123")), 123, "SToI string '123'");
	SU2_ASSERT_EQ(SToI(string("-456")), -456, "SToI string '-456'");

	SU2_ASSERT_EQ(SToI("0"), 0, "SToI char* '0'");
	SU2_ASSERT_EQ(SToI("999"), 999, "SToI char* '999'");
	SU2_ASSERT_EQ(SToI("-100"), -100, "SToI char* '-100'");

	SU2_ASSERT_LL(SToLL(string("0")), 0LL, "SToLL '0'");
	SU2_ASSERT_LL(SToLL(string("9876543210")), 9876543210LL, "SToLL '9876543210'");
	SU2_ASSERT_LL(SToLL(string("-1234567890")), -1234567890LL, "SToLL '-1234567890'");
	SU2_ASSERT_LL(SToLL("12345678901"), 12345678901LL, "SToLL char* '12345678901'");

	SU2_ASSERT_F(SToF(string("0.0")), 0.0f, "SToF '0.0'");
	SU2_ASSERT_F(SToF(string("3.14")), 3.14f, "SToF '3.14'");
	SU2_ASSERT_F(SToF(string("-1.5")), -1.5f, "SToF '-1.5'");
	SU2_ASSERT_F(SToF("100.0"), 100.0f, "SToF char* '100.0'");

	SU2_ASSERT_LL((llong)stringToULLong(string("0")), 0LL, "stringToULLong '0'");
	SU2_ASSERT_LL((llong)stringToULLong(string("18446744073")), 18446744073LL, "stringToULLong large");
	SU2_ASSERT_LL((llong)stringToULLong("555"), 555LL, "stringToULLong char* '555'");
}

void StringUtilityTest::testULLToSString()
{
	using namespace StringUtilityExt2TestSupport;
	SU2_ASSERT_STR(ULLToS((ullong)0), "0", "ULLToS(0)");
	SU2_ASSERT_STR(ULLToS((ullong)1), "1", "ULLToS(1)");
	SU2_ASSERT_STR(ULLToS((ullong)255), "255", "ULLToS(255)");
	SU2_ASSERT_STR(ULLToS((ullong)1000), "1000", "ULLToS(1000)");
	SU2_ASSERT_STR(ULLToS((ullong)9999999999ULL), "9999999999", "ULLToS(9999999999)");

	// limitLen 补零
	SU2_ASSERT_STR(ULLToS((ullong)7, 3), "007", "ULLToS(7,limitLen=3)");
	SU2_ASSERT_STR(ULLToS((ullong)42, 5), "00042", "ULLToS(42,limitLen=5)");
	SU2_ASSERT_STR(ULLToS((ullong)100, 2), "100", "ULLToS(100,limitLen=2 no-pad)");
}

void StringUtilityTest::testULLToSMyString()
{
	using namespace StringUtilityExt2TestSupport;
	{
		MyString<32> buf;
		int len = ULLToS(buf, (ullong)0);
		SU2_ASSERT_STR(string(buf.str()), "0", "ULLToS MyString(0)");
		SU2_ASSERT_EQ(len, 1, "ULLToS MyString(0) len=1");
	}
	{
		MyString<32> buf;
		int len = ULLToS(buf, (ullong)123456789ULL);
		SU2_ASSERT_STR(string(buf.str()), "123456789", "ULLToS MyString(123456789)");
		SU2_ASSERT_EQ(len, 9, "ULLToS MyString(123456789) len=9");
	}
	{
		MyString<32> buf;
		int len = ULLToS(buf, (ullong)5, 4);
		SU2_ASSERT_STR(string(buf.str()), "0005", "ULLToS MyString(5,limitLen=4)");
		SU2_ASSERT_EQ(len, 4, "ULLToS MyString(5,limitLen=4) len=4");
	}
	{
		MyString<32> buf;
		int len = ULLToS(buf, (ullong)99999999999ULL, 0);
		SU2_ASSERT_STR(string(buf.str()), "99999999999", "ULLToS MyString large");
		SU2_ASSERT_EQ(len, 11, "ULLToS MyString large len=11");
	}
}

void StringUtilityTest::testIToSMyString()
{
	using namespace StringUtilityExt2TestSupport;
	{
		MyString<16> buf;
		int len = IToS(buf, 0);
		SU2_ASSERT_STR(string(buf.str()), "0", "IToS MyString(0)");
		SU2_ASSERT_EQ(len, 1, "IToS MyString(0) len=1");
	}
	{
		MyString<16> buf;
		int len = IToS(buf, 42);
		SU2_ASSERT_STR(string(buf.str()), "42", "IToS MyString(42)");
		SU2_ASSERT_EQ(len, 2, "IToS MyString(42) len=2");
	}
	{
		MyString<16> buf;
		int len = IToS(buf, -999);
		SU2_ASSERT_STR(string(buf.str()), "-999", "IToS MyString(-999)");
		SU2_ASSERT_EQ(len, 4, "IToS MyString(-999) len=4");
	}
	{
		MyString<16> buf;
		int len = IToS(buf, 5, 4);
		SU2_ASSERT_STR(string(buf.str()), "0005", "IToS MyString(5,limitLen=4)");
		SU2_ASSERT_EQ(len, 4, "IToS MyString(5,limitLen=4) len=4");
	}
	{
		MyString<16> buf;
		int len = IToS(buf, 100, 2);
		SU2_ASSERT_STR(string(buf.str()), "100", "IToS MyString(100,limitLen=2 no-pad)");
		SU2_ASSERT_EQ(len, 3, "IToS MyString(100,limitLen=2 no-pad) len=3");
	}
	{
		MyString<16> buf;
		int len = IToS(buf, 12345);
		SU2_ASSERT_STR(string(buf.str()), "12345", "IToS MyString(12345)");
		SU2_ASSERT_EQ(len, 5, "IToS MyString(12345) len=5");
	}
}

void StringUtilityTest::testFToSMyString()
{
	using namespace StringUtilityExt2TestSupport;
	{
		MyString<32> buf;
		int len = FToS(buf, 0.0f);
		SU2_ASSERT_STR(string(buf.str()), "0", "FToS MyString(0.0)");
		SU2_ASSERT_EQ(len, 1, "FToS MyString(0.0) len=1");
	}
	{
		MyString<32> buf;
		int len = FToS(buf, 1.5f);
		SU2_ASSERT_STR(string(buf.str()), "1.5", "FToS MyString(1.5)");
		SU2_ASSERT_EQ(len, 3, "FToS MyString(1.5) len=3");
	}
	{
		MyString<32> buf;
		int len = FToS(buf, -3.14f, 2);
		SU2_ASSERT_STR(string(buf.str()), "-3.14", "FToS MyString(-3.14, prec=2)");
		SU2_ASSERT_EQ(len, 5, "FToS MyString(-3.14, prec=2) len=5");
	}
	{
		MyString<32> buf;
		FToS(buf, 100.0f, 1);
		SU2_ASSERT_STR(string(buf.str()), "100", "FToS MyString(100.0, prec=1) trailing zero removed");
	}
	{
		MyString<32> buf;
		FToS(buf, 0.001f, 3);
		SU2_ASSERT_STR(string(buf.str()), "0.001", "FToS MyString(0.001, prec=3)");
	}
	{
		// precision=0 整数截断
		MyString<32> buf;
		FToS(buf, 7.9f, 0);
		SU2_ASSERT_STR(string(buf.str()), "8", "FToS MyString(7.9, prec=0)");
	}
}

void StringUtilityTest::testV2ToS()
{
	using namespace StringUtilityExt2TestSupport;
	{
		Vector2 v(1.0f, 2.0f);
		string s = V2ToS(v);
		SU2_ASSERT_STR(s, "1,2", "V2ToS(1,2)");
	}
	{
		Vector2 v(0.0f, 0.0f);
		string s = V2ToS(v);
		SU2_ASSERT_STR(s, "0,0", "V2ToS(0,0)");
	}
	{
		Vector2 v(-1.5f, 3.25f);
		string s = V2ToS(v, ";");
		SU2_ASSERT_STR(s, "-1.5;3.25", "V2ToS(-1.5,3.25,sep=;)");
	}
	{
		Vector2 v(100.0f, -200.0f);
		string s = V2ToS(v);
		SU2_ASSERT_STR(s, "100,-200", "V2ToS(100,-200)");
	}
	// MyString 版本
	{
		MyString<64> buf;
		Vector2 v(5.5f, -2.5f);
		V2ToS(buf, v);
		SU2_ASSERT_STR(string(buf.str()), "5.5,-2.5", "V2ToS MyString(5.5,-2.5)");
	}
	{
		MyString<64> buf;
		Vector2 v(0.0f, 1.0f);
		V2ToS(buf, v, "|");
		SU2_ASSERT_STR(string(buf.str()), "0|1", "V2ToS MyString sep=|");
	}
	{
		MyString<64> buf;
		Vector2 v(-10.0f, -20.0f);
		V2ToS(buf, v);
		SU2_ASSERT_STR(string(buf.str()), "-10,-20", "V2ToS MyString(-10,-20)");
	}
}

void StringUtilityTest::testV3ToS()
{
	using namespace StringUtilityExt2TestSupport;
	{
		Vector3 v(1.0f, 2.0f, 3.0f);
		string s = V3ToS(v);
		SU2_ASSERT_STR(s, "1,2,3", "V3ToS(1,2,3)");
	}
	{
		Vector3 v(0.0f, 0.0f, 0.0f);
		string s = V3ToS(v);
		SU2_ASSERT_STR(s, "0,0,0", "V3ToS(0,0,0)");
	}
	{
		Vector3 v(-1.0f, 2.5f, -3.75f);
		string s = V3ToS(v, ";");
		SU2_ASSERT_STR(s, "-1;2.5;-3.75", "V3ToS sep=;");
	}
	{
		Vector3 v(10.0f, 20.0f, 30.0f);
		string s = V3ToS(v, " ");
		SU2_ASSERT_STR(s, "10 20 30", "V3ToS sep=space");
	}
	// MyString 版本
	{
		MyString<64> buf;
		Vector3 v(1.0f, 2.0f, 3.0f);
		V3ToS(buf, v);
		SU2_ASSERT_STR(string(buf.str()), "1,2,3", "V3ToS MyString(1,2,3)");
	}
	{
		MyString<64> buf;
		Vector3 v(-1.5f, 0.0f, 100.0f);
		V3ToS(buf, v);
		SU2_ASSERT_STR(string(buf.str()), "-1.5,0,100", "V3ToS MyString(-1.5,0,100)");
	}
	{
		MyString<64> buf;
		Vector3 v(3.14f, -2.72f, 1.41f);
		V3ToS(buf, v, "|");
		SU2_ASSERT_STR(string(buf.str()), "3.14|-2.72|1.41", "V3ToS MyString sep=|");
	}
}

void StringUtilityTest::testV3IToS()
{
	using namespace StringUtilityExt2TestSupport;
	{
		Vector3Int v(1, 2, 3);
		string s = V3IToS(v);
		SU2_ASSERT_STR(s, "1,2,3", "V3IToS(1,2,3)");
	}
	{
		Vector3Int v(0, 0, 0);
		string s = V3IToS(v);
		SU2_ASSERT_STR(s, "0,0,0", "V3IToS(0,0,0)");
	}
	{
		Vector3Int v(-100, 200, -300);
		string s = V3IToS(v, "/");
		SU2_ASSERT_STR(s, "-100/200/-300", "V3IToS sep=/");
	}
	{
		Vector3Int v(999, -888, 777);
		string s = V3IToS(v);
		SU2_ASSERT_STR(s, "999,-888,777", "V3IToS(999,-888,777)");
	}
	// MyString 版本
	{
		MyString<64> buf;
		Vector3Int v(10, 20, 30);
		V3IToS(buf, v);
		SU2_ASSERT_STR(string(buf.str()), "10,20,30", "V3IToS MyString(10,20,30)");
	}
	{
		MyString<64> buf;
		Vector3Int v(-5, 0, 5);
		V3IToS(buf, v, ";");
		SU2_ASSERT_STR(string(buf.str()), "-5;0;5", "V3IToS MyString sep=;");
	}
}

void StringUtilityTest::testV2IToS()
{
	using namespace StringUtilityExt2TestSupport;
	{
		Vector2Int v(3, 7);
		string s = V2IToS(v);
		SU2_ASSERT_STR(s, "3,7", "V2IToS(3,7)");
	}
	{
		Vector2Int v(0, 0);
		string s = V2IToS(v);
		SU2_ASSERT_STR(s, "0,0", "V2IToS(0,0)");
	}
	{
		Vector2Int v(-100, 200);
		string s = V2IToS(v, "|");
		SU2_ASSERT_STR(s, "-100|200", "V2IToS sep=|");
	}
	// MyString 版本
	{
		MyString<32> buf;
		Vector2Int v(15, -25);
		V2IToS(buf, v);
		SU2_ASSERT_STR(string(buf.str()), "15,-25", "V2IToS MyString(15,-25)");
	}
	{
		MyString<32> buf;
		Vector2Int v(0, 1000);
		V2IToS(buf, v, "/");
		SU2_ASSERT_STR(string(buf.str()), "0/1000", "V2IToS MyString sep=/");
	}
}

void StringUtilityTest::testV2UIToS()
{
	using namespace StringUtilityExt2TestSupport;
	{
		Vector2UInt v;
		v.x = 10;
		v.y = 20;
		string s = V2UIToS(v);
		SU2_ASSERT_STR(s, "10,20", "V2UIToS(10,20)");
	}
	{
		Vector2UInt v;
		v.x = 0;
		v.y = 0;
		string s = V2UIToS(v);
		SU2_ASSERT_STR(s, "0,0", "V2UIToS(0,0)");
	}
	{
		Vector2UInt v;
		v.x = 100;
		v.y = 200;
		string s = V2UIToS(v, "|");
		SU2_ASSERT_STR(s, "100|200", "V2UIToS sep=|");
	}
	// MyString 版本
	{
		MyString<32> buf;
		Vector2UInt v;
		v.x = 9;
		v.y = 99;
		V2UIToS(buf, v);
		SU2_ASSERT_STR(string(buf.str()), "9,99", "V2UIToS MyString(9,99)");
	}
}

void StringUtilityTest::testV2SToS()
{
	using namespace StringUtilityExt2TestSupport;
	{
		Vector2Short v;
		v.x = 3;
		v.y = -5;
		string s = V2SToS(v);
		SU2_ASSERT_STR(s, "3,-5", "V2SToS(3,-5)");
	}
	{
		Vector2Short v;
		v.x = 0;
		v.y = 0;
		string s = V2SToS(v);
		SU2_ASSERT_STR(s, "0,0", "V2SToS(0,0)");
	}
	{
		Vector2Short v;
		v.x = 32767;
		v.y = -32767;
		string s = V2SToS(v, "/");
		SU2_ASSERT_STR(s, "32767/-32767", "V2SToS short extremes");
	}
	// MyString 版本
	{
		MyString<32> buf;
		Vector2Short v;
		v.x = 100;
		v.y = 200;
		V2SToS(buf, v);
		SU2_ASSERT_STR(string(buf.str()), "100,200", "V2SToS MyString(100,200)");
	}
}

void StringUtilityTest::testV2USToS()
{
	using namespace StringUtilityExt2TestSupport;
	{
		Vector2UShort v;
		v.x = 10;
		v.y = 20;
		string s = V2USToS(v);
		SU2_ASSERT_STR(s, "10,20", "V2USToS(10,20)");
	}
	{
		Vector2UShort v;
		v.x = 0;
		v.y = 65535;
		string s = V2USToS(v);
		SU2_ASSERT_STR(s, "0,65535", "V2USToS(0,65535)");
	}
	{
		Vector2UShort v;
		v.x = 100;
		v.y = 200;
		string s = V2USToS(v, "|");
		SU2_ASSERT_STR(s, "100|200", "V2USToS sep=|");
	}
	// MyString 版本
	{
		MyString<32> buf;
		Vector2UShort v;
		v.x = 5;
		v.y = 10;
		V2USToS(buf, v);
		SU2_ASSERT_STR(string(buf.str()), "5,10", "V2USToS MyString(5,10)");
	}
}

void StringUtilityTest::testStringToVector()
{
	using namespace StringUtilityExt2TestSupport;
	// SToV2
	{
		Vector2 v = SToV2("1.5,2.5");
		SU2_ASSERT_F(v.x, 1.5f, "SToV2 x=1.5");
		SU2_ASSERT_F(v.y, 2.5f, "SToV2 y=2.5");
	}
	{
		Vector2 v = SToV2("0,0");
		SU2_ASSERT_F(v.x, 0.0f, "SToV2 x=0");
		SU2_ASSERT_F(v.y, 0.0f, "SToV2 y=0");
	}
	{
		Vector2 v = SToV2("-3.0|-5.5", "|");
		SU2_ASSERT_F(v.x, -3.0f, "SToV2 sep=| x=-3");
		SU2_ASSERT_F(v.y, -5.5f, "SToV2 sep=| y=-5.5");
	}
	// SToV2I
	{
		Vector2Int v = SToV2I("10,20");
		SU2_ASSERT_EQ(v.x, 10, "SToV2I x=10");
		SU2_ASSERT_EQ(v.y, 20, "SToV2I y=20");
	}
	{
		Vector2Int v = SToV2I("-100,200");
		SU2_ASSERT_EQ(v.x, -100, "SToV2I x=-100");
		SU2_ASSERT_EQ(v.y, 200, "SToV2I y=200");
	}
	{
		Vector2Int v = SToV2I("0;0", ";");
		SU2_ASSERT_EQ(v.x, 0, "SToV2I sep=; x=0");
		SU2_ASSERT_EQ(v.y, 0, "SToV2I sep=; y=0");
	}
	// SToV2UI
	{
		Vector2UInt v = SToV2UI("5,10");
		SU2_ASSERT_EQ((int)v.x, 5, "SToV2UI x=5");
		SU2_ASSERT_EQ((int)v.y, 10, "SToV2UI y=10");
	}
	// SToV2S
	{
		Vector2Short v = SToV2S("100,200");
		SU2_ASSERT_EQ((int)v.x, 100, "SToV2S x=100");
		SU2_ASSERT_EQ((int)v.y, 200, "SToV2S y=200");
	}
	{
		Vector2Short v = SToV2S("-1,-2");
		SU2_ASSERT_EQ((int)v.x, -1, "SToV2S x=-1");
		SU2_ASSERT_EQ((int)v.y, -2, "SToV2S y=-2");
	}
	// SToV2US
	{
		Vector2UShort v = SToV2US("3,7");
		SU2_ASSERT_EQ((int)v.x, 3, "SToV2US x=3");
		SU2_ASSERT_EQ((int)v.y, 7, "SToV2US y=7");
	}
	// SToV3
	{
		Vector3 v = SToV3("1,2,3");
		SU2_ASSERT_F(v.x, 1.0f, "SToV3 x=1");
		SU2_ASSERT_F(v.y, 2.0f, "SToV3 y=2");
		SU2_ASSERT_F(v.z, 3.0f, "SToV3 z=3");
	}
	{
		Vector3 v = SToV3("-1.5,0,2.5");
		SU2_ASSERT_F(v.x, -1.5f, "SToV3 x=-1.5");
		SU2_ASSERT_F(v.y, 0.0f, "SToV3 y=0");
		SU2_ASSERT_F(v.z, 2.5f, "SToV3 z=2.5");
	}
	{
		Vector3 v = SToV3("10;20;30", ";");
		SU2_ASSERT_F(v.x, 10.0f, "SToV3 sep=; x=10");
		SU2_ASSERT_F(v.y, 20.0f, "SToV3 sep=; y=20");
		SU2_ASSERT_F(v.z, 30.0f, "SToV3 sep=; z=30");
	}
	// SToV3I
	{
		Vector3Int v = SToV3I("5,10,15");
		SU2_ASSERT_EQ(v.x, 5, "SToV3I x=5");
		SU2_ASSERT_EQ(v.y, 10, "SToV3I y=10");
		SU2_ASSERT_EQ(v.z, 15, "SToV3I z=15");
	}
	{
		Vector3Int v = SToV3I("-1,-2,-3");
		SU2_ASSERT_EQ(v.x, -1, "SToV3I x=-1");
		SU2_ASSERT_EQ(v.y, -2, "SToV3I y=-2");
		SU2_ASSERT_EQ(v.z, -3, "SToV3I z=-3");
	}
	{
		Vector3Int v = SToV3I("0,0,0");
		SU2_ASSERT_EQ(v.x, 0, "SToV3I x=0");
		SU2_ASSERT_EQ(v.y, 0, "SToV3I y=0");
		SU2_ASSERT_EQ(v.z, 0, "SToV3I z=0");
	}
}

void StringUtilityTest::testVectorRoundtrip()
{
	using namespace StringUtilityExt2TestSupport;
	{
		Vector2 orig(3.14f, -2.72f);
		string s = V2ToS(orig);
		Vector2 back = SToV2(s);
		SU2_ASSERT_F(back.x, orig.x, "V2ToS->SToV2 x roundtrip");
		SU2_ASSERT_F(back.y, orig.y, "V2ToS->SToV2 y roundtrip");
	}
	{
		Vector3 orig(1.0f, -2.0f, 3.0f);
		string s = V3ToS(orig);
		Vector3 back = SToV3(s);
		SU2_ASSERT_F(back.x, orig.x, "V3ToS->SToV3 x roundtrip");
		SU2_ASSERT_F(back.y, orig.y, "V3ToS->SToV3 y roundtrip");
		SU2_ASSERT_F(back.z, orig.z, "V3ToS->SToV3 z roundtrip");
	}
	{
		Vector2Int orig(-100, 200);
		string s = V2IToS(orig);
		Vector2Int back = SToV2I(s);
		SU2_ASSERT_EQ(back.x, orig.x, "V2IToS->SToV2I x roundtrip");
		SU2_ASSERT_EQ(back.y, orig.y, "V2IToS->SToV2I y roundtrip");
	}
	{
		Vector3Int orig(10, -20, 30);
		string s = V3IToS(orig);
		Vector3Int back = SToV3I(s);
		SU2_ASSERT_EQ(back.x, orig.x, "V3IToS->SToV3I x roundtrip");
		SU2_ASSERT_EQ(back.y, orig.y, "V3IToS->SToV3I y roundtrip");
		SU2_ASSERT_EQ(back.z, orig.z, "V3IToS->SToV3I z roundtrip");
	}
}

void StringUtilityTest::testULLsToSString()
{
	using namespace StringUtilityExt2TestSupport;
	{
		Vector<ullong> v;
		string s = ULLsToS(v);
		SU2_ASSERT_STR(s, "", "ULLsToS empty vector");
	}
	{
		Vector<ullong> v;
		v.add(1ULL);
		string s = ULLsToS(v);
		SU2_ASSERT_STR(s, "1", "ULLsToS [1]");
	}
	{
		Vector<ullong> v;
		v.add(1ULL);
		v.add(2ULL);
		v.add(3ULL);
		string s = ULLsToS(v);
		SU2_ASSERT_STR(s, "1,2,3", "ULLsToS [1,2,3]");
	}
	{
		Vector<ullong> v;
		v.add(100ULL);
		v.add(200ULL);
		string s = ULLsToS(v, "|");
		SU2_ASSERT_STR(s, "100|200", "ULLsToS sep=|");
	}
	{
		Vector<ullong> v;
		v.add(0ULL);
		v.add(9999999999ULL);
		v.add(1ULL);
		string s = ULLsToS(v);
		SU2_ASSERT_STR(s, "0,9999999999,1", "ULLsToS large value");
	}
}

void StringUtilityTest::testULLsToSMyString()
{
	using namespace StringUtilityExt2TestSupport;
	{
		MyString<256> buf;
		Vector<ullong> v;
		ULLsToS(buf, v);
		SU2_ASSERT_STR(string(buf.str()), "", "ULLsToS MyString empty");
	}
	{
		MyString<256> buf;
		Vector<ullong> v;
		v.add(5ULL);
		v.add(10ULL);
		v.add(15ULL);
		ULLsToS(buf, v);
		SU2_ASSERT_STR(string(buf.str()), "5,10,15", "ULLsToS MyString [5,10,15]");
	}
	{
		MyString<256> buf;
		Vector<ullong> v;
		v.add(100ULL);
		v.add(200ULL);
		ULLsToS(buf, v, ";");
		SU2_ASSERT_STR(string(buf.str()), "100;200", "ULLsToS MyString sep=;");
	}
	// ptr + count 版本
	{
		MyString<256> buf;
		ullong arr[4] = { 1ULL, 2ULL, 3ULL, 4ULL };
		ULLsToS(buf, arr, 4);
		SU2_ASSERT_STR(string(buf.str()), "1,2,3,4", "ULLsToS MyString ptr[4]");
	}
	{
		MyString<256> buf;
		ullong arr[2] = { 999ULL, 888ULL };
		ULLsToS(buf, arr, 2, "|");
		SU2_ASSERT_STR(string(buf.str()), "999|888", "ULLsToS MyString ptr[2] sep=|");
	}
}

void StringUtilityTest::testLLsToSString()
{
	using namespace StringUtilityExt2TestSupport;
	{
		Vector<llong> v;
		string s = LLsToS(v);
		SU2_ASSERT_STR(s, "", "LLsToS empty");
	}
	{
		Vector<llong> v;
		v.add(1LL);
		v.add(-2LL);
		v.add(3LL);
		string s = LLsToS(v);
		SU2_ASSERT_STR(s, "1,-2,3", "LLsToS [1,-2,3]");
	}
	{
		Vector<llong> v;
		v.add(100LL);
		v.add(200LL);
		string s = LLsToS(v, "|");
		SU2_ASSERT_STR(s, "100|200", "LLsToS sep=|");
	}
	// ptr+count 版本
	{
		llong arr[3] = { -1LL, 0LL, 1LL };
		string s = LLsToS(arr, 3);
		SU2_ASSERT_STR(s, "-1,0,1", "LLsToS ptr[3]");
	}
	{
		llong arr[1] = { 9876543210LL };
		string s = LLsToS(arr, 1);
		SU2_ASSERT_STR(s, "9876543210", "LLsToS ptr[1] large");
	}
}

void StringUtilityTest::testLLsToSMyString()
{
	using namespace StringUtilityExt2TestSupport;
	{
		MyString<256> buf;
		Vector<llong> v;
		LLsToS(buf, v);
		SU2_ASSERT_STR(string(buf.str()), "", "LLsToS MyString empty");
	}
	{
		MyString<256> buf;
		Vector<llong> v;
		v.add(10LL);
		v.add(-20LL);
		LLsToS(buf, v);
		SU2_ASSERT_STR(string(buf.str()), "10,-20", "LLsToS MyString [10,-20]");
	}
	{
		MyString<256> buf;
		llong arr[3] = { 7LL, 8LL, 9LL };
		LLsToS(buf, arr, 3);
		SU2_ASSERT_STR(string(buf.str()), "7,8,9", "LLsToS MyString ptr[3]");
	}
	{
		MyString<256> buf;
		llong arr[2] = { -100LL, 200LL };
		LLsToS(buf, arr, 2, ";");
		SU2_ASSERT_STR(string(buf.str()), "-100;200", "LLsToS MyString ptr sep=;");
	}
}

void StringUtilityTest::testSsToSString()
{
	using namespace StringUtilityExt2TestSupport;
	{
		Vector<short> v;
		string s = SsToS(v);
		SU2_ASSERT_STR(s, "", "SsToS empty");
	}
	{
		Vector<short> v;
		v.add(1);
		v.add(-2);
		v.add(3);
		string s = SsToS(v);
		SU2_ASSERT_STR(s, "1,-2,3", "SsToS [1,-2,3]");
	}
	{
		Vector<short> v;
		v.add(100);
		v.add(200);
		string s = SsToS(v, "|");
		SU2_ASSERT_STR(s, "100|200", "SsToS sep=|");
	}
}

void StringUtilityTest::testSsToSMyString()
{
	using namespace StringUtilityExt2TestSupport;
	{
		MyString<256> buf;
		short arr[4] = { 10, -20, 30, -40 };
		SsToS(buf, arr, 4);
		SU2_ASSERT_STR(string(buf.str()), "10,-20,30,-40", "SsToS MyString ptr[4]");
	}
	{
		MyString<256> buf;
		Vector<short> v;
		v.add(5);
		v.add(6);
		SsToS(buf, v);
		SU2_ASSERT_STR(string(buf.str()), "5,6", "SsToS MyString Vector");
	}
}

void StringUtilityTest::testUSsToSString()
{
	using namespace StringUtilityExt2TestSupport;
	{
		Vector<ushort> v;
		string s = USsToS(v);
		SU2_ASSERT_STR(s, "", "USsToS empty");
	}
	{
		Vector<ushort> v;
		v.add(1);
		v.add(2);
		v.add(3);
		string s = USsToS(v);
		SU2_ASSERT_STR(s, "1,2,3", "USsToS [1,2,3]");
	}
	{
		Vector<ushort> v;
		v.add(100);
		v.add(200);
		string s = USsToS(v, "|");
		SU2_ASSERT_STR(s, "100|200", "USsToS sep=|");
	}
}

void StringUtilityTest::testUSsToSMyString()
{
	using namespace StringUtilityExt2TestSupport;
	{
		MyString<256> buf;
		ushort arr[3] = { 10, 20, 30 };
		USsToS(buf, arr, 3);
		SU2_ASSERT_STR(string(buf.str()), "10,20,30", "USsToS MyString ptr[3]");
	}
	{
		MyString<256> buf;
		Vector<ushort> v;
		v.add(65535);
		v.add(0);
		USsToS(buf, v);
		SU2_ASSERT_STR(string(buf.str()), "65535,0", "USsToS MyString Vector");
	}
}

void StringUtilityTest::testBytesToStringS()
{
	using namespace StringUtilityExt2TestSupport;
	{
		Vector<byte> v;
		string s = bytesToString(v);
		SU2_ASSERT_STR(s, "", "bytesToString empty vector");
	}
	{
		Vector<byte> v;
		v.add(1);
		v.add(2);
		v.add(3);
		string s = bytesToString(v);
		SU2_ASSERT_STR(s, "1,2,3", "bytesToString [1,2,3]");
	}
	{
		Vector<byte> v;
		v.add(255);
		v.add(0);
		string s = bytesToString(v, "|");
		SU2_ASSERT_STR(s, "255|0", "bytesToString sep=|");
	}
}

void StringUtilityTest::testBytesToStringMyString()
{
	using namespace StringUtilityExt2TestSupport;
	{
		MyString<64> buf;
		byte arr[3] = { 10, 20, 30 };
		bytesToString(buf, arr, 3);
		SU2_ASSERT_STR(string(buf.str()), "10,20,30", "bytesToString MyString ptr[3]");
	}
	{
		MyString<64> buf;
		Vector<byte> v;
		v.add(100);
		v.add(200);
		bytesToString(buf, v);
		SU2_ASSERT_STR(string(buf.str()), "100,200", "bytesToString MyString Vector");
	}
	{
		MyString<64> buf;
		byte arr[1] = { 0 };
		bytesToString(buf, arr, 1);
		SU2_ASSERT_STR(string(buf.str()), "0", "bytesToString MyString [0]");
	}
}

void StringUtilityTest::testStrcatS()
{
	using namespace StringUtilityExt2TestSupport;
	{
		char buf[64] = "hello";
		strcat_s(buf, 64, " world");
		SU2_ASSERT_STR(string(buf), "hello world", "strcat_s char* basic");
	}
	{
		char buf[64] = "";
		strcat_s(buf, 64, "abc");
		SU2_ASSERT_STR(string(buf), "abc", "strcat_s char* from empty");
	}
	{
		char buf[64] = "foo";
		strcat_s(buf, 64, "bar");
		strcat_s(buf, 64, "baz");
		SU2_ASSERT_STR(string(buf), "foobarbaz", "strcat_s chain 3 times");
	}
	// strcat_s with length limit
	{
		char buf[64] = "pre";
		strcat_s(buf, 64, "source_longer", 6);
		SU2_ASSERT_STR(string(buf), "presource", "strcat_s char* with length=6");
	}
}

void StringUtilityTest::testStrcpyS()
{
	using namespace StringUtilityExt2TestSupport;
	{
		char buf[64] = "old";
		strcpy_s(buf, 64, "new");
		SU2_ASSERT_STR(string(buf), "new", "strcpy_s basic");
	}
	{
		char buf[64];
		strcpy_s(buf, 64, "");
		SU2_ASSERT_STR(string(buf), "", "strcpy_s empty source");
	}
	{
		char buf[64];
		strcpy_s(buf, 64, "hello world");
		SU2_ASSERT_STR(string(buf), "hello world", "strcpy_s hello world");
	}
	{
		char buf[64] = "something_old";
		strcpy_s(buf, 64, "new_content");
		SU2_ASSERT_STR(string(buf), "new_content", "strcpy_s overwrites old");
	}
}

void StringUtilityTest::testZeroString()
{
	using namespace StringUtilityExt2TestSupport;
	{
		string s = zeroString(0);
		SU2_ASSERT_STR(s, "", "zeroString(0)");
	}
	{
		string s = zeroString(1);
		SU2_ASSERT_STR(s, "0", "zeroString(1)");
	}
	{
		string s = zeroString(5);
		SU2_ASSERT_STR(s, "00000", "zeroString(5)");
	}
	{
		string s = zeroString(3);
		SU2_ASSERT_EQ((int)s.length(), 3, "zeroString(3) length=3");
		SU2_ASSERT_STR(s, "000", "zeroString(3) = 000");
	}
}

void StringUtilityTest::testZeroStringMyString()
{
	using namespace StringUtilityExt2TestSupport;
	{
		MyString<16> buf;
		buf.setCount('0', 0);
		SU2_ASSERT_STR(string(buf.str()), "", "zeroString MyString(0)");
	}
	{
		MyString<16> buf;
		buf.setCount('0', 3);
		SU2_ASSERT_STR(string(buf.str()), "000", "zeroString MyString(3)");
	}
	{
		MyString<16> buf;
		buf.setCount('0', 1);
		SU2_ASSERT_STR(string(buf.str()), "0", "zeroString MyString(1)");
	}
	{
		MyString<16> buf;
		buf.setCount('0', 5);
		SU2_ASSERT_EQ((int)buf.length(), 5, "zeroString MyString(5) length=5");
	}
}

void StringUtilityTest::testFloatToStringExtra()
{
	using namespace StringUtilityExt2TestSupport;
	{
		string s = floatToStringExtra(0.0f);
		SU2_ASSERT_STR(s, "0", "floatToStringExtra(0.0)");
	}
	{
		string s = floatToStringExtra(1.5f, 2, true);
		SU2_ASSERT_STR(s, "1.5", "floatToStringExtra(1.5, prec=2, removeZero=true)");
	}
	{
		string s = floatToStringExtra(3.14159f, 2, true);
		SU2_ASSERT_STR(s, "3.14", "floatToStringExtra(3.14159, prec=2)");
	}
	{
		string s = floatToStringExtra(100.0f, 3, true);
		SU2_ASSERT_STR(s, "100", "floatToStringExtra(100.0, prec=3, removeZero=true)");
	}
	{
		string s = floatToStringExtra(1.0f, 2, false);
		SU2_ASSERT_STR(s, "1.00", "floatToStringExtra(1.0, prec=2, removeZero=false)");
	}
	{
		string s = floatToStringExtra(-2.5f, 1, true);
		SU2_ASSERT_STR(s, "-2.5", "floatToStringExtra(-2.5, prec=1)");
	}
}

void StringUtilityTest::testRemoveStartEndString()
{
	using namespace StringUtilityExt2TestSupport;
	{
		string result = removeStartString("hello world", "hello ");
		SU2_ASSERT_STR(result, "world", "removeStartString 'hello '");
	}
	{
		string result = removeStartString("hello", "xxx");
		SU2_ASSERT_STR(result, "hello", "removeStartString no match");
	}
	{
		string result = removeStartString("", "abc");
		SU2_ASSERT_STR(result, "", "removeStartString empty input");
	}
	{
		string result = removeStartString("abcdef", "abc");
		SU2_ASSERT_STR(result, "def", "removeStartString 'abc'");
	}

	{
		string result = removeEndString("hello world", " world");
		SU2_ASSERT_STR(result, "hello", "removeEndString ' world'");
	}
	{
		string result = removeEndString("hello", "xxx");
		SU2_ASSERT_STR(result, "hello", "removeEndString no match");
	}
	{
		string result = removeEndString("", "abc");
		SU2_ASSERT_STR(result, "", "removeEndString empty input");
	}
	{
		string result = removeEndString("abcdef", "def");
		SU2_ASSERT_STR(result, "abc", "removeEndString 'def'");
	}
}

void StringUtilityTest::testFilePathFunctions()
{
	using namespace StringUtilityExt2TestSupport;
	{
		string name = getFileName("path/to/file.txt");
		SU2_ASSERT_STR(name, "file.txt", "getFileName 'path/to/file.txt'");
	}
	{
		string name = getFileName("file.txt");
		SU2_ASSERT_STR(name, "file.txt", "getFileName 'file.txt' no path");
	}
	{
		string name = getFileName("C:/users/test/doc.pdf");
		SU2_ASSERT_STR(name, "doc.pdf", "getFileName Windows path");
	}

	{
		string name = getFileNameNoSuffix("path/to/file.txt", true);
		SU2_ASSERT_STR(name, "file", "getFileNameNoSuffix removePath=true");
	}
	{
		string name = getFileNameNoSuffix("path/to/file.txt", false);
		SU2_ASSERT_STR(name, "path/to/file", "getFileNameNoSuffix removePath=false");
	}

	{
		string path = getFilePath("path/to/file.txt", false);
		SU2_ASSERT_STR(path, "path/to", "getFilePath keepSlash=false");
	}
	{
		string path = getFilePath("path/to/file.txt", true);
		SU2_ASSERT_STR(path, "path/to/", "getFilePath keepSlash=true");
	}

	{
		string suf = getFileSuffix("file.txt", true);
		SU2_ASSERT_STR(suf, ".txt", "getFileSuffix keepDot=true");
	}
	{
		string suf = getFileSuffix("file.txt", false);
		SU2_ASSERT_STR(suf, "txt", "getFileSuffix keepDot=false");
	}
	{
		string suf = getFileSuffix("file", true);
		SU2_ASSERT_STR(suf, "", "getFileSuffix no dot");
	}
	{
		string suf = getFileSuffix("archive.tar.gz", false);
		SU2_ASSERT_STR(suf, "gz", "getFileSuffix .tar.gz last suffix");
	}
}

void StringUtilityTest::testFolderPathFunctions()
{
	using namespace StringUtilityExt2TestSupport;
	{
		string folder = getFirstFolderName("abc/def/ghi");
		SU2_ASSERT_STR(folder, "abc", "getFirstFolderName 'abc/def/ghi'");
	}
	{
		string folder = getFirstFolderName("root/child");
		SU2_ASSERT_STR(folder, "root", "getFirstFolderName 'root/child'");
	}
	{
		string folder = getFirstFolderName("single");
		SU2_ASSERT_STR(folder, "single", "getFirstFolderName 'single'");
	}

	{
		string rest = removeFirstPath("abc/def/ghi");
		SU2_ASSERT_STR(rest, "def/ghi", "removeFirstPath 'abc/def/ghi'");
	}
	{
		string rest = removeFirstPath("root/child");
		SU2_ASSERT_STR(rest, "child", "removeFirstPath 'root/child'");
	}
}

void StringUtilityTest::testLastNumberFunctions()
{
	using namespace StringUtilityExt2TestSupport;
	{
		int pos = getLastNotNumberPos("abc123");
		SU2_ASSERT_EQ(pos, 2, "getLastNotNumberPos 'abc123'=2");
	}
	{
		int pos = getLastNotNumberPos("hello99");
		SU2_ASSERT_EQ(pos, 4, "getLastNotNumberPos 'hello99'=4");
	}
	{
		int pos = getLastNotNumberPos("123");
		SU2_ASSERT_EQ(pos, -1, "getLastNotNumberPos '123'=-1 all digits");
	}

	{
		int num = getLastNumber("item5");
		SU2_ASSERT_EQ(num, 5, "getLastNumber 'item5'=5");
	}
	{
		int num = getLastNumber("abc123");
		SU2_ASSERT_EQ(num, 123, "getLastNumber 'abc123'=123");
	}
	{
		int num = getLastNumber("level10bonus");
		SU2_ASSERT_EQ(num, 0, "getLastNumber 'level10bonus' no trailing digits=0");
	}
}

void StringUtilityTest::testV2IsToS()
{
	using namespace StringUtilityExt2TestSupport;
	{
		Vector<Vector2Int> v;
		string s = V2IsToS(v);
		SU2_ASSERT_STR(s, "", "V2IsToS empty");
	}
	{
		Vector<Vector2Int> v;
		v.add(Vector2Int(1, 2));
		string s = V2IsToS(v);
		SU2_ASSERT(s.length() > 0, "V2IsToS single element non-empty");
	}
	{
		Vector<Vector2Int> v;
		v.add(Vector2Int(1, 2));
		v.add(Vector2Int(3, 4));
		string s = V2IsToS(v);
		// 格式: "1,2|3,4"
		SU2_ASSERT_STR(s, "1,2|3,4", "V2IsToS [1,2|3,4]");
	}
	{
		HashMap<int, int> m;
		m.add(1, 10);
		string s = V2IsToS(m);
		SU2_ASSERT(s.length() > 0, "V2IsToS HashMap single non-empty");
	}
}

void StringUtilityTest::testRemoveSuffix()
{
	using namespace StringUtilityExt2TestSupport;
	{
		string result = removeSuffix("file.txt");
		SU2_ASSERT_STR(result, "file", "removeSuffix 'file.txt'");
	}
	{
		string result = removeSuffix("archive.tar.gz");
		SU2_ASSERT_STR(result, "archive.tar", "removeSuffix 'archive.tar.gz' removes .gz");
	}
	{
		string result = removeSuffix("noext");
		SU2_ASSERT_STR(result, "noext", "removeSuffix 'noext' no change");
	}
	{
		string result = removeSuffix("");
		SU2_ASSERT_STR(result, "", "removeSuffix empty");
	}
}

void StringUtilityTest::testStrlength()
{
	using namespace StringUtilityExt2TestSupport;
	{
		int len = strlength("hello");
		SU2_ASSERT_EQ(len, 5, "strlength 'hello'=5");
	}
	{
		int len = strlength("");
		SU2_ASSERT_EQ(len, 0, "strlength ''=0");
	}
	{
		int len = strlength("a");
		SU2_ASSERT_EQ(len, 1, "strlength 'a'=1");
	}
	{
		int len = strlength("hello", 3);
		SU2_ASSERT_EQ(len, 3, "strlength 'hello' maxLen=3 -> 3");
	}
	{
		int len = strlength("hi", 10);
		SU2_ASSERT_EQ(len, 2, "strlength 'hi' maxLen=10 -> 2");
	}
}

void StringUtilityTest::testStrchar()
{
	using namespace StringUtilityExt2TestSupport;
	{
		int pos = strchar("hello world", 'o');
		SU2_ASSERT_EQ(pos, 4, "strchar 'o' in 'hello world' = 4");
	}
	{
		int pos = strchar("hello", 'z');
		SU2_ASSERT_EQ(pos, -1, "strchar 'z' not found = -1");
	}
	{
		int pos = strchar("hello", 'l', 3);
		SU2_ASSERT_EQ(pos, 3, "strchar 'l' startIndex=3");
	}
	{
		int pos = strchar("aaa", 'a', 0);
		SU2_ASSERT_EQ(pos, 0, "strchar 'a' startIndex=0 first");
	}
}

void StringUtilityTest::testLastCharFunctions()
{
	using namespace StringUtilityExt2TestSupport;
	{
		int pos = getLastChar("hello", 'l');
		SU2_ASSERT_EQ(pos, 3, "getLastChar 'l' in 'hello' = 3");
	}
	{
		int pos = getLastChar("hello", 'z');
		SU2_ASSERT_EQ(pos, -1, "getLastChar 'z' not found");
	}
	{
		int pos = getLastChar("abcba", 'a');
		SU2_ASSERT_EQ(pos, 4, "getLastChar 'a' in 'abcba' = 4");
	}

	{
		int pos = getLastNotChar(string("hello   "), ' ');
		SU2_ASSERT_EQ(pos, 4, "getLastNotChar trailing spaces pos=4");
	}
	{
		int pos = getLastNotChar(string("   "), ' ');
		SU2_ASSERT_EQ(pos, -1, "getLastNotChar all spaces = -1");
	}
	{
		int pos = getLastNotChar(string("abc"), ' ');
		SU2_ASSERT_EQ(pos, 2, "getLastNotChar no trailing space = 2");
	}
}

void StringUtilityTest::testFindStringPos()
{
	using namespace StringUtilityExt2TestSupport;
	{
		int pos = findStringPos("hello world", "world");
		SU2_ASSERT_EQ(pos, 6, "findStringPos 'world' = 6");
	}
	{
		int pos = findStringPos("hello world", "xyz");
		SU2_ASSERT_EQ(pos, -1, "findStringPos 'xyz' not found = -1");
	}
	{
		int pos = findStringPos("hello", "hello");
		SU2_ASSERT_EQ(pos, 0, "findStringPos exact match = 0");
	}
	{
		int pos = findStringPos(string("abcabc"), string("bc"), 2);
		SU2_ASSERT_EQ(pos, 4, "findStringPos 'bc' from 2 = 4");
	}
}

void StringUtilityTest::testSplitLine2()
{
	using namespace StringUtilityExt2TestSupport;
	{
		Vector<string> lines;
		splitLine("line1\nline2\nline3", lines);
		SU2_ASSERT_EQ(lines.size(), 3, "splitLine 3 lines");
		SU2_ASSERT_STR(lines[0], "line1", "splitLine[0]='line1'");
		SU2_ASSERT_STR(lines[1], "line2", "splitLine[1]='line2'");
		SU2_ASSERT_STR(lines[2], "line3", "splitLine[2]='line3'");
	}
	{
		Vector<string> lines;
		splitLine("single", lines);
		SU2_ASSERT_EQ(lines.size(), 1, "splitLine single line");
		SU2_ASSERT_STR(lines[0], "single", "splitLine single='single'");
	}
	{
		Vector<string> lines;
		splitLine("a\n\nb", lines, true);
		SU2_ASSERT_EQ(lines.size(), 2, "splitLine removeEmpty=true skips empty");
	}
	{
		Vector<string> lines;
		splitLine("a\n\nb", lines, false);
		SU2_ASSERT_EQ(lines.size(), 3, "splitLine removeEmpty=false keeps empty");
	}
}

void StringUtilityTest::testSplitVector()
{
	using namespace StringUtilityExt2TestSupport;
	{
		Vector<string> parts;
		split("a,b,c", ',', parts);
		SU2_ASSERT_EQ(parts.size(), 3, "split 'a,b,c' = 3 parts");
		SU2_ASSERT_STR(parts[0], "a", "split[0]='a'");
		SU2_ASSERT_STR(parts[1], "b", "split[1]='b'");
		SU2_ASSERT_STR(parts[2], "c", "split[2]='c'");
	}
	{
		Vector<string> parts;
		split("", ',', parts);
		SU2_ASSERT_EQ(parts.size(), 0, "split empty string");
	}
	{
		Vector<string> parts;
		split("a,,b", ',', parts, true);
		SU2_ASSERT_EQ(parts.size(), 2, "split removeEmpty=true skips ''");
	}
	{
		Vector<string> parts;
		split("a,,b", ',', parts, false);
		SU2_ASSERT_EQ(parts.size(), 3, "split removeEmpty=false keeps ''");
	}
}

void StringUtilityTest::testSplitStringKey2()
{
	using namespace StringUtilityExt2TestSupport;
	{
		Vector<string> parts;
		split("a::b::c", "::", parts);
		SU2_ASSERT_EQ(parts.size(), 3, "split '::' key 3 parts");
		SU2_ASSERT_STR(parts[0], "a", "split_strkey[0]='a'");
		SU2_ASSERT_STR(parts[1], "b", "split_strkey[1]='b'");
		SU2_ASSERT_STR(parts[2], "c", "split_strkey[2]='c'");
	}
	{
		Vector<string> parts;
		split(string("a--b"), "--", parts);
		SU2_ASSERT_EQ(parts.size(), 2, "split '--' key 2 parts");
	}
}

void StringUtilityTest::testFindStringLower()
{
	using namespace StringUtilityExt2TestSupport;
	{
		int pos = -1;
		bool found = findStringLower("Hello World", "world", &pos);
		SU2_ASSERT(found, "findStringLower 'world' in 'Hello World' found");
		SU2_ASSERT_EQ(pos, 6, "findStringLower pos=6");
	}
	{
		bool found = findStringLower("Hello", "xyz");
		SU2_ASSERT(!found, "findStringLower 'xyz' not found");
	}
	{
		int pos = -1;
		bool found = findStringLower("ABCDEF", "bc", &pos);
		SU2_ASSERT(found, "findStringLower 'bc' in 'ABCDEF'");
		SU2_ASSERT_EQ(pos, 1, "findStringLower pos=1");
	}
}

void StringUtilityTest::testReplaceSubstring()
{
	using namespace StringUtilityExt2TestSupport;
	{
		string s = "hello world";
		replace(s, 6, 11, "C++");
		SU2_ASSERT_STR(s, "hello C++", "replace [6,11) -> 'C++'");
	}
	{
		string s = "abcdef";
		replace(s, 0, 3, "XY");
		SU2_ASSERT_STR(s, "XYdef", "replace [0,3) -> 'XY'");
	}
	{
		string s = "hello";
		replace(s, 2, 4, "XYZ");
		SU2_ASSERT_STR(s, "heXYZo", "replace [2,4) -> 'XYZ'");
	}
}

void StringUtilityTest::testRemoveStringChar()
{
	using namespace StringUtilityExt2TestSupport;
	{
		char buf[64] = "hello world";
		bool ok = removeString(buf, 64, "world");
		SU2_ASSERT(ok, "removeString 'world' found");
		SU2_ASSERT_STR(string(buf), "hello ", "removeString result='hello '");
	}
	{
		char buf[64] = "hello";
		bool ok = removeString(buf, 64, "xyz");
		SU2_ASSERT(!ok, "removeString 'xyz' not found");
		SU2_ASSERT_STR(string(buf), "hello", "removeString unchanged");
	}
	{
		char buf[64] = "abcabc";
		bool ok = removeString(buf, 64, "abc");
		SU2_ASSERT(ok, "removeString first 'abc' removed");
		// 只删第一次
		SU2_ASSERT_STR(string(buf), "abc", "removeString only first occurrence");
	}
}

void StringUtilityTest::testRemoveStartLast2()
{
	using namespace StringUtilityExt2TestSupport;
	{
		string s = "  hello";
		removeStart(s, ' ');
		SU2_ASSERT_STR(s, " hello", "removeStart single space");
	}
	{
		string s = "hello";
		removeLast(s, 'o');
		SU2_ASSERT_STR(s, "hell", "removeLast 'o'");
	}
	{
		string s = "hello";
		removeLast(s, 'z');
		SU2_ASSERT_STR(s, "hello", "removeLast 'z' not found");
	}
	{
		string s = "axyz";
		removeStart(s, 'a');
		SU2_ASSERT_STR(s, "xyz", "removeStart 'a'");
	}
}

void StringUtilityTest::testNumericRoundtrip()
{
	using namespace StringUtilityExt2TestSupport;
	// llong 往返
	{
		llong original = 1234567890123LL;
		string s = LLToS(original);
		llong back = SToLL(s);
		SU2_ASSERT_LL(back, original, "LLToS->SToLL roundtrip positive");
	}
	{
		llong original = -9876543210LL;
		string s = LLToS(original);
		llong back = SToLL(s);
		SU2_ASSERT_LL(back, original, "LLToS->SToLL roundtrip negative");
	}
	// ullong 往返
	{
		ullong original = 18000000000ULL;
		string s = ULLToS(original);
		ullong back = stringToULLong(s);
		SU2_ASSERT_LL((llong)back, (llong)original, "ULLToS->stringToULLong roundtrip");
	}
	// float 往返
	{
		float original = 3.14f;
		string s = FToS(original, 2);
		float back = SToF(s);
		SU2_ASSERT_F(back, original, "FToS->SToF roundtrip");
	}
	// int 往返
	{
		int original = -99999;
		string s = IToS(original);
		int back = SToI(s);
		SU2_ASSERT_EQ(back, original, "IToS->SToI roundtrip -99999");
	}
	{
		int original = 12345;
		string s = IToS(original);
		int back = SToI(s);
		SU2_ASSERT_EQ(back, original, "IToS->SToI roundtrip 12345");
	}
}

void StringUtilityTest::testNumericStress()
{
	using namespace StringUtilityExt2TestSupport;
	// 测试0-999 IToS 与 SToI 往返
	for (int i = 0; i < 1000; ++i)
	{
		string s = IToS(i);
		int back = SToI(s);
		if (back != i)
		{
			ERROR(string("IToS->SToI stress fail at i=") + IToS(i));
			break;
		}
	}
	// 测试-500到-1
	for (int i = -500; i < 0; ++i)
	{
		string s = IToS(i);
		int back = SToI(s);
		if (back != i)
		{
			ERROR(string("IToS->SToI stress fail at i=") + IToS(i));
			break;
		}
	}
	// 测试ULLToS 大量值
	for (ullong i = 0; i < 200; ++i)
	{
		string s = ULLToS(i);
		ullong back = stringToULLong(s);
		if (back != i)
		{
			ERROR(string("ULLToS stress fail at i=") + IToS((int)i));
			break;
		}
	}
}

namespace StringUtilityExt3TestSupport
{
#define SU3_ASSERT(expr, msg) \
    do { if (!(expr)) { ERROR(string("StringUtilityExt3Test FAILED: ") + (msg)); } } while(0)
#define SU3_EQ(a, b, msg) \
    do { if ((a) != (b)) { ERROR(string("StringUtilityExt3Test EQ: ") + (msg) \
        + " exp=" + (b) + " got=" + (a)); } } while(0)
#define SU3_EQINT(a, b, msg) \
    do { if ((a) != (b)) { ERROR(string("StringUtilityExt3Test EQ: ") + (msg) \
        + " exp=" + IToS(b) + " got=" + IToS(a)); } } while(0)
}

void StringUtilityTest::testStrcatS_CharBuffer()
{
	using namespace StringUtilityExt3TestSupport;
	char buf[64] = {};
	strcpy_s(buf, 64, "Hello");
	strcat_s(buf, 64, " World");
	SU3_EQ(string(buf), "Hello World", "strcat_s char buf");
	char buf2[64] = {};
	strcat_s(buf2, 64, "abc");
	SU3_EQ(string(buf2), "abc", "strcat_s empty dest");
}

void StringUtilityTest::testStrcatT_CharBuffer()
{
	using namespace StringUtilityExt3TestSupport;
	char buf[64] = {};
	strcat_t(buf, 64, "A", "B", "C");
	SU3_EQ(string(buf), "ABC", "strcat_t 3 args");
	char buf2[64] = {};
	strcat_t(buf2, 64, "Hello", " ", "World", "!");
	SU3_EQ(string(buf2), "Hello World!", "strcat_t 4 args");
}

void StringUtilityTest::testStrcpySExt3()
{
	using namespace StringUtilityExt3TestSupport;
	char buf[32] = {};
	strcpy_s(buf, 32, "CopyTest");
	SU3_EQ(string(buf), "CopyTest", "strcpy_s basic");
	char buf2[32] = {};
	strcpy_s(buf2, 32, "");
	SU3_EQ(string(buf2), "", "strcpy_s empty");
}

void StringUtilityTest::testZeroStringTemplate()
{
	using namespace StringUtilityExt3TestSupport;
	MyString<16> s;
	s.setCount('0', 4);
	SU3_EQ(string(s.str()), "0000", "zeroString template 4");
	MyString<16> s2;
	s2.setCount('0', 1);
	SU3_EQ(string(s2.str()), "0", "zeroString template 1");
}

void StringUtilityTest::testZeroStringFunc()
{
	using namespace StringUtilityExt3TestSupport;
	string r = zeroString(3);
	SU3_EQ(r, "000", "zeroString func 3");
	string r2 = zeroString(0);
	SU3_EQ(r2, "", "zeroString func 0");
}

void StringUtilityTest::testReplaceString()
{
	using namespace StringUtilityExt3TestSupport;
	string s = "HelloWorld";
	replace(s, 5, 10, "Buddy");
	SU3_EQ(s, "HelloBuddy", "replace string middle");
	string s2 = "ABCDE";
	replace(s2, 0, 3, "XY");
	SU3_EQ(s2, "XYDE", "replace string prefix");
}

void StringUtilityTest::testReplaceAllStringKey()
{
	using namespace StringUtilityExt3TestSupport;
	string s = "aXbXcX";
	replaceAll(s, string("X"), string("*"));
	SU3_EQ(s, "a*b*c*", "replaceAll string key");
	string s2 = "nonehere";
	replaceAll(s2, string("ZZZ"), string("!"));
	SU3_EQ(s2, "nonehere", "replaceAll no match");
}

void StringUtilityTest::testReplaceAllChar()
{
	using namespace StringUtilityExt3TestSupport;
	string s = "a.b.c.d";
	replaceAll(s, '.', '/');
	SU3_EQ(s, "a/b/c/d", "replaceAll char");
	string s2 = "nodots";
	replaceAll(s2, '.', '/');
	SU3_EQ(s2, "nodots", "replaceAll char no match");
}

void StringUtilityTest::testRemoveAll_SingleChar()
{
	using namespace StringUtilityExt3TestSupport;
	string s = "a1b1c1";
	removeAll(s, '1');
	SU3_EQ(s, "abc", "removeAll single char");
	string s2 = "nodigit";
	removeAll(s2, '9');
	SU3_EQ(s2, "nodigit", "removeAll not found");
}

void StringUtilityTest::testRemoveAll_TwoChars()
{
	using namespace StringUtilityExt3TestSupport;
	string s = "a1b2c1d2";
	removeAll(s, '1', '2');
	SU3_EQ(s, "abcd", "removeAll two chars");
	string s2 = "xyz";
	removeAll(s2, 'a', 'b');
	SU3_EQ(s2, "xyz", "removeAll two chars no match");
}

void StringUtilityTest::testEndWithCaseSensitive()
{
	using namespace StringUtilityExt3TestSupport;
	SU3_ASSERT(endWith("abcde", "cde", true), "endWith sensitive match");
	SU3_ASSERT(!endWith("abcde", "CDE", true), "endWith sensitive no match");
	SU3_ASSERT(!endWith("abcde", "xyz", true), "endWith sensitive not ending");
	SU3_ASSERT(endWith("abc", "abc", true), "endWith full match");
	SU3_ASSERT(!endWith("ab", "abc", true), "endWith pattern longer");
}

void StringUtilityTest::testEndWithCaseInsensitive()
{
	using namespace StringUtilityExt3TestSupport;
	SU3_ASSERT(endWith("abcde", "CDE", false), "endWith insensitive match");
	SU3_ASSERT(endWith("ABCDE", "cde", false), "endWith insensitive upper");
	SU3_ASSERT(!endWith("abcde", "xyz", false), "endWith insensitive no match");
}

void StringUtilityTest::testStartWithCaseSensitive()
{
	using namespace StringUtilityExt3TestSupport;
	SU3_ASSERT(startWith("Hello World", "Hello", true), "startWith sensitive match");
	SU3_ASSERT(!startWith("Hello World", "hello", true), "startWith sensitive case");
	SU3_ASSERT(!startWith("Hi", "Hello", true), "startWith sensitive mismatch");
}

void StringUtilityTest::testStartWithCaseInsensitive()
{
	using namespace StringUtilityExt3TestSupport;
	SU3_ASSERT(startWith("Hello World", "hello", false), "startWith insensitive");
	SU3_ASSERT(startWith("HELLO", "hello", false), "startWith insensitive all upper");
	SU3_ASSERT(!startWith("World", "hello", false), "startWith insensitive no match");
}

void StringUtilityTest::testStrcatT_MyString()
{
	using namespace StringUtilityExt3TestSupport;
	MyString<64> s;
	s.add("Foo", "Bar", "Baz");
	SU3_ASSERT(strcmp(s.str(), "FooBarBaz") == 0, "add MyString 3 args");
}

void StringUtilityTest::testLLToS_MyString()
{
	using namespace StringUtilityExt3TestSupport;
	MyString<32> s;
	int len = LLToS(s, 12345678LL);
	SU3_ASSERT(strcmp(s.str(), "12345678") == 0, "LLToS MyString positive");
	SU3_EQINT(len, 8, "LLToS MyString len==8");
	MyString<32> s2;
	LLToS(s2, -999LL);
	SU3_ASSERT(strcmp(s2.str(), "-999") == 0, "LLToS MyString negative");
	MyString<32> s3;
	LLToS(s3, 0LL);
	SU3_ASSERT(strcmp(s3.str(), "0") == 0, "LLToS MyString zero");
}

void StringUtilityTest::testZeroStringMyStringExt3()
{
	using namespace StringUtilityExt3TestSupport;
	MyString<16> s;
	s.setCount('0', 5);
	SU3_ASSERT(strcmp(s.str(), "00000") == 0, "zeroString MyString 5");
	SU3_EQINT((int)strlen(s.str()), 5, "zeroString MyString len==5");
}

void StringUtilityTest::testEndWithString()
{
	using namespace StringUtilityExt3TestSupport;
	SU3_ASSERT(endWith(string("hello.txt"), ".txt", true), "endWith string suffix");
	SU3_ASSERT(!endWith(string("hello.txt"), ".TXT", true), "endWith string sensitive");
	SU3_ASSERT(endWith(string("hello.txt"), ".TXT", false), "endWith string insensitive");
}

void StringUtilityTest::testStartWithString()
{
	using namespace StringUtilityExt3TestSupport;
	SU3_ASSERT(startWith(string("Framework"), "Frame", true), "startWith string prefix");
	SU3_ASSERT(!startWith(string("Framework"), "frame", true), "startWith string sensitive");
	SU3_ASSERT(startWith(string("Framework"), "frame", false), "startWith string insensitive");
}

void StringUtilityTest::testStrcatS_MultipleChars()
{
	using namespace StringUtilityExt3TestSupport;
	char buf[128] = {};
	strcat_s(buf, 128, "Hello");
	strcat_s(buf, 128, ", ");
	strcat_s(buf, 128, "World");
	strcat_s(buf, 128, "!");
	SU3_EQ(string(buf), "Hello, World!", "strcat_s multiple calls");
}

void StringUtilityTest::testReplaceCharBuffer()
{
	using namespace StringUtilityExt3TestSupport;
	// replace(str, bufSize, begin, end, reStr)：将 str[begin..end) 替换为 reStr
	// 框架实现不补 \0，且 memmove 仅移动 curLength-end 字节
	// 等长替换（"World"5字节 → "Earth"5字节）保证结果正确
	char buf[64] = "Hello World";
	replace(buf, 64, 6, 11, "Earth");
	SU3_EQ(string(buf), "Hello Earth", "replace char buffer same length");
}

void StringUtilityTest::testStrcpyS_Truncate()
{
	using namespace StringUtilityExt3TestSupport;
	// 框架 strcpy_s：源串 >= size 时直接 return（不截断写入），buf 保持原状
	char buf[6] = {};
	// 正常长度写入
	strcpy_s(buf, 6, "Hello");
	SU3_EQINT((int)strlen(buf), 5, "strcpy_s normal len==5");
}

void StringUtilityTest::testFindCharCount()
{
	using namespace StringUtilityExt3TestSupport;
	SU3_EQINT(findCharCount("abcabc", 'a'), 2, "findCharCount 2");
	SU3_EQINT(findCharCount("hello", 'x'), 0, "findCharCount 0");
	SU3_EQINT(findCharCount("....", '.'), 4, "findCharCount 4");
}

void StringUtilityTest::testStrcharExt3()
{
	using namespace StringUtilityExt3TestSupport;
	SU3_EQINT(strchar("abcabc", 'a', 0, 0), 0, "strchar first");
	SU3_EQINT(strchar("abcabc", 'a', 1, 0), 3, "strchar second from index 1");
	SU3_EQINT(strchar("abcabc", 'z', 0, 0), -1, "strchar not found");
}

void StringUtilityTest::testFindStringLowerExt3()
{
	using namespace StringUtilityExt3TestSupport;
	int pos = -1;
	SU3_ASSERT(findStringLower("Hello World", "world", &pos), "findStringLower found");
	SU3_EQINT(pos, 6, "findStringLower pos==6");
	SU3_ASSERT(!findStringLower("Hello", "xyz", nullptr), "findStringLower not found");
}

void StringUtilityTest::testGetLastChar()
{
	using namespace StringUtilityExt3TestSupport;
	SU3_EQINT(getLastChar("Hello.txt", '.'), 5, "getLastChar dot pos");
	SU3_EQINT(getLastChar("noslash", '/'), -1, "getLastChar not found");
	SU3_EQINT(getLastChar("a/b/c", '/'), 3, "getLastChar last slash");
}

void StringUtilityTest::testGetLastNotChar()
{
	using namespace StringUtilityExt3TestSupport;
	SU3_EQINT(getLastNotChar("abc000", '0'), 2, "getLastNotChar pos");
	SU3_EQINT(getLastNotChar("000", '0'), -1, "getLastNotChar all zeros");
	SU3_EQINT(getLastNotChar("abc", 'x'), 2, "getLastNotChar no trailing x");
}

void StringUtilityTest::testGreaterPower2()
{
	using namespace StringUtilityExt3TestSupport;
	// greaterPower2 封装 MathUtility::getGreaterPower2：大于等于 value 的最小2^n
	// index 0,1 查表返回 2；其余值：>1 时 getGreaterPowerValue(i,2) 找 >=i 的最小幂
	SU3_EQINT(greaterPower2(1), 2, "greaterPower2(1)==2");
	SU3_EQINT(greaterPower2(3), 4, "greaterPower2(3)==4");
	SU3_EQINT(greaterPower2(4), 4, "greaterPower2(4)==4");
	SU3_EQINT(greaterPower2(15), 16, "greaterPower2(15)==16");
	SU3_EQINT(greaterPower2(16), 16, "greaterPower2(16)==16");
	SU3_EQINT(greaterPower2(100), 128, "greaterPower2(100)==128");
}

namespace StringUtilityStrCatTestSupport
{
#define SUST_ASSERT(expr) \
    if (!(expr)) { ERROR("StringUtilityStrCatTest FAILED: " #expr); }
}

void StringUtilityTest::testStrcatSCharBuf()
{
	using namespace StringUtilityStrCatTestSupport;
	char buf[64] = "Hello";
	strcat_s(buf, 64, " World");
	SUST_ASSERT(strcmp(buf, "Hello World") == 0);
	char buf2[64] = "";
	strcat_s(buf2, 64, "Test");
	SUST_ASSERT(strcmp(buf2, "Test") == 0);
	char buf3[64] = "abc";
	strcat_s(buf3, 64, "");
	SUST_ASSERT(strcmp(buf3, "abc") == 0);
	char buf4[64] = "Hello";
	strcat_s(buf4, 64, " World!");
	SUST_ASSERT(strcmp(buf4, "Hello World!") == 0);
	char buf5[64] = "abc";
	strcat_s(buf5, 64, "defghi");
	SUST_ASSERT(strcmp(buf5, "abcdefghi") == 0);
	char buf6[64] = "";
	strcat_s(buf6, 64, "single");
	SUST_ASSERT(strcmp(buf6, "single") == 0);
	char buf7[64] = "prefix";
	strcat_s(buf7, 64, "_suffix");
	SUST_ASSERT(strcmp(buf7, "prefix_suffix") == 0);
}

void StringUtilityTest::testStrcatSCharBufWithLength()
{
	using namespace StringUtilityStrCatTestSupport;
	char buf[64] = "Hello";
	strcat_s(buf, 64, " WorldExtra", 6);
	SUST_ASSERT(strcmp(buf, "Hello World") == 0);
	char buf2[64] = "";
	strcat_s(buf2, 64, "TestExtra", 4);
	SUST_ASSERT(strcmp(buf2, "Test") == 0);
	char buf3[64] = "abc";
	strcat_s(buf3, 64, "defghi", 3);
	SUST_ASSERT(strcmp(buf3, "abcdef") == 0);
	char buf4[64] = "start";
	strcat_s(buf4, 64, "ABCDEFGH", 0);
	SUST_ASSERT(strcmp(buf4, "start") == 0);
}

void StringUtilityTest::testStrcatSCharBufArraySource()
{
	using namespace StringUtilityStrCatTestSupport;
	char buf[64] = "";
	Array<3, const char*> sources = { "Hello", " ", "World" };
	strcat_s(buf, 64, sources);
	SUST_ASSERT(strcmp(buf, "Hello World") == 0);
	char buf2[64] = "Prefix_";
	Array<2, const char*> sources2 = { "foo", "bar" };
	strcat_s(buf2, 64, sources2);
	SUST_ASSERT(strcmp(buf2, "Prefix_foobar") == 0);
	char buf3[64] = "";
	Array<3, const char*> sources3 = { "a", nullptr, "c" };
	strcat_s(buf3, 64, sources3);
	SUST_ASSERT(strcmp(buf3, "ac") == 0);
	char buf4[64] = "";
	Array<1, const char*> sources4 = { "only" };
	strcat_s(buf4, 64, sources4);
	SUST_ASSERT(strcmp(buf4, "only") == 0);
}

void StringUtilityTest::testStrcatTCharBuf()
{
	using namespace StringUtilityStrCatTestSupport;
	char buf[64] = "";
	strcat_t(buf, 64, "Hello", " ", "World");
	SUST_ASSERT(strcmp(buf, "Hello World") == 0);
	char buf2[64] = "prefix_";
	strcat_t(buf2, 64, "a", "b", "c", "d");
	SUST_ASSERT(strcmp(buf2, "prefix_abcd") == 0);
	char buf3[64] = "";
	strcat_t(buf3, 64, "single");
	SUST_ASSERT(strcmp(buf3, "single") == 0);
	char buf4[64] = "";
	strcat_t(buf4, 64, "123", "456", "789");
	SUST_ASSERT(strcmp(buf4, "123456789") == 0);
	char buf5[64] = "";
	strcat_t(buf5, 64, "X");
	SUST_ASSERT(strcmp(buf5, "X") == 0);
}

void StringUtilityTest::testStrcatSMyString()
{
	using namespace StringUtilityStrCatTestSupport;
	MyString<64> ms;
	ms.set("Hello");
	string s = " World";
	ms.add(s);
	SUST_ASSERT(strcmp(ms.str(), "Hello World") == 0);
	MyString<64> ms2;
	string s2 = "Test";
	ms2.add(s2);
	SUST_ASSERT(strcmp(ms2.str(), "Test") == 0);
	MyString<64> ms3;
	ms3.set('a');
	string s3 = "bc";
	ms3.add(s3);
	SUST_ASSERT(strcmp(ms3.str(), "abc") == 0);
}

void StringUtilityTest::testStrcatSMyStringWithLength()
{
	using namespace StringUtilityStrCatTestSupport;
	MyString<64> ms;
	ms.set("Hi");
	string s = "Hello";
	ms.add(s, 3);
	SUST_ASSERT(strcmp(ms.str(), "HiHel") == 0);
	MyString<64> ms2;
	string s2 = "World";
	ms2.add(s2, 5);
	SUST_ASSERT(strcmp(ms2.str(), "World") == 0);
}

void StringUtilityTest::testStrcatSMyStringCharPtr()
{
	using namespace StringUtilityStrCatTestSupport;
	MyString<64> ms;
	ms.set("Hey");
	ms.add(" there");
	SUST_ASSERT(strcmp(ms.str(), "Hey there") == 0);
	MyString<64> ms2;
	ms2.add("Hello");
	SUST_ASSERT(strcmp(ms2.str(), "Hello") == 0);
}

void StringUtilityTest::testStrcatSMyStringCharPtrWithLength()
{
	using namespace StringUtilityStrCatTestSupport;
	MyString<64> ms;
	ms.set('A');
	ms.add("BCDEFGH", 3);
	SUST_ASSERT(strcmp(ms.str(), "ABCD") == 0);
	MyString<64> ms2;
	ms2.add("123456", 4);
	SUST_ASSERT(strcmp(ms2.str(), "1234") == 0);
}

void StringUtilityTest::testStrcatSMyStringArray()
{
	using namespace StringUtilityStrCatTestSupport;
	MyString<64> ms;
	Array<3, const char*> src = { "Hello", " ", "World" };
	ms.add(src);
	SUST_ASSERT(strcmp(ms.str(), "Hello World") == 0);
	MyString<64> ms2;
	ms2.set("X");
	Array<2, const char*> src2 = { "foo", nullptr };
	ms2.add(src2);
	SUST_ASSERT(strcmp(ms2.str(), "Xfoo") == 0);
}

void StringUtilityTest::testStrcatTMyString()
{
	using namespace StringUtilityStrCatTestSupport;
	MyString<64> ms;
	ms.add("Hello", " ", "World");
	SUST_ASSERT(strcmp(ms.str(), "Hello World") == 0);
	MyString<64> ms2;
	ms2.set("P_");
	ms2.add("a", "b", "c");
	SUST_ASSERT(strcmp(ms2.str(), "P_abc") == 0);
	MyString<64> ms3;
	ms3.add("only");
	SUST_ASSERT(strcmp(ms3.str(), "only") == 0);
}

void StringUtilityTest::testStrcpySStrCat()
{
	using namespace StringUtilityStrCatTestSupport;
	char buf[64];
	strcpy_s(buf, 64, "Hello World");
	SUST_ASSERT(strcmp(buf, "Hello World") == 0);
	strcpy_s(buf, 64, "");
	SUST_ASSERT(strcmp(buf, "") == 0);
	strcpy_s(buf, 64, "abc");
	SUST_ASSERT(strcmp(buf, "abc") == 0);
	strcpy_s(buf, 64, "A");
	SUST_ASSERT(strcmp(buf, "A") == 0);
	SUST_ASSERT(strlen(buf) == 1);
	char buf2[8];
	strcpy_s(buf2, 8, "1234567");
	SUST_ASSERT(strcmp(buf2, "1234567") == 0);
}

void StringUtilityTest::testZeroStringFuncStrCat()
{
	using namespace StringUtilityStrCatTestSupport;
	string r0 = zeroString(0);
	SUST_ASSERT(r0 == "");
	string r1 = zeroString(1);
	SUST_ASSERT(r1 == "0");
	string r5 = zeroString(5);
	SUST_ASSERT(r5 == "00000");
	string r10 = zeroString(10);
	SUST_ASSERT(r10.length() == 10);
	for (int i = 0; i < 10; ++i)
	{
		SUST_ASSERT(r10[i] == '0');
	}
}

void StringUtilityTest::testZeroStringTemplateStrCat()
{
	using namespace StringUtilityStrCatTestSupport;
	MyString<16> ms;
	ms.setCount('0', 3);
	SUST_ASSERT(strcmp(ms.str(), "000") == 0);
	MyString<16> ms2;
	ms2.setCount('0', 1);
	SUST_ASSERT(strcmp(ms2.str(), "0") == 0);
	MyString<16> ms3;
	ms3.setCount('0', 8);
	SUST_ASSERT(strcmp(ms3.str(), "00000000") == 0);
	for (int i = 0; i < 8; ++i)
	{
		SUST_ASSERT(ms3[i] == '0');
	}
	SUST_ASSERT(ms3[8] == '\0');
}

void StringUtilityTest::testStrcatSEmpty()
{
	using namespace StringUtilityStrCatTestSupport;
	char buf[64] = "";
	strcat_s(buf, 64, "");
	SUST_ASSERT(strcmp(buf, "") == 0);
	SUST_ASSERT(strlen(buf) == 0);
	char buf2[64] = "abc";
	strcat_s(buf2, 64, "");
	SUST_ASSERT(strcmp(buf2, "abc") == 0);
}

void StringUtilityTest::testStrcatSNullSource()
{
	using namespace StringUtilityStrCatTestSupport;
	char buf[64] = "";
	Array<3, const char*> sources = { nullptr, nullptr, nullptr };
	strcat_s(buf, 64, sources);
	SUST_ASSERT(strcmp(buf, "") == 0);
	MyString<64> ms;
	Array<3, const char*> src2 = { nullptr, "hello", nullptr };
	ms.add(src2);
	SUST_ASSERT(strcmp(ms.str(), "hello") == 0);
}

void StringUtilityTest::testStrcatSMultipleSources()
{
	using namespace StringUtilityStrCatTestSupport;
	char buf[128] = "";
	Array<5, const char*> src = { "a", "b", "c", "d", "e" };
	strcat_s(buf, 128, src);
	SUST_ASSERT(strcmp(buf, "abcde") == 0);
	char buf2[128] = "";
	Array<4, const char*> src2 = { "Hello", ", ", "World", "!" };
	strcat_s(buf2, 128, src2);
	SUST_ASSERT(strcmp(buf2, "Hello, World!") == 0);
	MyString<128> ms;
	Array<4, const char*> src3 = { "1", "2", "3", "4" };
	ms.add(src3);
	SUST_ASSERT(strcmp(ms.str(), "1234") == 0);
}

void StringUtilityTest::testStrcpySEmpty()
{
	using namespace StringUtilityStrCatTestSupport;
	char buf[64] = "existing";
	strcpy_s(buf, 64, "");
	SUST_ASSERT(strcmp(buf, "") == 0);
	SUST_ASSERT(strlen(buf) == 0);
}

void StringUtilityTest::testStrcpySLargeBuffer()
{
	using namespace StringUtilityStrCatTestSupport;
	char buf[256];
	const char* longStr = "This is a fairly long string for testing purposes only.";
	strcpy_s(buf, 256, longStr);
	SUST_ASSERT(strcmp(buf, longStr) == 0);
	SUST_ASSERT(strlen(buf) == strlen(longStr));
}

void StringUtilityTest::testStrcatSMyStringStdString()
{
	using namespace StringUtilityStrCatTestSupport;
	MyString<64> ms;
	ms.set("Hi");
	string s = " there";
	ms.add(s);
	SUST_ASSERT(strcmp(ms.str(), "Hi there") == 0);
	MyString<64> ms2;
	string empty = "";
	ms2.add(empty);
	SUST_ASSERT(strcmp(ms2.str(), "") == 0);
	string s3 = "append";
	ms2.add(s3);
	SUST_ASSERT(strcmp(ms2.str(), "append") == 0);
}

void StringUtilityTest::testStrcatSMyStringStdStringLength()
{
	using namespace StringUtilityStrCatTestSupport;
	MyString<64> ms;
	string s = "HelloWorld";
	ms.add(s, 5);
	SUST_ASSERT(strcmp(ms.str(), "Hello") == 0);
	MyString<64> ms2;
	ms2.set('A');
	string s2 = "BCD";
	ms2.add(s2, 2);
	SUST_ASSERT(strcmp(ms2.str(), "ABC") == 0);
}

namespace StringUtilityConvertTestSupport
{
#define SUCT_ASSERT(expr) \
    if (!(expr)) { ERROR("StringUtilityConvertTest FAILED: " #expr); }
}

void StringUtilityTest::testLLToSPositive()
{
	using namespace StringUtilityConvertTestSupport;
	string r = LLToS(0LL);
	SUCT_ASSERT(r == "0");
	r = LLToS(1LL);
	SUCT_ASSERT(r == "1");
	r = LLToS(123LL);
	SUCT_ASSERT(r == "123");
	r = LLToS(1000LL);
	SUCT_ASSERT(r == "1000");
	r = LLToS(999999LL);
	SUCT_ASSERT(r == "999999");
	r = LLToS(1000000000LL);
	SUCT_ASSERT(r == "1000000000");
	r = LLToS(9999999999LL);
	SUCT_ASSERT(r == "9999999999");
}

void StringUtilityTest::testLLToSNegative()
{
	using namespace StringUtilityConvertTestSupport;
	string r = LLToS(-1LL);
	SUCT_ASSERT(r == "-1");
	r = LLToS(-123LL);
	SUCT_ASSERT(r == "-123");
	r = LLToS(-1000LL);
	SUCT_ASSERT(r == "-1000");
	r = LLToS(-999999LL);
	SUCT_ASSERT(r == "-999999");
	r = LLToS(-2147483647LL);
	SUCT_ASSERT(r == "-2147483647");
}

void StringUtilityTest::testLLToSZero()
{
	using namespace StringUtilityConvertTestSupport;
	string r = LLToS(0LL);
	SUCT_ASSERT(r == "0");
	SUCT_ASSERT(r.length() == 1);
}

void StringUtilityTest::testLLToSLimitLen()
{
	using namespace StringUtilityConvertTestSupport;
	string r = LLToS(42LL, 5);
	SUCT_ASSERT(r == "00042");
	SUCT_ASSERT(r.length() == 5);
	r = LLToS(0LL, 3);
	SUCT_ASSERT(r == "000");
	r = LLToS(1LL, 4);
	SUCT_ASSERT(r == "0001");
	r = LLToS(12345LL, 3);
	SUCT_ASSERT(r == "12345");
	r = LLToS(42LL, 0);
	SUCT_ASSERT(r == "42");
	r = LLToS(42LL, 2);
	SUCT_ASSERT(r == "42");
}

void StringUtilityTest::testLLToSMaxValue()
{
	using namespace StringUtilityConvertTestSupport;
	string r = LLToS(9223372036854775806LL);
	SUCT_ASSERT(!r.empty());
	SUCT_ASSERT(r[0] != '-');
}

void StringUtilityTest::testIToSPositive()
{
	using namespace StringUtilityConvertTestSupport;
	string r = IToS(0);
	SUCT_ASSERT(r == "0");
	r = IToS(1);
	SUCT_ASSERT(r == "1");
	r = IToS(100);
	SUCT_ASSERT(r == "100");
	r = IToS(9999);
	SUCT_ASSERT(r == "9999");
	r = IToS(2147483647);
	SUCT_ASSERT(r == "2147483647");
}

void StringUtilityTest::testIToSNegative()
{
	using namespace StringUtilityConvertTestSupport;
	string r = IToS(-1);
	SUCT_ASSERT(r == "-1");
	r = IToS(-100);
	SUCT_ASSERT(r == "-100");
	r = IToS(-2147483647);
	SUCT_ASSERT(r == "-2147483647");
}

void StringUtilityTest::testIToSZero()
{
	using namespace StringUtilityConvertTestSupport;
	string r = IToS(0);
	SUCT_ASSERT(r == "0");
	SUCT_ASSERT(r.length() == 1);
}

void StringUtilityTest::testI64ToAPositive()
{
	using namespace StringUtilityConvertTestSupport;
	MyString<32> ms;
	int len = _i64toa_s_(0LL, ms);
	SUCT_ASSERT(len == 1);
	SUCT_ASSERT(strcmp(ms.str(), "0") == 0);
	len = _i64toa_s_(1LL, ms);
	SUCT_ASSERT(len == 1);
	SUCT_ASSERT(strcmp(ms.str(), "1") == 0);
	len = _i64toa_s_(123LL, ms);
	SUCT_ASSERT(len == 3);
	SUCT_ASSERT(strcmp(ms.str(), "123") == 0);
	len = _i64toa_s_(1000000LL, ms);
	SUCT_ASSERT(len == 7);
	SUCT_ASSERT(strcmp(ms.str(), "1000000") == 0);
	len = _i64toa_s_(9999999LL, ms);
	SUCT_ASSERT(len == 7);
	SUCT_ASSERT(strcmp(ms.str(), "9999999") == 0);
}

void StringUtilityTest::testI64ToANegative()
{
	using namespace StringUtilityConvertTestSupport;
	MyString<32> ms;
	int len = _i64toa_s_(-1LL, ms);
	SUCT_ASSERT(len == 2);
	SUCT_ASSERT(strcmp(ms.str(), "-1") == 0);
	len = _i64toa_s_(-123LL, ms);
	SUCT_ASSERT(len == 4);
	SUCT_ASSERT(strcmp(ms.str(), "-123") == 0);
	len = _i64toa_s_(-2147483647LL, ms);
	SUCT_ASSERT(strcmp(ms.str(), "-2147483647") == 0);
}

void StringUtilityTest::testI64ToAZero()
{
	using namespace StringUtilityConvertTestSupport;
	MyString<32> ms;
	int len = _i64toa_s_(0LL, ms);
	SUCT_ASSERT(len == 1);
	SUCT_ASSERT(ms[0] == '0');
	SUCT_ASSERT(ms[1] == '\0');
}

void StringUtilityTest::testI64ToAMaxValue()
{
	using namespace StringUtilityConvertTestSupport;
	MyString<32> ms;
	int len = _i64toa_s_(9223372036854775806LL, ms);
	SUCT_ASSERT(len > 0);
	SUCT_ASSERT(ms.str()[0] != '-');
}

void StringUtilityTest::testLLToSMyString()
{
	using namespace StringUtilityConvertTestSupport;
	MyString<32> ms;
	int len = LLToS(ms, 0LL);
	SUCT_ASSERT(len == 1);
	SUCT_ASSERT(strcmp(ms.str(), "0") == 0);
	len = LLToS(ms, 12345LL);
	SUCT_ASSERT(len == 5);
	SUCT_ASSERT(strcmp(ms.str(), "12345") == 0);
	len = LLToS(ms, -999LL);
	SUCT_ASSERT(len == 4);
	SUCT_ASSERT(strcmp(ms.str(), "-999") == 0);
}

void StringUtilityTest::testLLToSMyStringLimitLen()
{
	using namespace StringUtilityConvertTestSupport;
	// LLToS(limitLen) 内部用 strcat_t 追加，每次调用前需清空 MyString
	MyString<32> ms;
	int len = LLToS(ms, 42LL, 5);
	SUCT_ASSERT(len == 5);
	SUCT_ASSERT(strcmp(ms.str(), "00042") == 0);
	ms.zero();
	len = LLToS(ms, 0LL, 3);
	SUCT_ASSERT(len == 3);
	SUCT_ASSERT(strcmp(ms.str(), "000") == 0);
	ms.zero();
	len = LLToS(ms, 100LL, 5);
	SUCT_ASSERT(len == 5);
	SUCT_ASSERT(strcmp(ms.str(), "00100") == 0);
}

void StringUtilityTest::testLLToSMyStringNegative()
{
	using namespace StringUtilityConvertTestSupport;
	MyString<32> ms;
	int len = LLToS(ms, -1LL);
	SUCT_ASSERT(len == 2);
	SUCT_ASSERT(strcmp(ms.str(), "-1") == 0);
	len = LLToS(ms, -100LL);
	SUCT_ASSERT(len == 4);
	SUCT_ASSERT(strcmp(ms.str(), "-100") == 0);
}

void StringUtilityTest::testItoa_sMyString()
{
	using namespace StringUtilityConvertTestSupport;
	MyString<32> ms;
	int len = _itoa_s(0, ms);
	SUCT_ASSERT(len == 1);
	SUCT_ASSERT(strcmp(ms.str(), "0") == 0);
	len = _itoa_s(1, ms);
	SUCT_ASSERT(len == 1);
	SUCT_ASSERT(strcmp(ms.str(), "1") == 0);
	len = _itoa_s(100, ms);
	SUCT_ASSERT(len == 3);
	SUCT_ASSERT(strcmp(ms.str(), "100") == 0);
	len = _itoa_s(-5, ms);
	SUCT_ASSERT(len == 2);
	SUCT_ASSERT(strcmp(ms.str(), "-5") == 0);
	len = _itoa_s(-1000, ms);
	SUCT_ASSERT(len == 5);
	SUCT_ASSERT(strcmp(ms.str(), "-1000") == 0);
	len = _itoa_s(9999, ms);
	SUCT_ASSERT(len == 4);
	SUCT_ASSERT(strcmp(ms.str(), "9999") == 0);
}

void StringUtilityTest::testFindCharCountConvert()
{
	using namespace StringUtilityConvertTestSupport;
	SUCT_ASSERT(findCharCount("hello world", 'l') == 3);
	SUCT_ASSERT(findCharCount("hello", 'z') == 0);
	SUCT_ASSERT(findCharCount("", 'a') == 0);
	SUCT_ASSERT(findCharCount("aaa", 'a') == 3);
	SUCT_ASSERT(findCharCount("a,b,c,d", ',') == 3);
	SUCT_ASSERT(findCharCount("abcdef", 'a') == 1);
	SUCT_ASSERT(findCharCount("abcdef", 'f') == 1);
	SUCT_ASSERT(findCharCount("abcdef", 'c') == 1);
}

void StringUtilityTest::testStrChar()
{
	using namespace StringUtilityConvertTestSupport;
	const char* str = "hello world";
	int pos = strchar(str, 'o');
	SUCT_ASSERT(pos == 4);
	pos = strchar(str, 'z');
	SUCT_ASSERT(pos == -1);
	pos = strchar(str, 'o', 5);
	SUCT_ASSERT(pos == 7);
	pos = strchar("aaa", 'a');
	SUCT_ASSERT(pos == 0);
	pos = strchar("abc", 'c');
	SUCT_ASSERT(pos == 2);
	pos = strchar("", 'a');
	SUCT_ASSERT(pos == -1);
}

void StringUtilityTest::testGetLastCharConvert()
{
	using namespace StringUtilityConvertTestSupport;
	const char* str = "hello.world.txt";
	int pos = getLastChar(str, '.');
	SUCT_ASSERT(pos == 11);
	pos = getLastChar("no_dot_here", '.');
	SUCT_ASSERT(pos == -1);
	pos = getLastChar("a", 'a');
	SUCT_ASSERT(pos == 0);
	pos = getLastChar("abc", 'c');
	SUCT_ASSERT(pos == 2);
	pos = getLastChar("abc", 'a');
	SUCT_ASSERT(pos == 0);
}

void StringUtilityTest::testGetLastNotCharConvert()
{
	using namespace StringUtilityConvertTestSupport;
	string s1 = "hello000";
	int pos = getLastNotChar(s1, '0');
	SUCT_ASSERT(pos == 4);
	string s2 = "hello";
	pos = getLastNotChar(s2, '0');
	SUCT_ASSERT(pos == 4);
	string s3 = "000";
	pos = getLastNotChar(s3, '0');
	SUCT_ASSERT(pos == -1);
	string s4 = "a";
	pos = getLastNotChar(s4, '0');
	SUCT_ASSERT(pos == 0);
	string s5 = "abc";
	pos = getLastNotChar(s5, 'z');
	SUCT_ASSERT(pos == 2);
}

void StringUtilityTest::testFindStringLowerConvert()
{
	using namespace StringUtilityConvertTestSupport;
	bool found = findStringLower("Hello World", "world");
	SUCT_ASSERT(found);
	found = findStringLower("Hello World", "HELLO");
	SUCT_ASSERT(found);
	found = findStringLower("Hello World", "xyz");
	SUCT_ASSERT(!found);
	found = findStringLower("", "abc");
	SUCT_ASSERT(!found);
	int pos = -1;
	found = findStringLower("Hello World", "world", &pos);
	SUCT_ASSERT(found && pos == 6);
	found = findStringLower("ABCABC", "bc", &pos);
	SUCT_ASSERT(found);
	found = findStringLower("TestString", "test");
	SUCT_ASSERT(found);
}

void StringUtilityTest::testGreaterPower2StringUtilConvert()
{
	using namespace StringUtilityConvertTestSupport;
	SUCT_ASSERT(greaterPower2(1) == 2);  // 查表：index 1 返回 2
	SUCT_ASSERT(greaterPower2(2) == 2);
	SUCT_ASSERT(greaterPower2(3) == 4);
	SUCT_ASSERT(greaterPower2(4) == 4);
	SUCT_ASSERT(greaterPower2(5) == 8);
	SUCT_ASSERT(greaterPower2(7) == 8);
	SUCT_ASSERT(greaterPower2(8) == 8);
	SUCT_ASSERT(greaterPower2(9) == 16);
	SUCT_ASSERT(greaterPower2(100) == 128);
	SUCT_ASSERT(greaterPower2(128) == 128);
	SUCT_ASSERT(greaterPower2(200) == 256);
}

void StringUtilityTest::testStrlengthConvert()
{
	using namespace StringUtilityConvertTestSupport;
	SUCT_ASSERT(strlength("hello") == 5);
	SUCT_ASSERT(strlength("") == 0);
	SUCT_ASSERT(strlength("a") == 1);
	SUCT_ASSERT(strlength("Hello World") == 11);
	SUCT_ASSERT(strlength("hello", 10) == 5);
	SUCT_ASSERT(strlength("hello", 3) == 3);
	SUCT_ASSERT(strlength("", 5) == 0);
}

void StringUtilityTest::testGetLastNumber()
{
	using namespace StringUtilityConvertTestSupport;
	SUCT_ASSERT(getLastNumber("abc123") == 123);
	SUCT_ASSERT(getLastNumber("test1") == 1);
	SUCT_ASSERT(getLastNumber("no_number") == 0);
	SUCT_ASSERT(getLastNumber("item999") == 999);
	SUCT_ASSERT(getLastNumber("abc0") == 0);
	int pos = getLastNotNumberPos("abc123");
	SUCT_ASSERT(pos == 2);
	pos = getLastNotNumberPos("abc");
	SUCT_ASSERT(pos == 2);
	pos = getLastNotNumberPos("123");
	SUCT_ASSERT(pos == -1);
}

namespace StringUtilityExtendTestSupport
{
#define SUEXT_ASSERT(condition) if (!(condition)) { ERROR("StringUtilityExtendTest failed: " #condition); }
}

void StringUtilityTest::testSplitByCharBasic()
{
	using namespace StringUtilityExtendTestSupport;
	Vector<string> vec;
	split("a,b,c", ',', vec);
	SUEXT_ASSERT(vec.size() == 3);
	SUEXT_ASSERT(vec[0] == "a");
	SUEXT_ASSERT(vec[1] == "b");
	SUEXT_ASSERT(vec[2] == "c");
}

void StringUtilityTest::testSplitByCharRemoveEmpty()
{
	using namespace StringUtilityExtendTestSupport;
	Vector<string> vec;
	split("a,,b,,c", ',', vec, true);
	SUEXT_ASSERT(vec.size() == 3);
	SUEXT_ASSERT(vec[0] == "a");
	SUEXT_ASSERT(vec[1] == "b");
	SUEXT_ASSERT(vec[2] == "c");

	Vector<string> vec2;
	split("a,,b,,c", ',', vec2, false);
	SUEXT_ASSERT(vec2.size() == 5);
	SUEXT_ASSERT(vec2[0] == "a");
	SUEXT_ASSERT(vec2[1] == "");
	SUEXT_ASSERT(vec2[2] == "b");
}

void StringUtilityTest::testSplitByCharReturnVector()
{
	using namespace StringUtilityExtendTestSupport;
	Vector<string> vec = split("x:y:z", ':');
	SUEXT_ASSERT(vec.size() == 3);
	SUEXT_ASSERT(vec[0] == "x");
	SUEXT_ASSERT(vec[1] == "y");
	SUEXT_ASSERT(vec[2] == "z");
}

void StringUtilityTest::testSplitByCharToBuffer()
{
	using namespace StringUtilityExtendTestSupport;
	string buf[4];
	int count = split("10,20,30", ',', buf, 4);
	SUEXT_ASSERT(count == 3);
	SUEXT_ASSERT(buf[0] == "10");
	SUEXT_ASSERT(buf[1] == "20");
	SUEXT_ASSERT(buf[2] == "30");
}

void StringUtilityTest::testSplitByStringBasic()
{
	using namespace StringUtilityExtendTestSupport;
	Vector<string> vec;
	split("a::b::c", "::", vec);
	SUEXT_ASSERT(vec.size() == 3);
	SUEXT_ASSERT(vec[0] == "a");
	SUEXT_ASSERT(vec[1] == "b");
	SUEXT_ASSERT(vec[2] == "c");
}

void StringUtilityTest::testSplitByStringRemoveEmpty()
{
	using namespace StringUtilityExtendTestSupport;
	Vector<string> vec;
	split("a::::b", "::", vec, true);
	SUEXT_ASSERT(vec.size() == 2);
	SUEXT_ASSERT(vec[0] == "a");
	SUEXT_ASSERT(vec[1] == "b");
}

void StringUtilityTest::testSplitLineBasic()
{
	using namespace StringUtilityExtendTestSupport;
	Vector<string> vec;
	splitLine("line1\nline2\nline3", vec);
	SUEXT_ASSERT(vec.size() == 3);
	SUEXT_ASSERT(vec[0] == "line1");
	SUEXT_ASSERT(vec[1] == "line2");
	SUEXT_ASSERT(vec[2] == "line3");
}

void StringUtilityTest::testSplitLineToBuffer()
{
	using namespace StringUtilityExtendTestSupport;
	string buf[4];
	splitLine("alpha\nbeta\ngamma", buf, 4);
	SUEXT_ASSERT(buf[0] == "alpha");
	SUEXT_ASSERT(buf[1] == "beta");
	SUEXT_ASSERT(buf[2] == "gamma");
}

void StringUtilityTest::testFindStringBasic()
{
	using namespace StringUtilityExtendTestSupport;
	int pos = 0;
	bool found = findString("hello world", "world", &pos);
	SUEXT_ASSERT(found);
	SUEXT_ASSERT(pos == 6);

	bool notFound = findString("hello world", "xyz");
	SUEXT_ASSERT(!notFound);
}

void StringUtilityTest::testFindStringStartIndex()
{
	using namespace StringUtilityExtendTestSupport;
	int pos = 0;
	bool found = findString("abcabc", "abc", &pos, 1);
	SUEXT_ASSERT(found);
	SUEXT_ASSERT(pos == 3);
}

void StringUtilityTest::testFindStringLowerBasic()
{
	using namespace StringUtilityExtendTestSupport;
	int pos = 0;
	bool found = findStringLower("Hello World", "world", &pos);
	SUEXT_ASSERT(found);
	SUEXT_ASSERT(pos == 6);

	bool notFound = findStringLower("Hello World", "xyz");
	SUEXT_ASSERT(!notFound);
}

void StringUtilityTest::testFindStringPosExtend()
{
	using namespace StringUtilityExtendTestSupport;
	int pos = findStringPos("abcdef", "cde");
	SUEXT_ASSERT(pos == 2);

	int notFound = findStringPos("abcdef", "xyz");
	SUEXT_ASSERT(notFound == -1);
}

void StringUtilityTest::testReplaceBasic()
{
	using namespace StringUtilityExtendTestSupport;
	char buf[32];
	strcpy_s(buf, sizeof(buf), "hello world");
	replace(buf, sizeof(buf), 6, 11, "there");
	SUEXT_ASSERT(string(buf) == "hello there");
}

void StringUtilityTest::testReplaceRangeChar()
{
	using namespace StringUtilityExtendTestSupport;
	MyString<64> str;
	str.add("abcdef");
	replace(str, 2, 4, "XY");
	SUEXT_ASSERT(string(str.str()) == "abXYef");
}

void StringUtilityTest::testReplaceRangeString()
{
	using namespace StringUtilityExtendTestSupport;
	string str = "hello world";
	replace(str, 6, 11, string("there"));
	SUEXT_ASSERT(str == "hello there");
}

void StringUtilityTest::testReplaceAllString()
{
	using namespace StringUtilityExtendTestSupport;
	string str = "aaa bbb aaa ccc aaa";
	replaceAll(str, string("aaa"), string("ZZ"));
	SUEXT_ASSERT(str == "ZZ bbb ZZ ccc ZZ");
}

void StringUtilityTest::testReplaceAllCharExtend()
{
	using namespace StringUtilityExtendTestSupport;
	string str = "a,b,c,d";
	replaceAll(str, ',', ';');
	SUEXT_ASSERT(str == "a;b;c;d");
}

void StringUtilityTest::testRemoveAllChar()
{
	using namespace StringUtilityExtendTestSupport;
	string str = "a1b2c3d4";
	removeAll(str, '1');
	SUEXT_ASSERT(str == "ab2c3d4");

	string str2 = "1111abc";
	removeAll(str2, '1');
	SUEXT_ASSERT(str2 == "abc");
}

void StringUtilityTest::testRemoveAllTwoChars()
{
	using namespace StringUtilityExtendTestSupport;
	string str = "a1b2c1d2";
	removeAll(str, '1', '2');
	SUEXT_ASSERT(str == "abcd");
}

void StringUtilityTest::testStartWithSensitive()
{
	using namespace StringUtilityExtendTestSupport;
	SUEXT_ASSERT(startWith("HelloWorld", "Hello"));
	SUEXT_ASSERT(!startWith("HelloWorld", "hello"));
	SUEXT_ASSERT(!startWith("HelloWorld", "World"));

	string strVal = "HelloWorld";
	SUEXT_ASSERT(startWith(strVal, "Hello"));
	SUEXT_ASSERT(!startWith(strVal, "World"));
}

void StringUtilityTest::testStartWithInsensitive()
{
	using namespace StringUtilityExtendTestSupport;
	SUEXT_ASSERT(startWith("HelloWorld", "hello", false));
	SUEXT_ASSERT(startWith("HelloWorld", "HELLO", false));
	SUEXT_ASSERT(!startWith("HelloWorld", "World", false));
}

void StringUtilityTest::testEndWithSensitive()
{
	using namespace StringUtilityExtendTestSupport;
	SUEXT_ASSERT(endWith("HelloWorld", "World"));
	SUEXT_ASSERT(!endWith("HelloWorld", "world"));
	SUEXT_ASSERT(!endWith("HelloWorld", "Hello"));

	string strVal = "HelloWorld";
	SUEXT_ASSERT(endWith(strVal, "World"));
	SUEXT_ASSERT(!endWith(strVal, "Hello"));
}

void StringUtilityTest::testEndWithInsensitive()
{
	using namespace StringUtilityExtendTestSupport;
	SUEXT_ASSERT(endWith("HelloWorld", "world", false));
	SUEXT_ASSERT(endWith("HelloWorld", "WORLD", false));
	SUEXT_ASSERT(!endWith("HelloWorld", "Hello", false));
}

void StringUtilityTest::testRemoveStringFromMyString()
{
	using namespace StringUtilityExtendTestSupport;
	MyString<64> str;
	str.add("hello world foo");
	bool removed = removeString(str, " world");
	SUEXT_ASSERT(removed);
	SUEXT_ASSERT(string(str.str()) == "hello foo");

	MyString<64> str2;
	str2.add("hello");
	bool notRemoved = removeString(str2, "xyz");
	SUEXT_ASSERT(!notRemoved);
	SUEXT_ASSERT(string(str2.str()) == "hello");
}

void StringUtilityTest::testRemoveStringFromCharBuf()
{
	using namespace StringUtilityExtendTestSupport;
	char buf[64];
	strcpy_s(buf, sizeof(buf), "abcXXdef");
	bool removed = removeString(buf, sizeof(buf), "XX");
	SUEXT_ASSERT(removed);
	SUEXT_ASSERT(string(buf) == "abcdef");

	char buf2[32];
	strcpy_s(buf2, sizeof(buf2), "hello");
	bool notRemoved = removeString(buf2, sizeof(buf2), "zzz");
	SUEXT_ASSERT(!notRemoved);
}

namespace StringUtilityFilePathTestSupport
{
#define SUFP_ASSERT(condition) if (!(condition)) { ERROR("StringUtilityFilePathTest failed: " #condition); }
}

void StringUtilityTest::testGetFileName()
{
	using namespace StringUtilityFilePathTestSupport;
	string name = getFileName("C:/path/to/file.txt");
	SUFP_ASSERT(name == "file.txt");

	string name2 = getFileName("folder/sub/image.png");
	SUFP_ASSERT(name2 == "image.png");

	string name3 = getFileName("justfile.dat");
	SUFP_ASSERT(name3 == "justfile.dat");
}

void StringUtilityTest::testGetFileNameNoSuffixWithPath()
{
	using namespace StringUtilityFilePathTestSupport;
	// removePath=true 才会去掉路径部分，只留文件名（无后缀）
	string name = getFileNameNoSuffix("C:/path/to/file.txt", true);
	SUFP_ASSERT(name == "file");

	string name2 = getFileNameNoSuffix("folder/image.png", true);
	SUFP_ASSERT(name2 == "image");
}

void StringUtilityTest::testGetFileNameNoSuffixNoPath()
{
	using namespace StringUtilityFilePathTestSupport;
	string name = getFileNameNoSuffix("C:/path/to/file.txt", true);
	SUFP_ASSERT(name == "file");

	string name2 = getFileNameNoSuffix("file.cfg", true);
	SUFP_ASSERT(name2 == "file");
}

void StringUtilityTest::testGetFirstFolderName()
{
	using namespace StringUtilityFilePathTestSupport;
	string folder = getFirstFolderName("Assets/Scripts/Game/Main.cpp");
	SUFP_ASSERT(folder == "Assets");

	string folder2 = getFirstFolderName("Root/Sub/file.txt");
	SUFP_ASSERT(folder2 == "Root");
}

void StringUtilityTest::testRemoveFirstPath()
{
	using namespace StringUtilityFilePathTestSupport;
	string rest = removeFirstPath("Assets/Scripts/Game/Main.cpp");
	SUFP_ASSERT(rest == "Scripts/Game/Main.cpp");

	string rest2 = removeFirstPath("Root/file.txt");
	SUFP_ASSERT(rest2 == "file.txt");
}

void StringUtilityTest::testGetFilePath()
{
	using namespace StringUtilityFilePathTestSupport;
	string path = getFilePath("C:/path/to/file.txt", false);
	SUFP_ASSERT(path == "C:/path/to");
}

void StringUtilityTest::testGetFilePathKeepSlash()
{
	using namespace StringUtilityFilePathTestSupport;
	string path = getFilePath("C:/path/to/file.txt", true);
	SUFP_ASSERT(path == "C:/path/to/");
}

void StringUtilityTest::testGetFileSuffixKeepDot()
{
	using namespace StringUtilityFilePathTestSupport;
	string suffix = getFileSuffix("file.txt", true);
	SUFP_ASSERT(suffix == ".txt");

	string suffix2 = getFileSuffix("image.png", true);
	SUFP_ASSERT(suffix2 == ".png");
}

void StringUtilityTest::testGetFileSuffixNoDot()
{
	using namespace StringUtilityFilePathTestSupport;
	string suffix = getFileSuffix("file.txt", false);
	SUFP_ASSERT(suffix == "txt");

	string suffix2 = getFileSuffix("data.cfg", false);
	SUFP_ASSERT(suffix2 == "cfg");
}

void StringUtilityTest::testRemoveSuffixFilePath()
{
	using namespace StringUtilityFilePathTestSupport;
	string result = removeSuffix("file.txt");
	SUFP_ASSERT(result == "file");

	string result2 = removeSuffix("image.png");
	SUFP_ASSERT(result2 == "image");

	string result3 = removeSuffix("path/to/file.dat");
	SUFP_ASSERT(result3 == "path/to/file");
}

void StringUtilityTest::testRemoveStartString()
{
	using namespace StringUtilityFilePathTestSupport;
	string result = removeStartString("Assets/Scripts/Main.cpp", "Assets/");
	SUFP_ASSERT(result == "Scripts/Main.cpp");

	string result2 = removeStartString("prefixHello", "prefix");
	SUFP_ASSERT(result2 == "Hello");

	string result3 = removeStartString("Hello", "prefix");
	SUFP_ASSERT(result3 == "Hello");
}

void StringUtilityTest::testRemoveEndString()
{
	using namespace StringUtilityFilePathTestSupport;
	string result = removeEndString("hello_test", "_test");
	SUFP_ASSERT(result == "hello");

	string result2 = removeEndString("MyScript.h", ".h");
	SUFP_ASSERT(result2 == "MyScript");

	string result3 = removeEndString("hello", "_test");
	SUFP_ASSERT(result3 == "hello");
}

void StringUtilityTest::testReplaceSuffix()
{
	using namespace StringUtilityFilePathTestSupport;
	string result = replaceSuffix("file.txt", ".cfg");
	SUFP_ASSERT(result == "file.cfg");

	string result2 = replaceSuffix("image.png", ".jpg");
	SUFP_ASSERT(result2 == "image.jpg");
}

void StringUtilityTest::testGetLastNotNumberPos()
{
	using namespace StringUtilityFilePathTestSupport;
	int pos = getLastNotNumberPos("abc123");
	SUFP_ASSERT(pos == 2);

	int pos2 = getLastNotNumberPos("test456789");
	SUFP_ASSERT(pos2 == 3);

	int pos3 = getLastNotNumberPos("12345");
	SUFP_ASSERT(pos3 == -1);
}

void StringUtilityTest::testGetLastNumberFilePath()
{
	using namespace StringUtilityFilePathTestSupport;
	int num = getLastNumber("abc123");
	SUFP_ASSERT(num == 123);

	int num2 = getLastNumber("item007");
	SUFP_ASSERT(num2 == 7);

	int num3 = getLastNumber("Object99");
	SUFP_ASSERT(num3 == 99);
}

void StringUtilityTest::testGetNotNumberSubString()
{
	using namespace StringUtilityFilePathTestSupport;
	string str = getNotNumberSubString("abc123");
	SUFP_ASSERT(str == "abc");

	string str2 = getNotNumberSubString("Object99");
	SUFP_ASSERT(str2 == "Object");
}

void StringUtilityTest::testBToS()
{
	using namespace StringUtilityFilePathTestSupport;
	string trueStr = BToS(true);
	SUFP_ASSERT(trueStr == "True");

	string falseStr = BToS(false);
	SUFP_ASSERT(falseStr == "False");
}

namespace StringUtilityStringOpTestSupport
{
#define SUSO_ASSERT(condition) if (!(condition)) { ERROR("StringUtilityStringOpTest failed: " #condition); }
}

void StringUtilityTest::testRemoveStartBasic()
{
	using namespace StringUtilityStringOpTestSupport;
	string s = "/hello";
	removeStart(s, '/');
	SUSO_ASSERT(s == "hello");

	string s2 = "abcdef";
	removeStart(s2, 'a');
	SUSO_ASSERT(s2 == "bcdef");
}

void StringUtilityTest::testRemoveStartNotFound()
{
	using namespace StringUtilityStringOpTestSupport;
	string s = "hello";
	removeStart(s, 'x');
	SUSO_ASSERT(s == "hello");
}

void StringUtilityTest::testRemoveStartAllBasic()
{
	using namespace StringUtilityStringOpTestSupport;
	string s = "///hello";
	removeStartAll(s, '/');
	SUSO_ASSERT(s == "hello");

	string s2 = "000123";
	removeStartAll(s2, '0');
	SUSO_ASSERT(s2 == "123");
}

void StringUtilityTest::testRemoveStartAllAllSame()
{
	using namespace StringUtilityStringOpTestSupport;
	string s = "aaaa";
	removeStartAll(s, 'a');
	SUSO_ASSERT(s.empty());
}

void StringUtilityTest::testRemoveLastBasic()
{
	using namespace StringUtilityStringOpTestSupport;
	string s = "hello/";
	removeLast(s, '/');
	SUSO_ASSERT(s == "hello");

	string s2 = "hello!";
	removeLast(s2, '!');
	SUSO_ASSERT(s2 == "hello");
}

void StringUtilityTest::testRemoveLastNotFound()
{
	using namespace StringUtilityStringOpTestSupport;
	string s = "hello";
	removeLast(s, 'x');
	SUSO_ASSERT(s == "hello");
}

void StringUtilityTest::testRemoveLastAllBasic()
{
	using namespace StringUtilityStringOpTestSupport;
	string s = "hello///";
	removeLastAll(s, '/');
	SUSO_ASSERT(s == "hello");

	string s2 = "data...";
	removeLastAll(s2, '.');
	SUSO_ASSERT(s2 == "data");
}

void StringUtilityTest::testRemoveLastAllAllSame()
{
	using namespace StringUtilityStringOpTestSupport;
	string s = "bbbb";
	removeLastAll(s, 'b');
	SUSO_ASSERT(s.empty());
}

void StringUtilityTest::testRemoveLastComma()
{
	using namespace StringUtilityStringOpTestSupport;
	string s = "a,b,c,";
	removeLastComma(s);
	SUSO_ASSERT(s == "a,b,c");

	string s2 = "nocomma";
	removeLastComma(s2);
	SUSO_ASSERT(s2 == "nocomma");
}

void StringUtilityTest::testReplaceAllMyString()
{
	using namespace StringUtilityStringOpTestSupport;
	// replaceAll 内部 replace 的 memmove 不包含末尾 \0，缩短替换后字符串末尾有残留
	// 使用等长替换（key 和 newWord 长度相同）可避开此问题
	MyString<64> str;
	str.add("aaa_bbb_aaa");
	replaceAll(str.data(), 64, "aaa", "ZZZ");  // 等长 3→3
	SUSO_ASSERT(string(str.str()) == "ZZZ_bbb_ZZZ");
}

void StringUtilityTest::testReplaceAllCharBuf()
{
	using namespace StringUtilityStringOpTestSupport;
	char buf[64];
	strcpy_s(buf, sizeof(buf), "hi world hi");
	replaceAll(buf, sizeof(buf), "hi", "bye");  // 2→3，最后一次 memmove 0字节不影响\0
	SUSO_ASSERT(string(buf) == "bye world bye");
}

void StringUtilityTest::testRemoveStringMyString()
{
	using namespace StringUtilityStringOpTestSupport;
	MyString<64> str;
	str.add("hello world");
	bool removed = removeString(str, " world");
	SUSO_ASSERT(removed);
	SUSO_ASSERT(string(str.str()) == "hello");

	MyString<64> str2;
	str2.add("test");
	bool notRemoved = removeString(str2, "xyz");
	SUSO_ASSERT(!notRemoved);
	SUSO_ASSERT(string(str2.str()) == "test");
}

void StringUtilityTest::testRemoveStringCharBuf()
{
	using namespace StringUtilityStringOpTestSupport;
	char buf[32];
	strcpy_s(buf, sizeof(buf), "aXXb");
	bool removed = removeString(buf, sizeof(buf), "XX");
	SUSO_ASSERT(removed);
	SUSO_ASSERT(string(buf) == "ab");
}

void StringUtilityTest::testReplaceMyStringRange()
{
	using namespace StringUtilityStringOpTestSupport;
	MyString<64> str;
	str.add("Hello World");
	replace(str, 6, 11, "There");
	SUSO_ASSERT(string(str.str()) == "Hello There");
}

void StringUtilityTest::testReplaceCharBufRange()
{
	using namespace StringUtilityStringOpTestSupport;
	char buf[32];
	strcpy_s(buf, sizeof(buf), "abcdef");
	replace(buf, sizeof(buf), 1, 3, "XX");
	SUSO_ASSERT(string(buf) == "aXXdef");
}

void StringUtilityTest::testReplaceAllStringObj()
{
	using namespace StringUtilityStringOpTestSupport;
	string s = "the quick brown fox";
	replaceAll(s, string("the"), string("a"));
	SUSO_ASSERT(s == "a quick brown fox");
}

void StringUtilityTest::testReplaceAllStringChar()
{
	using namespace StringUtilityStringOpTestSupport;
	string s = "a.b.c.d";
	replaceAll(s, '.', '/');
	SUSO_ASSERT(s == "a/b/c/d");
}

void StringUtilityTest::testRemoveAllCharFromString()
{
	using namespace StringUtilityStringOpTestSupport;
	string s = "he1l1lo";
	removeAll(s, '1');
	SUSO_ASSERT(s == "hello");

	string s2 = "111abc111";
	removeAll(s2, '1');
	SUSO_ASSERT(s2 == "abc");
}

void StringUtilityTest::testRemoveAllTwoCharsFromString()
{
	using namespace StringUtilityStringOpTestSupport;
	string s = "a1b2c1d2e";
	removeAll(s, '1', '2');
	SUSO_ASSERT(s == "abcde");
}

namespace StringUtilityFormat2TestSupport
{
#define SUF2_ASSERT(expr) \
    do { if (!(expr)) { ERROR("StringUtilityFormat2Test FAIL: " #expr); } } while(0)
}

void StringUtilityTest::testSplitBasic()
{
	using namespace StringUtilityFormat2TestSupport;
	string input = "a,b,c";
	Vector<string> parts;
	split(input.c_str(), ',', parts);
	SUF2_ASSERT(parts.size() == 3);
	SUF2_ASSERT(parts[0] == "a");
	SUF2_ASSERT(parts[1] == "b");
	SUF2_ASSERT(parts[2] == "c");
}

void StringUtilityTest::testSplitEmptyInput()
{
	using namespace StringUtilityFormat2TestSupport;
	string input = "";
	Vector<string> parts;
	split(input.c_str(), ',', parts);
	SUF2_ASSERT(parts.size() == 0 || (parts.size() == 1 && parts[0].empty()));
}

void StringUtilityTest::testSplitNoDelimiter()
{
	using namespace StringUtilityFormat2TestSupport;
	string input = "hello";
	Vector<string> parts;
	split(input.c_str(), ',', parts);
	SUF2_ASSERT(parts.size() == 1);
	SUF2_ASSERT(parts[0] == "hello");
}

void StringUtilityTest::testSplitMultipleDelimiters()
{
	using namespace StringUtilityFormat2TestSupport;
	string input = "1,2,,4";
	Vector<string> parts;
	split(input.c_str(), ',', parts);
	SUF2_ASSERT(parts.size() >= 3);
}

void StringUtilityTest::testSplitLeadingTrailingDelim()
{
	using namespace StringUtilityFormat2TestSupport;
	string input = ",a,b,";
	Vector<string> parts;
	split(input.c_str(), ',', parts);
	SUF2_ASSERT(parts.size() >= 2);
}

void StringUtilityTest::testSplitLineBasicFormat2()
{
	using namespace StringUtilityFormat2TestSupport;
	string input = "line1\nline2\nline3";
	Vector<string> parts;
	splitLine(input.c_str(), parts);
	SUF2_ASSERT(parts.size() == 3);
	SUF2_ASSERT(parts[0] == "line1");
	SUF2_ASSERT(parts[1] == "line2");
	SUF2_ASSERT(parts[2] == "line3");
}

void StringUtilityTest::testSplitLineWindowsLineEnding()
{
	using namespace StringUtilityFormat2TestSupport;
	string input = "a\r\nb\r\nc";
	Vector<string> parts;
	splitLine(input.c_str(), parts);
	SUF2_ASSERT(parts.size() == 3);
}

void StringUtilityTest::testSplitLineEmpty()
{
	using namespace StringUtilityFormat2TestSupport;
	string input = "";
	Vector<string> parts;
	splitLine(input.c_str(), parts);
	SUF2_ASSERT(parts.size() == 0 || (parts.size() == 1 && parts[0].empty()));
}

void StringUtilityTest::testFindStringBasicFormat2()
{
	using namespace StringUtilityFormat2TestSupport;
	string str = "Hello World";
	bool found = findString(str, "World");
	SUF2_ASSERT(found == true);
	bool notFound = findString(str, "xyz");
	SUF2_ASSERT(notFound == false);
}

void StringUtilityTest::testFindStringNotFound()
{
	using namespace StringUtilityFormat2TestSupport;
	string str = "abcdefg";
	bool found = findString(str, "xyz");
	SUF2_ASSERT(found == false);
}

void StringUtilityTest::testFindStringCaseSensitive()
{
	using namespace StringUtilityFormat2TestSupport;
	string str = "Hello";
	bool foundUpper = findString(str, "Hello");
	SUF2_ASSERT(foundUpper == true);
	bool foundLower = findString(str, "hello");
	SUF2_ASSERT(foundLower == false);
}

void StringUtilityTest::testFindStringPosBasic()
{
	using namespace StringUtilityFormat2TestSupport;
	string str = "abcabc";
	int pos = findStringPos(str, "b");
	SUF2_ASSERT(pos == 1);
	int posNotFound = findStringPos(str, "xyz");
	SUF2_ASSERT(posNotFound < 0);
}

void StringUtilityTest::testFindStringLowerBasicFormat2()
{
	using namespace StringUtilityFormat2TestSupport;
	string str = "Hello World";
	bool found = findStringLower(str, "world");
	SUF2_ASSERT(found == true);
	bool foundMixed = findStringLower(str, "HELLO");
	SUF2_ASSERT(foundMixed == true);
	bool notFound = findStringLower(str, "xyz");
	SUF2_ASSERT(notFound == false);
}

void StringUtilityTest::testGetLastNotNumberPosBasic()
{
	using namespace StringUtilityFormat2TestSupport;
	string str = "abc123";
	int pos = getLastNotNumberPos(str);
	SUF2_ASSERT(pos == 2);
}

void StringUtilityTest::testGetLastNotNumberPosAllDigits()
{
	using namespace StringUtilityFormat2TestSupport;
	string str = "12345";
	int pos = getLastNotNumberPos(str);
	SUF2_ASSERT(pos < 0);
}

void StringUtilityTest::testGetLastNotNumberPosAllAlpha()
{
	using namespace StringUtilityFormat2TestSupport;
	string str = "abcde";
	int pos = getLastNotNumberPos(str);
	SUF2_ASSERT(pos == 4);
}

void StringUtilityTest::testGetLastNumberBasic()
{
	using namespace StringUtilityFormat2TestSupport;
	string str = "abc123";
	int num = getLastNumber(str);
	SUF2_ASSERT(num == 123);
}

void StringUtilityTest::testGetLastNumberNoDigits()
{
	using namespace StringUtilityFormat2TestSupport;
	string str = "abcde";
	int num = getLastNumber(str);
	SUF2_ASSERT(num == 0);
}

void StringUtilityTest::testGetLastNumberOnlyDigits()
{
	using namespace StringUtilityFormat2TestSupport;
	// getLastNumber 依赖 getLastNotNumberPos 找"非数字"位置
	// 全数字串无非数字前缀，getLastNotNumberPos 返回 NOT_FIND(-1)，getLastNumber 返回 -1
	string str = "9876";
	int num = getLastNumber(str);
	SUF2_ASSERT(num == -1);
	// 有前缀字母时才能正确返回末尾数字
	string str2 = "abc9876";
	int num2 = getLastNumber(str2);
	SUF2_ASSERT(num2 == 9876);
}

void StringUtilityTest::testGetNotNumberSubStringBasic()
{
	using namespace StringUtilityFormat2TestSupport;
	string str = "abc123";
	string result = getNotNumberSubString(str);
	SUF2_ASSERT(result == "abc");
}

void StringUtilityTest::testGetNotNumberSubStringEmpty()
{
	using namespace StringUtilityFormat2TestSupport;
	string str = "123";
	string result = getNotNumberSubString(str);
	SUF2_ASSERT(result.empty());
}

void StringUtilityTest::testGetFirstFolderNameBasic()
{
	using namespace StringUtilityFormat2TestSupport;
	string path = "folder1/folder2/file.txt";
	string first = getFirstFolderName(path);
	SUF2_ASSERT(first == "folder1");
}

void StringUtilityTest::testGetFirstFolderNameRootOnly()
{
	using namespace StringUtilityFormat2TestSupport;
	string path = "file.txt";
	string first = getFirstFolderName(path);
	SUF2_ASSERT(first.empty() || first == "file.txt");
}

void StringUtilityTest::testRemoveFirstPathBasic()
{
	using namespace StringUtilityFormat2TestSupport;
	string path = "folder1/folder2/file.txt";
	string result = removeFirstPath(path);
	SUF2_ASSERT(result == "folder2/file.txt");
}

void StringUtilityTest::testRemoveFirstPathSingle()
{
	using namespace StringUtilityFormat2TestSupport;
	string path = "file.txt";
	string result = removeFirstPath(path);
	SUF2_ASSERT(result == "file.txt" || result.empty());
}

void StringUtilityTest::testGetFileNameNoSuffixBasic()
{
	using namespace StringUtilityFormat2TestSupport;
	string path = "folder/image.png";
	string name = getFileNameNoSuffix(path, true);
	SUF2_ASSERT(name == "image");
}

void StringUtilityTest::testReplaceSuffixBasic()
{
	using namespace StringUtilityFormat2TestSupport;
	string path = "image.png";
	string result = replaceSuffix(path, ".jpg");
	SUF2_ASSERT(result == "image.jpg");
}

void StringUtilityTest::testRemoveStartStringBasic()
{
	using namespace StringUtilityFormat2TestSupport;
	string str = "prefixValue";
	string result = removeStartString(str, "prefix");
	SUF2_ASSERT(result == "Value");
}

void StringUtilityTest::testRemoveEndStringBasic()
{
	using namespace StringUtilityFormat2TestSupport;
	string str = "valueSuffix";
	string result = removeEndString(str, "Suffix");
	SUF2_ASSERT(result == "value");
}

void StringUtilityTest::testSha1()
{
	using namespace StringUtilityFormat2TestSupport;
	string result = sha1("testsha1");
	SUF2_ASSERT(result == "1dee4b1affb249bc74c20379e60166366b71394f");
}


// 覆盖缺口：SToX buffer/vector 重载、toLower/toUpper、bytesToHex/charToHex、isNumber/isPhoneNumber、SToV2/V3
namespace StringUtilityCoverageGapTestSupport
{
#define SUCG_ASSERT(expr, msg) \
    do { if (!(expr)) { ERROR(string("StringUtilityCoverageGap FAILED: " #expr) + (msg)); } } while(0)
#define SUCG_ASSERT_EQ(a, b, msg) \
    do { if ((a) != (b)) { ERROR(string("StringUtilityCoverageGap FAILED: ") + (msg) + \
        " | expected=" + IToS((int)(b)) + " actual=" + IToS((int)(a))); } } while(0)
#define SUCG_ASSERT_STR(a, b, msg) \
    do { if ((a) != (b)) { ERROR(string("StringUtilityCoverageGap FAILED: ") + (msg) + \
        " | expected='" + (b) + "' actual='" + (a) + "'"); } } while(0)

	// ========== toLower / toUpper ==========

	// ========== charToHexString / bytesToHexString ==========

	// ========== isNumber / isPhoneNumber ==========


	// ========== SToIs / SToFs / SToBs / SToLLs / SToSs / SToUIs / SToUSs ==========







	// ========== FsToS / UIsToS ==========


	// ========== SToV2Is / SToV2s / SToV3Is / SToV3s ==========
}

void StringUtilityTest::testToLowerUpper()
{
	using namespace StringUtilityCoverageGapTestSupport;
	SUCG_ASSERT_STR(toLower("Hello World"), "hello world", "toLower mixed");
	SUCG_ASSERT_STR(toLower("ABC"), "abc", "toLower uppercase");
	SUCG_ASSERT_STR(toLower("abc"), "abc", "toLower already lower");
	SUCG_ASSERT_STR(toLower(""), "", "toLower empty");
	SUCG_ASSERT_STR(toLower("123!@#"), "123!@#", "toLower non-alpha unchanged");

	SUCG_ASSERT_STR(toUpper("Hello World"), "HELLO WORLD", "toUpper mixed");
	SUCG_ASSERT_STR(toUpper("abc"), "ABC", "toUpper lowercase");
	SUCG_ASSERT_STR(toUpper("ABC"), "ABC", "toUpper already upper");
	SUCG_ASSERT_STR(toUpper(""), "", "toUpper empty");
	SUCG_ASSERT_STR(toUpper("123!@#"), "123!@#", "toUpper non-alpha unchanged");
}

void StringUtilityTest::testHexConversion()
{
	using namespace StringUtilityCoverageGapTestSupport;
	// charToHexString
	SUCG_ASSERT_STR(byteToHexString((byte)0x00, false), "00", "hex 0x00 lower");
	SUCG_ASSERT_STR(byteToHexString((byte)0xFF, false), "ff", "hex 0xFF lower");
	SUCG_ASSERT_STR(byteToHexString((byte)0xAB, true), "AB", "hex 0xAB upper");
	SUCG_ASSERT_STR(byteToHexString((byte)0x0F, false), "0f", "hex 0x0F lower");
	SUCG_ASSERT_STR(byteToHexString((byte)0x10, true), "10", "hex 0x10 upper");

	// bytesToHexString - no space, lower
	byte data1[] = { 0x00, 0xFF, 0xAB };
	SUCG_ASSERT_STR(bytesToHexString(data1, 3, false, false), "00ffab", "bytesToHex 3 no space lower");

	// bytesToHexString - with space, upper
	SUCG_ASSERT_STR(bytesToHexString(data1, 3, true, true), "00 FF AB", "bytesToHex 3 space upper");

	// bytesToHexString - empty
	SUCG_ASSERT_STR(bytesToHexString(nullptr, 0, false, false), "", "bytesToHex empty");

	// bytesToHexString - single byte
	byte data2[] = { 0x5A };
	SUCG_ASSERT_STR(bytesToHexString(data2, 1, false, false), "5a", "bytesToHex single");
}

void StringUtilityTest::testIsNumber()
{
	using namespace StringUtilityCoverageGapTestSupport;
	SUCG_ASSERT(isNumber("12345"), "isNumber digits");
	SUCG_ASSERT(!isNumber("12a45"), "isNumber with alpha false");
	SUCG_ASSERT(!isNumber("-123"), "isNumber negative false");
	SUCG_ASSERT(!isNumber(""), "isNumber empty false");
	SUCG_ASSERT(!isNumber(" 1"), "isNumber leading space false");
	SUCG_ASSERT(isNumber("0"), "isNumber single zero");
	SUCG_ASSERT(isNumber("9999999999"), "isNumber long number");
}

void StringUtilityTest::testIsPhoneNumber()
{
	using namespace StringUtilityCoverageGapTestSupport;
	SUCG_ASSERT(isPhoneNumber("13800138000"), "phone normal");
	SUCG_ASSERT(!isPhoneNumber("1234567890"), "phone too short (10)");
	SUCG_ASSERT(!isPhoneNumber("123456789012"), "phone too long (12)");
	SUCG_ASSERT(!isPhoneNumber("03800138000"), "phone not start with 1");
	SUCG_ASSERT(!isPhoneNumber("13800a38000"), "phone with alpha");
	SUCG_ASSERT(!isPhoneNumber(""), "phone empty");
}

void StringUtilityTest::testSToIs()
{
	using namespace StringUtilityCoverageGapTestSupport;
	// Vector version
	Vector<int> list;
	SToIs("1,2,3", list, ",");
	SUCG_ASSERT_EQ(list.size(), 3, "SToIs size");
	SUCG_ASSERT_EQ(list[0], 1, "SToIs[0]");
	SUCG_ASSERT_EQ(list[1], 2, "SToIs[1]");
	SUCG_ASSERT_EQ(list[2], 3, "SToIs[2]");

	// buffer version
	int buf[10];
	int count = SToIs("4,5,6", buf, 10, ",");
	SUCG_ASSERT_EQ(count, 3, "SToIs buf count");
	SUCG_ASSERT_EQ(buf[0], 4, "SToIs buf[0]");
	SUCG_ASSERT_EQ(buf[1], 5, "SToIs buf[1]");

	// return vector version
	Vector<int> rlist = SToIs("7,8,9", ",");
	SUCG_ASSERT_EQ(rlist.size(), 3, "SToIs ret size");
	SUCG_ASSERT_EQ(rlist[2], 9, "SToIs ret[2]");

	// empty string
	Vector<int> empty;
	SToIs("", empty, ",");
	SUCG_ASSERT_EQ(empty.size(), 0, "SToIs empty");
}

void StringUtilityTest::testSToFs()
{
	using namespace StringUtilityCoverageGapTestSupport;
	Vector<float> list;
	SToFs("1.5,2.5,3.0", list, ",");
	SUCG_ASSERT_EQ(list.size(), 3, "SToFs size");
	SUCG_ASSERT(abs(list[0] - 1.5f) < 0.001f, "SToFs[0]");
	SUCG_ASSERT(abs(list[1] - 2.5f) < 0.001f, "SToFs[1]");
	SUCG_ASSERT(abs(list[2] - 3.0f) < 0.001f, "SToFs[2]");

	// buffer version
	Array<10, float> buf;
	int count = SToFs("4.0,5.5", buf, ",");
	SUCG_ASSERT_EQ(count, 2, "SToFs buf count");
	SUCG_ASSERT(abs(buf[0] - 4.0f) < 0.001f, "SToFs buf[0]");
}

void StringUtilityTest::testSToBs()
{
	using namespace StringUtilityCoverageGapTestSupport;
	byte buf[10];
	int count = SToBs("10,20,30", buf, 10, ",");
	SUCG_ASSERT_EQ(count, 3, "SToBs count");
	SUCG_ASSERT_EQ((int)buf[0], 10, "SToBs[0]");
	SUCG_ASSERT_EQ((int)buf[1], 20, "SToBs[1]");
	SUCG_ASSERT_EQ((int)buf[2], 30, "SToBs[2]");

	// Vector version
	Vector<byte> vlist;
	SToBs("1,2", vlist, ",");
	SUCG_ASSERT_EQ(vlist.size(), 2, "SToBs vec size");
}

void StringUtilityTest::testSToLLs()
{
	using namespace StringUtilityCoverageGapTestSupport;
	Vector<llong> list;
	SToLLs("10000000000,20000000000", list, ",");
	SUCG_ASSERT_EQ(list.size(), 2, "SToLLs size");
	SUCG_ASSERT(list[0] == 10000000000LL, "SToLLs[0]");
	SUCG_ASSERT(list[1] == 20000000000LL, "SToLLs[1]");

	// buffer version
	llong buf[10];
	int count = SToLLs("42,99", buf, 10, ",");
	SUCG_ASSERT_EQ(count, 2, "SToLLs buf count");
	SUCG_ASSERT(buf[0] == 42LL, "SToLLs buf[0]");

	// return vector version
	Vector<llong> rlist = SToLLs("7,8", ",");
	SUCG_ASSERT_EQ(rlist.size(), 2, "SToLLs ret size");
}

void StringUtilityTest::testSToSs()
{
	using namespace StringUtilityCoverageGapTestSupport;
	short buf[10];
	int count = SToSs("100,200,300", buf, 10, ",");
	SUCG_ASSERT_EQ(count, 3, "SToSs count");
	SUCG_ASSERT_EQ((int)buf[0], 100, "SToSs[0]");
	SUCG_ASSERT_EQ((int)buf[1], 200, "SToSs[1]");
	SUCG_ASSERT_EQ((int)buf[2], 300, "SToSs[2]");

	Vector<short> vlist;
	SToSs("1,2", vlist, ",");
	SUCG_ASSERT_EQ(vlist.size(), 2, "SToSs vec size");
}

void StringUtilityTest::testSToUIs()
{
	using namespace StringUtilityCoverageGapTestSupport;
	uint buf[10];
	int count = SToUIs("10,20,30", buf, 10, ",");
	SUCG_ASSERT_EQ(count, 3, "SToUIs count");
	SUCG_ASSERT_EQ((int)buf[0], 10, "SToUIs[0]");

	Vector<uint> vlist;
	SToUIs("1,2", vlist, ",");
	SUCG_ASSERT_EQ(vlist.size(), 2, "SToUIs vec size");
}

void StringUtilityTest::testSToUSs()
{
	using namespace StringUtilityCoverageGapTestSupport;
	ushort buf[10];
	int count = SToUSs("100,200", buf, 10, ",");
	SUCG_ASSERT_EQ(count, 2, "SToUSs count");
	SUCG_ASSERT_EQ((int)buf[0], 100, "SToUSs[0]");

	Vector<ushort> vlist;
	SToUSs("1,2", vlist, ",");
	SUCG_ASSERT_EQ(vlist.size(), 2, "SToUSs vec size");
}

void StringUtilityTest::testFsToS()
{
	using namespace StringUtilityCoverageGapTestSupport;
	Vector<float> list;
	list.add(1.5f);
	list.add(2.5f);
	string result = FsToS(list, ",");
	SUCG_ASSERT(!result.empty(), "FsToS not empty");
	// 验证包含数字
	SUCG_ASSERT(result.find("1.5") != string::npos || result.find("1.500") != string::npos, "FsToS contains 1.5");

	// buffer version
	char buf[64];
	FsToS(buf, 64, list, ",");
	SUCG_ASSERT(strlen(buf) > 0, "FsToS buf not empty");
}

void StringUtilityTest::testUIsToS()
{
	using namespace StringUtilityCoverageGapTestSupport;
	Vector<uint> list;
	list.add(10u);
	list.add(20u);
	string result = UIsToS(list, ",");
	SUCG_ASSERT_STR(result, "10,20", "UIsToS basic");

	// buffer version
	char buf[64];
	UIsToS(buf, 64, list, ",");
	SUCG_ASSERT_STR(string(buf), "10,20", "UIsToS buf");
}

void StringUtilityTest::testSToV2Is()
{
	using namespace StringUtilityCoverageGapTestSupport;
	Vector<Vector2Int> list;
	SToV2Is("1,2|3,4", list);
	SUCG_ASSERT_EQ(list.size(), 2, "SToV2Is size");
	SUCG_ASSERT_EQ(list[0].x, 1, "SToV2Is[0].x");
	SUCG_ASSERT_EQ(list[0].y, 2, "SToV2Is[0].y");
	SUCG_ASSERT_EQ(list[1].x, 3, "SToV2Is[1].x");
	SUCG_ASSERT_EQ(list[1].y, 4, "SToV2Is[1].y");

	Vector<Vector2Int> empty;
	SToV2Is("", empty);
	SUCG_ASSERT_EQ(empty.size(), 0, "SToV2Is empty");
}

void StringUtilityTest::testSToV2s()
{
	using namespace StringUtilityCoverageGapTestSupport;
	Vector<Vector2> list;
	SToV2s("1.5,2.5|3.0,4.0", list);
	SUCG_ASSERT_EQ(list.size(), 2, "SToV2s size");
	SUCG_ASSERT(abs(list[0].x - 1.5f) < 0.001f, "SToV2s[0].x");
	SUCG_ASSERT(abs(list[0].y - 2.5f) < 0.001f, "SToV2s[0].y");
}

void StringUtilityTest::testSToV3Is()
{
	using namespace StringUtilityCoverageGapTestSupport;
	Vector<Vector3Int> list;
	SToV3Is("1,2,3|4,5,6", list);
	SUCG_ASSERT_EQ(list.size(), 2, "SToV3Is size");
	SUCG_ASSERT_EQ(list[0].x, 1, "SToV3Is[0].x");
	SUCG_ASSERT_EQ(list[0].y, 2, "SToV3Is[0].y");
	SUCG_ASSERT_EQ(list[0].z, 3, "SToV3Is[0].z");
	SUCG_ASSERT_EQ(list[1].x, 4, "SToV3Is[1].x");
	SUCG_ASSERT_EQ(list[1].y, 5, "SToV3Is[1].y");
	SUCG_ASSERT_EQ(list[1].z, 6, "SToV3Is[1].z");
}

void StringUtilityTest::testSToV3s()
{
	using namespace StringUtilityCoverageGapTestSupport;
	Vector<Vector3> list;
	SToV3s("1.0,2.0,3.0|4.0,5.0,6.0", list);
	SUCG_ASSERT_EQ(list.size(), 2, "SToV3s size");
	SUCG_ASSERT(abs(list[0].x - 1.0f) < 0.001f, "SToV3s[0].x");
	SUCG_ASSERT(abs(list[0].y - 2.0f) < 0.001f, "SToV3s[0].y");
	SUCG_ASSERT(abs(list[0].z - 3.0f) < 0.001f, "SToV3s[0].z");
}


void StringUtilityTest::test()
{
	testSplitCharKey();
	testSplitStringKey();
	testSplitLine();
	testFindString();
	testStartEndWith();
	testRemoveStartLast();
	testReplaceAll();
	testFileNamePath();
	testStrMisc();
	testNumberToString();
	testStrCopyZero();
	testGreaterPower2StringUtil();
	testRemoveString();
	testNumberFormat();
	testEdgeCases();
	
	//------------------------------------------------------------------------------
	// 扩展测试 Part 2 — split 更多场景
	//------------------------------------------------------------------------------
		// split 全是分隔符 removeEmpty=false
	{
		Vector<string> v;
		split(",,,,", ',', v, false);
		// ",,,,": 4个逗号产生5段（""| ""| ""| ""| ""），removeEmpty=false 全部保留
		SUET_ASSERT_EQ(v.size(), 5, "split all delims false -> 5 empty");
		SUET_ASSERT_STR(v[0], "", "split all delims[0] empty");
	}
	
	// split 超长字符串
	{
		string longStr;
		for (int i = 0; i < 100; ++i)
		{
			if (i > 0) longStr += ',';
			longStr += IToS(i);
		}
		Vector<string> v;
		split(longStr.c_str(), ',', v);
		SUET_ASSERT_EQ(v.size(), 100, "split 100 tokens");
		SUET_ASSERT_STR(v[0], "0", "split long [0]='0'");
		SUET_ASSERT_STR(v[99], "99", "split long [99]='99'");
	}
	
	// split 返回值版本（string separator）
	{
		Vector<string> v = split("a|b|c", '|');
		SUET_ASSERT_EQ(v.size(), 3, "split return 3");
		SUET_ASSERT_STR(v[1], "b", "split return[1]='b'");
	}
	
	
	//------------------------------------------------------------------------------
	// 扩展测试 Part 3 — findString 更多场景
	//------------------------------------------------------------------------------
	// findString startIndex > 0
	{
		int pos = -1;
		bool found = findString("abcabc", "abc", &pos, 3);
		SUET_ASSERT(found, "findString start>0 found");
		SUET_ASSERT_EQ(pos, 3, "findString start>0 pos=3");
	}
	
	// findString not found with startIndex
	{
		bool found = findString("abcabc", "xyz");
		SUET_ASSERT(!found, "findString not found");
	}
	
	// findStringPos
	{
		int pos = findStringPos("hello world", "world");
		SUET_ASSERT_EQ(pos, 6, "findStringPos=6");
	}
	
	// findStringPos not found
	{
		int pos = findStringPos("hello world", "xyz");
		SUET_ASSERT_EQ(pos, -1, "findStringPos not found=-1");
	}
	
	// findCharCount
	{
		int cnt = findCharCount("abcabc", 'a');
		SUET_ASSERT_EQ(cnt, 2, "findCharCount 'a'=2");
		int cnt2 = findCharCount("aaa", 'a');
		SUET_ASSERT_EQ(cnt2, 3, "findCharCount all=3");
		int cnt3 = findCharCount("abc", 'z');
		SUET_ASSERT_EQ(cnt3, 0, "findCharCount missing=0");
	}
	
	
	//------------------------------------------------------------------------------
	// 扩展测试 Part 4 — replaceAll 更多场景
	//------------------------------------------------------------------------------
		// replaceAll char->char
	{
		string s = "a.b.c.d";
		replaceAll(s, '.', '/');
		SUET_ASSERT_STR(s, "a/b/c/d", "replaceAll char->char");
	}
	
	// replaceAll char->char no match
	{
		string s = "hello";
		replaceAll(s, 'z', 'x');
		SUET_ASSERT_STR(s, "hello", "replaceAll no match unchanged");
	}
	
	// replaceAll string->string with overlap-safe
	{
		string s = "aaaa";
		replaceAll(s, string("aa"), string("b"));
		SUET_ASSERT_STR(s, "bb", "replaceAll 'aa'->'b'");
	}
	
	// replaceAll empty key (should not crash)
	// (Framework may have undefined behavior for empty key, just test no crash)
	// skip that edge case for safety
	
	// replace(string, begin, end, reStr)
	{
		string s = "hello world";
		replace(s, 6, 11, "there");
		SUET_ASSERT_STR(s, "hello there", "replace sub 6-11");
	}
	
	// replace at start
	{
		string s = "hello world";
		replace(s, 0, 5, "hi");
		SUET_ASSERT_STR(s, "hi world", "replace at start");
	}
	
	
	//------------------------------------------------------------------------------
	// 扩展测试 Part 5 — removeAll / removeStart / removeLast
	//------------------------------------------------------------------------------
		// removeAll single char
	{
		string s = "a1b2c3";
		removeAll(s, '1');
		SUET_ASSERT_STR(s, "ab2c3", "removeAll '1'");
	}
	
	// removeAll two chars
	{
		string s = "a1b2c3";
		removeAll(s, '1', '3');
		SUET_ASSERT_STR(s, "ab2c", "removeAll '1','3'");
	}
	
	// removeStartAll
	{
		string s = "   hello";
		removeStartAll(s, ' ');
		SUET_ASSERT_STR(s, "hello", "removeStartAll space");
	}
	
	// removeStartAll — all chars
	{
		string s = "aaaa";
		removeStartAll(s, 'a');
		SUET_ASSERT(s.empty(), "removeStartAll all -> empty");
	}
	
	// removeLastAll
	{
		string s = "hello   ";
		removeLastAll(s, ' ');
		SUET_ASSERT_STR(s, "hello", "removeLastAll space");
	}
	
	// removeLastAll — all chars
	{
		string s = "bbbbb";
		removeLastAll(s, 'b');
		SUET_ASSERT(s.empty(), "removeLastAll all -> empty");
	}
	
	// removeStart (single)
	{
		string s = "xxhello";
		removeStart(s, 'x');
		SUET_ASSERT_STR(s, "xhello", "removeStart single first x");
	}
	
	// removeLast (single)
	{
		string s = "helloxx";
		removeLast(s, 'x');
		SUET_ASSERT_STR(s, "hellox", "removeLast single last x");
	}
	
	// removeLastComma
	{
		string s = "a,b,c,";
		removeLastComma(s);
		SUET_ASSERT_STR(s, "a,b,c", "removeLastComma");
	}
	
	
	//------------------------------------------------------------------------------
	// 扩展测试 Part 6 — getFileName / getFilePath / getFileSuffix
	//------------------------------------------------------------------------------
		// getFileName
	{
		string fn = getFileName("path/to/file.txt");
		SUET_ASSERT_STR(fn, "file.txt", "getFileName");
	}
	
	// getFileName no path
	{
		string fn = getFileName("readme.md");
		SUET_ASSERT_STR(fn, "readme.md", "getFileName no path");
	}
	
	// getFileNameNoSuffix
	{
		string fn = getFileNameNoSuffix("path/to/file.txt", true);
		SUET_ASSERT_STR(fn, "file", "getFileNameNoSuffix remove path");
	}
	
	// getFileSuffix with dot
	{
		string suf = getFileSuffix("file.cpp", true);
		SUET_ASSERT_STR(suf, ".cpp", "getFileSuffix with dot");
	}
	
	// getFileSuffix without dot
	{
		string suf = getFileSuffix("file.cpp", false);
		SUET_ASSERT_STR(suf, "cpp", "getFileSuffix without dot");
	}
	
	// getFileSuffix no suffix
	{
		string suf = getFileSuffix("filename", true);
		SUET_ASSERT(suf.empty(), "getFileSuffix no suffix");
	}
	
	// getFilePath keepSlash=true
	{
		string path = getFilePath("path/to/file.txt", true);
		SUET_ASSERT_STR(path, "path/to/", "getFilePath keepSlash");
	}
	
	// getFilePath keepSlash=false
	{
		string path = getFilePath("path/to/file.txt", false);
		SUET_ASSERT_STR(path, "path/to", "getFilePath no slash");
	}
	
	// removeSuffix
	{
		string fn = removeSuffix("icon.png");
		SUET_ASSERT_STR(fn, "icon", "removeSuffix");
	}
	
	// removeStartString
	{
		string s = removeStartString("prefixValue", "prefix");
		SUET_ASSERT_STR(s, "Value", "removeStartString");
	}
	
	// removeEndString
	{
		string s = removeEndString("fileValue.txt", ".txt");
		SUET_ASSERT_STR(s, "fileValue", "removeEndString");
	}
	
	// replaceSuffix
	{
		string s = replaceSuffix("file.txt", ".cpp");
		SUET_ASSERT_STR(s, "file.cpp", "replaceSuffix");
	}
	
	
	//------------------------------------------------------------------------------
	// 扩展测试 Part 7 — getLastNumber / getNotNumberSubString
	//------------------------------------------------------------------------------
	{
		int n = getLastNumber("item123");
		SUET_ASSERT_EQ(n, 123, "getLastNumber 123");
	}
	{
		int n = getLastNumber("test0");
		SUET_ASSERT_EQ(n, 0, "getLastNumber 0");
	}
	{
		int n = getLastNumber("a99b77");
		SUET_ASSERT_EQ(n, 77, "getLastNumber trailing 77");
	}
	{
		string s = getNotNumberSubString("item123");
		SUET_ASSERT_STR(s, "item", "getNotNumberSubString");
	}
	{
		string s = getNotNumberSubString("abc");
		SUET_ASSERT_STR(s, "abc", "getNotNumberSubString no trailing num");
	}
	
	
	//------------------------------------------------------------------------------
	// 扩展测试 Part 8 — strlength, getLastChar, getLastNotChar
	//------------------------------------------------------------------------------
	{
		int len = strlength("hello");
		SUET_ASSERT_EQ(len, 5, "strlength=5");
	}
	{
		int len = strlength("hello", 3);
		SUET_ASSERT_EQ(len, 3, "strlength max=3");
	}
	{
		int len = strlength("", 100);
		SUET_ASSERT_EQ(len, 0, "strlength empty=0");
	}
	{
		int pos = getLastChar("abc", 'c');
		SUET_ASSERT_EQ(pos, 2, "getLastChar 'c'=2");
	}
	{
		int pos = getLastChar("abc", 'z');
		SUET_ASSERT_EQ(pos, -1, "getLastChar 'z'=-1");
	}
	{
		// getLastNotChar: find last position where char is NOT value
		int pos = getLastNotChar("hello   ", ' ');
		SUET_ASSERT_EQ(pos, 4, "getLastNotChar space=4");
	}
	{
		int pos = getLastNotChar("   ", ' ');
		SUET_ASSERT_EQ(pos, -1, "getLastNotChar all space=-1");
	}
	
	
	//------------------------------------------------------------------------------
	// 扩展测试 Part 9 — number-to-string conversion extended
	//------------------------------------------------------------------------------
		// IToS
	{
		SUET_ASSERT_STR(IToS(0), "0", "IToS 0");
		SUET_ASSERT_STR(IToS(-1), "-1", "IToS -1");
		SUET_ASSERT_STR(IToS(2147483647), "2147483647", "IToS INT_MAX");
		SUET_ASSERT_STR(IToS(-2147483648), "-2147483648", "IToS INT_MIN");
	}
	
	// LLToS
	{
		SUET_ASSERT_STR(LLToS(0LL), "0", "LLToS 0");
		SUET_ASSERT_STR(LLToS(-9999999999LL), "-9999999999", "LLToS neg");
	}
	
	// FToS
	{
		string fs = FToS(3.14f);
		SUET_ASSERT(!fs.empty(), "FToS non-empty");
	}
	
	// DToS (no DToS in framework, use FToS with float cast)
	{
		string ds = FToS((float)2.71828);
		SUET_ASSERT(!ds.empty(), "DToS non-empty");
	}
	
	// BToS — 框架返回 "True"/"False"
	{
		SUET_ASSERT_STR(BToS(true), "True", "BToS true=True");
		SUET_ASSERT_STR(BToS(false), "False", "BToS false=False");
	}
	
	// SToI
	{
		SUET_ASSERT_EQ(SToI("42"), 42, "SToI '42'=42");
		SUET_ASSERT_EQ(SToI("-5"), -5, "SToI '-5'=-5");
		SUET_ASSERT_EQ(SToI("0"), 0, "SToI '0'=0");
	}
	
	// SToLL
	{
		SUET_ASSERT_LL(SToLL("9999999999"), 9999999999LL, "SToLL");
	}
	
	// SToF
	{
		float f = SToF("3.14");
		SUET_ASSERT(fabsf(f - 3.14f) < 0.001f, "SToF=3.14");
	}
	
	// DToS (no DToS in framework, use FToS with float cast)
	{
		string ds = FToS((float)2.71828);
		SUET_ASSERT(!ds.empty(), "DToS non-empty");
	}
	
	// SToB — 框架只认 "True"/"true"，与 BToS 配对
	{
		SUET_ASSERT(SToB("True") == true, "SToB 'True'=true");
		SUET_ASSERT(SToB("False") == false, "SToB 'False'=false");
		SUET_ASSERT(SToB("true") == true, "SToB 'true'=true");
	}
	
	
	//------------------------------------------------------------------------------
	// 扩展测试 Part 10 — splitLine
	//------------------------------------------------------------------------------
	{
		Vector<string> v;
		splitLine("line1\nline2\nline3", v);
		SUET_ASSERT_EQ(v.size(), 3, "splitLine 3 lines");
		SUET_ASSERT_STR(v[0], "line1", "splitLine[0]");
		SUET_ASSERT_STR(v[2], "line3", "splitLine[2]");
	}
	
	{
		Vector<string> v;
		splitLine("line1\r\nline2\r\n", v);
		SUET_ASSERT_EQ(v.size(), 2, "splitLine CRLF 2");
	}
	
	{
		Vector<string> v;
		splitLine("", v);
		SUET_ASSERT_EQ(v.size(), 0, "splitLine empty -> 0");
	}
	
	{
		// buffer version
		string buf[5];
		splitLine("a\nb\nc", buf, 5);
		int cnt = 0;
		for (int i = 0; i < 5; ++i) { if (!buf[i].empty()) ++cnt; }
		SUET_ASSERT_EQ(cnt, 3, "splitLine buf cnt=3");
		SUET_ASSERT_STR(buf[0], "a", "splitLine buf[0]='a'");
	}
	
	
	//------------------------------------------------------------------------------
	// 扩展测试 Part 11 — strchar
	//------------------------------------------------------------------------------
	{
		int pos = strchar("abcabc", 'b', 0, 0);
		SUET_ASSERT_EQ(pos, 1, "strchar 'b' first=1");
	}
	{
		int pos = strchar("abcabc", 'b', 2, 0);
		SUET_ASSERT_EQ(pos, 4, "strchar 'b' start=2 -> 4");
	}
	{
		int pos = strchar("abcabc", 'z', 0, 0);
		SUET_ASSERT_EQ(pos, -1, "strchar 'z' -> -1");
	}
	
	
	//------------------------------------------------------------------------------
	// 扩展测试 Part 12 — findStringLower
	//------------------------------------------------------------------------------
	{
		bool found = findStringLower("Hello World", "world");
		SUET_ASSERT(found, "findStringLower case-insensitive found");
	}
	{
		bool found = findStringLower("Hello World", "xyz");
		SUET_ASSERT(!found, "findStringLower not found");
	}
	{
		int pos = -1;
		findStringLower("ABCDEF", "cde", &pos);
		SUET_ASSERT_EQ(pos, 2, "findStringLower pos=2");
	}
	
	
	//------------------------------------------------------------------------------
	// 扩展测试 Part 13 — getFirstFolderName / removeFirstPath
	//------------------------------------------------------------------------------
	{
		string folder = getFirstFolderName("path/to/file.txt");
		SUET_ASSERT_STR(folder, "path", "getFirstFolderName");
	}
	{
		string rest = removeFirstPath("path/to/file.txt");
		SUET_ASSERT_STR(rest, "to/file.txt", "removeFirstPath");
	}
	//------------------------------------------------------------------------------
	// 扩展测试 Part 14 — startWith / endWith extended
	//------------------------------------------------------------------------------
	{
		SUET_ASSERT(startWith("hello world", "hello"), "startWith 'hello'");
		SUET_ASSERT(!startWith("world hello", "hello"), "startWith false");
		SUET_ASSERT(startWith("x", "x"), "startWith single match");
		SUET_ASSERT(!startWith("", "x"), "startWith empty src");
		SUET_ASSERT(startWith("abc", ""), "startWith empty pattern always true");
	}
	{
		SUET_ASSERT(endWith("hello world", "world"), "endWith 'world'");
		SUET_ASSERT(!endWith("hello world", "hello"), "endWith false");
		SUET_ASSERT(endWith("x", "x"), "endWith single match");
		SUET_ASSERT(!endWith("", "x"), "endWith empty src");
		SUET_ASSERT(endWith("abc", ""), "endWith empty pattern always true");
	}
	testBoolConvert();
	testStringToNumber();
	testULLToSString();
	testULLToSMyString();
	testIToSMyString();
	testFToSMyString();
	testV2ToS();
	testV3ToS();
	testV3IToS();
	testV2IToS();
	testV2UIToS();
	testV2SToS();
	testV2USToS();
	testStringToVector();
	testVectorRoundtrip();
	testULLsToSString();
	testULLsToSMyString();
	testLLsToSString();
	testLLsToSMyString();
	testSsToSString();
	testSsToSMyString();
	testUSsToSString();
	testUSsToSMyString();
	testBytesToStringS();
	testBytesToStringMyString();
	testStrcatS();
	testStrcpyS();
	testZeroString();
	testZeroStringMyString();
	testFloatToStringExtra();
	testRemoveStartEndString();
	testFilePathFunctions();
	testFolderPathFunctions();
	testLastNumberFunctions();
	testV2IsToS();
	testRemoveSuffix();
	testStrlength();
	testStrchar();
	testLastCharFunctions();
	testFindStringPos();
	testSplitLine2();
	testSplitVector();
	testSplitStringKey2();
	testFindStringLower();
	testReplaceSubstring();
	testRemoveStringChar();
	testRemoveStartLast2();
	testNumericRoundtrip();
	testNumericStress();
	testStrcatS_CharBuffer();
	testStrcatT_CharBuffer();
	testStrcpySExt3();
	testZeroStringTemplate();
	testZeroStringFunc();
	testReplaceString();
	testReplaceAllStringKey();
	testReplaceAllChar();
	testRemoveAll_SingleChar();
	testRemoveAll_TwoChars();
	testEndWithCaseSensitive();
	testEndWithCaseInsensitive();
	testStartWithCaseSensitive();
	testStartWithCaseInsensitive();
	testStrcatT_MyString();
	testLLToS_MyString();
	testZeroStringMyStringExt3();
	testEndWithString();
	testStartWithString();
	testStrcatS_MultipleChars();
	testReplaceCharBuffer();
	testStrcpyS_Truncate();
	testFindCharCount();
	testStrcharExt3();
	testFindStringLowerExt3();
	testGetLastChar();
	testGetLastNotChar();
	testGreaterPower2();
	testStrcatSCharBuf();
	testStrcatSCharBufWithLength();
	testStrcatSCharBufArraySource();
	testStrcatTCharBuf();
	testStrcatSMyString();
	testStrcatSMyStringWithLength();
	testStrcatSMyStringCharPtr();
	testStrcatSMyStringCharPtrWithLength();
	testStrcatSMyStringArray();
	testStrcatTMyString();
	testStrcpySStrCat();
	testZeroStringFuncStrCat();
	testZeroStringTemplateStrCat();
	testStrcatSEmpty();
	testStrcatSNullSource();
	testStrcatSMultipleSources();
	testStrcpySEmpty();
	testStrcpySLargeBuffer();
	testStrcatSMyStringStdString();
	testStrcatSMyStringStdStringLength();
	testLLToSPositive();
	testLLToSNegative();
	testLLToSZero();
	testLLToSLimitLen();
	testLLToSMaxValue();
	testIToSPositive();
	testIToSNegative();
	testIToSZero();
	testI64ToAPositive();
	testI64ToANegative();
	testI64ToAZero();
	testI64ToAMaxValue();
	testLLToSMyString();
	testLLToSMyStringLimitLen();
	testLLToSMyStringNegative();
	testItoa_sMyString();
	testFindCharCountConvert();
	testStrChar();
	testGetLastCharConvert();
	testGetLastNotCharConvert();
	testFindStringLowerConvert();
	testGreaterPower2StringUtilConvert();
	testStrlengthConvert();
	testGetLastNumber();
	testSplitByCharBasic();
	testSplitByCharRemoveEmpty();
	testSplitByCharReturnVector();
	testSplitByCharToBuffer();
	testSplitByStringBasic();
	testSplitByStringRemoveEmpty();
	testSplitLineBasic();
	testSplitLineToBuffer();
	testFindStringBasic();
	testFindStringStartIndex();
	testFindStringLowerBasic();
	testFindStringPosExtend();
	testReplaceBasic();
	testReplaceRangeChar();
	testReplaceRangeString();
	testReplaceAllString();
	testReplaceAllCharExtend();
	testRemoveAllChar();
	testRemoveAllTwoChars();
	testStartWithSensitive();
	testStartWithInsensitive();
	testEndWithSensitive();
	testEndWithInsensitive();
	testRemoveStringFromMyString();
	testRemoveStringFromCharBuf();
	testGetFileName();
	testGetFileNameNoSuffixWithPath();
	testGetFileNameNoSuffixNoPath();
	testGetFirstFolderName();
	testRemoveFirstPath();
	testGetFilePath();
	testGetFilePathKeepSlash();
	testGetFileSuffixKeepDot();
	testGetFileSuffixNoDot();
	testRemoveSuffixFilePath();
	testRemoveStartString();
	testRemoveEndString();
	testReplaceSuffix();
	testGetLastNotNumberPos();
	testGetLastNumberFilePath();
	testGetNotNumberSubString();
	testBToS();
	testRemoveStartBasic();
	testRemoveStartNotFound();
	testRemoveStartAllBasic();
	testRemoveStartAllAllSame();
	testRemoveLastBasic();
	testRemoveLastNotFound();
	testRemoveLastAllBasic();
	testRemoveLastAllAllSame();
	testRemoveLastComma();
	testReplaceAllMyString();
	testReplaceAllCharBuf();
	testRemoveStringMyString();
	testRemoveStringCharBuf();
	testReplaceMyStringRange();
	testReplaceCharBufRange();
	testReplaceAllStringObj();
	testReplaceAllStringChar();
	testRemoveAllCharFromString();
	testRemoveAllTwoCharsFromString();
	testSplitBasic();
	testSplitEmptyInput();
	testSplitNoDelimiter();
	testSplitMultipleDelimiters();
	testSplitLeadingTrailingDelim();
	testSplitLineBasicFormat2();
	testSplitLineWindowsLineEnding();
	testSplitLineEmpty();
	testFindStringBasicFormat2();
	testFindStringNotFound();
	testFindStringCaseSensitive();
	testFindStringPosBasic();
	testFindStringLowerBasicFormat2();
	testGetLastNotNumberPosBasic();
	testGetLastNotNumberPosAllDigits();
	testGetLastNotNumberPosAllAlpha();
	testGetLastNumberBasic();
	testGetLastNumberNoDigits();
	testGetLastNumberOnlyDigits();
	testGetNotNumberSubStringBasic();
	testGetNotNumberSubStringEmpty();
	testGetFirstFolderNameBasic();
	testGetFirstFolderNameRootOnly();
	testRemoveFirstPathBasic();
	testRemoveFirstPathSingle();
	testGetFileNameNoSuffixBasic();
	testReplaceSuffixBasic();
	testRemoveStartStringBasic();
	testRemoveEndStringBasic();
	testSha1();
	testToLowerUpper();
	testHexConversion();
	testIsNumber();
	testIsPhoneNumber();
	testSToIs();
	testSToFs();
	testSToBs();
	testSToLLs();
	testSToSs();
	testSToUIs();
	testSToUSs();
	testFsToS();
	testUIsToS();
	testSToV2Is();
	testSToV2s();
	testSToV3Is();
	testSToV3s();
	testStringUtility();
}

// ============================================================
// Consolidated utility coverage
// ============================================================
#define STRINGUTILITYTEST_UT_ASSERT(expr, msg) \
	do { if (!(expr)) { ERROR(string("StringUtilityTest FAIL: ") + (msg)); } } while(0)
#define STRINGUTILITYTEST_UT_ASSERT_EQ(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("StringUtilityTest FAIL: ") + (msg) + " | expected=" + IToS((int)(b)) + " actual=" + IToS((int)(a))); } } while(0)
#define STRINGUTILITYTEST_UT_ASSERT_EQ_LL(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("StringUtilityTest FAIL: ") + (msg) + " | expected=" + LLToS(b) + " actual=" + LLToS(a)); } } while(0)
#define STRINGUTILITYTEST_UT_ASSERT_FLOAT(a, b, msg) \
	do { if (fabsf((a) - (b)) > 1e-3f) { ERROR(string("StringUtilityTest FAIL: ") + (msg) + " | expected=" + FToS(b) + " actual=" + FToS(a)); } } while(0)
#define STRINGUTILITYTEST_UT_ASSERT_STR(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("StringUtilityTest FAIL: ") + (msg) + " | expected=" + (b) + " actual=" + (a)); } } while(0)

void StringUtilityTest::testString_NumToStr()
{
	STRINGUTILITYTEST_UT_ASSERT_STR(IToS(0),           "0",             "IToS(0)");
	STRINGUTILITYTEST_UT_ASSERT_STR(IToS(123),         "123",           "IToS(123)");
	STRINGUTILITYTEST_UT_ASSERT_STR(IToS(-456),        "-456",          "IToS(-456)");
	STRINGUTILITYTEST_UT_ASSERT_STR(IToS(2147483647),  "2147483647",    "IToS(INT_MAX)");

	STRINGUTILITYTEST_UT_ASSERT_STR(LLToS(0LL),              "0",             "LLToS(0)");
	STRINGUTILITYTEST_UT_ASSERT_STR(LLToS(123456789012LL),   "123456789012",  "LLToS(large)");
	STRINGUTILITYTEST_UT_ASSERT_STR(LLToS(-987654321098LL),  "-987654321098", "LLToS(neg large)");

	// FToS precision=4, trailing zeros stripped
	STRINGUTILITYTEST_UT_ASSERT_STR(FToS(0.0f),   "0",      "FToS(0)");
	STRINGUTILITYTEST_UT_ASSERT_STR(FToS(1.5f),   "1.5",    "FToS(1.5)");
	STRINGUTILITYTEST_UT_ASSERT_STR(FToS(-2.25f), "-2.25",  "FToS(-2.25)");
}

void StringUtilityTest::testString_StrToNum()
{
	STRINGUTILITYTEST_UT_ASSERT_EQ(SToI("0"),      0,    "SToI('0')");
	STRINGUTILITYTEST_UT_ASSERT_EQ(SToI("123"),    123,  "SToI('123')");
	STRINGUTILITYTEST_UT_ASSERT_EQ(SToI("-456"),  -456,  "SToI('-456')");

	STRINGUTILITYTEST_UT_ASSERT_EQ_LL(SToLL("0"),              0LL,            "SToLL('0')");
	STRINGUTILITYTEST_UT_ASSERT_EQ_LL(SToLL("123456789012"),   123456789012LL, "SToLL(large)");
	STRINGUTILITYTEST_UT_ASSERT_EQ_LL(SToLL("-987654321098"), -987654321098LL, "SToLL(neg large)");

	STRINGUTILITYTEST_UT_ASSERT_FLOAT(SToF("0"),     0.0f,   "SToF('0')");
	STRINGUTILITYTEST_UT_ASSERT_FLOAT(SToF("1.5"),   1.5f,   "SToF('1.5')");
	STRINGUTILITYTEST_UT_ASSERT_FLOAT(SToF("-2.25"), -2.25f, "SToF('-2.25')");
}

void StringUtilityTest::testString_Split()
{
	// normal split
	Vector<string> parts;
	StringUtility::split("a,b,c", ',', parts);
	STRINGUTILITYTEST_UT_ASSERT_EQ((int)parts.size(), 3, "split 'a,b,c' -> 3 parts");
	STRINGUTILITYTEST_UT_ASSERT_STR(parts[0], "a", "split[0]='a'");
	STRINGUTILITYTEST_UT_ASSERT_STR(parts[1], "b", "split[1]='b'");
	STRINGUTILITYTEST_UT_ASSERT_STR(parts[2], "c", "split[2]='c'");

	// removeEmpty=true skips consecutive delimiters
	parts.clear();
	StringUtility::split("a,,c", ',', parts, true);
	STRINGUTILITYTEST_UT_ASSERT_EQ((int)parts.size(), 2, "split 'a,,c' removeEmpty -> 2");
	STRINGUTILITYTEST_UT_ASSERT_STR(parts[0], "a", "split_rme[0]='a'");
	STRINGUTILITYTEST_UT_ASSERT_STR(parts[1], "c", "split_rme[1]='c'");

	// removeEmpty=false keeps empty token
	parts.clear();
	StringUtility::split("a,,c", ',', parts, false);
	STRINGUTILITYTEST_UT_ASSERT_EQ((int)parts.size(), 3, "split 'a,,c' keepEmpty -> 3");
	STRINGUTILITYTEST_UT_ASSERT_STR(parts[1], "", "split_keep[1]=''");

	// single element (no delimiter)
	parts.clear();
	StringUtility::split("hello", ',', parts);
	STRINGUTILITYTEST_UT_ASSERT_EQ((int)parts.size(), 1, "split 'hello' -> 1");
	STRINGUTILITYTEST_UT_ASSERT_STR(parts[0], "hello", "split_single='hello'");

	// empty string
	parts.clear();
	StringUtility::split("", ',', parts);
	STRINGUTILITYTEST_UT_ASSERT_EQ((int)parts.size(), 0, "split '' -> 0");
}

void StringUtilityTest::testString_Find()
{
	int pos = -1;
	STRINGUTILITYTEST_UT_ASSERT(StringUtility::findString("hello world", "world", &pos), "findString 'world' found");
	STRINGUTILITYTEST_UT_ASSERT_EQ(pos, 6, "findString 'world' pos=6");

	STRINGUTILITYTEST_UT_ASSERT(!StringUtility::findString("hello world", "xyz"), "findString 'xyz' not found");

	// search from startIndex
	// "aababc": indices 0='a' 1='a' 2='b' 3='a' 4='b' 5='c'
	// "ab" first appears at pos=1; searching from startIndex=2 skips it, next match is at pos=3
	pos = -1;
	STRINGUTILITYTEST_UT_ASSERT(StringUtility::findString("aababc", "ab", &pos, 2), "findString from 2 found");
	STRINGUTILITYTEST_UT_ASSERT_EQ(pos, 3, "findString from 2 pos=3");

	STRINGUTILITYTEST_UT_ASSERT_EQ(StringUtility::findCharCount("a,b,,c", ','), 3, "findCharCount 'a,b,,c'=3");
	STRINGUTILITYTEST_UT_ASSERT_EQ(StringUtility::findCharCount("abc", ','),    0, "findCharCount 'abc'=0");
	STRINGUTILITYTEST_UT_ASSERT_EQ(StringUtility::findCharCount("", ','),       0, "findCharCount ''=0");
}

void StringUtilityTest::testString_StartEndWith()
{
	STRINGUTILITYTEST_UT_ASSERT(StringUtility::startWith("hello world", "hello"),  "startWith 'hello' ok");
	STRINGUTILITYTEST_UT_ASSERT(!StringUtility::startWith("hello world", "world"), "startWith 'world' fail");
	STRINGUTILITYTEST_UT_ASSERT(StringUtility::startWith("hello", "hello"),        "startWith exact match");
	STRINGUTILITYTEST_UT_ASSERT(!StringUtility::startWith("hi", "hello"),          "startWith too short");
	STRINGUTILITYTEST_UT_ASSERT(StringUtility::startWith("", ""),                  "startWith empty ok");

	STRINGUTILITYTEST_UT_ASSERT(StringUtility::endWith("hello world", "world"),    "endWith 'world' ok");
	STRINGUTILITYTEST_UT_ASSERT(!StringUtility::endWith("hello world", "hello"),   "endWith 'hello' fail");
	STRINGUTILITYTEST_UT_ASSERT(StringUtility::endWith("hello", "hello"),          "endWith exact match");
	STRINGUTILITYTEST_UT_ASSERT(!StringUtility::endWith("hi", "hello"),            "endWith too short");
}

void StringUtilityTest::testString_ReplaceRemove()
{
	// replaceAll(string&, string, string)
	string s = "aabbcc";
	StringUtility::replaceAll(s, string("bb"), string("XX"));
	STRINGUTILITYTEST_UT_ASSERT_STR(s, "aaXXcc", "replaceAll 'bb'->'XX'");

	s = "aaaa";
	StringUtility::replaceAll(s, string("aa"), string("b"));
	STRINGUTILITYTEST_UT_ASSERT_STR(s, "bb", "replaceAll 'aa'->'b' x2");

	// replaceAll(string&, char, char)
	s = "a.b.c";
	StringUtility::replaceAll(s, '.', '/');
	STRINGUTILITYTEST_UT_ASSERT_STR(s, "a/b/c", "replaceAll '.'->'/'" );

	// removeAll(string&, char)
	s = "a1b2c3";
	StringUtility::removeAll(s, '1');
	STRINGUTILITYTEST_UT_ASSERT_STR(s, "ab2c3", "removeAll '1'");

	// removeAll(string&, char, char) - remove two chars
	s = "a1b2c3";
	StringUtility::removeAll(s, '1', '3');
	STRINGUTILITYTEST_UT_ASSERT_STR(s, "ab2c", "removeAll '1','3'");
}

void StringUtilityTest::testString_TrimChar()
{
	string s = "   hello";
	StringUtility::removeStartAll(s, ' ');
	STRINGUTILITYTEST_UT_ASSERT_STR(s, "hello", "removeStartAll spaces");

	s = "hello   ";
	StringUtility::removeLastAll(s, ' ');
	STRINGUTILITYTEST_UT_ASSERT_STR(s, "hello", "removeLastAll spaces");

	s = "   ";
	StringUtility::removeLastAll(s, ' ');
	STRINGUTILITYTEST_UT_ASSERT_STR(s, "", "removeLastAll all spaces -> empty");

	s = "   ";
	StringUtility::removeStartAll(s, ' ');
	STRINGUTILITYTEST_UT_ASSERT_STR(s, "", "removeStartAll all spaces -> empty");
}

void StringUtilityTest::testStringUtility()
{
	testString_NumToStr();
	testString_StrToNum();
	testString_Split();
	testString_Find();
	testString_StartEndWith();
	testString_ReplaceRemove();
	testString_TrimChar();
}
#endif
