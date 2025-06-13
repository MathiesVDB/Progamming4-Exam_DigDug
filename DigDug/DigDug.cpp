    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <SDL.h>

    #if _DEBUG
    #if __has_include(<vld.h>)
    #include <vld.h>
    #endif
    #endif

#include "CollisionHandler.h"
#include "Command.h"
#include "FPSComponent.h"
#include "GameDirector.h"
#include "InputManager.h"
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
        dae::EventRegistry::GetInstance().RegisterEvent("GameOver");
        dae::EventRegistry::GetInstance().RegisterEvent("RockBroke");

        static auto collisionHandler = std::make_shared<CollisionHandler>();
        ServiceLocator::GetCollisionSystem().AddObserver(collisionHandler);

        auto skipLevelCommand = std::make_unique<SkipLevelCommand>();
        skipLevelCommand->SetGlobal();

        auto muteCommand = std::make_unique<MuteCommand>();
        muteCommand->SetGlobal();

        InputManager::GetInstance().AddCommand(SDL_SCANCODE_F1, KeyState::Down, std::move(skipLevelCommand));
        InputManager::GetInstance().AddCommand(SDL_SCANCODE_F2, KeyState::Down, std::move(muteCommand));

        static auto soundHandler = std::make_shared<SoundHandler>();
        static auto scoreHandler = std::make_shared<ScoreHandler>();

        GameDirector::GetInstance().Init(soundHandler, scoreHandler);
    }

    int main(int, char* []) {
	    dae::Minigin engine("../Data/");
	    engine.Run(load);
	    return 0;
    }