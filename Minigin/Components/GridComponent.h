#pragma once
#include <memory>
#include <vec2.hpp>
#include <vector>
#include "Helpers.h"
#include "Component.h"

class GridComponent final : public dae::Component
{
public:
	struct Cell
	{
		bool hasBeenDug{ false };
		glm::vec2 spawnPosition{};
		glm::vec2 centerPoint{};

		//Digging
		dae::GameObject* coverTile{ nullptr };
		MoveDirection digDirection{ MoveDirection::Right };

		//Rocks
		bool hasRock{ false };
	};

	GridComponent(dae::GameObject* owner);

	void Render() const override;

	void CreateGrid();

	void DrawGrid() const;
	std::vector<Cell>& GetGrid() { return m_Grid; }

	int GetCellIndex(const glm::vec2& pos);

	//Constants
	static constexpr int ROWS		{ 14 };
	static constexpr int COLUMNS	{ 18 };
	static constexpr int CELL_SIZE	{ 40 };
private:
	std::vector<Cell> m_Grid;
};
