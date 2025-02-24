#pragma once

#include "AnimationCurve.h"

class MICRO_LEGEND_FRAME_API CurveQuadInOut : public AnimationCurve
{
	BASE(CurveQuadInOut, AnimationCurve);
public:
	float evaluate(float time) const override 
	{
		if (time * 0.5f < 1.0f)
		{
			return 0.5f * time * time;
		}
		--time;
        return -0.5f * (time * (time - 2.0f) - 1.0f);
	}
};