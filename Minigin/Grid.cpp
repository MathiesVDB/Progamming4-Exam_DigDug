//---------------------------
// Include Files
//---------------------------
#include "Grid.h"
#include "Renderer.h"

//---------------------------
// Constructor & Destructor
//---------------------------
Grid::Grid()
{
	CreateGrid();

#if _DEBUG
	std::cout << "Grid created with " << ROWS * COLUMNS << " cells." << std::endl;
	DrawGrid();
#endif
}

Grid::~Grid()
{
	// nothing to destroy
}

void Grid::CreateGrid()
{
	m_Grid = std::vector<Cell>(ROWS * COLUMNS);
}

void Grid::DrawGrid() const
{
	SDL_Renderer* sdlRenderer = dae::Renderer::GetInstance().GetSDLRenderer();
	SDL_SetRenderDrawColor(sdlRenderer, 255, 255, 255, 255);

	for (int y = 0; y < ROWS; ++y)
	{
		for (int x = 0; x < COLUMNS; ++x)
		{
			SDL_Rect cellRect{};
			cellRect.x = x * CELL_SIZE;
			cellRect.y = y * CELL_SIZE;
			cellRect.w =	 CELL_SIZE;
			cellRect.h =	 CELL_SIZE;

			SDL_RenderDrawRect(sdlRenderer, &cellRect);
		}
	}
}

int Grid::GetCellIndex(const Point2f& pos)
{
	int x = static_cast<int>(pos.x) / CELL_SIZE;
	int y = static_cast<int>(pos.y) / CELL_SIZE;

	if (x < 0 || x >= COLUMNS || y < 0 || y >= ROWS)
	{
		return -1; // Out of bounds
	}

	return y * COLUMNS + x;
}