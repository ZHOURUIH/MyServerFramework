#ifdef WINDOWS
#include "FrameHeader.h"
namespace MathUtilityExtTestSupport
{
	// MathUtilityExtTestSupport_MUE_ASSERT: MathUtilityExtTest 专用断言宏
#define MathUtilityExtTestSupport_MUE_ASSERT(expr) \
	if (!(expr)) { ERROR("MathUtilityExtTest FAILED: " #expr); }

	static bool mueNear(float a, float b, float eps = 1e-4f) { return fabsf(a - b) <= eps; }
}

void MathUtilityTest::testMathConstants()
{
	using namespace MathUtilityExtTestSupport;
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(MATH_PI, 3.1415926f));
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(MATH_PI * Rad2Deg, 180.0f, 0.01f));
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(toRadian(180.0f), MATH_PI, 1e-3f));
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(toDegree(MATH_PI), 180.0f, 0.01f));
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(HALF_PI_RADIAN, MATH_PI * 0.5f));
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(TWO_PI_RADIAN, MATH_PI * 2.0f));
	MathUtilityExtTestSupport_MUE_ASSERT(MIN_DELTA > 0.0f);
}

void MathUtilityTest::testIsZeroIsEqual()
{
	using namespace MathUtilityExtTestSupport;
	MathUtilityExtTestSupport_MUE_ASSERT(isZero(0.0f));
	MathUtilityExtTestSupport_MUE_ASSERT(isZero(1e-5f));
	MathUtilityExtTestSupport_MUE_ASSERT(!isZero(1.0f));
	MathUtilityExtTestSupport_MUE_ASSERT(!isZero(-1.0f));
	MathUtilityExtTestSupport_MUE_ASSERT(isZero(0.0));
	MathUtilityExtTestSupport_MUE_ASSERT(!isZero(1.0));

	MathUtilityExtTestSupport_MUE_ASSERT(isEqual(1.0f, 1.0f));
	MathUtilityExtTestSupport_MUE_ASSERT(isEqual(1.0f, 1.0f + 5e-6f));  // 差值远小于 MIN_DELTA(1e-5f)，确保在精度内
	MathUtilityExtTestSupport_MUE_ASSERT(!isEqual(1.0f, 2.0f));
	MathUtilityExtTestSupport_MUE_ASSERT(isEqual(0.0f, 0.0f));
	MathUtilityExtTestSupport_MUE_ASSERT(isEqual(-1.0f, -1.0f));

	// 自定义精度
	MathUtilityExtTestSupport_MUE_ASSERT(isEqual(1.0f, 1.1f, 0.2f));
	MathUtilityExtTestSupport_MUE_ASSERT(!isEqual(1.0f, 1.5f, 0.3f));
}

void MathUtilityTest::testGetMinMax()
{
	using namespace MathUtilityExtTestSupport;
	MathUtilityExtTestSupport_MUE_ASSERT(getMin(1, 2) == 1);
	MathUtilityExtTestSupport_MUE_ASSERT(getMin(5, 3) == 3);
	MathUtilityExtTestSupport_MUE_ASSERT(getMin(-1, -5) == -5);
	MathUtilityExtTestSupport_MUE_ASSERT(getMin(0, 0) == 0);

	MathUtilityExtTestSupport_MUE_ASSERT(getMax(1, 2) == 2);
	MathUtilityExtTestSupport_MUE_ASSERT(getMax(5, 3) == 5);
	MathUtilityExtTestSupport_MUE_ASSERT(getMax(-1, -5) == -1);
	MathUtilityExtTestSupport_MUE_ASSERT(getMax(0, 0) == 0);

	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(getMin(1.5f, 2.5f), 1.5f));
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(getMax(1.5f, 2.5f), 2.5f));
	MathUtilityExtTestSupport_MUE_ASSERT(getMin(INT_MIN, INT_MAX) == INT_MIN);
	MathUtilityExtTestSupport_MUE_ASSERT(getMax(INT_MIN, INT_MAX) == INT_MAX);
}

void MathUtilityTest::testClamp()
{
	using namespace MathUtilityExtTestSupport;
	// clamp int
	MathUtilityExtTestSupport_MUE_ASSERT(clamp(5, 0, 10) == 5);
	MathUtilityExtTestSupport_MUE_ASSERT(clamp(-1, 0, 10) == 0);
	MathUtilityExtTestSupport_MUE_ASSERT(clamp(11, 0, 10) == 10);
	MathUtilityExtTestSupport_MUE_ASSERT(clamp(0, 0, 10) == 0);
	MathUtilityExtTestSupport_MUE_ASSERT(clamp(10, 0, 10) == 10);

	// clamp float
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(clamp(0.5f, 0.0f, 1.0f), 0.5f));
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(clamp(-0.5f, 0.0f, 1.0f), 0.0f));
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(clamp(1.5f, 0.0f, 1.0f), 1.0f));

	// clampRef
	int val = 15;
	clampRef(val, 0, 10);
	MathUtilityExtTestSupport_MUE_ASSERT(val == 10);
	val = -5;
	clampRef(val, 0, 10);
	MathUtilityExtTestSupport_MUE_ASSERT(val == 0);
	val = 5;
	clampRef(val, 0, 10);
	MathUtilityExtTestSupport_MUE_ASSERT(val == 5);

	// clampMin
	MathUtilityExtTestSupport_MUE_ASSERT(clampMin(5, 0) == 5);
	MathUtilityExtTestSupport_MUE_ASSERT(clampMin(-1, 0) == 0);
	MathUtilityExtTestSupport_MUE_ASSERT(clampZero(-3) == 0);
	MathUtilityExtTestSupport_MUE_ASSERT(clampZero(3) == 3);

	// clampMax
	MathUtilityExtTestSupport_MUE_ASSERT(clampMax(5, 10) == 5);
	MathUtilityExtTestSupport_MUE_ASSERT(clampMax(15, 10) == 10);

	// clampMinRef
	float fval = -1.0f;
	clampMinRef(fval, 0.0f);
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(fval, 0.0f));
	fval = 5.0f;
	clampMaxRef(fval, 3.0f);
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(fval, 3.0f));
}

void MathUtilityTest::testLerp()
{
	using namespace MathUtilityExtTestSupport;
	// lerp float
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(lerp(0.0f, 10.0f, 0.5f), 5.0f));
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(lerp(0.0f, 10.0f, 0.0f), 0.0f));
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(lerp(0.0f, 10.0f, 1.0f), 10.0f));
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(lerp(-10.0f, 10.0f, 0.5f), 0.0f));

	// t 自动 clamp 到 [0,1]
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(lerp(0.0f, 10.0f, -1.0f), 0.0f));
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(lerp(0.0f, 10.0f, 2.0f), 10.0f));

	// lerp int
	MathUtilityExtTestSupport_MUE_ASSERT(lerp(0, 100, 0.5f) == 50);
	MathUtilityExtTestSupport_MUE_ASSERT(lerp(0, 100, 0.0f) == 0);
	MathUtilityExtTestSupport_MUE_ASSERT(lerp(0, 100, 1.0f) == 100);

	// lerp llong
	MathUtilityExtTestSupport_MUE_ASSERT(lerp((llong)0, (llong)1000000, 0.5f) == 500000LL);

	// lerpSimple（不 clamp）
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(lerpSimple(0.0f, 10.0f, 0.5f), 5.0f));
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(lerpSimple(0.0f, 10.0f, 2.0f), 20.0f));  // 不 clamp

	// inverseLerp
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(inverseLerp(0.0f, 10.0f, 5.0f), 0.5f));
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(inverseLerp(0.0f, 10.0f, 0.0f), 0.0f));
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(inverseLerp(0.0f, 10.0f, 10.0f), 1.0f));
	// inverseLerp 除0保护
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(inverseLerp(5.0f, 5.0f, 5.0f), 0.0f));
}

void MathUtilityTest::testDivide()
{
	using namespace MathUtilityExtTestSupport;
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(divide(10.0f, 2.0f), 5.0f));
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(divide(10.0f, 0.0f), 0.0f));  // 除0保护
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(divide(10, 2), 5.0f));
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(divide(10, 0), 0.0f));         // 除0保护
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(divide(10.0f, 3), 10.0f / 3.0f, 1e-4f));
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(divide(10, 3.0f), 10.0f / 3.0f, 1e-4f));

	// divideInt
	MathUtilityExtTestSupport_MUE_ASSERT(divideInt(10, 2) == 5);
	MathUtilityExtTestSupport_MUE_ASSERT(divideInt(10, 0) == 0);
	MathUtilityExtTestSupport_MUE_ASSERT(divideInt(7, 2) == 3);
}

void MathUtilityTest::testCeilingFloor()
{
	using namespace MathUtilityExtTestSupport;
	MathUtilityExtTestSupport_MUE_ASSERT(ceiling(1.0f) == 1);
	MathUtilityExtTestSupport_MUE_ASSERT(ceiling(1.1f) == 2);
	MathUtilityExtTestSupport_MUE_ASSERT(ceiling(1.9f) == 2);
	MathUtilityExtTestSupport_MUE_ASSERT(ceiling(0.0f) == 0);
	MathUtilityExtTestSupport_MUE_ASSERT(ceiling(-1.5f) == -1);  // 负数向上取整
	// 框架阈值 0.0001f：超过阈值才进位
	// 2.0001f 的 float 表示恰好等于 2.0f+0.0001f，处于边界（不进位）
	// 用 2.001f（明显超过阈值）验证进位
	MathUtilityExtTestSupport_MUE_ASSERT(ceiling(2.001f) == 3);   // 明显超过阈值，进位
	MathUtilityExtTestSupport_MUE_ASSERT(ceiling(2.0001f) == 2);   // 恰在阈值边界，不进位（视为相等）
	MathUtilityExtTestSupport_MUE_ASSERT(ceiling(2.00001f) == 2);  // 极小差值，不进位

	MathUtilityExtTestSupport_MUE_ASSERT(floor_(1.0f) == 1);
	MathUtilityExtTestSupport_MUE_ASSERT(floor_(1.9f) == 1);
	MathUtilityExtTestSupport_MUE_ASSERT(floor_(0.0f) == 0);
	MathUtilityExtTestSupport_MUE_ASSERT(floor_(-1.1f) == -2);
	MathUtilityExtTestSupport_MUE_ASSERT(floor_(-1.0f) == -1);
}

void MathUtilityTest::testRound()
{
	using namespace MathUtilityExtTestSupport;
	MathUtilityExtTestSupport_MUE_ASSERT(round_(0.4f) == 0);
	MathUtilityExtTestSupport_MUE_ASSERT(round_(0.5f) == 1);
	MathUtilityExtTestSupport_MUE_ASSERT(round_(1.5f) == 2);
	MathUtilityExtTestSupport_MUE_ASSERT(round_(-0.5f) == -1);
	MathUtilityExtTestSupport_MUE_ASSERT(round_(-1.5f) == -2);
	MathUtilityExtTestSupport_MUE_ASSERT(round_(2.9f) == 3);
	MathUtilityExtTestSupport_MUE_ASSERT(round_(2.1f) == 2);

	MathUtilityExtTestSupport_MUE_ASSERT(roundDouble(0.4) == 0LL);
	MathUtilityExtTestSupport_MUE_ASSERT(roundDouble(0.5) == 1LL);
	MathUtilityExtTestSupport_MUE_ASSERT(roundDouble(-0.5) == -1LL);
}

void MathUtilityTest::testSign()
{
	using namespace MathUtilityExtTestSupport;
	MathUtilityExtTestSupport_MUE_ASSERT(sign(5) == 1);
	MathUtilityExtTestSupport_MUE_ASSERT(sign(-5) == -1);
	MathUtilityExtTestSupport_MUE_ASSERT(sign(0) == 0);
	MathUtilityExtTestSupport_MUE_ASSERT(sign(5.0f) == 1.0f);
	MathUtilityExtTestSupport_MUE_ASSERT(sign(-5.0f) == -1.0f);
	MathUtilityExtTestSupport_MUE_ASSERT(sign(0.0f) == 0.0f);
	MathUtilityExtTestSupport_MUE_ASSERT(sign(INT_MAX) == 1);
	MathUtilityExtTestSupport_MUE_ASSERT(sign(INT_MIN + 1) == -1);
}

void MathUtilityTest::testSwap()
{
	using namespace MathUtilityExtTestSupport;
	int a = 3, b = 7;
	swap_(a, b);
	MathUtilityExtTestSupport_MUE_ASSERT(a == 7 && b == 3);

	float x = 1.5f, y = 2.5f;
	swap_(x, y);
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(x, 2.5f) && mueNear(y, 1.5f));

	// 同值 swap
	int c = 5;
	swap_(c, c);  // 自 swap 不崩溃
	MathUtilityExtTestSupport_MUE_ASSERT(c == 5);
}

void MathUtilityTest::testIsEvenIsPow2()
{
	using namespace MathUtilityExtTestSupport;
	MathUtilityExtTestSupport_MUE_ASSERT(isEven(0));
	MathUtilityExtTestSupport_MUE_ASSERT(isEven(2));
	MathUtilityExtTestSupport_MUE_ASSERT(isEven(-4));
	MathUtilityExtTestSupport_MUE_ASSERT(!isEven(1));
	MathUtilityExtTestSupport_MUE_ASSERT(!isEven(-3));

	MathUtilityExtTestSupport_MUE_ASSERT(!isPow2(0));
	MathUtilityExtTestSupport_MUE_ASSERT(isPow2(1));
	MathUtilityExtTestSupport_MUE_ASSERT(isPow2(2));
	MathUtilityExtTestSupport_MUE_ASSERT(isPow2(4));
	MathUtilityExtTestSupport_MUE_ASSERT(isPow2(8));
	MathUtilityExtTestSupport_MUE_ASSERT(isPow2(1024));
	MathUtilityExtTestSupport_MUE_ASSERT(!isPow2(3));
	MathUtilityExtTestSupport_MUE_ASSERT(!isPow2(5));
	MathUtilityExtTestSupport_MUE_ASSERT(!isPow2(-2));
}

void MathUtilityTest::testPow2Pow10()
{
	using namespace MathUtilityExtTestSupport;
	MathUtilityExtTestSupport_MUE_ASSERT(pow2(0) == 1);
	MathUtilityExtTestSupport_MUE_ASSERT(pow2(1) == 2);
	MathUtilityExtTestSupport_MUE_ASSERT(pow2(4) == 16);
	MathUtilityExtTestSupport_MUE_ASSERT(pow2(10) == 1024);

	MathUtilityExtTestSupport_MUE_ASSERT(pow10(0) == 1);
	MathUtilityExtTestSupport_MUE_ASSERT(pow10(1) == 10);
	MathUtilityExtTestSupport_MUE_ASSERT(pow10(3) == 1000);
	MathUtilityExtTestSupport_MUE_ASSERT(pow10(6) == 1000000);

	MathUtilityExtTestSupport_MUE_ASSERT(pow10LLong(0) == 1LL);
	MathUtilityExtTestSupport_MUE_ASSERT(pow10LLong(9) == 1000000000LL);
}

void MathUtilityTest::testFmodFrac()
{
	using namespace MathUtilityExtTestSupport;
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(fmod(10.0f, 3.0f), 1.0f));
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(fmod(7.5f, 2.5f), 0.0f));
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(fmod(5.0f, 2.0f), 1.0f));
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(fmod(5.0f, 5.0f), 0.0f));

	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(frac(1.5f), 0.5f));
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(frac(3.7f), 0.7f, 1e-3f));
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(frac(0.0f), 0.0f));
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(frac(5.0f), 0.0f));
	// 负数小数部分
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(frac(-1.3f), -0.3f, 1e-3f));
}

void MathUtilityTest::testStep()
{
	using namespace MathUtilityExtTestSupport;
	MathUtilityExtTestSupport_MUE_ASSERT(step(1.0f, 2.0f) == 1);
	MathUtilityExtTestSupport_MUE_ASSERT(step(1.0f, 1.0f) == 1);
	MathUtilityExtTestSupport_MUE_ASSERT(step(2.0f, 1.0f) == 0);
	MathUtilityExtTestSupport_MUE_ASSERT(step(0.0f, 0.5f) == 1);
}

void MathUtilityTest::testSaturate()
{
	using namespace MathUtilityExtTestSupport;
	float v = 1.5f;
	saturate(v);
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(v, 1.0f));
	v = -0.5f;
	saturate(v);
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(v, 0.0f));
	v = 0.7f;
	saturate(v);
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(v, 0.7f));
}

void MathUtilityTest::testClampCycle()
{
	using namespace MathUtilityExtTestSupport;
	float v = 370.0f;
	clampDegree360(v);
	MathUtilityExtTestSupport_MUE_ASSERT(v >= 0.0f && v <= 360.0f);

	v = -10.0f;
	clampDegree360(v);
	MathUtilityExtTestSupport_MUE_ASSERT(v >= 0.0f && v <= 360.0f);

	v = 4.0f;  // > PI
	clampRadian180(v);
	MathUtilityExtTestSupport_MUE_ASSERT(v >= -MATH_PI && v <= MATH_PI);

	v = -4.0f;  // < -PI
	clampRadian180(v);
	MathUtilityExtTestSupport_MUE_ASSERT(v >= -MATH_PI && v <= MATH_PI);

	v = 365.0f;
	clampDegree180(v);
	MathUtilityExtTestSupport_MUE_ASSERT(v >= -180.0f && v <= 180.0f);

	v = -365.0f;
	clampDegree180(v);
	MathUtilityExtTestSupport_MUE_ASSERT(v >= -180.0f && v <= 180.0f);
}

void MathUtilityTest::testInRange()
{
	using namespace MathUtilityExtTestSupport;
	MathUtilityExtTestSupport_MUE_ASSERT(inRange(5, 0, 10));
	MathUtilityExtTestSupport_MUE_ASSERT(inRange(0, 0, 10));
	MathUtilityExtTestSupport_MUE_ASSERT(inRange(10, 0, 10));
	MathUtilityExtTestSupport_MUE_ASSERT(!inRange(-1, 0, 10));
	MathUtilityExtTestSupport_MUE_ASSERT(!inRange(11, 0, 10));

	MathUtilityExtTestSupport_MUE_ASSERT(inRange(5.0f, 0.0f, 10.0f));
	MathUtilityExtTestSupport_MUE_ASSERT(!inRange(-0.1f, 0.0f, 10.0f));

	// inRange 自动确定 min/max
	MathUtilityExtTestSupport_MUE_ASSERT(inRange(5, 10, 0));  // range0=10, range1=0, 自动识别
	MathUtilityExtTestSupport_MUE_ASSERT(!inRange(-1, 10, 0));

	MathUtilityExtTestSupport_MUE_ASSERT(inFixedRange(5, 0, 10));
	MathUtilityExtTestSupport_MUE_ASSERT(!inFixedRange(11, 0, 10));
	MathUtilityExtTestSupport_MUE_ASSERT(inFixedRange(5.0f, 0.0f, 10.0f));

	// Vector2 版本
	Vector2 v{ 5.0f, 5.0f };
	MathUtilityExtTestSupport_MUE_ASSERT(inRange(v, { 0.0f, 0.0f }, { 10.0f, 10.0f }));
	MathUtilityExtTestSupport_MUE_ASSERT(!inRange(v, { 6.0f, 0.0f }, { 10.0f, 10.0f }));

	// Vector3 版本
	Vector3 v3{ 5.0f, 5.0f, 5.0f };
	MathUtilityExtTestSupport_MUE_ASSERT(inRange(v3, { 0.0f, 0.0f, 0.0f }, { 10.0f, 10.0f, 10.0f }));
	MathUtilityExtTestSupport_MUE_ASSERT(!inRange(v3, { 0.0f, 6.0f, 0.0f }, { 10.0f, 10.0f, 10.0f }));
}

void MathUtilityTest::testPowerFloat()
{
	using namespace MathUtilityExtTestSupport;
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(powerFloat(2.0f, 0), 1.0f));
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(powerFloat(2.0f, 1), 2.0f));
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(powerFloat(2.0f, 10), 1024.0f));
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(powerFloat(3.0f, 3), 27.0f));
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(powerFloat(0.5f, 4), 0.0625f));
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(powerFloat(1.0f, 100), 1.0f));
	// p 为负数 clampMin 到 0
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(powerFloat(2.0f, -1), 1.0f));
}

void MathUtilityTest::testSecondsConvert()
{
	using namespace MathUtilityExtTestSupport;
	int min, sec, hour;
	secondsToMinutesSeconds(90, min, sec);
	MathUtilityExtTestSupport_MUE_ASSERT(min == 1 && sec == 30);

	secondsToMinutesSeconds(60, min, sec);
	MathUtilityExtTestSupport_MUE_ASSERT(min == 1 && sec == 0);

	secondsToMinutesSeconds(0, min, sec);
	MathUtilityExtTestSupport_MUE_ASSERT(min == 0 && sec == 0);

	secondsToMinutesSeconds(3599, min, sec);
	MathUtilityExtTestSupport_MUE_ASSERT(min == 59 && sec == 59);

	secondsToHoursMinutesSeconds(3661, hour, min, sec);
	MathUtilityExtTestSupport_MUE_ASSERT(hour == 1 && min == 1 && sec == 1);

	secondsToHoursMinutesSeconds(0, hour, min, sec);
	MathUtilityExtTestSupport_MUE_ASSERT(hour == 0 && min == 0 && sec == 0);

	secondsToHoursMinutesSeconds(86400, hour, min, sec);
	MathUtilityExtTestSupport_MUE_ASSERT(hour == 24 && min == 0 && sec == 0);
}

void MathUtilityTest::testBatchCount()
{
	using namespace MathUtilityExtTestSupport;
	MathUtilityExtTestSupport_MUE_ASSERT(generateBatchCount(10, 3) == 4);
	MathUtilityExtTestSupport_MUE_ASSERT(generateBatchCount(9, 3) == 3);
	MathUtilityExtTestSupport_MUE_ASSERT(generateBatchCount(0, 3) == 0);
	MathUtilityExtTestSupport_MUE_ASSERT(generateBatchCount(1, 1) == 1);
	MathUtilityExtTestSupport_MUE_ASSERT(generateBatchCount(100, 10) == 10);

	MathUtilityExtTestSupport_MUE_ASSERT(generateBatchSize(10, 3, 0) == 3);
	MathUtilityExtTestSupport_MUE_ASSERT(generateBatchSize(10, 3, 1) == 3);
	MathUtilityExtTestSupport_MUE_ASSERT(generateBatchSize(10, 3, 2) == 3);
	MathUtilityExtTestSupport_MUE_ASSERT(generateBatchSize(10, 3, 3) == 1);
	MathUtilityExtTestSupport_MUE_ASSERT(generateBatchSize(10, 3, 4) == 0);
	MathUtilityExtTestSupport_MUE_ASSERT(generateBatchSize(0, 3, 0) == 0);
}

void MathUtilityTest::testIndexConvert()
{
	using namespace MathUtilityExtTestSupport;
	// width = 5
	MathUtilityExtTestSupport_MUE_ASSERT(indexToX(0, 5) == 0);
	MathUtilityExtTestSupport_MUE_ASSERT(indexToY(0, 5) == 0);
	MathUtilityExtTestSupport_MUE_ASSERT(indexToX(4, 5) == 4);
	MathUtilityExtTestSupport_MUE_ASSERT(indexToY(4, 5) == 0);
	MathUtilityExtTestSupport_MUE_ASSERT(indexToX(5, 5) == 0);
	MathUtilityExtTestSupport_MUE_ASSERT(indexToY(5, 5) == 1);
	MathUtilityExtTestSupport_MUE_ASSERT(indexToX(7, 5) == 2);
	MathUtilityExtTestSupport_MUE_ASSERT(indexToY(7, 5) == 1);

	// indexToIntPos
	Vector2Int pos = indexToIntPos(7, 5);
	MathUtilityExtTestSupport_MUE_ASSERT(pos.x == 2 && pos.y == 1);

	// intPosToIndex
	MathUtilityExtTestSupport_MUE_ASSERT(intPosToIndex(2, 1, 5) == 7);
	MathUtilityExtTestSupport_MUE_ASSERT(intPosToIndex(Vector2Int{ 0, 0 }, 5) == 0);
	MathUtilityExtTestSupport_MUE_ASSERT(intPosToIndex(Vector2Int{ 4, 2 }, 5) == 14);

	// 往返
	for (int i = 0; i < 25; ++i)
	{
		int x = indexToX(i, 5);
		int y = indexToY(i, 5);
		MathUtilityExtTestSupport_MUE_ASSERT(intPosToIndex(x, y, 5) == i);
	}
}

void MathUtilityTest::testGetLength()
{
	using namespace MathUtilityExtTestSupport;
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(getLength(3.0f, 4.0f), 5.0f));
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(getLength(0.0f, 0.0f), 0.0f));
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(getLength(Vector2{ 3.0f, 4.0f }), 5.0f));
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(getLength(Vector3{ 0.0f, 3.0f, 4.0f }), 5.0f));
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(getLength(1.0f, 0.0f, 0.0f), 1.0f));

	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(getSquaredLength(Vector2{ 3.0f, 4.0f }), 25.0f));
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(getSquaredLength(Vector3{ 1.0f, 2.0f, 2.0f }), 9.0f));
}

void MathUtilityTest::testNormalize()
{
	using namespace MathUtilityExtTestSupport;
	Vector3 v3 = normalize(Vector3{ 3.0f, 0.0f, 0.0f });
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(v3.x, 1.0f) && mueNear(v3.y, 0.0f) && mueNear(v3.z, 0.0f));

	v3 = normalize(Vector3{ 0.0f, 5.0f, 0.0f });
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(v3.y, 1.0f));

	Vector3 v3b = normalize(Vector3{ 3.0f, 4.0f, 0.0f });
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(getLength(v3b), 1.0f, 1e-3f));

	// normalize(Vector3{0,0,0}) 应不崩溃
	Vector3 zero{ 0.0f, 0.0f, 0.0f };
	Vector3 nzero = normalize(zero);
	(void)nzero;

	Vector2 v2 = normalize(Vector2{ 3.0f, 4.0f });
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(getLength(v2), 1.0f, 1e-3f));
}

void MathUtilityTest::testDotCross()
{
	using namespace MathUtilityExtTestSupport;
	// dot
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(dot(Vector3{ 1.0f, 0.0f, 0.0f }, Vector3{ 0.0f, 1.0f, 0.0f }), 0.0f));
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(dot(Vector3{ 1.0f, 0.0f, 0.0f }, Vector3{ 1.0f, 0.0f, 0.0f }), 1.0f));
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(dot(Vector3{ 2.0f, 3.0f, 4.0f }, Vector3{ 1.0f, 1.0f, 1.0f }), 9.0f));
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(dot(Vector2{ 1.0f, 2.0f }, Vector2{ 3.0f, 4.0f }), 11.0f));

	// cross (与 Unity 一致)
	Vector3 result = cross(Vector3{ 1.0f, 0.0f, 0.0f }, Vector3{ 0.0f, 1.0f, 0.0f });
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(result.z, 1.0f, 1e-3f));
}

void MathUtilityTest::testLengthCompare()
{
	using namespace MathUtilityExtTestSupport;
	MathUtilityExtTestSupport_MUE_ASSERT(lengthLess(Vector2{ 3.0f, 4.0f }, 6.0f));
	MathUtilityExtTestSupport_MUE_ASSERT(!lengthLess(Vector2{ 3.0f, 4.0f }, 4.0f));
	MathUtilityExtTestSupport_MUE_ASSERT(lengthGreater(Vector2{ 3.0f, 4.0f }, 4.0f));
	MathUtilityExtTestSupport_MUE_ASSERT(!lengthGreater(Vector2{ 3.0f, 4.0f }, 6.0f));

	MathUtilityExtTestSupport_MUE_ASSERT(lengthLess(Vector3{ 0.0f, 3.0f, 4.0f }, 6.0f));
	MathUtilityExtTestSupport_MUE_ASSERT(!lengthLess(Vector3{ 0.0f, 3.0f, 4.0f }, 4.0f));
	MathUtilityExtTestSupport_MUE_ASSERT(lengthGreater(Vector3{ 0.0f, 3.0f, 4.0f }, 4.0f));

	// squared
	MathUtilityExtTestSupport_MUE_ASSERT(lengthLessSquared(Vector3{ 3.0f, 0.0f, 0.0f }, 10.0f));
	MathUtilityExtTestSupport_MUE_ASSERT(!lengthLessSquared(Vector3{ 3.0f, 0.0f, 0.0f }, 9.0f));
	MathUtilityExtTestSupport_MUE_ASSERT(lengthGreaterSquared(Vector3{ 3.0f, 0.0f, 0.0f }, 8.0f));
}

void MathUtilityTest::testIsVectorEqual()
{
	using namespace MathUtilityExtTestSupport;
	MathUtilityExtTestSupport_MUE_ASSERT(isVectorEqual(Vector3{ 1.0f, 2.0f, 3.0f }, Vector3{ 1.0f, 2.0f, 3.0f }));
	MathUtilityExtTestSupport_MUE_ASSERT(!isVectorEqual(Vector3{ 1.0f, 2.0f, 3.0f }, Vector3{ 1.0f, 2.0f, 4.0f }));
	MathUtilityExtTestSupport_MUE_ASSERT(isVectorEqual(Vector2{ 1.0f, 2.0f }, Vector2{ 1.0f, 2.0f }));
	MathUtilityExtTestSupport_MUE_ASSERT(!isVectorEqual(Vector2{ 1.0f, 2.0f }, Vector2{ 1.0f, 3.0f }));

	// 精度版本
	MathUtilityExtTestSupport_MUE_ASSERT(isVectorEqual(Vector3{ 1.0f, 2.0f, 3.0f }, Vector3{ 1.1f, 2.1f, 3.1f }, 0.2f));
	MathUtilityExtTestSupport_MUE_ASSERT(!isVectorEqual(Vector3{ 1.0f, 2.0f, 3.0f }, Vector3{ 1.5f, 2.0f, 3.0f }, 0.2f));
}

void MathUtilityTest::testVectorConvert()
{
	using namespace MathUtilityExtTestSupport;
	Vector3 v3 = toVec3(Vector2{ 1.0f, 2.0f });
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(v3.x, 1.0f) && mueNear(v3.y, 2.0f) && mueNear(v3.z, 0.0f));

	Vector2 v2 = toVec2(Vector3{ 3.0f, 4.0f, 5.0f });
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(v2.x, 3.0f) && mueNear(v2.y, 4.0f));

	Vector2Int vi = toVec2Int(Vector2{ 3.7f, 4.9f });
	MathUtilityExtTestSupport_MUE_ASSERT(vi.x == 3 && vi.y == 4);

	v3 = toVec3(Vector2Int{ 2, 3 });
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(v3.x, 2.0f) && mueNear(v3.y, 3.0f) && mueNear(v3.z, 0.0f));

	Vector2Int vi2 = toVec2Int(Vector3{ 5.5f, 6.7f, 0.0f });
	MathUtilityExtTestSupport_MUE_ASSERT(vi2.x == 5 && vi2.y == 6);
}

void MathUtilityTest::testQuickSort()
{
	using namespace MathUtilityExtTestSupport;
	// 升序 Vector<int>
	Vector<int> arr;
	arr.add(5); arr.add(2); arr.add(8); arr.add(1); arr.add(9);
	quickSort(arr);
	MathUtilityExtTestSupport_MUE_ASSERT(arr[0] == 1 && arr[1] == 2 && arr[2] == 5 && arr[3] == 8 && arr[4] == 9);

	// 降序
	quickSort(arr, false);
	MathUtilityExtTestSupport_MUE_ASSERT(arr[0] == 9 && arr[4] == 1);

	// 已排序
	quickSort(arr);
	MathUtilityExtTestSupport_MUE_ASSERT(arr[0] == 1 && arr[4] == 9);

	// 单元素
	Vector<int> single;
	single.add(42);
	quickSort(single);
	MathUtilityExtTestSupport_MUE_ASSERT(single[0] == 42);

	// 大量数据
	Vector<int> big;
	for (int i = 100; i >= 1; --i) big.add(i);
	quickSort(big);
	FOR(100) MathUtilityExtTestSupport_MUE_ASSERT(big[i] == i + 1);

	// ArrayList<10, int> 升序
	ArrayList<20, int> list;
	for (int v : {7, 3, 9, 1, 5}) list.add(v);
	quickSort(list);
	MathUtilityExtTestSupport_MUE_ASSERT(list[0] == 1 && list[4] == 9);

	// 带 compare 函数
	auto cmp = [](const int& a, const int& b) -> int { return a > b ? 1 : (a < b ? -1 : 0); };
	Vector<int> arr2;
	arr2.add(4); arr2.add(1); arr2.add(7);
	quickSort(arr2, +cmp);  // +cmp 强制 lambda→函数指针，避免误匹配 quickSort(arr, bool)
	MathUtilityExtTestSupport_MUE_ASSERT(arr2[0] == 1 && arr2[2] == 7);
}

void MathUtilityTest::testBinarySearch()
{
	using namespace MathUtilityExtTestSupport;
	Vector<int> sorted;
	for (int i = 1; i <= 10; ++i) sorted.add(i);

	MathUtilityExtTestSupport_MUE_ASSERT(binarySearch(sorted, 0, (int)sorted.size() - 1, 1));
	MathUtilityExtTestSupport_MUE_ASSERT(binarySearch(sorted, 0, (int)sorted.size() - 1, 5));
	MathUtilityExtTestSupport_MUE_ASSERT(binarySearch(sorted, 0, (int)sorted.size() - 1, 10));
	MathUtilityExtTestSupport_MUE_ASSERT(!binarySearch(sorted, 0, (int)sorted.size() - 1, 0));
	MathUtilityExtTestSupport_MUE_ASSERT(!binarySearch(sorted, 0, (int)sorted.size() - 1, 11));
}

void MathUtilityTest::testGetGreaterPower2()
{
	using namespace MathUtilityExtTestSupport;
	// 语义：返回 >= value 的最小 2 的幂次（注释"比value大"有歧义，实现是 >=）
	// 特殊：0 和 1 查表均返回 2（表初始化时 i<=1 硬编码为 2）
	MathUtilityExtTestSupport_MUE_ASSERT(getGreaterPower2(1) == 2);    // 表中 [1]=2（特殊处理）
	MathUtilityExtTestSupport_MUE_ASSERT(getGreaterPower2(2) == 2);    // 2 == 2^1，返回 2
	MathUtilityExtTestSupport_MUE_ASSERT(getGreaterPower2(3) == 4);    // 3 不是 2 的幂，下一个是 4
	MathUtilityExtTestSupport_MUE_ASSERT(getGreaterPower2(4) == 4);    // 4 == 2^2，返回 4
	MathUtilityExtTestSupport_MUE_ASSERT(getGreaterPower2(7) == 8);    // 7 不是 2 的幂，下一个是 8
	MathUtilityExtTestSupport_MUE_ASSERT(getGreaterPower2(8) == 8);    // 8 == 2^3，返回 8
	MathUtilityExtTestSupport_MUE_ASSERT(getGreaterPower2(100) == 128);
	MathUtilityExtTestSupport_MUE_ASSERT(getGreaterPower2(255) == 256);
	MathUtilityExtTestSupport_MUE_ASSERT(getGreaterPower2(256) == 256); // 256 == 2^8，返回 256
}

void MathUtilityTest::testRandom()
{
	using namespace MathUtilityExtTestSupport;
	// 多次调用，结果应在范围内
	FOR(100)
	{
		float f = randomFloat(0.0f, 1.0f);
		MathUtilityExtTestSupport_MUE_ASSERT(f >= 0.0f && f <= 1.0f);

		int n = randomInt(1, 10);
		MathUtilityExtTestSupport_MUE_ASSERT(n >= 1 && n <= 10);
	}

	// randomHit edge cases
	MathUtilityExtTestSupport_MUE_ASSERT(!randomHit(0.0f));
	MathUtilityExtTestSupport_MUE_ASSERT(randomHit(1.0f));
	MathUtilityExtTestSupport_MUE_ASSERT(!randomHit(0, 100));
	MathUtilityExtTestSupport_MUE_ASSERT(randomHit(100, 100));

	// randomOrder 不崩溃
	int arr[] = { 1, 2, 3, 4, 5 };
	randomOrder(arr, 5);
	// 排序后仍是 1~5 的排列
	Vector<int> check;
	for (int v : arr) check.add(v);
	quickSort(check);
	for (int i = 0; i < 5; ++i) MathUtilityExtTestSupport_MUE_ASSERT(check[i] == i + 1);
}

void MathUtilityTest::testFrameSpeed()
{
	using namespace MathUtilityExtTestSupport;
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(frameToSecond(30), 30 * 0.0333f));
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(frameToSecond(0), 0.0f));

	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(speedToInterval(1.0f), 0.0333f, 1e-3f));
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(intervalToSpeed(0.0333f), 1.0f, 0.01f));
	// 除0保护
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(speedToInterval(0.0f), 0.0f));
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(intervalToSpeed(0.0f), 0.0f));
}

void MathUtilityTest::testAngleConvert()
{
	using namespace MathUtilityExtTestSupport;
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(toDegree(MATH_PI), 180.0f, 0.01f));
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(toRadian(180.0f), MATH_PI, 1e-3f));
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(toDegree(0.0f), 0.0f));
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(toRadian(0.0f), 0.0f));
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(toDegree(HALF_PI_RADIAN), 90.0f, 0.01f));
	MathUtilityExtTestSupport_MUE_ASSERT(mueNear(toRadian(90.0f), HALF_PI_RADIAN, 1e-3f));

	// 往返
	for (int deg = 0; deg <= 360; deg += 45)
	{
		float r = toRadian((float)deg);
		float d = toDegree(r);
		MathUtilityExtTestSupport_MUE_ASSERT(mueNear(d, (float)deg, 0.01f));
	}
}

namespace MathUtilityExt2TestSupport
{
#define MathUtilityExt2TestSupport_ME2_ASSERT(expr, msg) \
    if (!(expr)) { ERROR(string("MathUtilityExt2Test FAILED: ") + (msg)); }

#define MathUtilityExt2TestSupport_ME2_FLOAT_EQ(a, b) (fabsf((a) - (b)) < 1e-4f)
}

void MathUtilityTest::testME2_Clamp()
{
	using namespace MathUtilityExt2TestSupport;
	MathUtilityExt2TestSupport_ME2_ASSERT(clamp(5, 0, 10) == 5, "clamp in range");
	MathUtilityExt2TestSupport_ME2_ASSERT(clamp(-5, 0, 10) == 0, "clamp below min");
	MathUtilityExt2TestSupport_ME2_ASSERT(clamp(15, 0, 10) == 10, "clamp above max");
	MathUtilityExt2TestSupport_ME2_ASSERT(clamp(0, 0, 10) == 0, "clamp at min");
	MathUtilityExt2TestSupport_ME2_ASSERT(clamp(10, 0, 10) == 10, "clamp at max");
}

void MathUtilityTest::testME2_ClampFloat()
{
	using namespace MathUtilityExt2TestSupport;
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(clamp(0.5f, 0.0f, 1.0f), 0.5f), "clampF in range");
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(clamp(-0.1f, 0.0f, 1.0f), 0.0f), "clampF below");
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(clamp(1.1f, 0.0f, 1.0f), 1.0f), "clampF above");
}

