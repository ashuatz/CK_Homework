#include <iostream>
#include <random>

#include "Game.h"
#include "Cell.h"

using namespace std;

void Game::InitMap()
{
	Cells = new Cell*[MapSize];
	for (int i = 0; i < MapSize; ++i)
	{
		Cells[i] = new Cell[MapSize];
		for (int j = 0; j < MapSize; ++j)
		{
			Cells[i][j].Initialize(this,i, j);
		}
	}
}

void Game::InitMine()
{
	random_device rd;
	mt19937 mt(rd());
	uniform_int<int> random_range(0, MapSize - 1);

	for (currentMineCount = 0; currentMineCount < MineCount; )
	{
		int x = random_range(mt);
		int y = random_range(mt);
		if (Cells[x][y].GetValue() != Cell::MINE_VALUE)
		{
			Cells[x][y].SetMine();
			++currentMineCount;
		}
	}

	for (int i = 0; i < MapSize; ++i)
	{
		for (int j = 0; j < MapSize; ++j)
		{
			Cells[i][j].CheckAdjacentMine();
		}
	}
}

Cell * Game::TryGetCell(const int & x, const int & y) const
{
	if (x < 0 || y < 0 || x >= MapSize || y >= MapSize)
		return nullptr;

	return &Cells[x][y];
}

int Game::TryGetCellsValue(const int & x, const int & y)const
{
	if (x < 0 || y < 0 || x >= MapSize || y >= MapSize)
		return 0;

	return Cells[x][y].GetValue();
}

void Game::Render() const
{
	for (int i = 0; i < MapSize; ++i)
	{
		for (int j = 0; j < MapSize; ++j)
		{
			cout << ' ' << Cells[j][i].GetShowValue();
		}
		cout << endl;
	}
	cout << "remain Mine = " << currentMineCount << endl;
	cout << "Left Mouse Click to open || Space to Set Flag" << endl;
}

void Game::OpenCell(const int & x, const int & y)
{
	Cell*&& target = TryGetCell(x, y);

	if (target != nullptr)
	{
		isDead = target->CheckMine();
	}
}

bool Game::CheckGameEnd() const
{
	int count = 0;
	for (int i = 0; i < MapSize; ++i)
	{
		for (int j = 0; j < MapSize; ++j)
		{
			if (Cells[j][i].isFlagOn() && Cells[j][i].GetValue() == Cell::MINE_VALUE)
				count++;
		}
	}
	return count == MineCount;
}

void Game::SetFlagToCell(const int & x, const int & y)
{
	Cell*&& target = TryGetCell(x, y);

	if (target->TryToggleFlag())
	{
		if (target->isFlagOn())
			this->currentMineCount--;
		else
			this->currentMineCount++;
	}
}

Game::~Game()
{
	for (int i = 0; i < MapSize; ++i)
	{
		delete[] Cells[i];
	}
	delete[] Cells;
}