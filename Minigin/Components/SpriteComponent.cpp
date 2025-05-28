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
SpriteComponent::SpriteComponent(dae::GameObject* owner, const std::string& fileName, int rows, int columns, float frameDelay, int firstframe, int lastframe, bool areFramesAccumulating)
	:	Component(owner),
		m_Texture(owner, fileName),
		m_Rows		{ rows },
		m_Columns	{ columns },
		m_FrameDelay{ frameDelay },
		m_StartFrame{ firstframe },
		m_MaxFrame	{ lastframe },
		m_Position	{},
		m_CurrentFrame{ firstframe },
		m_AreFramesAccumulating{ areFramesAccumulating }
{
	m_FrameWidth = m_Texture.GetWidth() / m_Columns;
	m_FrameHeight = m_Texture.GetHeight() / m_Rows;

	if (m_AreFramesAccumulating) m_AccumulatedFrameIndex = m_CurrentFrame - m_StartFrame;
	else m_AccumulatedFrameIndex = 1;
}

//---------------------------
// Member functions
//---------------------------
void SpriteComponent::Update(float deltaTime)
{
	if (m_AreFramesAccumulating)
	{
		m_AccumulatedFrameIndex = m_CurrentFrame - m_StartFrame;
	}

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
	int accumulatedWidth = m_FrameWidth;
	int sourceX{};

	if (m_AreFramesAccumulating)
	{
		int totalFrames = m_AccumulatedFrameIndex;
		sourceX = m_FrameWidth * (totalFrames * (totalFrames + 1)) / 2;
		accumulatedWidth = m_FrameWidth * (m_AccumulatedFrameIndex + 1);
	}
	else
	{
		const int colIndex{ m_CurrentFrame % m_Columns };
		sourceX = colIndex * m_FrameWidth;
	}

	const int rowIndex{ m_CurrentFrame / m_Columns };

	SDL_Rect sourceRect = {
		sourceX,
		rowIndex * m_FrameHeight,
		accumulatedWidth,
		m_FrameHeight
	};

	m_Texture.Render(m_Position.x, m_Position.y, sourceRect);

}

void SpriteComponent::SetSpriteBounds(const int newFirstFrame, const int newLastFrame, const bool setCurrentFrame)
{
	m_StartFrame	= newFirstFrame;
	m_MaxFrame		= newLastFrame;

	if (m_AreFramesAccumulating) m_AccumulatedFrameIndex = m_CurrentFrame - m_StartFrame;

	if (setCurrentFrame && (m_CurrentFrame < m_StartFrame || m_CurrentFrame > m_MaxFrame))
	{
		m_CurrentFrame = m_StartFrame;
	}
}

void SpriteComponent::SetSpriteLocation()
{
	m_Position.x = GetOwner()->GetWorldPosition().x;
	m_Position.y = GetOwner()->GetWorldPosition().y;
}

void SpriteComponent::SetNewTexture(const std::string& fileName, int rows, int columns, int firstframe, int lastframe)
{
	m_Texture.SetTexture(fileName);

	m_Rows = rows;
	m_Columns = columns;

	m_FrameWidth = m_Texture.GetWidth() / m_Columns;
	m_FrameHeight = m_Texture.GetHeight() / m_Rows;

	SetSpriteBounds(firstframe, lastframe, true);
}

bool SpriteComponent::HasReachedLastframe() const
{
	return m_CurrentFrame == m_MaxFrame;
}

bool SpriteComponent::IsAlreadyWithinBounds(int newStartFrame, int newMaxFrame) const
{
	return m_CurrentFrame >= newStartFrame && m_CurrentFrame <= newMaxFrame;
}

int SpriteComponent::GetCurrentFrame() const
{
	return m_CurrentFrame;
}

glm::vec2 SpriteComponent::GetSpriteSize() const
{
	int accumulatedWidth{ m_FrameWidth };
	if (m_AreFramesAccumulating)
	{
		accumulatedWidth = m_FrameWidth * (m_AccumulatedFrameIndex + 1);
	}

	return { accumulatedWidth, static_cast<float>(m_FrameHeight) };
}

