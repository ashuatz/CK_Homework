#include <iostream>
#include <Windows.h>
#include <string>
#include <conio.h>

#include "Map.h"
#include "util.h"

using namespace std;

int main()
{
	ScreenInit();
	int x, y;
	Map map;

	//Rendering
	ScreenClear();
	map.Render();
	ScreenFlipping();
	while(true)
	{
		if (_kbhit())
		{
			int key = _getch();
			if(key == 224)
				key = _getch();

			switch (key)
			{
				case   LEFT:	map.MoveCursor(-1, 0);	break;
				case   RIGHT:	map.MoveCursor(1, 0);	break;
				case   UP:		map.MoveCursor(0, -1);	break;
				case   DOWN:	map.MoveCursor(0, 1);	break;
				case   SPACE:	map.SetCell(WallType::Object);	break;
				case   ENTER:	map.SetCell(WallType::Wall);	break;
				default:
					break;
			}
		}

		ScreenClear();
		map.Render();
		ScreenFlipping();
	}

	system("pause");
	ScreenRelease();
}

//TODO : Input Module
//Game has Module