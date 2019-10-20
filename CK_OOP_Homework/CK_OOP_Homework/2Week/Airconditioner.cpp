#include "Airconditioner.h"
#include <iostream>
using namespace std;

void Airconditioner::Update()
{
	while (isOn)
	{
		if (targetTemperture > currentTemperature)
		{
			this->IncreaseCurrentTemperature();
		}
		else if (targetTemperture < currentTemperature)
		{
			this->DecreaseCurrentTemperature();
		}
	}
}

void Airconditioner::SetPower()
{
	isOn = !isOn;
	if (isOn)
	{
		cout << "에어컨이 켜졌습니다." << endl;
		CoolingSolution = new std::thread(&Airconditioner::Update, this);
		//CoolingSolution->join();
	}
	else
	{
		CoolingSolution->join();
		cout << "에어컨이 꺼졌습니다." << endl;
		delete CoolingSolution;
	}
}

void Airconditioner::SetTargetTemperature(const float & value)
{
	this->targetTemperture = value;
	cout << "희망 온도가 " << targetTemperture << "도 로 설정되었습니다." << endl;
}

float Airconditioner::GetCurrentTemperature() const
{
	return currentTemperature;
}

float Airconditioner::GetTargetTemperature() const
{
	return targetTemperture;
}

void Airconditioner::ShowCurrentTemperature() const
{
	cout << "현재 온도는 " << currentTemperature << "도 입니다." << endl;
}

void Airconditioner::IncreaseCurrentTemperature()
{
	currentTemperature++;

	cout << "온도가 증가하였습니다." << endl;
}

void Airconditioner::DecreaseCurrentTemperature()
{
	currentTemperature--;

	cout << "온도가 감소하였습니다." << endl;
}