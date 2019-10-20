#pragma once
#include <string>

class Player
{
protected:
	std::string value;

public:
	static const int InputCount;

	Player() {}
	virtual ~Player() {}

	virtual void SetValue() = 0;
	std::string GetValue() const { return value; }
};

