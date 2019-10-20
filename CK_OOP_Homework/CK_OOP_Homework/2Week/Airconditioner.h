#pragma once
/*
201913014 정지상 객체지향 과제
2주차 2번 - 에어컨
*/

#include <thread>
class Airconditioner
{
private:
	bool isOn;
	float targetTemperture;
	float currentTemperature;
	std::thread* CoolingSolution;

	void Update();

	void IncreaseCurrentTemperature();
	void DecreaseCurrentTemperature();
public:
	Airconditioner(const float& currentTemp)
	{
		isOn = false;
		currentTemperature = currentTemp;
		targetTemperture = 24;
	}

	void SetPower();
	void SetTargetTemperature(const float& value);

	float GetCurrentTemperature() const;
	float GetTargetTemperature() const;

	void ShowCurrentTemperature() const;
};