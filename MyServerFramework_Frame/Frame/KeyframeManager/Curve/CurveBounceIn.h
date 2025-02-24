#pragma once

#include "AnimationCurve.h"

class MICRO_LEGEND_FRAME_API CurveBounceIn : public AnimationCurve
{
	BASE(CurveBounceIn, AnimationCurve);
public:
	float evaluate(float time) const override { return bounceEaseIn(time); }
};