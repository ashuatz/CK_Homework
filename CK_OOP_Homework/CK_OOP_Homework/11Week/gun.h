#pragma once
#include <iostream>

using namespace std;

class Gun
{
private:
	int bullet;
public:
	inline Gun(int bnum) : bullet(bnum) {}
	inline void Shot() { cout << "���� ���" << endl; bullet--; }
	inline void ShowBullet() { cout << "���� �Ѿ��� ���� : " << bullet << endl; }
	inline const int GetBulletCount() { return bullet; }
};