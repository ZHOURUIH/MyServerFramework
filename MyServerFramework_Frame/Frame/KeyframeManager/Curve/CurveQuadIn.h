#pragma once

#include "AnimationCurve.h"

class MICRO_LEGEND_FRAME_API CurveQuadIn : public AnimationCurve
{
	BASE(CurveQuadIn, AnimationCurve);
public:
	float evaluate(float time) const override { return time * time; }
};