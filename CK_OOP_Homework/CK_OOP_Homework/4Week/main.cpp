#include <iostream>
#include "Rectangle.h"

using namespace std;

int main()
{
	Point p1, p2;

	cout << "ÁÂ »ó´Ü ÁÂÇ¥ ÀÔ·Â : ";
	cin >> p1.x >> p1.y;

	cout << "¿ì ÇÏ´Ü ÁÂÇ¥ ÀÔ·Â : ";
	cin >> p2.x >> p2.y;


	Rectangle r(p1, p2);

	cout << "³ÐÀÌ : " << r.GetArea() << endl;
}