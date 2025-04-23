#pragma once
#include "Observer.h"
#include <iostream>

class DamageSound : public Observer
{
public:
	DamageSound(const dae::GameObject* player, const std::string& soundPath);
	~DamageSound() override = default;

	void Notify(const dae::GameObject* gameObject, Event event) override;

private:
	const dae::GameObject* m_Player{ nullptr };
	std::string m_SoundPath;
};

