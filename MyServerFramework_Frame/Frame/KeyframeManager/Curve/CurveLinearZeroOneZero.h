#pragma once

#include "AnimationCurve.h"

class MICRO_LEGEND_FRAME_API CurveLinearZeroOneZero : public AnimationCurve
{
	BASE(CurveLinearZeroOneZero, AnimationCurve);
public:
	float evaluate(float time) const override 
	{
		if (time < 0.5f)
		{
			return time * 2.0f;
		}
		else
		{
			return 1.0f - (time - 0.5f) * 2.0f;
		}
	}
};