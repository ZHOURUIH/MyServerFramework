#pragma once

#include "AnimationCurve.h"

class MICRO_LEGEND_FRAME_API CurveQuintIn : public AnimationCurve
{
	BASE(CurveQuintIn, AnimationCurve);
public:
	float evaluate(float time) const override { return time * time * time * time * time; }
};