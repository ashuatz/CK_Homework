#include "Map.h"

void Map::SetTargetCell(const Point & pos, const ObjectType & target)
{
	if (!CheckAccess(pos))
		return;
	m[pos.y][pos.x] = target;
}

const ObjectType Map::GetTargetCell(const Point & pos) const
{
	if (!CheckAccess(pos))
		return ObjectType::Error;

	return m[pos.y][pos.x];
}

const bool Map::CheckAccess(const Point & value) const
{
	if (MapMin.x > value.x || MapMax.x <= value.x)
		return false;
	if (MapMin.y > value.y || MapMax.y <= value.y)
		return false;

	return true;
}