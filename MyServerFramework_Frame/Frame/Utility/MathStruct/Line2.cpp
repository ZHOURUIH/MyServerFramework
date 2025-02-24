#include "FrameHeader.h"

Line2::Line2(const Vector2& start, const Vector2& end) :
	mStart(start),
	mEnd(end)
{
	mHasK = !isZero(mEnd.x - mStart.x);
	if (mHasK)
	{
		mK = divide(mEnd.y - mStart.y, mEnd.x - mStart.x);
		mB = mStart.y - mK * mStart.x;
	}
	else
	{
		mK = 0;
		mB = 0;
	}
}

// 获取直线上指定y坐标对应的x坐标
bool Line2::getPointXOnLine(float y, float& x)
{
	// 没有斜率,是一条平行于Y轴的直线,所有X都是一样的
	if (!mHasK)
	{
		x = mStart.x;
		return true;
	}
	// 斜率为0,是一条平行于X轴的执行,获取不到x坐标
	if (isZero(mK))
	{
		x = 0;
		return false;
	}
	x = divide(y - mB, mK);
	return true;
}