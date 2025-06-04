#pragma once
#include "Observer.h"

class SoundHandler : public dae::Observer
{
public:
	SoundHandler();

	void Notify(dae::GameObject* gameObject, dae::EventID event) override;

private:
	std::unordered_map<std::string, std::string> m_SoundMap;
};

