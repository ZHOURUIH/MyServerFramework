#pragma once

#include "AnimationCurve.h"

class MICRO_LEGEND_FRAME_API CurveSineOut : public AnimationCurve
{
	BASE(CurveSineOut, AnimationCurve);
public:
	float evaluate(float time) const override { return sin(time * HALF_PI_RADIAN); }
};