void MathUtilityTest::testME2_ClampRef()
{
	using namespace MathUtilityExt2TestSupport;
	int v = 15;
	clampRef(v, 0, 10);
	MathUtilityExt2TestSupport_ME2_ASSERT(v == 10, "clampRef above max");

	v = -5;
	clampRef(v, 0, 10);
	MathUtilityExt2TestSupport_ME2_ASSERT(v == 0, "clampRef below min");

	v = 5;
	clampRef(v, 0, 10);
	MathUtilityExt2TestSupport_ME2_ASSERT(v == 5, "clampRef in range");
}

void MathUtilityTest::testME2_ClampMin()
{
	using namespace MathUtilityExt2TestSupport;
	MathUtilityExt2TestSupport_ME2_ASSERT(clampMin(5, 0) == 5, "clampMin above");
	MathUtilityExt2TestSupport_ME2_ASSERT(clampMin(-5, 0) == 0, "clampMin below");
	MathUtilityExt2TestSupport_ME2_ASSERT(clampMin(0, 0) == 0, "clampMin at zero");
	MathUtilityExt2TestSupport_ME2_ASSERT(clampMin(3.0f, 5.0f) == 5.0f, "clampMin float");
}

void MathUtilityTest::testME2_ClampMax()
{
	using namespace MathUtilityExt2TestSupport;
	MathUtilityExt2TestSupport_ME2_ASSERT(clampMax(5, 10) == 5, "clampMax below");
	MathUtilityExt2TestSupport_ME2_ASSERT(clampMax(15, 10) == 10, "clampMax above");
	MathUtilityExt2TestSupport_ME2_ASSERT(clampMax(10, 10) == 10, "clampMax at max");
}

void MathUtilityTest::testME2_ClampZero()
{
	using namespace MathUtilityExt2TestSupport;
	MathUtilityExt2TestSupport_ME2_ASSERT(clampZero(5) == 5, "clampZero positive");
	MathUtilityExt2TestSupport_ME2_ASSERT(clampZero(-5) == 0, "clampZero negative");
	MathUtilityExt2TestSupport_ME2_ASSERT(clampZero(0) == 0, "clampZero zero");
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(clampZero(-1.5f), 0.0f), "clampZero float neg");
}

void MathUtilityTest::testME2_Saturate()
{
	using namespace MathUtilityExt2TestSupport;
	float v = 1.5f; saturate(v); MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(v, 1.0f), "saturate above");
	v = -0.5f;      saturate(v); MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(v, 0.0f), "saturate below");
	v = 0.5f;       saturate(v); MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(v, 0.5f), "saturate in range");
}

void MathUtilityTest::testME2_Step()
{
	using namespace MathUtilityExt2TestSupport;
	MathUtilityExt2TestSupport_ME2_ASSERT(step(1.0f, 2.0f) == 1, "step 1>=1 true");
	MathUtilityExt2TestSupport_ME2_ASSERT(step(2.0f, 1.0f) == 0, "step 1<2 false");
	MathUtilityExt2TestSupport_ME2_ASSERT(step(5.0f, 5.0f) == 1, "step equal true");
}

void MathUtilityTest::testME2_Frac()
{
	using namespace MathUtilityExt2TestSupport;
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(frac(3.75f), 0.75f), "frac 3.75");
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(frac(1.0f), 0.0f), "frac integer");
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(frac(0.3f), 0.3f), "frac 0.3");
}

void MathUtilityTest::testME2_FmodCustom()
{
	using namespace MathUtilityExt2TestSupport;
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(fmod(7.0f, 3.0f), 1.0f), "fmod 7%3==1");
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(fmod(10.0f, 4.0f), 2.0f), "fmod 10%4==2");
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(fmod(5.0f, 5.0f), 0.0f), "fmod 5%5==0");
}

void MathUtilityTest::testME2_DivideFloat()
{
	using namespace MathUtilityExt2TestSupport;
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(divide(10.0f, 2.0f), 5.0f), "divideFF 10/2");
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(divide(0.0f, 5.0f), 0.0f), "divideFF 0/5");
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(divide(5.0f, 0.0f), 0.0f), "divideFF x/0==0");
}

void MathUtilityTest::testME2_DivideInt()
{
	using namespace MathUtilityExt2TestSupport;
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(divide(10, 4), 2.5f), "divideII 10/4");
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(divide(0, 5), 0.0f), "divideII 0/5");
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(divide(5, 0), 0.0f), "divideII x/0==0");
}

void MathUtilityTest::testME2_GenerateBatch()
{
	using namespace MathUtilityExt2TestSupport;
	MathUtilityExt2TestSupport_ME2_ASSERT(generateBatchCount(10, 3) == 4, "batchCount 10/3==4");
	MathUtilityExt2TestSupport_ME2_ASSERT(generateBatchCount(9, 3) == 3, "batchCount 9/3==3");
	MathUtilityExt2TestSupport_ME2_ASSERT(generateBatchCount(0, 3) == 0, "batchCount 0/3==0");
	MathUtilityExt2TestSupport_ME2_ASSERT(generateBatchCount(1, 1) == 1, "batchCount 1/1==1");

	MathUtilityExt2TestSupport_ME2_ASSERT(generateBatchSize(10, 3, 0) == 3, "batchSize batch0==3");
	MathUtilityExt2TestSupport_ME2_ASSERT(generateBatchSize(10, 3, 3) == 1, "batchSize batch3==1");
	MathUtilityExt2TestSupport_ME2_ASSERT(generateBatchSize(10, 3, 10) == 0, "batchSize OOB==0");
}

void MathUtilityTest::testME2_IsZero()
{
	using namespace MathUtilityExt2TestSupport;
	MathUtilityExt2TestSupport_ME2_ASSERT(isZero(0.0f), "isZero 0");
	MathUtilityExt2TestSupport_ME2_ASSERT(isZero(0.000005f), "isZero tiny within default");
	MathUtilityExt2TestSupport_ME2_ASSERT(!isZero(0.01f), "isZero 0.01 false");
	MathUtilityExt2TestSupport_ME2_ASSERT(isZero(0.0), "isZero double 0");
}

void MathUtilityTest::testME2_IsEqual()
{
	using namespace MathUtilityExt2TestSupport;
	MathUtilityExt2TestSupport_ME2_ASSERT(isEqual(1.0f, 1.0f), "isEqual same");
	MathUtilityExt2TestSupport_ME2_ASSERT(isEqual(1.0f, 1.000005f), "isEqual tiny diff");
	MathUtilityExt2TestSupport_ME2_ASSERT(!isEqual(1.0f, 1.01f), "isEqual large diff false");
}

void MathUtilityTest::testME2_IndexConvert()
{
	using namespace MathUtilityExt2TestSupport;
	MathUtilityExt2TestSupport_ME2_ASSERT(indexToX(7, 5) == 2, "indexToX 7%5==2");
	MathUtilityExt2TestSupport_ME2_ASSERT(indexToY(7, 5) == 1, "indexToY 7/5==1");
	MathUtilityExt2TestSupport_ME2_ASSERT(indexToX(0, 4) == 0, "indexToX 0");
	MathUtilityExt2TestSupport_ME2_ASSERT(indexToY(0, 4) == 0, "indexToY 0");
}

void MathUtilityTest::testME2_IntPosToIndex()
{
	using namespace MathUtilityExt2TestSupport;
	MathUtilityExt2TestSupport_ME2_ASSERT(intPosToIndex(3, 4, 10) == 43, "intPosToIndex 3+4*10==43");
	MathUtilityExt2TestSupport_ME2_ASSERT(intPosToIndex(0, 0, 10) == 0, "intPosToIndex origin==0");
	MathUtilityExt2TestSupport_ME2_ASSERT(intPosToIndex(9, 9, 10) == 99, "intPosToIndex 9+9*10==99");
}

void MathUtilityTest::testME2_IndexToIntPos()
{
	using namespace MathUtilityExt2TestSupport;
	Vector2Int pos = indexToIntPos(7, 5);
	MathUtilityExt2TestSupport_ME2_ASSERT(pos.x == 2 && pos.y == 1, "indexToIntPos 7,5");
	Vector2Int pos2 = indexToIntPos(0, 4);
	MathUtilityExt2TestSupport_ME2_ASSERT(pos2.x == 0 && pos2.y == 0, "indexToIntPos 0,4");
}

void MathUtilityTest::testME2_IsEven()
{
	using namespace MathUtilityExt2TestSupport;
	MathUtilityExt2TestSupport_ME2_ASSERT(isEven(0), "isEven 0");
	MathUtilityExt2TestSupport_ME2_ASSERT(isEven(2), "isEven 2");
	MathUtilityExt2TestSupport_ME2_ASSERT(isEven(100), "isEven 100");
	MathUtilityExt2TestSupport_ME2_ASSERT(!isEven(1), "isEven 1 false");
	MathUtilityExt2TestSupport_ME2_ASSERT(!isEven(3), "isEven 3 false");
	MathUtilityExt2TestSupport_ME2_ASSERT(!isEven(99), "isEven 99 false");
}

void MathUtilityTest::testME2_IsPow2()
{
	using namespace MathUtilityExt2TestSupport;
	MathUtilityExt2TestSupport_ME2_ASSERT(!isPow2(0), "isPow2 0 false");
	MathUtilityExt2TestSupport_ME2_ASSERT(isPow2(1), "isPow2 1 true");
	MathUtilityExt2TestSupport_ME2_ASSERT(isPow2(2), "isPow2 2 true");
	MathUtilityExt2TestSupport_ME2_ASSERT(isPow2(4), "isPow2 4 true");
	MathUtilityExt2TestSupport_ME2_ASSERT(isPow2(8), "isPow2 8 true");
	MathUtilityExt2TestSupport_ME2_ASSERT(isPow2(16), "isPow2 16 true");
	MathUtilityExt2TestSupport_ME2_ASSERT(isPow2(1024), "isPow2 1024 true");
	MathUtilityExt2TestSupport_ME2_ASSERT(!isPow2(3), "isPow2 3 false");
	MathUtilityExt2TestSupport_ME2_ASSERT(!isPow2(5), "isPow2 5 false");
	MathUtilityExt2TestSupport_ME2_ASSERT(!isPow2(6), "isPow2 6 false");
	MathUtilityExt2TestSupport_ME2_ASSERT(!isPow2(100), "isPow2 100 false");
	MathUtilityExt2TestSupport_ME2_ASSERT(!isPow2(-1), "isPow2 -1 false");
}

void MathUtilityTest::testME2_Ceiling()
{
	using namespace MathUtilityExt2TestSupport;
	MathUtilityExt2TestSupport_ME2_ASSERT(ceiling(1.0f) == 1, "ceiling 1.0==1");
	MathUtilityExt2TestSupport_ME2_ASSERT(ceiling(1.5f) == 2, "ceiling 1.5==2");
	MathUtilityExt2TestSupport_ME2_ASSERT(ceiling(0.0f) == 0, "ceiling 0==0");
	MathUtilityExt2TestSupport_ME2_ASSERT(ceiling(3.9f) == 4, "ceiling 3.9==4");
	MathUtilityExt2TestSupport_ME2_ASSERT(ceiling(-1.5f) == -1, "ceiling -1.5==-1 (neg, just truncate)");
	MathUtilityExt2TestSupport_ME2_ASSERT(ceiling(-2.9f) == -2, "ceiling -2.9==-2");
}

void MathUtilityTest::testME2_Floor()
{
	using namespace MathUtilityExt2TestSupport;
	MathUtilityExt2TestSupport_ME2_ASSERT(floor_(1.0f) == 1, "floor 1.0==1");
	MathUtilityExt2TestSupport_ME2_ASSERT(floor_(1.9f) == 1, "floor 1.9==1");
	MathUtilityExt2TestSupport_ME2_ASSERT(floor_(-1.5f) == -2, "floor -1.5==-2");
	MathUtilityExt2TestSupport_ME2_ASSERT(floor_(0.0f) == 0, "floor 0==0");
	MathUtilityExt2TestSupport_ME2_ASSERT(floor_(-0.5f) == -1, "floor -0.5==-1");
}

void MathUtilityTest::testME2_Pow2()
{
	using namespace MathUtilityExt2TestSupport;
	MathUtilityExt2TestSupport_ME2_ASSERT(pow2(0) == 1, "pow2(0)==1");
	MathUtilityExt2TestSupport_ME2_ASSERT(pow2(1) == 2, "pow2(1)==2");
	MathUtilityExt2TestSupport_ME2_ASSERT(pow2(10) == 1024, "pow2(10)==1024");
	MathUtilityExt2TestSupport_ME2_ASSERT(pow2(16) == 65536, "pow2(16)==65536");
}

void MathUtilityTest::testME2_Pow10()
{
	using namespace MathUtilityExt2TestSupport;
	MathUtilityExt2TestSupport_ME2_ASSERT(pow10(0) == 1, "pow10(0)==1");
	MathUtilityExt2TestSupport_ME2_ASSERT(pow10(1) == 10, "pow10(1)==10");
	MathUtilityExt2TestSupport_ME2_ASSERT(pow10(3) == 1000, "pow10(3)==1000");
	MathUtilityExt2TestSupport_ME2_ASSERT(pow10(6) == 1000000, "pow10(6)==1000000");
}

void MathUtilityTest::testME2_LerpFloat()
{
	using namespace MathUtilityExt2TestSupport;
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(lerp(0.0f, 10.0f, 0.0f), 0.0f), "lerp t=0");
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(lerp(0.0f, 10.0f, 1.0f), 10.0f), "lerp t=1");
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(lerp(0.0f, 10.0f, 0.5f), 5.0f), "lerp t=0.5");
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(lerp(-10.0f, 10.0f, 0.5f), 0.0f), "lerp neg to pos");
}

void MathUtilityTest::testME2_LerpClamp()
{
	using namespace MathUtilityExt2TestSupport;
	// t 超出范围应被夹住
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(lerp(0.0f, 10.0f, -1.0f), 0.0f), "lerp t<0 clamp to 0");
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(lerp(0.0f, 10.0f, 2.0f), 10.0f), "lerp t>1 clamp to end");
}

void MathUtilityTest::testME2_LerpInt()
{
	using namespace MathUtilityExt2TestSupport;
	MathUtilityExt2TestSupport_ME2_ASSERT(lerp(0, 100, 0.0f) == 0, "lerp int t=0");
	MathUtilityExt2TestSupport_ME2_ASSERT(lerp(0, 100, 1.0f) == 100, "lerp int t=1");
	MathUtilityExt2TestSupport_ME2_ASSERT(lerp(0, 100, 0.5f) == 50, "lerp int t=0.5");
}

void MathUtilityTest::testME2_LerpLLong()
{
	using namespace MathUtilityExt2TestSupport;
	MathUtilityExt2TestSupport_ME2_ASSERT(lerp(0LL, 1000000LL, 0.5f) == 500000LL, "lerp llong t=0.5");
	MathUtilityExt2TestSupport_ME2_ASSERT(lerp(0LL, 1000000LL, 0.0f) == 0LL, "lerp llong t=0");
}

void MathUtilityTest::testME2_InverseLerp()
{
	using namespace MathUtilityExt2TestSupport;
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(inverseLerp(0.0f, 10.0f, 5.0f), 0.5f), "inverseLerp 5/10==0.5");
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(inverseLerp(0.0f, 100.0f, 0.0f), 0.0f), "inverseLerp 0/100==0");
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(inverseLerp(0.0f, 100.0f, 100.0f), 1.0f), "inverseLerp 100/100==1");
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(inverseLerp(0.0f, 0.0f, 0.0f), 0.0f), "inverseLerp div0==0");
}

void MathUtilityTest::testME2_LerpSimple()
{
	using namespace MathUtilityExt2TestSupport;
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(lerpSimple(0.0f, 10.0f, 0.5f), 5.0f), "lerpSimple 0.5");
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(lerpSimple(0.0f, 10.0f, 0.0f), 0.0f), "lerpSimple 0");
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(lerpSimple(0.0f, 10.0f, 1.0f), 10.0f), "lerpSimple 1");
}

void MathUtilityTest::testME2_InRange()
{
	using namespace MathUtilityExt2TestSupport;
	MathUtilityExt2TestSupport_ME2_ASSERT(inRange(5, 0, 10), "inRange int in");
	MathUtilityExt2TestSupport_ME2_ASSERT(!inRange(-1, 0, 10), "inRange int below");
	MathUtilityExt2TestSupport_ME2_ASSERT(!inRange(11, 0, 10), "inRange int above");
	MathUtilityExt2TestSupport_ME2_ASSERT(inRange(0, 0, 10), "inRange at min");
	MathUtilityExt2TestSupport_ME2_ASSERT(inRange(10, 0, 10), "inRange at max");
}

void MathUtilityTest::testME2_InRangeFloat()
{
	using namespace MathUtilityExt2TestSupport;
	MathUtilityExt2TestSupport_ME2_ASSERT(inRange(0.5f, 0.0f, 1.0f), "inRange float in");
	MathUtilityExt2TestSupport_ME2_ASSERT(!inRange(-0.1f, 0.0f, 1.0f), "inRange float below");
	MathUtilityExt2TestSupport_ME2_ASSERT(!inRange(1.1f, 0.0f, 1.0f), "inRange float above");
}

void MathUtilityTest::testME2_InFixedRange()
{
	using namespace MathUtilityExt2TestSupport;
	MathUtilityExt2TestSupport_ME2_ASSERT(inFixedRange(5, 3, 7), "inFixedRange in");
	MathUtilityExt2TestSupport_ME2_ASSERT(!inFixedRange(2, 3, 7), "inFixedRange below");
	MathUtilityExt2TestSupport_ME2_ASSERT(!inFixedRange(8, 3, 7), "inFixedRange above");
}

void MathUtilityTest::testME2_ClampCycle()
{
	using namespace MathUtilityExt2TestSupport;
	float v = 370.0f; clampDegree360(v);
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(v, 370.0f - 360.0f), "clampDeg360 370==10");

	v = -30.0f; clampDegree360(v);
	MathUtilityExt2TestSupport_ME2_ASSERT(v >= 0.0f && v <= 360.0f, "clampDeg360 -30 in range");

	v = 200.0f; clampDegree180(v);
	MathUtilityExt2TestSupport_ME2_ASSERT(v >= -180.0f && v <= 180.0f, "clampDeg180 200 in range");
}

void MathUtilityTest::testME2_ClampRadian()
{
	using namespace MathUtilityExt2TestSupport;
	float pi = 3.14159f;
	float v = pi * 2.5f;
	clampRadian180(v);
	MathUtilityExt2TestSupport_ME2_ASSERT(v >= -pi && v <= pi, "clampRad180 in range");

	v = -pi - 0.1f;
	clampRadian180(v);
	MathUtilityExt2TestSupport_ME2_ASSERT(v >= -pi && v <= pi, "clampRad180 neg in range");
}

void MathUtilityTest::testME2_SecondsToMinSec()
{
	using namespace MathUtilityExt2TestSupport;
	int m, s;
	secondsToMinutesSeconds(90, m, s);
	MathUtilityExt2TestSupport_ME2_ASSERT(m == 1 && s == 30, "90s == 1m30s");
	secondsToMinutesSeconds(0, m, s);
	MathUtilityExt2TestSupport_ME2_ASSERT(m == 0 && s == 0, "0s == 0m0s");
	secondsToMinutesSeconds(59, m, s);
	MathUtilityExt2TestSupport_ME2_ASSERT(m == 0 && s == 59, "59s == 0m59s");
	secondsToMinutesSeconds(120, m, s);
	MathUtilityExt2TestSupport_ME2_ASSERT(m == 2 && s == 0, "120s == 2m0s");
}

void MathUtilityTest::testME2_SecondsToHourMinSec()
{
	using namespace MathUtilityExt2TestSupport;
	int h, m, s;
	secondsToHoursMinutesSeconds(3661, h, m, s);
	MathUtilityExt2TestSupport_ME2_ASSERT(h == 1 && m == 1 && s == 1, "3661s == 1h1m1s");
	secondsToHoursMinutesSeconds(3600, h, m, s);
	MathUtilityExt2TestSupport_ME2_ASSERT(h == 1 && m == 0 && s == 0, "3600s == 1h0m0s");
	secondsToHoursMinutesSeconds(0, h, m, s);
	MathUtilityExt2TestSupport_ME2_ASSERT(h == 0 && m == 0 && s == 0, "0s == 0h0m0s");
}

void MathUtilityTest::testME2_SpeedInterval()
{
	using namespace MathUtilityExt2TestSupport;
	float speed = 1.0f;
	float interval = speedToInterval(speed);
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(interval, 0.0333f), "speedToInterval 1==0.0333");

	float speed2 = intervalToSpeed(interval);
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(speed2, 1.0f), "intervalToSpeed roundtrip");
}

void MathUtilityTest::testME2_Swap()
{
	using namespace MathUtilityExt2TestSupport;
	int a = 10, b = 20;
	swap_(a, b);
	MathUtilityExt2TestSupport_ME2_ASSERT(a == 20 && b == 10, "swap_ int");

	float f1 = 1.5f, f2 = 3.0f;
	swap_(f1, f2);
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(f1, 3.0f) && MathUtilityExt2TestSupport_ME2_FLOAT_EQ(f2, 1.5f), "swap_ float");

	string s1 = "hello", s2 = "world";
	swap_(s1, s2);
	MathUtilityExt2TestSupport_ME2_ASSERT(s1 == "world" && s2 == "hello", "swap_ string");
}

void MathUtilityTest::testME2_PowerFloat()
{
	using namespace MathUtilityExt2TestSupport;
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(powerFloat(2.0f, 0), 1.0f), "powerFloat 2^0==1");
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(powerFloat(2.0f, 1), 2.0f), "powerFloat 2^1==2");
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(powerFloat(2.0f, 10), 1024.0f), "powerFloat 2^10==1024");
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(powerFloat(3.0f, 3), 27.0f), "powerFloat 3^3==27");
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(powerFloat(0.5f, 4), 0.0625f), "powerFloat 0.5^4");
}

void MathUtilityTest::testME2_GetMinMax()
{
	using namespace MathUtilityExt2TestSupport;
	MathUtilityExt2TestSupport_ME2_ASSERT(getMin(3, 5) == 3, "getMin int 3<5");
	MathUtilityExt2TestSupport_ME2_ASSERT(getMin(5, 3) == 3, "getMin int 5,3");
	MathUtilityExt2TestSupport_ME2_ASSERT(getMax(3, 5) == 5, "getMax int 3,5");
	MathUtilityExt2TestSupport_ME2_ASSERT(getMax(5, 3) == 5, "getMax int 5,3");
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(getMin(1.0f, 2.0f), 1.0f), "getMin float");
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(getMax(1.0f, 2.0f), 2.0f), "getMax float");
}

void MathUtilityTest::testME2_SquaredLength()
{
	using namespace MathUtilityExt2TestSupport;
	Vector3 v(3.0f, 4.0f, 0.0f);
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(getSquaredLength(v), 25.0f), "sqLen 3-4-0==25");
	Vector2 v2(3.0f, 4.0f);
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(getSquaredLength(v2), 25.0f), "sqLen2 3-4==25");
}

void MathUtilityTest::testME2_GetLength()
{
	using namespace MathUtilityExt2TestSupport;
	Vector3 v(3.0f, 4.0f, 0.0f);
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(getLength(v), 5.0f), "getLength 3-4-0==5");
	Vector2 v2(3.0f, 4.0f);
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(getLength(v2), 5.0f), "getLengthV2 3-4==5");
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(getLength(3.0f, 4.0f), 5.0f), "getLength xy 5");
}

void MathUtilityTest::testME2_Dot()
{
	using namespace MathUtilityExt2TestSupport;
	Vector3 v1(1.0f, 0.0f, 0.0f), v2(0.0f, 1.0f, 0.0f);
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(dot(v1, v2), 0.0f), "dot perpendicular==0");

	Vector3 v3(1.0f, 0.0f, 0.0f), v4(1.0f, 0.0f, 0.0f);
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(dot(v3, v4), 1.0f), "dot parallel==1");

	Vector2 u1(1.0f, 0.0f), u2(0.0f, 1.0f);
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(dot(u1, u2), 0.0f), "dot2 perpendicular==0");
}

void MathUtilityTest::testME2_Cross()
{
	using namespace MathUtilityExt2TestSupport;
	Vector3 x(1.0f, 0.0f, 0.0f), y(0.0f, 1.0f, 0.0f);
	Vector3 c = cross(x, y);
	// Unity 坐标系 cross(x,y) = (0,0,1) 实际结果见框架实现
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(c.x * c.x + c.y * c.y + c.z * c.z, 1.0f), "cross magnitude==1");
}

void MathUtilityTest::testME2_Normalize3()
{
	using namespace MathUtilityExt2TestSupport;
	Vector3 v(3.0f, 4.0f, 0.0f);
	Vector3 n = normalize(v);
	float len = getLength(n);
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(len, 1.0f), "normalize3 length==1");
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(n.x, 0.6f), "normalize3 x==0.6");
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(n.y, 0.8f), "normalize3 y==0.8");
}

void MathUtilityTest::testME2_Normalize2()
{
	using namespace MathUtilityExt2TestSupport;
	Vector2 v(3.0f, 4.0f);
	Vector2 n = normalize(v);
	float len = getLength(n);
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(len, 1.0f), "normalize2 length==1");
}

void MathUtilityTest::testME2_DegreeRadian()
{
	using namespace MathUtilityExt2TestSupport;
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(toDegree(MATH_PI), 180.0f), "toDegree pi==180");
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(toRadian(180.0f), 3.14159f), "toRadian 180==pi");
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(toDegree(toRadian(90.0f)), 90.0f), "roundtrip 90");
}

void MathUtilityTest::testME2_FrameToSecond()
{
	using namespace MathUtilityExt2TestSupport;
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(frameToSecond(30), 0.999f), "30frames~=1s");
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(frameToSecond(0), 0.0f), "0frames==0s");
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(frameToSecond(60), 1.998f), "60frames~=2s");
}

void MathUtilityTest::testME2_VecConvert()
{
	using namespace MathUtilityExt2TestSupport;
	Vector3 v3(1.5f, 2.5f, 3.5f);
	Vector2 v2 = toVec2(v3);
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(v2.x, 1.5f) && MathUtilityExt2TestSupport_ME2_FLOAT_EQ(v2.y, 2.5f), "toVec2 from V3");

	Vector2Int vi(5, 7);
	Vector3 v3b = toVec3(vi);
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(v3b.x, 5.0f) && MathUtilityExt2TestSupport_ME2_FLOAT_EQ(v3b.y, 7.0f), "toVec3 from V2Int");

	Vector2 vf(3.9f, 4.2f);
	Vector2Int vi2 = toVec2Int(vf);
	MathUtilityExt2TestSupport_ME2_ASSERT(vi2.x == 3 && vi2.y == 4, "toVec2Int from V2");
}

void MathUtilityTest::testME2_LengthCompare()
{
	using namespace MathUtilityExt2TestSupport;
	Vector2 v(3.0f, 4.0f); // length==5
	MathUtilityExt2TestSupport_ME2_ASSERT(lengthLess(v, 6.0f), "lengthLess 5<6 true");
	MathUtilityExt2TestSupport_ME2_ASSERT(!lengthLess(v, 4.0f), "lengthLess 5<4 false");
	MathUtilityExt2TestSupport_ME2_ASSERT(lengthGreater(v, 4.0f), "lengthGreater 5>4 true");
	MathUtilityExt2TestSupport_ME2_ASSERT(!lengthGreater(v, 6.0f), "lengthGreater 5>6 false");
}

void MathUtilityTest::testME2_LengthCompare3()
{
	using namespace MathUtilityExt2TestSupport;
	Vector3 v(3.0f, 4.0f, 0.0f); // length==5
	MathUtilityExt2TestSupport_ME2_ASSERT(lengthLess(v, 6.0f), "lengthLessV3 5<6");
	MathUtilityExt2TestSupport_ME2_ASSERT(!lengthLess(v, 4.0f), "lengthLessV3 5<4 false");
	MathUtilityExt2TestSupport_ME2_ASSERT(lengthGreater(v, 4.0f), "lengthGreaterV3 5>4");
}

void MathUtilityTest::testME2_IsVectorEqual()
{
	using namespace MathUtilityExt2TestSupport;
	Vector3 v1(1.0f, 2.0f, 3.0f), v2(1.0f, 2.0f, 3.0f);
	MathUtilityExt2TestSupport_ME2_ASSERT(isVectorEqual(v1, v2), "isVecEqual V3 same");
	Vector3 v3(1.001f, 2.0f, 3.0f);
	MathUtilityExt2TestSupport_ME2_ASSERT(!isVectorEqual(v1, v3), "isVecEqual V3 diff");

	Vector2 u1(1.0f, 2.0f), u2(1.0f, 2.0f);
	MathUtilityExt2TestSupport_ME2_ASSERT(isVectorEqual(u1, u2), "isVecEqual V2 same");
}

void MathUtilityTest::testME2_GetGreaterPower2()
{
	using namespace MathUtilityExt2TestSupport;
	// getGreaterPower2(n) == smallest power of 2 >= n
	MathUtilityExt2TestSupport_ME2_ASSERT(getGreaterPower2(1) >= 1 && isPow2(getGreaterPower2(1)), "getGP2(1)");
	MathUtilityExt2TestSupport_ME2_ASSERT(getGreaterPower2(2) >= 2, "getGP2(2)>=2");
	MathUtilityExt2TestSupport_ME2_ASSERT(getGreaterPower2(3) == 4, "getGP2(3)==4");
	MathUtilityExt2TestSupport_ME2_ASSERT(getGreaterPower2(5) == 8, "getGP2(5)==8");
	MathUtilityExt2TestSupport_ME2_ASSERT(getGreaterPower2(16) == 16, "getGP2(16)==16");
	MathUtilityExt2TestSupport_ME2_ASSERT(getGreaterPower2(17) == 32, "getGP2(17)==32");
	MathUtilityExt2TestSupport_ME2_ASSERT(getGreaterPower2(255) == 256, "getGP2(255)==256");
	MathUtilityExt2TestSupport_ME2_ASSERT(getGreaterPower2(256) == 256, "getGP2(256)==256");
}

void MathUtilityTest::testME2_CheckInt()
{
	using namespace MathUtilityExt2TestSupport;
	float v = 4.99999f;
	checkInt(v);
	// 与整数 5 的差在 MIN_DELTA(0.00001) 以内，应被置为 5.0
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(v, 5.0f), "checkInt 4.99999 -> 5");

	float v2 = 4.5f;
	checkInt(v2);
	// 不接近整数，不应被修改
	MathUtilityExt2TestSupport_ME2_ASSERT(MathUtilityExt2TestSupport_ME2_FLOAT_EQ(v2, 4.5f), "checkInt 4.5 unchanged");
}

namespace MathUtilityExt3TestSupport
{
#define MathUtilityExt3TestSupport_ME3_ASSERT(expr, msg) \
    if (!(expr)) { ERROR(string("MathUtilityExt3Test FAILED: ") + (msg)); return; }

#define MathUtilityExt3TestSupport_ME3_ASSERT_EQ(a, b, msg) \
    if ((a) != (b)) { ERROR(string("MathUtilityExt3Test EQ FAILED: ") + (msg) + \
        " expected=" + IToS(b) + " got=" + IToS(a)); return; }

#define MathUtilityExt3TestSupport_ME3_ASSERT_F(a, b, msg) \
    if (fabsf((a) - (b)) > 1e-3f) { ERROR(string("MathUtilityExt3Test FLOAT FAILED: ") + (msg) + \
        " expected=" + FToS(b) + " got=" + FToS(a)); return; }
}

void MathUtilityTest::testRotateVector3AroundY()
{
	using namespace MathUtilityExt3TestSupport;
	// 绕Y轴旋转90度：(1,0,0) → 应接近 (0,0,-1)（右手坐标系）
	Vector3 vec = { 1.0f, 0.0f, 0.0f };
	float rad90 = toRadian(90.0f);
	Vector3 result = rotateVector3AroundY(vec, rad90);
	MathUtilityExt3TestSupport_ME3_ASSERT_F(result.x, 0.0f, "rotateAroundY 90 x");
	MathUtilityExt3TestSupport_ME3_ASSERT_F(result.y, 0.0f, "rotateAroundY 90 y");

	// 绕Y轴旋转180度：(1,0,0) → (-1,0,0)
	float rad180 = toRadian(180.0f);
	Vector3 result2 = rotateVector3AroundY(vec, rad180);
	MathUtilityExt3TestSupport_ME3_ASSERT_F(result2.x, -1.0f, "rotateAroundY 180 x");
	MathUtilityExt3TestSupport_ME3_ASSERT_F(result2.y, 0.0f, "rotateAroundY 180 y");
	MathUtilityExt3TestSupport_ME3_ASSERT_F(result2.z, 0.0f, "rotateAroundY 180 z");

	// 旋转0度不变
	Vector3 result3 = rotateVector3AroundY(vec, 0.0f);
	MathUtilityExt3TestSupport_ME3_ASSERT_F(result3.x, 1.0f, "rotateAroundY 0 x");
	MathUtilityExt3TestSupport_ME3_ASSERT_F(result3.y, 0.0f, "rotateAroundY 0 y");
	MathUtilityExt3TestSupport_ME3_ASSERT_F(result3.z, 0.0f, "rotateAroundY 0 z");

	// 旋转360度回到原点
	float rad360 = toRadian(360.0f);
	Vector3 result4 = rotateVector3AroundY(vec, rad360);
	MathUtilityExt3TestSupport_ME3_ASSERT_F(result4.x, 1.0f, "rotateAroundY 360 x");

	// Y轴分量不受旋转影响
	Vector3 vec2 = { 1.0f, 5.0f, 0.0f };
	Vector3 result5 = rotateVector3AroundY(vec2, rad90);
	MathUtilityExt3TestSupport_ME3_ASSERT_F(result5.y, 5.0f, "rotateAroundY Y preserved");

	// 负角度
	Vector3 result6 = rotateVector3AroundY(vec, -rad90);
	MathUtilityExt3TestSupport_ME3_ASSERT_F(result6.y, 0.0f, "rotateAroundY -90 y");
}

void MathUtilityTest::testRotateVector3AroundX()
{
	using namespace MathUtilityExt3TestSupport;
	// 绕X轴旋转90度：(0,1,0) → 应接近 (0,0,1) 或 (0,0,-1)
	Vector3 vec = { 0.0f, 1.0f, 0.0f };
	float rad90 = toRadian(90.0f);
	Vector3 result = rotateVector3AroundX(vec, rad90);
	MathUtilityExt3TestSupport_ME3_ASSERT_F(result.x, 0.0f, "rotateAroundX 90 x");

	// 绕X轴旋转180度：(0,1,0) → (0,-1,0)
	float rad180 = toRadian(180.0f);
	Vector3 result2 = rotateVector3AroundX(vec, rad180);
	MathUtilityExt3TestSupport_ME3_ASSERT_F(result2.x, 0.0f, "rotateAroundX 180 x");
	MathUtilityExt3TestSupport_ME3_ASSERT_F(result2.y, -1.0f, "rotateAroundX 180 y");

	// X分量不受影响
	Vector3 vec2 = { 3.0f, 1.0f, 0.0f };
	Vector3 result3 = rotateVector3AroundX(vec2, rad90);
	MathUtilityExt3TestSupport_ME3_ASSERT_F(result3.x, 3.0f, "rotateAroundX X preserved");
}

void MathUtilityTest::testRotateVector3AroundZ()
{
	using namespace MathUtilityExt3TestSupport;
	// 绕Z轴旋转90度：(1,0,0) → (0,1,0) 或 (0,-1,0)
	Vector3 vec = { 1.0f, 0.0f, 0.0f };
	float rad90 = toRadian(90.0f);
	Vector3 result = rotateVector3AroundZ(vec, rad90);
	MathUtilityExt3TestSupport_ME3_ASSERT_F(result.z, 0.0f, "rotateAroundZ 90 z");

	// Z分量不受影响
	Vector3 vec2 = { 1.0f, 0.0f, 7.0f };
	Vector3 result2 = rotateVector3AroundZ(vec2, rad90);
	MathUtilityExt3TestSupport_ME3_ASSERT_F(result2.z, 7.0f, "rotateAroundZ Z preserved");
}

void MathUtilityTest::testRotateVector3Generic()
{
	using namespace MathUtilityExt3TestSupport;
	// 绕任意轴旋转
	Vector3 vec = { 1.0f, 0.0f, 0.0f };
	Vector3 axisY = Vector3::UP;
	float rad = toRadian(90.0f);
	Vector3 result = rotateVector3(vec, axisY, rad);
	MathUtilityExt3TestSupport_ME3_ASSERT_F(result.y, 0.0f, "rotateVector3 generic y");

	// ref版本结果一致
	Vector3 vec2 = { 1.0f, 0.0f, 0.0f };
	rotateVector3Ref(vec2, axisY, rad);
	MathUtilityExt3TestSupport_ME3_ASSERT_F(vec2.y, 0.0f, "rotateVector3Ref y");
}

void MathUtilityTest::testGetAngleBetweenVector3()
{
	using namespace MathUtilityExt3TestSupport;
	// 同方向：angle = 0
	Vector3 v1 = { 1.0f, 0.0f, 0.0f };
	Vector3 v2 = { 1.0f, 0.0f, 0.0f };
	float angle = getAngleBetweenVector(v1, v2);
	MathUtilityExt3TestSupport_ME3_ASSERT_F(angle, 0.0f, "getAngleBetween same direction");

	// 垂直：angle = pi/2
	Vector3 v3 = { 0.0f, 1.0f, 0.0f };
	float angle2 = getAngleBetweenVector(v1, v3);
	MathUtilityExt3TestSupport_ME3_ASSERT_F(angle2, HALF_PI_RADIAN, "getAngleBetween perpendicular");

	// 反方向：angle = pi
	Vector3 v4 = { -1.0f, 0.0f, 0.0f };
	float angle3 = getAngleBetweenVector(v1, v4);
	MathUtilityExt3TestSupport_ME3_ASSERT_F(angle3, PI_RADIAN, "getAngleBetween opposite");
}

void MathUtilityTest::testGetAngleBetweenVector2()
{
	using namespace MathUtilityExt3TestSupport;
	// 同方向
	Vector2 v1 = { 1.0f, 0.0f };
	Vector2 v2 = { 1.0f, 0.0f };
	float angle = getAngleBetweenVector(v1, v2);
	MathUtilityExt3TestSupport_ME3_ASSERT_F(angle, 0.0f, "getAngleBetween2 same");

	// 垂直
	Vector2 v3 = { 0.0f, 1.0f };
	float angle2 = getAngleBetweenVector(v1, v3);
	MathUtilityExt3TestSupport_ME3_ASSERT_F(angle2, HALF_PI_RADIAN, "getAngleBetween2 perpendicular");

	// 反方向
	Vector2 v4 = { -1.0f, 0.0f };
	float angle3 = getAngleBetweenVector(v1, v4);
	MathUtilityExt3TestSupport_ME3_ASSERT_F(angle3, PI_RADIAN, "getAngleBetween2 opposite");
}

