#pragma once
#include "Component.h"

class RopeComponent;

class RopeHeadComponent final : public dae::Component
{
public:
    explicit RopeHeadComponent(dae::GameObject* owner, RopeComponent* ropeOwner)
        : Component(owner), m_RopeOwner(ropeOwner) {}

    RopeComponent* GetRopeOwner() const { return m_RopeOwner; }

private:
    RopeComponent* m_RopeOwner;
};
