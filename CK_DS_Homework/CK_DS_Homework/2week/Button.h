#pragma once
#include <string>

class Dispenser;
class Button
{
private :
	Dispenser* Owner;
public:
	std::string buttonName;

	Button(Dispenser* owner, std::string name)
	{
		buttonName = name;
		Owner = owner;
	}

	void PushButton();
};