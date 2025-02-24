#pragma once

#include "AnimationCurve.h"

class MICRO_LEGEND_FRAME_API CurveExpoOut : public AnimationCurve
{
	BASE(CurveExpoOut, AnimationCurve);
public:
	float evaluate(float time) const override 
	{
		if (isEqual(time, 1.0f))
		{
			return 1.0f;
		}
		return -pow(2.0f, -10.0f * time) + 1.0f;
	}
};