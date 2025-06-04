#pragma once
#include "Observer.h"

class ScoreHandler : public dae::Observer
{
	ScoreHandler();

	void Notify(const dae::GameObject* gameObject, dae::EventID event) override;
};

