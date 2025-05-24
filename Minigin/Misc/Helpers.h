#pragma once

struct Point2f
{
	float x;
	float y;

	bool operator==(const Point2f& other) const
	{
		return x == other.x && y == other.y;
	}
};

enum class MoveDirection
{
	Left,
	Right,
	Up,
	Down
};

enum class RenderLayer
{
	Ground,
	Entity,
	Player
};