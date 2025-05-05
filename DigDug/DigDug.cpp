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
    #include "SpriteComponent.h"

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

        auto Player1 = std::make_unique<dae::GameObject>();
        Player1->AddComponent<SpriteComponent>("Sprites/Player/WalkingSprite.bmp", 1, 8, 0.25f, 0, 1);
        Player1->GetComponent<Transform>()->SetPosition(180, 90, 0);
        Player1->AddComponent<HealthComponent>(3);
        scene.Add(Player1);

        auto lifeDisplay1GameObject = std::make_unique<dae::GameObject>();
        lifeDisplay1GameObject->AddComponent<dae::TextObject>("Lives: " + std::to_string(Player1->GetComponent<HealthComponent>()->GetLives()), font);
        lifeDisplay1GameObject->GetComponent<Transform>()->SetPosition(10, 150, 0);
        scene.Add(lifeDisplay1GameObject);

        auto healthDisplay = std::make_unique<HealthDisplay>(lifeDisplay1GameObject.get(), Player1.get());
        Player1->GetComponent<HealthComponent>()->AddObserver(std::move(healthDisplay));

        auto damageSound = std::make_unique<DamageSound>(Player1.get(), "Dig Dug SFX (4).wav");
        Player1->GetComponent<HealthComponent>()->AddObserver(std::move(damageSound));

        auto& inputManager = InputManager::GetInstance();

        inputManager.AddCommand(SDL_SCANCODE_W, KeyState::Pressed, std::make_shared<MoveCommand>(Player1.get(), MoveCommand::Direction::Up));
        inputManager.AddCommand(SDL_SCANCODE_A, KeyState::Pressed, std::make_shared<MoveCommand>(Player1.get(), MoveCommand::Direction::Left));
        inputManager.AddCommand(SDL_SCANCODE_S, KeyState::Pressed, std::make_shared<MoveCommand>(Player1.get(), MoveCommand::Direction::Down));
        inputManager.AddCommand(SDL_SCANCODE_D, KeyState::Pressed, std::make_shared<MoveCommand>(Player1.get(), MoveCommand::Direction::Right));

        inputManager.AddCommand(SDL_SCANCODE_C, KeyState::Pressed, std::make_shared<DamageCommand>(Player1.get()));
    }

    int main(int, char* []) {
	    dae::Minigin engine("../Data/");
	    engine.Run(load);
	    return 0;
    }