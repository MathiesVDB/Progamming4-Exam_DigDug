#pragma once

//-----------------------------------------------------
// Include Files
//-----------------------------------------------------
#include "Component.h"
#include <memory>
#include <string>

namespace dae {
	class TextObject;
	class Font;
}
//-----------------------------------------------------
// FPSComponent Class									 
//-----------------------------------------------------
class FPSComponent final : public Component
{
public:
	FPSComponent(); // Constructor

	// -------------------------
	// Copy/move constructors and assignment operators
	// -------------------------    
	FPSComponent(const FPSComponent& other)					= default;
	FPSComponent(FPSComponent&& other) noexcept				= default;
	FPSComponent& operator=(const FPSComponent& other)		= default;
	FPSComponent& operator=(FPSComponent&& other) noexcept	= default;

	//-------------------------------------------------
	// Member functions						
	//-------------------------------------------------
	void Update(float deltaTime) override;
	void Render() const override;

private:
	//-------------------------------------------------
	// Private member functions								
	//-------------------------------------------------
	void RecalculateFPS();

	//-------------------------------------------------
	// Datamembers								
	//-------------------------------------------------
	std::shared_ptr<dae::TextObject> m_TextObject;
	int m_FrameCount = 0;
	float m_ElapsedTime = 0.0f;
	float m_DeltaTime{}; // Need this to give deltaTime to my render function
};
