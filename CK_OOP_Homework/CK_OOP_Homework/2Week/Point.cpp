#include "Point.h"
#include <iostream>
using namespace std;

void Pointer::setPos(const float & x, const float & y)
{
	this->x = x;
	this->y = y;
}

void Pointer::Show()
{
	cout << "Position is [" << x << "," << y << "]" << endl;
}
