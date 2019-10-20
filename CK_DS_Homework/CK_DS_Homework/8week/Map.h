#pragma once
#include <string>

using namespace std;

enum class WallType
{
	None = 0,
	Object,
	Wall,
};

class Map
{
private:
	WallType** cells;
	int px, py;
	static constexpr int Height = 20;
	static constexpr int Width = 30;

	bool isOutOfRange(const int& x, const int& y) const;
	string GetSymbol(const WallType& target) const;
public:
	Map()
	{
		cells = new WallType*[Height];
		for (int i = 0; i < Height; ++i)
		{
			cells[i] = new WallType[Width]{ WallType::None };
		}
	}

	void MoveCursor(const int& dx, const int& dy);
	void Render() const;
	void SetCell(const WallType& target);

	~Map()
	{
		for (int i = 0; i < Height; ++i)
		{
			delete[] cells[i];
		}
		delete[] cells;
	}
};