#pragma once

#include "point.h"
#include "type.h"
#include "object.h"

class Map
{
private:
	const Point MapMin;
	const Point MapMax;

public:
	static constexpr int MapWidth = 10;
	static constexpr int MapHeight = 10;

	ObjectType** m;
	Map() : MapMin(Point(0, 0)), MapMax(Point(MapWidth, MapHeight))
	{
		m = new ObjectType*[10];
		for (int i = 0; i < 10; ++i)
		{
			m[i] = new ObjectType[10];
			for (int j = 0; j < 10; ++j)
			{
				m[i][j] = ObjectType::None;
			}
		}
	}

	void SetTargetCell(const Point& pos, const ObjectType& target);

	const ObjectType GetTargetCell(const Point& pos) const;

	const bool CheckAccess(const Point& value) const;

	~Map()
	{
		for (int i = 0; i < MapHeight; ++i)
		{
			delete[] m[i];
		}
		delete[] m;
	}
};