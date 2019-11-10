#pragma once
#include "gun.h"

class Police
{
private:
	int handCuffCount;
	Gun* pistol;

public:
	inline Police(int hcCount, int ammo) : handCuffCount(hcCount)
	{
		if (ammo > 0)
			pistol = new Gun(ammo);
		else
			pistol = nullptr;
	}

	inline Police(const Police& copy) : handCuffCount(copy.handCuffCount)
	{
		if (copy.pistol != nullptr && copy.pistol->GetBulletCount() > 0)
			pistol = new Gun(copy.pistol->GetBulletCount());
		else
			pistol = nullptr;
	}

	inline Police& operator=(const Police& copy)
	{
		this->handCuffCount = copy.handCuffCount;

		if (this->pistol != nullptr)
		{
			delete pistol;
			pistol = nullptr;
		}

		if (copy.pistol != nullptr && copy.pistol->GetBulletCount() > 0)
		{
			this->pistol = new Gun(copy.pistol->GetBulletCount());
		}

		return *this;
	}

	inline void PutHandcuff()
	{
		cout << "PutHandcuff" << endl;
		handCuffCount--;
		cout << "Left Handcuff : " << handCuffCount << endl;
	}

	inline void Shot()
	{
		if (pistol != nullptr && pistol->GetBulletCount() > 0)
		{
			cout << "Shot" << endl;
			pistol->Shot();
			pistol->ShowBullet();
		}
	}

	inline ~Police()
	{
		delete pistol;
	}
};
