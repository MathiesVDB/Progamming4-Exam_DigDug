#include "HealthDisplayer.h"
#include "GridComponent.h"
#include "Observer.h"
#include "SceneManager.h"
#include "Scene.h"
#include "SpriteComponent.h"

HealthDisplay::HealthDisplay(dae::GameObject* player, GridComponent* grid)
    : m_Player{player}, m_Grid{grid}
{
    m_Health = player->GetComponent<HealthComponent>();

    CreateLives();
}

void HealthDisplay::CreateLives()
{
    if (!m_Health)
    {
        std::cerr << "No healthcomponent found!" << std::endl;
        return;
    }

    // Clear existing icons
    for (auto* life : m_DisplayedLives)
    {
        dae::SceneManager::GetInstance().GetActiveScene().MarkForDeletion(life);
    }
    m_DisplayedLives.clear();

    m_Lives = m_Health->GetLives();

    int startIndex{ 238 };
    for (int counter{}; counter < m_Lives; ++counter)
    {
        glm::vec2 drawPos{ m_Grid->GetGrid()[startIndex + counter].spawnPosition };

        auto life = std::make_unique<dae::GameObject>();
        life->ToggleOutOfBounds();
        life->AddComponent<SpriteComponent>("Sprites/Player/WalkingSprite.png", 1, 8, 1.f, 1, 1);
        life->SetLocalPosition(drawPos);

        m_DisplayedLives.emplace_back(life.get());

        dae::SceneManager::GetInstance().GetActiveScene().Add(life);
    }
}

void HealthDisplay::Notify(dae::GameObject*, dae::EventID event)
{
    std::string eventName = dae::EventRegistry::GetInstance().GetName(event);

    if (eventName == "LifeLost")
    {
        OnLifeLost();
    }
    else if (eventName == "HealthIncrease")
    {
        CreateLives();
    }
}

void HealthDisplay::OnLifeLost()
{
    if (!m_Health) return;

    m_Lives = m_Health->GetLives();
    UpdateLives();
}

void HealthDisplay::SetLives(int lives)
{
    m_Lives = lives;
}

void HealthDisplay::UpdateLives()
{
    if (!m_DisplayedLives.empty())
    {
        auto* lastLife = m_DisplayedLives.back();
        dae::SceneManager::GetInstance().GetActiveScene().MarkForDeletion(lastLife);
        m_DisplayedLives.pop_back();
    }
}
