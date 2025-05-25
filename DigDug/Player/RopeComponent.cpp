#include "RopeComponent.h"

#include "ColliderComponent.h"
#include "GameObject.h"
#include "Scene.h"
#include "TextureComponent.h"

//RopeComponent::RopeComponent(dae::GameObject* owner, MoveDirection direction, Player& player)
//	:	Component(owner),
//		m_Direction(direction),
//		m_Player(player)
//{
//    constexpr float segmentSize = 16.f;
//    glm::vec3 start = owner->GetWorldPosition();
//    glm::vec3 offset{};
//
//    switch (m_Direction)
//    {
//    case MoveDirection::Left:  offset = { -segmentSize, 0, 0 }; break;
//    case MoveDirection::Right: offset = { segmentSize, 0, 0 };  break;
//    case MoveDirection::Up:    offset = { 0, -segmentSize, 0 }; break;
//    case MoveDirection::Down:  offset = { 0, segmentSize, 0 };  break;
//    }
//
//    for (int i = 0; i < 3; ++i)
//    {
//        glm::vec3 pos = start + offset * static_cast<float>(i + 1);
//        bool isHead = (i == 2); // last segment
//        SpawnSegment(pos, isHead);
//    }
//}
//
//void RopeComponent::SpawnSegment(const glm::vec3& pos, bool isHead)
//{
//    auto segment = std::make_unique<dae::GameObject>();
//    segment->SetLocalPosition(pos);
//
//    auto sprite = segment->AddComponent<TextureComponent>();
//
//    // Set collider (only head needs to check hit)
//    if (isHead)
//    {
//        auto collider = segment->AddComponent<ColliderComponent>();
//        // Setup collision detection with enemies
//    }
//
//    // Parent it to rope object for transform inheritance
//    m_pOwner->AddChild(segment.get());
//
//    // Add to scene & tracking
//    m_Segments.push_back(segment.get());
//    dae::SceneManager::()->Add(std::move(segment));
//}
