#pragma once

enum class CellType
{
	Error = -1,
	None = 0,
	Player,
	Item,
	Enemy,

};

enum class ObjectType
{
	Error = -1,
	None = 0,
	SnakeBody,
	Item
};