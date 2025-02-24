#pragma once

#include "AnimationCurve.h"

class MICRO_LEGEND_FRAME_API CurveBackIn : public AnimationCurve
{
	BASE(CurveBackIn, AnimationCurve);
public:
	float evaluate(float time) const override
	{ 
		return time * time * ((mOvershootOrAmplitude + 1) * time - mOvershootOrAmplitude);
	}
};