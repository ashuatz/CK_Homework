#include "list.h"

void PositionList::Add(const Point & target)
{
	points[count++] = new Point(target);
}

const int PositionList::IndexOf(const Point & target) const
{
	for (int i = 0; i < count; ++i)
	{
		if ((*points[i]) == target)
			return i;
	}

	return -1;
}

void PositionList::Remove(const Point & target)
{
	int idx = IndexOf(target);
	if (idx == -1)
		return;

	auto* temp = points[idx];
	for (int i = idx; i < count - 1; i++)
	{
		points[i] = points[i + 1];
	}

	count--;
	delete temp;
}

void PositionList::Remove(const int & idx)
{
	if (idx == -1 || idx >= count)
		return;

	auto* temp = points[idx];
	for (int i = idx; i < count - 1; i++)
	{
		points[i] = points[i + 1];
	}

	count--;
	delete temp;
}

Point * PositionList::Retrieve(const int & index) const
{
	if (index < 0 || index >= count)
		return nullptr;

	return points[index];
}

Point * PositionList::Find(const Point & pred) const
{
	for (int i = 0; i < count; ++i)
	{
		if ((*points[i]) == pred)
			return points[i];
	}
	return nullptr;
}

const bool PositionList::Contains(const Point & target) const
{
	for (int i = 0; i < count; ++i)
	{
		if ((*points[i]) == target)
			return true;
	}

	return false;
}

void SnakeList::CreateBodyToFirst(const Point & pos)
{
	if (count > 0)
	{
		bodys[0]->SetType(ObjectType::PlayerBody);

		Shift();
	}

	bodys[0] = new SnakeBody(0, pos, ObjectType::PlayerHead);

	count++;
}

void SnakeList::SetHeadDirection(const Point & dir) const
{
	if (isEmpty())
		return;

	bodys[0]->SetDir(dir);
}

SnakeBody * SnakeList::GetBody(const int & index) const
{
	if (index < 0 || index >= count)
		return nullptr;

	return bodys[index];
}

void SnakeList::Update()
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
		if (i + 1 < count)
		{
			bodys[i + 1]->SetDir(bodys[i]->GetDirection());
		}
	}
}
