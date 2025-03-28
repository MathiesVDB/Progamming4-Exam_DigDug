#include "FPSComponent.h"
#include "TextObject.h"
#include "Time.h"
#include "ResourceManager.h"
#include <memory>
#include <string>
#include <cmath>

FPSComponent::FPSComponent()
{
    auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 30);
    m_TextObject = std::make_shared<dae::TextObject>("0 FPS", font);
}

void FPSComponent::Update(float deltaTime)
{
    m_FrameCount++;
    m_ElapsedTime += deltaTime;

    if (m_ElapsedTime >= 0.2f)
    {
        RecalculateFPS();
    }

    m_DeltaTime = deltaTime;
}

void FPSComponent::RecalculateFPS()
{
    float fps = std::round(m_FrameCount / m_ElapsedTime);
    m_TextObject->SetText(std::to_string(static_cast<int>(fps)) + " FPS");
    m_FrameCount = 0;
    m_ElapsedTime = 0.0f;
}

void FPSComponent::Render() const
{
    m_TextObject->Update(m_DeltaTime);
    m_TextObject->Render();
}