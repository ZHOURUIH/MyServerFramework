#ifdef WINDOWS
#include "FrameHeader.h"

// TP_ASSERT: TimePointTest 专用断言宏
#define TP_ASSERT(expr) \
	if (!(expr)) { ERROR(string("TimePointTest FAILED: ") + (#expr)); }

// ========== 辅助子类：暴露 protected 字段供测试 ==========
class TestTP : public TimePointInDay
{
	BASE(TestTP, TimePointInDay);
public:
	TimeValue&          getTime()        { return mTime; }
	VoidCallback&		getCallback()	 { return mCallback; }
	float&              getArrivalCD()   { return mArrivalCD; }
	bool&               getArrivalTime() { return mArrivalTime; }
};

// ========== TimePointInDay::setParam 属性测试 ==========
void TimePointTest::testTimePointInDay()
{
	TestTP tp;
	// setParam 设置 hour/minute，second 固定为 0
	tp.setParam(8, 30);
	TP_ASSERT(tp.getTime().mHour == 8);
	TP_ASSERT(tp.getTime().mMinute == 30);
	TP_ASSERT(tp.getTime().mSecond == 0);

	tp.setParam(23, 59);
	TP_ASSERT(tp.getTime().mHour == 23);
	TP_ASSERT(tp.getTime().mMinute == 59);
	TP_ASSERT(tp.getTime().mSecond == 0);

	tp.setParam(0, 0);
	TP_ASSERT(tp.getTime().mHour == 0);
	TP_ASSERT(tp.getTime().mMinute == 0);
	TP_ASSERT(tp.getTime().mSecond == 0);

	// resetProperty 清零
	tp.setParam(12, 30);
	tp.resetProperty();
	TP_ASSERT(tp.getTime().mHour == 0);
	TP_ASSERT(tp.getTime().mMinute == 0);
	TP_ASSERT(tp.getTime().mSecond == 0);

	// 多次 setParam
	for (int h = 0; h < 24; ++h)
	{
		tp.setParam(h, h % 60);
		TP_ASSERT(tp.getTime().mHour == h);
		TP_ASSERT(tp.getTime().mMinute == h % 60);
		TP_ASSERT(tp.getTime().mSecond == 0);
	}

}

// ========== TimePoint::resetProperty 测试 ==========
void TimePointTest::testTimePointResetProperty()
{
	// TimePoint 是 ClassObject 子类，不能直接 new，测试 resetProperty 副作用
	// 通过 TestTP（TestTP -> TimePointInDay -> TimePoint）间接验证
	TestTP tp;
	tp.setParam(15, 45);
	tp.setCallback(nullptr);
	tp.resetProperty();

	// resetProperty 后回调/userData 置空
	TP_ASSERT(tp.getCallback() == nullptr);
	TP_ASSERT(tp.getArrivalCD() == -1.0f);
	TP_ASSERT(tp.getArrivalTime() == false);
	TP_ASSERT(tp.getTime().mHour == 0);
	TP_ASSERT(tp.getTime().mMinute == 0);

}

// ========== TimePoint setCallback/userData 测试 ==========
void TimePointTest::testTimePointCallback()
{
	// 验证 setCallback 后 mUserData 被正确存储，且 mCallback 不为 null
	int callCount = 0;
	TestTP tp;
	tp.setCallback([&callCount]() { ++callCount; });
	// mCallback 非空，mUserData 指向 callCount
	TP_ASSERT(tp.getCallback() != nullptr);
	// 实际调用回调，验证功能
	tp.getCallback()();
	TP_ASSERT(callCount == 1);

	// 多次 setCallback 覆盖
	int otherCount = 0;
	tp.setCallback([&otherCount]() { otherCount += 10; });
	TP_ASSERT(tp.getCallback() != nullptr);
	tp.getCallback()();
	TP_ASSERT(otherCount == 10);

	// setCallback(nullptr, nullptr) 清空
	tp.setCallback(nullptr);
	TP_ASSERT(tp.getCallback() == nullptr);
}

// ========== 主入口 ==========
void TimePointTest::test()
{
	testTimePointInDay();
	testTimePointResetProperty();
	testTimePointCallback();
}
#endif
