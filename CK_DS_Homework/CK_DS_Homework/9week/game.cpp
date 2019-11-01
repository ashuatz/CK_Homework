#include "game.h"
#include "util.h"

void Game::Start()
{
	ScreenInit();

	Enabled = true;

	ItemSpawnTimer.start();
	UpdateTimer.start();

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

void Game::Update()
{
	//Time
	remainItemSpawnTime -= UpdateTick * 0.001f;

	bool isAdded = false;

	//Collision with Item
	int targetIndex = ItemPositions.IndexOf(Player[0]->GetPosition());
	if (targetIndex != -1)
	{
		ItemPositions.Remove(targetIndex);

		Player.CreateBodyToFirst(Player[0]->GetPosition());
		isAdded = true;
	}


	//Movement
	int count = Player.GetLength();
	for (int i = 0; i < count; ++i)
	{
		map.SetTargetCell(Player[i]->GetPosition(), ObjectType::None);
	}

	Player[0]->SetDir(LastInput);

	if (isAdded)
	{
		Player[0]->SetPosition(Player[0]->GetPosition() + Player[0]->GetDirection());
		Player[1]->SetDir(LastInput);
	}
	else
	{
		Player.Update();
	}
	
	for (int i = 0; i < count; ++i)
	{
		map.SetTargetCell(Player[i]->GetPosition(), Player[i]->GetType());
	}


	//Check Game End
	//Head Contact Body
	if (map.GetTargetCell(Player[0]->GetPosition()) == ObjectType::PlayerBody)
	{
		Destroy();
	}

	//Out of Map
	for (int i = 0; i < count; ++i)
	{
		if (!map.CheckAccess(Player[i]->GetPosition()))
		{
			this->Enabled = false;
			break;
		}
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
		.append(GetCellSymbol(ObjectType::Item))
		.append("   | Next Spawn : ")
		.append(leftItemTime)
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

		if (map.GetTargetCell(target) == ObjectType::None)
			break;
	}

	ItemPositions.Add(target);
	ItemCount++;

	map.SetTargetCell(target, ObjectType::Item);
	remainItemSpawnTime = ItemSpawnTime;
}
