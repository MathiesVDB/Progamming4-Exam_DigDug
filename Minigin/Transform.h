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
		void Update(float deltaTime) override;

		const glm::vec3& GetPosition() const;
		void SetPosition(float x, float y, float z);
	private:
		glm::vec3 m_Position{0, 0, 0};
		float m_Time{ 0.0f };
	};
}
