#include <iostream>
#include "Rectangle.h"

using namespace std;

int main()
{
	Point p1, p2;

	cout << "�� ��� ��ǥ �Է� : ";
	cin >> p1.x >> p1.y;

	cout << "�� �ϴ� ��ǥ �Է� : ";
	cin >> p2.x >> p2.y;


	Rectangle r(p1, p2);

	cout << "���� : " << r.GetArea() << endl;
}