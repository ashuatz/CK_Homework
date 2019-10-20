#pragma once

class Cell;
class Game
{
private:
	static constexpr int MapSize = 10;
	static constexpr int MineCount = 10;

	bool isDead;
	Cell** Cells;
	int currentMineCount;

	void InitMap();
	void InitMine();

	bool isOutRange(const int& x, const int& y) const
	{
		return x < 0 || x >= MapSize || y < 0 || y >= MapSize;
	}

public:

	Game() : isDead{ false }, currentMineCount{ 0 }
	{
		InitMap();
		InitMine();
	}

	bool GetIsDead() const { return isDead; }
	bool CheckGameEnd() const;
	void Render() const;

	void OpenCell(const int & x, const int & y);
	void SetFlagToCell(const int & x, const int & y);

	~Game();
};
