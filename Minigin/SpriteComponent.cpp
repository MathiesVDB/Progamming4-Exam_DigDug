//---------------------------
// Include Files
//---------------------------
#include "SpriteComponent.h"

#include "GameObject.h"
#include "Renderer.h"
#include "Texture2D.h"

//---------------------------
// Constructor & Destructor
//---------------------------
SpriteComponent::SpriteComponent(dae::GameObject* owner, const std::string& fileName, int rows, int columns, float frameDelay, int firstframe, int lastframe)
	:	Component(owner),
		m_Texture(owner, fileName),
		m_Rows		{ rows },
		m_Columns	{ columns },
		m_FrameDelay{ frameDelay },
		m_StartFrame{ firstframe },
		m_MaxFrame	{ lastframe },
		m_Position	{}
{
	m_FrameWidth = m_Texture.GetWidth() / m_Columns;
	m_FrameHeight = m_Texture.GetHeight() / m_Rows;
}

//---------------------------
// Member functions
//---------------------------
void SpriteComponent::Update(float deltaTime)
{
	if (GetOwner())
	{
		SetSpriteLocation();
	}

	if (m_StartFrame == m_MaxFrame) { return; /* No animation needed for static sprites */ }

	m_AccumulatedTime += deltaTime;

	if (m_AccumulatedTime >= m_FrameDelay)
	{
		// trigger jump to the next frame, reset if necessary
		m_AccumulatedTime -= m_FrameDelay;

		++m_CurrentFrame;

		if (m_CurrentFrame > m_MaxFrame)
		{
			m_CurrentFrame = m_StartFrame;
		}
	}
}

void SpriteComponent::Render() const
{
	const int rowIndex{ m_CurrentFrame / m_Columns };
	const int colIndex{ m_CurrentFrame % m_Columns };
	SDL_Rect sourceRect = { colIndex * m_FrameWidth,
						rowIndex * m_FrameHeight,
				     (colIndex + 1) * m_FrameWidth,
					 (rowIndex + 1) * m_FrameHeight };

	m_Texture.Render(m_Position.x, m_Position.y, sourceRect);
}

void SpriteComponent::SetSpriteBounds(const int newFirstFrame, const int newLastFrame, const bool setCurrentFrame)
{
	m_StartFrame = newFirstFrame;
	m_MaxFrame = newLastFrame;

	if (setCurrentFrame)
	{
		m_CurrentFrame = m_StartFrame;
	}
}

void SpriteComponent::SetSpriteLocation()
{
	m_Position.x = GetOwner()->GetWorldPosition().x;
	m_Position.y = GetOwner()->GetWorldPosition().y;

}

int SpriteComponent::GetCurrentFrame() const
{
	return m_CurrentFrame;
}

