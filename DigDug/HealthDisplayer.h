#pragma once
#include "HealthComponent.h"
#include "TextObject.h"
#include "Observer.h"

class HealthDisplay : public Observer
{
public:
    explicit HealthDisplay(const dae::GameObject* displayObject, dae::GameObject* player);
    ~HealthDisplay() override = default;

    void Notify(const dae::GameObject* gameObject, Event event) override;

    void OnLivesChanged();

    void SetLives(int lives);

private:
    void UpdateText();

    dae::GameObject* m_Player{ nullptr };
    HealthComponent* m_Health{ nullptr };
    dae::TextObject* m_Text{ nullptr };
    int m_Lives{ 0 };
};

