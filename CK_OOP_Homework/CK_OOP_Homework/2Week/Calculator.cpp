#include "Calculator.h"

float Calculator::Add(const float & rhs, const float & lhs)
{
	return rhs + lhs;
}

float Calculator::Remove(const float & rhs, const float & lhs)
{
	return rhs - lhs;
}

float Calculator::Multi(const float & rhs, const float & lhs)
{
	return rhs * lhs;
}

float Calculator::Divide(const float & rhs, const float & lhs)
{
	return rhs / lhs;
}

int Calculator::GetLength(float value) const
{
	int size = 0;
	if (value - (int)value > 0)
	{
		size += 1;
		while (value - (int)value > 0)
		{
			value *= 10;
		}
	}

	while ((int)value > 0)
	{
		size++;
		value *= 0.1;
	}

	return size;
}

float Calculator::Calculate(char * expression)
{
	int index = 0;
	bool isOperatorInput = false;
	float arr[10]{ 0 };

	float value = 0;
	for (char* it = expression; (*it) != '\0'; it++)
	{
		if ((*it) <= '9' && (*it) >= '0')
		{
			arr[index] = atof(&(*it));
			it += GetLength(arr[index]) - 1;	//it++를 할 것이기 때문에 다음자리로 하면 안됨

			if (isOperatorInput)
			{
				isOperatorInput = false;
				float value = (this->*current)(arr[index - 1], arr[index]);
				arr[--index] = value;
			}
			index++;
		}
		else
		{
			switch ((*it))
			{
				case '+':current = &Calculator::Add; isOperatorInput = true; break;
				case '-':current = &Calculator::Remove; isOperatorInput = true; break;
				case '*':current = &Calculator::Multi; isOperatorInput = true; break;
				case '/':current = &Calculator::Divide; isOperatorInput = true; break;

				default:
					break;
			}
		}
	}

	return arr[0];
}