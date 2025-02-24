#pragma once

#include "FrameDefine.h"

class MICRO_LEGEND_FRAME_API Rect
{
public:
	Rect(const Vector2& min, const Vector2& size):
		x(min.x),
		y(min.y),
		width(size.x),
		height(size.y)
	{}
	Vector2 getSize() const		{ return { width, height }; }
	Vector2 getMin() const		{ return { x, y }; }
	Vector2 getMax() const		{ return { x + width, y + height }; }
	Vector2 getCenter() const	{ return { x + width * 0.5f, y + height * 0.5f }; }
	// 对角线的长度
	float getDiagonal() const	{ return getLength(width, height); }
public:
	float x;
	float y;
	float width;
	float height;
};