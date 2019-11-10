#pragma once
#include "pch.h"
#include "object.h"
#include "point.h"
#include "timer.h"
#include "Map.h"
#include "type.h"
#include "list.h"
#include "singleton.h"

using namespace std;

class GameSystem : public Singleton<GameSystem>
{
protected:
	LinkedList* EnemyPool;
	LinkedList* BulletPool;
	Timer<void(void)> UpdateTimer;

	mt19937 mt;

	float time;

	void Update()
	{
		time += 0.016f;
	}

public:

	GameSystem() : EnemyPool(nullptr), BulletPool(nullptr), time(0), mt((random_device())())
	{
		std::function<void(void)> Update = std::bind(&GameSystem::Update, this);
		UpdateTimer.Initialize(Update, 16);
		UpdateTimer.start();
	}

	template <typename T>
	static const T GetRandomValue(uniform_int<T> range) { return range(GetInstance().mt); }

	const float GetTime() const { return time; }

	void Initialize()
	{
		if (EnemyPool != nullptr)
		{
			int count = 0;
			auto temp = EnemyPool->removeAll(count);

			for (int i = 0; i < count; ++i)
			{
				delete temp[i];
			}

			delete[] temp;
			delete EnemyPool;
		}

		if (BulletPool != nullptr)
		{
			int count = 0;
			auto temp = BulletPool->removeAll(count);

			for (int i = 0; i < count; ++i)
			{
				delete temp[i];
			}

			delete[] temp;
			delete BulletPool;
		}

		EnemyPool = new LinkedList();
		BulletPool = new LinkedList();
	}

	void ReleaseObject(Enemy* target)
	{
		EnemyPool->PushBack(target);
	}

	void ReleaseObject(Bullet* target)
	{
		BulletPool->PushBack(target);
	}

	void DeleteObject(Object* target)
	{
		if (target != nullptr)
			delete target;
	}

	Object* SpawnObject(ObjectType type)
	{
		Object* target = nullptr;
		switch (type)
		{
			case ObjectType::Enemy:
				target = static_cast<Enemy*>(EnemyPool->remove(0));
				if (target == nullptr)
				{
					//log = string("cache miss, count : ").append(std::to_string(cacheMiss++));
					target = new Enemy();
				}
				break;

			case ObjectType::Bullet:
				target = static_cast<Bullet*>(BulletPool->remove(0));
				if (target == nullptr)
				{
					//log = string("cache miss, count : ").append(std::to_string(cacheMiss++));
					target = new Bullet();
				}
				break;

			default:
				return nullptr;
		}

		return target;
	}

	//debugging
	//std::string log;
	//int cacheMiss;
};