#include "Cell.h"
#include "Game.h"

#include <iostream>
#include <string>

using namespace std;

void Cell::TryCheckMine(const Cell & self, const int & target_x, const int & target_y)
{
	Cell* target;
	target = self.owner->TryGetCell(target_x, target_y);

	if (target != nullptr)
		target->CheckMine();
}

void Cell::SetMine()
{
	this->value = MINE_VALUE;
}

void Cell::CheckAdjacentMine()
{
	if (this->value == MINE_VALUE)
	{
		return;
	}
	if (owner == nullptr)
	{
		cout << "[Cell] : Set Owner first" << endl;
		return;
	}

	if (owner->currentMineCount != owner->MineCount)
	{
		cout << "[Cell] : Set Mine first" << endl;
		return;
	}

	this->value += owner->TryGetCellsValue(x + 1, y + 1) == MINE_VALUE;
	this->value += owner->TryGetCellsValue(x + 1, y - 1) == MINE_VALUE;
	this->value += owner->TryGetCellsValue(x + 1, y) == MINE_VALUE;

	this->value += owner->TryGetCellsValue(x - 1, y + 1) == MINE_VALUE;
	this->value += owner->TryGetCellsValue(x - 1, y - 1) == MINE_VALUE;
	this->value += owner->TryGetCellsValue(x - 1, y) == MINE_VALUE;

	this->value += owner->TryGetCellsValue(x, y + 1) == MINE_VALUE;
	this->value += owner->TryGetCellsValue(x, y - 1) == MINE_VALUE;
}

bool Cell::TryToggleFlag()
{
	if (isOpen)
		return false;

	this->Flag = !Flag;
	return true;
}

void Cell::Initialize(Game * owner,const int& x, const int& y)
{
	this->owner = owner;
	isOpen = false;
	this->x = x;
	this->y = y;
	this->value = 0;
}

bool Cell::CheckMine()
{
	if (isOpen)
		return false;

	if (Flag)
		return false;

	isOpen = true;

	if (this->value == MINE_VALUE)
		return true;

	if (this->value == 0)
	{
		TryCheckMine(*this, x + 1, y + 1);
		TryCheckMine(*this, x + 1, y - 1);
		TryCheckMine(*this, x + 1, y    );

		TryCheckMine(*this, x - 1, y + 1);
		TryCheckMine(*this, x - 1, y - 1);
		TryCheckMine(*this, x - 1, y    );

		TryCheckMine(*this, x    , y + 1);
		TryCheckMine(*this, x    , y - 1);
	}

	return false;
}

const int Cell::GetValue() const
{
	return value;
}

const char Cell::GetShowValue() const
{
	if (Flag)
		return 'F';

	if (!isOpen)
		return '?';

	if (value == MINE_VALUE)
		return '*';

	return std::to_string(value).c_str()[0];
}

const bool Cell::isFlagOn() const
{
	return Flag;
}

const bool Cell::isOpened() const
{
	return isOpen;
}
