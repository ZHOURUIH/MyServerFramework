#pragma once

#include "FrameDefine.h"

class MICRO_LEGEND_FRAME_API TriangleIntersect
{
public:
	Vector2 mIntersectPoint; // 交点
	Vector2 mLinePoint0;     // 交点所在的三角形的一条边的起点
	Vector2 mLinePoint1;     // 交点所在的三角形的一条边的终点
};