//---------------------------
// Include Files
//---------------------------
#include "GridComponent.h"

#include <vec2.hpp>

#include "Renderer.h"

//---------------------------
// Constructor & Destructor
//---------------------------
GridComponent::GridComponent(dae::GameObject* owner, int columns, int rows, int cellSize)
	:	Component(owner),
		m_Columns	{ columns	},
		m_Rows		{ rows		},
		m_CellSize	{ cellSize	}
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
	m_Grid = std::vector<Cell>(m_Columns * m_Rows);

	for (int col = 0; col < m_Rows; ++col)
	{
		for (int row = 0; row < m_Columns; ++row)
		{
			int index = col * m_Columns + row;
			m_Grid[index].spawnPosition = glm::vec2{
				static_cast<float>(row * m_CellSize),
				static_cast<float>(col * m_CellSize)
			};
			m_Grid[index].centerPoint = {
			m_Grid[index].spawnPosition.x + m_CellSize / 2.0f,
			m_Grid[index].spawnPosition.y + m_CellSize / 2.0f
			};
			m_Grid[index].hasBeenDug = true; //Set default on true and turn false in level loader
		}
	}
}

void GridComponent::DrawGrid() const
{
	SDL_Renderer* sdlRenderer = dae::Renderer::GetInstance().GetSDLRenderer();
	SDL_SetRenderDrawColor(sdlRenderer, 255, 255, 255, 255);

	for (int col = 0; col < m_Rows; ++col)
	{
		for (int row = 0; row < m_Columns; ++row)
		{
			SDL_Rect cellRect{};
			cellRect.x = row * m_CellSize;
			cellRect.y = col * m_CellSize;
			cellRect.w = m_CellSize;
			cellRect.h = m_CellSize;

			SDL_RenderDrawRect(sdlRenderer, &cellRect);
		}
	}
}

int GridComponent::GetCellIndex(const glm::vec2& pos)
{
	int row = static_cast<int>(pos.x) / m_CellSize;
	int col = static_cast<int>(pos.y) / m_CellSize;

	if (row < 0 || row >= m_Columns || col < 0 || col >= m_Rows)
	{
		return -1; // Out of bounds
	}

	return col * m_Columns + row;
}