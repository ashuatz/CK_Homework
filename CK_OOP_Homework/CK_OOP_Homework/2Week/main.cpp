#include <iostream>

#include "Point.h"
#include "Airconditioner.h"
#include "Calculator.h"

using namespace std;

int main()
{
	cout << "201913014 정지상 과제 결과물" << endl;

	//Point
	Pointer p;
	p.setPos(10, 6);
	p.Show();
	cout << endl;

	//Aircon
	Airconditioner aircon(30);
	aircon.ShowCurrentTemperature();
	aircon.SetTargetTemperature(18);
	aircon.SetPower();
	aircon.SetTargetTemperature(19);

	while (aircon.GetCurrentTemperature() != aircon.GetTargetTemperature());
	aircon.ShowCurrentTemperature();
	aircon.SetPower();
	cout << endl;

	//Calculator
	char* expression1 = const_cast<char*>("10.2 + 20.5");
	char* expression2 = const_cast<char*>("30 * HAHAHAHA 10 /100 + Im fine thanks, And you ? 10 -5");
	char* expression3 = const_cast<char*>("3 * 3.14159");
	char* expression4 = const_cast<char*>("80/40");

	Calculator calculator;
	cout << expression1 << " = " << calculator.Calculate(expression1) << endl;
	cout << expression2 << " = " << calculator.Calculate(expression2) << endl;
	cout << expression3 << " = " << calculator.Calculate(expression3) << endl;
	cout << expression4 << " = " << calculator.Calculate(expression4) << endl;
}