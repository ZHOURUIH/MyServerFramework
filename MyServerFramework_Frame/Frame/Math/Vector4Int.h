﻿#pragma once

#include "FrameMacro.h"

struct MICRO_LEGEND_FRAME_API Vector4Int
{
public:
	int x = 0;
	int y = 0;
	int z = 0;
	int w = 0;
public:
	Vector4Int() = default;
	Vector4Int(const int xx, const int yy, const int zz, const int ww):
		x(xx),
		y(yy),
		z(zz),
		w(ww)
	{}
	void clear()
	{
		x = 0;
		y = 0;
		z = 0;
		w = 0;
	}
	Vector4Int operator+(const Vector4Int& that) const { return { x + that.x, y + that.y, z + that.z, w + that.w }; }
	Vector4Int operator-(const Vector4Int& that) const { return { x - that.x, y - that.y, z - that.z, w - that.w }; }
	Vector4Int& operator+=(const Vector4Int& that)
	{
		x += that.x;
		y += that.y;
		z += that.z;
		w += that.w;
		return *this;
	}
	Vector4Int& operator-=(const Vector4Int& that)
	{
		x -= that.x;
		y -= that.y;
		z -= that.z;
		w -= that.w;
		return *this;
	}
	Vector4Int operator-() const { return { -x, -y, -z, -w }; }
};