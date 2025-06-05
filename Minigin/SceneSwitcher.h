#pragma once
#include <string>
#include <functional>
#include "Singleton.h"

namespace dae
{
	class Scene;
	class SceneSwitcher : public dae::Singleton<SceneSwitcher>
	{
	public:
		void QueueSceneChange(const std::string& sceneName);
		void QueueSceneChange(const std::function<void()>& loadSceneFunc);

		void Update();

	private:
		SceneSwitcher() = default;

		std::function<void()> m_NextSceneFunc{};
		bool m_HasPendingChange{ false };
	};
}
