#pragma once
class Game;

class Cell
{
private:
	Game* owner;

	__int8 value;
	__int8 x, y;
	bool Flag;
	bool isOpen;
public:
	static constexpr int MINE_VALUE = 9;

	Cell() : Flag{ false }, isOpen{ false }	{}

	//Initialize area
	void Initialize(Game * owner, const int& x, const int& y);
	void SetMine();
	void CheckAdjacentMine();

	//Access in Playing
	bool CheckMine();
	bool TryToggleFlag();

	//Get Stat
	const bool isFlagOn() const;
	const bool isOpened() const;
	const int GetValue() const;
	const char GetShowValue() const;

private:
	static void TryCheckMine(const Cell & self, const int & x, const int & y);
};