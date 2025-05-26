#include "Scene.h"
#include "GameObject.h"
#include <algorithm>
#include <ranges>
#include "ServiceLocator.h"

using namespace dae;

unsigned int Scene::m_idCounter = 0;

dae::GameObject* Scene::GetPlayer(unsigned int index) const
{
	if (index < m_Players.size())
	{
		return m_Players[index].get();
	}
	return nullptr;
}

dae::GameObject* Scene::GetEntity(const glm::vec2& position) const
{
	auto it = std::ranges::find_if(m_Entities, [&position](const std::unique_ptr<dae::GameObject>& obj)
		{
			return obj->GetWorldPosition().x == position.x && obj->GetWorldPosition().y == position.y;
		});

	if (it != m_Entities.end())
	{
		return it->get();
	}
	return nullptr;
}

dae::GameObject* Scene::GetGround(const glm::vec2& position) const
{
	auto it = std::ranges::find_if(m_Ground, [&position](const std::unique_ptr<dae::GameObject>& obj)
		{
			return obj->GetWorldPosition().x == position.x && obj->GetWorldPosition().y == position.y;
		});

	if (it != m_Ground.end())
	{
		return it->get();
	}
	return nullptr;
}

Scene::Scene(const std::string& name, bool setActive)
	:	m_name(name),
		m_IsActive{ setActive }
{}

Scene::~Scene() = default;

void Scene::Add(std::unique_ptr<dae::GameObject>& object)
{
	RenderLayer layer = object->GetRenderLayer();

	switch (layer)
	{
	case RenderLayer::Ground:
		m_Ground.emplace_back(std::move(object));
		break;
	case RenderLayer::Entity:
		m_Entities.emplace_back(std::move(object));
		break;
	case RenderLayer::Player:
		m_Players.emplace_back(std::move(object));
		break;
	default: //No layer = middleground (always over ground but under player)
		m_Entities.emplace_back(std::move(object));
		break;
	}
}

void Scene::MarkForDeletion(dae::GameObject* object)
{
	if (object && std::find(m_PendingDeleteObjects.begin(), m_PendingDeleteObjects.end(), object) == m_PendingDeleteObjects.end())
	{
		m_PendingDeleteObjects.emplace_back(object);
	}
}

void Scene::Remove(dae::GameObject* object)
{
	auto removeFrom = [object](std::vector<std::unique_ptr<dae::GameObject>>& container)
		{
			container.erase(
				std::remove_if(container.begin(), container.end(),
					[object](const std::unique_ptr<dae::GameObject>& ptr)
					{
						return ptr.get() == object;
					}),
				container.end());
		};

	switch (object->GetRenderLayer())
	{
	case RenderLayer::Ground:
		removeFrom(m_Ground);
		break;
	case RenderLayer::Entity:
		removeFrom(m_Entities);
		break;
	case RenderLayer::Player:
		removeFrom(m_Players);
		break;
	}
}

void Scene::RemoveAll()
{
	m_Ground.clear();
	m_Entities.clear();
	m_Players.clear();
}

void Scene::Update(float deltaTime)
{
	for (auto& object : m_Ground)   object->Update(deltaTime);
	for (auto& object : m_Entities) object->Update(deltaTime);
	for (auto& object : m_Players)  object->Update(deltaTime);

	// Collision check
	ServiceLocator::GetCollisionSystem().CheckCollisions();

	// Remove objects marked for deletion
	for (auto* object : m_PendingDeleteObjects)
	{
		Remove(object);
	}
	m_PendingDeleteObjects.clear();
}

void Scene::Render() const
{
	for (auto& object : m_Ground)   object->Render();
	for (auto& object : m_Entities) object->Render();
	for (auto& object : m_Players)  object->Render();
}

