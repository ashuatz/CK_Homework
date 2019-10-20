#include "Cell.h"

void Cell::Initialize(const int & x, const int & y)
{
	this->x = x;
	this->y = y;
	Flag = false;
	isOpen = false;
	value = CellType::NONE;
}

void Cell::SetValue(const CellType & value)
{
	if (this->value == CellType::MINE)
		return;

	this->value = value;
}

bool Cell::TryToggleFlag()
{
	if (isOpen)
		return false;

	this->Flag = !Flag;
	return true;
}

int Cell::Open()
{
	if (isOpen)
		return Cell::OPEN_ERROR;

	if (Flag)
		return Cell::OPEN_ERROR;

	isOpen = true;

	return this->value;
}

std::string Cell::GetShowValue() const
{

	if (Flag)
		return std::string("¢Ã");
	if (!isOpen)
		return std::string("¡à");
	if (value == CellType::MINE)
		return std::string("¡Ø");
	if (value == CellType::NONE)
		return std::string("  ");

	return std::to_string(value).append(" ");
}
