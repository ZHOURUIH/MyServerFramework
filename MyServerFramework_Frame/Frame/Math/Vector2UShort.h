﻿#pragma once

#include "FrameMacro.h"

struct MICRO_LEGEND_FRAME_API Vector2UShort
{
public:
	ushort x = 0;
	ushort y = 0;
public:
	Vector2UShort() = default;
	Vector2UShort(const ushort xx, const ushort yy):
		x(xx),
		y(yy)
	{}
	void clear()
	{
		x = 0;
		y = 0;
	}
	Vector2UShort operator*(const short that) const { return { (ushort)(x * that), (ushort)(y * that) }; }
	Vector2UShort operator+(const Vector2UShort& that) const { return { (ushort)(x + that.x), (ushort)(y + that.y) }; }
	Vector2UShort operator-(const Vector2UShort& that) const { return { (ushort)(x - that.x), (ushort)(y - that.y) }; }
	Vector2UShort& operator+=(const Vector2UShort& that)
	{
		x += that.x;
		y += that.y;
		return *this;
	}
	Vector2UShort& operator-=(const Vector2UShort& that)
	{
		x -= that.x;
		y -= that.y;
		return *this;
	}
	bool operator<(const Vector2UShort& other) const { return x + y < other.x + other.y; }
	bool operator==(const Vector2UShort& other) const { return x == other.x && y == other.y; }
	bool operator!=(const Vector2UShort& other) const { return x != other.x || y != other.y; }
};