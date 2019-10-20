#include "Rectangle.h"

unsigned __int64 Rectangle::GetArea() const
{
	return abs(LT.x - RB.x) * abs(LT.y - RB.y);
}