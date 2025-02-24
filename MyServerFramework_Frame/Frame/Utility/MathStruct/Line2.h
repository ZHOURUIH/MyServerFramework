#pragma once

#include "FrameDefine.h"

class MICRO_LEGEND_FRAME_API Line2
{
public:
	Line2(const Vector2& start, const Vector2& end);
	// 获取直线上指定x坐标对应的y坐标
	bool getPointYOnLine(float x, float& y)
	{
		y = mHasK ? mK * x + mB : 0.0f;
		return mHasK;
	}
	// 获取直线上指定y坐标对应的x坐标
	bool getPointXOnLine(float y, float& x);
public:
	Vector2 mStart;
	Vector2 mEnd;
	float mK;		// 直线的斜率
	float mB;		// 直线与Y轴的交点
	bool mHasK;		// 此直线是否有斜率,与Y轴平行的直线没有斜率
};