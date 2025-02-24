#pragma once

#include "AnimationCurve.h"

class MICRO_LEGEND_FRAME_API CurveBounceOut : public AnimationCurve
{
	BASE(CurveBounceOut, AnimationCurve);
public:
	float evaluate(float time) const override { return bounceEaseOut(time); }
};