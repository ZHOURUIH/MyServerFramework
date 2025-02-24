#pragma once

#include "AnimationCurve.h"

class MICRO_LEGEND_FRAME_API CurveExpoIn : public AnimationCurve
{
	BASE(CurveExpoIn, AnimationCurve);
public:
	float evaluate(float time) const override 
	{
		if (isZero(time))
		{
			return 0.0f;
		}
		return pow(2.0f, 10.0f * (time - 1.0f));
	}
};