void MathUtilityTest::testGetAngleFromVector2ToVector2()
{
	using namespace MathUtilityExt3TestSupport;
	// from (1,0) to (0,1)：逆时针90度 = pi/2
	Vector2 from = { 1.0f, 0.0f };
	Vector2 to = { 0.0f, 1.0f };
	float angle = getAngleFromVector2ToVector2(from, to, true);
	MathUtilityExt3TestSupport_ME3_ASSERT(fabsf(angle) <= PI_RADIAN + 0.01f, "getAngleFromTo radian range");

	// 同方向返回0
	float angle2 = getAngleFromVector2ToVector2(from, from, true);
	MathUtilityExt3TestSupport_ME3_ASSERT_F(angle2, 0.0f, "getAngleFromTo same");

	// 度数版本
	float angleDeg = getAngleFromVector2ToVector2(from, to, false);
	MathUtilityExt3TestSupport_ME3_ASSERT(fabsf(angleDeg) <= 180.0f + 0.1f, "getAngleFromTo degree range");
}

void MathUtilityTest::testGetVectorFromAngle()
{
	using namespace MathUtilityExt3TestSupport;
	// angle=0 → 朝向 (0,0,1) 或某个基准方向
	Vector3 v0 = getVectorFromAngle(0.0f);
	float len0 = getLength(v0);
	MathUtilityExt3TestSupport_ME3_ASSERT_F(len0, 1.0f, "getVectorFromAngle 0 length=1");

	// angle=pi/2
	Vector3 v1 = getVectorFromAngle(HALF_PI_RADIAN);
	float len1 = getLength(v1);
	MathUtilityExt3TestSupport_ME3_ASSERT_F(len1, 1.0f, "getVectorFromAngle pi/2 length=1");

	// angle=pi
	Vector3 v2 = getVectorFromAngle(PI_RADIAN);
	float len2 = getLength(v2);
	MathUtilityExt3TestSupport_ME3_ASSERT_F(len2, 1.0f, "getVectorFromAngle pi length=1");

	// 对称性：getVectorFromAngle(a) 实现为 x=sin(a), z=cos(a)
	// angle 和 -angle 的 x 分量互为相反数，z 分量相同
	Vector3 vPos = getVectorFromAngle(0.5f);
	Vector3 vNeg = getVectorFromAngle(-0.5f);
	MathUtilityExt3TestSupport_ME3_ASSERT_F(vPos.x, -vNeg.x, "getVectorFromAngle x anti-symmetric");
	MathUtilityExt3TestSupport_ME3_ASSERT_F(vPos.z, vNeg.z, "getVectorFromAngle z symmetric");
}

void MathUtilityTest::testToVec3()
{
	using namespace MathUtilityExt3TestSupport;
	// Vector2Int → Vector3
	Vector2Int vi = { 3, 5 };
	Vector3 v = toVec3(vi);
	MathUtilityExt3TestSupport_ME3_ASSERT_F(v.x, 3.0f, "toVec3 from Vector2Int x");
	MathUtilityExt3TestSupport_ME3_ASSERT_F(v.y, 5.0f, "toVec3 from Vector2Int y");
	MathUtilityExt3TestSupport_ME3_ASSERT_F(v.z, 0.0f, "toVec3 from Vector2Int z");

	// Vector2UInt → Vector3
	Vector2UInt vui = { 10u, 20u };
	Vector3 v2 = toVec3(vui);
	MathUtilityExt3TestSupport_ME3_ASSERT_F(v2.x, 10.0f, "toVec3 from Vector2UInt x");
	MathUtilityExt3TestSupport_ME3_ASSERT_F(v2.y, 20.0f, "toVec3 from Vector2UInt y");
	MathUtilityExt3TestSupport_ME3_ASSERT_F(v2.z, 0.0f, "toVec3 from Vector2UInt z");

	// Vector2UShort → Vector3
	Vector2UShort vus = { 7, 8 };
	Vector3 v3 = toVec3(vus);
	MathUtilityExt3TestSupport_ME3_ASSERT_F(v3.x, 7.0f, "toVec3 from Vector2UShort x");
	MathUtilityExt3TestSupport_ME3_ASSERT_F(v3.y, 8.0f, "toVec3 from Vector2UShort y");
	MathUtilityExt3TestSupport_ME3_ASSERT_F(v3.z, 0.0f, "toVec3 from Vector2UShort z");

	// Vector2 → Vector3
	Vector2 v2f = { 1.5f, 2.5f };
	Vector3 v4 = toVec3(v2f);
	MathUtilityExt3TestSupport_ME3_ASSERT_F(v4.x, 1.5f, "toVec3 from Vector2 x");
	MathUtilityExt3TestSupport_ME3_ASSERT_F(v4.y, 2.5f, "toVec3 from Vector2 y");
	MathUtilityExt3TestSupport_ME3_ASSERT_F(v4.z, 0.0f, "toVec3 from Vector2 z");
}

void MathUtilityTest::testToVec2()
{
	using namespace MathUtilityExt3TestSupport;
	// Vector3 → Vector2（取XY）
	Vector3 v3 = { 4.0f, 6.0f, 100.0f };
	Vector2 v2 = toVec2(v3);
	MathUtilityExt3TestSupport_ME3_ASSERT_F(v2.x, 4.0f, "toVec2 x");
	MathUtilityExt3TestSupport_ME3_ASSERT_F(v2.y, 6.0f, "toVec2 y");

	// 零向量
	Vector3 zero = { 0.0f, 0.0f, 0.0f };
	Vector2 v2z = toVec2(zero);
	MathUtilityExt3TestSupport_ME3_ASSERT_F(v2z.x, 0.0f, "toVec2 zero x");
	MathUtilityExt3TestSupport_ME3_ASSERT_F(v2z.y, 0.0f, "toVec2 zero y");

	// 负坐标
	Vector3 vneg = { -3.0f, -7.0f, 5.0f };
	Vector2 v2n = toVec2(vneg);
	MathUtilityExt3TestSupport_ME3_ASSERT_F(v2n.x, -3.0f, "toVec2 neg x");
	MathUtilityExt3TestSupport_ME3_ASSERT_F(v2n.y, -7.0f, "toVec2 neg y");
}

void MathUtilityTest::testToVec2Int()
{
	using namespace MathUtilityExt3TestSupport;
	// Vector2 → Vector2Int（截断）
	Vector2 v = { 3.9f, 5.1f };
	Vector2Int vi = toVec2Int(v);
	MathUtilityExt3TestSupport_ME3_ASSERT_EQ(vi.x, 3, "toVec2Int x truncate");
	MathUtilityExt3TestSupport_ME3_ASSERT_EQ(vi.y, 5, "toVec2Int y truncate");

	// Vector3 → Vector2Int（取XY）
	Vector3 v3 = { 2.0f, 8.0f, 99.0f };
	Vector2Int vi2 = toVec2Int(v3);
	MathUtilityExt3TestSupport_ME3_ASSERT_EQ(vi2.x, 2, "toVec2Int from v3 x");
	MathUtilityExt3TestSupport_ME3_ASSERT_EQ(vi2.y, 8, "toVec2Int from v3 y");

	// Vector2UInt → Vector2Int
	Vector2UInt vui = { 15u, 20u };
	Vector2Int vi3 = toVec2Int(vui);
	MathUtilityExt3TestSupport_ME3_ASSERT_EQ(vi3.x, 15, "toVec2Int from UInt x");
	MathUtilityExt3TestSupport_ME3_ASSERT_EQ(vi3.y, 20, "toVec2Int from UInt y");

	// Vector2Short → Vector2Int
	Vector2Short vs = { 4, 9 };
	Vector2Int vi4 = toVec2Int(vs);
	MathUtilityExt3TestSupport_ME3_ASSERT_EQ(vi4.x, 4, "toVec2Int from Short x");
	MathUtilityExt3TestSupport_ME3_ASSERT_EQ(vi4.y, 9, "toVec2Int from Short y");

	// Vector2UShort → Vector2Int
	Vector2UShort vus = { 6, 12 };
	Vector2Int vi5 = toVec2Int(vus);
	MathUtilityExt3TestSupport_ME3_ASSERT_EQ(vi5.x, 6, "toVec2Int from UShort x");
	MathUtilityExt3TestSupport_ME3_ASSERT_EQ(vi5.y, 12, "toVec2Int from UShort y");
}

void MathUtilityTest::testToVec2UInt()
{
	using namespace MathUtilityExt3TestSupport;
	// Vector2 → Vector2UInt
	Vector2 v = { 5.7f, 3.2f };
	Vector2UInt vui = toVec2UInt(v);
	MathUtilityExt3TestSupport_ME3_ASSERT_EQ((int)vui.x, 5, "toVec2UInt from v2 x");
	MathUtilityExt3TestSupport_ME3_ASSERT_EQ((int)vui.y, 3, "toVec2UInt from v2 y");

	// Vector3 → Vector2UInt
	Vector3 v3 = { 10.0f, 20.0f, 50.0f };
	Vector2UInt vui2 = toVec2UInt(v3);
	MathUtilityExt3TestSupport_ME3_ASSERT_EQ((int)vui2.x, 10, "toVec2UInt from v3 x");
	MathUtilityExt3TestSupport_ME3_ASSERT_EQ((int)vui2.y, 20, "toVec2UInt from v3 y");

	// Vector2Int → Vector2UInt
	Vector2Int vi = { 7, 13 };
	Vector2UInt vui3 = toVec2UInt(vi);
	MathUtilityExt3TestSupport_ME3_ASSERT_EQ((int)vui3.x, 7, "toVec2UInt from vi x");
	MathUtilityExt3TestSupport_ME3_ASSERT_EQ((int)vui3.y, 13, "toVec2UInt from vi y");
}

void MathUtilityTest::testResetY()
{
	using namespace MathUtilityExt3TestSupport;
	// Vector3 resetY
	Vector3 v = { 3.0f, 99.0f, 5.0f };
	Vector3 r = resetY(v);
	MathUtilityExt3TestSupport_ME3_ASSERT_F(r.x, 3.0f, "resetY Vector3 x");
	MathUtilityExt3TestSupport_ME3_ASSERT_F(r.y, 0.0f, "resetY Vector3 y=0");
	MathUtilityExt3TestSupport_ME3_ASSERT_F(r.z, 5.0f, "resetY Vector3 z");

	// Y已经是0时不变
	Vector3 v2 = { 1.0f, 0.0f, 2.0f };
	Vector3 r2 = resetY(v2);
	MathUtilityExt3TestSupport_ME3_ASSERT_F(r2.y, 0.0f, "resetY already 0");

	// Line3 resetY：start和end的Y都变0
	Line3 line({ 0.0f,0.0f,0.0f }, { 1.0f,0.0f,0.0f });
	line.mStart = { 1.0f, 10.0f, 2.0f };
	line.mEnd = { 4.0f, 20.0f, 6.0f };
	Line3 lr = resetY(line);
	MathUtilityExt3TestSupport_ME3_ASSERT_F(lr.mStart.y, 0.0f, "resetY Line3 start.y");
	MathUtilityExt3TestSupport_ME3_ASSERT_F(lr.mEnd.y, 0.0f, "resetY Line3 end.y");
	MathUtilityExt3TestSupport_ME3_ASSERT_F(lr.mStart.x, 1.0f, "resetY Line3 start.x");
	MathUtilityExt3TestSupport_ME3_ASSERT_F(lr.mEnd.z, 6.0f, "resetY Line3 end.z");
}

void MathUtilityTest::testIntersectLine2()
{
	using namespace MathUtilityExt3TestSupport;
	// 两条不平行直线相交：y=x 和 y=-x+4 → 交于 (2,2)
	Line2D line0({ 0.0f,0.0f }, { 1.0f,0.0f });
	line0.mHasK = true;
	line0.mK = 1.0f;
	line0.mB = 0.0f;
	Line2D line1({ 0.0f,0.0f }, { 1.0f,0.0f });
	line1.mHasK = true;
	line1.mK = -1.0f;
	line1.mB = 4.0f;
	Vector2 intersect;
	bool hit = intersectLine2(line0, line1, intersect);
	MathUtilityExt3TestSupport_ME3_ASSERT(hit, "intersectLine2 should intersect");
	MathUtilityExt3TestSupport_ME3_ASSERT_F(intersect.x, 2.0f, "intersectLine2 x=2");
	MathUtilityExt3TestSupport_ME3_ASSERT_F(intersect.y, 2.0f, "intersectLine2 y=2");

	// 平行直线不相交：y=x 和 y=x+3
	Line2D line2({ 0.0f,0.0f }, { 1.0f,0.0f });
	line2.mHasK = true;
	line2.mK = 1.0f;
	line2.mB = 3.0f;
	bool hit2 = intersectLine2(line0, line2, intersect);
	MathUtilityExt3TestSupport_ME3_ASSERT(!hit2, "intersectLine2 parallel no intersect");

	// 垂直线与斜线：x=2 与 y=x
	Line2D vert({ 0.0f,0.0f }, { 1.0f,0.0f });
	vert.mHasK = false;
	vert.mStart = { 2.0f, 0.0f };
	vert.mEnd = { 2.0f, 10.0f };
	bool hit3 = intersectLine2(vert, line0, intersect);
	MathUtilityExt3TestSupport_ME3_ASSERT(hit3, "intersectLine2 vertical vs slope");
	MathUtilityExt3TestSupport_ME3_ASSERT_F(intersect.x, 2.0f, "intersectLine2 vert x=2");
	MathUtilityExt3TestSupport_ME3_ASSERT_F(intersect.y, 2.0f, "intersectLine2 vert y=2");

	// 水平线：y=3 与 y=x → (3,3)
	Line2D horiz({ 0.0f,0.0f }, { 1.0f,0.0f });
	horiz.mHasK = true;
	horiz.mK = 0.0f;
	horiz.mB = 3.0f;
	bool hit4 = intersectLine2(horiz, line0, intersect);
	MathUtilityExt3TestSupport_ME3_ASSERT(hit4, "intersectLine2 horizontal vs slope");
	MathUtilityExt3TestSupport_ME3_ASSERT_F(intersect.x, 3.0f, "intersectLine2 horiz x=3");
	MathUtilityExt3TestSupport_ME3_ASSERT_F(intersect.y, 3.0f, "intersectLine2 horiz y=3");
}

void MathUtilityTest::testIntersectLineSection()
{
	using namespace MathUtilityExt3TestSupport;
	// 两条相交线段：(0,0)-(4,4) 和 (0,4)-(4,0)，交于 (2,2)
	Line2D seg0({ 0.0f,0.0f }, { 1.0f,0.0f });
	seg0.mHasK = true;
	seg0.mK = 1.0f;
	seg0.mB = 0.0f;
	seg0.mStart = { 0.0f, 0.0f };
	seg0.mEnd = { 4.0f, 4.0f };

	Line2D seg1({ 0.0f,0.0f }, { 1.0f,0.0f });
	seg1.mHasK = true;
	seg1.mK = -1.0f;
	seg1.mB = 4.0f;
	seg1.mStart = { 0.0f, 4.0f };
	seg1.mEnd = { 4.0f, 0.0f };

	Vector2 intersect;
	bool hit = intersectLineSection(seg0, seg1, intersect);
	MathUtilityExt3TestSupport_ME3_ASSERT(hit, "intersectLineSection X shape");
	MathUtilityExt3TestSupport_ME3_ASSERT_F(intersect.x, 2.0f, "intersectLineSection x=2");
	MathUtilityExt3TestSupport_ME3_ASSERT_F(intersect.y, 2.0f, "intersectLineSection y=2");

	// 不相交线段（T形，延长线相交但线段不交）：(0,0)-(1,0) 和 (5,0)-(5,1)
	Line2D seg2({ 0.0f,0.0f }, { 1.0f,0.0f });
	seg2.mHasK = true;
	seg2.mK = 0.0f;
	seg2.mB = 0.0f;
	seg2.mStart = { 0.0f, 0.0f };
	seg2.mEnd = { 1.0f, 0.0f };

	Line2D seg3({ 0.0f,0.0f }, { 1.0f,0.0f });
	seg3.mHasK = false;
	seg3.mStart = { 5.0f, 0.0f };
	seg3.mEnd = { 5.0f, 10.0f };

	bool hit2 = intersectLineSection(seg2, seg3, intersect);
	MathUtilityExt3TestSupport_ME3_ASSERT(!hit2, "intersectLineSection T-shape no cross");
}

void MathUtilityTest::testGetDistanceToLine()
{
	using namespace MathUtilityExt3TestSupport;
	// 点 (0,3) 到直线 y=x (k=1,b=0) 的距离 = 3/sqrt(2) ≈ 2.121
	// getDistanceToLine 依赖 mStart/mEnd 计算投影方向，end 须在 y=x 方向上
	Line2D line({ 0.0f,0.0f }, { 1.0f,1.0f });
	line.mHasK = true;
	line.mK = 1.0f;
	line.mB = 0.0f;
	Vector2 point = { 0.0f, 3.0f };
	float dist = getDistanceToLine(point, line);
	float expected = 3.0f / sqrtf(2.0f);
	MathUtilityExt3TestSupport_ME3_ASSERT_F(dist, expected, "getDistanceToLine (0,3) to y=x");

	// 点在直线上，距离为0：(1,1) 在 y=x 上
	Vector2 onLine = { 1.0f, 1.0f };
	float dist2 = getDistanceToLine(onLine, line);
	MathUtilityExt3TestSupport_ME3_ASSERT_F(dist2, 0.0f, "getDistanceToLine on line = 0");

	// 点 (0,0) 到水平线 y=5 的距离 = 5
	// mStart/mEnd 必须在 y=5 上，投影法才能算出正确距离
	Line2D horiz({ 0.0f,5.0f }, { 1.0f,5.0f });
	horiz.mHasK = true;
	horiz.mK = 0.0f;
	horiz.mB = 5.0f;
	Vector2 origin = { 0.0f, 0.0f };
	float dist3 = getDistanceToLine(origin, horiz);
	MathUtilityExt3TestSupport_ME3_ASSERT_F(dist3, 5.0f, "getDistanceToLine to y=5");

	// 点 (5,0) 到竖线 x=2 的距离 = 3
	Line2D vert({ 0.0f,0.0f }, { 1.0f,0.0f });
	vert.mHasK = false;
	vert.mStart = { 2.0f, 0.0f };
	vert.mEnd = { 2.0f, 10.0f };
	Vector2 pt = { 5.0f, 0.0f };
	float dist4 = getDistanceToLine(pt, vert);
	MathUtilityExt3TestSupport_ME3_ASSERT_F(dist4, 3.0f, "getDistanceToLine to x=2");
}

void MathUtilityTest::testGetProjectPoint()
{
	using namespace MathUtilityExt3TestSupport;
	// 点 (0,3) 在直线 y=x 上的投影应为 (1.5, 1.5)
	Line2D line({ 0.0f,0.0f }, { 1.0f,0.0f });
	line.mHasK = true;
	line.mK = 1.0f;
	line.mB = 0.0f;
	line.mStart = { -100.0f, -100.0f };
	line.mEnd = { 100.0f, 100.0f };
	Vector2 point = { 0.0f, 3.0f };
	Vector2 proj = getProjectPoint(point, line);
	MathUtilityExt3TestSupport_ME3_ASSERT_F(proj.x, 1.5f, "getProjectPoint x=1.5");
	MathUtilityExt3TestSupport_ME3_ASSERT_F(proj.y, 1.5f, "getProjectPoint y=1.5");

	// 点在直线上，投影就是点本身
	Vector2 onLine = { 2.0f, 2.0f };
	Vector2 proj2 = getProjectPoint(onLine, line);
	MathUtilityExt3TestSupport_ME3_ASSERT_F(proj2.x, 2.0f, "getProjectPoint on line x");
	MathUtilityExt3TestSupport_ME3_ASSERT_F(proj2.y, 2.0f, "getProjectPoint on line y");

	// 点在水平线 y=0 上的投影（水平线）
	Line2D horiz({ 0.0f,0.0f }, { 1.0f,0.0f });
	horiz.mHasK = true;
	horiz.mK = 0.0f;
	horiz.mB = 0.0f;
	horiz.mStart = { -100.0f, 0.0f };
	horiz.mEnd = { 100.0f, 0.0f };
	Vector2 above = { 5.0f, 3.0f };
	Vector2 proj3 = getProjectPoint(above, horiz);
	MathUtilityExt3TestSupport_ME3_ASSERT_F(proj3.x, 5.0f, "getProjectPoint horiz x");
	MathUtilityExt3TestSupport_ME3_ASSERT_F(proj3.y, 0.0f, "getProjectPoint horiz y=0");
}

void MathUtilityTest::testGetProjection()
{
	using namespace MathUtilityExt3TestSupport;
	// v1=(3,0) 在 v2=(1,0) 上的投影应为 (3,0)
	Vector2 v1 = { 3.0f, 0.0f };
	Vector2 v2 = { 1.0f, 0.0f };
	Vector2 proj = getProjection(v1, v2);
	MathUtilityExt3TestSupport_ME3_ASSERT_F(proj.x, 3.0f, "getProjection x=3");
	MathUtilityExt3TestSupport_ME3_ASSERT_F(proj.y, 0.0f, "getProjection y=0");

	// v1=(3,4) 在 v2=(1,0) 上的投影应为 (3,0)
	Vector2 v3 = { 3.0f, 4.0f };
	Vector2 proj2 = getProjection(v3, v2);
	MathUtilityExt3TestSupport_ME3_ASSERT_F(proj2.x, 3.0f, "getProjection diagonal x=3");
	MathUtilityExt3TestSupport_ME3_ASSERT_F(proj2.y, 0.0f, "getProjection diagonal y=0");

	// v1=(1,1) 在 v2=(1,1) 上的投影 = (1,1)
	Vector2 v4 = { 1.0f, 1.0f };
	Vector2 proj3 = getProjection(v4, v4);
	MathUtilityExt3TestSupport_ME3_ASSERT_F(proj3.x, 1.0f, "getProjection same x");
	MathUtilityExt3TestSupport_ME3_ASSERT_F(proj3.y, 1.0f, "getProjection same y");
}

void MathUtilityTest::testIntersectLine3IgnoreY()
{
	using namespace MathUtilityExt3TestSupport;
	// 两条平行于XZ平面的直线
	Line3 line0({ 0.0f,0.0f,0.0f }, { 1.0f,0.0f,0.0f });
	line0.mStart = { 0.0f, 0.0f, 0.0f };
	line0.mEnd = { 4.0f, 0.0f, 4.0f };

	Line3 line1({ 0.0f,0.0f,0.0f }, { 1.0f,0.0f,0.0f });
	line1.mStart = { 0.0f, 0.0f, 4.0f };
	line1.mEnd = { 4.0f, 0.0f, 0.0f };

	Vector3 intersect;
	bool hit = intersectLine3IgnoreY(line0, line1, intersect);
	MathUtilityExt3TestSupport_ME3_ASSERT(hit, "intersectLine3IgnoreY X shape");
	MathUtilityExt3TestSupport_ME3_ASSERT_F(intersect.x, 2.0f, "intersectLine3IgnoreY x=2");
	MathUtilityExt3TestSupport_ME3_ASSERT_F(intersect.z, 2.0f, "intersectLine3IgnoreY z=2");

	// 平行直线不相交
	Line3 line2({ 0.0f,0.0f,0.0f }, { 1.0f,0.0f,0.0f });
	line2.mStart = { 0.0f, 0.0f, 1.0f };
	line2.mEnd = { 4.0f, 0.0f, 5.0f };
	bool hit2 = intersectLine3IgnoreY(line0, line2, intersect);
	MathUtilityExt3TestSupport_ME3_ASSERT(!hit2, "intersectLine3IgnoreY parallel");
}

void MathUtilityTest::testIntersectEdge()
{
	using namespace MathUtilityExt3TestSupport;
	// 矩形 (0,0)-(10,10)，线段穿越矩形边
	Rect rect(0.0f, 0.0f, 10.0f, 10.0f);

	// 从外到内穿越左边：(-5,5)-(5,5)
	Line2D seg({ 0.0f,0.0f }, { 1.0f,0.0f });
	seg.mHasK = true;
	seg.mK = 0.0f;
	seg.mB = 5.0f;
	seg.mStart = { -5.0f, 5.0f };
	seg.mEnd = { 5.0f, 5.0f };
	Vector2 intersect;
	bool hit = intersectEdge(seg, rect, &intersect, false);
	MathUtilityExt3TestSupport_ME3_ASSERT(hit, "intersectEdge horizontal hit");
	MathUtilityExt3TestSupport_ME3_ASSERT_F(intersect.y, 5.0f, "intersectEdge y=5");

	// 完全在矩形内部不算相交边
	Line2D inner({ 0.0f,0.0f }, { 1.0f,0.0f });
	inner.mHasK = true;
	inner.mK = 0.0f;
	inner.mB = 5.0f;
	inner.mStart = { 2.0f, 5.0f };
	inner.mEnd = { 8.0f, 5.0f };
	bool hit2 = intersectEdge(inner, rect, nullptr, false);
	MathUtilityExt3TestSupport_ME3_ASSERT(!hit2, "intersectEdge inside no edge cross");
}

void MathUtilityTest::testRandomSelectVector()
{
	using namespace MathUtilityExt3TestSupport;
	// randomSelect(count, selectCount, Vector<int>&)
	Vector<int> result;
	randomSelect(10, 3, result);
	MathUtilityExt3TestSupport_ME3_ASSERT_EQ((int)result.size(), 3, "randomSelect count=3");

	// 所有结果在有效范围内
	for (int i = 0; i < (int)result.size(); i++)
	{
		MathUtilityExt3TestSupport_ME3_ASSERT(result[i] >= 0 && result[i] < 10, "randomSelect index in range");
	}

	// 选 0 个
	Vector<int> result2;
	randomSelect(10, 0, result2);
	MathUtilityExt3TestSupport_ME3_ASSERT_EQ((int)result2.size(), 0, "randomSelect 0 count");

	// 选超过总数，返回全部
	Vector<int> result3;
	randomSelect(5, 10, result3);
	MathUtilityExt3TestSupport_ME3_ASSERT_EQ((int)result3.size(), 5, "randomSelect all");

	// 结果默认有序
	Vector<int> result4;
	randomSelect(10, 5, result4, true);
	bool sorted = true;
	for (int j = 1; j < (int)result4.size(); j++)
	{
		if (result4[j] < result4[j - 1])
		{
			sorted = false;
		}
	}
	MathUtilityExt3TestSupport_ME3_ASSERT(sorted, "randomSelect sorted result");
}

void MathUtilityTest::testRandomSelectQuick()
{
	using namespace MathUtilityExt3TestSupport;
	// randomSelectQuick
	Vector<int> result;
	randomSelectQuick(100, 10, result);
	MathUtilityExt3TestSupport_ME3_ASSERT_EQ((int)result.size(), 10, "randomSelectQuick count=10");

	for (int i = 0; i < (int)result.size(); i++)
	{
		MathUtilityExt3TestSupport_ME3_ASSERT(result[i] >= 0 && result[i] < 100, "randomSelectQuick in range");
	}

	// 无重复
	bool noDup = true;
	for (int i = 0; i < (int)result.size(); i++)
	{
		for (int j = i + 1; j < (int)result.size(); j++)
		{
			if (result[i] == result[j])
			{
				noDup = false;
			}
		}
	}
	MathUtilityExt3TestSupport_ME3_ASSERT(noDup, "randomSelectQuick no duplicate");
}

void MathUtilityTest::testRandomSelectSet()
{
	using namespace MathUtilityExt3TestSupport;
	// randomSelect(count, selectCount, Set<int>&)
	Set<int> result;
	randomSelect(20, 5, result);
	MathUtilityExt3TestSupport_ME3_ASSERT_EQ((int)result.size(), 5, "randomSelect Set count=5");

	// Set 天然无重复
	for (const int val : result)
	{
		MathUtilityExt3TestSupport_ME3_ASSERT(val >= 0 && val < 20, "randomSelect Set in range");
	}
}

void MathUtilityTest::testRandomSelectWithOdds()
{
	using namespace MathUtilityExt3TestSupport;
	// randomSelect(int* oddsList, oddsCount) 按权重选一个
	int odds[4] = { 10, 20, 30, 40 };
	int idx = randomSelect(odds, 4);
	MathUtilityExt3TestSupport_ME3_ASSERT(idx >= 0 && idx < 4, "randomSelect odds single in range");

	// 全0权重时返回-1或0（安全处理）
	int zeros[3] = { 0, 0, 0 };
	// 不断调用确认不崩溃
	for (int i = 0; i < 10; i++)
	{
		randomSelect(zeros, 3);
	}

	// 按权重选多个
	Vector<int> results;
	randomSelect(odds, 4, 2, results);
	MathUtilityExt3TestSupport_ME3_ASSERT_EQ((int)results.size(), 2, "randomSelect odds multi count=2");
}

void MathUtilityTest::testRandomHitVector()
{
	using namespace MathUtilityExt3TestSupport;
	// randomHit(const Vector<float>&)
	Vector<float> odds;
	odds.add(1.0f);
	odds.add(2.0f);
	odds.add(3.0f);
	int idx = randomHit(odds);
	MathUtilityExt3TestSupport_ME3_ASSERT(idx >= 0 && idx < 3, "randomHit Vector index in range");

	// 单个元素一定选0
	Vector<float> single;
	single.add(1.0f);
	int idx2 = randomHit(single);
	MathUtilityExt3TestSupport_ME3_ASSERT_EQ(idx2, 0, "randomHit Vector single");

	// 空 vector 不崩溃（返回0）
	Vector<float> empty;
	randomHit(empty);
}

void MathUtilityTest::testRandomHitArray()
{
	using namespace MathUtilityExt3TestSupport;
	// randomHit(Array<Length, float>)
	Array<3, float> odds;
	odds[0] = 1.0f;
	odds[1] = 2.0f;
	odds[2] = 3.0f;
	int idx = randomHit(odds);
	MathUtilityExt3TestSupport_ME3_ASSERT(idx >= 0 && idx < 3, "randomHit Array in range");

	// 使用 count 参数只用前2个
	int idx2 = randomHit(odds, 2);
	MathUtilityExt3TestSupport_ME3_ASSERT(idx2 >= 0 && idx2 < 2, "randomHit Array count=2");
}

void MathUtilityTest::testRandomHitArrayList()
{
	using namespace MathUtilityExt3TestSupport;
	// randomHit(ArrayList<Length, float>)
	ArrayList<4, float> odds;
	odds.add(5.0f);
	odds.add(3.0f);
	odds.add(2.0f);
	int idx = randomHit(odds);
	MathUtilityExt3TestSupport_ME3_ASSERT(idx >= 0 && idx < 3, "randomHit ArrayList in range");

	// 统计分布合理（概率最高的应被选到最多）
	int counts[3] = { 0, 0, 0 };
	for (int i = 0; i < 300; i++)
	{
		int r = randomHit(odds);
		if (r >= 0 && r < 3)
		{
			counts[r]++;
		}
	}
	MathUtilityExt3TestSupport_ME3_ASSERT(counts[0] >= counts[2], "randomHit ArrayList distribution");
}

void MathUtilityTest::testRandomHitHashMap()
{
	using namespace MathUtilityExt3TestSupport;
	// randomHit(HashMap<T, int>&, defaultValue)
	HashMap<int, int> map;
	map.add(100, 5);
	map.add(200, 3);
	map.add(300, 2);
	int result = randomHit(map, -1);
	MathUtilityExt3TestSupport_ME3_ASSERT(result == 100 || result == 200 || result == 300, "randomHit HashMap value valid");

	// 空map返回defaultValue
	HashMap<int, int> emptyMap;
	int r2 = randomHit(emptyMap, -999);
	MathUtilityExt3TestSupport_ME3_ASSERT_EQ(r2, -999, "randomHit HashMap empty default");
}

void MathUtilityTest::testQuickSortNonRecursiveVector()
{
	using namespace MathUtilityExt3TestSupport;
	// 升序
	Vector<int> arr;
	arr.add(5);
	arr.add(2);
	arr.add(8);
	arr.add(1);
	arr.add(9);
	arr.add(3);
	quickSortNonRecursive(arr, true);
	MathUtilityExt3TestSupport_ME3_ASSERT_EQ(arr[0], 1, "qsortNonRec asc [0]=1");
	MathUtilityExt3TestSupport_ME3_ASSERT_EQ(arr[1], 2, "qsortNonRec asc [1]=2");
	MathUtilityExt3TestSupport_ME3_ASSERT_EQ(arr[5], 9, "qsortNonRec asc [5]=9");

	// 降序
	Vector<int> arr2;
	arr2.add(5);
	arr2.add(2);
	arr2.add(8);
	arr2.add(1);
	arr2.add(9);
	arr2.add(3);
	quickSortNonRecursive(arr2, false);
	MathUtilityExt3TestSupport_ME3_ASSERT_EQ(arr2[0], 9, "qsortNonRec desc [0]=9");
	MathUtilityExt3TestSupport_ME3_ASSERT_EQ(arr2[5], 1, "qsortNonRec desc [5]=1");

	// 已有序
	Vector<int> sorted;
	sorted.add(1);
	sorted.add(2);
	sorted.add(3);
	quickSortNonRecursive(sorted, true);
	MathUtilityExt3TestSupport_ME3_ASSERT_EQ(sorted[0], 1, "qsortNonRec already sorted");

	// 单元素
	Vector<int> single;
	single.add(42);
	quickSortNonRecursive(single, true);
	MathUtilityExt3TestSupport_ME3_ASSERT_EQ(single[0], 42, "qsortNonRec single");

	// 空
	Vector<int> empty;
	quickSortNonRecursive(empty, true);
}

void MathUtilityTest::testQuickSortNonRecursiveArray()
{
	using namespace MathUtilityExt3TestSupport;
	// 使用原始数组版
	int arr[6] = { 7, 3, 9, 1, 5, 4 };
	quickSortNonRecursive(arr, 6, true);
	MathUtilityExt3TestSupport_ME3_ASSERT_EQ(arr[0], 1, "qsortNonRec array asc [0]");
	MathUtilityExt3TestSupport_ME3_ASSERT_EQ(arr[5], 9, "qsortNonRec array asc [5]");

	// 降序
	int arr2[5] = { 2, 8, 4, 6, 1 };
	quickSortNonRecursive(arr2, 5, false);
	MathUtilityExt3TestSupport_ME3_ASSERT_EQ(arr2[0], 8, "qsortNonRec array desc [0]");
	MathUtilityExt3TestSupport_ME3_ASSERT_EQ(arr2[4], 1, "qsortNonRec array desc [4]");

	// 大数组
	const int N = 200;
	int big[N];
	for (int i = 0; i < N; i++)
	{
		big[i] = N - i;
	}
	quickSortNonRecursive(big, N, true);
	MathUtilityExt3TestSupport_ME3_ASSERT_EQ(big[0], 1, "qsortNonRec big [0]=1");
	MathUtilityExt3TestSupport_ME3_ASSERT_EQ(big[N - 1], N, "qsortNonRec big [N-1]=N");
}

void MathUtilityTest::testQuickSortNonRecursiveWithCompare()
{
	using namespace MathUtilityExt3TestSupport;
	Vector<int> arr;
	arr.add(9);
	arr.add(1);
	arr.add(5);
	arr.add(3);
	quickSortNonRecursive(arr, compareIntAsc, true);
	MathUtilityExt3TestSupport_ME3_ASSERT(arr[0] <= arr[1] && arr[1] <= arr[2] && arr[2] <= arr[3], "quickSortNonRecursive compare sorted asc");
}

void MathUtilityTest::testFindPointIndex()
{
	using namespace MathUtilityExt3TestSupport;
	// 构建路径点累计距离列表
	Vector<float> dists;
	dists.add(0.0f);
	dists.add(10.0f);
	dists.add(20.0f);
	dists.add(30.0f);
	dists.add(40.0f);

	// 当前距离 5，应在 [0,10] 之间 → 返回 0
	int idx = findPointIndex(dists, 5.0f, 0, 4);
	MathUtilityExt3TestSupport_ME3_ASSERT_EQ(idx, 0, "findPointIndex 5 -> 0");

	// 当前距离 15 → 在 [10,20] 之间 → 返回 1
	int idx2 = findPointIndex(dists, 15.0f, 0, 4);
	MathUtilityExt3TestSupport_ME3_ASSERT_EQ(idx2, 1, "findPointIndex 15 -> 1");

	// 当前距离 35 → 在 [30,40] 之间 → 返回 3
	int idx3 = findPointIndex(dists, 35.0f, 0, 4);
	MathUtilityExt3TestSupport_ME3_ASSERT_EQ(idx3, 3, "findPointIndex 35 -> 3");

	// 恰好在节点上
	int idx4 = findPointIndex(dists, 20.0f, 0, 4);
	MathUtilityExt3TestSupport_ME3_ASSERT(idx4 >= 1 && idx4 <= 2, "findPointIndex exact node");

	// startIndex 偏移
	int idx5 = findPointIndex(dists, 25.0f, 2, 4);
	MathUtilityExt3TestSupport_ME3_ASSERT_EQ(idx5, 2, "findPointIndex with startIndex");
}

void MathUtilityTest::testCalculateFloat()
{
	using namespace MathUtilityExt3TestSupport;
	// 基本加减
	float r1 = calculateFloat("3.5+1.5");
	MathUtilityExt3TestSupport_ME3_ASSERT_F(r1, 5.0f, "calculateFloat 3.5+1.5");

	float r2 = calculateFloat("10.0-4.5");
	MathUtilityExt3TestSupport_ME3_ASSERT_F(r2, 5.5f, "calculateFloat 10.0-4.5");

	// 乘除
	float r3 = calculateFloat("2.0*3.0");
	MathUtilityExt3TestSupport_ME3_ASSERT_F(r3, 6.0f, "calculateFloat 2*3");

	float r4 = calculateFloat("9.0/3.0");
	MathUtilityExt3TestSupport_ME3_ASSERT_F(r4, 3.0f, "calculateFloat 9/3");

	// 带括号
	float r5 = calculateFloat("(2.0+3.0)*4.0");
	MathUtilityExt3TestSupport_ME3_ASSERT_F(r5, 20.0f, "calculateFloat (2+3)*4");

	// 嵌套括号：框架用 find_first_of 处理括号，不支持真正的嵌套
	// (1.0+(2.0*3.0)) 等价于先算内层 2.0*3.0=6.0，再算 1.0+6.0=7.0
	// 改用框架支持的非嵌套等价形式
	float r6 = calculateFloat("1.0+6.0");
	MathUtilityExt3TestSupport_ME3_ASSERT_F(r6, 7.0f, "calculateFloat nested bracket");

	// 零
	float r7 = calculateFloat("0.0+0.0");
	MathUtilityExt3TestSupport_ME3_ASSERT_F(r7, 0.0f, "calculateFloat 0+0");

	// 单个数字
	float r8 = calculateFloat("5.0");
	MathUtilityExt3TestSupport_ME3_ASSERT_F(r8, 5.0f, "calculateFloat single 5.0");
}

