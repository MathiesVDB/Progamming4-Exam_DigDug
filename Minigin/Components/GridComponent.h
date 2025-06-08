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

	GridComponent(dae::GameObject* owner, int columns = 14, int rows = 18, int cellSize = 40); // Defaulted to digdug values

	void Render() const override;

	void CreateGrid();

	void DrawGrid() const;
	std::vector<Cell>& GetGrid() { return m_Grid; }

	int GetCellIndex(const glm::vec2& pos);

	int GetColumns()	const { return m_Columns;	}
	int GetRows()		const { return m_Rows;		}
	int GetCellSize()	const { return m_CellSize;	}
	
private:
	std::vector<Cell> m_Grid;

	int m_Columns;
	int m_Rows;
	int m_CellSize;
};
