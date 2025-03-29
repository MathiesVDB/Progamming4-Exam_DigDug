#include "HealthDisplayer.h"

#include "Observer.h"

HealthDisplay::HealthDisplay(const dae::GameObject* displayObject, dae::GameObject* player)
    : m_Player(player)
{
    m_Text = displayObject->GetComponent<dae::TextObject>().get();
    m_Health = player->GetComponent<HealthComponent>().get();

    if (m_Health)
    {
        m_Lives = m_Health->GetLives();
    }
}

void HealthDisplay::Notify(const dae::GameObject*, Event event)
{
    if (event == Event::HealthChanged)
    {
        OnLivesChanged();
    }
}

void HealthDisplay::OnLivesChanged()
{
    if (!m_Health) return;

    m_Lives = m_Health->GetLives();
    UpdateText();
}

void HealthDisplay::SetLives(int lives)
{
    m_Lives = lives;
}

void HealthDisplay::UpdateText()
{
    if (!m_Text) return;
    m_Text->SetText("Lives: " + std::to_string(m_Lives));
}
