#pragma once

#include "FrameDefine.h"

class MICRO_LEGEND_FRAME_API Triangle2
{
public:
	Triangle2() = default;
	Triangle2(const Vector2& point0, const Vector2& point1, const Vector2& point2):
		mPoint0(point0),
		mPoint1(point1),
		mPoint2(point2)
	{}
public:
	Vector2 mPoint0;
	Vector2 mPoint1;
	Vector2 mPoint2;
};