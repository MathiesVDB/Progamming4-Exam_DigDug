    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <SDL.h>

    #if _DEBUG
    #if __has_include(<vld.h>)
    #include <vld.h>
    #endif
    #endif

    #include "FPSComponent.h"
    #include "Minigin.h"
    #include "SceneManager.h"
    #include "ResourceManager.h"
    #include "Scene.h"
    #include "SoundSystem.h"
    #include "ServiceLocator.h"
	#include "Level.h"

    using namespace dae;

    void load()
    {
        ServiceLocator::RegisterSoundSystem(std::make_unique<RealSoundSystem>());
        ServiceLocator::RegisterCollisionSystem(std::make_unique<RealCollisionSystem>());

        auto& scene = dae::SceneManager::GetInstance().CreateScene("DigDug");

        auto FPSGameObject = std::make_unique<dae::GameObject>();
        FPSGameObject->AddComponent<FPSComponent>();
        scene.Add(FPSGameObject);

        auto levelGameObject = std::make_unique<dae::GameObject>();
        levelGameObject->AddComponent<Level>("Level1");
        levelGameObject->AddComponent<GridComponent>();
		levelGameObject->GetComponent<Level>()->LoadLevel("Level1.json");
        scene.Add(levelGameObject);
    }

    int main(int, char* []) {
	    dae::Minigin engine("../Data/");
	    engine.Run(load);
	    return 0;
    }