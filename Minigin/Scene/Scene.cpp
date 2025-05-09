#include "Scene.h"
#include "GameObject.h"

#include <algorithm>

#include "ServiceLocator.h"

using namespace dae;

unsigned int Scene::m_idCounter = 0;

Scene::Scene(const std::string& name) : m_name(name) {}

Scene::~Scene() = default;

void Scene::Add(std::unique_ptr<dae::GameObject>& object)
{
	m_objects.emplace_back(std::move(object));
}

void Scene::Remove(dae::GameObject* object)
{
	m_objects.erase(
		std::remove_if(m_objects.begin(), m_objects.end(),
			[object](const std::unique_ptr<dae::GameObject>& ptr)
			{
				return ptr.get() == object;
			}),
		m_objects.end());
}


void Scene::RemoveAll()
{
	m_objects.clear();
}

void Scene::Update(float deltaTime)
{
	for(auto& object : m_objects)
	{
		object->Update(deltaTime);
	}

	// Collision check
	ServiceLocator::GetCollisionSystem().CheckCollisions();
}

void Scene::Render() const
{
	for (const auto& object : m_objects)
	{
		object->Render();
	}
}

