#pragma once

#include "AnimationCurve.h"

class MICRO_LEGEND_FRAME_API CurveBackOut : public AnimationCurve
{
	BASE(CurveBackOut, AnimationCurve);
public:
	float evaluate(float time) const override
	{
		time -= 1.0f;
		return time * time * ((mOvershootOrAmplitude + 1.0f) * time + mOvershootOrAmplitude) + 1.0f;
	}
};