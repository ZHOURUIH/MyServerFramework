#pragma once

#include "AnimationCurve.h"

class MICRO_LEGEND_FRAME_API CurveQuintInOut : public AnimationCurve
{
	BASE(CurveQuintInOut, AnimationCurve);
public:
	float evaluate(float time) const override 
	{
		if (time * 0.5f < 1.0f)
		{
			return 0.5f * time * time * time * time * time;
		}
		time -= 2.0f;
		return 0.5f * (time * time * time * time * time + 2.0f);
	}
};