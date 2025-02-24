#pragma once

#include "AnimationCurve.h"

class MICRO_LEGEND_FRAME_API CurveCubicIn : public AnimationCurve
{
	BASE(CurveCubicIn, AnimationCurve);
public:
	float evaluate(float time) const override { return time * time * time; }
};