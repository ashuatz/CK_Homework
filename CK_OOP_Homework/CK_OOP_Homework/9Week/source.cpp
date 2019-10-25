#include <iostream>
#include <random>
#include <vector>
#include <functional>
#include <conio.h>
#include <string>

#include "util.h"
#include "timer.h"

using namespace std;


template <typename T>
int CustomRemove(std::vector<T>* target, const T& rhs)
{
	auto lastSize = target->size();
	auto end = target->erase(std::remove_if(target->begin(), target->end(),
		[=](const T& t)
	{
		return t == rhs;
	}), target->end());

	return lastSize - std::distance(target->begin(), end);
}


class Point
{
public:
	int x, y;

	Point() :x(0), y(0) {}
	Point(const int& x, const int& y) :x(x), y(y) {}

	Point& operator+=(const Point& rhs)
	{
		this->x += rhs.x;
		this->y += rhs.y;
		return *this;
	}
	Point& operator-=(const Point& rhs)
	{
		this->x -= rhs.x;
		this->y -= rhs.y;
		return *this;
	}

	const Point operator+(const Point& rhs) const
	{
		return Point{ this->x + rhs.x,this->y + rhs.y };
	}

	const Point operator-(const Point& rhs) const
	{
		return Point{ this->x - rhs.x,this->y - rhs.y };
	}

	const bool operator==(const Point& rhs) const
	{
		return this->x == rhs.x && this->y == rhs.y;
	}
	const bool operator!=(const Point& rhs) const
	{
		return this->x != rhs.x || this->y != rhs.y;
	}
};





enum class CellType
{
	ERROR = -1,
	None = 0,
	Player,
	Item,
	Enemy,

};

class Map
{
private:
	const Point MapMin;
	const Point MapMax;

public:
	static constexpr int MapWidth = 10;
	static constexpr int MapHeight = 10;

	CellType** m;
	Map() : MapMin(Point(0, 0)), MapMax(Point(MapWidth, MapHeight))
	{
		m = new CellType*[10];
		for (int i = 0; i < 10; ++i)
		{
			m[i] = new CellType[10];
			for (int j = 0; j < 10; ++j)
			{
				m[i][j] = CellType::None;
			}
		}
	}

	void SetTargetCell(const Point& pos, const CellType& target)
	{
		if (!CheckAccess(pos))
			return;

		m[pos.y][pos.x] = target;
	}

	const CellType GetTargetCell(const Point& pos)
	{
		if (!CheckAccess(pos))
			return CellType::ERROR;

		return m[pos.y][pos.x];
	}

	bool CheckAccess(const Point& value)
	{
		if (MapMin.x > value.x || MapMax.x <= value.x)
			return false;
		if (MapMin.y > value.y || MapMax.y <= value.y)
			return false;

		return true;
	}

};

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
	Game() :mt((random_device())()), currentScore(0), mapRange(0, 9), CellSymbols{ "□","◈","◇","◆" }
	{
		std::function<void(void)> SpawnEnemy = std::bind(&Game::SpawnEnemy,this);
		std::function<void(void)> SpawnItem = std::bind(&Game::SpawnItem,this);
		std::function<void(void)> Update = std::bind(&Game::Update,this);

		EnemySpawnTimer.Initialize(SpawnEnemy, EnemySpawnTime * 1000);
		ItemSpawnTimer.Initialize(SpawnItem, ItemSpawnTime * 1000);
		UpdateTimer.Initialize(Update, 16);
	}

	void Start()
	{
		ScreenInit();

		Enabled = true;

		EnemySpawnTimer.start();
		ItemSpawnTimer.start();
		UpdateTimer.start();

		PlayerPosition = Point(0, 0);
		map.SetTargetCell(PlayerPosition, CellType::Player);

		Input();
	}

	void Destroy()
	{
		UpdateTimer.stop();
		ItemSpawnTimer.stop();
		EnemySpawnTimer.stop();

		Enabled = false;
	}

	~Game()
	{
		if (Enabled)
			Destroy();

		ScreenRelease();
	}

