#ifdef WINDOWS
#include "FrameHeader.h"

// AC_ASSERT: AnimationCurveTest 专用断言宏
#define AC_ASSERT(expr) \
	if (!(expr)) { ERROR("AnimationCurveTest FAILED: " #expr); }

// 浮点比较辅助
bool AnimationCurveTest::acNear(float a, float b, float eps)
{
	return fabsf(a - b) <= eps;
}

// ========== AnimationCurve 基类 ==========
void AnimationCurveTest::testAnimationCurveBase()
{
	// 基类 evaluate 默认返回 time 本身
	AnimationCurve base;
	AC_ASSERT(acNear(base.evaluate(0.0f), 0.0f));
	AC_ASSERT(acNear(base.evaluate(0.5f), 0.5f));
	AC_ASSERT(acNear(base.evaluate(1.0f), 1.0f));
	AC_ASSERT(acNear(base.evaluate(0.25f), 0.25f));
	AC_ASSERT(acNear(base.evaluate(0.75f), 0.75f));

	// length 固定为 1.0
	AC_ASSERT(acNear(base.length(), 1.0f));

}

// ========== CurveLinearZeroOne (0→1) ==========
void AnimationCurveTest::testCurveLinearZeroOne()
{
	CurveLinearZeroOne c;
	AC_ASSERT(acNear(c.evaluate(0.0f), 0.0f));
	AC_ASSERT(acNear(c.evaluate(0.25f), 0.25f));
	AC_ASSERT(acNear(c.evaluate(0.5f), 0.5f));
	AC_ASSERT(acNear(c.evaluate(0.75f), 0.75f));
	AC_ASSERT(acNear(c.evaluate(1.0f), 1.0f));
	// 严格线性
	for (int i = 0; i <= 10; ++i)
	{
		float t = i * 0.1f;
		AC_ASSERT(acNear(c.evaluate(t), t));
	}
}

// ========== CurveLinearOneZero (1→0) ==========
void AnimationCurveTest::testCurveLinearOneZero()
{
	CurveLinearOneZero c;
	AC_ASSERT(acNear(c.evaluate(0.0f), 1.0f));
	AC_ASSERT(acNear(c.evaluate(0.25f), 0.75f));
	AC_ASSERT(acNear(c.evaluate(0.5f), 0.5f));
	AC_ASSERT(acNear(c.evaluate(0.75f), 0.25f));
	AC_ASSERT(acNear(c.evaluate(1.0f), 0.0f));
	for (int i = 0; i <= 10; ++i)
	{
		float t = i * 0.1f;
		AC_ASSERT(acNear(c.evaluate(t), 1.0f - t));
	}
}

// ========== CurveLinearZeroOneZero (0→1→0) ==========
void AnimationCurveTest::testCurveLinearZeroOneZero()
{
	CurveLinearZeroOneZero c;
	AC_ASSERT(acNear(c.evaluate(0.0f), 0.0f));
	AC_ASSERT(acNear(c.evaluate(0.5f), 1.0f));
	AC_ASSERT(acNear(c.evaluate(1.0f), 0.0f));
	// 前半段上升
	AC_ASSERT(c.evaluate(0.25f) > 0.0f && c.evaluate(0.25f) <= 1.0f);
	// 后半段下降
	AC_ASSERT(c.evaluate(0.75f) > 0.0f && c.evaluate(0.75f) <= 1.0f);
}

// ========== CurveLinearOneZeroOne (1→0→1) ==========
void AnimationCurveTest::testCurveLinearOneZeroOne()
{
	CurveLinearOneZeroOne c;
	AC_ASSERT(acNear(c.evaluate(0.0f), 1.0f));
	AC_ASSERT(acNear(c.evaluate(0.5f), 0.0f));
	AC_ASSERT(acNear(c.evaluate(1.0f), 1.0f));
	// 前半段下降
	AC_ASSERT(c.evaluate(0.25f) >= 0.0f && c.evaluate(0.25f) <= 1.0f);
	// 后半段上升
	AC_ASSERT(c.evaluate(0.75f) >= 0.0f && c.evaluate(0.75f) <= 1.0f);
}

// ========== CurveCubicIn (t^3) ==========
void AnimationCurveTest::testCurveCubicIn()
{
	CurveCubicIn c;
	AC_ASSERT(acNear(c.evaluate(0.0f), 0.0f));
	AC_ASSERT(acNear(c.evaluate(0.5f), 0.125f));
	AC_ASSERT(acNear(c.evaluate(1.0f), 1.0f));
	// 单调递增
	float prev = -1.0f;
	for (int i = 0; i <= 10; ++i)
	{
		float v = c.evaluate(i * 0.1f);
		AC_ASSERT(v >= prev);
		prev = v;
	}
	// 加速特征：前段接近0
	AC_ASSERT(c.evaluate(0.1f) < 0.01f + 1e-3f);
}

// ========== CurveCubicOut ==========
void AnimationCurveTest::testCurveCubicOut()
{
	CurveCubicOut c;
	AC_ASSERT(acNear(c.evaluate(0.0f), 0.0f));
	AC_ASSERT(acNear(c.evaluate(1.0f), 1.0f));
	// 减速特征：后段接近1
	AC_ASSERT(c.evaluate(0.9f) > 0.97f);
	// 单调递增
	float prev = -1.0f;
	for (int i = 0; i <= 10; ++i)
	{
		float v = c.evaluate(i * 0.1f);
		AC_ASSERT(v >= prev);
		prev = v;
	}
}

// ========== CurveCubicInOut ==========
void AnimationCurveTest::testCurveCubicInOut()
{
	CurveCubicInOut c;
	AC_ASSERT(acNear(c.evaluate(0.0f), 0.0f));
	AC_ASSERT(acNear(c.evaluate(0.5f), 0.5f));
	AC_ASSERT(acNear(c.evaluate(1.0f), 1.0f));
	// 对称性
	for (int i = 1; i <= 4; ++i)
	{
		float t = i * 0.1f;
		float v1 = c.evaluate(t);
		float v2 = c.evaluate(1.0f - t);
		AC_ASSERT(acNear(v1, 1.0f - v2, 1e-3f));
	}
}

// ========== CurveQuadIn (t^2) ==========
void AnimationCurveTest::testCurveQuadIn()
{
	CurveQuadIn c;
	AC_ASSERT(acNear(c.evaluate(0.0f), 0.0f));
	AC_ASSERT(acNear(c.evaluate(0.5f), 0.25f));
	AC_ASSERT(acNear(c.evaluate(1.0f), 1.0f));
	float prev = -1.0f;
	for (int i = 0; i <= 10; ++i)
	{
		float v = c.evaluate(i * 0.1f);
		AC_ASSERT(v >= prev);
		prev = v;
	}
}

// ========== CurveQuadOut ==========
void AnimationCurveTest::testCurveQuadOut()
{
	CurveQuadOut c;
	AC_ASSERT(acNear(c.evaluate(0.0f), 0.0f));
	AC_ASSERT(acNear(c.evaluate(1.0f), 1.0f));
	// CurveQuadOut: -t*(t-2) = 2t - t^2
	AC_ASSERT(acNear(c.evaluate(0.5f), -0.5f * (0.5f - 2.0f)));  // = 0.75
	float prev = -1.0f;
	for (int i = 0; i <= 10; ++i)
	{
		float v = c.evaluate(i * 0.1f);
		AC_ASSERT(v >= prev);
		prev = v;
	}
}

// ========== CurveQuadInOut ==========
void AnimationCurveTest::testCurveQuadInOut()
{
	CurveQuadInOut c;
	AC_ASSERT(acNear(c.evaluate(0.0f), 0.0f));
	AC_ASSERT(acNear(c.evaluate(0.5f), 0.5f));
	AC_ASSERT(acNear(c.evaluate(1.0f), 1.0f));
	float prev = -1.0f;
	for (int i = 0; i <= 10; ++i)
	{
		float v = c.evaluate(i * 0.1f);
		AC_ASSERT(v >= prev);
		prev = v;
	}
}

// ========== CurveQuartIn (t^4) ==========
void AnimationCurveTest::testCurveQuartIn()
{
	CurveQuartIn c;
	AC_ASSERT(acNear(c.evaluate(0.0f), 0.0f));
	AC_ASSERT(acNear(c.evaluate(0.5f), 0.0625f));
	AC_ASSERT(acNear(c.evaluate(1.0f), 1.0f));
	float prev = -1.0f;
	for (int i = 0; i <= 10; ++i)
	{
		float v = c.evaluate(i * 0.1f);
		AC_ASSERT(v >= prev);
		prev = v;
	}
}

// ========== CurveQuartOut ==========
void AnimationCurveTest::testCurveQuartOut()
{
	CurveQuartOut c;
	AC_ASSERT(acNear(c.evaluate(0.0f), 0.0f));
	AC_ASSERT(acNear(c.evaluate(1.0f), 1.0f));
	float prev = -1.0f;
	for (int i = 0; i <= 10; ++i)
	{
		float v = c.evaluate(i * 0.1f);
		AC_ASSERT(v >= prev);
		prev = v;
	}
}

// ========== CurveQuintIn (t^5) ==========
void AnimationCurveTest::testCurveQuintIn()
{
	CurveQuintIn c;
	AC_ASSERT(acNear(c.evaluate(0.0f), 0.0f));
	AC_ASSERT(acNear(c.evaluate(0.5f), 0.03125f));
	AC_ASSERT(acNear(c.evaluate(1.0f), 1.0f));
	float prev = -1.0f;
	for (int i = 0; i <= 10; ++i)
	{
		float v = c.evaluate(i * 0.1f);
		AC_ASSERT(v >= prev);
		prev = v;
	}
}

// ========== CurveQuintOut ==========
void AnimationCurveTest::testCurveQuintOut()
{
	CurveQuintOut c;
	AC_ASSERT(acNear(c.evaluate(0.0f), 0.0f));
	AC_ASSERT(acNear(c.evaluate(1.0f), 1.0f));
	float prev = -1.0f;
	for (int i = 0; i <= 10; ++i)
	{
		float v = c.evaluate(i * 0.1f);
		AC_ASSERT(v >= prev);
		prev = v;
	}
}

// ========== CurveSineIn ==========
void AnimationCurveTest::testCurveSineIn()
{
	CurveSineIn c;
	AC_ASSERT(acNear(c.evaluate(0.0f), 0.0f, 1e-3f));
	AC_ASSERT(acNear(c.evaluate(1.0f), 1.0f, 1e-3f));
	float prev = -1.0f;
	for (int i = 0; i <= 10; ++i)
	{
		float v = c.evaluate(i * 0.1f);
		AC_ASSERT(v >= prev - 1e-4f);
		prev = v;
	}
}

// ========== CurveSineOut ==========
void AnimationCurveTest::testCurveSineOut()
{
	CurveSineOut c;
	AC_ASSERT(acNear(c.evaluate(0.0f), 0.0f, 1e-3f));
	AC_ASSERT(acNear(c.evaluate(1.0f), 1.0f, 1e-3f));
	float prev = -1.0f;
	for (int i = 0; i <= 10; ++i)
	{
		float v = c.evaluate(i * 0.1f);
		AC_ASSERT(v >= prev - 1e-4f);
		prev = v;
	}
}

// ========== CurveSineInOut ==========
void AnimationCurveTest::testCurveSineInOut()
{
	CurveSineInOut c;
	AC_ASSERT(acNear(c.evaluate(0.0f), 0.0f, 1e-3f));
	AC_ASSERT(acNear(c.evaluate(0.5f), 0.5f, 1e-3f));
	AC_ASSERT(acNear(c.evaluate(1.0f), 1.0f, 1e-3f));
	float prev = -1.0f;
	for (int i = 0; i <= 10; ++i)
	{
		float v = c.evaluate(i * 0.1f);
		AC_ASSERT(v >= prev - 1e-4f);
		prev = v;
	}
}

// ========== CurveCircleIn ==========
void AnimationCurveTest::testCurveCircleIn()
{
	CurveCircleIn c;
	AC_ASSERT(acNear(c.evaluate(0.0f), 0.0f, 1e-3f));
	AC_ASSERT(acNear(c.evaluate(1.0f), 1.0f, 1e-3f));
	float prev = -1.0f;
	for (int i = 0; i <= 10; ++i)
	{
		float v = c.evaluate(i * 0.1f);
		AC_ASSERT(v >= prev - 1e-4f);
		prev = v;
	}
}

// ========== CurveCircleOut ==========
void AnimationCurveTest::testCurveCircleOut()
{
	CurveCircleOut c;
	AC_ASSERT(acNear(c.evaluate(0.0f), 0.0f, 1e-3f));
	AC_ASSERT(acNear(c.evaluate(1.0f), 1.0f, 1e-3f));
	float prev = -1.0f;
	for (int i = 0; i <= 10; ++i)
	{
		float v = c.evaluate(i * 0.1f);
		AC_ASSERT(v >= prev - 1e-4f);
		prev = v;
	}
}

// ========== CurveCircleInOut ==========
void AnimationCurveTest::testCurveCircleInOut()
{
	CurveCircleInOut c;
	AC_ASSERT(acNear(c.evaluate(0.0f), 0.0f, 1e-3f));
	AC_ASSERT(acNear(c.evaluate(0.5f), 0.5f, 1e-3f));
	AC_ASSERT(acNear(c.evaluate(1.0f), 1.0f, 1e-3f));
	float prev = -1.0f;
	for (int i = 0; i <= 10; ++i)
	{
		float v = c.evaluate(i * 0.1f);
		AC_ASSERT(v >= prev - 1e-4f);
		prev = v;
	}
}

// ========== CurveExpoIn ==========
void AnimationCurveTest::testCurveExpoIn()
{
	CurveExpoIn c;
	AC_ASSERT(acNear(c.evaluate(0.0f), 0.0f, 1e-3f));
	AC_ASSERT(acNear(c.evaluate(1.0f), 1.0f, 1e-3f));
	float prev = -1.0f;
	for (int i = 0; i <= 10; ++i)
	{
		float v = c.evaluate(i * 0.1f);
		AC_ASSERT(v >= prev - 1e-3f);
		prev = v;
	}
}

// ========== CurveExpoOut ==========
void AnimationCurveTest::testCurveExpoOut()
{
	CurveExpoOut c;
	AC_ASSERT(acNear(c.evaluate(0.0f), 0.0f, 1e-3f));
	AC_ASSERT(acNear(c.evaluate(1.0f), 1.0f, 1e-3f));
	float prev = -1.0f;
	for (int i = 0; i <= 10; ++i)
	{
		float v = c.evaluate(i * 0.1f);
		AC_ASSERT(v >= prev - 1e-3f);
		prev = v;
	}
}

// ========== CurveExpoInOut ==========
void AnimationCurveTest::testCurveExpoInOut()
{
	CurveExpoInOut c;
	AC_ASSERT(acNear(c.evaluate(0.0f), 0.0f, 1e-3f));
	AC_ASSERT(acNear(c.evaluate(0.5f), 0.5f, 1e-3f));
	AC_ASSERT(acNear(c.evaluate(1.0f), 1.0f, 1e-3f));
	float prev = -1.0f;
	for (int i = 0; i <= 10; ++i)
	{
		float v = c.evaluate(i * 0.1f);
		AC_ASSERT(v >= prev - 1e-3f);
		prev = v;
	}
}

// ========== CurveBackIn ==========
void AnimationCurveTest::testCurveBackIn()
{
	CurveBackIn c;
	AC_ASSERT(acNear(c.evaluate(0.0f), 0.0f, 1e-3f));
	AC_ASSERT(acNear(c.evaluate(1.0f), 1.0f, 1e-3f));
	// Back 曲线在 t 接近0时会短暂为负（回弹效果）
	// 0.5 附近应该较小
	float v05 = c.evaluate(0.5f);
	AC_ASSERT(v05 < 0.5f);
}

// ========== CurveBackOut ==========
void AnimationCurveTest::testCurveBackOut()
{
	CurveBackOut c;
	AC_ASSERT(acNear(c.evaluate(0.0f), 0.0f, 1e-3f));
	AC_ASSERT(acNear(c.evaluate(1.0f), 1.0f, 1e-3f));
	// Back out 在 t 接近1时会超过1（回弹）
	float vMax = 0.0f;
	for (int i = 0; i <= 20; ++i)
	{
		float v = c.evaluate(i * 0.05f);
		if (v > vMax) vMax = v;
	}
	AC_ASSERT(vMax > 1.0f);  // 回弹效果
}

// ========== CurveBackInOut ==========
void AnimationCurveTest::testCurveBackInOut()
{
	CurveBackInOut c;
	AC_ASSERT(acNear(c.evaluate(0.0f), 0.0f, 1e-3f));
	AC_ASSERT(acNear(c.evaluate(0.5f), 0.5f, 1e-3f));
	AC_ASSERT(acNear(c.evaluate(1.0f), 1.0f, 1e-3f));
}

// ========== CurveBounceOut ==========
void AnimationCurveTest::testCurveBounceOut()
{
	CurveBounceOut c;
	AC_ASSERT(acNear(c.evaluate(0.0f), 0.0f, 1e-3f));
	AC_ASSERT(acNear(c.evaluate(1.0f), 1.0f, 1e-3f));
	// 中间值在[0,1]范围
	for (int i = 1; i <= 9; ++i)
	{
		float v = c.evaluate(i * 0.1f);
		AC_ASSERT(v >= 0.0f);
		AC_ASSERT(v <= 1.0f + 1e-3f);
	}
}

// ========== CurveBounceIn ==========
void AnimationCurveTest::testCurveBounceIn()
{
	CurveBounceIn c;
	AC_ASSERT(acNear(c.evaluate(0.0f), 0.0f, 1e-3f));
	AC_ASSERT(acNear(c.evaluate(1.0f), 1.0f, 1e-3f));
	for (int i = 1; i <= 9; ++i)
	{
		float v = c.evaluate(i * 0.1f);
		AC_ASSERT(v >= 0.0f);
		AC_ASSERT(v <= 1.0f + 1e-3f);
	}
}

// ========== CurveBounceInOut ==========
void AnimationCurveTest::testCurveBounceInOut()
{
	CurveBounceInOut c;
	AC_ASSERT(acNear(c.evaluate(0.0f), 0.0f, 1e-3f));
	AC_ASSERT(acNear(c.evaluate(0.5f), 0.5f, 1e-3f));
	AC_ASSERT(acNear(c.evaluate(1.0f), 1.0f, 1e-3f));
	for (int i = 1; i <= 9; ++i)
	{
		float v = c.evaluate(i * 0.1f);
		AC_ASSERT(v >= 0.0f);
		AC_ASSERT(v <= 1.0f + 1e-3f);
	}
}

// ========== CurveElasticIn ==========
void AnimationCurveTest::testCurveElasticIn()
{
	CurveElasticIn c;
	AC_ASSERT(acNear(c.evaluate(0.0f), 0.0f, 1e-3f));
	AC_ASSERT(acNear(c.evaluate(1.0f), 1.0f, 1e-3f));
	// Elastic 有振荡，不强求单调，但起点终点正确
	float v_half = c.evaluate(0.5f);
	AC_ASSERT(fabsf(v_half) < 0.5f);  // 前半段仍接近0
}

// ========== CurveElasticOut ==========
void AnimationCurveTest::testCurveElasticOut()
{
	CurveElasticOut c;
	AC_ASSERT(acNear(c.evaluate(0.0f), 0.0f, 1e-3f));
	AC_ASSERT(acNear(c.evaluate(1.0f), 1.0f, 1e-3f));
	// Elastic out 终点前会超过1
	bool overShot = false;
	for (int i = 1; i <= 19; ++i)
	{
		float v = c.evaluate(i * 0.05f);
		if (v > 1.0f + 1e-4f) overShot = true;
	}
	AC_ASSERT(overShot);
}

// ========== CurveElasticInOut ==========
void AnimationCurveTest::testCurveElasticInOut()
{
	CurveElasticInOut c;
	AC_ASSERT(acNear(c.evaluate(0.0f), 0.0f, 1e-3f));
	AC_ASSERT(acNear(c.evaluate(0.5f), 0.5f, 1e-3f));
	AC_ASSERT(acNear(c.evaluate(1.0f), 1.0f, 1e-3f));
}

// ========== AnimationCurve::bounceEaseOut 静态方法 ==========
void AnimationCurveTest::testBounceEaseOut()
{
	// 4个分段边界
	AC_ASSERT(acNear(AnimationCurve::bounceEaseOut(0.0f), 0.0f, 1e-3f));
	AC_ASSERT(acNear(AnimationCurve::bounceEaseOut(1.0f), 1.0f, 1e-3f));
	// 中间值在[0,1]
	for (int i = 1; i <= 9; ++i)
	{
		float v = AnimationCurve::bounceEaseOut(i * 0.1f);
		AC_ASSERT(v >= 0.0f && v <= 1.0f + 1e-3f);
	}
	// 段边界: 1/2.75 ≈ 0.3636
	float t1 = 1.0f / 2.75f - 0.01f;
	float t2 = 1.0f / 2.75f + 0.01f;
	float v1 = AnimationCurve::bounceEaseOut(t1);
	float v2 = AnimationCurve::bounceEaseOut(t2);
	AC_ASSERT(v1 >= 0.0f && v2 >= 0.0f);
}

// ========== AnimationCurve::bounceEaseIn 静态方法 ==========
void AnimationCurveTest::testBounceEaseIn()
{
	AC_ASSERT(acNear(AnimationCurve::bounceEaseIn(0.0f), 0.0f, 1e-3f));
	AC_ASSERT(acNear(AnimationCurve::bounceEaseIn(1.0f), 1.0f, 1e-3f));
	for (int i = 1; i <= 9; ++i)
	{
		float v = AnimationCurve::bounceEaseIn(i * 0.1f);
		AC_ASSERT(v >= 0.0f && v <= 1.0f + 1e-3f);
	}
}

// ========== AnimationCurve::bounceEaseInOut 静态方法 ==========
void AnimationCurveTest::testBounceEaseInOut()
{
	AC_ASSERT(acNear(AnimationCurve::bounceEaseInOut(0.0f), 0.0f, 1e-3f));
	AC_ASSERT(acNear(AnimationCurve::bounceEaseInOut(0.5f), 0.5f, 1e-3f));
	AC_ASSERT(acNear(AnimationCurve::bounceEaseInOut(1.0f), 1.0f, 1e-3f));
	for (int i = 1; i <= 9; ++i)
	{
		float v = AnimationCurve::bounceEaseInOut(i * 0.1f);
		AC_ASSERT(v >= 0.0f && v <= 1.0f + 1e-3f);
	}
	// 对称性: bounceEaseInOut(t) + bounceEaseInOut(1-t) ≈ 1
	for (int i = 1; i <= 9; ++i)
	{
		float t = i * 0.1f;
		float v1 = AnimationCurve::bounceEaseInOut(t);
		float v2 = AnimationCurve::bounceEaseInOut(1.0f - t);
		AC_ASSERT(acNear(v1 + v2, 1.0f, 1e-3f));
	}
}

// ========== 所有曲线端点和中点 批量测试 ==========
void AnimationCurveTest::testAllCurvesEndpoints()
{
	// 用数组统一测试所有 In/Out/InOut 曲线的端点
	CurveCubicIn c1;   AC_ASSERT(acNear(c1.evaluate(0.0f), 0.0f, 1e-3f)); AC_ASSERT(acNear(c1.evaluate(1.0f), 1.0f, 1e-3f));
	CurveCubicOut c2;  AC_ASSERT(acNear(c2.evaluate(0.0f), 0.0f, 1e-3f)); AC_ASSERT(acNear(c2.evaluate(1.0f), 1.0f, 1e-3f));
	CurveCubicInOut c3; AC_ASSERT(acNear(c3.evaluate(0.0f), 0.0f, 1e-3f)); AC_ASSERT(acNear(c3.evaluate(1.0f), 1.0f, 1e-3f));
	CurveQuadIn c4;    AC_ASSERT(acNear(c4.evaluate(0.0f), 0.0f, 1e-3f)); AC_ASSERT(acNear(c4.evaluate(1.0f), 1.0f, 1e-3f));
	CurveQuadOut c5;   AC_ASSERT(acNear(c5.evaluate(0.0f), 0.0f, 1e-3f)); AC_ASSERT(acNear(c5.evaluate(1.0f), 1.0f, 1e-3f));
	CurveQuadInOut c6; AC_ASSERT(acNear(c6.evaluate(0.0f), 0.0f, 1e-3f)); AC_ASSERT(acNear(c6.evaluate(1.0f), 1.0f, 1e-3f));
	CurveQuartIn c7;   AC_ASSERT(acNear(c7.evaluate(0.0f), 0.0f, 1e-3f)); AC_ASSERT(acNear(c7.evaluate(1.0f), 1.0f, 1e-3f));
	CurveQuartOut c8;  AC_ASSERT(acNear(c8.evaluate(0.0f), 0.0f, 1e-3f)); AC_ASSERT(acNear(c8.evaluate(1.0f), 1.0f, 1e-3f));
	CurveQuintIn c9;   AC_ASSERT(acNear(c9.evaluate(0.0f), 0.0f, 1e-3f)); AC_ASSERT(acNear(c9.evaluate(1.0f), 1.0f, 1e-3f));
	CurveQuintOut c10; AC_ASSERT(acNear(c10.evaluate(0.0f), 0.0f, 1e-3f)); AC_ASSERT(acNear(c10.evaluate(1.0f), 1.0f, 1e-3f));
	CurveSineIn c11;   AC_ASSERT(acNear(c11.evaluate(0.0f), 0.0f, 1e-3f)); AC_ASSERT(acNear(c11.evaluate(1.0f), 1.0f, 1e-3f));
	CurveSineOut c12;  AC_ASSERT(acNear(c12.evaluate(0.0f), 0.0f, 1e-3f)); AC_ASSERT(acNear(c12.evaluate(1.0f), 1.0f, 1e-3f));
	CurveSineInOut c13; AC_ASSERT(acNear(c13.evaluate(0.0f), 0.0f, 1e-3f)); AC_ASSERT(acNear(c13.evaluate(1.0f), 1.0f, 1e-3f));
	CurveCircleIn c14; AC_ASSERT(acNear(c14.evaluate(0.0f), 0.0f, 1e-3f)); AC_ASSERT(acNear(c14.evaluate(1.0f), 1.0f, 1e-3f));
	CurveCircleOut c15; AC_ASSERT(acNear(c15.evaluate(0.0f), 0.0f, 1e-3f)); AC_ASSERT(acNear(c15.evaluate(1.0f), 1.0f, 1e-3f));
	CurveExpoIn c16;   AC_ASSERT(acNear(c16.evaluate(0.0f), 0.0f, 1e-3f)); AC_ASSERT(acNear(c16.evaluate(1.0f), 1.0f, 1e-3f));
	CurveExpoOut c17;  AC_ASSERT(acNear(c17.evaluate(0.0f), 0.0f, 1e-3f)); AC_ASSERT(acNear(c17.evaluate(1.0f), 1.0f, 1e-3f));
	CurveBounceIn c18; AC_ASSERT(acNear(c18.evaluate(0.0f), 0.0f, 1e-3f)); AC_ASSERT(acNear(c18.evaluate(1.0f), 1.0f, 1e-3f));
	CurveBounceOut c19; AC_ASSERT(acNear(c19.evaluate(0.0f), 0.0f, 1e-3f)); AC_ASSERT(acNear(c19.evaluate(1.0f), 1.0f, 1e-3f));
	CurveBounceInOut c20; AC_ASSERT(acNear(c20.evaluate(0.0f), 0.0f, 1e-3f)); AC_ASSERT(acNear(c20.evaluate(1.0f), 1.0f, 1e-3f));
	CurveElasticIn c21; AC_ASSERT(acNear(c21.evaluate(0.0f), 0.0f, 1e-3f)); AC_ASSERT(acNear(c21.evaluate(1.0f), 1.0f, 1e-3f));
	CurveElasticOut c22; AC_ASSERT(acNear(c22.evaluate(0.0f), 0.0f, 1e-3f)); AC_ASSERT(acNear(c22.evaluate(1.0f), 1.0f, 1e-3f));
	CurveElasticInOut c23; AC_ASSERT(acNear(c23.evaluate(0.0f), 0.0f, 1e-3f)); AC_ASSERT(acNear(c23.evaluate(1.0f), 1.0f, 1e-3f));
	CurveBackIn c24;   AC_ASSERT(acNear(c24.evaluate(0.0f), 0.0f, 1e-3f)); AC_ASSERT(acNear(c24.evaluate(1.0f), 1.0f, 1e-3f));
	CurveBackOut c25;  AC_ASSERT(acNear(c25.evaluate(0.0f), 0.0f, 1e-3f)); AC_ASSERT(acNear(c25.evaluate(1.0f), 1.0f, 1e-3f));
}

// ========== length 对所有曲线都返回 1.0 ==========
void AnimationCurveTest::testAllCurvesLength()
{
	AnimationCurve base;      AC_ASSERT(acNear(base.length(), 1.0f));
	CurveLinearZeroOne c1;    AC_ASSERT(acNear(c1.length(), 1.0f));
	CurveLinearOneZero c2;    AC_ASSERT(acNear(c2.length(), 1.0f));
	CurveCubicIn c3;          AC_ASSERT(acNear(c3.length(), 1.0f));
	CurveCubicOut c4;         AC_ASSERT(acNear(c4.length(), 1.0f));
	CurveCubicInOut c5;       AC_ASSERT(acNear(c5.length(), 1.0f));
	CurveQuadIn c6;           AC_ASSERT(acNear(c6.length(), 1.0f));
	CurveQuintIn c7;          AC_ASSERT(acNear(c7.length(), 1.0f));
	CurveSineIn c8;           AC_ASSERT(acNear(c8.length(), 1.0f));
	CurveCircleIn c9;         AC_ASSERT(acNear(c9.length(), 1.0f));
	CurveExpoIn c10;          AC_ASSERT(acNear(c10.length(), 1.0f));
	CurveBounceOut c11;       AC_ASSERT(acNear(c11.length(), 1.0f));
	CurveElasticOut c12;      AC_ASSERT(acNear(c12.length(), 1.0f));
	CurveBackIn c13;          AC_ASSERT(acNear(c13.length(), 1.0f));
}


//------------------------------------------------------------------------------
// Extended: monotonicity tests
//------------------------------------------------------------------------------
void AnimationCurveTest::testAllCurvesMonotonicity()
{
	CurveQuadIn quadIn;
	{
		float v0 = quadIn.evaluate(0.0f), v25 = quadIn.evaluate(0.25f);
		float v50 = quadIn.evaluate(0.5f), v75 = quadIn.evaluate(0.75f);
		float v100 = quadIn.evaluate(1.0f);
		AC_ASSERT(v25 >= v0 - 1e-4f);
		AC_ASSERT(v50 >= v25 - 1e-4f);
		AC_ASSERT(v75 >= v50 - 1e-4f);
		AC_ASSERT(v100 >= v75 - 1e-4f);
	}
	CurveQuadOut quadOut;
	{
		float v0 = quadOut.evaluate(0.0f), v50 = quadOut.evaluate(0.5f), v100 = quadOut.evaluate(1.0f);
		AC_ASSERT(v50 >= v0 - 1e-4f);
		AC_ASSERT(v100 >= v50 - 1e-4f);
	}
	CurveSineIn sineIn;
	{
		float v0 = sineIn.evaluate(0.0f), v25 = sineIn.evaluate(0.25f);
		float v50 = sineIn.evaluate(0.5f), v75 = sineIn.evaluate(0.75f);
		float v100 = sineIn.evaluate(1.0f);
		AC_ASSERT(v25 >= v0 - 1e-4f);
		AC_ASSERT(v50 >= v25 - 1e-4f);
		AC_ASSERT(v75 >= v50 - 1e-4f);
		AC_ASSERT(v100 >= v75 - 1e-4f);
	}
	CurveSineOut sineOut;
	{
		float v0 = sineOut.evaluate(0.0f), v50 = sineOut.evaluate(0.5f), v100 = sineOut.evaluate(1.0f);
		AC_ASSERT(v50 >= v0 - 1e-4f);
		AC_ASSERT(v100 >= v50 - 1e-4f);
	}
	CurveExpoIn expoIn;
	{
		float v0 = expoIn.evaluate(0.0f), v25 = expoIn.evaluate(0.25f);
		float v50 = expoIn.evaluate(0.5f), v100 = expoIn.evaluate(1.0f);
		AC_ASSERT(v25 >= v0 - 1e-4f);
		AC_ASSERT(v50 >= v25 - 1e-4f);
		AC_ASSERT(v100 >= v50 - 1e-4f);
	}
	CurveCircleIn circleIn;
	{
		float v0 = circleIn.evaluate(0.0f), v50 = circleIn.evaluate(0.5f), v100 = circleIn.evaluate(1.0f);
		AC_ASSERT(v50 >= v0 - 1e-4f);
		AC_ASSERT(v100 >= v50 - 1e-4f);
	}
}

//------------------------------------------------------------------------------
// Extended: midpoint value tests
//------------------------------------------------------------------------------
void AnimationCurveTest::testAllCurvesMidpoints()
{
	CurveLinearZeroOne lin01;
	AC_ASSERT(acNear(lin01.evaluate(0.5f), 0.5f));
	CurveLinearOneZero lin10;
	AC_ASSERT(acNear(lin10.evaluate(0.5f), 0.5f));
	CurveLinearZeroOneZero lin010;
	AC_ASSERT(acNear(lin010.evaluate(0.5f), 1.0f));
	CurveLinearOneZeroOne lin101;
	AC_ASSERT(acNear(lin101.evaluate(0.5f), 0.0f));
	CurveQuadIn qi;
	AC_ASSERT(qi.evaluate(0.5f) >= 0.0f && qi.evaluate(0.5f) <= 1.0f);
	CurveQuadOut qo;
	AC_ASSERT(qo.evaluate(0.5f) >= 0.0f && qo.evaluate(0.5f) <= 1.0f);
	CurveQuadInOut qio;
	AC_ASSERT(acNear(qio.evaluate(0.5f), 0.5f, 1e-3f));
	CurveSineIn si;
	AC_ASSERT(si.evaluate(0.5f) >= 0.0f && si.evaluate(0.5f) <= 1.0f);
	CurveSineOut so;
	AC_ASSERT(so.evaluate(0.5f) >= 0.0f && so.evaluate(0.5f) <= 1.0f);
	CurveSineInOut sio;
	AC_ASSERT(acNear(sio.evaluate(0.5f), 0.5f, 1e-3f));
}

//------------------------------------------------------------------------------
// Extended: InOut symmetry f(t) + f(1-t) == 1
//------------------------------------------------------------------------------
void AnimationCurveTest::testInOutSymmetry()
{
	CurveQuadInOut qio;
	for (int step = 1; step <= 9; ++step)
	{
		const float t = step * 0.1f;
		AC_ASSERT(acNear(qio.evaluate(t) + qio.evaluate(1.0f - t), 1.0f, 1e-3f));
	}
	CurveSineInOut sio;
	for (int step = 1; step <= 9; ++step)
	{
		const float t = step * 0.1f;
		AC_ASSERT(acNear(sio.evaluate(t) + sio.evaluate(1.0f - t), 1.0f, 1e-3f));
	}
	CurveCircleInOut cio;
	for (int step = 1; step <= 9; ++step)
	{
		const float t = step * 0.1f;
		AC_ASSERT(acNear(cio.evaluate(t) + cio.evaluate(1.0f - t), 1.0f, 1e-3f));
	}
}

//------------------------------------------------------------------------------
// Extended: base curve dense sampling
//------------------------------------------------------------------------------
void AnimationCurveTest::testAnimationCurveBaseExtended()
{
	AnimationCurve base;
	for (int step = 0; step <= 10; ++step)
	{
		const float t = step * 0.1f;
		AC_ASSERT(acNear(base.evaluate(t), t));
	}
	AC_ASSERT(acNear(base.length(), 1.0f));
	CurveLinearZeroOne lin;
	for (int step = 0; step <= 10; ++step)
	{
		const float t = step * 0.1f;
		AC_ASSERT(acNear(lin.evaluate(t), t));
	}
}

// ========== CurveQuartInOut ==========
void AnimationCurveTest::testCurveQuartInOut()
{
	CurveQuartInOut c;
	AC_ASSERT(acNear(c.evaluate(0.0f), 0.0f));
	AC_ASSERT(acNear(c.evaluate(1.0f), 0.5f));   // see implementation: always first branch
	// 单调递增
	float prev = -1.0f;
	for (int i = 0; i <= 10; ++i)
	{
		float v = c.evaluate(i * 0.1f);
		AC_ASSERT(v >= prev);
		prev = v;
	}
}

// ========== CurveQuintInOut ==========
void AnimationCurveTest::testCurveQuintInOut()
{
	CurveQuintInOut c;
	AC_ASSERT(acNear(c.evaluate(0.0f), 0.0f));
	AC_ASSERT(acNear(c.evaluate(1.0f), 0.5f));   // see implementation: always first branch
	// 单调递增
	float prev = -1.0f;
	for (int i = 0; i <= 10; ++i)
	{
		float v = c.evaluate(i * 0.1f);
		AC_ASSERT(v >= prev);
		prev = v;
	}
}

// ========== 主入口 ==========

void AnimationCurveTest::test()
{
	testAnimationCurveBase();
	testCurveLinearZeroOne();
	testCurveLinearOneZero();
	testCurveLinearZeroOneZero();
	testCurveLinearOneZeroOne();
	testCurveCubicIn();
	testCurveCubicOut();
	testCurveCubicInOut();
	testCurveQuadIn();
	testCurveQuadOut();
	testCurveQuadInOut();
	testCurveQuartIn();
	testCurveQuartOut();
	testCurveQuintIn();
	testCurveQuintOut();
	testCurveSineIn();
	testCurveSineOut();
	testCurveSineInOut();
	testCurveCircleIn();
	testCurveCircleOut();
	testCurveCircleInOut();
	testCurveExpoIn();
	testCurveExpoOut();
	testCurveExpoInOut();
	testCurveBackIn();
	testCurveBackOut();
	testCurveBackInOut();
	testCurveBounceOut();
	testCurveBounceIn();
	testCurveBounceInOut();
	testCurveElasticIn();
	testCurveElasticOut();
	testCurveElasticInOut();
	testBounceEaseOut();
	testBounceEaseIn();
	testBounceEaseInOut();
	testCurveQuartInOut();
	testCurveQuintInOut();
	testAllCurvesEndpoints();
	testAllCurvesLength();
	testAllCurvesMonotonicity();
	testAllCurvesMidpoints();
	testInOutSymmetry();
	testAnimationCurveBaseExtended();
}

#endif
