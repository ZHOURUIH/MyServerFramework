#pragma once

#include "AnimationCurve.h"

class MICRO_LEGEND_FRAME_API CurveElasticIn : public AnimationCurve
{
	BASE(CurveElasticIn, AnimationCurve);
public:
	float evaluate(float time) const override 
	{
		if (isZero(time))
		{
			return 0.0f;
		}
		if (isEqual(time, 1.0f))
		{
			return 1.0f;
		}
		const float period = 0.3f;
		const float s0 = period / TWO_PI_RADIAN * asin(1 / mOvershootOrAmplitude);
		time -= 1.0f;
		return -(mOvershootOrAmplitude * pow(2.0f, 10.0f * time) * sin((time - s0) * TWO_PI_RADIAN / period));
	}
};