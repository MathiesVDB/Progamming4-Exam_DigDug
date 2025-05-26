//---------------------------
// Include Files
//---------------------------
#include "GridComponent.h"

#include <vec2.hpp>

#include "Renderer.h"

//---------------------------
// Constructor & Destructor
//---------------------------
GridComponent::GridComponent(dae::GameObject* owner)
	: Component(owner)
{
	CreateGrid();

#if _DEBUG
	DrawGrid();
#endif
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

	for (int col = 0; col < COLUMNS; ++col)
	{
		for (int row = 0; row < ROWS; ++row)
		{
			int index = col * ROWS + row;
			m_Grid[index].spawnPosition = glm::vec2{
				static_cast<float>(row * CELL_SIZE),
				static_cast<float>(col * CELL_SIZE)
			};
			m_Grid[index].centerPoint = {
			m_Grid[index].spawnPosition.x + CELL_SIZE / 2.0f,
			m_Grid[index].spawnPosition.y + CELL_SIZE / 2.0f
			};
			m_Grid[index].hasBeenDug = true; //Set default on true and turn false in level loader
		}
	}
}


void GridComponent::DrawGrid() const
{
	SDL_Renderer* sdlRenderer = dae::Renderer::GetInstance().GetSDLRenderer();
	SDL_SetRenderDrawColor(sdlRenderer, 255, 255, 255, 255);

	for (int col = 0; col < COLUMNS; ++col)
	{
		for (int row = 0; row < ROWS; ++row)
		{
			SDL_Rect cellRect{};
			cellRect.x = row * CELL_SIZE;
			cellRect.y = col * CELL_SIZE;
			cellRect.w = CELL_SIZE;
			cellRect.h = CELL_SIZE;

			SDL_RenderDrawRect(sdlRenderer, &cellRect);
		}
	}
}

int GridComponent::GetCellIndex(const glm::vec2& pos)
{
	int row = static_cast<int>(pos.x) / CELL_SIZE;
	int col = static_cast<int>(pos.y) / CELL_SIZE;

	if (row < 0 || row >= ROWS || col < 0 || col >= COLUMNS)
	{
		return -1; // Out of bounds
	}

	return col * ROWS + row;
}