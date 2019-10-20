#pragma once

class Cell;
class Game
{
private:
	void InitMap();
	void InitMine();

	Cell** Cells;
public:

	static constexpr int MapSize = 10;
	static constexpr int MineCount = 10;
	int currentMineCount;

	bool isDead;

	Game() : isDead{ false }, currentMineCount{ 0 }
	{
		InitMap();
		InitMine();
	}

	Cell* TryGetCell(const int & x, const int & y) const;

	int TryGetCellsValue(const int & x, const int & y)const;

	bool CheckGameEnd() const;
	void Render() const;

	void OpenCell(const int & x, const int & y);
	void SetFlagToCell(const int & x, const int & y);

	~Game();
};
