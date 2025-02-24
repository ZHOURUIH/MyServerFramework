#pragma once

#include "AnimationCurve.h"

class MICRO_LEGEND_FRAME_API CurveLinearOneZeroOne : public AnimationCurve
{
	BASE(CurveLinearOneZeroOne, AnimationCurve);
public:
	float evaluate(float time) const override 
	{
		if (time < 0.5f)
		{
			return 1.0f - time * 2.0f;
		}
		else
		{
			return (time - 0.5f) * 2.0f;
		}
	}
};