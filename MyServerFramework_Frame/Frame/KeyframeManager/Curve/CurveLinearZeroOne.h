#pragma once

#include "AnimationCurve.h"

class MICRO_LEGEND_FRAME_API CurveLinearZeroOne : public AnimationCurve
{
	BASE(CurveLinearZeroOne, AnimationCurve);
public:
	float evaluate(float time) const override { return time; }
};