#pragma once

#include "AnimationCurve.h"

class MICRO_LEGEND_FRAME_API CurveCircleInOut : public AnimationCurve
{
	BASE(CurveCircleInOut, AnimationCurve);
public:
	float evaluate(float time) const override
	{
		time *= 2.0f;
		if (time < 1.0f)
		{
			return -0.5f * (sqrt(1.0f - time * time) - 1.0f);
		}
		time -= 2.0f;
		return 0.5f * (sqrt(1.0f - time * time) + 1.0f);
	}
};