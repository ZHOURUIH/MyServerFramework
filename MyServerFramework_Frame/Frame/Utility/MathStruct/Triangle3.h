#pragma once

#include "FrameDefine.h"

class MICRO_LEGEND_FRAME_API Triangle3
{
public:
	Triangle3() = default;
	Triangle3(const Vector3& point0, const Vector3& point1, const Vector3& point2):
		mPoint0(point0),
		mPoint1(point1),
		mPoint2(point2)
	{}
public:
	Vector3 mPoint0;
	Vector3 mPoint1;
	Vector3 mPoint2;
};