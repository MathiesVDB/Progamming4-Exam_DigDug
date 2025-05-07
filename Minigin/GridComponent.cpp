//---------------------------
// Include Files
//---------------------------
#include "GridComponent.h"
#include "Renderer.h"

//---------------------------
// Constructor & Destructor
//---------------------------
GridComponent::GridComponent(dae::GameObject* owner)
	: Component(owner)
{
	CreateGrid();

#if _DEBUG
	std::cout << "GridComponent created with " << ROWS * COLUMNS << " cells." << std::endl;
	DrawGrid();
#endif
}

void GridComponent::Update(float )
{
}

void GridComponent::Render() const
{
#if _DEBUG
	DrawGrid();
#endif
}

void GridComponent::CreateGrid()
{
	m_Grid = std::vector<Cell>(ROWS * COLUMNS);

	for (int row = 0; row < ROWS; ++row)
	{
		for (int col = 0; col < COLUMNS; ++col)
		{
			int index = row * COLUMNS + col;
			m_Grid[index].spawnPosition = Point2f{ col * CELL_SIZE * 1.0f, row * CELL_SIZE * 1.0f };
		}
	}
}

void GridComponent::DrawGrid() const
{
	SDL_Renderer* sdlRenderer = dae::Renderer::GetInstance().GetSDLRenderer();
	SDL_SetRenderDrawColor(sdlRenderer, 255, 255, 255, 255);

	for (int y = 0; y < COLUMNS; ++y)
	{
		for (int x = 0; x < ROWS; ++x)
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

int GridComponent::GetCellIndex(const Point2f& pos)
{
	int x = static_cast<int>(pos.x) / CELL_SIZE;
	int y = static_cast<int>(pos.y) / CELL_SIZE;

	if (x < 0 || x >= COLUMNS || y < 0 || y >= ROWS)
	{
		return -1; // Out of bounds
	}

	return y * COLUMNS + x;
}