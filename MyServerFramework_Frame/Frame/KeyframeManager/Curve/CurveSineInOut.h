#pragma once

#include "AnimationCurve.h"

class MICRO_LEGEND_FRAME_API CurveSineInOut : public AnimationCurve
{
	BASE(CurveSineInOut, AnimationCurve);
public:
	float evaluate(float time) const override { return -0.5f * (cos(PI_RADIAN * time) - 1.0f); }
};