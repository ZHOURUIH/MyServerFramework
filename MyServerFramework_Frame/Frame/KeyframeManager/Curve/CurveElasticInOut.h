#pragma once

#include "AnimationCurve.h"

class MICRO_LEGEND_FRAME_API CurveElasticInOut : public AnimationCurve
{
	BASE(CurveElasticInOut, AnimationCurve);
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
		const float period = 0.45f;
		const float s = period / TWO_PI_RADIAN * asin(1.0f / mOvershootOrAmplitude);
        if (time < 1.0f)
        {
            time -= 1.0f;
            return -0.5f * (mOvershootOrAmplitude * pow(2.0f, 10.0f * time) * sin((time - s) * TWO_PI_RADIAN / period));
        }
        else
        {
            time -= 1.0f;
            return mOvershootOrAmplitude * pow(2.0f, -10.0f * time) * sin((time - s) * TWO_PI_RADIAN / period) * 0.5f + 1.0f;
        }
	}
};