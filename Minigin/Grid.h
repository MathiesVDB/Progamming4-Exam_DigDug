#pragma once
#include <vector>
#include <iostream>
#include "Helpers.h"

class Grid final
{
public:
	struct Cell
	{
		bool hasBeenDug{ false };
	};

	Grid();
	~Grid();

	Grid(const Grid& other) = default;
	Grid(Grid&& other) noexcept = default;
	Grid& operator=(const Grid& other) = default;
	Grid& operator=(Grid&& other) noexcept = default;

	void CreateGrid();

	void DrawGrid() const;

	int GetCellIndex(const Point2f& pos);

private:
	static constexpr int ROWS		{ 14 };
	static constexpr int COLUMNS	{ 18 };
	static constexpr int CELL_SIZE	{ 40 };

	std::vector<Cell> m_Grid;
};