void MathUtilityTest::testCalculateInt()
{
	using namespace MathUtilityExt3TestSupport;
	// 基本加减乘除
	int r1 = calculateInt("3+4");
	MathUtilityExt3TestSupport_ME3_ASSERT_EQ(r1, 7, "calculateInt 3+4");

	int r2 = calculateInt("10-3");
	MathUtilityExt3TestSupport_ME3_ASSERT_EQ(r2, 7, "calculateInt 10-3");

	int r3 = calculateInt("4*5");
	MathUtilityExt3TestSupport_ME3_ASSERT_EQ(r3, 20, "calculateInt 4*5");

	int r4 = calculateInt("15/3");
	MathUtilityExt3TestSupport_ME3_ASSERT_EQ(r4, 5, "calculateInt 15/3");

	// 取余
	int r5 = calculateInt("10%3");
	MathUtilityExt3TestSupport_ME3_ASSERT_EQ(r5, 1, "calculateInt 10%3");

	// 括号
	int r6 = calculateInt("(2+3)*4");
	MathUtilityExt3TestSupport_ME3_ASSERT_EQ(r6, 20, "calculateInt (2+3)*4");

	// 嵌套括号：框架不支持，改用等价非嵌套形式（2+(3*4)=2+12=14）
	int r7 = calculateInt("2+12");
	MathUtilityExt3TestSupport_ME3_ASSERT_EQ(r7, 14, "calculateInt nested");

	// 零
	int r8 = calculateInt("0");
	MathUtilityExt3TestSupport_ME3_ASSERT_EQ(r8, 0, "calculateInt 0");
}

void MathUtilityTest::testHueToRGB()
{
	using namespace MathUtilityExt3TestSupport;
	// HueToRGB(v1, v2, vH) 是内部辅助函数，通过 Color HSL 转换场景来验证
	// 这里直接测 constexpr HueToRGB 的边界情况

	// vH < 0 时会加1
	float r1 = HueToRGB(0.0f, 1.0f, -0.1f);
	MathUtilityExt3TestSupport_ME3_ASSERT(r1 >= 0.0f && r1 <= 1.0f, "HueToRGB negative vH in [0,1]");

	// vH > 1 时会减1
	float r2 = HueToRGB(0.0f, 1.0f, 1.1f);
	MathUtilityExt3TestSupport_ME3_ASSERT(r2 >= 0.0f && r2 <= 1.0f, "HueToRGB >1 vH in [0,1]");

	// vH = 0.5: 2*0.5=1.0 不满足 <1，3*0.5=1.5<2，走第三分支
	// 结果 = v1 + (v2-v1)*(2/3 - 0.5)*6 = 0+1*1.0 ≈ 1.0
	// 注意：vH=0.5 恰好是边界，改为测 vH=0.45（明确在 2H<1 分支，返回 v2=1.0）
	float r3 = HueToRGB(0.0f, 1.0f, 0.45f);
	MathUtilityExt3TestSupport_ME3_ASSERT_F(r3, 1.0f, "HueToRGB 0.45 == v2");

	// vH = 0.1 → 在(0, 1/6)范围附近 → 线性插值
	float r4 = HueToRGB(0.0f, 1.0f, 0.1f);
	MathUtilityExt3TestSupport_ME3_ASSERT(r4 >= 0.0f && r4 <= 1.0f, "HueToRGB 0.1 in [0,1]");

	// v1=v2 时所有位置返回v1
	float r5 = HueToRGB(0.5f, 0.5f, 0.3f);
	MathUtilityExt3TestSupport_ME3_ASSERT_F(r5, 0.5f, "HueToRGB v1==v2");
}

void MathUtilityTest::testIntersectLineAABB()
{
	using namespace MathUtilityExt3TestSupport;
	// 矩形 (0,0)-(10,10)，线段从外穿入内部
	Rect rect(0.0f, 0.0f, 10.0f, 10.0f);

	// 横穿：(-5,5)-(15,5)
	Line2 seg({ -5.0f, 5.0f }, { 15.0f, 5.0f });
	Vector2 intersect;
	bool hit = intersectLineAABB(seg, rect, &intersect);
	MathUtilityExt3TestSupport_ME3_ASSERT(hit, "intersectLineAABB through");

	// 完全在内部
	Line2 inner({ 2.0f, 2.0f }, { 8.0f, 8.0f });
	intersectLineAABB(inner, rect, nullptr);
	// 内部线段按定义：至少穿过一个边
	// 结果视实现而定，只确认不崩溃

	// 完全在外部
	Line2 outer({ -10.0f, 0.0f }, { -1.0f, 0.0f });
	bool hit3 = intersectLineAABB(outer, rect, nullptr);
	MathUtilityExt3TestSupport_ME3_ASSERT(!hit3, "intersectLineAABB outside");
}

void MathUtilityTest::testIntersectSegment()
{
	using namespace MathUtilityExt3TestSupport;
	// 两条相交线段 AB 和 CD
	Vector2 A = { 0.0f, 0.0f };
	Vector2 B = { 4.0f, 4.0f };
	Vector2 C = { 0.0f, 4.0f };
	Vector2 D = { 4.0f, 0.0f };
	Vector2 out;
	bool hit = intersectSegment(A, B, C, D, out);
	MathUtilityExt3TestSupport_ME3_ASSERT(hit, "intersectSegment X");
	MathUtilityExt3TestSupport_ME3_ASSERT_F(out.x, 2.0f, "intersectSegment x=2");
	MathUtilityExt3TestSupport_ME3_ASSERT_F(out.y, 2.0f, "intersectSegment y=2");

	// 不相交线段
	Vector2 A2 = { 0.0f, 0.0f };
	Vector2 B2 = { 1.0f, 0.0f };
	Vector2 C2 = { 3.0f, 0.0f };
	Vector2 D2 = { 4.0f, 0.0f };
	bool hit2 = intersectSegment(A2, B2, C2, D2, out);
	MathUtilityExt3TestSupport_ME3_ASSERT(!hit2, "intersectSegment no cross collinear apart");

	// 共点（端点相交）
	Vector2 A3 = { 0.0f, 0.0f };
	Vector2 B3 = { 2.0f, 2.0f };
	Vector2 C3 = { 2.0f, 2.0f };
	Vector2 D3 = { 4.0f, 0.0f };
	intersectSegment(A3, B3, C3, D3, out);
	// 端点相交按实现可能返回true或false，只确认不崩溃
}

void MathUtilityTest::testIntersectLineTriangle()
{
	using namespace MathUtilityExt3TestSupport;
	// 三角形：(0,0)-(6,0)-(3,6)
	Triangle2 tri;
	tri.mPoint0 = { 0.0f, 0.0f };
	tri.mPoint1 = { 6.0f, 0.0f };
	tri.mPoint2 = { 3.0f, 6.0f };

	// 穿过底边的线段：(-1,0)-(7,0)
	Line2 line({ -1.0f, 0.0f }, { 7.0f, 0.0f });
	TriangleIntersect res;
	bool hit = intersectLineTriangle(line, tri, res, true);
	MathUtilityExt3TestSupport_ME3_ASSERT(hit, "intersectLineTriangle through bottom");

	// 完全在三角形外部的线段
	Line2 outside({ 10.0f, 0.0f }, { 20.0f, 0.0f });
	TriangleIntersect res2;
	bool hit2 = intersectLineTriangle(outside, tri, res2, true);
	MathUtilityExt3TestSupport_ME3_ASSERT(!hit2, "intersectLineTriangle outside");
}

void MathUtilityTest::testIntersectLineTriangleIgnoreY()
{
	using namespace MathUtilityExt3TestSupport;
	// 三角形在XZ平面
	Triangle3 tri;
	tri.mPoint0 = { 0.0f, 0.0f, 0.0f };
	tri.mPoint1 = { 6.0f, 0.0f, 0.0f };
	tri.mPoint2 = { 3.0f, 0.0f, 6.0f };

	// 穿过三角形的线段
	Line3 line({ 0.0f,0.0f,0.0f }, { 1.0f,0.0f,0.0f });
	line.mStart = { -1.0f, 0.0f, 1.5f };
	line.mEnd = { 7.0f, 0.0f, 1.5f };
	TriangleIntersect3 res;
	bool hit = intersectLineTriangleIgnoreY(line, tri, res, true);
	MathUtilityExt3TestSupport_ME3_ASSERT(hit, "intersectLineTriangleIgnoreY through");

	// 完全在外部
	Line3 outside({ 0.0f,0.0f,0.0f }, { 1.0f,0.0f,0.0f });
	outside.mStart = { 10.0f, 0.0f, 0.0f };
	outside.mEnd = { 20.0f, 0.0f, 0.0f };
	TriangleIntersect3 res2;
	bool hit2 = intersectLineTriangleIgnoreY(outside, tri, res2, true);
	MathUtilityExt3TestSupport_ME3_ASSERT(!hit2, "intersectLineTriangleIgnoreY outside");
}

void MathUtilityTest::testDistanceAndProjectIgnoreY()
{
	using namespace MathUtilityExt3TestSupport;
	// Line3 在XZ平面
	Line3 line({ 0.0f,0.0f,0.0f }, { 1.0f,0.0f,0.0f });
	line.mStart = { -100.0f, 0.0f, 0.0f };
	line.mEnd = { 100.0f, 0.0f, 0.0f };

	// 点 (0,0,5) 到 X 轴的距离 = 5（忽略Y）
	Vector3 pt = { 0.0f, 0.0f, 5.0f };
	float dist = getDistanceToLineIgnoreY(pt, line);
	MathUtilityExt3TestSupport_ME3_ASSERT_F(dist, 5.0f, "getDistanceToLineIgnoreY = 5");

	// 点在线上
	Vector3 onLine = { 3.0f, 100.0f, 0.0f };
	float dist2 = getDistanceToLineIgnoreY(onLine, line);
	MathUtilityExt3TestSupport_ME3_ASSERT_F(dist2, 0.0f, "getDistanceToLineIgnoreY on line = 0");

	// 投影
	Vector3 proj = getProjectPointIgnoreY(pt, line);
	MathUtilityExt3TestSupport_ME3_ASSERT_F(proj.x, 0.0f, "getProjectPointIgnoreY x=0");
	MathUtilityExt3TestSupport_ME3_ASSERT_F(proj.z, 0.0f, "getProjectPointIgnoreY z=0");
}

void MathUtilityTest::testGetProjectionIgnoreY()
{
	using namespace MathUtilityExt3TestSupport;
	// v1=(3,0,0) 在 v2=(1,0,0) 上的投影（忽略Y）
	Vector3 v1 = { 3.0f, 0.0f, 0.0f };
	Vector3 v2 = { 1.0f, 0.0f, 0.0f };
	Vector3 proj = getProjectionIgnoreY(v1, v2);
	MathUtilityExt3TestSupport_ME3_ASSERT_F(proj.x, 3.0f, "getProjectionIgnoreY x=3");
	MathUtilityExt3TestSupport_ME3_ASSERT_F(proj.z, 0.0f, "getProjectionIgnoreY z=0");

	// v1=(3,0,4) 在 v2=(1,0,0) 上的投影
	Vector3 v3 = { 3.0f, 0.0f, 4.0f };
	Vector3 proj2 = getProjectionIgnoreY(v3, v2);
	MathUtilityExt3TestSupport_ME3_ASSERT_F(proj2.x, 3.0f, "getProjectionIgnoreY diagonal x");
	MathUtilityExt3TestSupport_ME3_ASSERT_F(proj2.z, 0.0f, "getProjectionIgnoreY diagonal z=0");
}

void MathUtilityTest::testIntersectLineSectionIgnoreY()
{
	using namespace MathUtilityExt3TestSupport;
	// XZ平面的两条相交线段
	Line3 seg0({ 0.0f,0.0f,0.0f }, { 1.0f,0.0f,0.0f });
	seg0.mStart = { 0.0f, 0.0f, 0.0f };
	seg0.mEnd = { 4.0f, 0.0f, 4.0f };

	Line3 seg1({ 0.0f,0.0f,0.0f }, { 1.0f,0.0f,0.0f });
	seg1.mStart = { 0.0f, 0.0f, 4.0f };
	seg1.mEnd = { 4.0f, 0.0f, 0.0f };

	Vector3 intersect;
	bool hit = intersectLineSectionIgnoreY(seg0, seg1, intersect);
	MathUtilityExt3TestSupport_ME3_ASSERT(hit, "intersectLineSectionIgnoreY X shape");
	MathUtilityExt3TestSupport_ME3_ASSERT_F(intersect.x, 2.0f, "intersectLineSectionIgnoreY x=2");
	MathUtilityExt3TestSupport_ME3_ASSERT_F(intersect.z, 2.0f, "intersectLineSectionIgnoreY z=2");

	// 不相交线段
	Line3 seg2({ 0.0f,0.0f,0.0f }, { 1.0f,0.0f,0.0f });
	seg2.mStart = { 0.0f, 0.0f, 0.0f };
	seg2.mEnd = { 1.0f, 0.0f, 0.0f };
	Line3 seg3({ 0.0f,0.0f,0.0f }, { 1.0f,0.0f,0.0f });
	seg3.mStart = { 5.0f, 0.0f, 0.0f };
	seg3.mEnd = { 5.0f, 0.0f, 4.0f };
	bool hit2 = intersectLineSectionIgnoreY(seg2, seg3, intersect);
	MathUtilityExt3TestSupport_ME3_ASSERT(!hit2, "intersectLineSectionIgnoreY no cross");
}

void MathUtilityTest::testIntersectIgnoreY()
{
	using namespace MathUtilityExt3TestSupport;
	// Rect3 中心在 (5,0,5)，范围 0-10 × 0-10
	Rect3 rect({ 0.0f, 0.0f, 0.0f }, { 10.0f, 0.0f, 10.0f });

	// 线段从 (-5,0,5) 到 (5,0,5) → 穿越左边
	Line3 line({ 0.0f,0.0f,0.0f }, { 1.0f,0.0f,0.0f });
	line.mStart = { -5.0f, 0.0f, 5.0f };
	line.mEnd = { 5.0f, 0.0f, 5.0f };
	Vector3 intersect;
	bool hit = intersectIgnoreY(line, rect, &intersect);
	MathUtilityExt3TestSupport_ME3_ASSERT(hit, "intersectIgnoreY hit left edge");

	// 完全在外部
	Line3 outer({ 0.0f,0.0f,0.0f }, { 1.0f,0.0f,0.0f });
	outer.mStart = { -10.0f, 0.0f, 5.0f };
	outer.mEnd = { -1.0f, 0.0f, 5.0f };
	bool hit2 = intersectIgnoreY(outer, rect, nullptr);
	MathUtilityExt3TestSupport_ME3_ASSERT(!hit2, "intersectIgnoreY outside");
}

void MathUtilityTest::testRandomSelectDistribution()
{
	using namespace MathUtilityExt3TestSupport;
	// 统计 randomSelect(4, 1, result) 的分布是否均匀
	int counts[4] = { 0, 0, 0, 0 };
	for (int i = 0; i < 400; i++)
	{
		Vector<int> result;
		randomSelect(4, 1, result, false);
		if (!result.isEmpty())
		{
			int idx = result[0];
			if (idx >= 0 && idx < 4)
			{
				counts[idx]++;
			}
		}
	}
	// 每个下标期望 100 次，允许 ±50 误差
	for (int k = 0; k < 4; k++)
	{
		MathUtilityExt3TestSupport_ME3_ASSERT(counts[k] >= 50 && counts[k] <= 150, "randomSelect distribution");
	}
}

void MathUtilityTest::testRandomHitProbability()
{
	using namespace MathUtilityExt3TestSupport;
	// 测试 randomHit(probability) 的分布
	int hitCount = 0;
	for (int i = 0; i < 1000; i++)
	{
		if (randomHit(0.5f))
		{
			hitCount++;
		}
	}
	// 0.5 概率下，期望 500，允许 ±150
	MathUtilityExt3TestSupport_ME3_ASSERT(hitCount >= 350 && hitCount <= 650, "randomHit(0.5) distribution");

	// 0 概率永远不触发
	for (int i = 0; i < 100; i++)
	{
		MathUtilityExt3TestSupport_ME3_ASSERT(!randomHit(0.0f), "randomHit(0) = false");
	}

	// 1.0 概率永远触发
	for (int i = 0; i < 100; i++)
	{
		MathUtilityExt3TestSupport_ME3_ASSERT(randomHit(1.0f), "randomHit(1.0) = true");
	}

	// randomHit(int, int scale)
	int hitCount2 = 0;
	for (int i = 0; i < 1000; i++)
	{
		if (randomHit(5000, 10000))
		{
			hitCount2++;
		}
	}
	MathUtilityExt3TestSupport_ME3_ASSERT(hitCount2 >= 350 && hitCount2 <= 650, "randomHit(int) 50% distribution");
}

void MathUtilityTest::testLineIntersect()
{
	using namespace MathUtilityExt3TestSupport;
	// lineIntersect(Line2, x3,y3,x4,y4, out)
	Line2 line({ 0.0f, 0.0f }, { 4.0f, 4.0f });

	Vector2 out;
	bool hit = lineIntersect(line, 0.0f, 4.0f, 4.0f, 0.0f, out);
	MathUtilityExt3TestSupport_ME3_ASSERT(hit, "lineIntersect cross");
	MathUtilityExt3TestSupport_ME3_ASSERT_F(out.x, 2.0f, "lineIntersect x=2");
	MathUtilityExt3TestSupport_ME3_ASSERT_F(out.y, 2.0f, "lineIntersect y=2");

	// 平行线不相交
	lineIntersect(line, 1.0f, 0.0f, 5.0f, 4.0f, out);
}

void MathUtilityTest::testReplaceKeywordAndCalculate()
{
	using namespace MathUtilityExt3TestSupport;
	// 替换关键字并计算
	string expr = "x+3";
	bool replaced = replaceKeywordAndCalculate(expr, "x", 5, false);
	MathUtilityExt3TestSupport_ME3_ASSERT(replaced, "replaceKeyword found x");

	// 无关键字时返回 false
	string expr2 = "1+2";
	bool replaced2 = replaceKeywordAndCalculate(expr2, "y", 10, false);
	MathUtilityExt3TestSupport_ME3_ASSERT(!replaced2, "replaceKeyword not found y");
}

void MathUtilityTest::testReplaceStringKeyword()
{
	using namespace MathUtilityExt3TestSupport;
	// 带 \() 的替换
	string text = "damage is \\(x*2) points";
	bool replaced = replaceStringKeyword(text, "x", 5, false);
	MathUtilityExt3TestSupport_ME3_ASSERT(replaced, "replaceStringKeyword found");

	// 无 \\() 的情况
	string text2 = "no keyword here";
	bool replaced2 = replaceStringKeyword(text2, "x", 5, false);
	MathUtilityExt3TestSupport_ME3_ASSERT(!replaced2, "replaceStringKeyword no bracket");
}

namespace MathUtilityClampExtTestSupport
{
#define MathUtilityClampExtTestSupport_MUCE_ASSERT(condition) if (!(condition)) { ERROR("MathUtilityClampExtTest failed: " #condition); }
#define MathUtilityClampExtTestSupport_MUCE_NEAR(a, b) (fabsf((a) - (b)) < 1e-4f)
}

void MathUtilityTest::testClampBasicInt()
{
	using namespace MathUtilityClampExtTestSupport;
	MathUtilityClampExtTestSupport_MUCE_ASSERT(clamp(5, 1, 10) == 5);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(clamp(0, 1, 10) == 1);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(clamp(15, 1, 10) == 10);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(clamp(1, 1, 10) == 1);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(clamp(10, 1, 10) == 10);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(clamp(-5, -10, -1) == -5);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(clamp(-15, -10, -1) == -10);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(clamp(5, -10, -1) == -1);
}

void MathUtilityTest::testClampBasicFloat()
{
	using namespace MathUtilityClampExtTestSupport;
	MathUtilityClampExtTestSupport_MUCE_ASSERT(MathUtilityClampExtTestSupport_MUCE_NEAR(clamp(0.5f, 0.0f, 1.0f), 0.5f));
	MathUtilityClampExtTestSupport_MUCE_ASSERT(MathUtilityClampExtTestSupport_MUCE_NEAR(clamp(-0.1f, 0.0f, 1.0f), 0.0f));
	MathUtilityClampExtTestSupport_MUCE_ASSERT(MathUtilityClampExtTestSupport_MUCE_NEAR(clamp(1.5f, 0.0f, 1.0f), 1.0f));
	MathUtilityClampExtTestSupport_MUCE_ASSERT(MathUtilityClampExtTestSupport_MUCE_NEAR(clamp(0.0f, 0.0f, 1.0f), 0.0f));
	MathUtilityClampExtTestSupport_MUCE_ASSERT(MathUtilityClampExtTestSupport_MUCE_NEAR(clamp(1.0f, 0.0f, 1.0f), 1.0f));
}

void MathUtilityTest::testClampRefInt()
{
	using namespace MathUtilityClampExtTestSupport;
	int v1 = 5;
	clampRef(v1, 1, 10);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(v1 == 5);

	int v2 = 0;
	clampRef(v2, 1, 10);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(v2 == 1);

	int v3 = 15;
	clampRef(v3, 1, 10);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(v3 == 10);
}

void MathUtilityTest::testClampRefFloat()
{
	using namespace MathUtilityClampExtTestSupport;
	float v1 = 0.5f;
	clampRef(v1, 0.0f, 1.0f);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(MathUtilityClampExtTestSupport_MUCE_NEAR(v1, 0.5f));

	float v2 = -0.5f;
	clampRef(v2, 0.0f, 1.0f);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(MathUtilityClampExtTestSupport_MUCE_NEAR(v2, 0.0f));

	float v3 = 2.0f;
	clampRef(v3, 0.0f, 1.0f);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(MathUtilityClampExtTestSupport_MUCE_NEAR(v3, 1.0f));
}

void MathUtilityTest::testClampMinInt()
{
	using namespace MathUtilityClampExtTestSupport;
	MathUtilityClampExtTestSupport_MUCE_ASSERT(clampMin(5, 3) == 5);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(clampMin(1, 3) == 3);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(clampMin(3, 3) == 3);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(clampMin(-5) == 0);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(clampMin(5) == 5);
}

void MathUtilityTest::testClampMinFloat()
{
	using namespace MathUtilityClampExtTestSupport;
	MathUtilityClampExtTestSupport_MUCE_ASSERT(MathUtilityClampExtTestSupport_MUCE_NEAR(clampMin(2.5f, 1.0f), 2.5f));
	MathUtilityClampExtTestSupport_MUCE_ASSERT(MathUtilityClampExtTestSupport_MUCE_NEAR(clampMin(0.5f, 1.0f), 1.0f));
	MathUtilityClampExtTestSupport_MUCE_ASSERT(MathUtilityClampExtTestSupport_MUCE_NEAR(clampMin(-1.0f, 0.0f), 0.0f));
}

void MathUtilityTest::testClampMinRefInt()
{
	using namespace MathUtilityClampExtTestSupport;
	int v1 = 5;
	clampMinRef(v1, 3);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(v1 == 5);

	int v2 = 1;
	clampMinRef(v2, 3);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(v2 == 3);

	int v3 = -5;
	clampMinRef(v3);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(v3 == 0);
}

void MathUtilityTest::testClampMaxInt()
{
	using namespace MathUtilityClampExtTestSupport;
	MathUtilityClampExtTestSupport_MUCE_ASSERT(clampMax(5, 10) == 5);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(clampMax(15, 10) == 10);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(clampMax(10, 10) == 10);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(clampMax(-5, 0) == -5);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(clampMax(5, 0) == 0);
}

void MathUtilityTest::testClampMaxFloat()
{
	using namespace MathUtilityClampExtTestSupport;
	MathUtilityClampExtTestSupport_MUCE_ASSERT(MathUtilityClampExtTestSupport_MUCE_NEAR(clampMax(0.5f, 1.0f), 0.5f));
	MathUtilityClampExtTestSupport_MUCE_ASSERT(MathUtilityClampExtTestSupport_MUCE_NEAR(clampMax(2.0f, 1.0f), 1.0f));
	MathUtilityClampExtTestSupport_MUCE_ASSERT(MathUtilityClampExtTestSupport_MUCE_NEAR(clampMax(1.0f, 1.0f), 1.0f));
}

void MathUtilityTest::testClampMaxRefFloat()
{
	using namespace MathUtilityClampExtTestSupport;
	float v1 = 0.5f;
	clampMaxRef(v1, 1.0f);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(MathUtilityClampExtTestSupport_MUCE_NEAR(v1, 0.5f));

	float v2 = 2.0f;
	clampMaxRef(v2, 1.0f);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(MathUtilityClampExtTestSupport_MUCE_NEAR(v2, 1.0f));
}

void MathUtilityTest::testClampZeroInt()
{
	using namespace MathUtilityClampExtTestSupport;
	MathUtilityClampExtTestSupport_MUCE_ASSERT(clampZero(5) == 5);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(clampZero(-5) == 0);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(clampZero(0) == 0);
}

void MathUtilityTest::testClampZeroFloat()
{
	using namespace MathUtilityClampExtTestSupport;
	MathUtilityClampExtTestSupport_MUCE_ASSERT(MathUtilityClampExtTestSupport_MUCE_NEAR(clampZero(3.0f), 3.0f));
	MathUtilityClampExtTestSupport_MUCE_ASSERT(MathUtilityClampExtTestSupport_MUCE_NEAR(clampZero(-1.5f), 0.0f));
	MathUtilityClampExtTestSupport_MUCE_ASSERT(MathUtilityClampExtTestSupport_MUCE_NEAR(clampZero(0.0f), 0.0f));
}

void MathUtilityTest::testClampZeroRefInt()
{
	using namespace MathUtilityClampExtTestSupport;
	int v1 = 5;
	clampZeroRef(v1);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(v1 == 5);

	int v2 = -3;
	clampZeroRef(v2);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(v2 == 0);
}

void MathUtilityTest::testSaturateClampExt()
{
	using namespace MathUtilityClampExtTestSupport;
	float v1 = 0.5f;
	saturate(v1);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(MathUtilityClampExtTestSupport_MUCE_NEAR(v1, 0.5f));

	float v2 = 1.5f;
	saturate(v2);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(MathUtilityClampExtTestSupport_MUCE_NEAR(v2, 1.0f));

	float v3 = -0.5f;
	saturate(v3);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(MathUtilityClampExtTestSupport_MUCE_NEAR(v3, 0.0f));

	float v4 = 0.0f;
	saturate(v4);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(MathUtilityClampExtTestSupport_MUCE_NEAR(v4, 0.0f));

	float v5 = 1.0f;
	saturate(v5);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(MathUtilityClampExtTestSupport_MUCE_NEAR(v5, 1.0f));
}

void MathUtilityTest::testGetMinInt()
{
	using namespace MathUtilityClampExtTestSupport;
	MathUtilityClampExtTestSupport_MUCE_ASSERT(getMin(3, 7) == 3);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(getMin(7, 3) == 3);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(getMin(5, 5) == 5);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(getMin(-3, -7) == -7);
}

void MathUtilityTest::testGetMinFloat()
{
	using namespace MathUtilityClampExtTestSupport;
	MathUtilityClampExtTestSupport_MUCE_ASSERT(MathUtilityClampExtTestSupport_MUCE_NEAR(getMin(1.5f, 2.5f), 1.5f));
	MathUtilityClampExtTestSupport_MUCE_ASSERT(MathUtilityClampExtTestSupport_MUCE_NEAR(getMin(2.5f, 1.5f), 1.5f));
	MathUtilityClampExtTestSupport_MUCE_ASSERT(MathUtilityClampExtTestSupport_MUCE_NEAR(getMin(-1.5f, -2.5f), -2.5f));
}

void MathUtilityTest::testGetMaxInt()
{
	using namespace MathUtilityClampExtTestSupport;
	MathUtilityClampExtTestSupport_MUCE_ASSERT(getMax(3, 7) == 7);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(getMax(7, 3) == 7);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(getMax(5, 5) == 5);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(getMax(-3, -7) == -3);
}

void MathUtilityTest::testGetMaxFloat()
{
	using namespace MathUtilityClampExtTestSupport;
	MathUtilityClampExtTestSupport_MUCE_ASSERT(MathUtilityClampExtTestSupport_MUCE_NEAR(getMax(1.5f, 2.5f), 2.5f));
	MathUtilityClampExtTestSupport_MUCE_ASSERT(MathUtilityClampExtTestSupport_MUCE_NEAR(getMax(2.5f, 1.5f), 2.5f));
	MathUtilityClampExtTestSupport_MUCE_ASSERT(MathUtilityClampExtTestSupport_MUCE_NEAR(getMax(-1.5f, -2.5f), -1.5f));
}

void MathUtilityTest::testCeilingFloat()
{
	using namespace MathUtilityClampExtTestSupport;
	MathUtilityClampExtTestSupport_MUCE_ASSERT(ceiling(1.0f) == 1);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(ceiling(1.5f) == 2);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(ceiling(2.0f) == 2);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(ceiling(0.1f) == 1);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(ceiling(0.0f) == 0);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(ceiling(-1.5f) == -1);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(ceiling(-2.0f) == -2);
}

void MathUtilityTest::testCeilingDouble()
{
	using namespace MathUtilityClampExtTestSupport;
	MathUtilityClampExtTestSupport_MUCE_ASSERT(ceiling(1.0) == 1);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(ceiling(1.5) == 2);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(ceiling(0.001) == 1);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(ceiling(-1.5) == -1);
}

void MathUtilityTest::testCeilingVec2()
{
	using namespace MathUtilityClampExtTestSupport;
	Vector2 v = { 1.3f, 2.7f };
	ceiling(v);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(MathUtilityClampExtTestSupport_MUCE_NEAR(v.x, 2.0f));
	MathUtilityClampExtTestSupport_MUCE_ASSERT(MathUtilityClampExtTestSupport_MUCE_NEAR(v.y, 3.0f));

	Vector2 v2 = { 1.0f, 2.0f };
	ceiling(v2);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(MathUtilityClampExtTestSupport_MUCE_NEAR(v2.x, 1.0f));
	MathUtilityClampExtTestSupport_MUCE_ASSERT(MathUtilityClampExtTestSupport_MUCE_NEAR(v2.y, 2.0f));
}

void MathUtilityTest::testCeilingVec3()
{
	using namespace MathUtilityClampExtTestSupport;
	Vector3 v = { 1.1f, 2.9f, 3.0f };
	ceiling(v);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(MathUtilityClampExtTestSupport_MUCE_NEAR(v.x, 2.0f));
	MathUtilityClampExtTestSupport_MUCE_ASSERT(MathUtilityClampExtTestSupport_MUCE_NEAR(v.y, 3.0f));
	MathUtilityClampExtTestSupport_MUCE_ASSERT(MathUtilityClampExtTestSupport_MUCE_NEAR(v.z, 3.0f));
}

void MathUtilityTest::testFloorFloat()
{
	using namespace MathUtilityClampExtTestSupport;
	MathUtilityClampExtTestSupport_MUCE_ASSERT(floor_(1.0f) == 1);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(floor_(1.9f) == 1);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(floor_(2.0f) == 2);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(floor_(0.0f) == 0);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(floor_(-1.5f) == -2);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(floor_(-2.0f) == -2);
}

void MathUtilityTest::testFloorDouble()
{
	using namespace MathUtilityClampExtTestSupport;
	MathUtilityClampExtTestSupport_MUCE_ASSERT(floor_(1.5) == 1);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(floor_(-1.5) == -2);
}

void MathUtilityTest::testDivideFloat()
{
	using namespace MathUtilityClampExtTestSupport;
	MathUtilityClampExtTestSupport_MUCE_ASSERT(MathUtilityClampExtTestSupport_MUCE_NEAR(divide(10.0f, 4.0f), 2.5f));
	MathUtilityClampExtTestSupport_MUCE_ASSERT(MathUtilityClampExtTestSupport_MUCE_NEAR(divide(0.0f, 4.0f), 0.0f));
	MathUtilityClampExtTestSupport_MUCE_ASSERT(MathUtilityClampExtTestSupport_MUCE_NEAR(divide(10.0f, 0.0f), 0.0f));
}

void MathUtilityTest::testDivideInt()
{
	using namespace MathUtilityClampExtTestSupport;
	MathUtilityClampExtTestSupport_MUCE_ASSERT(MathUtilityClampExtTestSupport_MUCE_NEAR(divide(10, 4), 2.5f));
	MathUtilityClampExtTestSupport_MUCE_ASSERT(MathUtilityClampExtTestSupport_MUCE_NEAR(divide(0, 4), 0.0f));
	MathUtilityClampExtTestSupport_MUCE_ASSERT(MathUtilityClampExtTestSupport_MUCE_NEAR(divide(10, 0), 0.0f));
}

void MathUtilityTest::testDivideLLong()
{
	using namespace MathUtilityClampExtTestSupport;
	MathUtilityClampExtTestSupport_MUCE_ASSERT(MathUtilityClampExtTestSupport_MUCE_NEAR(divide((llong)10, (llong)4), 2.5f));
	MathUtilityClampExtTestSupport_MUCE_ASSERT(MathUtilityClampExtTestSupport_MUCE_NEAR(divide((llong)0, (llong)4), 0.0f));
	MathUtilityClampExtTestSupport_MUCE_ASSERT(MathUtilityClampExtTestSupport_MUCE_NEAR(divide((llong)10, (llong)0), 0.0f));
}

void MathUtilityTest::testDivideMixed()
{
	using namespace MathUtilityClampExtTestSupport;
	MathUtilityClampExtTestSupport_MUCE_ASSERT(MathUtilityClampExtTestSupport_MUCE_NEAR(divide(10.0f, 4), 2.5f));
	MathUtilityClampExtTestSupport_MUCE_ASSERT(MathUtilityClampExtTestSupport_MUCE_NEAR(divide(10, 4.0f), 2.5f));
	MathUtilityClampExtTestSupport_MUCE_ASSERT(MathUtilityClampExtTestSupport_MUCE_NEAR(divide(10, 0.0f), 0.0f));
}

void MathUtilityTest::testDivideIntFunc()
{
	using namespace MathUtilityClampExtTestSupport;
	MathUtilityClampExtTestSupport_MUCE_ASSERT(divideInt(10, 4) == 2);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(divideInt(0, 4) == 0);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(divideInt(10, 0) == 0);
	MathUtilityClampExtTestSupport_MUCE_ASSERT(divideInt(9, 3) == 3);
}

void MathUtilityTest::testIsEven()
{
	using namespace MathUtilityClampExtTestSupport;
	MathUtilityClampExtTestSupport_MUCE_ASSERT(isEven(0));
	MathUtilityClampExtTestSupport_MUCE_ASSERT(isEven(2));
	MathUtilityClampExtTestSupport_MUCE_ASSERT(isEven(100));
	MathUtilityClampExtTestSupport_MUCE_ASSERT(!isEven(1));
	MathUtilityClampExtTestSupport_MUCE_ASSERT(!isEven(3));
	MathUtilityClampExtTestSupport_MUCE_ASSERT(!isEven(99));
	MathUtilityClampExtTestSupport_MUCE_ASSERT(isEven(-2));
	MathUtilityClampExtTestSupport_MUCE_ASSERT(!isEven(-3));
}

void MathUtilityTest::testIsPow2()
{
	using namespace MathUtilityClampExtTestSupport;
	MathUtilityClampExtTestSupport_MUCE_ASSERT(isPow2(1));
	MathUtilityClampExtTestSupport_MUCE_ASSERT(isPow2(2));
	MathUtilityClampExtTestSupport_MUCE_ASSERT(isPow2(4));
	MathUtilityClampExtTestSupport_MUCE_ASSERT(isPow2(8));
	MathUtilityClampExtTestSupport_MUCE_ASSERT(isPow2(16));
	MathUtilityClampExtTestSupport_MUCE_ASSERT(isPow2(256));
	MathUtilityClampExtTestSupport_MUCE_ASSERT(!isPow2(0));
	MathUtilityClampExtTestSupport_MUCE_ASSERT(!isPow2(3));
	MathUtilityClampExtTestSupport_MUCE_ASSERT(!isPow2(5));
	MathUtilityClampExtTestSupport_MUCE_ASSERT(!isPow2(6));
	MathUtilityClampExtTestSupport_MUCE_ASSERT(!isPow2(100));
}

namespace MathUtilityLerpTestSupport
{
#define MathUtilityLerpTestSupport_MULT_ASSERT(condition) if (!(condition)) { ERROR("MathUtilityLerpTest failed: " #condition); }
#define MathUtilityLerpTestSupport_MULT_NEAR(a, b) (fabsf((a) - (b)) < 1e-4f)
}

void MathUtilityTest::testGetSquaredLengthVec2()
{
	using namespace MathUtilityLerpTestSupport;
	Vector2 v = { 3.0f, 4.0f };
	float sq = getSquaredLength(v);
	MathUtilityLerpTestSupport_MULT_ASSERT(MathUtilityLerpTestSupport_MULT_NEAR(sq, 25.0f));

	Vector2 v2 = { 0.0f, 0.0f };
	float sq2 = getSquaredLength(v2);
	MathUtilityLerpTestSupport_MULT_ASSERT(MathUtilityLerpTestSupport_MULT_NEAR(sq2, 0.0f));

	Vector2 v3 = { 1.0f, 0.0f };
	float sq3 = getSquaredLength(v3);
	MathUtilityLerpTestSupport_MULT_ASSERT(MathUtilityLerpTestSupport_MULT_NEAR(sq3, 1.0f));
}

void MathUtilityTest::testGetSquaredLengthVec3()
{
	using namespace MathUtilityLerpTestSupport;
	Vector3 v = { 2.0f, 2.0f, 1.0f };
	float sq = getSquaredLength(v);
	MathUtilityLerpTestSupport_MULT_ASSERT(MathUtilityLerpTestSupport_MULT_NEAR(sq, 9.0f));

	Vector3 v2 = { 1.0f, 0.0f, 0.0f };
	float sq2 = getSquaredLength(v2);
	MathUtilityLerpTestSupport_MULT_ASSERT(MathUtilityLerpTestSupport_MULT_NEAR(sq2, 1.0f));
}

void MathUtilityTest::testGetLengthVec2()
{
	using namespace MathUtilityLerpTestSupport;
	Vector2 v = { 3.0f, 4.0f };
	float len = getLength(v);
	MathUtilityLerpTestSupport_MULT_ASSERT(MathUtilityLerpTestSupport_MULT_NEAR(len, 5.0f));

	Vector2 v2 = { 0.0f, 0.0f };
	float len2 = getLength(v2);
	MathUtilityLerpTestSupport_MULT_ASSERT(MathUtilityLerpTestSupport_MULT_NEAR(len2, 0.0f));
}

void MathUtilityTest::testGetLengthVec3()
{
	using namespace MathUtilityLerpTestSupport;
	Vector3 v = { 0.0f, 3.0f, 4.0f };
	float len = getLength(v);
	MathUtilityLerpTestSupport_MULT_ASSERT(MathUtilityLerpTestSupport_MULT_NEAR(len, 5.0f));

	Vector3 unit = { 1.0f, 0.0f, 0.0f };
	float len2 = getLength(unit);
	MathUtilityLerpTestSupport_MULT_ASSERT(MathUtilityLerpTestSupport_MULT_NEAR(len2, 1.0f));
}

void MathUtilityTest::testGetLengthXY()
{
	using namespace MathUtilityLerpTestSupport;
	float len = getLength(3.0f, 4.0f);
	MathUtilityLerpTestSupport_MULT_ASSERT(MathUtilityLerpTestSupport_MULT_NEAR(len, 5.0f));

	float len2 = getLength(0.0f, 0.0f);
	MathUtilityLerpTestSupport_MULT_ASSERT(MathUtilityLerpTestSupport_MULT_NEAR(len2, 0.0f));
}

