#pragma once

#include "FrameBase.h"

class MICRO_LEGEND_FRAME_API AnimationCurveTest
{
#ifdef WINDOWS
public:
	static void test();
private:
	static bool acNear(float a, float b, float eps = 1e-4f);
	static void testAnimationCurveBase();
	static void testCurveLinearZeroOne();
	static void testCurveLinearOneZero();
	static void testCurveLinearZeroOneZero();
	static void testCurveLinearOneZeroOne();
	static void testCurveCubicIn();
	static void testCurveCubicOut();
	static void testCurveCubicInOut();
	static void testCurveQuadIn();
	static void testCurveQuadOut();
	static void testCurveQuadInOut();
	static void testCurveQuartIn();
	static void testCurveQuartOut();
	static void testCurveQuintIn();
	static void testCurveQuintOut();
	static void testCurveSineIn();
	static void testCurveSineOut();
	static void testCurveSineInOut();
	static void testCurveCircleIn();
	static void testCurveCircleOut();
	static void testCurveCircleInOut();
	static void testCurveExpoIn();
	static void testCurveExpoOut();
	static void testCurveExpoInOut();
	static void testCurveBackIn();
	static void testCurveBackOut();
	static void testCurveBackInOut();
	static void testCurveBounceOut();
	static void testCurveBounceIn();
	static void testCurveBounceInOut();
	static void testCurveElasticIn();
	static void testCurveElasticOut();
	static void testCurveElasticInOut();
	static void testBounceEaseOut();
	static void testBounceEaseIn();
	static void testBounceEaseInOut();
	static void testAllCurvesEndpoints();
	static void testAllCurvesLength();
	static void testAllCurvesMonotonicity();
	static void testAllCurvesMidpoints();
	static void testInOutSymmetry();
	static void testAnimationCurveBaseExtended();
	static void testCurveQuartInOut();
	static void testCurveQuintInOut();
#endif
};