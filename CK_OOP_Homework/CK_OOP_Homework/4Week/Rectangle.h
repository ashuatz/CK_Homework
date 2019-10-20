#pragma once
#include "Point.h"

class Rectangle
{
	Point LT, RB;//Left Top, Right Bottom
	static const __int64 abs(const __int64& value) { return value > 0 ? value : -value; }
public:
	Rectangle(const Point& lt, const Point& rb) : LT(lt), RB(rb) {}
	unsigned __int64 GetArea() const;
};