private:
	void Input()
	{
		while (Enabled)
		{
			while (!_kbhit());

			//input and processing
			int code = _getch();
			if (code == VK)
				code = _getch(); // for vk

			bool isPass = false;
			switch (code)
			{
				case DOWN:	LastInput = Point(0, 1); break;
				case UP:	LastInput = Point(0, -1); break;
				case LEFT:	LastInput = Point(-1, 0); break;
				case RIGHT:	LastInput = Point(1, 0); break;
				default: break;
			}

		}
	}

	void OnPlayerInput(const Point& delta)
	{
		if (!map.CheckAccess(PlayerPosition + delta))
			return;

		map.SetTargetCell(PlayerPosition, CellType::None);
		PlayerPosition += delta;
		map.SetTargetCell(PlayerPosition, CellType::Player);
	}

	void Update()
	{
		//Time
		remainEnemySpawnTime -= 16 * 0.001f;
		remainItemSpawnTime -= 16 * 0.001f;

		//Collision
		int enemyRemoveCount = CustomRemove(&EnemyPositions, PlayerPosition);
		if (enemyRemoveCount > 0)
		{
			//Player and Enemy Contact
			enemyRemoveCount = 0;
			Destroy();

			//enable = false
			//terminate game
		}

		int itemRemoveCount = CustomRemove(&ItemPositions, PlayerPosition);
		if (itemRemoveCount > 0)
		{
			//Player and player Contanct
			itemRemoveCount = 0;
			currentScore += 1;
			//add Score
		}

		//Movement
		if (LastInput != Point(0, 0))
		{
			OnPlayerInput(LastInput);
			LastInput = Point(0, 0);
		}
		
		//Render
		UpdateScreen();
	}

	void UpdateScreen()
	{
		SetCursorReset();
		//rendering UI

		//나올 정보 : 몬스터 모양, 몬스터 스폰까지 남은 시간
		ScreenPrint(string("Enemy : ")
			.append(GetCellSymbol(CellType::Enemy))
			.append("   | Next Spawn : ")
			.append(std::to_string(remainEnemySpawnTime).substr(0,3))
			.append("\n")
			.c_str());

		//나올 정보 : 아이템 모양, 아이템 스폰까지 남은 시간
		ScreenPrint(string("Item  : ")
			.append(GetCellSymbol(CellType::Item))
			.append("   | Next Spawn : ")
			.append(std::to_string(remainItemSpawnTime).substr(0, 3))
			.append("\n")
			.c_str());

		//나올 정보 : 현재 스코어
		ScreenPrint(string("Score : ")
			.append(std::to_string(currentScore))
			.append("\n")
			.c_str());

		ScreenPrint("\n");

		//rendering map
		for (int y = 0; y < map.MapHeight; ++y)
		{
			for (int x = 0; x < map.MapWidth; ++x)
			{
				ScreenPrint(GetCellSymbol(map.GetTargetCell(Point(x, y))).c_str());
			}
			ScreenPrint("\n");
		}

		if (!Enabled)
		{
			ScreenPrint("종료되었습니다 \n");
		}

		ScreenFlipping();
	}

	void SpawnEnemy()
	{
		//Not Good performance.
		//확률적 알고리즘. 초반 절반까지는 기대이상의성능을 낼 수 있지만, 절반을 넘어간다면 현저히 느려질 수 있다.
		Point target;
		while(true)
		{
			target = Point(GetRandom(), GetRandom());

			if (map.GetTargetCell(target) == CellType::None)
				break;
		}

		EnemyPositions.emplace_back(target);

		map.SetTargetCell(target, CellType::Enemy);
		remainEnemySpawnTime = EnemySpawnTime;
	}

	void SpawnItem()
	{
		Point target;
		while (true)
		{
			target = Point(GetRandom(), GetRandom());

			if (map.GetTargetCell(target) == CellType::None)
				break;
		}

		ItemPositions.emplace_back(target);

		map.SetTargetCell(target, CellType::Item);
		remainItemSpawnTime = ItemSpawnTime;
	}
};



int main()
{
	Game game;
	game.Start();
}