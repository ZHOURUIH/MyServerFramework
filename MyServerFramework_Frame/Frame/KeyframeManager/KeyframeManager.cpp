#include "FrameHeader.h"

void KeyframeManager::init()
{
	FrameComponent::init();
	loadAll();
}

// 创建所有关键帧曲线
void KeyframeManager::loadAll()
{
	createCurve<CurveLinearZeroOne>(KEYFRAME::LINEAR_ZERO_ONE);
	createCurve<CurveLinearZeroOneZero>(KEYFRAME::LINEAR_ZERO_ONE_ZERO);
	createCurve<CurveLinearOneZero>(KEYFRAME::LINEAR_ONE_ZERO);
	createCurve<CurveLinearOneZeroOne>(KEYFRAME::LINEAR_ONE_ZERO_ONE);
	createCurve<CurveBackIn>(KEYFRAME::BACK_IN);
	createCurve<CurveBackOut>(KEYFRAME::BACK_OUT);
	createCurve<CurveBackInOut>(KEYFRAME::BACK_IN_OUT);
	createCurve<CurveBounceIn>(KEYFRAME::BOUNCE_IN);
	createCurve<CurveBounceOut>(KEYFRAME::BOUNCE_OUT);
	createCurve<CurveBounceInOut>(KEYFRAME::BOUNCE_IN_OUT);
	createCurve<CurveCircleIn>(KEYFRAME::CIRCLE_IN);
	createCurve<CurveCircleOut>(KEYFRAME::CIRCLE_OUT);
	createCurve<CurveCircleInOut>(KEYFRAME::CIRCLE_IN_OUT);
	createCurve<CurveCubicIn>(KEYFRAME::CUBIC_IN);
	createCurve<CurveCubicOut>(KEYFRAME::CUBIC_OUT);
	createCurve<CurveCubicInOut>(KEYFRAME::CUBIC_IN_OUT);
	createCurve<CurveElasticIn>(KEYFRAME::ELASTIC_IN);
	createCurve<CurveElasticOut>(KEYFRAME::ELASTIC_OUT);
	createCurve<CurveElasticInOut>(KEYFRAME::ELASTIC_IN_OUT);
	createCurve<CurveExpoIn>(KEYFRAME::EXPO_IN);
	createCurve<CurveExpoOut>(KEYFRAME::EXPO_OUT);
	createCurve<CurveExpoInOut>(KEYFRAME::EXPO_IN_OUT);
	createCurve<CurveQuadIn>(KEYFRAME::QUAD_IN);
	createCurve<CurveQuadOut>(KEYFRAME::QUAD_OUT);
	createCurve<CurveQuadInOut>(KEYFRAME::QUAD_IN_OUT);
	createCurve<CurveQuartIn>(KEYFRAME::QUART_IN);
	createCurve<CurveQuartOut>(KEYFRAME::QUART_OUT);
	createCurve<CurveQuartInOut>(KEYFRAME::QUART_IN_OUT);
	createCurve<CurveQuintIn>(KEYFRAME::QUINT_IN);
	createCurve<CurveQuintOut>(KEYFRAME::QUINT_OUT);
	createCurve<CurveQuintInOut>(KEYFRAME::QUINT_IN_OUT);
	createCurve<CurveSineIn>(KEYFRAME::SINE_IN);
	createCurve<CurveSineOut>(KEYFRAME::SINE_OUT);
	createCurve<CurveSineInOut>(KEYFRAME::SINE_IN_OUT);
}
void KeyframeManager::quit()
{
	// 将实例化出的所有物体销毁
	for (const auto& curve : mCurveList)
	{
		delete curve.second;
	}
	mCurveList.clear();
}