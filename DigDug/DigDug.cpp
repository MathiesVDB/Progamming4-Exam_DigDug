    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <SDL.h>

    #if _DEBUG
    #if __has_include(<vld.h>)
    #include <vld.h>
    #endif
    #endif

#include "CollisionHandler.h"
#include "FPSComponent.h"
#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "SoundSystem.h"
#include "ServiceLocator.h"
#include "Level.h"
#include "Observer.h"
#include "ScoreHandler.h"
#include "SoundHandler.h"

    using namespace dae;

    void load()
    {
        ServiceLocator::RegisterSoundSystem(std::make_unique<RealSoundSystem>());
        ServiceLocator::RegisterCollisionSystem(std::make_unique<RealCollisionSystem>());

        dae::EventRegistry::GetInstance().RegisterEvent("HealthIncrease");
        dae::EventRegistry::GetInstance().RegisterEvent("CollisionEvent");
        dae::EventRegistry::GetInstance().RegisterEvent("LifeLost"); // Player animation finished and player died
        dae::EventRegistry::GetInstance().RegisterEvent("EnemyDied");
        dae::EventRegistry::GetInstance().RegisterEvent("FygarFire");
        dae::EventRegistry::GetInstance().RegisterEvent("PlayerHit"); //Moment player gets hit
        dae::EventRegistry::GetInstance().RegisterEvent("PlayerAttack");
        dae::EventRegistry::GetInstance().RegisterEvent("PumpEnemy");
        dae::EventRegistry::GetInstance().RegisterEvent("Victory");
        dae::EventRegistry::GetInstance().RegisterEvent("RockHitEntity");

        static auto collisionHandler = std::make_shared<CollisionHandler>();
        ServiceLocator::GetCollisionSystem().AddObserver(collisionHandler);

        static auto soundHandler = std::make_shared<SoundHandler>();
        static auto scoreHandler = std::make_shared<ScoreHandler>();

        auto& scene = dae::SceneManager::GetInstance().CreateScene("MainScene");

        auto FPSGameObject = std::make_unique<dae::GameObject>();
        FPSGameObject->AddComponent<FPSComponent>();
        scene.Add(FPSGameObject);

        auto levelGameObject = std::make_unique<dae::GameObject>();
        levelGameObject->AddComponent<Level>("Level1", soundHandler, scoreHandler);
        levelGameObject->AddComponent<GridComponent>();
		levelGameObject->GetComponent<Level>()->LoadLevel("Level1.json");
        scene.Add(levelGameObject);
    }

    int main(int, char* []) {
	    dae::Minigin engine("../Data/");
	    engine.Run(load);
	    return 0;
    }