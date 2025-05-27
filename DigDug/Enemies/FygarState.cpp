#include "FygarState.h"
#include "GameObject.h"
#include "GridComponent.h"
#include "Fygar.h"
#include "SceneManager.h"
#include "Scene.h"
#include "SpriteComponent.h"

using namespace FygarStates;
//-----------------------------------------------------
// MovingState Class
//-----------------------------------------------------

FygarStates::FygarState* MovingState::Update(Fygar& , float )
{
    return nullptr;
}

void MovingState::OnEnter(Fygar& pooka)
{
    m_Sprite = pooka.GetOwner()->GetComponent<SpriteComponent>();

    if (pooka.IsLookingLeft())  m_Sprite->SetNewTexture("Sprites/Pooka/PookaDefaultSprite.png", 2, 8, 8, 9);
    else                        m_Sprite->SetNewTexture("Sprites/Pooka/PookaDefaultSprite.png", 2, 8, 0, 1);
}

void MovingState::OnExit(Fygar&)
{
}

//-----------------------------------------------------
// Inflated Class
//-----------------------------------------------------

FygarStates::FygarState* InflatedState::Update(Fygar& fygar, float deltaTime)
{
    if (fygar.GetInflatedState() == m_PreviousState)
    {
        m_ResetTimer += deltaTime;

        if (m_ResetTimer >= RESET_THRESHOLD) return &FygarStates::FygarState::moving;
        return nullptr;
    }

    m_PreviousState = fygar.GetInflatedState();

    switch (m_PreviousState)
    {
    case Inflated::Stage1:
        m_Sprite->SetSpriteBounds(0, 0, true);
        m_ResetTimer = 0.f;
        break;
    case Inflated::Stage2:
        m_Sprite->SetSpriteBounds(1, 1, true);
        m_ResetTimer = 0.f;
        break;
    case Inflated::Stage3:
        m_Sprite->SetSpriteBounds(2, 2, true);
        m_ResetTimer = 0.f;
        break;
    case Inflated::Exploded:
        m_Sprite->SetSpriteBounds(3, 3, true);
        m_ResetTimer = 0.f;
        fygar.ResetInflation();
    }

    return nullptr;
}

void InflatedState::OnEnter(Fygar& fygar)
{
    m_ResetTimer = 0.f;
    m_Sprite = fygar.GetOwner()->GetComponent<SpriteComponent>();
    if (fygar.IsLookingLeft())  m_Sprite->SetNewTexture("Sprites/Fygar/FygarInflateLeftSprite.png" , 1, 4, 0, 0);
    else                        m_Sprite->SetNewTexture("Sprites/Fygar/FygarInflateRightSprite.png", 1, 4, 0, 0);
}

void InflatedState::OnExit(Fygar& pooka)
{
    pooka.ResetInflation();
}

//-----------------------------------------------------
// DeathState Class
//-----------------------------------------------------

FygarStates::FygarState* DeathState::Update(Fygar& fygar, float deltaTime)
{
    // Give time to show the death animation before removing it from the scene
    m_DeathTimer += deltaTime;

    if (m_DeathTimer >= DEATH_TIME)
    {
        dae::SceneManager::GetInstance().GetActiveScene().MarkForDeletion(fygar.GetOwner());
        return nullptr;
    }

    return nullptr;
}

void DeathState::OnEnter(Fygar& fygar)
{
    m_Sprite = fygar.GetOwner()->GetComponent<SpriteComponent>();

    if (!fygar.WasCrushed()) return;

    if (fygar.IsLookingLeft()) m_Sprite->SetNewTexture("Sprites/Fygar/FygarDefaultSprite.png", 2, 8,  2,  2);
    else                       m_Sprite->SetNewTexture("Sprites/Fygar/FygarDefaultSprite.png", 2, 8, 10, 10);
}

void DeathState::OnExit(Fygar&)
{

}

//-----------------------------------------------------
// GhostState Class
//-----------------------------------------------------

FygarStates::FygarState* GhostState::Update(Fygar& , float )
{
    //glm::vec2 pookaCenter{ m_PookaCollider->GetBoundingBox().x + m_PookaCollider->GetBoundingBox().w / 2.0f,
    //                       m_PookaCollider->GetBoundingBox().y + m_PookaCollider->GetBoundingBox().h / 2.0f };

    //int index{ fygar.GetGridPtr()->GetCellIndex(pookaCenter) };
    //if (fygar.GetGridPtr()->GetGrid()[index].hasBeenDug)
    //{
    //    fygar.GetOwner()->SetLocalPosition(fygar.GetGridPtr()->GetGrid()[index].spawnPosition);
    //    return &FygarStates::FygarState::moving;
    //}

    //auto targetPos{ fygar.GetTarget() };
    //auto pookaPos{ fygar.GetOwner()->GetWorldPosition() };

    //glm::vec2 direction = targetPos - pookaPos;

    //glm::vec2 normalizedDir = glm::normalize(direction);

    //fygar.GetOwner()->SetVelocity(normalizedDir * MOVEMENT_SPEED * deltaTime);

    return nullptr;
}

void GhostState::OnEnter(Fygar& fygar)
{
    m_Sprite = fygar.GetOwner()->GetComponent<SpriteComponent>();
    m_Sprite->SetNewTexture("Sprites/Fygar/FygarDefaultSprite.png", 2, 8, 6, 7);

    m_PookaCollider = fygar.GetOwner()->GetComponent<ColliderComponent>();
}

void GhostState::OnExit(Fygar&)
{
}

//-----------------------------------------------------
// AttackState Class
//-----------------------------------------------------

FygarStates::FygarState* AttackState::Update(Fygar&, float)
{
    return nullptr;
}

void AttackState::OnEnter(Fygar& fygar)
{
    fygar.GetOwner()->GetComponent<SpriteComponent>()->SetNewTexture("Sprites/Fygar/FygarDefaultSprite.png", 2, 8, 0, 0);
}

void AttackState::OnExit(Fygar&)
{
}

//-----------------------------------------------------
// Statics
//-----------------------------------------------------

MovingState     FygarState::moving;
InflatedState   FygarState::inflating;
DeathState      FygarState::dying;
GhostState      FygarState::ghosting;
AttackState     FygarState::attacking;