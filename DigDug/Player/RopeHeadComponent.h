#pragma once
#include "Component.h"

class RopeComponent;

class RopeHeadComponent final : public dae::Component
{
public:
    explicit RopeHeadComponent(dae::GameObject* owner)
        : Component(owner) {}

    RopeComponent* GetRopeOwner() const { return m_RopeOwner; }
    void SetRopeComponent(RopeComponent* rope) { m_RopeOwner = rope; }

private:
    RopeComponent* m_RopeOwner{nullptr};
};
