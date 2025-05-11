#pragma once
#include "Observer.h"

namespace dae
{
	class GameObject;
}

class CollisionHandler : public dae::Observer
{
public:
    void Notify(const dae::GameObject* gameObject, dae::EventID event) override;
};

