#pragma once
#include <string>

enum CellType
{
	NONE = 0, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT,
	MINE
};

class Cell
{
private:
	__int8 x, y;

	bool Flag;
	bool isOpen;

	int value;
public:
	static constexpr int OPEN_ERROR = -1;
	Cell() : Flag{ false }, isOpen{ false }, value(-1)	{}

	//Initialize area
	void Initialize(const int& x, const int& y);
	void SetValue(const CellType& value);

	//Access in Playing
	bool TryToggleFlag();
	int Open();

	//Get Stat
	const bool isFlagOn() const { return Flag; }
	const bool isOpened() const { return isOpen; }
	const int GetValue() const { return value; }
	std::string GetShowValue() const;
};