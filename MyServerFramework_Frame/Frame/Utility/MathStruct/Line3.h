#pragma once

#include "FrameDefine.h"

class MICRO_LEGEND_FRAME_API Line3
{
public:
	Line3(const Vector3& start, const Vector3& end):
		mStart(start),
		mEnd(end)
	{}
	Line2 toLine2IgnoreY() const
	{
		return Line2({ mStart.x, mStart.z }, { mEnd.x, mEnd.z });
	}
	Line2 toLine2IgnoreX() const
	{
		return Line2({ mStart.z, mStart.y }, { mEnd.z, mEnd.y });
	}
public:
	Vector3 mStart;
	Vector3 mEnd;
};