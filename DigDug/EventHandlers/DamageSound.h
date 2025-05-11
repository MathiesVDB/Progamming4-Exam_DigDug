#pragma once
#include "Observer.h"
#include <iostream>

class DamageSound : public dae::Observer
{
public:
	DamageSound(const dae::GameObject* player, const std::string& soundPath);

	void Notify(const dae::GameObject* gameObject, dae::EventID event) override;

private:
	const dae::GameObject* m_Player{ nullptr };
	std::string m_SoundPath;
};

