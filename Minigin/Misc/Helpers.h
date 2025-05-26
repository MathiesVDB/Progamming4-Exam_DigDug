#pragma once
#include <vec3.hpp>

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

enum class Inflated
{
	None,
	Stage1,
	Stage2,
	Stage3,
	Exploded
};