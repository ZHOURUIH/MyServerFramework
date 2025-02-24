#pragma once

#include "AnimationCurve.h"

class MICRO_LEGEND_FRAME_API CurveBounceInOut : public AnimationCurve
{
	BASE(CurveBounceInOut, AnimationCurve);
public:
	float evaluate(float time) const override { return bounceEaseInOut(time); }
};