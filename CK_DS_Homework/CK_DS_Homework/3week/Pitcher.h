#pragma once
#include "Player.h"

class Pitcher : public Player
{
public:
	Pitcher() {}
	virtual ~Pitcher() {};

	virtual void SetValue() override;
};

