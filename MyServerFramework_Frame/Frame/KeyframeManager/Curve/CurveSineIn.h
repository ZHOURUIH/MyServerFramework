#pragma once

#include "AnimationCurve.h"

class MICRO_LEGEND_FRAME_API CurveSineIn : public AnimationCurve
{
	BASE(CurveSineIn, AnimationCurve);
public:
	float evaluate(float time) const override { return -cos(time * HALF_PI_RADIAN) + 1.0f; }
};