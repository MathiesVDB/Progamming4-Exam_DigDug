#pragma once
#include "Observer.h"
#include <iostream>

class SoundHandler : public dae::Observer
{
public:
	SoundHandler();

	void Notify(const dae::GameObject* gameObject, dae::EventID event) override;

private:
};

