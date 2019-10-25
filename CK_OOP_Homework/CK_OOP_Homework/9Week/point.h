#pragma once

class Point
{
public:
	int x, y;

	Point() :x(0), y(0) {}
	Point(const int& x, const int& y) :x(x), y(y) {}

	Point& operator+=(const Point& rhs)
	{
		this->x += rhs.x;
		this->y += rhs.y;
		return *this;
	}
	Point& operator-=(const Point& rhs)
	{
		this->x -= rhs.x;
		this->y -= rhs.y;
		return *this;
	}

	const Point operator+(const Point& rhs) const
	{
		return Point{ this->x + rhs.x,this->y + rhs.y };
	}

	const Point operator-(const Point& rhs) const
	{
		return Point{ this->x - rhs.x,this->y - rhs.y };
	}

	const bool operator==(const Point& rhs) const
	{
		return this->x == rhs.x && this->y == rhs.y;
	}
	const bool operator!=(const Point& rhs) const
	{
		return this->x != rhs.x || this->y != rhs.y;
	}
};