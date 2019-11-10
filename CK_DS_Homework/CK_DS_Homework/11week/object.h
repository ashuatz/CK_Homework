#pragma once
#include "pch.h"
#include "point.h"
#include "type.h"

class Object
{
protected:
	Point position;
	ObjectType type;
	int tier;

	static void SetTier(Object* target, int tier) { target->tier = tier; }
public:

	Object(const Point& pos, const ObjectType& type)
		:position(pos), type(type) {}

	virtual void Initialize(const Point& pos) = 0;

	void SetPosition(const Point& pos) { position = pos; }
	void SetType(const ObjectType& t) { type = t; }

	virtual const Point GetPosition() const { return position; }
	const ObjectType GetType() const { return type; }
	const int GetTier() const { return tier; }
};

class Bullet : public Object
{
protected:
public:
	Bullet(const Point& pos) : Object(pos, ObjectType::Bullet) {}
	Bullet() : Bullet(Point(0, 0)) {}

	void Initialize(const Point& pos) override;
	void Update();
};

class Player : public Object
{
protected:

public:
	Player(const Point& pos) : Object(pos, ObjectType::Player) {}

	void Initialize(const Point& pos) override;
	void MovePosition(const Point& delta);
};

class Enemy : public Object
{
protected:
	Point Dir;
	Point LastDelta;
	float moveOffset;

	Point ConnectedPosition;
public:
	Enemy(const Point& pos) : Object(pos, ObjectType::Enemy) {}
	Enemy() : Enemy(Point(0, 0)) {}

	const Point GetPosition() const override { return ConnectedPosition; }
	void Initialize(const Point& pos) override;
	void Update();
};