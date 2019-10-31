#pragma once
#include "pch.h"
#include "object.h"
#include "point.h"

class PositionList
{
private:
	int capacity;
	int count;
	Point** points;
	
	void Resize()
	{
		capacity *= 2;

		auto temp = points;
		points = new Point*[capacity];
		for (int i = 0; i < count; ++i)
		{
			points[i] = temp[i];
		}

		delete[] temp;
	}

public:
	PositionList() : count(0), capacity(2)
	{
		points = new Point*[capacity];
	}

	void Add(const Point& target);

	const int IndexOf(const Point& target) const;

	void Remove(const Point& target);

	void Remove(const int& idx);

	Point* Retrieve(const int& index) const;

	Point* Find(const Point& pred) const;

	const bool Contains(const Point& target) const;
};


class SnakeList
{
private:
	int count;
	int capacity;
	SnakeBody** bodys; // low is first
	
	void Resize()
	{
		capacity *= 2;

		auto temp = bodys;
		bodys = new SnakeBody*[capacity];
		for (int i = 0; i < count; ++i)
		{
			bodys[i] = temp[i];
		}

		delete[] temp;
	}

	void Shift()
	{
		if (isEmpty())
			return;

		if (isFull())
			Resize();

		for (int i = count-1; i >= 0; --i)
		{
			bodys[i + 1] = bodys[i];
			bodys[i + 1]->SetIndex(i + 1);
		}

		bodys[0] = nullptr;
	}
	
public:
	SnakeList() : count(0), capacity(2) 
	{
		bodys = new SnakeBody*[capacity];
		for (int i = 0; i < count; ++i)
		{
			bodys[i] = new SnakeBody(i,  Point(0, 0));
			bodys[i]->SetDir(Point(1, 0));
		}
	}

	SnakeList(const int& cnt,const int& capa) : count(cnt),capacity(capa)
	{
		bodys = new SnakeBody*[capacity];
		for (int i = 0; i < count; ++i)
		{
			bodys[i] = new SnakeBody(i, Point(0, 0), i == 0 ? ObjectType::PlayerHead : ObjectType::PlayerBody);
			bodys[i]->SetDir(Point(1, 0));
		}
	}

	void CreateBodyToFirst(const Point& pos);

	void SetHeadDirection(const Point& dir) const;

	SnakeBody* GetBody(const int& index) const;

	void Update();

	const bool isEmpty() const { return count <= 0; }
	const bool isFull() const { return capacity == count; }
	const int GetLength() const { return count; }

	SnakeBody* operator[](const int& index)
	{
		return GetBody(index);
	}

	~SnakeList() {}
};