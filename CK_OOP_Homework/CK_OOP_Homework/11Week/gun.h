#pragma once
#include <iostream>

using namespace std;

class Gun
{
private:
	int bullet;
public:
	inline Gun(int bnum) : bullet(bnum) {}
	inline void Shot() { cout << "ÃÑÀ» ½î´Ù" << endl; bullet--; }
	inline void ShowBullet() { cout << "³²Àº ÃÑ¾ËÀÇ °³¼ö : " << bullet << endl; }
	inline const int GetBulletCount() { return bullet; }
};