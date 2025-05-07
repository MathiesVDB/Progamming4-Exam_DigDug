    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
    #include <SDL.h>

    #if _DEBUG
    #if __has_include(<vld.h>)
    #include <vld.h>
    #endif
    #endif

    #include "FPSComponent.h"
    #include "HealthComponent.h"
    #include "HealthDisplayer.h"
    #include "Minigin.h"
    #include "SceneManager.h"
    #include "ResourceManager.h"
    #include "TextObject.h"
    #include "Scene.h"
    #include "TextureComponent.h"
    #include "Transform.h"
    #include "InputManager.h"
    #include "SoundSystem.h"
    #include "ServiceLocator.h"
    #include "DamageSound.h"
    #include "Pooka.h"
    #include "SpriteComponent.h"
	#include "Level.h"

    using namespace dae;

    void load()
    {
        ServiceLocator::RegisterSoundSystem(std::make_unique<SoundSystem>());

        auto& scene = dae::SceneManager::GetInstance().CreateScene("DigDug");

        auto backgroundGameObject = std::make_unique<dae::GameObject>();
        backgroundGameObject->AddComponent<TextureComponent>("background.tga");
        scene.Add(backgroundGameObject);

        auto logoGameObject = std::make_unique<dae::GameObject>();
        logoGameObject->AddComponent<TextureComponent>("logo.tga");
        logoGameObject->GetComponent<Transform>()->SetPosition(216, 180, 0);
        scene.Add(logoGameObject);

        auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
        auto textGameObject = std::make_unique<dae::GameObject>();
        textGameObject->AddComponent<dae::TextObject>("Programming 4 Assignment", font);
        textGameObject->GetComponent<Transform>()->SetPosition(80, 20, 0);
        scene.Add(textGameObject);

        auto smallFont = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 24);
        auto infoTextGameObject = std::make_unique<dae::GameObject>();
        infoTextGameObject->AddComponent<dae::TextObject>("Press 'C' to damage (and play damage sound)", smallFont);
        infoTextGameObject->GetComponent<Transform>()->SetPosition(5, 120, 0);
        scene.Add(infoTextGameObject);

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