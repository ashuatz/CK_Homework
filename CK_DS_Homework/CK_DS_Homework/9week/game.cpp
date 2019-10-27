#include "game.h"
#include "util.h"

void Game::Start()
{
	ScreenInit();

	Enabled = true;

	ItemSpawnTimer.start();
	UpdateTimer.start();

	PlayerPosition = Point(0, 0);
	map.SetTargetCell(PlayerPosition, CellType::Player);

	Input();
}

void Game::Destroy()
{
	UpdateTimer.stop();
	ItemSpawnTimer.stop();
	Enabled = false;
}

Game::~Game()
{
	if (Enabled)
		Destroy();

	ScreenRelease();
}

void Game::Input()
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

void Game::OnPlayerInput(const Point & delta)
{
	if (!map.CheckAccess(PlayerPosition + delta))
		return;

	map.SetTargetCell(PlayerPosition, CellType::None);
	PlayerPosition += delta;
	map.SetTargetCell(PlayerPosition, CellType::Player);
}

void Game::Update()
{
	//Time
	remainItemSpawnTime -= 16 * 0.001f;

	//Collision
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

void Game::UpdateScreen() const
{
	SetCursorReset();
	//rendering UI

	//나올 정보 : 아이템 모양, 아이템 스폰까지 남은 시간
	string leftItemTime = remainItemSpawnTime > 0 ? std::to_string(remainItemSpawnTime).substr(0, 3) : "----";
	ScreenPrint(string("Item  : ")
		.append(GetCellSymbol(CellType::Item))
		.append("   | Next Spawn : ")
		.append(leftItemTime)
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


void Game::SpawnItem()
{
	if (MaxItemCount <= ItemCount)
		return;

	Point target;
	while (true)
	{
		target = Point(GetRandom(), GetRandom());

		if (map.GetTargetCell(target) == CellType::None)
			break;
	}

	ItemPositions.emplace_back(target);
	ItemCount++;

	map.SetTargetCell(target, CellType::Item);
	remainItemSpawnTime = ItemSpawnTime;
}
