#pragma once

#include "FrameBase.h"

class MICRO_LEGEND_FRAME_API AnimationCurve
{
public:
    virtual ~AnimationCurve() = default;
	virtual float evaluate(float time) const    { return time; }
	float length() const                        { return 1.0f; }
    static float bounceEaseIn(float time)       { return 1.0f - bounceEaseOut(1.0f - time); }
    static float bounceEaseOut(float time);
    static float bounceEaseInOut(float time);
public:
    static constexpr float mOvershootOrAmplitude = 1.70158f;
};