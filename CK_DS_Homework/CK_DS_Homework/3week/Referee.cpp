#include "Referee.h"
#include <string>

int Referee::Compare(Player & p1, Player & p2)
{
	LastStrikeCount = 0;
	LastBallCount = 0;

	auto&& a_ = p1.GetValue();
	auto&& b_ = p2.GetValue();

	for (int i = 0; i < Player::InputCount; ++i)
	{
		auto&& foundIndex = b_.find(a_[i]);

		//strike
		if (foundIndex == i)
			LastStrikeCount++;

		else if (foundIndex != std::string::npos)
			LastBallCount++;
	}

	return LastStrikeCount * Player::InputCount + LastBallCount;
}