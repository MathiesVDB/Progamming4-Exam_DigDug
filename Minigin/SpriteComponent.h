#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "Component.h"
#include "Helpers.h"
#include "TextureComponent.h"

//-----------------------------------------------------
// SpriteComponent Class									 
//-----------------------------------------------------
class SpriteComponent final : public dae::Component
{
public:
	SpriteComponent(dae::GameObject* owner, const std::string& fileName, int rows, int columns, float frameDelay, int firstframe, int lastframe); //Don't forget to set the position of the sprite after
	~SpriteComponent() = default;

	// -------------------------
	// Copy/move constructors and assignment operators
	// -------------------------    
	SpriteComponent(const SpriteComponent& other)					= default;
	SpriteComponent(SpriteComponent&& other) noexcept				= default;
	SpriteComponent& operator=(const SpriteComponent& other)		= default;
	SpriteComponent& operator=(SpriteComponent&& other) noexcept	= default;

	//-------------------------------------------------
	// Member functions						
	//-------------------------------------------------
	void Update(float deltaTime) override;
	void Render() const override;

	void SetSpriteBounds(const int newFirstFrame, const int newLastFrame, const bool setCurrentFrame);
	void SetSpriteLocation(); //Does not need argument, because it uses the position of the GameObject

	int GetCurrentFrame() const;

private:
	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------
	Point2f m_Position;
	TextureComponent m_Texture;

	int m_Rows;
	int m_Columns;
	int m_FrameWidth{ -1 };
	int m_FrameHeight{ -1 };
	int m_CurrentFrame{};
	int m_MaxFrame{};
	int m_StartFrame{};

	float m_FrameDelay;
	float m_AccumulatedTime{};
};
