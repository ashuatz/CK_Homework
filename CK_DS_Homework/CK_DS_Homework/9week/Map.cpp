#include "Map.h"

void Map::ContactTargetCell(const Point& pos, Object* accessor)
{
	if (!CheckAccess(pos))
		return;

	m[pos.y][pos.x].SetLocatedObject(accessor);
}

const ObjectType Map::GetAttachedObjectType(const Point & pos) const
{
	if (!CheckAccess(pos))
		return ObjectType::Error;

	return m[pos.y][pos.x].GetLocatedObjectType();
}

const bool Map::CheckAccess(const Point & value) const
{
	if (MapMin.x > value.x || MapMax.x <= value.x)
		return false;
	if (MapMin.y > value.y || MapMax.y <= value.y)
		return false;

	return true;
}