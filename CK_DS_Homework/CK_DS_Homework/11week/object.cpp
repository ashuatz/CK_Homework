#include "object.h"
#include "Map.h"
#include "game.h"
#include "util.h"

void Player::Initialize(const Point & pos)
{
	Map::GetInstance().SetTargetCell(position, this);
	tier = 1;
}

void Player::MovePosition(const Point & delta)
{
	if (!Map::GetInstance().CheckAccess(position + delta))
		return;

	Map::GetInstance().SetTargetCell(position, nullptr);
	position.x += delta.x;
	Map::GetInstance().SetTargetCell(position, this);
}

void Enemy::Initialize(const Point & pos)
{
	position = pos;
	LastDelta = Point(-1, -1);
	ConnectedPosition = Point(-1, -1);

	Dir = Point(1, 0);
	moveOffset = GameSystem::GetRandomValue(uniform_int<>(-100, 100)) / 100.f;

	tier = 1;
}

void Enemy::Update()
{
	if (tier < 0)
		return;

	auto delta = Dir * (pingpong<float>(GameSystem::GetInstance().GetTime() + moveOffset, 4) - 2);

	if (delta != LastDelta)
	{
		if (!Map::GetInstance().CheckAccess(delta + position))
			return;

		auto preAllocator = Map::GetInstance().GetTargetCell(delta + position);
		if (preAllocator != nullptr)
		{
			//blocked
			if (preAllocator->GetType() == ObjectType::Enemy)
				return;

			//contact
			if (preAllocator->GetType() == ObjectType::Bullet)
			{
				//스스로 destory 되는 상황을 어떻게 처리하지?
				//Game은 Bullet을 어렵게 알지만 Bullet은 스스로의 container를 알지 못한다.
				//이벤트 리스닝이 안되는 구조에서 나는 어떻게 짤 수 있을까.

				Object::SetTier(this, -1);
				Object::SetTier(preAllocator, -1);
				return;
			}
		}

		//update Position
		Map::GetInstance().SetTargetCell(LastDelta + position, nullptr);
		Map::GetInstance().SetTargetCell(delta + position, this);
		ConnectedPosition = delta + position;
		LastDelta = delta;
	}
}

void Bullet::Initialize(const Point & pos)
{
	position = pos;
	Map::GetInstance().SetTargetCell(pos, this);
	tier = 1;
}

void Bullet::Update()
{
	if (tier < 0)
		return;

	auto delta = Point(0, -1);

	//out of map
	if (!Map::GetInstance().CheckAccess(delta + position))
	{
		Object::SetTier(this, -1);
		return;
	}

	auto preAllocator = Map::GetInstance().GetTargetCell(delta + position);
	if (preAllocator != nullptr)
	{
		//contact
		if (preAllocator->GetType() == ObjectType::Enemy)
		{
			//스스로 destory 되는 상황을 어떻게 처리하지?
			//Game은 Bullet을 어렵게 알지만 Bullet은 스스로의 container를 알지 못한다.
			//이벤트 리스닝이 안되는 구조에서 나는 어떻게 짤 수 있을까.

			Object::SetTier(this, -1);
			Object::SetTier(preAllocator, -1);
			return;
		}
	}

	Map::GetInstance().SetTargetCell(position, nullptr);
	Map::GetInstance().SetTargetCell(delta + position, this);
	position += delta;
}