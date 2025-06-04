#pragma once
#include "Observer.h"

namespace dae
{
	class GameObject;
}

class CollisionHandler : public dae::Observer
{
public:
    void Notify(dae::GameObject* gameObject, dae::EventID event) override;
};

