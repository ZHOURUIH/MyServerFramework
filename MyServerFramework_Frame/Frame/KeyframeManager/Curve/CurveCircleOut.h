#pragma once

#include "AnimationCurve.h"

class MICRO_LEGEND_FRAME_API CurveCircleOut : public AnimationCurve
{
	BASE(CurveCircleOut, AnimationCurve);
public:
	float evaluate(float time) const override 
	{
		time -= 1.0f;
		return sqrt(1.0f - time * time); 
	}
};