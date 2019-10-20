#pragma once
#include "Player.h"

class Referee
{
private:
	int LastStrikeCount;
	int LastBallCount;

public:
	Referee() {}
	~Referee() {}

	int Compare(Player& a, Player& b);
	const int getLastStrikeCount() const { return LastStrikeCount; }
	const int getLastBallCount() const { return LastBallCount; }
};

