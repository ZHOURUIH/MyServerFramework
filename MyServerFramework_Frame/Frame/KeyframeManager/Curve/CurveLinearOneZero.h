#pragma once

#include "AnimationCurve.h"

class MICRO_LEGEND_FRAME_API CurveLinearOneZero : public AnimationCurve
{
	BASE(CurveLinearOneZero, AnimationCurve);
public:
	float evaluate(float time) const override { return 1.0f - time; }
};