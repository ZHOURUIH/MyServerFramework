#pragma once

#include "AnimationCurve.h"

class MICRO_LEGEND_FRAME_API CurveCircleIn : public AnimationCurve
{
	BASE(CurveCircleIn, AnimationCurve);
public:
	float evaluate(float time) const override { return -(sqrt(1.0f - time * time) - 1.0f); }
};