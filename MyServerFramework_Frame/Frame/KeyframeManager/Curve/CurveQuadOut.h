#pragma once

#include "AnimationCurve.h"

class MICRO_LEGEND_FRAME_API CurveQuadOut : public AnimationCurve
{
	BASE(CurveQuadOut, AnimationCurve);
public:
	float evaluate(float time) const override { return -time * (time - 2.0f); }
};