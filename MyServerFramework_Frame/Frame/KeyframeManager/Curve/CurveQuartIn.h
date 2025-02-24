#pragma once

#include "AnimationCurve.h"

class MICRO_LEGEND_FRAME_API CurveQuartIn : public AnimationCurve
{
	BASE(CurveQuartIn, AnimationCurve);
public:
	float evaluate(float time) const override { return time * time * time * time; }
};