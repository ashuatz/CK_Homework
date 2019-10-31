#pragma once
#include "pch.h"
#include "point.h"
#include "type.h"

class Object
{
private:
	Point position;
	ObjectType type;
public:
	Object(const Point& pos, const ObjectType& type)
		:position(pos), type(type) {}

	void SetPosition(const Point& pos) { position = pos; }
	void SetType(const ObjectType& t) { type = t; }

	const Point GetPosition() const { return position; }
	const ObjectType GetType() const { return type; }
};

class SnakeBody : public Object
{
private:
	int index;
	Point direction;
public:
	SnakeBody(const int& idx, const Point& pos)
		: Object(pos, ObjectType::PlayerBody), index(idx) {}
	SnakeBody(const int& idx, const Point& pos, const ObjectType& type)
		: Object(pos, type), index(idx) {}

	void SetIndex(const int& idx) { index = idx; }
	void SetDir(const Point& dir) { direction = dir; }

	const int GetIndex() const { return index; }
	const Point GetDirection()const { return direction; }
};

class Item : public Object
{
public:
	Item(const Point& pos) :
		Object(pos, ObjectType::Item) {}
};