#pragma once
#include "pch.h"
#include "object.h"

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

	//direction only
	void Move()
	{
		if (isEmpty())
			return;

		if (isFull())
			Resize();

		for (int i = count - 1; i >= 0; --i)
		{
			//move
			
			bodys[i]->SetPosition(bodys[i]->GetPosition() + bodys[i]->GetDirection());
			//pass
			bodys[i + 1]->SetDir(bodys[i]->GetDirection());
		}

		bodys[0] = nullptr;
	}

public:
	SnakeList() : count(0), capacity(2) 
	{
		bodys = new SnakeBody*[capacity];
		for (int i = 0; i < count; ++i)
		{
			bodys[i] = new SnakeBody(i, "◈", Point(0, 0));
			bodys[i]->SetDir(Point(1, 0));
		}
	}

	SnakeList(const int& cnt,const int& capa) : count(cnt),capacity(capa)
	{
		bodys = new SnakeBody*[capacity];
		for (int i = 0; i < count; ++i)
		{
			bodys[i] = new SnakeBody(i, "◈", Point(0, 0));
			bodys[i]->SetDir(Point(1, 0));
		}
	}

	void CreateBodyToFirst(const Point& pos)
	{
		Shift();

		bodys[0] = new SnakeBody(0, "◈", pos);

		count++;

		if (count > 1)
		{
			bodys[1]->SetSymbol("◇");
		}
	}

	void SetHeadDirection(const Point& dir)
	{
		if (isEmpty())
			return;

		bodys[0]->SetDir(dir);
	}

	void Update()
	{
		for (int i = 0; i < count; ++i)
		{

		}
	}

	const bool isEmpty() const { return count <= 0; }
	const bool isFull() const { return capacity == count; }

	~SnakeList() {}
};