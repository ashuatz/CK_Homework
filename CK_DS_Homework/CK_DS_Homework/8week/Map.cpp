#include "Map.h"
#include "util.h"

bool Map::isOutOfRange(const int & x, const int & y) const
{
	return x < 0 || x >= Width || y < 0 || y >= Height;
}

string Map::GetSymbol(const WallType & target) const
{
	switch (target)
	{
		case WallType::None: return "бр";
		case WallType::Object: return "в├";
		case WallType::Wall: return "бс";
		default:
			return "";
	}
}

void Map::MoveCursor(const int & dx, const int & dy)
{
	if (isOutOfRange(dx + px, dy + py))
		return;

	px += dx;
	py += dy;
}

void Map::Render() const
{
	SetCursorReset();
	for (int y = 0; y < Height; ++y)
	{
		for (int x = 0; x < Width; ++x)
		{
			if (x == px && y == py)
			{
				ScreenPrint("б▐");
			}
			else
			{
				ScreenPrint(GetSymbol(cells[y][x]).c_str());
			}
		}
		ScreenPrint("\n");
	}

}

void Map::SetCell(const WallType & target)
{
	cells[py][px] = target;
}