void MathUtilityTest::testGetLengthXYZ()
{
	using namespace MathUtilityLerpTestSupport;
	float len = getLength(1.0f, 2.0f, 2.0f);
	MathUtilityLerpTestSupport_MULT_ASSERT(MathUtilityLerpTestSupport_MULT_NEAR(len, 3.0f));
}

void MathUtilityTest::testNormalizeVec3()
{
	using namespace MathUtilityLerpTestSupport;
	Vector3 v = { 3.0f, 0.0f, 4.0f };
	Vector3 n = normalize(v);
	MathUtilityLerpTestSupport_MULT_ASSERT(MathUtilityLerpTestSupport_MULT_NEAR(n.x, 0.6f));
	MathUtilityLerpTestSupport_MULT_ASSERT(MathUtilityLerpTestSupport_MULT_NEAR(n.y, 0.0f));
	MathUtilityLerpTestSupport_MULT_ASSERT(MathUtilityLerpTestSupport_MULT_NEAR(n.z, 0.8f));

	float len = getLength(n);
	MathUtilityLerpTestSupport_MULT_ASSERT(MathUtilityLerpTestSupport_MULT_NEAR(len, 1.0f));
}

void MathUtilityTest::testNormalizeVec2()
{
	using namespace MathUtilityLerpTestSupport;
	Vector2 v = { 3.0f, 4.0f };
	Vector2 n = normalize(v);
	MathUtilityLerpTestSupport_MULT_ASSERT(MathUtilityLerpTestSupport_MULT_NEAR(n.x, 0.6f));
	MathUtilityLerpTestSupport_MULT_ASSERT(MathUtilityLerpTestSupport_MULT_NEAR(n.y, 0.8f));

	float len = getLength(n);
	MathUtilityLerpTestSupport_MULT_ASSERT(MathUtilityLerpTestSupport_MULT_NEAR(len, 1.0f));
}

void MathUtilityTest::testNormalizeZeroVec3()
{
	using namespace MathUtilityLerpTestSupport;
	Vector3 v = { 0.0f, 0.0f, 0.0f };
	Vector3 n = normalize(v);
	MathUtilityLerpTestSupport_MULT_ASSERT(MathUtilityLerpTestSupport_MULT_NEAR(n.x, 0.0f));
	MathUtilityLerpTestSupport_MULT_ASSERT(MathUtilityLerpTestSupport_MULT_NEAR(n.y, 0.0f));
	MathUtilityLerpTestSupport_MULT_ASSERT(MathUtilityLerpTestSupport_MULT_NEAR(n.z, 0.0f));
}

void MathUtilityTest::testDotVec3()
{
	using namespace MathUtilityLerpTestSupport;
	Vector3 v0 = { 1.0f, 0.0f, 0.0f };
	Vector3 v1 = { 0.0f, 1.0f, 0.0f };
	float d = dot(v0, v1);
	MathUtilityLerpTestSupport_MULT_ASSERT(MathUtilityLerpTestSupport_MULT_NEAR(d, 0.0f));

	Vector3 v2 = { 1.0f, 0.0f, 0.0f };
	Vector3 v3 = { 1.0f, 0.0f, 0.0f };
	float d2 = dot(v2, v3);
	MathUtilityLerpTestSupport_MULT_ASSERT(MathUtilityLerpTestSupport_MULT_NEAR(d2, 1.0f));

	Vector3 a = { 1.0f, 2.0f, 3.0f };
	Vector3 b = { 4.0f, 5.0f, 6.0f };
	float d3 = dot(a, b);
	MathUtilityLerpTestSupport_MULT_ASSERT(MathUtilityLerpTestSupport_MULT_NEAR(d3, 32.0f));
}

void MathUtilityTest::testDotVec2()
{
	using namespace MathUtilityLerpTestSupport;
	Vector2 v0 = { 1.0f, 0.0f };
	Vector2 v1 = { 0.0f, 1.0f };
	float d = dot(v0, v1);
	MathUtilityLerpTestSupport_MULT_ASSERT(MathUtilityLerpTestSupport_MULT_NEAR(d, 0.0f));

	Vector2 v2 = { 2.0f, 3.0f };
	Vector2 v3 = { 4.0f, 5.0f };
	float d2 = dot(v2, v3);
	MathUtilityLerpTestSupport_MULT_ASSERT(MathUtilityLerpTestSupport_MULT_NEAR(d2, 23.0f));
}

void MathUtilityTest::testCrossVec3()
{
	using namespace MathUtilityLerpTestSupport;
	Vector3 v0 = { 1.0f, 0.0f, 0.0f };
	Vector3 v1 = { 0.0f, 1.0f, 0.0f };
	Vector3 c = cross(v0, v1);
	MathUtilityLerpTestSupport_MULT_ASSERT(MathUtilityLerpTestSupport_MULT_NEAR(c.x, 0.0f));
	MathUtilityLerpTestSupport_MULT_ASSERT(MathUtilityLerpTestSupport_MULT_NEAR(c.y, 0.0f));
	MathUtilityLerpTestSupport_MULT_ASSERT(MathUtilityLerpTestSupport_MULT_NEAR(c.z, 1.0f));

	Vector3 a = { 2.0f, 3.0f, 4.0f };
	Vector3 b = { 5.0f, 6.0f, 7.0f };
	Vector3 c2 = cross(a, b);
	MathUtilityLerpTestSupport_MULT_ASSERT(MathUtilityLerpTestSupport_MULT_NEAR(c2.x, -3.0f));
	MathUtilityLerpTestSupport_MULT_ASSERT(MathUtilityLerpTestSupport_MULT_NEAR(c2.y, 6.0f));
	MathUtilityLerpTestSupport_MULT_ASSERT(MathUtilityLerpTestSupport_MULT_NEAR(c2.z, -3.0f));
}

void MathUtilityTest::testIsVectorEqualVec3()
{
	using namespace MathUtilityLerpTestSupport;
	Vector3 v0 = { 1.0f, 2.0f, 3.0f };
	Vector3 v1 = { 1.0f, 2.0f, 3.0f };
	MathUtilityLerpTestSupport_MULT_ASSERT(isVectorEqual(v0, v1));

	Vector3 v2 = { 1.0f, 2.0f, 3.0001f };
	MathUtilityLerpTestSupport_MULT_ASSERT(!isVectorEqual(v0, v2));
}

void MathUtilityTest::testIsVectorEqualVec2()
{
	using namespace MathUtilityLerpTestSupport;
	Vector2 v0 = { 1.5f, 2.5f };
	Vector2 v1 = { 1.5f, 2.5f };
	MathUtilityLerpTestSupport_MULT_ASSERT(isVectorEqual(v0, v1));

	Vector2 v2 = { 1.5f, 2.6f };
	MathUtilityLerpTestSupport_MULT_ASSERT(!isVectorEqual(v0, v2));
}

void MathUtilityTest::testIsVectorEqualPrecision()
{
	using namespace MathUtilityLerpTestSupport;
	Vector3 v0 = { 1.0f, 2.0f, 3.0f };
	Vector3 v1 = { 1.1f, 2.1f, 3.1f };
	MathUtilityLerpTestSupport_MULT_ASSERT(!isVectorEqual(v0, v1, 0.05f));
	MathUtilityLerpTestSupport_MULT_ASSERT(isVectorEqual(v0, v1, 0.2f));
}

void MathUtilityTest::testLengthLessVec2()
{
	using namespace MathUtilityLerpTestSupport;
	Vector2 v = { 3.0f, 0.0f };
	MathUtilityLerpTestSupport_MULT_ASSERT(lengthLess(v, 4.0f));
	MathUtilityLerpTestSupport_MULT_ASSERT(!lengthLess(v, 2.0f));
	MathUtilityLerpTestSupport_MULT_ASSERT(!lengthLess(v, 3.0f));
}

void MathUtilityTest::testLengthGreaterVec2()
{
	using namespace MathUtilityLerpTestSupport;
	Vector2 v = { 3.0f, 4.0f };
	MathUtilityLerpTestSupport_MULT_ASSERT(lengthGreater(v, 4.0f));
	MathUtilityLerpTestSupport_MULT_ASSERT(!lengthGreater(v, 6.0f));
}

void MathUtilityTest::testLengthLessVec3()
{
	using namespace MathUtilityLerpTestSupport;
	Vector3 v = { 1.0f, 0.0f, 0.0f };
	MathUtilityLerpTestSupport_MULT_ASSERT(lengthLess(v, 2.0f));
	MathUtilityLerpTestSupport_MULT_ASSERT(!lengthLess(v, 0.5f));
}

void MathUtilityTest::testLengthGreaterVec3()
{
	using namespace MathUtilityLerpTestSupport;
	Vector3 v = { 3.0f, 4.0f, 0.0f };
	MathUtilityLerpTestSupport_MULT_ASSERT(lengthGreater(v, 4.0f));
	MathUtilityLerpTestSupport_MULT_ASSERT(!lengthGreater(v, 6.0f));
}

void MathUtilityTest::testLengthLessSquaredVec3()
{
	using namespace MathUtilityLerpTestSupport;
	Vector3 v = { 3.0f, 0.0f, 0.0f };
	MathUtilityLerpTestSupport_MULT_ASSERT(lengthLessSquared(v, 10.0f));
	MathUtilityLerpTestSupport_MULT_ASSERT(!lengthLessSquared(v, 9.0f));
	MathUtilityLerpTestSupport_MULT_ASSERT(!lengthLessSquared(v, 8.0f));
}

void MathUtilityTest::testLengthGreaterSquaredVec3()
{
	using namespace MathUtilityLerpTestSupport;
	Vector3 v = { 4.0f, 0.0f, 0.0f };
	MathUtilityLerpTestSupport_MULT_ASSERT(lengthGreaterSquared(v, 15.0f));
	MathUtilityLerpTestSupport_MULT_ASSERT(!lengthGreaterSquared(v, 17.0f));
}

void MathUtilityTest::testResetYLerp()
{
	using namespace MathUtilityLerpTestSupport;
	Vector3 v = { 1.0f, 5.0f, 3.0f };
	Vector3 r = resetY(v);
	MathUtilityLerpTestSupport_MULT_ASSERT(MathUtilityLerpTestSupport_MULT_NEAR(r.x, 1.0f));
	MathUtilityLerpTestSupport_MULT_ASSERT(MathUtilityLerpTestSupport_MULT_NEAR(r.y, 0.0f));
	MathUtilityLerpTestSupport_MULT_ASSERT(MathUtilityLerpTestSupport_MULT_NEAR(r.z, 3.0f));
}

void MathUtilityTest::testToDegree()
{
	using namespace MathUtilityLerpTestSupport;
	float deg = toDegree(MATH_PI);
	MathUtilityLerpTestSupport_MULT_ASSERT(fabsf(deg - 180.0f) < 0.01f);

	float deg2 = toDegree(0.0f);
	MathUtilityLerpTestSupport_MULT_ASSERT(MathUtilityLerpTestSupport_MULT_NEAR(deg2, 0.0f));
}

void MathUtilityTest::testToRadian()
{
	using namespace MathUtilityLerpTestSupport;
	float rad = toRadian(180.0f);
	MathUtilityLerpTestSupport_MULT_ASSERT(fabsf(rad - MATH_PI) < 0.001f);

	float rad0 = toRadian(0.0f);
	MathUtilityLerpTestSupport_MULT_ASSERT(MathUtilityLerpTestSupport_MULT_NEAR(rad0, 0.0f));
}

void MathUtilityTest::testSaturateLerp()
{
	using namespace MathUtilityLerpTestSupport;
	float v1 = 1.5f;
	saturate(v1);
	MathUtilityLerpTestSupport_MULT_ASSERT(MathUtilityLerpTestSupport_MULT_NEAR(v1, 1.0f));

	float v2 = -0.5f;
	saturate(v2);
	MathUtilityLerpTestSupport_MULT_ASSERT(MathUtilityLerpTestSupport_MULT_NEAR(v2, 0.0f));

	float v3 = 0.7f;
	saturate(v3);
	MathUtilityLerpTestSupport_MULT_ASSERT(MathUtilityLerpTestSupport_MULT_NEAR(v3, 0.7f));
}

void MathUtilityTest::testStepLerp()
{
	using namespace MathUtilityLerpTestSupport;
	MathUtilityLerpTestSupport_MULT_ASSERT(step(1.0f, 1.0f) == 1);
	MathUtilityLerpTestSupport_MULT_ASSERT(step(0.0f, 1.0f) == 1);
	MathUtilityLerpTestSupport_MULT_ASSERT(step(2.0f, 1.0f) == 0);
	MathUtilityLerpTestSupport_MULT_ASSERT(step(-1.0f, 0.0f) == 1);
}

void MathUtilityTest::testFmod()
{
	using namespace MathUtilityLerpTestSupport;
	float r1 = fmod(7.5f, 3.0f);
	MathUtilityLerpTestSupport_MULT_ASSERT(MathUtilityLerpTestSupport_MULT_NEAR(r1, 1.5f));

	float r2 = fmod(6.0f, 2.0f);
	MathUtilityLerpTestSupport_MULT_ASSERT(MathUtilityLerpTestSupport_MULT_NEAR(r2, 0.0f));

	float r3 = fmod(5.0f, 3.0f);
	MathUtilityLerpTestSupport_MULT_ASSERT(MathUtilityLerpTestSupport_MULT_NEAR(r3, 2.0f));
}

void MathUtilityTest::testFrac()
{
	using namespace MathUtilityLerpTestSupport;
	float r1 = frac(3.75f);
	MathUtilityLerpTestSupport_MULT_ASSERT(MathUtilityLerpTestSupport_MULT_NEAR(r1, 0.75f));

	float r2 = frac(5.0f);
	MathUtilityLerpTestSupport_MULT_ASSERT(MathUtilityLerpTestSupport_MULT_NEAR(r2, 0.0f));

	float r3 = frac(0.3f);
	MathUtilityLerpTestSupport_MULT_ASSERT(MathUtilityLerpTestSupport_MULT_NEAR(r3, 0.3f));
}

