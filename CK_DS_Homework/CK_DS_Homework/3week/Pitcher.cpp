#include "Pitcher.h"
#include <random>

void Pitcher::SetValue()
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int<int> uniform(0, 9);

	for (int i = 0; i < InputCount; )
	{
		std::string temp = std::to_string(uniform(mt));

		if (this->value.find(temp) != std::string::npos)
			continue;

		this->value.append(temp);

		++i;
	}
}

