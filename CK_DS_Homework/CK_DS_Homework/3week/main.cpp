#include "Player.h"
#include "Hitter.h"
#include "Pitcher.h"
#include "Referee.h"
#include <iostream>
int main()
{
	Referee referee;

	Player* user = new Hitter();
	Player* computer = new Pitcher();
	computer->SetValue();

	const int playCount = 10;
	int turnCount;
	for (turnCount = 1; turnCount <= playCount; turnCount++)
	{
		user->SetValue();

		int&& result = referee.Compare(*user, *computer);

		if (result == 0)
		{
			std::cout << "OUT!" << std::endl;
		}
		else
		{
			std::cout << referee.getLastStrikeCount() << " Strike" << std::endl;
			std::cout << referee.getLastBallCount() << " Ball" << std::endl;
		}

		if (referee.getLastStrikeCount() == Player::InputCount)
			break;

		std::cout << "Turn end" << std::endl << std::endl;
	}

	std::cout << "Game Set. " << turnCount << "/" << playCount << std::endl;

	delete computer;
	delete user;
}
