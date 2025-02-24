#pragma once

#include "FrameDefine.h"

class MICRO_LEGEND_FRAME_API Rect3
{
public:
	Rect3(const Vector3& min, const Vector3& max):
		mMin(min),
		mMax(max)
	{}
	Vector3 getSize() const			{ return mMax - mMin; }
	Vector3 getCenter() const		{ return (mMax + mMin) * 0.5f; }
	// 在忽略Y轴的情况下获得对角线的长度
	float getDiagonalIgnoreY() const{ return getLength(resetY(mMax - mMin)); }
public:
	Vector3 mMin;
	Vector3 mMax;
};