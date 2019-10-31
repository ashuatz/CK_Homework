#pragma once
#include "pch.h"
#include "point.h"
#include "timer.h"
#include "Map.h"
#include "type.h"
#include "list.h"

using namespace std;

class Game
{
	bool Enabled = false;
	PositionList ItemPositions;

	Timer<void(void)> UpdateTimer;
	Timer<void(void)> ItemSpawnTimer;

	float remainItemSpawnTime;
	static constexpr float ItemSpawnTime = 5;

	Map map;
	Point LastInput;
	SnakeList Player;

	int ItemCount;
	static constexpr int MaxItemCount = Map::MapHeight * Map::MapWidth;

	const string CellSymbols[4];
	const int UpdateTick;

	mt19937 mt;
	uniform_int<int> mapRange;

	int GetRandom() { return mapRange(mt); }

	const string GetCellSymbol(const ObjectType& type) const
	{
		switch (type)
		{
			case ObjectType::None:
			case ObjectType::PlayerHead:
			case ObjectType::Item:
			case ObjectType::PlayerBody:
				return CellSymbols[(int)type];
			default:
				return "E";
		}
	}

public:
	Game()
		:mt((random_device())()), ItemCount(0),
		mapRange(0, 9), CellSymbols{ "¡à","¢Â","¡ß","¡Þ" },
		Player(1, 2), UpdateTick(128)
	{
		std::function<void(void)> SpawnItem = std::bind(&Game::SpawnItem, this);
		std::function<void(void)> Update = std::bind(&Game::Update, this);

		ItemSpawnTimer.Initialize(SpawnItem, ItemSpawnTime * 1000);
		UpdateTimer.Initialize(Update, UpdateTick);
	}

	void Start();

	void Destroy();

	~Game();

private:
	void Input();

	void Update();
	void UpdateScreen() const;

	void SpawnItem();
};