#ifdef WINDOWS
#include "FrameHeader.h"

#define LIT_ASSERT(expr, msg) \
	do { if (!(expr)) { ERROR(string("LogInfoTest FAILED: ") + (msg)); } } while(0)

void LogInfoTest::test()
{
	testDefaultState();
	testResetPropertyClearsAllFields();
	testResetPropertyAfterModification();
}

// 1. 构造后默认值全为零/空/false
void LogInfoTest::testDefaultState()
{
	LogInfo info;
	LIT_ASSERT(info.mPlayerGUID == 0, "default mPlayerGUID = 0");
	LIT_ASSERT(info.mTime == 0, "default mTime = 0");
	LIT_ASSERT(info.mLog.empty(), "default mLog is empty");
	LIT_ASSERT(info.mStack.empty(), "default mStack is empty");
	LIT_ASSERT(!info.mError, "default mError = false");
}

// 2. resetProperty 将所有字段恢复默认值
void LogInfoTest::testResetPropertyClearsAllFields()
{
	// 先设置非默认值
	LogInfo info;
	info.mPlayerGUID = 99999;
	info.mTime = 1234567890;
	info.mLog = "test log message";
	info.mStack = "stack trace here";
	info.mError = true;

	// 调用resetProperty
	info.resetProperty();

	// 验证全部归零
	LIT_ASSERT(info.mPlayerGUID == 0, "after reset: mPlayerGUID = 0");
	LIT_ASSERT(info.mTime == 0, "after reset: mTime = 0");
	LIT_ASSERT(info.mLog.empty(), "after reset: mLog is empty");
	LIT_ASSERT(info.mStack.empty(), "after reset: mStack is empty");
	LIT_ASSERT(!info.mError, "after reset: mError = false");
}

// 3. 多次修改+reset 循环不泄漏状态
void LogInfoTest::testResetPropertyAfterModification()
{
	LogInfo info;

	// 第一轮
	info.mPlayerGUID = 111;
	info.mLog = "first round";
	info.mError = true;
	info.resetProperty();
	LIT_ASSERT(info.mPlayerGUID == 0 && info.mLog.empty() && !info.mError, "round1 reset complete");

	// 第二轮（不同值）
	info.mPlayerGUID = 22222;
	info.mTime = 9876543210;
	info.mStack = "second round stack";
	info.resetProperty();
	LIT_ASSERT(info.mPlayerGUID == 0 && info.mTime == 0 && info.mStack.empty(), "round2 reset complete");

	// 第三轮（仅部分字段）
	info.mError = true;
	info.resetProperty();
	LIT_ASSERT(!info.mError, "round3: error reset to false");
}
#endif
