#pragma once
#include "pch.h"
#include "object.h"
#include "point.h"
#include "timer.h"
#include "Map.h"
#include "type.h"
#include "list.h"
#include "singleton.h"
#include "gameSystem.h"

using namespace std;

class Game
{
	bool Enabled = false;

	Timer<void(void)> UpdateTimer;
	Point LastInput;
	bool LastSpaceInput;
	
	//관리 대상 객체
	Player player;
	LinkedList EnemyList;
	LinkedList BulletList;//Bullet을 player가...하..........

	float lastSpawnTime;
	static constexpr float SpawnTime = 4;

	const string CellSymbols[4];
	const int UpdateTick;

	uniform_int<int> mapWidthRange;
	uniform_int<int> mapHeightRange;

	mutex inputMutex;

	const Point GetRandomPosition() const
	{
		return Point(
			GameSystem::GetRandomValue(mapWidthRange),
			GameSystem::GetRandomValue(mapHeightRange));
	}

	const Point GetRandomPosition(uniform_int<> widthRange, uniform_int<> heightRange) const
	{
		return Point(
			GameSystem::GetRandomValue(widthRange),
			GameSystem::GetRandomValue(heightRange));
	}

	const string GetCellSymbol(const Object * const obj) const
	{
		ObjectType type = ObjectType::None;
		if (obj != nullptr)
		{
			type = obj->GetType();
		}

		switch (type)
		{
			case ObjectType::None:
			case ObjectType::Player:
			case ObjectType::Enemy:
			case ObjectType::Bullet:
				return CellSymbols[(int)type];
			default:
				return "E";
		}
	}

public:
	Game()
		:mapWidthRange(0, Map::MapWidth - 1),
		mapHeightRange(0, Map::MapHeight - 1),
		CellSymbols{ "  ","◈","□","·" },
		UpdateTick(16), player(Point(GameSystem::GetInstance().GetRandomValue(mapWidthRange), Map::MapHeight - 1))
	{
		std::function<void(void)> Update = std::bind(&Game::Update, this);

		UpdateTimer.Initialize(Update, UpdateTick);

		GameSystem::GetInstance().Initialize();

		Map::GetInstance().SetTargetCell(player.GetPosition(), &player);
	}

	void Start();

	void Destroy();

	~Game();

private:
	void Input();

	void Update();
	void UpdateScreen() const;
};