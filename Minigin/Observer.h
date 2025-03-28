#pragma once
#include <vector>
#include <memory>

#include "Component.h"


enum class Event
{
    HealthChanged,
    ScoreChanged
};

class Observer
{
public:
    virtual ~Observer() = default;
    virtual void Notify(const dae::GameObject* gameObject, Event event) = 0;
};

class Subject
{
public:
    void AddObserver(std::shared_ptr<Observer> observer)
    {
        m_Observers.push_back(observer);
    }

    void RemoveObserver(const std::shared_ptr<Observer> observer)
    {
        m_Observers.erase(std::remove(m_Observers.begin(), m_Observers.end(), observer), m_Observers.end());
    }

protected:
    void Notify(const dae::GameObject* gameObject, Event event)
    {
        for (const std::shared_ptr<Observer>& observer : m_Observers)
        {
            observer->Notify(gameObject, event);
        }
    }

private:
    std::vector<std::shared_ptr<Observer>> m_Observers;
};


//class ScoreDisplay
//{
//public:
//    explicit ScoreDisplay(std::shared_ptr<dae::GameObject>& displayObject)
//    {
//        m_DisplayObject = std::move(displayObject);
//
//        Observer::GetInstance().AddEventListener(Event::ScoreChanged, &ScoreDisplay::UpdateText);
//    }
//
//    static void UpdateText()
//    {
//        auto textComponent = instance->m_DisplayObject->GetComponent<dae::TextObject>();
//        if (!textComponent) return;
//
//        textComponent->SetText("Score: " + std::to_string(instance->m_Score));
//    }
//
//    void SetScore(int score)
//    {
//	    m_Score = score;
//    	instance = this;
//
//		Observer::GetInstance().Notify(Event::ScoreChanged);
//    }
//	int GetScore() const { return m_Score; }
//
//private:
//    std::shared_ptr<dae::GameObject> m_DisplayObject;
//    int m_Score = 0;
//    static ScoreDisplay* instance;
//};