#include "game.h"
#include "util.h"

void Game::Start()
{
	ScreenInit();

	Enabled = true;

	UpdateTimer.start();

	Input();
}

void Game::Destroy()
{
	UpdateTimer.stop();
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
			case SPACE: LastSpaceInput = true; break;
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
	bool isAdded = false;

	//input
	if (LastInput != Point(0, 0))
	{
		player.MovePosition(LastInput);
		LastInput = Point(0, 0);
	}
	if (LastSpaceInput)
	{
		//MakeBullet
		auto bullet = dynamic_cast<Bullet*>(GameSystem::GetInstance().SpawnObject(ObjectType::Bullet));
		if (bullet != nullptr)
		{
			BulletList.PushBack(bullet);
			bullet->Initialize(player.GetPosition() + Point(0, -1));
		}

		LastSpaceInput = false;
	}

	//Entity Update
	for (int i = 0; i < BulletList.Count(); ++i)
	{
		auto bullet = dynamic_cast<Bullet*>(BulletList.retrieve(i));
		if (bullet != nullptr)
		{
			bullet->Update();
		}
	}

	for (int i = 0; i < EnemyList.Count(); ++i)
	{
		auto enemy = dynamic_cast<Enemy*>(EnemyList.retrieve(i));
		if (enemy != nullptr)
		{
			enemy->Update();
		}
	}

	//SpawnMonster
	if (GameSystem::GetInstance().GetTime() > lastSpawnTime + SpawnTime)
	{
		//Find EmptyCell
		bool pass = false;
		Point spawnPosition;
		while (!pass)
		{
			spawnPosition = GetRandomPosition(mapWidthRange, uniform_int<>(0, 2));
			if (Map::GetInstance().GetTargetCell(spawnPosition) == nullptr)
			{
				break;
			}
		}

		//Create Enemy to Empty cell
		auto enemy = dynamic_cast<Enemy*>(GameSystem::GetInstance().SpawnObject(ObjectType::Enemy));
		if (enemy != nullptr)
		{
			EnemyList.PushBack(enemy);
			enemy->Initialize(GetRandomPosition(mapWidthRange, uniform_int<>(0, 2)));
		}

		lastSpawnTime = GameSystem::GetInstance().GetTime();
	}

	//Collecting unUseEntity
	for (int i = 0; i < BulletList.Count();)
	{
		//O(N log N) cost...bad...
		auto bullet = dynamic_cast<Bullet*>(BulletList.retrieve(i));
		if (bullet != nullptr && bullet->GetTier() < 0)
		{
			auto target = dynamic_cast<Bullet*>(BulletList.remove(i));
			Map::GetInstance().SetTargetCell(target->GetPosition(), nullptr);
			GameSystem::GetInstance().ReleaseObject(target);
		}
		else
		{
			++i;
		}
	}

	for (int i = 0; i < EnemyList.Count();)
	{
		auto enemy = dynamic_cast<Enemy*>(EnemyList.retrieve(i));
		if (enemy != nullptr && enemy->GetTier() < 0)
		{
			auto target = dynamic_cast<Enemy*>(EnemyList.remove(i));
			Map::GetInstance().SetTargetCell(target->GetPosition(), nullptr);
			GameSystem::GetInstance().ReleaseObject(target);
		}
		else
		{
			++i;
		}
	}

	//Render
	UpdateScreen();
}

void Game::UpdateScreen() const
{
	SetCursorReset();

	//rendering UI
	auto remainTime = std::to_string(lastSpawnTime + SpawnTime - GameSystem::GetInstance().GetTime()).substr(0, 4);
	ScreenPrint(string("Next Enemy Spawn Remain Time : ").append(remainTime).append("\n").c_str());

	//rendering map
	const auto & map = Map::GetInstance();

	for (int y = 0; y < map.MapHeight; ++y)
	{
		for (int x = 0; x < map.MapWidth; ++x)
		{
			ScreenPrint(GetCellSymbol(map.GetTargetCell(Point(x, y))).c_str());
		}
		ScreenPrint("\n");
	}

	//rendering Log
	//if (GameSystem::GetInstance().log.length() > 0)
	//{
	//	ScreenPrint(GameSystem::GetInstance().log.c_str());
	//}

	if (!Enabled)
	{
		ScreenPrint("종료되었습니다 \n");
	}

	ScreenFlipping();
}