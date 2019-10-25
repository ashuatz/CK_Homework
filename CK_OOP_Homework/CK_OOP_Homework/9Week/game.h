#pragma once
#include "pch.h"
#include "point.h"
#include "timer.h"
#include "Map.h"
#include "type.h"

using namespace std;

class Game
{
	mt19937 mt;
	uniform_int<int> mapRange;

	vector<Point> EnemyPositions;
	vector<Point> ItemPositions;

	Timer<void(void)> EnemySpawnTimer;
	float remainEnemySpawnTime;
	static constexpr float EnemySpawnTime = 10;

	Timer<void(void)> ItemSpawnTimer;
	float remainItemSpawnTime;
	static constexpr float ItemSpawnTime = 5;

	Timer<void(void)> UpdateTimer;

	const string CellSymbols[4];

	Point LastInput;
	Point PlayerPosition;
	Map map;

	int ItemCount;
	static constexpr int MaxItemCount = Map::MapHeight * Map::MapWidth;
	int EnemyCount;
	static constexpr int MaxEnemyCount = 10;

	int currentScore;

	bool Enabled = false;

	int GetRandom() { return mapRange(mt); }

	const string GetCellSymbol(const CellType& type) const
	{
		switch (type)
		{
			case CellType::None:
			case CellType::Player:
			case CellType::Item:
			case CellType::Enemy:
				return CellSymbols[(int)type];
			default:
				return "E";
		}
	}

public:
	Game() :mt((random_device())()), currentScore(0), mapRange(0, 9), CellSymbols{ "¡à","¢Â","¡Þ","¡ß" }
	{
		std::function<void(void)> SpawnEnemy = std::bind(&Game::SpawnEnemy, this);
		std::function<void(void)> SpawnItem = std::bind(&Game::SpawnItem, this);
		std::function<void(void)> Update = std::bind(&Game::Update, this);

		EnemySpawnTimer.Initialize(SpawnEnemy, EnemySpawnTime * 1000);
		ItemSpawnTimer.Initialize(SpawnItem, ItemSpawnTime * 1000);
		UpdateTimer.Initialize(Update, 16);
	}

	void Start();

	void Destroy();

	~Game();

private:
	void Input();
	void OnPlayerInput(const Point& delta);

	void Update();
	void UpdateScreen() const;

	void SpawnEnemy();
	void SpawnItem();
};