#include "Motor.h"
#include <iostream>

std::string Motor::RunMotor(std::string targetName) const
{
	std::cout << targetName << " ����" << std::endl;
	return targetName;
}
