#pragma once
#include "pch.h"
#include "point.h"
#include "type.h"


class Object
{
private:
	std::string Symbol;
	Point position;
	ObjectType type;
public:
	Object(const std::string& sym, const Point& pos, const ObjectType& type)
		:Symbol(sym), position(pos), type(type) {}

	void SetPosition(const Point& pos) { position = pos; }
	void SetSymbol(const std::string& sym) { Symbol = sym; }

	const Point GetPosition() const { return position; }
	const std::string GetSymbol() const { return Symbol; }
	const ObjectType GetType() const { return type; }
};

class SnakeBody : public Object
{
private:
	int index;
	Point direction;
public:
	SnakeBody(const int& idx, const std::string& sym, const Point& pos)
		: Object(sym, pos, ObjectType::SnakeBody), index(idx) {}
	SnakeBody(const int& idx, const std::string& sym, const Point& pos, const ObjectType& type)
		: Object(sym, pos, type), index(idx) {}

	void SetIndex(const int& idx) { index = idx; }
	void SetDir(const Point& dir) { direction = dir; }

	const int GetIndex() const { return index; }
	const Point GetDirection()const { return direction; }
};

class Item : public Object
{
public:
	Item(const std::string sym, const Point& pos) :
		Object(sym, pos, ObjectType::Item) {}
};

class Cell : public Object
{
private:
	Object* LocatedObject;

public:
	Cell()
		: Object("□", Point(0, 0), ObjectType::None), LocatedObject(nullptr) {}
	Cell(const Point& pos)
		:Object("□", pos, ObjectType::None), LocatedObject(nullptr) {}

	void SetLocatedObject(Object* const target)
	{
		LocatedObject = target;
	}

	const ObjectType GetLocatedObjectType() const
	{
		if (LocatedObject == nullptr)
			return ObjectType::None;

		return LocatedObject->GetType();
	}

	const Object* const GetLocatedObject() const
	{
		return LocatedObject;
	}
};