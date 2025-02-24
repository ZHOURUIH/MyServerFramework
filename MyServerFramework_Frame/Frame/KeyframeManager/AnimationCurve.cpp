#include "FrameHeader.h"

float AnimationCurve::bounceEaseOut(float time)
{
    if (time < 1.0f / 2.75f)
    {
        return 7.5625f * time * time;
    }
    if (time < 2.0f / 2.75f)
    {
        time -= 1.5f / 2.75f;
        return 7.5625f * time * time + 0.75f;
    }
    if (time < 2.5f / 2.75f)
    {
        time -= 2.25f / 2.75f;
        return 7.5625f * time * time + 0.9375f;
    }
    time -= 2.625f / 2.75f;
    return 7.5625f * time * time + 0.984375f;
}

float AnimationCurve::bounceEaseInOut(const float time)
{
    if (time < 0.5f)
    {
        return bounceEaseIn(time * 2.0f) * 0.5f;
    }
    return bounceEaseOut(time * 2.0f - 1.0f) * 0.5f + 0.5f;
}