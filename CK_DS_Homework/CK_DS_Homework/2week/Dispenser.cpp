#include "Dispenser.h"
#include <iostream>

void Dispenser::PushButton(Button * caller)
{
	std::string output = motor.RunMotor(caller->buttonName);
	std::cout << output << " ��ȯ" << std::endl;
}