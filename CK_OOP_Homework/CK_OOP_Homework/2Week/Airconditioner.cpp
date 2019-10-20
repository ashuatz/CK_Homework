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
		cout << "�������� �������ϴ�." << endl;
		CoolingSolution = new std::thread(&Airconditioner::Update, this);
		//CoolingSolution->join();
	}
	else
	{
		CoolingSolution->join();
		cout << "�������� �������ϴ�." << endl;
		delete CoolingSolution;
	}
}

void Airconditioner::SetTargetTemperature(const float & value)
{
	this->targetTemperture = value;
	cout << "��� �µ��� " << targetTemperture << "�� �� �����Ǿ����ϴ�." << endl;
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
	cout << "���� �µ��� " << currentTemperature << "�� �Դϴ�." << endl;
}

void Airconditioner::IncreaseCurrentTemperature()
{
	currentTemperature++;

	cout << "�µ��� �����Ͽ����ϴ�." << endl;
}

void Airconditioner::DecreaseCurrentTemperature()
{
	currentTemperature--;

	cout << "�µ��� �����Ͽ����ϴ�." << endl;
}