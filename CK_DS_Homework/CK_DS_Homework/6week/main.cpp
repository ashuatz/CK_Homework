#include <iostream>
#include <Windows.h>

#include "Game.h"

using namespace std;

constexpr int FontXOffset = 4;
constexpr int FontXSize = 8 + FontXOffset * 2;
constexpr int FontYSize = 16;

bool current[256]{ false, };
bool last[256]{ false, };

int GetMouseClick(int vk) {
	if (current[vk] && last[vk]) return 2;
	if (!current[vk] && last[vk]) return 1;
	if (current[vk] && !last[vk]) return -1;
	return -2;
}

int main()
{
	Game game;
	game.Render();
	int x, y;
	while (!game.CheckGameEnd())
	{
		//wait for input
		int LButtonState = 0;
		int RButtonState = 0;
		while (RButtonState != 1 && LButtonState != 1)
		{
			for (int i = 0; i < 256; i++)
			{
				last[i] = current[i];
				current[i] = GetAsyncKeyState(i) & 0x8000;
			}
			LButtonState = GetMouseClick(VK_LBUTTON);
			RButtonState = GetMouseClick(VK_SPACE);
		}
		system("cls");

		POINT target;
		GetCursorPos(&target);
		ScreenToClient(GetConsoleWindow(), &target);

		if (RButtonState == 1)
		{
			game.SetFlagToCell((target.x - FontXOffset) / FontXSize, target.y / FontYSize);
		}
		else if (LButtonState == 1)
		{
			game.OpenCell((target.x - FontXOffset) / FontXSize, target.y / FontYSize);
		}
		game.Render();

		cout << "Target : " << (target.x - FontXOffset) / FontXSize << "," << target.y / FontYSize << endl;
	}

	if (!game.GetIsDead())
	{
		cout << "½Â¸®! Á¾·á!" << endl;
	}
	else
	{
		cout << "BOOM" << endl;
	}
}