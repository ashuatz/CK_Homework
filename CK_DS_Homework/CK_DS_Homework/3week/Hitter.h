#pragma once
#include "Player.h"

class Hitter : public Player
{
public:
	Hitter() {}
	virtual ~Hitter() {}

	virtual void SetValue() override;
};

