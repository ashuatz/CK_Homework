#pragma once
#include "Motor.h"
#include "Button.h"

class Dispenser
{
private:
	Motor motor;

public:
	Button WaterButton = Button(this, "Water");
	Button IceButton = Button(this, "Ice");
	Button SlushButton = Button(this, "Slush");

	void PushButton(Button* caller);
};