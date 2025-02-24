#pragma once

#include "FrameDefine.h"

class MICRO_LEGEND_FRAME_API TriangleIntersect3
{
public:
	Vector3 mIntersectPoint; // 交点
	Vector3 mLinePoint0;     // 交点所在的三角形的一条边的起点
	Vector3 mLinePoint1;     // 交点所在的三角形的一条边的终点
};