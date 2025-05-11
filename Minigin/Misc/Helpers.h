#pragma once

struct Point2f
{
	float x;
	float y;
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