#include "Hitter.h"
#include <iostream>

void Hitter::SetValue()
{
	std::string input;

	while (true)
	{
		bool pass = true;

		std::cout << "0~9 숫자 " << InputCount << "자리 입력 : ";
		std::cin >> input;

		//Compare size
		if (input.size() != InputCount)
			continue;
		
		//check duplicated value
		for (auto& it : input)
		{
			//input string is must contains separator. so, find character in string after separator.
			auto&& start = input.find(it) + 1;
			pass &= input.find(it, start) == std::string::npos;
		}

		if (!pass)
			continue;

		//allow to setup
		value = input;
		break;
	}
}