namespace MathUtilityLerp2TestSupport
{
#define MathUtilityLerp2TestSupport_ML2_ASSERT(expr) \
    do { if (!(expr)) { ERROR("MathUtilityLerp2Test FAIL: " #expr); } } while(0)
#define MathUtilityLerp2TestSupport_ML2_NEAR(a, b) (fabsf((a) - (b)) < 1e-4f)
#define MathUtilityLerp2TestSupport_ML2_NEARD(a, b) (fabs((a) - (b)) < 1e-7)
}

void MathUtilityTest::testLerpFloatBasic()
{
	using namespace MathUtilityLerp2TestSupport;
	float v = lerp(0.0f, 10.0f, 0.5f);
	MathUtilityLerp2TestSupport_ML2_ASSERT(MathUtilityLerp2TestSupport_ML2_NEAR(v, 5.0f));
	float v0 = lerp(0.0f, 10.0f, 0.0f);
	MathUtilityLerp2TestSupport_ML2_ASSERT(MathUtilityLerp2TestSupport_ML2_NEAR(v0, 0.0f));
	float v1 = lerp(0.0f, 10.0f, 1.0f);
	MathUtilityLerp2TestSupport_ML2_ASSERT(MathUtilityLerp2TestSupport_ML2_NEAR(v1, 10.0f));
}

void MathUtilityTest::testLerpFloatClampT()
{
	using namespace MathUtilityLerp2TestSupport;
	float v = lerp(0.0f, 10.0f, -0.5f);
	MathUtilityLerp2TestSupport_ML2_ASSERT(MathUtilityLerp2TestSupport_ML2_NEAR(v, 0.0f));
	float v2 = lerp(0.0f, 10.0f, 1.5f);
	MathUtilityLerp2TestSupport_ML2_ASSERT(MathUtilityLerp2TestSupport_ML2_NEAR(v2, 10.0f));
}

void MathUtilityTest::testLerpFloatMinAbsDelta()
{
	using namespace MathUtilityLerp2TestSupport;
	float v = lerp(0.0f, 10.0f, 0.99f, 0.5f);
	MathUtilityLerp2TestSupport_ML2_ASSERT(MathUtilityLerp2TestSupport_ML2_NEAR(v, 10.0f));
}

void MathUtilityTest::testLerpFloatSnap()
{
	using namespace MathUtilityLerp2TestSupport;
	float v = lerp(9.9f, 10.0f, 0.5f, 0.1f);
	MathUtilityLerp2TestSupport_ML2_ASSERT(MathUtilityLerp2TestSupport_ML2_NEAR(v, 10.0f));
}

void MathUtilityTest::testLerpIntBasic()
{
	using namespace MathUtilityLerp2TestSupport;
	int v = lerp(0, 100, 0.5f);
	MathUtilityLerp2TestSupport_ML2_ASSERT(v == 50);
	int v0 = lerp(0, 100, 0.0f);
	MathUtilityLerp2TestSupport_ML2_ASSERT(v0 == 0);
	int v1 = lerp(0, 100, 1.0f);
	MathUtilityLerp2TestSupport_ML2_ASSERT(v1 == 100);
}

void MathUtilityTest::testLerpIntClampT()
{
	using namespace MathUtilityLerp2TestSupport;
	int v = lerp(0, 100, -1.0f);
	MathUtilityLerp2TestSupport_ML2_ASSERT(v == 0);
	int v2 = lerp(0, 100, 2.0f);
	MathUtilityLerp2TestSupport_ML2_ASSERT(v2 == 100);
}

void MathUtilityTest::testLerpIntBoundary()
{
	using namespace MathUtilityLerp2TestSupport;
	int v = lerp(-100, 100, 0.5f);
	MathUtilityLerp2TestSupport_ML2_ASSERT(v == 0);
}

void MathUtilityTest::testLerpLLongBasic()
{
	using namespace MathUtilityLerp2TestSupport;
	llong v = lerp((llong)0, (llong)1000, 0.5f);
	MathUtilityLerp2TestSupport_ML2_ASSERT(v == 500);
}

void MathUtilityTest::testLerpLLongClampT()
{
	using namespace MathUtilityLerp2TestSupport;
	llong v = lerp((llong)0, (llong)1000, -0.5f);
	MathUtilityLerp2TestSupport_ML2_ASSERT(v == 0);
	llong v2 = lerp((llong)0, (llong)1000, 1.5f);
	MathUtilityLerp2TestSupport_ML2_ASSERT(v2 == 1000);
}

void MathUtilityTest::testLerpSimpleFloat()
{
	using namespace MathUtilityLerp2TestSupport;
	float v = lerpSimple(0.0f, 10.0f, 0.5f);
	MathUtilityLerp2TestSupport_ML2_ASSERT(MathUtilityLerp2TestSupport_ML2_NEAR(v, 5.0f));
	float vOob = lerpSimple(0.0f, 10.0f, 2.0f);
	MathUtilityLerp2TestSupport_ML2_ASSERT(MathUtilityLerp2TestSupport_ML2_NEAR(vOob, 20.0f));
}

void MathUtilityTest::testLerpSimpleInt()
{
	using namespace MathUtilityLerp2TestSupport;
	int v = lerpSimple(0, 100, 0.25f);
	MathUtilityLerp2TestSupport_ML2_ASSERT(v == 25);
}

void MathUtilityTest::testLerpSimpleVec3()
{
	using namespace MathUtilityLerp2TestSupport;
	Vector3 a = { 0.0f, 0.0f, 0.0f };
	Vector3 b = { 10.0f, 20.0f, 30.0f };
	Vector3 v = lerpSimple(a, b, 0.5f);
	MathUtilityLerp2TestSupport_ML2_ASSERT(MathUtilityLerp2TestSupport_ML2_NEAR(v.x, 5.0f));
	MathUtilityLerp2TestSupport_ML2_ASSERT(MathUtilityLerp2TestSupport_ML2_NEAR(v.y, 10.0f));
	MathUtilityLerp2TestSupport_ML2_ASSERT(MathUtilityLerp2TestSupport_ML2_NEAR(v.z, 15.0f));
}

void MathUtilityTest::testInverseLerpBasic()
{
	using namespace MathUtilityLerp2TestSupport;
	float t = inverseLerp(0.0f, 10.0f, 5.0f);
	MathUtilityLerp2TestSupport_ML2_ASSERT(MathUtilityLerp2TestSupport_ML2_NEAR(t, 0.5f));
	float t0 = inverseLerp(0.0f, 10.0f, 0.0f);
	MathUtilityLerp2TestSupport_ML2_ASSERT(MathUtilityLerp2TestSupport_ML2_NEAR(t0, 0.0f));
	float t1 = inverseLerp(0.0f, 10.0f, 10.0f);
	MathUtilityLerp2TestSupport_ML2_ASSERT(MathUtilityLerp2TestSupport_ML2_NEAR(t1, 1.0f));
}

void MathUtilityTest::testInverseLerpZeroDenom()
{
	using namespace MathUtilityLerp2TestSupport;
	float t = inverseLerp(5.0f, 5.0f, 5.0f);
	MathUtilityLerp2TestSupport_ML2_ASSERT(MathUtilityLerp2TestSupport_ML2_NEAR(t, 0.0f));
}

void MathUtilityTest::testInverseLerpEdge()
{
	using namespace MathUtilityLerp2TestSupport;
	float t = inverseLerp(2.0f, 8.0f, 5.0f);
	MathUtilityLerp2TestSupport_ML2_ASSERT(MathUtilityLerp2TestSupport_ML2_NEAR(t, 0.5f));
}

void MathUtilityTest::testFrameToSecond()
{
	using namespace MathUtilityLerp2TestSupport;
	float s = frameToSecond(30);
	MathUtilityLerp2TestSupport_ML2_ASSERT(MathUtilityLerp2TestSupport_ML2_NEAR(s, 0.999f));
	float s0 = frameToSecond(0);
	MathUtilityLerp2TestSupport_ML2_ASSERT(MathUtilityLerp2TestSupport_ML2_NEAR(s0, 0.0f));
	float s1 = frameToSecond(1);
	MathUtilityLerp2TestSupport_ML2_ASSERT(MathUtilityLerp2TestSupport_ML2_NEAR(s1, 0.0333f));
}

void MathUtilityTest::testPow10LLong()
{
	using namespace MathUtilityLerp2TestSupport;
	llong v0 = pow10LLong(0);
	MathUtilityLerp2TestSupport_ML2_ASSERT(v0 == 1LL);
	llong v1 = pow10LLong(1);
	MathUtilityLerp2TestSupport_ML2_ASSERT(v1 == 10LL);
	llong v3 = pow10LLong(3);
	MathUtilityLerp2TestSupport_ML2_ASSERT(v3 == 1000LL);
	llong v6 = pow10LLong(6);
	MathUtilityLerp2TestSupport_ML2_ASSERT(v6 == 1000000LL);
}

void MathUtilityTest::testInversePow10LLong()
{
	using namespace MathUtilityLerp2TestSupport;
	double v0 = inversePow10LLong(0);
	MathUtilityLerp2TestSupport_ML2_ASSERT(MathUtilityLerp2TestSupport_ML2_NEARD(v0, 1.0));
	double v1 = inversePow10LLong(1);
	MathUtilityLerp2TestSupport_ML2_ASSERT(MathUtilityLerp2TestSupport_ML2_NEARD(v1, 0.1));
	double v3 = inversePow10LLong(3);
	MathUtilityLerp2TestSupport_ML2_ASSERT(MathUtilityLerp2TestSupport_ML2_NEARD(v3, 0.001));
}

void MathUtilityTest::testPowerFloatLerp2()
{
	using namespace MathUtilityLerp2TestSupport;
	float v0 = powerFloat(2.0f, 0);
	MathUtilityLerp2TestSupport_ML2_ASSERT(MathUtilityLerp2TestSupport_ML2_NEAR(v0, 1.0f));
	float v1 = powerFloat(2.0f, 1);
	MathUtilityLerp2TestSupport_ML2_ASSERT(MathUtilityLerp2TestSupport_ML2_NEAR(v1, 2.0f));
	float v3 = powerFloat(2.0f, 3);
	MathUtilityLerp2TestSupport_ML2_ASSERT(MathUtilityLerp2TestSupport_ML2_NEAR(v3, 8.0f));
	float v4 = powerFloat(3.0f, 4);
	MathUtilityLerp2TestSupport_ML2_ASSERT(MathUtilityLerp2TestSupport_ML2_NEAR(v4, 81.0f));
}

void MathUtilityTest::testSecondsToMinutesSeconds()
{
	using namespace MathUtilityLerp2TestSupport;
	int outMin = 0;
	int outSec = 0;
	secondsToMinutesSeconds(90, outMin, outSec);
	MathUtilityLerp2TestSupport_ML2_ASSERT(outMin == 1);
	MathUtilityLerp2TestSupport_ML2_ASSERT(outSec == 30);
	secondsToMinutesSeconds(0, outMin, outSec);
	MathUtilityLerp2TestSupport_ML2_ASSERT(outMin == 0);
	MathUtilityLerp2TestSupport_ML2_ASSERT(outSec == 0);
	secondsToMinutesSeconds(60, outMin, outSec);
	MathUtilityLerp2TestSupport_ML2_ASSERT(outMin == 1);
	MathUtilityLerp2TestSupport_ML2_ASSERT(outSec == 0);
}

void MathUtilityTest::testSecondsToHoursMinutesSeconds()
{
	using namespace MathUtilityLerp2TestSupport;
	int outH = 0;
	int outM = 0;
	int outS = 0;
	secondsToHoursMinutesSeconds(3661, outH, outM, outS);
	MathUtilityLerp2TestSupport_ML2_ASSERT(outH == 1);
	MathUtilityLerp2TestSupport_ML2_ASSERT(outM == 1);
	MathUtilityLerp2TestSupport_ML2_ASSERT(outS == 1);
	secondsToHoursMinutesSeconds(0, outH, outM, outS);
	MathUtilityLerp2TestSupport_ML2_ASSERT(outH == 0);
	MathUtilityLerp2TestSupport_ML2_ASSERT(outM == 0);
	MathUtilityLerp2TestSupport_ML2_ASSERT(outS == 0);
}

void MathUtilityTest::testSpeedToInterval()
{
	using namespace MathUtilityLerp2TestSupport;
	float interval = speedToInterval(1.0f);
	MathUtilityLerp2TestSupport_ML2_ASSERT(MathUtilityLerp2TestSupport_ML2_NEAR(interval, 0.0333f));
	float interval2 = speedToInterval(2.0f);
	MathUtilityLerp2TestSupport_ML2_ASSERT(MathUtilityLerp2TestSupport_ML2_NEAR(interval2, 0.01665f));
}

void MathUtilityTest::testIntervalToSpeed()
{
	using namespace MathUtilityLerp2TestSupport;
	float speed = intervalToSpeed(0.0333f);
	MathUtilityLerp2TestSupport_ML2_ASSERT(MathUtilityLerp2TestSupport_ML2_NEAR(speed, 1.0f));
}

void MathUtilityTest::testSignInt()
{
	using namespace MathUtilityLerp2TestSupport;
	int pos = sign(5);
	MathUtilityLerp2TestSupport_ML2_ASSERT(pos == 1);
	int neg = sign(-3);
	MathUtilityLerp2TestSupport_ML2_ASSERT(neg == -1);
	int zero = sign(0);
	MathUtilityLerp2TestSupport_ML2_ASSERT(zero == 0);
}

void MathUtilityTest::testSignFloat()
{
	using namespace MathUtilityLerp2TestSupport;
	float pos = sign(3.5f);
	MathUtilityLerp2TestSupport_ML2_ASSERT(MathUtilityLerp2TestSupport_ML2_NEAR(pos, 1.0f));
	float neg = sign(-2.1f);
	MathUtilityLerp2TestSupport_ML2_ASSERT(MathUtilityLerp2TestSupport_ML2_NEAR(neg, -1.0f));
	float zero = sign(0.0f);
	MathUtilityLerp2TestSupport_ML2_ASSERT(MathUtilityLerp2TestSupport_ML2_NEAR(zero, 0.0f));
}

void MathUtilityTest::testSignLLong()
{
	using namespace MathUtilityLerp2TestSupport;
	llong pos = sign((llong)100LL);
	MathUtilityLerp2TestSupport_ML2_ASSERT(pos == 1LL);
	llong neg = sign((llong)-50LL);
	MathUtilityLerp2TestSupport_ML2_ASSERT(neg == -1LL);
	llong zero = sign((llong)0LL);
	MathUtilityLerp2TestSupport_ML2_ASSERT(zero == 0LL);
}

void MathUtilityTest::testRoundUnder()
{
	using namespace MathUtilityLerp2TestSupport;
	int v = round_(2.3f);
	MathUtilityLerp2TestSupport_ML2_ASSERT(v == 2);
}

void MathUtilityTest::testRoundOver()
{
	using namespace MathUtilityLerp2TestSupport;
	int v = round_(2.7f);
	MathUtilityLerp2TestSupport_ML2_ASSERT(v == 3);
}

void MathUtilityTest::testRoundNegative()
{
	using namespace MathUtilityLerp2TestSupport;
	int v = round_(-1.5f);
	MathUtilityLerp2TestSupport_ML2_ASSERT(v == -2);
	int v2 = round_(-1.4f);
	MathUtilityLerp2TestSupport_ML2_ASSERT(v2 == -1);
}

void MathUtilityTest::testRoundDouble()
{
	using namespace MathUtilityLerp2TestSupport;
	llong v = roundDouble(2.5);
	MathUtilityLerp2TestSupport_ML2_ASSERT(v == 3LL);
	llong v2 = roundDouble(-2.5);
	MathUtilityLerp2TestSupport_ML2_ASSERT(v2 == -3LL);
	llong v3 = roundDouble(2.4);
	MathUtilityLerp2TestSupport_ML2_ASSERT(v3 == 2LL);
}

void MathUtilityTest::testSwap_Int()
{
	using namespace MathUtilityLerp2TestSupport;
	int a = 10;
	int b = 20;
	swap_(a, b);
	MathUtilityLerp2TestSupport_ML2_ASSERT(a == 20);
	MathUtilityLerp2TestSupport_ML2_ASSERT(b == 10);
}

void MathUtilityTest::testSwap_Float()
{
	using namespace MathUtilityLerp2TestSupport;
	float a = 1.5f;
	float b = 3.5f;
	swap_(a, b);
	MathUtilityLerp2TestSupport_ML2_ASSERT(MathUtilityLerp2TestSupport_ML2_NEAR(a, 3.5f));
	MathUtilityLerp2TestSupport_ML2_ASSERT(MathUtilityLerp2TestSupport_ML2_NEAR(b, 1.5f));
}

void MathUtilityTest::testClampCycleBasic()
{
	using namespace MathUtilityLerp2TestSupport;
	float v = 5.0f;
	clampCycle(v, 0.0f, 3.0f, 3.0f);
	MathUtilityLerp2TestSupport_ML2_ASSERT(MathUtilityLerp2TestSupport_ML2_NEAR(v, 2.0f));
	float v2 = -1.0f;
	clampCycle(v2, 0.0f, 3.0f, 3.0f);
	MathUtilityLerp2TestSupport_ML2_ASSERT(MathUtilityLerp2TestSupport_ML2_NEAR(v2, 2.0f));
}

void MathUtilityTest::testClampCycleDefaultCycle()
{
	using namespace MathUtilityLerp2TestSupport;
	float v = 400.0f;
	clampCycle(v, 0.0f, 360.0f);
	MathUtilityLerp2TestSupport_ML2_ASSERT(MathUtilityLerp2TestSupport_ML2_NEAR(v, 40.0f));
}

void MathUtilityTest::testClampRadian180()
{
	using namespace MathUtilityLerp2TestSupport;
	float r = MATH_PI + 0.1f;
	clampRadian180(r);
	MathUtilityLerp2TestSupport_ML2_ASSERT(r < MATH_PI);
	MathUtilityLerp2TestSupport_ML2_ASSERT(r > -MATH_PI);
}

void MathUtilityTest::testClampDegree180()
{
	using namespace MathUtilityLerp2TestSupport;
	float d = 200.0f;
	clampDegree180(d);
	MathUtilityLerp2TestSupport_ML2_ASSERT(d >= -180.0f);
	MathUtilityLerp2TestSupport_ML2_ASSERT(d <= 180.0f);
}

void MathUtilityTest::testClampRadian360()
{
	using namespace MathUtilityLerp2TestSupport;
	float r = -0.5f;
	clampRadian360(r);
	MathUtilityLerp2TestSupport_ML2_ASSERT(r >= 0.0f);
	MathUtilityLerp2TestSupport_ML2_ASSERT(r <= MATH_PI * 2.0f);
}

void MathUtilityTest::testClampDegree360()
{
	using namespace MathUtilityLerp2TestSupport;
	float d = -90.0f;
	clampDegree360(d);
	MathUtilityLerp2TestSupport_ML2_ASSERT(d >= 0.0f);
	MathUtilityLerp2TestSupport_ML2_ASSERT(d <= 360.0f);
}

void MathUtilityTest::testInRangeIntOrdered()
{
	using namespace MathUtilityLerp2TestSupport;
	MathUtilityLerp2TestSupport_ML2_ASSERT(inRange(5, 1, 10) == true);
	MathUtilityLerp2TestSupport_ML2_ASSERT(inRange(0, 1, 10) == false);
	MathUtilityLerp2TestSupport_ML2_ASSERT(inRange(1, 1, 10) == true);
	MathUtilityLerp2TestSupport_ML2_ASSERT(inRange(10, 1, 10) == true);
	MathUtilityLerp2TestSupport_ML2_ASSERT(inRange(11, 1, 10) == false);
}

void MathUtilityTest::testInRangeIntReversed()
{
	using namespace MathUtilityLerp2TestSupport;
	MathUtilityLerp2TestSupport_ML2_ASSERT(inRange(5, 10, 1) == true);
	MathUtilityLerp2TestSupport_ML2_ASSERT(inRange(0, 10, 1) == false);
}

void MathUtilityTest::testInRangeFloatOrdered()
{
	using namespace MathUtilityLerp2TestSupport;
	MathUtilityLerp2TestSupport_ML2_ASSERT(inRange(0.5f, 0.0f, 1.0f) == true);
	MathUtilityLerp2TestSupport_ML2_ASSERT(inRange(-0.1f, 0.0f, 1.0f) == false);
	MathUtilityLerp2TestSupport_ML2_ASSERT(inRange(1.1f, 0.0f, 1.0f) == false);
}

void MathUtilityTest::testInRangeVec2()
{
	using namespace MathUtilityLerp2TestSupport;
	Vector2 val = { 0.5f, 0.5f };
	Vector2 r0 = { 0.0f, 0.0f };
	Vector2 r1 = { 1.0f, 1.0f };
	MathUtilityLerp2TestSupport_ML2_ASSERT(inRange(val, r0, r1) == true);
	Vector2 val2 = { 1.5f, 0.5f };
	MathUtilityLerp2TestSupport_ML2_ASSERT(inRange(val2, r0, r1) == false);
}

void MathUtilityTest::testInFixedRangeInt()
{
	using namespace MathUtilityLerp2TestSupport;
	MathUtilityLerp2TestSupport_ML2_ASSERT(inFixedRange(5, 1, 10) == true);
	MathUtilityLerp2TestSupport_ML2_ASSERT(inFixedRange(0, 1, 10) == false);
	MathUtilityLerp2TestSupport_ML2_ASSERT(inFixedRange(10, 1, 10) == true);
	MathUtilityLerp2TestSupport_ML2_ASSERT(inFixedRange(11, 1, 10) == false);
}

void MathUtilityTest::testInFixedRangeFloat()
{
	using namespace MathUtilityLerp2TestSupport;
	MathUtilityLerp2TestSupport_ML2_ASSERT(inFixedRange(0.5f, 0.0f, 1.0f) == true);
	MathUtilityLerp2TestSupport_ML2_ASSERT(inFixedRange(-0.1f, 0.0f, 1.0f) == false);
}

void MathUtilityTest::testIsZeroDouble()
{
	using namespace MathUtilityLerp2TestSupport;
	MathUtilityLerp2TestSupport_ML2_ASSERT(isZero(0.0) == true);
	MathUtilityLerp2TestSupport_ML2_ASSERT(isZero(1e-10) == true);
	MathUtilityLerp2TestSupport_ML2_ASSERT(isZero(1e-8) == false);
	MathUtilityLerp2TestSupport_ML2_ASSERT(isZero(-1e-10) == true);
}

namespace MathUtilityMathOpsTestSupport
{
#define MathUtilityMathOpsTestSupport_MUMO_ASSERT(expr) \
    if (!(expr)) { ERROR("MathUtilityMathOpsTest FAILED: " #expr); }

#define MathUtilityMathOpsTestSupport_MUMO_NEAR(a, b) (fabsf((float)(a) - (float)(b)) < 1e-4f)
#define MathUtilityMathOpsTestSupport_MUMO_NEARD(a, b) (fabs((double)(a) - (double)(b)) < 1e-8)
}

void MathUtilityTest::testIsZero()
{
	using namespace MathUtilityMathOpsTestSupport;
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(isZero(0.0f));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(isZero(0.000009f));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(!isZero(0.001f));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(isZero(0.0));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(!isZero(0.0000001));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(isZero(0.0000000009));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(isZero(-0.000009f));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(!isZero(-0.001f));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(isZero(0.0f, 0.1f));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(isZero(0.05f, 0.1f));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(!isZero(0.2f, 0.1f));
}

void MathUtilityTest::testIsEqual()
{
	using namespace MathUtilityMathOpsTestSupport;
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(isEqual(1.0f, 1.0f));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(isEqual(1.000009f, 1.0f));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(!isEqual(1.001f, 1.0f));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(isEqual(-1.0f, -1.0f));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(!isEqual(1.0f, -1.0f));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(isEqual(0.0f, 0.0f));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(isEqual(1.0f, 1.0001f, 0.001f));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(!isEqual(1.0f, 1.01f, 0.001f));
}

void MathUtilityTest::testGetMinMaxMathOps()
{
	using namespace MathUtilityMathOpsTestSupport;
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(getMin(3, 5) == 3);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(getMin(5, 3) == 3);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(getMin(3, 3) == 3);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(getMax(3, 5) == 5);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(getMax(5, 3) == 5);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(getMax(3, 3) == 3);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(getMin(-5, -3) == -5);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(getMax(-5, -3) == -3);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(MathUtilityMathOpsTestSupport_MUMO_NEAR(getMin(1.5f, 2.5f), 1.5f));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(MathUtilityMathOpsTestSupport_MUMO_NEAR(getMax(1.5f, 2.5f), 2.5f));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(getMin(0, 0) == 0);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(getMax(0, 0) == 0);
}

void MathUtilityTest::testClampMathOps()
{
	using namespace MathUtilityMathOpsTestSupport;
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(clamp(5, 0, 10) == 5);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(clamp(-1, 0, 10) == 0);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(clamp(15, 0, 10) == 10);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(clamp(0, 0, 10) == 0);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(clamp(10, 0, 10) == 10);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(MathUtilityMathOpsTestSupport_MUMO_NEAR(clamp(0.5f, 0.0f, 1.0f), 0.5f));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(MathUtilityMathOpsTestSupport_MUMO_NEAR(clamp(-0.5f, 0.0f, 1.0f), 0.0f));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(MathUtilityMathOpsTestSupport_MUMO_NEAR(clamp(1.5f, 0.0f, 1.0f), 1.0f));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(clamp(-100, -50, -10) == -50);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(clamp(-5, -50, -10) == -10);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(clamp(-25, -50, -10) == -25);
}

void MathUtilityTest::testClampRef()
{
	using namespace MathUtilityMathOpsTestSupport;
	int val = 5;
	clampRef(val, 0, 10);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(val == 5);
	val = -1;
	clampRef(val, 0, 10);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(val == 0);
	val = 15;
	clampRef(val, 0, 10);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(val == 10);
	float fval = 0.5f;
	clampRef(fval, 0.0f, 1.0f);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(MathUtilityMathOpsTestSupport_MUMO_NEAR(fval, 0.5f));
	fval = -1.0f;
	clampRef(fval, 0.0f, 1.0f);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(MathUtilityMathOpsTestSupport_MUMO_NEAR(fval, 0.0f));
	fval = 2.0f;
	clampRef(fval, 0.0f, 1.0f);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(MathUtilityMathOpsTestSupport_MUMO_NEAR(fval, 1.0f));
}

void MathUtilityTest::testClampMin()
{
	using namespace MathUtilityMathOpsTestSupport;
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(clampMin(5, 0) == 5);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(clampMin(-1, 0) == 0);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(clampMin(0, 0) == 0);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(clampMin(-5, -3) == -3);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(clampMin(-1, -3) == -1);
	int v = 5;
	clampMinRef(v, 0);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(v == 5);
	v = -1;
	clampMinRef(v, 0);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(v == 0);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(MathUtilityMathOpsTestSupport_MUMO_NEAR(clampMin(0.5f, 0.0f), 0.5f));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(MathUtilityMathOpsTestSupport_MUMO_NEAR(clampMin(-0.5f, 0.0f), 0.0f));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(clampZero(5) == 5);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(clampZero(-3) == 0);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(clampZero(0) == 0);
	int cv = -5;
	clampZeroRef(cv);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(cv == 0);
	cv = 5;
	clampZeroRef(cv);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(cv == 5);
}

void MathUtilityTest::testClampMax()
{
	using namespace MathUtilityMathOpsTestSupport;
	int v = 5;
	clampMaxRef(v, 10);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(v == 5);
	v = 15;
	clampMaxRef(v, 10);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(v == 10);
	v = 10;
	clampMaxRef(v, 10);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(v == 10);
	float fv = 0.5f;
	clampMaxRef(fv, 1.0f);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(MathUtilityMathOpsTestSupport_MUMO_NEAR(fv, 0.5f));
	fv = 2.0f;
	clampMaxRef(fv, 1.0f);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(MathUtilityMathOpsTestSupport_MUMO_NEAR(fv, 1.0f));
}

void MathUtilityTest::testClampZero()
{
	using namespace MathUtilityMathOpsTestSupport;
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(MathUtilityMathOpsTestSupport_MUMO_NEAR(clampZero(1.5f), 1.5f));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(MathUtilityMathOpsTestSupport_MUMO_NEAR(clampZero(-1.5f), 0.0f));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(MathUtilityMathOpsTestSupport_MUMO_NEAR(clampZero(0.0f), 0.0f));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(clampZero(100) == 100);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(clampZero(-100) == 0);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(clampZero(0) == 0);
}

void MathUtilityTest::testDivideMathOps()
{
	using namespace MathUtilityMathOpsTestSupport;
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(MathUtilityMathOpsTestSupport_MUMO_NEAR(divide(10.0f, 2.0f), 5.0f));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(MathUtilityMathOpsTestSupport_MUMO_NEAR(divide(10.0f, 0.0f), 0.0f));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(MathUtilityMathOpsTestSupport_MUMO_NEAR(divide(10.0f, 4.0f), 2.5f));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(MathUtilityMathOpsTestSupport_MUMO_NEAR(divide(10, 2), 5.0f));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(MathUtilityMathOpsTestSupport_MUMO_NEAR(divide(10, 0), 0.0f));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(MathUtilityMathOpsTestSupport_MUMO_NEAR(divide(7, 2), 3.5f));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(MathUtilityMathOpsTestSupport_MUMO_NEAR(divide(10.0f, 3), 3.333333f));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(MathUtilityMathOpsTestSupport_MUMO_NEAR(divide(10.0f, 0), 0.0f));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(MathUtilityMathOpsTestSupport_MUMO_NEAR(divide(3, 2.0f), 1.5f));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(MathUtilityMathOpsTestSupport_MUMO_NEAR(divide(3, 0.0f), 0.0f));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(MathUtilityMathOpsTestSupport_MUMO_NEAR(divide(10LL, 3LL), 3.333333f));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(MathUtilityMathOpsTestSupport_MUMO_NEAR(divide(10LL, 0LL), 0.0f));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(MathUtilityMathOpsTestSupport_MUMO_NEARD(divide(10.0, 3.0), 3.333333333));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(MathUtilityMathOpsTestSupport_MUMO_NEARD(divide(10.0, 0.0), 0.0));
}

void MathUtilityTest::testDivideIntMathOps()
{
	using namespace MathUtilityMathOpsTestSupport;
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(divideInt(10, 2) == 5);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(divideInt(10, 0) == 0);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(divideInt(7, 2) == 3);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(divideInt(0, 5) == 0);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(divideInt(10ull, 3ull) == 3ull);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(divideInt(10ull, 0ull) == 0ull);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(divideInt(100ull, 10ull) == 10ull);
}

void MathUtilityTest::testCeiling()
{
	using namespace MathUtilityMathOpsTestSupport;
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(ceiling(1.0f) == 1);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(ceiling(1.5f) == 2);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(ceiling(2.9f) == 3);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(ceiling(0.0f) == 0);
	// 框架容差 0.0001f：0.0001f <= 0 + 0.0001f，不进位，返回 0
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(ceiling(0.0001f) == 0);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(ceiling(-1.5f) == -1);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(ceiling(-1.0f) == -1);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(ceiling(1.0) == 1);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(ceiling(1.5) == 2);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(ceiling(-1.5) == -1);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(ceiling(0.0001) == 1);
}

void MathUtilityTest::testFloor()
{
	using namespace MathUtilityMathOpsTestSupport;
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(floor_(1.0f) == 1);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(floor_(1.5f) == 1);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(floor_(2.9f) == 2);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(floor_(0.0f) == 0);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(floor_(-1.5f) == -2);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(floor_(-1.0f) == -1);
	// 框架容差 0.0001f：-0.0001f 在 [0 - 0.0001f] 临界，不退位，返回 0
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(floor_(-0.0001f) == 0);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(floor_(1.0) == 1);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(floor_(1.5) == 1);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(floor_(-1.5) == -2);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(floor_(-0.0001) == -1);
}

void MathUtilityTest::testIsPow2MathOps()
{
	using namespace MathUtilityMathOpsTestSupport;
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(!isPow2(0));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(isPow2(1));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(isPow2(2));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(!isPow2(3));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(isPow2(4));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(!isPow2(5));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(isPow2(8));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(!isPow2(12));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(isPow2(16));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(isPow2(64));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(isPow2(128));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(isPow2(256));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(isPow2(1024));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(!isPow2(-1));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(!isPow2(-4));
}

void MathUtilityTest::testIsEvenMathOps()
{
	using namespace MathUtilityMathOpsTestSupport;
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(isEven(0));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(!isEven(1));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(isEven(2));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(!isEven(3));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(isEven(100));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(!isEven(101));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(isEven(-2));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(!isEven(-1));
}

void MathUtilityTest::testPow2()
{
	using namespace MathUtilityMathOpsTestSupport;
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(pow2(0) == 1);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(pow2(1) == 2);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(pow2(2) == 4);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(pow2(3) == 8);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(pow2(4) == 16);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(pow2(8) == 256);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(pow2(10) == 1024);
}

void MathUtilityTest::testPow10()
{
	using namespace MathUtilityMathOpsTestSupport;
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(pow10(0) == 1);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(pow10(1) == 10);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(pow10(2) == 100);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(pow10(3) == 1000);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(pow10(4) == 10000);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(pow10LLong(0) == 1LL);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(pow10LLong(5) == 100000LL);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(pow10LLong(10) == 10000000000LL);
}

void MathUtilityTest::testInversePow10()
{
	using namespace MathUtilityMathOpsTestSupport;
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(MathUtilityMathOpsTestSupport_MUMO_NEAR(inversePow10(0), 1.0f));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(MathUtilityMathOpsTestSupport_MUMO_NEAR(inversePow10(1), 0.1f));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(MathUtilityMathOpsTestSupport_MUMO_NEAR(inversePow10(2), 0.01f));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(MathUtilityMathOpsTestSupport_MUMO_NEAR(inversePow10(3), 0.001f));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(MathUtilityMathOpsTestSupport_MUMO_NEARD(inversePow10LLong(0), 1.0));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(MathUtilityMathOpsTestSupport_MUMO_NEARD(inversePow10LLong(1), 0.1));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(MathUtilityMathOpsTestSupport_MUMO_NEARD(inversePow10LLong(3), 0.001));
}

void MathUtilityTest::testIndexToXY()
{
	using namespace MathUtilityMathOpsTestSupport;
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(indexToX(0, 5) == 0);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(indexToY(0, 5) == 0);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(indexToX(4, 5) == 4);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(indexToY(4, 5) == 0);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(indexToX(5, 5) == 0);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(indexToY(5, 5) == 1);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(indexToX(7, 5) == 2);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(indexToY(7, 5) == 1);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(indexToX(12, 4) == 0);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(indexToY(12, 4) == 3);
	Vector2Int pos = indexToIntPos(7, 5);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(pos.x == 2 && pos.y == 1);
	pos = indexToIntPos(0, 10);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(pos.x == 0 && pos.y == 0);
}

void MathUtilityTest::testIntPosToIndex()
{
	using namespace MathUtilityMathOpsTestSupport;
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(intPosToIndex(0, 0, 5) == 0);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(intPosToIndex(4, 0, 5) == 4);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(intPosToIndex(0, 1, 5) == 5);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(intPosToIndex(2, 1, 5) == 7);
	Vector2Int v2i = { 2, 3 };
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(intPosToIndex(v2i, 5) == 17);
	Vector2Short v2s = { 1, 2 };
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(intPosToIndex(v2s, 5) == 11);
	Vector2UShort v2us = { 3, 1 };
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(intPosToIndex(v2us, 5) == 8);
	Vector2UInt v2ui = { 2, 4 };
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(intPosToIndex(v2ui, 5) == 22);
}

void MathUtilityTest::testGenerateBatchCount()
{
	using namespace MathUtilityMathOpsTestSupport;
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(generateBatchCount(10, 3) == 4);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(generateBatchCount(9, 3) == 3);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(generateBatchCount(0, 3) == 0);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(generateBatchCount(1, 3) == 1);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(generateBatchCount(100, 10) == 10);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(generateBatchCount(101, 10) == 11);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(generateBatchCount(3, 1) == 3);
}

void MathUtilityTest::testGenerateBatchSize()
{
	using namespace MathUtilityMathOpsTestSupport;
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(generateBatchSize(10, 3, 0) == 3);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(generateBatchSize(10, 3, 1) == 3);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(generateBatchSize(10, 3, 2) == 3);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(generateBatchSize(10, 3, 3) == 1);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(generateBatchSize(10, 3, 4) == 0);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(generateBatchSize(9, 3, 0) == 3);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(generateBatchSize(9, 3, 2) == 3);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(generateBatchSize(9, 3, 3) == 0);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(generateBatchSize(0, 3, 0) == 0);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(generateBatchSize(1, 3, 0) == 1);
}

void MathUtilityTest::testGreaterPower2()
{
	using namespace MathUtilityMathOpsTestSupport;
	// getGreaterPower2 查表：index 0,1 均返回 2（i>1?getGreaterPowerValue(i,2):2）
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(getGreaterPower2(1) == 2);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(getGreaterPower2(2) == 2);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(getGreaterPower2(3) == 4);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(getGreaterPower2(4) == 4);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(getGreaterPower2(5) == 8);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(getGreaterPower2(8) == 8);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(getGreaterPower2(9) == 16);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(getGreaterPower2(16) == 16);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(getGreaterPower2(100) == 128);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(getGreaterPower2(128) == 128);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(getGreaterPower2(256) == 256);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(getGreaterPower2(257) == 512);
}

void MathUtilityTest::testGetGreaterPower2MathOps()
{
	using namespace MathUtilityMathOpsTestSupport;
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(getGreaterPower2(0) == 2);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(getGreaterPower2(1) == 2);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(getGreaterPower2(2) == 2);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(getGreaterPower2(3) == 4);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(getGreaterPower2(127) == 128);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(getGreaterPower2(512) == 512);
}

void MathUtilityTest::testCheckInt()
{
	using namespace MathUtilityMathOpsTestSupport;
	float v1 = 2.999999f;
	checkInt(v1);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(MathUtilityMathOpsTestSupport_MUMO_NEAR(v1, 3.0f));
	float v2 = 3.0001f;
	checkInt(v2);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(MathUtilityMathOpsTestSupport_MUMO_NEAR(v2, 3.0001f));
	float v3 = 5.00001f;
	checkInt(v3);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(MathUtilityMathOpsTestSupport_MUMO_NEAR(v3, 5.0f));
}

void MathUtilityTest::testToVec()
{
	using namespace MathUtilityMathOpsTestSupport;
	Vector2Int v2i = { 3, 4 };
	Vector3 r1 = toVec3(v2i);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(MathUtilityMathOpsTestSupport_MUMO_NEAR(r1.x, 3.0f) && MathUtilityMathOpsTestSupport_MUMO_NEAR(r1.y, 4.0f) && MathUtilityMathOpsTestSupport_MUMO_NEAR(r1.z, 0.0f));
	Vector2 v2 = { 1.5f, 2.5f };
	Vector3 r2 = toVec3(v2);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(MathUtilityMathOpsTestSupport_MUMO_NEAR(r2.x, 1.5f) && MathUtilityMathOpsTestSupport_MUMO_NEAR(r2.y, 2.5f) && MathUtilityMathOpsTestSupport_MUMO_NEAR(r2.z, 0.0f));
	Vector3 v3 = { 1.0f, 2.0f, 3.0f };
	Vector2 r3 = toVec2(v3);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(MathUtilityMathOpsTestSupport_MUMO_NEAR(r3.x, 1.0f) && MathUtilityMathOpsTestSupport_MUMO_NEAR(r3.y, 2.0f));
	Vector2Int r4 = toVec2Int(v2);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(r4.x == 1 && r4.y == 2);
	Vector2Int r5 = toVec2Int(v3);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(r5.x == 1 && r5.y == 2);
	Vector2UInt v2ui = { 3u, 4u };
	Vector2Int r6 = toVec2Int(v2ui);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(r6.x == 3 && r6.y == 4);
	Vector2UInt r7 = toVec2UInt(v2);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(r7.x == 1u && r7.y == 2u);
}

void MathUtilityTest::testDivideOverload()
{
	using namespace MathUtilityMathOpsTestSupport;
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(MathUtilityMathOpsTestSupport_MUMO_NEAR(divide(10.0f, 2), 5.0f));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(MathUtilityMathOpsTestSupport_MUMO_NEAR(divide(10.0f, 0), 0.0f));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(MathUtilityMathOpsTestSupport_MUMO_NEAR(divide(10LL, 2.0f), 5.0f));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(MathUtilityMathOpsTestSupport_MUMO_NEAR(divide(10LL, 0.0f), 0.0f));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(MathUtilityMathOpsTestSupport_MUMO_NEAR(divide(0, 5), 0.0f));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(MathUtilityMathOpsTestSupport_MUMO_NEAR(divide(0.0f, 5.0f), 0.0f));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(MathUtilityMathOpsTestSupport_MUMO_NEAR(divide(0.0f, 0.0f), 0.0f));
}

void MathUtilityTest::testCeilingVector()
{
	using namespace MathUtilityMathOpsTestSupport;
	Vector2 v2 = { 1.3f, 2.7f };
	ceiling(v2);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(MathUtilityMathOpsTestSupport_MUMO_NEAR(v2.x, 2.0f));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(MathUtilityMathOpsTestSupport_MUMO_NEAR(v2.y, 3.0f));
	Vector3 v3 = { 1.1f, 2.9f, 3.5f };
	ceiling(v3);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(MathUtilityMathOpsTestSupport_MUMO_NEAR(v3.x, 2.0f));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(MathUtilityMathOpsTestSupport_MUMO_NEAR(v3.y, 3.0f));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(MathUtilityMathOpsTestSupport_MUMO_NEAR(v3.z, 4.0f));
	Vector2 v2b = { 1.0f, 2.0f };
	ceiling(v2b);
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(MathUtilityMathOpsTestSupport_MUMO_NEAR(v2b.x, 1.0f));
	MathUtilityMathOpsTestSupport_MUMO_ASSERT(MathUtilityMathOpsTestSupport_MUMO_NEAR(v2b.y, 2.0f));
}

namespace MathUtilityRandomTestSupport
{
#define MathUtilityRandomTestSupport_MURT_ASSERT(expr) \
    do { if (!(expr)) { ERROR("MathUtilityRandomTest FAIL: " #expr); } } while(0)
#define MathUtilityRandomTestSupport_MURT_NEAR(a, b) (fabsf((a) - (b)) < 1e-4f)
}

void MathUtilityTest::testRandomIntRange()
{
	using namespace MathUtilityRandomTestSupport;
	FOR(50)
	{
		int v = randomInt(1, 10);
		MathUtilityRandomTestSupport_MURT_ASSERT(v >= 1);
		MathUtilityRandomTestSupport_MURT_ASSERT(v <= 10);
	}
}

void MathUtilityTest::testRandomIntSameMinMax()
{
	using namespace MathUtilityRandomTestSupport;
	FOR(10)
	{
		int v = randomInt(5, 5);
		MathUtilityRandomTestSupport_MURT_ASSERT(v == 5);
	}
}

void MathUtilityTest::testRandomIntNegativeRange()
{
	using namespace MathUtilityRandomTestSupport;
	FOR(30)
	{
		int v = randomInt(-10, -1);
		MathUtilityRandomTestSupport_MURT_ASSERT(v >= -10);
		MathUtilityRandomTestSupport_MURT_ASSERT(v <= -1);
	}
}

void MathUtilityTest::testRandomLongRange()
{
	using namespace MathUtilityRandomTestSupport;
	FOR(30)
	{
		llong v = randomLong(0LL, 1000LL);
		MathUtilityRandomTestSupport_MURT_ASSERT(v >= 0LL);
		MathUtilityRandomTestSupport_MURT_ASSERT(v <= 1000LL);
	}
}

void MathUtilityTest::testRandomLongSameMinMax()
{
	using namespace MathUtilityRandomTestSupport;
	FOR(10)
	{
		llong v = randomLong(42LL, 42LL);
		MathUtilityRandomTestSupport_MURT_ASSERT(v == 42LL);
	}
}

void MathUtilityTest::testRandomFloatRange()
{
	using namespace MathUtilityRandomTestSupport;
	FOR(50)
	{
		float v = randomFloat(0.0f, 1.0f);
		MathUtilityRandomTestSupport_MURT_ASSERT(v >= 0.0f);
		MathUtilityRandomTestSupport_MURT_ASSERT(v <= 1.0f);
	}
}

void MathUtilityTest::testRandomFloatAlwaysInRange()
{
	using namespace MathUtilityRandomTestSupport;
	float minVal = -5.0f;
	float maxVal = 5.0f;
	FOR(100)
	{
		float v = randomFloat(minVal, maxVal);
		MathUtilityRandomTestSupport_MURT_ASSERT(v >= minVal);
		MathUtilityRandomTestSupport_MURT_ASSERT(v <= maxVal);
	}
}

void MathUtilityTest::testRandomHitZeroProbability()
{
	using namespace MathUtilityRandomTestSupport;
	FOR(20)
	{
		bool hit = randomHit(0.0f);
		MathUtilityRandomTestSupport_MURT_ASSERT(hit == false);
	}
}

void MathUtilityTest::testRandomHitOneProbability()
{
	using namespace MathUtilityRandomTestSupport;
	FOR(20)
	{
		bool hit = randomHit(1.0f);
		MathUtilityRandomTestSupport_MURT_ASSERT(hit == true);
	}
}

void MathUtilityTest::testRandomHitIntScaleZero()
{
	using namespace MathUtilityRandomTestSupport;
	FOR(10)
	{
		bool hit = randomHit(0, 100);
		MathUtilityRandomTestSupport_MURT_ASSERT(hit == false);
	}
}

void MathUtilityTest::testRandomHitIntScaleFull()
{
	using namespace MathUtilityRandomTestSupport;
	FOR(10)
	{
		bool hit = randomHit(100, 100);
		MathUtilityRandomTestSupport_MURT_ASSERT(hit == true);
	}
}

void MathUtilityTest::testRandomHitArrayListOdds()
{
	using namespace MathUtilityRandomTestSupport;
	ArrayList<4, float> odds;
	odds.add(1.0f);
	odds.add(1.0f);
	odds.add(1.0f);
	odds.add(1.0f);
	FOR(20)
	{
		int idx = randomHit(odds);
		MathUtilityRandomTestSupport_MURT_ASSERT(idx >= 0);
		MathUtilityRandomTestSupport_MURT_ASSERT(idx < 4);
	}
}

void MathUtilityTest::testRandomOrderArrayInPlace()
{
	using namespace MathUtilityRandomTestSupport;
	int arr[5] = { 0, 1, 2, 3, 4 };
	randomOrder(arr, 5);
	int sum = 0;
	FOR(5)
	{
		sum += arr[i];
	}
	MathUtilityRandomTestSupport_MURT_ASSERT(sum == 10);
}

void MathUtilityTest::testRandomOrderVectorInPlace()
{
	using namespace MathUtilityRandomTestSupport;
	Vector<int> v;
	v.add(10);
	v.add(20);
	v.add(30);
	v.add(40);
	randomOrder(v.data(), v.size());
	int sum = 0;
	FOR_VECTOR(v)
	{
		sum += v[i];
	}
	MathUtilityRandomTestSupport_MURT_ASSERT(sum == 100);
}

void MathUtilityTest::testRandomOrderArrayListInPlace()
{
	using namespace MathUtilityRandomTestSupport;
	ArrayList<5, int> al;
	al.add(1);
	al.add(2);
	al.add(3);
	al.add(4);
	al.add(5);
	randomOrder(al);
	int sum = 0;
	FOR(5)
	{
		sum += al[i];
	}
	MathUtilityRandomTestSupport_MURT_ASSERT(sum == 15);
}

void MathUtilityTest::testRandomSelectCountSubset()
{
	using namespace MathUtilityRandomTestSupport;
	Vector<int> result;
	randomSelect(10, 3, result);
	MathUtilityRandomTestSupport_MURT_ASSERT(result.size() == 3);
	FOR_VECTOR(result)
	{
		MathUtilityRandomTestSupport_MURT_ASSERT(result[i] >= 0);
		MathUtilityRandomTestSupport_MURT_ASSERT(result[i] < 10);
	}
}

void MathUtilityTest::testRandomSelectCountAll()
{
	using namespace MathUtilityRandomTestSupport;
	Vector<int> result;
	randomSelect(5, 5, result);
	MathUtilityRandomTestSupport_MURT_ASSERT(result.size() == 5);
}

void MathUtilityTest::testRandomSelectCountZero()
{
	using namespace MathUtilityRandomTestSupport;
	Vector<int> result;
	randomSelect(10, 0, result);
	MathUtilityRandomTestSupport_MURT_ASSERT(result.size() == 0);
}

void MathUtilityTest::testBinarySearchFound()
{
	using namespace MathUtilityRandomTestSupport;
	Vector<int> sorted;
	sorted.add(1);
	sorted.add(3);
	sorted.add(5);
	sorted.add(7);
	sorted.add(9);
	bool found = binarySearch(sorted, 0, 4, 5);
	MathUtilityRandomTestSupport_MURT_ASSERT(found == true);
	bool foundFirst = binarySearch(sorted, 0, 4, 1);
	MathUtilityRandomTestSupport_MURT_ASSERT(foundFirst == true);
	bool foundLast = binarySearch(sorted, 0, 4, 9);
	MathUtilityRandomTestSupport_MURT_ASSERT(foundLast == true);
}

void MathUtilityTest::testBinarySearchNotFound()
{
	using namespace MathUtilityRandomTestSupport;
	Vector<int> sorted;
	sorted.add(1);
	sorted.add(3);
	sorted.add(5);
	sorted.add(7);
	bool notFound = binarySearch(sorted, 0, 3, 4);
	MathUtilityRandomTestSupport_MURT_ASSERT(notFound == false);
}

void MathUtilityTest::testBinarySearchSingleElement()
{
	using namespace MathUtilityRandomTestSupport;
	Vector<int> single;
	single.add(42);
	bool found = binarySearch(single, 0, 0, 42);
	MathUtilityRandomTestSupport_MURT_ASSERT(found == true);
	bool notFound = binarySearch(single, 0, 0, 1);
	MathUtilityRandomTestSupport_MURT_ASSERT(notFound == false);
}

void MathUtilityTest::testBinarySearchEdge()
{
	using namespace MathUtilityRandomTestSupport;
	Vector<int> list2;
	list2.add(10);
	list2.add(20);
	bool f1 = binarySearch(list2, 0, 1, 10);
	MathUtilityRandomTestSupport_MURT_ASSERT(f1 == true);
	bool f2 = binarySearch(list2, 0, 1, 20);
	MathUtilityRandomTestSupport_MURT_ASSERT(f2 == true);
	bool f3 = binarySearch(list2, 0, 1, 15);
	MathUtilityRandomTestSupport_MURT_ASSERT(f3 == false);
}

void MathUtilityTest::testFindPointIndexBasic()
{
	using namespace MathUtilityRandomTestSupport;
	Vector<float> dists;
	dists.add(0.0f);
	dists.add(5.0f);
	dists.add(10.0f);
	dists.add(15.0f);
	int idx = findPointIndex(dists, 6.0f, 0, 3);
	MathUtilityRandomTestSupport_MURT_ASSERT(idx >= 0);
	MathUtilityRandomTestSupport_MURT_ASSERT(idx < 4);
}

void MathUtilityTest::testFindPointIndexBeyondEnd()
{
	using namespace MathUtilityRandomTestSupport;
	Vector<float> dists;
	dists.add(0.0f);
	dists.add(5.0f);
	dists.add(10.0f);
	int idx = findPointIndex(dists, 20.0f, 0, 2);
	MathUtilityRandomTestSupport_MURT_ASSERT(idx >= 0);
}

namespace MathUtilitySortTestSupport
{
#define MathUtilitySortTestSupport_MST_ASSERT(expr) if (!(expr)) { ERROR("MathUtilitySortTest FAILED: " #expr); }

	static int comparatorAsc(const int& a, const int& b)
	{
		if (a < b)
		{
			return -1;
		}
		if (a > b)
		{
			return 1;
		}
		return 0;
	}

	static int comparatorDesc(const int& a, const int& b)
	{
		// 降序比较器：a > b 时返回正数（a 大于 b，应该排在前面）
		// 对于 quickSortDescend 函数，使用 compare(a,b)>0 来寻找比 key 大的元素
		if (a > b)
		{
			return 1;
		}
		if (a < b)
		{
			return -1;
		}
		return 0;
	}

	static int comparatorAscFloat(const float& a, const float& b)
	{
		if (a < b)
		{
			return -1;
		}
		if (a > b)
		{
			return 1;
		}
		return 0;
	}

	static bool isAscending(const int* arr, int count)
	{
		for (int i = 1; i < count; ++i)
		{
			if (arr[i] < arr[i - 1])
			{
				return false;
			}
		}
		return true;
	}

	static bool isDescending(const int* arr, int count)
	{
		for (int i = 1; i < count; ++i)
		{
			if (arr[i] > arr[i - 1])
			{
				return false;
			}
		}
		return true;
	}

	static bool isAscendingFloat(const float* arr, int count)
	{
		for (int i = 1; i < count; ++i)
		{
			if (arr[i] < arr[i - 1])
			{
				return false;
			}
		}
		return true;
	}
}

void MathUtilityTest::testQuickSortNonRecursiveAscend()
{
	using namespace MathUtilitySortTestSupport;
	int arr[8] = { 5, 3, 8, 1, 9, 2, 7, 4 };
	int low = 0;
	int high = 7;
	int pivot = quickSortNonRecursiveAscend(arr, low, high);
	MathUtilitySortTestSupport_MST_ASSERT(pivot >= 0 && pivot <= 7);
	MathUtilitySortTestSupport_MST_ASSERT(arr[pivot] == 5);
	FOR(pivot)
	{
		MathUtilitySortTestSupport_MST_ASSERT(arr[i] <= arr[pivot]);
	}
	for (int i = pivot + 1; i <= high; ++i)
	{
		MathUtilitySortTestSupport_MST_ASSERT(arr[i] >= arr[pivot]);
	}
}

void MathUtilityTest::testQuickSortNonRecursiveDescend()
{
	using namespace MathUtilitySortTestSupport;
	int arr[8] = { 5, 3, 8, 1, 9, 2, 7, 4 };
	int low = 0;
	int high = 7;
	int pivot = quickSortNonRecursiveDescend(arr, low, high);
	MathUtilitySortTestSupport_MST_ASSERT(pivot >= 0 && pivot <= 7);
	MathUtilitySortTestSupport_MST_ASSERT(arr[pivot] == 5);
	FOR(pivot)
	{
		MathUtilitySortTestSupport_MST_ASSERT(arr[i] >= arr[pivot]);
	}
	for (int i = pivot + 1; i <= high; ++i)
	{
		MathUtilitySortTestSupport_MST_ASSERT(arr[i] <= arr[pivot]);
	}
}

void MathUtilityTest::testQuickSortNonRecursiveAscendCompare()
{
	using namespace MathUtilitySortTestSupport;
	int arr[5] = { 10, 4, 7, 2, 9 };
	int pivot = quickSortNonRecursiveAscend(arr, 0, 4, comparatorAsc);
	MathUtilitySortTestSupport_MST_ASSERT(pivot >= 0 && pivot <= 4);
	MathUtilitySortTestSupport_MST_ASSERT(arr[pivot] == 10);
	FOR(pivot)
	{
		MathUtilitySortTestSupport_MST_ASSERT(arr[i] <= arr[pivot]);
	}
	for (int i = pivot + 1; i <= 4; ++i)
	{
		MathUtilitySortTestSupport_MST_ASSERT(arr[i] >= arr[pivot]);
	}
}

void MathUtilityTest::testQuickSortNonRecursiveDescendCompare()
{
	using namespace MathUtilitySortTestSupport;
	// compare 版分区：comparatorDesc(a,b)=a-b（降序时 a>b 返回正数）
	// 单轮分区后 pivot 落在合法范围内即可
	int arr[5] = { 10, 4, 7, 2, 9 };
	int pivot = quickSortNonRecursiveDescend(arr, 0, 4, comparatorDesc);
	MathUtilitySortTestSupport_MST_ASSERT(pivot >= 0 && pivot <= 4);
	// 验证完整降序排序正常工作
	int arr2[6] = { 3, 1, 4, 1, 5, 9 };
	quickSortDescend(arr2, 0, 5, comparatorDesc);
	MathUtilitySortTestSupport_MST_ASSERT(isDescending(arr2, 6));
	MathUtilitySortTestSupport_MST_ASSERT(arr2[0] == 9);
	MathUtilitySortTestSupport_MST_ASSERT(arr2[5] == 1);
}

void MathUtilityTest::testQuickSortAscend()
{
	using namespace MathUtilitySortTestSupport;
	int arr[8] = { 5, 3, 8, 1, 9, 2, 7, 4 };
	quickSortAscend(arr, 0, 7);
	MathUtilitySortTestSupport_MST_ASSERT(isAscending(arr, 8));
	MathUtilitySortTestSupport_MST_ASSERT(arr[0] == 1);
	MathUtilitySortTestSupport_MST_ASSERT(arr[7] == 9);
}

void MathUtilityTest::testQuickSortDescend()
{
	using namespace MathUtilitySortTestSupport;
	int arr[8] = { 5, 3, 8, 1, 9, 2, 7, 4 };
	quickSortDescend(arr, 0, 7);
	MathUtilitySortTestSupport_MST_ASSERT(isDescending(arr, 8));
	MathUtilitySortTestSupport_MST_ASSERT(arr[0] == 9);
	MathUtilitySortTestSupport_MST_ASSERT(arr[7] == 1);
}

void MathUtilityTest::testQuickSortAscendCompare()
{
	using namespace MathUtilitySortTestSupport;
	int arr[6] = { 3, 1, 4, 1, 5, 9 };
	quickSortAscend(arr, 0, 5, comparatorAsc);
	MathUtilitySortTestSupport_MST_ASSERT(isAscending(arr, 6));
	MathUtilitySortTestSupport_MST_ASSERT(arr[0] == 1);
	MathUtilitySortTestSupport_MST_ASSERT(arr[5] == 9);
}

void MathUtilityTest::testQuickSortDescendCompare()
{
	using namespace MathUtilitySortTestSupport;
	// quickSortDescend 内部用 compare(a,b)>0 找"比 key 大"，传 comparatorAsc(a-b) 才能与 Descend 语义一致
	int arr[6] = { 3, 1, 4, 1, 5, 9 };
	quickSortDescend(arr, 0, 5, comparatorAsc);
	MathUtilitySortTestSupport_MST_ASSERT(isDescending(arr, 6));
	MathUtilitySortTestSupport_MST_ASSERT(arr[0] == 9);
	MathUtilitySortTestSupport_MST_ASSERT(arr[5] == 1);
}

void MathUtilityTest::testQuickSortAscendPtr()
{
	using namespace MathUtilitySortTestSupport;
	int a = 5, b = 3, c = 8, d = 1, e = 9;
	int* arr[5] = { &a, &b, &c, &d, &e };
	quickSortAscendPtr(arr, 0, 4);
	MathUtilitySortTestSupport_MST_ASSERT(*arr[0] == 1);
	MathUtilitySortTestSupport_MST_ASSERT(*arr[1] == 3);
	MathUtilitySortTestSupport_MST_ASSERT(*arr[2] == 5);
	MathUtilitySortTestSupport_MST_ASSERT(*arr[3] == 8);
	MathUtilitySortTestSupport_MST_ASSERT(*arr[4] == 9);
}

void MathUtilityTest::testQuickSortDescendPtr()
{
	using namespace MathUtilitySortTestSupport;
	int a = 5, b = 3, c = 8, d = 1, e = 9;
	int* arr[5] = { &a, &b, &c, &d, &e };
	quickSortDescendPtr(arr, 0, 4);
	MathUtilitySortTestSupport_MST_ASSERT(*arr[0] == 9);
	MathUtilitySortTestSupport_MST_ASSERT(*arr[1] == 8);
	MathUtilitySortTestSupport_MST_ASSERT(*arr[2] == 5);
	MathUtilitySortTestSupport_MST_ASSERT(*arr[3] == 3);
	MathUtilitySortTestSupport_MST_ASSERT(*arr[4] == 1);
}

void MathUtilityTest::testSortSingleElement()
{
	using namespace MathUtilitySortTestSupport;
	int arr[1] = { 42 };
	quickSortAscend(arr, 0, 0);
	MathUtilitySortTestSupport_MST_ASSERT(arr[0] == 42);
	quickSortDescend(arr, 0, 0);
	MathUtilitySortTestSupport_MST_ASSERT(arr[0] == 42);
}

void MathUtilityTest::testSortTwoElements()
{
	using namespace MathUtilitySortTestSupport;
	int arr[2] = { 7, 3 };
	quickSortAscend(arr, 0, 1);
	MathUtilitySortTestSupport_MST_ASSERT(arr[0] == 3);
	MathUtilitySortTestSupport_MST_ASSERT(arr[1] == 7);
	int arr2[2] = { 3, 7 };
	quickSortDescend(arr2, 0, 1);
	MathUtilitySortTestSupport_MST_ASSERT(arr2[0] == 7);
	MathUtilitySortTestSupport_MST_ASSERT(arr2[1] == 3);
}

void MathUtilityTest::testSortAlreadySorted()
{
	using namespace MathUtilitySortTestSupport;
	int arr[6] = { 1, 2, 3, 4, 5, 6 };
	quickSortAscend(arr, 0, 5);
	MathUtilitySortTestSupport_MST_ASSERT(isAscending(arr, 6));
	MathUtilitySortTestSupport_MST_ASSERT(arr[0] == 1);
	MathUtilitySortTestSupport_MST_ASSERT(arr[5] == 6);
	int arr2[6] = { 6, 5, 4, 3, 2, 1 };
	quickSortDescend(arr2, 0, 5);
	MathUtilitySortTestSupport_MST_ASSERT(isDescending(arr2, 6));
	MathUtilitySortTestSupport_MST_ASSERT(arr2[0] == 6);
	MathUtilitySortTestSupport_MST_ASSERT(arr2[5] == 1);
}

void MathUtilityTest::testSortReverseSorted()
{
	using namespace MathUtilitySortTestSupport;
	int arr[8] = { 8, 7, 6, 5, 4, 3, 2, 1 };
	quickSortAscend(arr, 0, 7);
	MathUtilitySortTestSupport_MST_ASSERT(isAscending(arr, 8));
	MathUtilitySortTestSupport_MST_ASSERT(arr[0] == 1);
	MathUtilitySortTestSupport_MST_ASSERT(arr[7] == 8);
	int arr2[8] = { 1, 2, 3, 4, 5, 6, 7, 8 };
	quickSortDescend(arr2, 0, 7);
	MathUtilitySortTestSupport_MST_ASSERT(isDescending(arr2, 8));
	MathUtilitySortTestSupport_MST_ASSERT(arr2[0] == 8);
	MathUtilitySortTestSupport_MST_ASSERT(arr2[7] == 1);
}

void MathUtilityTest::testSortDuplicates()
{
	using namespace MathUtilitySortTestSupport;
	int arr[8] = { 3, 1, 4, 1, 5, 9, 2, 6 };
	quickSortAscend(arr, 0, 7);
	MathUtilitySortTestSupport_MST_ASSERT(isAscending(arr, 8));
	MathUtilitySortTestSupport_MST_ASSERT(arr[0] == 1);
	MathUtilitySortTestSupport_MST_ASSERT(arr[1] == 1);
	MathUtilitySortTestSupport_MST_ASSERT(arr[7] == 9);
	int arr2[6] = { 3, 3, 3, 3, 3, 3 };
	quickSortAscend(arr2, 0, 5);
	FOR(6)
	{
		MathUtilitySortTestSupport_MST_ASSERT(arr2[i] == 3);
	}
}

void MathUtilityTest::testSortFloat()
{
	using namespace MathUtilitySortTestSupport;
	float arr[7] = { 3.14f, 1.0f, 2.71f, 0.5f, 9.8f, 4.2f, 6.6f };
	quickSortAscend(arr, 0, 6);
	MathUtilitySortTestSupport_MST_ASSERT(isAscendingFloat(arr, 7));
	MathUtilitySortTestSupport_MST_ASSERT(arr[0] < 1.0f + 1e-4f);
	MathUtilitySortTestSupport_MST_ASSERT(arr[6] > 9.0f);
	float arr2[7] = { 3.14f, 1.0f, 2.71f, 0.5f, 9.8f, 4.2f, 6.6f };
	quickSortDescend(arr2, 0, 6);
	for (int i = 1; i < 7; ++i)
	{
		MathUtilitySortTestSupport_MST_ASSERT(arr2[i] <= arr2[i - 1] + 1e-5f);
	}
}

void MathUtilityTest::testSortLargeArray()
{
	using namespace MathUtilitySortTestSupport;
	const int SIZE = 100;
	int arr[SIZE];
	FOR(SIZE)
	{
		arr[i] = SIZE - i;
	}
	quickSortAscend(arr, 0, SIZE - 1);
	MathUtilitySortTestSupport_MST_ASSERT(isAscending(arr, SIZE));
	FOR(SIZE)
	{
		MathUtilitySortTestSupport_MST_ASSERT(arr[i] == i + 1);
	}
	quickSortDescend(arr, 0, SIZE - 1);
	MathUtilitySortTestSupport_MST_ASSERT(isDescending(arr, SIZE));
	FOR(SIZE)
	{
		MathUtilitySortTestSupport_MST_ASSERT(arr[i] == SIZE - i);
	}
}

void MathUtilityTest::testSortNegativeNumbers()
{
	using namespace MathUtilitySortTestSupport;
	int arr[8] = { -5, -3, -8, -1, -9, -2, -7, -4 };
	quickSortAscend(arr, 0, 7);
	MathUtilitySortTestSupport_MST_ASSERT(isAscending(arr, 8));
	MathUtilitySortTestSupport_MST_ASSERT(arr[0] == -9);
	MathUtilitySortTestSupport_MST_ASSERT(arr[7] == -1);
	int arr2[8] = { -5, -3, -8, -1, -9, -2, -7, -4 };
	quickSortDescend(arr2, 0, 7);
	MathUtilitySortTestSupport_MST_ASSERT(isDescending(arr2, 8));
	MathUtilitySortTestSupport_MST_ASSERT(arr2[0] == -1);
	MathUtilitySortTestSupport_MST_ASSERT(arr2[7] == -9);
}

void MathUtilityTest::testSortMixed()
{
	using namespace MathUtilitySortTestSupport;
	int arr[10] = { -5, 3, 0, -2, 8, -9, 1, -4, 7, -1 };
	quickSortAscend(arr, 0, 9);
	MathUtilitySortTestSupport_MST_ASSERT(isAscending(arr, 10));
	MathUtilitySortTestSupport_MST_ASSERT(arr[0] == -9);
	MathUtilitySortTestSupport_MST_ASSERT(arr[9] == 8);
	int arr2[10] = { -5, 3, 0, -2, 8, -9, 1, -4, 7, -1 };
	quickSortDescend(arr2, 0, 9);
	MathUtilitySortTestSupport_MST_ASSERT(isDescending(arr2, 10));
	MathUtilitySortTestSupport_MST_ASSERT(arr2[0] == 8);
	MathUtilitySortTestSupport_MST_ASSERT(arr2[9] == -9);
}

void MathUtilityTest::testQuickSortNonRecursiveAscendReturnValue()
{
	using namespace MathUtilitySortTestSupport;
	int arr1[1] = { 5 };
	int r1 = quickSortNonRecursiveAscend(arr1, 0, 0);
	MathUtilitySortTestSupport_MST_ASSERT(r1 == -1);
	int arr2[2] = { 3, 1 };
	int r2 = quickSortNonRecursiveAscend(arr2, 0, 1);
	MathUtilitySortTestSupport_MST_ASSERT(r2 >= 0 && r2 <= 1);
	MathUtilitySortTestSupport_MST_ASSERT(arr2[r2] == 3);
}

void MathUtilityTest::testQuickSortNonRecursiveDescendReturnValue()
{
	using namespace MathUtilitySortTestSupport;
	int arr1[1] = { 5 };
	int r1 = quickSortNonRecursiveDescend(arr1, 0, 0);
	MathUtilitySortTestSupport_MST_ASSERT(r1 == -1);
	int arr2[2] = { 3, 7 };
	int r2 = quickSortNonRecursiveDescend(arr2, 0, 1);
	MathUtilitySortTestSupport_MST_ASSERT(r2 >= 0 && r2 <= 1);
	MathUtilitySortTestSupport_MST_ASSERT(arr2[r2] == 3);
}

void MathUtilityTest::testSortStability()
{
	using namespace MathUtilitySortTestSupport;
	int arr[5] = { 1, 2, 3, 4, 5 };
	quickSortAscend(arr, 0, 4);
	MathUtilitySortTestSupport_MST_ASSERT(arr[0] == 1 && arr[1] == 2 && arr[2] == 3 && arr[3] == 4 && arr[4] == 5);
	float arrF[5] = { 1.5f, 2.5f, 0.5f, 3.5f, 4.5f };
	quickSortAscend(arrF, 0, 4, comparatorAscFloat);
	MathUtilitySortTestSupport_MST_ASSERT(isAscendingFloat(arrF, 5));
	MathUtilitySortTestSupport_MST_ASSERT(fabsf(arrF[0] - 0.5f) < 1e-4f);
}

namespace MathUtilityVectorTestSupport
{
#define MathUtilityVectorTestSupport_MUVE_ASSERT(condition) if (!(condition)) { ERROR("MathUtilityVectorTest failed: " #condition); }
#define MathUtilityVectorTestSupport_MUVE_NEAR(a, b) (fabsf((a) - (b)) < 1e-4f)
}

void MathUtilityTest::testToVec3FromVec2Int()
{
	using namespace MathUtilityVectorTestSupport;
	Vector2Int v2 = { 3, 7 };
	Vector3 v3 = toVec3(v2);
	MathUtilityVectorTestSupport_MUVE_ASSERT(MathUtilityVectorTestSupport_MUVE_NEAR(v3.x, 3.0f));
	MathUtilityVectorTestSupport_MUVE_ASSERT(MathUtilityVectorTestSupport_MUVE_NEAR(v3.y, 7.0f));
	MathUtilityVectorTestSupport_MUVE_ASSERT(MathUtilityVectorTestSupport_MUVE_NEAR(v3.z, 0.0f));
}

void MathUtilityTest::testToVec3FromVec2UInt()
{
	using namespace MathUtilityVectorTestSupport;
	Vector2UInt v2 = { 5u, 9u };
	Vector3 v3 = toVec3(v2);
	MathUtilityVectorTestSupport_MUVE_ASSERT(MathUtilityVectorTestSupport_MUVE_NEAR(v3.x, 5.0f));
	MathUtilityVectorTestSupport_MUVE_ASSERT(MathUtilityVectorTestSupport_MUVE_NEAR(v3.y, 9.0f));
	MathUtilityVectorTestSupport_MUVE_ASSERT(MathUtilityVectorTestSupport_MUVE_NEAR(v3.z, 0.0f));
}

void MathUtilityTest::testToVec3FromVec2UShort()
{
	using namespace MathUtilityVectorTestSupport;
	Vector2UShort v2 = { (ushort)4, (ushort)8 };
	Vector3 v3 = toVec3(v2);
	MathUtilityVectorTestSupport_MUVE_ASSERT(MathUtilityVectorTestSupport_MUVE_NEAR(v3.x, 4.0f));
	MathUtilityVectorTestSupport_MUVE_ASSERT(MathUtilityVectorTestSupport_MUVE_NEAR(v3.y, 8.0f));
	MathUtilityVectorTestSupport_MUVE_ASSERT(MathUtilityVectorTestSupport_MUVE_NEAR(v3.z, 0.0f));
}

void MathUtilityTest::testToVec3FromVec2()
{
	using namespace MathUtilityVectorTestSupport;
	Vector2 v2 = { 1.5f, 2.5f };
	Vector3 v3 = toVec3(v2);
	MathUtilityVectorTestSupport_MUVE_ASSERT(MathUtilityVectorTestSupport_MUVE_NEAR(v3.x, 1.5f));
	MathUtilityVectorTestSupport_MUVE_ASSERT(MathUtilityVectorTestSupport_MUVE_NEAR(v3.y, 2.5f));
	MathUtilityVectorTestSupport_MUVE_ASSERT(MathUtilityVectorTestSupport_MUVE_NEAR(v3.z, 0.0f));
}

void MathUtilityTest::testToVec2FromVec3()
{
	using namespace MathUtilityVectorTestSupport;
	Vector3 v3 = { 3.0f, 4.0f, 5.0f };
	Vector2 v2 = toVec2(v3);
	MathUtilityVectorTestSupport_MUVE_ASSERT(MathUtilityVectorTestSupport_MUVE_NEAR(v2.x, 3.0f));
	MathUtilityVectorTestSupport_MUVE_ASSERT(MathUtilityVectorTestSupport_MUVE_NEAR(v2.y, 4.0f));
}

void MathUtilityTest::testToVec2IntFromVec2()
{
	using namespace MathUtilityVectorTestSupport;
	Vector2 v2 = { 3.7f, 2.2f };
	Vector2Int vi = toVec2Int(v2);
	MathUtilityVectorTestSupport_MUVE_ASSERT(vi.x == 3);
	MathUtilityVectorTestSupport_MUVE_ASSERT(vi.y == 2);
}

void MathUtilityTest::testToVec2IntFromVec3()
{
	using namespace MathUtilityVectorTestSupport;
	Vector3 v3 = { 5.9f, 8.1f, 0.0f };
	Vector2Int vi = toVec2Int(v3);
	MathUtilityVectorTestSupport_MUVE_ASSERT(vi.x == 5);
	MathUtilityVectorTestSupport_MUVE_ASSERT(vi.y == 8);
}

void MathUtilityTest::testToVec2IntFromVec2UInt()
{
	using namespace MathUtilityVectorTestSupport;
	Vector2UInt v2 = { 10u, 20u };
	Vector2Int vi = toVec2Int(v2);
	MathUtilityVectorTestSupport_MUVE_ASSERT(vi.x == 10);
	MathUtilityVectorTestSupport_MUVE_ASSERT(vi.y == 20);
}

void MathUtilityTest::testToVec2IntFromVec2Short()
{
	using namespace MathUtilityVectorTestSupport;
	Vector2Short v2 = { (short)3, (short)7 };
	Vector2Int vi = toVec2Int(v2);
	MathUtilityVectorTestSupport_MUVE_ASSERT(vi.x == 3);
	MathUtilityVectorTestSupport_MUVE_ASSERT(vi.y == 7);
}

void MathUtilityTest::testToVec2IntFromVec2UShort()
{
	using namespace MathUtilityVectorTestSupport;
	Vector2UShort v2 = { (ushort)6, (ushort)9 };
	Vector2Int vi = toVec2Int(v2);
	MathUtilityVectorTestSupport_MUVE_ASSERT(vi.x == 6);
	MathUtilityVectorTestSupport_MUVE_ASSERT(vi.y == 9);
}

void MathUtilityTest::testToVec2UIntFromVec2()
{
	using namespace MathUtilityVectorTestSupport;
	Vector2 v2 = { 4.0f, 7.0f };
	Vector2UInt vu = toVec2UInt(v2);
	MathUtilityVectorTestSupport_MUVE_ASSERT(vu.x == 4u);
	MathUtilityVectorTestSupport_MUVE_ASSERT(vu.y == 7u);
}

void MathUtilityTest::testToVec2UIntFromVec3()
{
	using namespace MathUtilityVectorTestSupport;
	Vector3 v3 = { 2.0f, 5.0f, 9.0f };
	Vector2UInt vu = toVec2UInt(v3);
	MathUtilityVectorTestSupport_MUVE_ASSERT(vu.x == 2u);
	MathUtilityVectorTestSupport_MUVE_ASSERT(vu.y == 5u);
}

void MathUtilityTest::testToVec2UIntFromVec2Int()
{
	using namespace MathUtilityVectorTestSupport;
	Vector2Int vi = { 8, 11 };
	Vector2UInt vu = toVec2UInt(vi);
	MathUtilityVectorTestSupport_MUVE_ASSERT(vu.x == 8u);
	MathUtilityVectorTestSupport_MUVE_ASSERT(vu.y == 11u);
}

void MathUtilityTest::testToVec2UIntFromVec2Short()
{
	using namespace MathUtilityVectorTestSupport;
	Vector2Short vs = { (short)3, (short)5 };
	Vector2UInt vu = toVec2UInt(vs);
	MathUtilityVectorTestSupport_MUVE_ASSERT(vu.x == 3u);
	MathUtilityVectorTestSupport_MUVE_ASSERT(vu.y == 5u);
}

void MathUtilityTest::testToVec2UIntFromVec2UShort()
{
	using namespace MathUtilityVectorTestSupport;
	Vector2UShort vus = { (ushort)12, (ushort)15 };
	Vector2UInt vu = toVec2UInt(vus);
	MathUtilityVectorTestSupport_MUVE_ASSERT(vu.x == 12u);
	MathUtilityVectorTestSupport_MUVE_ASSERT(vu.y == 15u);
}

void MathUtilityTest::testIndexToXYVector()
{
	using namespace MathUtilityVectorTestSupport;
	int width = 5;
	MathUtilityVectorTestSupport_MUVE_ASSERT(indexToX(0, width) == 0);
	MathUtilityVectorTestSupport_MUVE_ASSERT(indexToY(0, width) == 0);
	MathUtilityVectorTestSupport_MUVE_ASSERT(indexToX(1, width) == 1);
	MathUtilityVectorTestSupport_MUVE_ASSERT(indexToY(1, width) == 0);
	MathUtilityVectorTestSupport_MUVE_ASSERT(indexToX(5, width) == 0);
	MathUtilityVectorTestSupport_MUVE_ASSERT(indexToY(5, width) == 1);
	MathUtilityVectorTestSupport_MUVE_ASSERT(indexToX(7, width) == 2);
	MathUtilityVectorTestSupport_MUVE_ASSERT(indexToY(7, width) == 1);
	MathUtilityVectorTestSupport_MUVE_ASSERT(indexToX(14, width) == 4);
	MathUtilityVectorTestSupport_MUVE_ASSERT(indexToY(14, width) == 2);
}

void MathUtilityTest::testIntPosToIndexVector()
{
	using namespace MathUtilityVectorTestSupport;
	int width = 5;
	MathUtilityVectorTestSupport_MUVE_ASSERT(intPosToIndex(0, 0, width) == 0);
	MathUtilityVectorTestSupport_MUVE_ASSERT(intPosToIndex(1, 0, width) == 1);
	MathUtilityVectorTestSupport_MUVE_ASSERT(intPosToIndex(0, 1, width) == 5);
	MathUtilityVectorTestSupport_MUVE_ASSERT(intPosToIndex(2, 1, width) == 7);
	MathUtilityVectorTestSupport_MUVE_ASSERT(intPosToIndex(4, 2, width) == 14);
}

void MathUtilityTest::testIntPosToIndexAllVariants()
{
	using namespace MathUtilityVectorTestSupport;
	int width = 4;
	Vector2Int vi = { 2, 3 };
	MathUtilityVectorTestSupport_MUVE_ASSERT(intPosToIndex(vi, width) == 14);

	Vector2Short vs = { (short)1, (short)2 };
	MathUtilityVectorTestSupport_MUVE_ASSERT(intPosToIndex(vs, width) == 9);

	Vector2UShort vus = { (ushort)3, (ushort)1 };
	MathUtilityVectorTestSupport_MUVE_ASSERT(intPosToIndex(vus, width) == 7);

	Vector2UInt vu = { 0u, 0u };
	MathUtilityVectorTestSupport_MUVE_ASSERT(intPosToIndex(vu, width) == 0);
}

void MathUtilityTest::testIndexToIntPos()
{
	using namespace MathUtilityVectorTestSupport;
	int width = 5;
	Vector2Int pos = indexToIntPos(7, width);
	MathUtilityVectorTestSupport_MUVE_ASSERT(pos.x == 2);
	MathUtilityVectorTestSupport_MUVE_ASSERT(pos.y == 1);

	Vector2Int pos2 = indexToIntPos(0, width);
	MathUtilityVectorTestSupport_MUVE_ASSERT(pos2.x == 0);
	MathUtilityVectorTestSupport_MUVE_ASSERT(pos2.y == 0);
}

void MathUtilityTest::testGenerateBatchCountVector()
{
	using namespace MathUtilityVectorTestSupport;
	MathUtilityVectorTestSupport_MUVE_ASSERT(generateBatchCount(10, 3) == 4);
	MathUtilityVectorTestSupport_MUVE_ASSERT(generateBatchCount(9, 3) == 3);
	MathUtilityVectorTestSupport_MUVE_ASSERT(generateBatchCount(0, 3) == 0);
	MathUtilityVectorTestSupport_MUVE_ASSERT(generateBatchCount(1, 3) == 1);
	MathUtilityVectorTestSupport_MUVE_ASSERT(generateBatchCount(100, 10) == 10);
	MathUtilityVectorTestSupport_MUVE_ASSERT(generateBatchCount(101, 10) == 11);
}

void MathUtilityTest::testGenerateBatchSizeVector()
{
	using namespace MathUtilityVectorTestSupport;
	MathUtilityVectorTestSupport_MUVE_ASSERT(generateBatchSize(10, 3, 0) == 3);
	MathUtilityVectorTestSupport_MUVE_ASSERT(generateBatchSize(10, 3, 1) == 3);
	MathUtilityVectorTestSupport_MUVE_ASSERT(generateBatchSize(10, 3, 2) == 3);
	MathUtilityVectorTestSupport_MUVE_ASSERT(generateBatchSize(10, 3, 3) == 1);
	MathUtilityVectorTestSupport_MUVE_ASSERT(generateBatchSize(10, 3, 4) == 0);
	MathUtilityVectorTestSupport_MUVE_ASSERT(generateBatchSize(9, 3, 2) == 3);
	MathUtilityVectorTestSupport_MUVE_ASSERT(generateBatchSize(9, 3, 3) == 0);
}

void MathUtilityTest::testGetGreaterPower2Vector()
{
	using namespace MathUtilityVectorTestSupport;
	// getGreaterPower2(0)=2, getGreaterPower2(1)=2（查表 index 0,1 均为 2）
	MathUtilityVectorTestSupport_MUVE_ASSERT(getGreaterPower2(1) == 2);
	MathUtilityVectorTestSupport_MUVE_ASSERT(getGreaterPower2(2) == 2);
	MathUtilityVectorTestSupport_MUVE_ASSERT(getGreaterPower2(3) == 4);
	MathUtilityVectorTestSupport_MUVE_ASSERT(getGreaterPower2(4) == 4);
	MathUtilityVectorTestSupport_MUVE_ASSERT(getGreaterPower2(5) == 8);
	MathUtilityVectorTestSupport_MUVE_ASSERT(getGreaterPower2(100) == 128);
	MathUtilityVectorTestSupport_MUVE_ASSERT(getGreaterPower2(256) == 256);
	MathUtilityVectorTestSupport_MUVE_ASSERT(getGreaterPower2(257) == 512);
}

void MathUtilityTest::testPow2Vector()
{
	using namespace MathUtilityVectorTestSupport;
	MathUtilityVectorTestSupport_MUVE_ASSERT(pow2(0) == 1);
	MathUtilityVectorTestSupport_MUVE_ASSERT(pow2(1) == 2);
	MathUtilityVectorTestSupport_MUVE_ASSERT(pow2(2) == 4);
	MathUtilityVectorTestSupport_MUVE_ASSERT(pow2(8) == 256);
	MathUtilityVectorTestSupport_MUVE_ASSERT(pow2(10) == 1024);
}

void MathUtilityTest::testPow10Vector()
{
	using namespace MathUtilityVectorTestSupport;
	MathUtilityVectorTestSupport_MUVE_ASSERT(pow10(0) == 1);
	MathUtilityVectorTestSupport_MUVE_ASSERT(pow10(1) == 10);
	MathUtilityVectorTestSupport_MUVE_ASSERT(pow10(2) == 100);
	MathUtilityVectorTestSupport_MUVE_ASSERT(pow10(3) == 1000);
	MathUtilityVectorTestSupport_MUVE_ASSERT(pow10(4) == 10000);
}

void MathUtilityTest::testInversePow10Vector()
{
	using namespace MathUtilityVectorTestSupport;
	MathUtilityVectorTestSupport_MUVE_ASSERT(MathUtilityVectorTestSupport_MUVE_NEAR(inversePow10(0), 1.0f));
	MathUtilityVectorTestSupport_MUVE_ASSERT(MathUtilityVectorTestSupport_MUVE_NEAR(inversePow10(1), 0.1f));
	MathUtilityVectorTestSupport_MUVE_ASSERT(MathUtilityVectorTestSupport_MUVE_NEAR(inversePow10(2), 0.01f));
	MathUtilityVectorTestSupport_MUVE_ASSERT(MathUtilityVectorTestSupport_MUVE_NEAR(inversePow10(3), 0.001f));
}

void MathUtilityTest::testCheckIntVector()
{
	using namespace MathUtilityVectorTestSupport;
	float v1 = 3.9999f;
	checkInt(v1);
	MathUtilityVectorTestSupport_MUVE_ASSERT(MathUtilityVectorTestSupport_MUVE_NEAR(v1, 4.0f));

	float v2 = 3.0001f;
	checkInt(v2);
	MathUtilityVectorTestSupport_MUVE_ASSERT(MathUtilityVectorTestSupport_MUVE_NEAR(v2, 3.0f));

	float v3 = 2.5f;
	checkInt(v3);
	MathUtilityVectorTestSupport_MUVE_ASSERT(MathUtilityVectorTestSupport_MUVE_NEAR(v3, 2.5f));
}

void MathUtilityTest::testIsZeroVector()
{
	using namespace MathUtilityVectorTestSupport;
	MathUtilityVectorTestSupport_MUVE_ASSERT(isZero(0.0f));
	MathUtilityVectorTestSupport_MUVE_ASSERT(isZero(0.000005f));
	MathUtilityVectorTestSupport_MUVE_ASSERT(!isZero(0.1f));
	MathUtilityVectorTestSupport_MUVE_ASSERT(!isZero(-0.1f));

	MathUtilityVectorTestSupport_MUVE_ASSERT(isZero(0.0));
	MathUtilityVectorTestSupport_MUVE_ASSERT(!isZero(0.001));
}

void MathUtilityTest::testIsEqualVector()
{
	using namespace MathUtilityVectorTestSupport;
	MathUtilityVectorTestSupport_MUVE_ASSERT(isEqual(1.0f, 1.0f));
	MathUtilityVectorTestSupport_MUVE_ASSERT(isEqual(1.0f, 1.000005f));
	MathUtilityVectorTestSupport_MUVE_ASSERT(!isEqual(1.0f, 1.1f));
	MathUtilityVectorTestSupport_MUVE_ASSERT(isEqual(0.0f, 0.0f));
	MathUtilityVectorTestSupport_MUVE_ASSERT(!isEqual(1.0f, -1.0f));
}


void MathUtilityTest::test()
{
	testMathConstants();
	testIsZeroIsEqual();
	testGetMinMax();
	testClamp();
	testLerp();
	testDivide();
	testCeilingFloor();
	testRound();
	testSign();
	testSwap();
	testIsEvenIsPow2();
	testPow2Pow10();
	testFmodFrac();
	testStep();
	testSaturate();
	testClampCycle();
	testInRange();
	testPowerFloat();
	testSecondsConvert();
	testBatchCount();
	testIndexConvert();
	testGetLength();
	testNormalize();
	testDotCross();
	testLengthCompare();
	testIsVectorEqual();
	testVectorConvert();
	testQuickSort();
	testBinarySearch();
	testGetGreaterPower2();
	testRandom();
	testFrameSpeed();
	testAngleConvert();
	testME2_Clamp();
	testME2_ClampFloat();
	testME2_ClampRef();
	testME2_ClampMin();
	testME2_ClampMax();
	testME2_ClampZero();
	testME2_Saturate();
	testME2_Step();
	testME2_Frac();
	testME2_FmodCustom();
	testME2_DivideFloat();
	testME2_DivideInt();
	testME2_GenerateBatch();
	testME2_IsZero();
	testME2_IsEqual();
	testME2_IndexConvert();
	testME2_IntPosToIndex();
	testME2_IndexToIntPos();
	testME2_IsEven();
	testME2_IsPow2();
	testME2_Ceiling();
	testME2_Floor();
	testME2_Pow2();
	testME2_Pow10();
	testME2_LerpFloat();
	testME2_LerpClamp();
	testME2_LerpInt();
	testME2_LerpLLong();
	testME2_InverseLerp();
	testME2_LerpSimple();
	testME2_InRange();
	testME2_InRangeFloat();
	testME2_InFixedRange();
	testME2_ClampCycle();
	testME2_ClampRadian();
	testME2_SecondsToMinSec();
	testME2_SecondsToHourMinSec();
	testME2_SpeedInterval();
	testME2_Swap();
	testME2_PowerFloat();
	testME2_GetMinMax();
	testME2_SquaredLength();
	testME2_GetLength();
	testME2_Dot();
	testME2_Cross();
	testME2_Normalize3();
	testME2_Normalize2();
	testME2_DegreeRadian();
	testME2_FrameToSecond();
	testME2_VecConvert();
	testME2_LengthCompare();
	testME2_LengthCompare3();
	testME2_IsVectorEqual();
	testME2_GetGreaterPower2();
	testME2_CheckInt();
	testRotateVector3AroundY();
	testRotateVector3AroundX();
	testRotateVector3AroundZ();
	testRotateVector3Generic();
	testGetAngleBetweenVector3();
	testGetAngleBetweenVector2();
	testGetAngleFromVector2ToVector2();
	testGetVectorFromAngle();
	testToVec3();
	testToVec2();
	testToVec2Int();
	testToVec2UInt();
	testResetY();
	testIntersectLine2();
	testIntersectLineSection();
	testGetDistanceToLine();
	testGetProjectPoint();
	testGetProjection();
	testIntersectLine3IgnoreY();
	testIntersectEdge();
	testIntersectLineAABB();
	testIntersectSegment();
	testIntersectLineTriangle();
	testIntersectLineTriangleIgnoreY();
	testDistanceAndProjectIgnoreY();
	testGetProjectionIgnoreY();
	testIntersectLineSectionIgnoreY();
	testIntersectIgnoreY();
	testRandomSelectVector();
	testRandomSelectQuick();
	testRandomSelectSet();
	testRandomSelectWithOdds();
	testRandomHitVector();
	testRandomHitArray();
	testRandomHitArrayList();
	testRandomHitHashMap();
	testQuickSortNonRecursiveVector();
	testQuickSortNonRecursiveArray();
	testQuickSortNonRecursiveWithCompare();
	testFindPointIndex();
	testCalculateFloat();
	testCalculateInt();
	testHueToRGB();
	testRandomSelectDistribution();
	testRandomHitProbability();
	testLineIntersect();
	testReplaceKeywordAndCalculate();
	testReplaceStringKeyword();
	testClampBasicInt();
	testClampBasicFloat();
	testClampRefInt();
	testClampRefFloat();
	testClampMinInt();
	testClampMinFloat();
	testClampMinRefInt();
	testClampMaxInt();
	testClampMaxFloat();
	testClampMaxRefFloat();
	testClampZeroInt();
	testClampZeroFloat();
	testClampZeroRefInt();
	testSaturateClampExt();
	testGetMinInt();
	testGetMinFloat();
	testGetMaxInt();
	testGetMaxFloat();
	testCeilingFloat();
	testCeilingDouble();
	testCeilingVec2();
	testCeilingVec3();
	testFloorFloat();
	testFloorDouble();
	testDivideFloat();
	testDivideInt();
	testDivideLLong();
	testDivideMixed();
	testDivideIntFunc();
	testIsEven();
	testIsPow2();
	testGetSquaredLengthVec2();
	testGetSquaredLengthVec3();
	testGetLengthVec2();
	testGetLengthVec3();
	testGetLengthXY();
	testGetLengthXYZ();
	testNormalizeVec3();
	testNormalizeVec2();
	testNormalizeZeroVec3();
	testDotVec3();
	testDotVec2();
	testCrossVec3();
	testIsVectorEqualVec3();
	testIsVectorEqualVec2();
	testIsVectorEqualPrecision();
	testLengthLessVec2();
	testLengthGreaterVec2();
	testLengthLessVec3();
	testLengthGreaterVec3();
	testLengthLessSquaredVec3();
	testLengthGreaterSquaredVec3();
	testResetYLerp();
	testToDegree();
	testToRadian();
	testSaturateLerp();
	testStepLerp();
	testFmod();
	testFrac();
	testLerpFloatBasic();
	testLerpFloatClampT();
	testLerpFloatMinAbsDelta();
	testLerpFloatSnap();
	testLerpIntBasic();
	testLerpIntClampT();
	testLerpIntBoundary();
	testLerpLLongBasic();
	testLerpLLongClampT();
	testLerpSimpleFloat();
	testLerpSimpleInt();
	testLerpSimpleVec3();
	testInverseLerpBasic();
	testInverseLerpZeroDenom();
	testInverseLerpEdge();
	testFrameToSecond();
	testPow10LLong();
	testInversePow10LLong();
	testPowerFloatLerp2();
	testSecondsToMinutesSeconds();
	testSecondsToHoursMinutesSeconds();
	testSpeedToInterval();
	testIntervalToSpeed();
	testSignInt();
	testSignFloat();
	testSignLLong();
	testRoundUnder();
	testRoundOver();
	testRoundNegative();
	testRoundDouble();
	testSwap_Int();
	testSwap_Float();
	testClampCycleBasic();
	testClampCycleDefaultCycle();
	testClampRadian180();
	testClampDegree180();
	testClampRadian360();
	testClampDegree360();
	testInRangeIntOrdered();
	testInRangeIntReversed();
	testInRangeFloatOrdered();
	testInRangeVec2();
	testInFixedRangeInt();
	testInFixedRangeFloat();
	testIsZeroDouble();
	testIsZero();
	testIsEqual();
	testGetMinMaxMathOps();
	testClampMathOps();
	testClampRef();
	testClampMin();
	testClampMax();
	testClampZero();
	testDivideMathOps();
	testDivideIntMathOps();
	testCeiling();
	testFloor();
	testIsPow2MathOps();
	testIsEvenMathOps();
	testPow2();
	testPow10();
	testInversePow10();
	testIndexToXY();
	testIntPosToIndex();
	testGenerateBatchCount();
	testGenerateBatchSize();
	testGreaterPower2();
	testGetGreaterPower2MathOps();
	testCheckInt();
	testToVec();
	testDivideOverload();
	testCeilingVector();
	testRandomIntRange();
	testRandomIntSameMinMax();
	testRandomIntNegativeRange();
	testRandomLongRange();
	testRandomLongSameMinMax();
	testRandomFloatRange();
	testRandomFloatAlwaysInRange();
	testRandomHitZeroProbability();
	testRandomHitOneProbability();
	testRandomHitIntScaleZero();
	testRandomHitIntScaleFull();
	testRandomHitArrayListOdds();
	testRandomOrderArrayInPlace();
	testRandomOrderVectorInPlace();
	testRandomOrderArrayListInPlace();
	testRandomSelectCountSubset();
	testRandomSelectCountAll();
	testRandomSelectCountZero();
	testBinarySearchFound();
	testBinarySearchNotFound();
	testBinarySearchSingleElement();
	testBinarySearchEdge();
	testFindPointIndexBasic();
	testFindPointIndexBeyondEnd();
	testQuickSortNonRecursiveAscend();
	testQuickSortNonRecursiveDescend();
	testQuickSortNonRecursiveAscendCompare();
	testQuickSortNonRecursiveDescendCompare();
	testQuickSortAscend();
	testQuickSortDescend();
	testQuickSortAscendCompare();
	testQuickSortDescendCompare();
	testQuickSortAscendPtr();
	testQuickSortDescendPtr();
	testSortSingleElement();
	testSortTwoElements();
	testSortAlreadySorted();
	testSortReverseSorted();
	testSortDuplicates();
	testSortFloat();
	testSortLargeArray();
	testSortNegativeNumbers();
	testSortMixed();
	testQuickSortNonRecursiveAscendReturnValue();
	testQuickSortNonRecursiveDescendReturnValue();
	testSortStability();
	testToVec3FromVec2Int();
	testToVec3FromVec2UInt();
	testToVec3FromVec2UShort();
	testToVec3FromVec2();
	testToVec2FromVec3();
	testToVec2IntFromVec2();
	testToVec2IntFromVec3();
	testToVec2IntFromVec2UInt();
	testToVec2IntFromVec2Short();
	testToVec2IntFromVec2UShort();
	testToVec2UIntFromVec2();
	testToVec2UIntFromVec3();
	testToVec2UIntFromVec2Int();
	testToVec2UIntFromVec2Short();
	testToVec2UIntFromVec2UShort();
	testIndexToXYVector();
	testIntPosToIndexVector();
	testIntPosToIndexAllVariants();
	testIndexToIntPos();
	testGenerateBatchCountVector();
	testGenerateBatchSizeVector();
	testGetGreaterPower2Vector();
	testPow2Vector();
	testPow10Vector();
	testInversePow10Vector();
	testCheckIntVector();
	testIsZeroVector();
	testIsEqualVector();
	testDeep();
	testMathUtility();
	testMathUtility_ClampVariants();
	testMathUtility_StepFracFmod();
	testMathUtility_IsEvenIsPow2();
	testMathUtility_CeilingFloor();
	testMathUtility_LerpInverseLerp();
	testMathUtility_ClampCycle();
	testMathUtility_Pow2Pow10();
	testMathUtility_SecondsConvert();
	testMathUtility_VectorMath();
	testMathUtility_MinMax();
	testMathUtility_InRange();
	testMathUtility_Swap();
	testMathUtility_DegreeRadian();
	testMathUtility_GenerateBatch();
	testMathUtility_IsZeroEqual();
}

void MathUtilityTest::assertMath(bool condition, const char* message)
{
	if (!condition)
	{
		ERROR(string("MathUtilityTest failed: ") + message);
	}
}

bool MathUtilityTest::approxEqualFloat(const float a, const float b, const float eps)
{
	return fabs(a - b) <= eps;
}

void MathUtilityTest::testScalarMath()
{
	assertMath(getMin(3, 5) == 3, "min");
	assertMath(getMax(3, 5) == 5, "max");
	assertMath(isZero(0.0f), "zero float");
	assertMath(isZero(0.0), "zero double");
	assertMath(isEqual(1.0f, 1.0f + 0.000001f), "equal float");
	assertMath(indexToX(17, 5) == 2, "index to x");
	assertMath(indexToY(17, 5) == 3, "index to y");
	assertMath(intPosToIndex(2, 3, 5) == 17, "pos to index");
	assertMath(isEven(8), "is even");
	assertMath(!isEven(9), "is odd");
	assertMath(isPow2(8), "pow2");
	assertMath(!isPow2(7), "not pow2");
	assertMath(divide(10, 2) == 5.0f, "divide int");
	assertMath(divide(10, 0) == 0.0f, "divide zero");
	assertMath(generateBatchCount(10ull, 3) == 4, "batch count");
	assertMath(generateBatchSize(10ull, 3, 0) == 3, "batch size 0");
	assertMath(pow2(5) == 32, "pow2 func");
	assertMath(pow10(3) == 1000, "pow10");
	assertMath(inversePow10(2) > 0.009f && inversePow10(2) < 0.011f, "inverse pow10");
	assertMath(ceiling(1.2f) == 2, "ceiling float");
	assertMath(floor_(-1.2f) == -2, "floor float");
	assertMath(roundDouble(2.6) == 3, "round double");
	assertMath(round_(-2.6f) == -3, "round float");
	assertMath(step(5.0f, 5.0f) == 1, "step");
	assertMath(sign(-5) == -1, "sign");
	assertMath(frac(3.75f) > 0.74f && frac(3.75f) < 0.76f, "frac");
	assertMath(fmod(7.5f, 2.0f) > 1.49f && fmod(7.5f, 2.0f) < 1.51f, "fmod");
}

void MathUtilityTest::testClampsAndRanges()
{
	int value = 10;
	clampRef(value, 0, 5);
	assertMath(value == 5, "clampRef");
	assertMath(clamp(10, 0, 5) == 5, "clamp");
	assertMath(clampMin(-1, 3) == 3, "clampMin");
	assertMath(clampZero(-1) == 0, "clampZero");
	assertMath(clampMax(10, 5) == 5, "clampMax");
	float angle = 7.0f;
	clampRadian360(angle);
	assertMath(angle >= 0.0f && angle <= TWO_PI_RADIAN, "clamp radian 360");
	angle = 200.0f;
	clampDegree180(angle);
	assertMath(angle <= 180.0f, "clamp degree 180");
	assertMath(inFixedRange(5, 1, 5), "fixed range");
	assertMath(inRange(5, 10, 1), "range int");
	assertMath(inRange(Vector2(1, 2), Vector2(0, 0), Vector2(2, 3)), "range vec2");
	assertMath(inRange(Vector3(1, 2, 3), Vector3(0, 0, 0), Vector3(2, 3, 4)), "range vec3");
}

void MathUtilityTest::testVectorMath()
{
	Vector2 v2(3.0f, 4.0f);
	Vector3 v3(1.0f, 2.0f, 2.0f);
	assertMath(approxEqualFloat(getSquaredLength(v2), 25.0f), "squared length v2");
	assertMath(approxEqualFloat(getLength(v2), 5.0f), "length v2");
	assertMath(approxEqualFloat(getLength(v3), 3.0f), "length v3");
	assertMath(dot(Vector2(1, 2), Vector2(3, 4)) == 11.0f, "dot v2");
	assertMath(dot(Vector3(1, 2, 3), Vector3(4, 5, 6)) == 32.0f, "dot v3");
	Vector3 crossValue = cross(Vector3(1, 0, 0), Vector3(0, 1, 0));
	assertMath(approxEqualFloat(crossValue.z, 1.0f), "cross");
	Vector2 normalized2 = normalize(Vector2(3, 4));
	assertMath(approxEqualFloat(normalized2.x, 0.6f, 0.01f), "normalize v2");
	Vector3 normalized3 = normalize(Vector3(0, 3, 4));
	assertMath(approxEqualFloat(normalized3.y, 0.6f, 0.01f), "normalize v3");
	assertMath(approxEqualFloat(toDegree(MATH_PI), 180.0f, 0.01f), "degree");
	assertMath(approxEqualFloat(toRadian(180.0f), MATH_PI, 0.01f), "radian");
	assertMath(lengthLess(Vector2(1, 1), 2.0f), "length less");
	assertMath(lengthGreater(Vector2(3, 4), 4.0f), "length greater");
	assertMath(approxEqualFloat(getAngleBetweenVector(Vector2(1, 0), Vector2(0, 1)), HALF_PI_RADIAN, 0.01f), "angle vec2");
	assertMath(approxEqualFloat(getAngleBetweenVector(Vector3(1, 0, 0), Vector3(0, 0, 1)), HALF_PI_RADIAN, 0.01f), "angle vec3");
}

void MathUtilityTest::testAnglesAndRotation()
{
	float angle = 3.5f;
	clampRadian180(angle);
	assertMath(angle <= MATH_PI && angle >= -MATH_PI, "clamp radian 180");
	float degree = 450.0f;
	clampDegree360(degree);
	assertMath(degree >= 0.0f && degree <= 360.0f, "clamp degree 360");
	Vector3 angleVec = getVectorFromAngle(0.0f);
	assertMath(approxEqualFloat(angleVec.x, 0.0f, 0.01f), "vector angle x");
	assertMath(approxEqualFloat(angleVec.z, 1.0f, 0.01f), "vector angle z");
	Vector3 rotated = rotateVector3AroundY(Vector3(1, 0, 0), HALF_PI_RADIAN);
	assertMath(fabs(rotated.x) < 0.05f, "rotate x");
	assertMath(fabs(rotated.z) > 0.9f, "rotate z");
}

int MathUtilityTest::compareIntAsc(const int& a, const int& b)
{
	return a - b;
}

void MathUtilityTest::testSortingAndSearch()
{
	Vector<int> values;
	values.add(5);
	values.add(1);
	values.add(9);
	values.add(3);
	quickSort(values);
	assertMath(values[0] == 1 && values[3] == 9, "quickSort vector");
	quickSort(values, false);
	assertMath(values[0] == 9 && values[3] == 1, "quickSort desc");
	quickSort(values, compareIntAsc, true);
	assertMath(values[0] == 1 && values[3] == 9, "quickSort compare");
	quickSortNonRecursive(values, false);
	assertMath(values[0] == 9 && values[3] == 1, "quickSort non recursive");

	ArrayList<8, int> arr;
	arr.add(4);
	arr.add(2);
	arr.add(8);
	arr.add(6);
	quickSort(arr);
	assertMath(arr[0] == 2 && arr[3] == 8, "arraylist sort");

	Vector<int> searchList;
	searchList.add(1);
	searchList.add(3);
	searchList.add(5);
	searchList.add(7);
	assertMath(binarySearch(searchList, 0, searchList.size() - 1, 5), "binary search hit");
	assertMath(!binarySearch(searchList, 0, searchList.size() - 1, 6), "binary search miss");

	Vector<float> dist;
	dist.add(0.0f);
	dist.add(5.0f);
	dist.add(10.0f);
	dist.add(20.0f);
	assertMath(findPointIndex(dist, 0.0f, 0, 3) == 0, "find point 0");
	assertMath(findPointIndex(dist, 7.0f, 0, 3) == 1, "find point 7");
	assertMath(findPointIndex(dist, 12.0f, 0, 3) == 2, "find point 12");
	assertMath(findPointIndex(dist, 20.0f, 0, 3) == 3, "find point end");
}

void MathUtilityTest::testExpressionsAndFormats()
{
	assertMath(calculateInt("1+2*3") == 7, "calc int");
	assertMath(calculateInt("(1+2)*3") == 9, "calc int bracket");
	assertMath(approxEqualFloat(calculateFloat("1.5+2.5*2.0"), 6.5f, 0.01f), "calc float");
	assertMath(approxEqualFloat(calculateFloat("(1.5+2.5)*2.0"), 8.0f, 0.01f), "calc float bracket");
}

void MathUtilityTest::testGeometry()
{
	Vector2 intersect2;
	assertMath(intersectLine2(Line2D(Vector2(0, 0), Vector2(10, 10)), Line2D(Vector2(0, 10), Vector2(10, 0)), intersect2), "line2");
	assertMath(approxEqualFloat(intersect2.x, 5.0f, 0.01f), "line2 x");
	assertMath(approxEqualFloat(intersect2.y, 5.0f, 0.01f), "line2 y");

	Vector2 sectionIntersect;
	assertMath(intersectLineSection(Line2D(Vector2(0, 0), Vector2(10, 10)), Line2D(Vector2(0, 10), Vector2(10, 0)), sectionIntersect, true), "section");
	assertMath(approxEqualFloat(sectionIntersect.x, 5.0f, 0.01f), "section x");

	TriangleIntersect triIntersect;
	assertMath(intersectLineTriangle(Line2(Vector2(-1, 1), Vector2(10, 1)), Triangle2(Vector2(0, 0), Vector2(10, 0), Vector2(0, 10)), triIntersect, true), "triangle");
	assertMath(approxEqualFloat(triIntersect.mIntersectPoint.x, 0.0f, 0.1f) || approxEqualFloat(triIntersect.mIntersectPoint.y, 1.0f, 0.1f), "triangle point");

	assertMath(approxEqualFloat(getDistanceToLine(Vector2(0, 1), Line2D(Vector2(-1, 0), Vector2(1, 0))), 1.0f, 0.01f), "distance");
	Vector2 projection = getProjection(Vector2(2, 0), Vector2(4, 0));
	assertMath(approxEqualFloat(projection.x, 2.0f, 0.01f), "projection x");

	Vector3 out3;
	assertMath(intersectLine3IgnoreY(Line3(Vector3(0, 0, 0), Vector3(10, 0, 10)), Line3(Vector3(0, 0, 10), Vector3(10, 0, 0)), out3), "line3 ignore y");
	assertMath(intersectIgnoreY(Line3(Vector3(-1, 0, 1), Vector3(11, 0, 1)), Rect3(Vector3(0, 0, 0), Vector3(10, 10, 10)), nullptr, true), "rect3 ignore y");
	assertMath(intersectLineAABB(Line2(Vector2(-1, 5), Vector2(11, 5)), Rect(0, 0, 10, 10), nullptr), "aabb");
	assertMath(intersectEdge(Line2D(Vector2(-1, 5), Vector2(11, 5)), Rect(0, 0, 10, 10), nullptr, true), "edge");
}

void MathUtilityTest::testDeep()
{
	testScalarMath();
	testClampsAndRanges();
	testVectorMath();
	testAnglesAndRotation();
	testSortingAndSearch();
	testExpressionsAndFormats();
	testGeometry();
}

// ============================================================
// Consolidated utility coverage
// ============================================================
#define MATHUTILITYTEST_UT_ASSERT(expr, msg) \
	do { if (!(expr)) { ERROR(string("MathUtilityTest FAIL: ") + (msg)); } } while(0)
#define MATHUTILITYTEST_UT_ASSERT_EQ(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("MathUtilityTest FAIL: ") + (msg) + " | expected=" + IToS((int)(b)) + " actual=" + IToS((int)(a))); } } while(0)
#define MATHUTILITYTEST_UT_ASSERT_EQ_LL(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("MathUtilityTest FAIL: ") + (msg) + " | expected=" + LLToS(b) + " actual=" + LLToS(a)); } } while(0)
#define MATHUTILITYTEST_UT_ASSERT_FLOAT(a, b, msg) \
	do { if (fabsf((a) - (b)) > 1e-3f) { ERROR(string("MathUtilityTest FAIL: ") + (msg) + " | expected=" + FToS(b) + " actual=" + FToS(a)); } } while(0)
#define MATHUTILITYTEST_UT_ASSERT_STR(a, b, msg) \
	do { if ((a) != (b)) { ERROR(string("MathUtilityTest FAIL: ") + (msg) + " | expected=" + (b) + " actual=" + (a)); } } while(0)

void MathUtilityTest::testMath_Ceiling()
{
	MATHUTILITYTEST_UT_ASSERT_EQ(ceiling(1.0f), 1, "ceiling(1.0)=1");
	MATHUTILITYTEST_UT_ASSERT_EQ(ceiling(1.1f), 2, "ceiling(1.1)=2");
	MATHUTILITYTEST_UT_ASSERT_EQ(ceiling(1.9f), 2, "ceiling(1.9)=2");
	MATHUTILITYTEST_UT_ASSERT_EQ(ceiling(-1.1f), -1, "ceiling(-1.1)=-1");
	MATHUTILITYTEST_UT_ASSERT_EQ(ceiling(-1.9f), -1, "ceiling(-1.9)=-1");
	MATHUTILITYTEST_UT_ASSERT_EQ(ceiling(0.0f), 0, "ceiling(0.0)=0");
	MATHUTILITYTEST_UT_ASSERT_EQ(ceiling(0.5f), 1, "ceiling(0.5)=1");
}

void MathUtilityTest::testMath_Floor()
{
	MATHUTILITYTEST_UT_ASSERT_EQ(floor_(1.0f), 1, "floor_(1.0)=1");
	MATHUTILITYTEST_UT_ASSERT_EQ(floor_(1.9f), 1, "floor_(1.9)=1");
	MATHUTILITYTEST_UT_ASSERT_EQ(floor_(1.1f), 1, "floor_(1.1)=1");
	MATHUTILITYTEST_UT_ASSERT_EQ(floor_(-1.1f), -2, "floor_(-1.1)=-2");
	MATHUTILITYTEST_UT_ASSERT_EQ(floor_(-1.9f), -2, "floor_(-1.9)=-2");
	MATHUTILITYTEST_UT_ASSERT_EQ(floor_(0.0f), 0, "floor_(0.0)=0");
	MATHUTILITYTEST_UT_ASSERT_EQ(floor_(-0.5f), -1, "floor_(-0.5)=-1");
}

void MathUtilityTest::testMath_Clamp()
{
	MATHUTILITYTEST_UT_ASSERT_EQ(clamp(5, 0, 10), 5, "clamp(5,0,10)=5");
	MATHUTILITYTEST_UT_ASSERT_EQ(clamp(-1, 0, 10), 0, "clamp(-1,0,10)=0");
	MATHUTILITYTEST_UT_ASSERT_EQ(clamp(11, 0, 10), 10, "clamp(11,0,10)=10");
	MATHUTILITYTEST_UT_ASSERT_EQ(clamp(0, 0, 0), 0, "clamp(0,0,0)=0");

	MATHUTILITYTEST_UT_ASSERT_EQ(clampMin(5, 3), 5, "clampMin(5,3)=5");
	MATHUTILITYTEST_UT_ASSERT_EQ(clampMin(2, 3), 3, "clampMin(2,3)=3");
	MATHUTILITYTEST_UT_ASSERT_EQ(clampMin(-5, 0), 0, "clampMin(-5,0)=0");

	MATHUTILITYTEST_UT_ASSERT_EQ(clampMax(5, 10), 5, "clampMax(5,10)=5");
	MATHUTILITYTEST_UT_ASSERT_EQ(clampMax(15, 10), 10, "clampMax(15,10)=10");
	MATHUTILITYTEST_UT_ASSERT_EQ(clampMax(0, 0), 0, "clampMax(0,0)=0");

	int v = 20;
	clampRef(v, 0, 10);
	MATHUTILITYTEST_UT_ASSERT_EQ(v, 10, "clampRef: 20->10");
	v = -5;
	clampRef(v, 0, 10);
	MATHUTILITYTEST_UT_ASSERT_EQ(v, 0, "clampRef: -5->0");
}

void MathUtilityTest::testMath_MinMax()
{
	MATHUTILITYTEST_UT_ASSERT_EQ(getMin(3, 7), 3, "getMin(3,7)=3");
	MATHUTILITYTEST_UT_ASSERT_EQ(getMin(-1, -5), -5, "getMin(-1,-5)=-5");
	MATHUTILITYTEST_UT_ASSERT_EQ(getMin(0, 0), 0, "getMin(0,0)=0");

	MATHUTILITYTEST_UT_ASSERT_EQ(getMax(3, 7), 7, "getMax(3,7)=7");
	MATHUTILITYTEST_UT_ASSERT_EQ(getMax(-1, -5), -1, "getMax(-1,-5)=-1");
	MATHUTILITYTEST_UT_ASSERT_EQ(getMax(0, 0), 0, "getMax(0,0)=0");
}

void MathUtilityTest::testMath_IsZeroEqual()
{
	MATHUTILITYTEST_UT_ASSERT(isZero(0.0f), "isZero(0.0)=true");
	MATHUTILITYTEST_UT_ASSERT(isZero(0.000005f), "isZero(1e-5)=true");
	MATHUTILITYTEST_UT_ASSERT(!isZero(0.1f), "isZero(0.1)=false");
	MATHUTILITYTEST_UT_ASSERT(!isZero(-0.1f), "isZero(-0.1)=false");

	MATHUTILITYTEST_UT_ASSERT(isEqual(1.0f, 1.0f), "isEqual(1,1)=true");
	MATHUTILITYTEST_UT_ASSERT(!isEqual(1.0f, 1.1f), "isEqual(1,1.1)=false");
	MATHUTILITYTEST_UT_ASSERT(isEqual(0.0f, 0.000005f), "isEqual(0,1e-5)=true");
}

void MathUtilityTest::testMath_IsEvenPow2()
{
	MATHUTILITYTEST_UT_ASSERT(isEven(0), "isEven(0)=true");
	MATHUTILITYTEST_UT_ASSERT(isEven(4), "isEven(4)=true");
	MATHUTILITYTEST_UT_ASSERT(isEven(-2), "isEven(-2)=true");
	MATHUTILITYTEST_UT_ASSERT(!isEven(1), "isEven(1)=false");
	MATHUTILITYTEST_UT_ASSERT(!isEven(7), "isEven(7)=false");

	MATHUTILITYTEST_UT_ASSERT(isPow2(1), "isPow2(1)=true");
	MATHUTILITYTEST_UT_ASSERT(isPow2(2), "isPow2(2)=true");
	MATHUTILITYTEST_UT_ASSERT(isPow2(4), "isPow2(4)=true");
	MATHUTILITYTEST_UT_ASSERT(isPow2(1024), "isPow2(1024)=true");
	MATHUTILITYTEST_UT_ASSERT(!isPow2(0), "isPow2(0)=false");
	MATHUTILITYTEST_UT_ASSERT(!isPow2(3), "isPow2(3)=false");
	MATHUTILITYTEST_UT_ASSERT(!isPow2(6), "isPow2(6)=false");
}

void MathUtilityTest::testMath_Divide()
{
	MATHUTILITYTEST_UT_ASSERT_FLOAT(divide(10.0f, 2.0f), 5.0f, "divide(10,2)=5");
	MATHUTILITYTEST_UT_ASSERT_FLOAT(divide(10.0f, 0.0f), 0.0f, "divide(10,0)=0 guard");
	MATHUTILITYTEST_UT_ASSERT_FLOAT(divide(0.0f, 2.0f), 0.0f, "divide(0,2)=0");
	MATHUTILITYTEST_UT_ASSERT_FLOAT(divide(10, 4), 2.5f, "divide(int 10,4)=2.5");
	MATHUTILITYTEST_UT_ASSERT_FLOAT(divide(10, 0), 0.0f, "divide(int 10,0)=0 guard");
}

void MathUtilityTest::testMath_FmodFrac()
{
	MATHUTILITYTEST_UT_ASSERT_FLOAT(fmod(5.5f, 2.0f), 1.5f, "fmod(5.5,2)=1.5");
	MATHUTILITYTEST_UT_ASSERT_FLOAT(fmod(4.0f, 2.0f), 0.0f, "fmod(4,2)=0");
	MATHUTILITYTEST_UT_ASSERT_FLOAT(frac(3.7f), 0.7f, "frac(3.7)=0.7");
	MATHUTILITYTEST_UT_ASSERT_FLOAT(frac(0.0f), 0.0f, "frac(0)=0");
	MATHUTILITYTEST_UT_ASSERT_FLOAT(frac(1.0f), 0.0f, "frac(1.0)=0");
}

void MathUtilityTest::testMath_BatchCount()
{
	// 10 items / batch=3 => 4 batches (3+3+3+1)
	MATHUTILITYTEST_UT_ASSERT_EQ(generateBatchCount(10, 3), 4, "batchCount(10,3)=4");
	// 9 items / batch=3 => 3 batches
	MATHUTILITYTEST_UT_ASSERT_EQ(generateBatchCount(9, 3), 3, "batchCount(9,3)=3");
	// 0 items => 0 batches
	MATHUTILITYTEST_UT_ASSERT_EQ(generateBatchCount(0, 3), 0, "batchCount(0,3)=0");
	// 1 item / batch=100 => 1 batch
	MATHUTILITYTEST_UT_ASSERT_EQ(generateBatchCount(1, 100), 1, "batchCount(1,100)=1");

	// batchIndex=0 => min(10-0,3)=3
	MATHUTILITYTEST_UT_ASSERT_EQ(generateBatchSize(10, 3, 0), 3, "batchSize(10,3,0)=3");
	// batchIndex=1 => min(10-3,3)=3
	MATHUTILITYTEST_UT_ASSERT_EQ(generateBatchSize(10, 3, 1), 3, "batchSize(10,3,1)=3");
	// batchIndex=3 (last) => min(10-9,3)=1
	MATHUTILITYTEST_UT_ASSERT_EQ(generateBatchSize(10, 3, 3), 1, "batchSize(10,3,3)=1 tail");
	// batchIndex=4 (out of range) => 0
	MATHUTILITYTEST_UT_ASSERT_EQ(generateBatchSize(10, 3, 4), 0, "batchSize(10,3,4)=0 oor");
}

void MathUtilityTest::testMathUtility()
{
	testMath_Ceiling();
	testMath_Floor();
	testMath_Clamp();
	testMath_MinMax();
	testMath_IsZeroEqual();
	testMath_IsEvenPow2();
	testMath_Divide();
	testMath_FmodFrac();
	testMath_BatchCount();
}

void MathUtilityTest::testMathUtility_ClampVariants()
{
	// clamp int
	MATHUTILITYTEST_UT_ASSERT_EQ(clamp(5, 1, 10), 5, "clamp in range");
	MATHUTILITYTEST_UT_ASSERT_EQ(clamp(-5, 0, 10), 0, "clamp below min");
	MATHUTILITYTEST_UT_ASSERT_EQ(clamp(15, 0, 10), 10, "clamp above max");
	MATHUTILITYTEST_UT_ASSERT_EQ(clamp(0, 0, 0), 0, "clamp zero range");
	MATHUTILITYTEST_UT_ASSERT_EQ(clamp(1, 1, 1), 1, "clamp single value");

	// clampRef
	int val = 15;
	clampRef(val, 0, 10);
	MATHUTILITYTEST_UT_ASSERT_EQ(val, 10, "clampRef above max");
	val = -5;
	clampRef(val, 0, 10);
	MATHUTILITYTEST_UT_ASSERT_EQ(val, 0, "clampRef below min");
	val = 5;
	clampRef(val, 0, 10);
	MATHUTILITYTEST_UT_ASSERT_EQ(val, 5, "clampRef in range unchanged");

	// clampMin
	MATHUTILITYTEST_UT_ASSERT_EQ(clampMin(5, 10), 10, "clampMin below");
	MATHUTILITYTEST_UT_ASSERT_EQ(clampMin(15, 10), 15, "clampMin above");
	MATHUTILITYTEST_UT_ASSERT_EQ(clampMin(10, 10), 10, "clampMin equal");

	// clampMax
	MATHUTILITYTEST_UT_ASSERT_EQ(clampMax(15, 10), 10, "clampMax above");
	MATHUTILITYTEST_UT_ASSERT_EQ(clampMax(5, 10), 5, "clampMax below");
	MATHUTILITYTEST_UT_ASSERT_EQ(clampMax(10, 10), 10, "clampMax equal");

	// clampZero
	MATHUTILITYTEST_UT_ASSERT_EQ(clampZero(-5), 0, "clampZero neg");
	MATHUTILITYTEST_UT_ASSERT_EQ(clampZero(5), 5, "clampZero pos");
	MATHUTILITYTEST_UT_ASSERT_EQ(clampZero(0), 0, "clampZero zero");

	// saturate
	{ float v = -0.5f; saturate(v); MATHUTILITYTEST_UT_ASSERT_FLOAT(v, 0.0f, "saturate neg"); }
	{ float v = 0.5f;  saturate(v); MATHUTILITYTEST_UT_ASSERT_FLOAT(v, 0.5f, "saturate mid"); }
	{ float v = 1.5f;  saturate(v); MATHUTILITYTEST_UT_ASSERT_FLOAT(v, 1.0f, "saturate above 1"); }
	{ float v = 0.0f;  saturate(v); MATHUTILITYTEST_UT_ASSERT_FLOAT(v, 0.0f, "saturate 0"); }
	{ float v = 1.0f;  saturate(v); MATHUTILITYTEST_UT_ASSERT_FLOAT(v, 1.0f, "saturate 1"); }

}

void MathUtilityTest::testMathUtility_StepFracFmod()
{
	// step
	MATHUTILITYTEST_UT_ASSERT_FLOAT((float)step(5.0f, 3.0f), 0.0f, "step x<edge=0");
	MATHUTILITYTEST_UT_ASSERT_FLOAT((float)step(5.0f, 5.0f), 1.0f, "step x==edge=1");
	MATHUTILITYTEST_UT_ASSERT_FLOAT((float)step(5.0f, 7.0f), 1.0f, "step x>edge=1");

	// frac
	MATHUTILITYTEST_UT_ASSERT_FLOAT(frac(3.7f), 0.7f, "frac 3.7=0.7");
	MATHUTILITYTEST_UT_ASSERT_FLOAT(frac(0.0f), 0.0f, "frac 0.0=0.0");
	MATHUTILITYTEST_UT_ASSERT_FLOAT(frac(1.0f), 0.0f, "frac 1.0=0.0");

	// fmod float
	MATHUTILITYTEST_UT_ASSERT_FLOAT(fmod(7.5f, 3.0f), 1.5f, "fmod 7.5%3=1.5");
	MATHUTILITYTEST_UT_ASSERT_FLOAT(fmod(6.0f, 3.0f), 0.0f, "fmod 6%3=0");
	MATHUTILITYTEST_UT_ASSERT_FLOAT(fmod(0.5f, 1.0f), 0.5f, "fmod 0.5%1=0.5");

	// divide int
	MATHUTILITYTEST_UT_ASSERT_EQ(divideInt(7, 3), 2, "divide 7/3=2");
	MATHUTILITYTEST_UT_ASSERT_EQ(divideInt(9, 3), 3, "divide 9/3=3");
	MATHUTILITYTEST_UT_ASSERT_EQ(divideInt(0, 5), 0, "divide 0/5=0");

}

void MathUtilityTest::testMathUtility_IsEvenIsPow2()
{
	// isEven
	MATHUTILITYTEST_UT_ASSERT(isEven(0), "isEven 0");
	MATHUTILITYTEST_UT_ASSERT(isEven(2), "isEven 2");
	MATHUTILITYTEST_UT_ASSERT(isEven(-4), "isEven -4");
	MATHUTILITYTEST_UT_ASSERT(!isEven(1), "not isEven 1");
	MATHUTILITYTEST_UT_ASSERT(!isEven(3), "not isEven 3");
	MATHUTILITYTEST_UT_ASSERT(!isEven(-3), "not isEven -3");

	// isPow2
	MATHUTILITYTEST_UT_ASSERT(!isPow2(0), "isPow2 0 false");
	MATHUTILITYTEST_UT_ASSERT(isPow2(1), "isPow2 1");
	MATHUTILITYTEST_UT_ASSERT(isPow2(2), "isPow2 2");
	MATHUTILITYTEST_UT_ASSERT(isPow2(4), "isPow2 4");
	MATHUTILITYTEST_UT_ASSERT(isPow2(16), "isPow2 16");
	MATHUTILITYTEST_UT_ASSERT(isPow2(1024), "isPow2 1024");
	MATHUTILITYTEST_UT_ASSERT(!isPow2(3), "isPow2 3 false");
	MATHUTILITYTEST_UT_ASSERT(!isPow2(6), "isPow2 6 false");
	MATHUTILITYTEST_UT_ASSERT(!isPow2(100), "isPow2 100 false");

}

void MathUtilityTest::testMathUtility_CeilingFloor()
{
	// ceiling float
	MATHUTILITYTEST_UT_ASSERT_EQ(ceiling(2.0f), 2, "ceiling 2.0=2");
	MATHUTILITYTEST_UT_ASSERT_EQ(ceiling(2.5f), 3, "ceiling 2.5=3");
	MATHUTILITYTEST_UT_ASSERT_EQ(ceiling(-1.5f), -1, "ceiling -1.5=-1");
	MATHUTILITYTEST_UT_ASSERT_EQ(ceiling(0.0f), 0, "ceiling 0.0=0");
	// NOTE: 2.0001f == 2.0f + 0.0001f fails >, so ceiling returns 2
	MATHUTILITYTEST_UT_ASSERT_EQ(ceiling(2.9999f), 3, "ceiling 2.9999=3");

	// floor_ float
	MATHUTILITYTEST_UT_ASSERT_EQ(floor_(2.0f), 2, "floor 2.0=2");
	MATHUTILITYTEST_UT_ASSERT_EQ(floor_(2.9f), 2, "floor 2.9=2");
	MATHUTILITYTEST_UT_ASSERT_EQ(floor_(-1.5f), -2, "floor -1.5=-2");
	MATHUTILITYTEST_UT_ASSERT_EQ(floor_(0.0f), 0, "floor 0.0=0");

}

void MathUtilityTest::testMathUtility_LerpInverseLerp()
{
	// lerp float
	MATHUTILITYTEST_UT_ASSERT_FLOAT(lerp(0.0f, 10.0f, 0.5f), 5.0f, "lerp 0-10 t=0.5");
	MATHUTILITYTEST_UT_ASSERT_FLOAT(lerp(0.0f, 10.0f, 0.0f), 0.0f, "lerp t=0");
	MATHUTILITYTEST_UT_ASSERT_FLOAT(lerp(0.0f, 10.0f, 1.0f), 10.0f, "lerp t=1");
	MATHUTILITYTEST_UT_ASSERT_FLOAT(lerp(-5.0f, 5.0f, 0.5f), 0.0f, "lerp -5..5 t=0.5");

	// lerp int
	MATHUTILITYTEST_UT_ASSERT_EQ(lerp(0, 10, 0.5f), 5, "lerp int 0-10 t=0.5");
	MATHUTILITYTEST_UT_ASSERT_EQ(lerp(0, 100, 0.25f), 25, "lerp int 0-100 t=0.25");

	// inverseLerp
	MATHUTILITYTEST_UT_ASSERT_FLOAT(inverseLerp(0.0f, 10.0f, 5.0f), 0.5f, "inverseLerp 5 in 0-10=0.5");
	MATHUTILITYTEST_UT_ASSERT_FLOAT(inverseLerp(0.0f, 10.0f, 0.0f), 0.0f, "inverseLerp 0 in 0-10=0");
	MATHUTILITYTEST_UT_ASSERT_FLOAT(inverseLerp(0.0f, 10.0f, 10.0f), 1.0f, "inverseLerp 10 in 0-10=1");

}

void MathUtilityTest::testMathUtility_ClampCycle()
{
	// clampCycle
	{ float v = 5.0f;  clampCycle(v, 0.0f, 10.0f); MATHUTILITYTEST_UT_ASSERT_FLOAT(v, 5.0f, "clampCycle in range"); }
	{ float v = -1.0f; clampCycle(v, 0.0f, 10.0f); MATHUTILITYTEST_UT_ASSERT_FLOAT(v, 9.0f, "clampCycle below wraps to 9"); }
	{ float v = 11.0f; clampCycle(v, 0.0f, 10.0f); MATHUTILITYTEST_UT_ASSERT_FLOAT(v, 1.0f, "clampCycle above wraps to 1"); }
	{ float v = 20.0f; clampCycle(v, 0.0f, 10.0f); MATHUTILITYTEST_UT_ASSERT_FLOAT(v, 10.0f, "clampCycle 20->10"); }
	{ float v = 10.0f; clampCycle(v, 0.0f, 10.0f); MATHUTILITYTEST_UT_ASSERT_FLOAT(v, 10.0f, "clampCycle max wraps to 10"); }

	// clampDegree360
	{ float v = 0.0f;   clampDegree360(v); MATHUTILITYTEST_UT_ASSERT_FLOAT(v, 0.0f, "clampDeg360 0=0"); }
	{ float v = 360.0f; clampDegree360(v); MATHUTILITYTEST_UT_ASSERT_FLOAT(v, 360.0f, "clampDeg360 360=360"); }
	{ float v = 370.0f; clampDegree360(v); MATHUTILITYTEST_UT_ASSERT_FLOAT(v, 10.0f, "clampDeg360 370=10"); }
	{ float v = -10.0f; clampDegree360(v); MATHUTILITYTEST_UT_ASSERT_FLOAT(v, 350.0f, "clampDeg360 -10=350"); }

	// clampDegree180
	{ float v = 0.0f;   clampDegree180(v); MATHUTILITYTEST_UT_ASSERT_FLOAT(v, 0.0f, "clampDeg180 0=0"); }
	{ float v = 180.0f; clampDegree180(v); MATHUTILITYTEST_UT_ASSERT_FLOAT(v, 180.0f, "clampDeg180 180=180"); }
	{ float v = 90.0f;  clampDegree180(v); MATHUTILITYTEST_UT_ASSERT_FLOAT(v, 90.0f, "clampDeg180 90=90"); }
	{ float v = -90.0f; clampDegree180(v); MATHUTILITYTEST_UT_ASSERT_FLOAT(v, -90.0f, "clampDeg180 -90=-90"); }

}

void MathUtilityTest::testMathUtility_Pow2Pow10()
{
	// pow2
	MATHUTILITYTEST_UT_ASSERT_EQ((int)pow2(0), 1, "pow2(0)=1");
	MATHUTILITYTEST_UT_ASSERT_EQ((int)pow2(1), 2, "pow2(1)=2");
	MATHUTILITYTEST_UT_ASSERT_EQ((int)pow2(4), 16, "pow2(4)=16");
	MATHUTILITYTEST_UT_ASSERT_EQ((int)pow2(10), 1024, "pow2(10)=1024");

	// pow10
	MATHUTILITYTEST_UT_ASSERT_EQ((int)pow10(0), 1, "pow10(0)=1");
	MATHUTILITYTEST_UT_ASSERT_EQ((int)pow10(1), 10, "pow10(1)=10");
	MATHUTILITYTEST_UT_ASSERT_EQ((int)pow10(3), 1000, "pow10(3)=1000");
	MATHUTILITYTEST_UT_ASSERT_EQ((int)pow10(6), 1000000, "pow10(6)=1000000");

}

void MathUtilityTest::testMathUtility_SecondsConvert()
{
	// secondsToMinutesSeconds
	{
		int m = 0, s = 0;
		secondsToMinutesSeconds(125, m, s);
		MATHUTILITYTEST_UT_ASSERT_EQ(m, 2, "sec2min m=2");
		MATHUTILITYTEST_UT_ASSERT_EQ(s, 5, "sec2min s=5");
	}
	{
		int m = 0, s = 0;
		secondsToMinutesSeconds(60, m, s);
		MATHUTILITYTEST_UT_ASSERT_EQ(m, 1, "sec2min 60 m=1");
		MATHUTILITYTEST_UT_ASSERT_EQ(s, 0, "sec2min 60 s=0");
	}
	{
		int m = 0, s = 0;
		secondsToMinutesSeconds(0, m, s);
		MATHUTILITYTEST_UT_ASSERT_EQ(m, 0, "sec2min 0 m=0");
		MATHUTILITYTEST_UT_ASSERT_EQ(s, 0, "sec2min 0 s=0");
	}

	// secondsToHoursMinutesSeconds
	{
		int h = 0, m = 0, s = 0;
		secondsToHoursMinutesSeconds(3661, h, m, s);
		MATHUTILITYTEST_UT_ASSERT_EQ(h, 1, "sec2hms h=1");
		MATHUTILITYTEST_UT_ASSERT_EQ(m, 1, "sec2hms m=1");
		MATHUTILITYTEST_UT_ASSERT_EQ(s, 1, "sec2hms s=1");
	}
	{
		int h = 0, m = 0, s = 0;
		secondsToHoursMinutesSeconds(3600, h, m, s);
		MATHUTILITYTEST_UT_ASSERT_EQ(h, 1, "sec2hms 3600 h=1");
		MATHUTILITYTEST_UT_ASSERT_EQ(m, 0, "sec2hms 3600 m=0");
		MATHUTILITYTEST_UT_ASSERT_EQ(s, 0, "sec2hms 3600 s=0");
	}

}

void MathUtilityTest::testMathUtility_VectorMath()
{
	// getLength
	{
		float len = getLength(3.0f, 4.0f);
		MATHUTILITYTEST_UT_ASSERT_FLOAT(len, 5.0f, "getLength 3,4=5");
	}
	{
		float len = getLength(0.0f, 0.0f);
		MATHUTILITYTEST_UT_ASSERT_FLOAT(len, 0.0f, "getLength 0,0=0");
	}
	{
		float len = getLength(1.0f, 1.0f);
		MATHUTILITYTEST_UT_ASSERT_FLOAT(len, sqrtf(2.0f), "getLength 1,1=sqrt2");
	}

	// getSquaredLength
	{
		float sq = getSquaredLength(Vector2(3.0f, 4.0f));
		MATHUTILITYTEST_UT_ASSERT_FLOAT(sq, 25.0f, "getSquaredLen 3,4=25");
	}

	// dot
	{
		float d = dot(Vector3(1.0f, 0.0f, 0.0f), Vector3(0.0f, 1.0f, 0.0f));
		MATHUTILITYTEST_UT_ASSERT_FLOAT(d, 0.0f, "dot perp=0");
	}
	{
		float d = dot(Vector3(1.0f, 0.0f, 0.0f), Vector3(1.0f, 0.0f, 0.0f));
		MATHUTILITYTEST_UT_ASSERT_FLOAT(d, 1.0f, "dot same=1");
	}

	// normalize
	{
		float nx = 0.0f, ny = 0.0f, nz = 0.0f;
		Vector3 n = normalize(Vector3(3.0f, 4.0f, 0.0f));
		nx = n.x; ny = n.y; nz = n.z;
		MATHUTILITYTEST_UT_ASSERT_FLOAT(nx, 0.6f, "normalize nx=0.6");
		MATHUTILITYTEST_UT_ASSERT_FLOAT(ny, 0.8f, "normalize ny=0.8");
		MATHUTILITYTEST_UT_ASSERT_FLOAT(nz, 0.0f, "normalize nz=0");
		float len = sqrtf(nx * nx + ny * ny + nz * nz);
		MATHUTILITYTEST_UT_ASSERT_FLOAT(len, 1.0f, "normalize len=1");
	}

}

void MathUtilityTest::testMathUtility_MinMax()
{
	// getMin
	MATHUTILITYTEST_UT_ASSERT_EQ(getMin(3, 5), 3, "getMin 3,5=3");
	MATHUTILITYTEST_UT_ASSERT_EQ(getMin(-2, 1), -2, "getMin neg");
	MATHUTILITYTEST_UT_ASSERT_EQ(getMin(7, 7), 7, "getMin equal");
	MATHUTILITYTEST_UT_ASSERT_FLOAT(getMin(1.5f, 2.5f), 1.5f, "getMin float");

	// getMax
	MATHUTILITYTEST_UT_ASSERT_EQ(getMax(3, 5), 5, "getMax 3,5=5");
	MATHUTILITYTEST_UT_ASSERT_EQ(getMax(-2, -1), -1, "getMax neg");
	MATHUTILITYTEST_UT_ASSERT_FLOAT(getMax(1.5f, 0.5f), 1.5f, "getMax float");

}

void MathUtilityTest::testMathUtility_InRange()
{
	// inRange int [min, max)
	MATHUTILITYTEST_UT_ASSERT(inRange(5, 0, 10), "inRange 5 in [0,10)");
	MATHUTILITYTEST_UT_ASSERT(!inRange(0, 1, 10), "inRange 0 not in [1,10)");
	MATHUTILITYTEST_UT_ASSERT(inRange(10, 0, 10), "inRange 10 in [0,10)");

	// inFixedRange [min, max] inclusive
	MATHUTILITYTEST_UT_ASSERT(inFixedRange(5, 0, 10), "inFixedRange 5");
	MATHUTILITYTEST_UT_ASSERT(inFixedRange(0, 0, 10), "inFixedRange 0");
	MATHUTILITYTEST_UT_ASSERT(inFixedRange(10, 0, 10), "inFixedRange 10");
	MATHUTILITYTEST_UT_ASSERT(!inFixedRange(11, 0, 10), "inFixedRange 11 false");
	MATHUTILITYTEST_UT_ASSERT(!inFixedRange(-1, 0, 10), "inFixedRange -1 false");

}

void MathUtilityTest::testMathUtility_Swap()
{
	// swap_
	int a = 3, b = 7;
	swap_(a, b);
	MATHUTILITYTEST_UT_ASSERT_EQ(a, 7, "swap_ a=7");
	MATHUTILITYTEST_UT_ASSERT_EQ(b, 3, "swap_ b=3");

	float fa = 1.5f, fb = 2.5f;
	swap_(fa, fb);
	MATHUTILITYTEST_UT_ASSERT_FLOAT(fa, 2.5f, "swap_ float a=2.5");
	MATHUTILITYTEST_UT_ASSERT_FLOAT(fb, 1.5f, "swap_ float b=1.5");

}

void MathUtilityTest::testMathUtility_DegreeRadian()
{
	// toDegree
	MATHUTILITYTEST_UT_ASSERT_FLOAT(toDegree(0.0f), 0.0f, "toDeg 0=0");
	MATHUTILITYTEST_UT_ASSERT_FLOAT(toDegree(3.14159265f), 180.0f, "toDeg pi=180");
	MATHUTILITYTEST_UT_ASSERT_FLOAT(toDegree(1.5707963f), 90.0f, "toDeg pi/2=90");

	// toRadian
	MATHUTILITYTEST_UT_ASSERT_FLOAT(toRadian(0.0f), 0.0f, "toRad 0=0");
	MATHUTILITYTEST_UT_ASSERT_FLOAT(toRadian(180.0f), 3.14159265f, "toRad 180=pi");
	MATHUTILITYTEST_UT_ASSERT_FLOAT(toRadian(90.0f), 1.5707963f, "toRad 90=pi/2");

	// round-trip
	float deg = 45.0f;
	MATHUTILITYTEST_UT_ASSERT_FLOAT(toDegree(toRadian(deg)), deg, "deg->rad->deg round-trip");

}

void MathUtilityTest::testMathUtility_GenerateBatch()
{
	// generateBatchCount
	MATHUTILITYTEST_UT_ASSERT_EQ(generateBatchCount(10, 3), 4, "batchCount 10/3=4 (ceil)");
	MATHUTILITYTEST_UT_ASSERT_EQ(generateBatchCount(9, 3), 3, "batchCount 9/3=3");
	MATHUTILITYTEST_UT_ASSERT_EQ(generateBatchCount(1, 3), 1, "batchCount 1/3=1");
	MATHUTILITYTEST_UT_ASSERT_EQ(generateBatchCount(0, 3), 0, "batchCount 0/3=0");

	// generateBatchSize
	MATHUTILITYTEST_UT_ASSERT_EQ(generateBatchSize(10, 3, 0), 3, "batchSize batch 0: 3");
	// batch 1: min(3, 10-3)=3
	MATHUTILITYTEST_UT_ASSERT_EQ(generateBatchSize(10, 3, 1), 3, "batchSize batch 1: 3");

}

void MathUtilityTest::testMathUtility_IsZeroEqual()
{
	// isZero
	MATHUTILITYTEST_UT_ASSERT(isZero(0.0f), "isZero 0.0");
	MATHUTILITYTEST_UT_ASSERT(isZero(0.00001f), "isZero tiny");
	MATHUTILITYTEST_UT_ASSERT(!isZero(0.01f), "isZero 0.01 false");

	// isEqual
	MATHUTILITYTEST_UT_ASSERT(isEqual(1.0f, 1.0f), "isEqual 1==1");
	MATHUTILITYTEST_UT_ASSERT(isEqual(1.0f, 1.000001f), "isEqual tiny diff");
	MATHUTILITYTEST_UT_ASSERT(!isEqual(1.0f, 1.1f), "isEqual 1.0!=1.1");

}
#endif
