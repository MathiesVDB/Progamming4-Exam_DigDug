#pragma once
#include <glm.hpp>
#include <cmath>

#include "Component.h"

namespace dae
{
	class GameObject;
	class Transform final : public Component
	{
	public:
		Transform(GameObject* owner) : Component(owner) {};

		void Update(float deltaTime) override;

		const glm::vec2& GetPosition() const;
		void SetPosition(float x, float y);
	private:
		glm::vec2 m_Position{0, 0};
	};
}
