#pragma once

#include "AnimationCurve.h"

class MICRO_LEGEND_FRAME_API CurveQuintOut : public AnimationCurve
{
	BASE(CurveQuintOut, AnimationCurve);
public:
	float evaluate(float time) const override 
	{
		time -= 1.0f;
		return time * time * time * time * time + 1.0f;
	}
};