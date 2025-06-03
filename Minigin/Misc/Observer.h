#pragma once
#include <vector>
#include <memory>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <cstdint>
#include "Component.h"
#include "Singleton.h"

namespace dae
{
	using EventID = std::uint32_t;

	class EventRegistry final : public Singleton<EventRegistry>
	{
	public:
		EventID RegisterEvent(const std::string& name)
		{
			auto it = m_NameToID.find(name);
			if (it != m_NameToID.end())
				return it->second;

			EventID newID = m_NextID++;
			m_NameToID[name] = newID;
			m_IDToName[newID] = name;
			return newID;
		}

		EventID GetEventID(const std::string& name) const
		{
			auto it = m_NameToID.find(name);
			if (it != m_NameToID.end())
				return it->second;
			return INVALID_EVENT_ID;
		}

		std::string GetName(EventID id) const
		{
			auto it = m_IDToName.find(id);
			if (it != m_IDToName.end())
				return it->second;
			return {};
		}

		static constexpr EventID INVALID_EVENT_ID = static_cast<EventID>(-1);

	private:
		std::unordered_map<std::string, EventID> m_NameToID;
		std::unordered_map<EventID, std::string> m_IDToName;
		EventID m_NextID{ 0 };
	};

	class Observer
	{
	public:
		virtual ~Observer() = default;

		Observer& operator=(const Observer&) = delete;
		Observer& operator=(Observer&&) = delete;

		virtual void Notify(const dae::GameObject* gameObject, EventID event) = 0;
	};

	class Subject
	{
	public:
		void AddObserver(std::shared_ptr<Observer> observer)
		{
			m_Observers.push_back(observer);
		}

		void RemoveObserver(const Observer* observer)
		{
			std::erase_if(
				m_Observers,
				[observer](const std::shared_ptr<Observer>& ptr) { return ptr.get() == observer; }
			);
		}

	protected:
		void Notify(const dae::GameObject* gameObject, EventID event) const
		{
			for (const auto& observer : m_Observers)
			{
				observer->Notify(gameObject, event);
			}
		}

	private:
		std::vector<std::shared_ptr<Observer>> m_Observers;
	};
}
