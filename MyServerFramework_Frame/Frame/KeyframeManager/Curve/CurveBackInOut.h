#pragma once

#include "AnimationCurve.h"

class MICRO_LEGEND_FRAME_API CurveBackInOut : public AnimationCurve
{
	BASE(CurveBackInOut, AnimationCurve);
public:
	float evaluate(float time) const override
	{
		time *= 2.0f;
		const float overshootOrAmplitude = mOvershootOrAmplitude * 1.525f;
		if (time < 1.0f)
		{
			return 0.5f * (time * time * ((overshootOrAmplitude + 1.0f) * time - overshootOrAmplitude));
		}
		time -= 2.0f;
		return 0.5f * (time * time * ((overshootOrAmplitude + 1.0f) * time + overshootOrAmplitude) + 2.0f);
	}
};