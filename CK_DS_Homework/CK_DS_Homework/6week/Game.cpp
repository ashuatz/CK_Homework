#include <iostream>
#include <random>

#include "Game.h"
#include "Cell.h"

using namespace std;

void Game::InitMap()
{
	Cells = new Cell*[MapSize];
	for (int y = 0; y < MapSize; ++y)
	{
		Cells[y] = new Cell[MapSize];
		for (int x = 0; x < MapSize; ++x)
		{
			Cells[y][x].Initialize(y, x);
		}
	}
}

void Game::InitMine()
{
	mt19937 mt((random_device())());
	uniform_int<int> random_range(0, MapSize - 1);

	for (currentMineCount = 0; currentMineCount < MineCount; )
	{
		int x = random_range(mt);
		int y = random_range(mt);
		if (Cells[y][x].GetValue() == CellType::MINE)
			continue;

		Cells[y][x].SetValue(CellType::MINE);

		for (int dy = -1; dy <= 1; ++dy)
		{
			for (int dx = -1; dx <= 1; ++dx)
			{
				if (isOutRange(x + dx, y + dy))
					continue;

				Cells[y + dy][x + dx].SetValue((CellType)(Cells[y + dy][x + dx].GetValue() + 1));
			}
		}

		++currentMineCount;
	}
}

bool Game::CheckGameEnd() const
{
	if (isDead)
		return true;

	int count = 0;
	for (int y = 0; y < MapSize; ++y)
	{
		for (int x = 0; x < MapSize; ++x)
		{
			if (isOutRange(x, y))
				continue;

			if (Cells[y][x].isFlagOn() && Cells[y][x].GetValue() != CellType::MINE)
				return false;

			if (Cells[y][x].isFlagOn() && Cells[y][x].isOpened() == false)
				count++;
		}
	}

	return count == MineCount;
}

void Game::Render() const
{
	for (int y = 0; y < MapSize; ++y)
	{
		for (int x = 0; x < MapSize; ++x)
		{
			auto&& value = Cells[y][x].GetShowValue();
			cout << value;
		}
		cout << endl;
	}
	cout << "remain Mine = " << currentMineCount << endl;
	cout << "Left Mouse Click to open || Space to Set Flag" << endl;
}

void Game::OpenCell(const int & x, const int & y)
{
	if (isOutRange(x, y))
		return;

	int result = Cells[y][x].Open();
	if (result == Cell::OPEN_ERROR)
		return;

	if (result == CellType::MINE)
	{
		isDead = true;
		return;
	}
	
	if (result != CellType::NONE)
		return;

	for (int dx = -1; dx <= 1; ++dx)
	{
		for (int dy = -1; dy <= 1; ++dy)
		{
			if (isOutRange(x + dx, y + dy))
				continue;

			OpenCell(x + dx, y + dy);
		}
	}
}

void Game::SetFlagToCell(const int & x, const int & y)
{
	if (isOutRange(x, y))
		return;

	bool isSuccess = Cells[y][x].TryToggleFlag();
	if (!isSuccess)
		return;

	if (Cells[y][x].isFlagOn())
		this->currentMineCount--;
	else
		this->currentMineCount++;
}

Game::~Game()
{
	for (int i = 0; i < MapSize; ++i)
	{
		delete[] Cells[i];
	}
	delete[] Cells;
}