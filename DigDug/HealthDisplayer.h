#pragma once
#include "HealthComponent.h"
#include "TextObject.h"
#include "Observer.h"

class GridComponent;

class HealthDisplay : public dae::Observer
{
public:
    explicit HealthDisplay(dae::GameObject* player, GridComponent* grid);
    ~HealthDisplay() override = default;

    void Notify(const dae::GameObject* gameObject, dae::EventID event) override;

    void OnLifeLost();

    void SetLives(int lives);
    void CreateLives();

private:
    void UpdateLives();

    dae::GameObject* m_Player   { nullptr };
    HealthComponent* m_Health   { nullptr };
    GridComponent*   m_Grid     { nullptr };

    std::vector<dae::GameObject*> m_DisplayedLives;

    int m_Lives{ 0 };
};