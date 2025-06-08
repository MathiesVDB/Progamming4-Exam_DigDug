#include "Scene.h"
#include "GameObject.h"
#include <algorithm>
#include <ranges>

#include "Renderer.h"
#include "ServiceLocator.h"

using namespace dae;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name, bool setActive)
	: m_name(name),
	m_IsActive{ setActive }
{
	if (m_IsActive) dae::SceneManager::GetInstance().SetActiveScene(*this);
}

Scene::~Scene() = default;


dae::GameObject* Scene::GetPlayerByIndex(unsigned int index) const
{
	auto players{ GetObjectsByRenderLayer(RenderLayer::Player) };

	if (index < players.size())
	{
		return players[index];
	}
	return nullptr;
}

dae::GameObject* Scene::GetObjectByPosition(const glm::vec2& position) const
{
	auto it = std::ranges::find_if(m_SceneObjects, [&position](const std::unique_ptr<dae::GameObject>& obj)
		{
			return obj->GetWorldPosition().x == position.x && obj->GetWorldPosition().y == position.y;
		});

	if (it != m_SceneObjects.end())
	{
		return it->get();
	}
	return nullptr;
}

std::vector<dae::GameObject*> Scene::GetObjectsByRenderLayer(RenderLayer layer) const
{
	std::vector<dae::GameObject*> results;
	for (const auto& object : m_SceneObjects)
	{
		if (object->GetRenderLayer() == layer)
		{
			results.push_back(object.get());
		}
	}
	return results;
}

void Scene::MarkForAdd(std::unique_ptr<dae::GameObject> object)
{
	m_PendingAddObjects.emplace_back(std::move(object));
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

	removeFrom(m_SceneObjects);
}

void Scene::Add(std::unique_ptr<dae::GameObject> object)
{
	m_SceneObjects.emplace_back(std::move(object));
}

void Scene::RemoveAll()
{
	m_SceneObjects.clear();
	dae::Renderer::GetInstance().SetDirty();
}

void Scene::Update(float deltaTime)
{
	for (auto& object : m_SceneObjects) object->Update(deltaTime);

	// Collision check
	ServiceLocator::GetCollisionSystem().CheckCollisions();

	//Add objects marked for addition
	for (auto& object : m_PendingAddObjects)
	{
		Add(std::move(object));
		dae::Renderer::GetInstance().SetDirty();
	}
	m_PendingAddObjects.clear();

	// Remove objects marked for deletion
	for (auto* object : m_PendingDeleteObjects)
	{
		Remove(object);
		dae::Renderer::GetInstance().SetDirty();
	}
	m_PendingDeleteObjects.clear();
}