//---------------------------
// Include Files
//---------------------------
#include "fstream"
#include "Level.h"
#include "ColliderComponent.h"
#include "Command.h"
#include "SoundHandler.h"
#include "Fygar.h"
#include "HealthDisplayer.h"
#include "InputManager.h"
#include "ResourceManager.h" 
#include "SpriteComponent.h"
#include "Pooka.h"
#include "Player.h"
#include "Rock.h"
#include "RopeComponent.h"
#include "RopeHeadComponent.h"
#include "ScoreDisplayer.h"
#include "ScoreHandler.h"

//---------------------------
// Constructor & Destructor
//---------------------------
Level::Level(dae::GameObject* owner, const std::string& sceneName)
	:   Component(owner),
		m_Scene(dae::SceneManager::GetInstance().CreateScene(sceneName, true)),
		m_SoundHandler{std::make_shared<SoundHandler>()}
{
    auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

    auto ScoreTextDisplayGameObject = std::make_unique<dae::GameObject>();
    auto scoreDisplayText = ScoreTextDisplayGameObject->AddComponent<dae::TextObject>("0", font);
    ScoreTextDisplayGameObject->SetLocalPosition({ dae::Minigin::WINDOW_WIDTH / 2 - 10, 25 });
    ScoreTextDisplayGameObject->SetRenderLayer(RenderLayer::Entity);

    m_ScoreDisplayer = std::make_unique<ScoreDisplayer>(scoreDisplayText);

    m_ScoreHandler = std::make_shared<ScoreHandler>(m_ScoreDisplayer.get());

    m_Scene.Add(ScoreTextDisplayGameObject);
}

void Level::LoadLevel(const std::string& fileName)
{
    m_GridComponent = GetOwner()->GetComponent<GridComponent>();

    std::string filePath = "../Data/Levels/" + fileName;
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << "Failed to open level file: " << fileName << std::endl;
        return;
    }

    std::string fileContent((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    // --- Parse "level" array ---
    size_t levelStart = fileContent.find("\"level\"");
    if (levelStart == std::string::npos)
    {
        std::cerr << "Missing \"level\" section in file.\n";
        return;
    }

    size_t levelArrayStart = fileContent.find("[", levelStart);
    size_t levelArrayEnd = fileContent.find("]", levelArrayStart);
    if (levelArrayStart == std::string::npos || levelArrayEnd == std::string::npos)
    {
        std::cerr << "Malformed \"level\" array.\n";
        return;
    }

    std::string levelData = fileContent.substr(levelArrayStart + 1, levelArrayEnd - levelArrayStart - 1);
    std::istringstream levelStream(levelData);
    std::string line;
    int row = 0;

    while (std::getline(levelStream, line, ','))
    {
        line.erase(remove(line.begin(), line.end(), '\"'), line.end());
        line.erase(remove(line.begin(), line.end(), '\n'), line.end());
        line.erase(remove(line.begin(), line.end(), '\r'), line.end());
        if (line.empty()) continue;

        line = line.substr(1);

        for (size_t col = 0; col < line.length(); ++col)
        {
            char tile = line[col];
            if (tile == ' ' || tile == 'P' || tile == 'p' || tile == 'F' || tile == 'R') continue;

            float x = static_cast<float>(col) * GridComponent::CELL_SIZE;
            float y = static_cast<float>(row) * GridComponent::CELL_SIZE;
            int index = m_GridComponent->GetCellIndex({ x, y });
            if (index < 0 || index >= GridComponent::COLUMNS * GridComponent::ROWS) continue;

            glm::vec2 spawnPos = m_GridComponent->GetGrid()[index].spawnPosition;
            if (tile == '#') m_GridComponent->GetGrid()[index].hasBeenDug = true;

            switch (tile)
            {
            case '1': SpawnDirtYellow(spawnPos);      break;
            case '2': SpawnDirtOrangeLight(spawnPos); break;
            case '3': SpawnDirtOrangeDark(spawnPos);  break;
            case '4': SpawnDirtRed(spawnPos);         break;
            case 'f': SpawnFlower(spawnPos);          break;
            case '#':
            {
                auto empty = SpawnEmpty(spawnPos);
                m_Scene.Add(empty);
                break;
            }
            default:
                std::cerr << "Unknown tile: " << tile << std::endl;
                break;
            }
        }
        ++row;
    }

    size_t spawnStart = fileContent.find("\"spawn\"");
    if (spawnStart == std::string::npos) return;

    size_t arrayStart = fileContent.find("[", spawnStart);
    if (arrayStart == std::string::npos) return;

    int bracketCount = 0;
    size_t arrayEnd = arrayStart;

    for (; arrayEnd < fileContent.size(); ++arrayEnd)
    {
        if (fileContent[arrayEnd] == '[') ++bracketCount;
        else if (fileContent[arrayEnd] == ']') --bracketCount;

        if (bracketCount == 0) break;
    }

    if (bracketCount != 0)
    {
        std::cerr << "Mismatched brackets in spawn array.\n";
        return;
    }

    std::string spawnSection = fileContent.substr(arrayStart, arrayEnd - arrayStart + 1);
    std::istringstream spawnStream(spawnSection);
    std::string entry;

    while (std::getline(spawnStream, entry, ']'))
    {
        size_t typeStart = entry.find('\"');
        size_t typeEnd = entry.find('\"', typeStart + 1);
        if (typeStart == std::string::npos || typeEnd == std::string::npos) continue;

        char tileType = entry[typeStart + 1];

        size_t comma = entry.find(',', typeEnd);
        if (comma == std::string::npos) continue;

        std::string indexStr = entry.substr(comma + 1);
        int index = std::stoi(indexStr);

        if (index < 0 || index >= GridComponent::COLUMNS * GridComponent::ROWS) continue;

        glm::vec2 spawnPos = m_GridComponent->GetGrid()[index].spawnPosition;

        switch (tileType)
        {
        case 'P': SpawnPooka(spawnPos);  break;
        case 'p': SpawnPlayer(spawnPos); break;
        case 'F': SpawnFygar(spawnPos);  break;
        case 'R': SpawnRock(spawnPos);   break;
        default:
            break;
        }
    }
}

void Level::SpawnPlayer(const glm::vec2& spawnPos) 
{
    auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

//--------------------------------------------------------------------------------------------------------------------------
//Create Rope (cannot make it seperate function without making a member variable to store rope before giving it to player)
//--------------------------------------------------------------------------------------------------------------------------

    //Start rope out of vision of the player
    auto RopeHeadGameObject = std::make_unique<dae::GameObject>();
    RopeHeadGameObject->ToggleOutOfBounds();
    RopeHeadGameObject->AddComponent<TextureComponent>("Sprites/Player/Weapon/Right/RightHead.png");
    RopeHeadGameObject->GetComponent<dae::Transform>()->SetPosition(-100, -100);
    RopeHeadGameObject->AddComponent<ColliderComponent>(ROPE);
    RopeHeadGameObject->SetRenderLayer(RenderLayer::Entity);

    auto RopeMiddleGameObject = std::make_unique<dae::GameObject>();
    RopeMiddleGameObject->ToggleOutOfBounds();
    RopeMiddleGameObject->AddComponent<TextureComponent>("Sprites/Player/Weapon/Right/RightMiddle.png");
    RopeMiddleGameObject->GetComponent<dae::Transform>()->SetPosition(-100, -100);
    RopeMiddleGameObject->SetRenderLayer(RenderLayer::Entity);

    auto RopeTailGameObject = std::make_unique<dae::GameObject>();
    RopeTailGameObject->ToggleOutOfBounds();
    RopeTailGameObject->AddComponent<TextureComponent>("Sprites/Player/Weapon/Right/RightTail.png");
    RopeTailGameObject->GetComponent<dae::Transform>()->SetPosition(-100, -100);
    RopeTailGameObject->SetRenderLayer(RenderLayer::Entity);

//--------------------------------------------------------------------------------------------------------------------------
//Create Player gameobject
//--------------------------------------------------------------------------------------------------------------------------

    auto Player1 = std::make_unique<dae::GameObject>();
    Player1->AddComponent<SpriteComponent>("Sprites/Player/WalkingSprite.png", 1, 8, 0.25f, 0, 1);
    Player1->GetComponent<dae::Transform>()->SetPosition(spawnPos.x, spawnPos.y);
    Player1->AddComponent<HealthComponent>(3);
	Player1->AddComponent<ColliderComponent>(FRIENDLY_ENTITY);
    auto ropeComponent = Player1->AddComponent<RopeComponent>(RopeHeadGameObject.get(), RopeMiddleGameObject.get(), RopeTailGameObject.get());
    Player1->AddComponent<Player>(m_GridComponent);
	Player1->GetComponent<Player>()->SetLevelPtr(this);

    RopeHeadGameObject->AddComponent<RopeHeadComponent>(ropeComponent);

    auto healthDisplay = std::make_shared<HealthDisplay>(Player1.get(), m_GridComponent);
    Player1->GetComponent<HealthComponent>()->AddObserver(healthDisplay);
    Player1->GetComponent<HealthComponent>()->AddObserver(m_SoundHandler);
    Player1->GetComponent<Player>()->AddObserver(m_SoundHandler);

//--------------------------------------------------------------------------------------------------------------------------
//Add player controls
//--------------------------------------------------------------------------------------------------------------------------

    auto& inputManager = InputManager::GetInstance();

    // Keyboard commands
    inputManager.AddCommand(SDL_SCANCODE_W, KeyState::Pressed, std::make_unique<MoveCommand>(Player1.get(), MoveDirection::Up   , 50.f));
    inputManager.AddCommand(SDL_SCANCODE_A, KeyState::Pressed, std::make_unique<MoveCommand>(Player1.get(), MoveDirection::Left , 50.f));
    inputManager.AddCommand(SDL_SCANCODE_S, KeyState::Pressed, std::make_unique<MoveCommand>(Player1.get(), MoveDirection::Down , 50.f));
    inputManager.AddCommand(SDL_SCANCODE_D, KeyState::Pressed, std::make_unique<MoveCommand>(Player1.get(), MoveDirection::Right, 50.f));

    inputManager.AddCommand(SDL_SCANCODE_C, KeyState::Down, std::make_unique<AttackCommand>(Player1.get()));

	// Controller commands
    inputManager.AddControllerCommand(SDL_CONTROLLER_BUTTON_DPAD_UP   , KeyState::Pressed, std::make_unique<MoveCommand>(Player1.get(), MoveDirection::Up   , 50.f));
	inputManager.AddControllerCommand(SDL_CONTROLLER_BUTTON_DPAD_LEFT , KeyState::Pressed, std::make_unique<MoveCommand>(Player1.get(), MoveDirection::Left , 50.f));
	inputManager.AddControllerCommand(SDL_CONTROLLER_BUTTON_DPAD_DOWN , KeyState::Pressed, std::make_unique<MoveCommand>(Player1.get(), MoveDirection::Down , 50.f));
	inputManager.AddControllerCommand(SDL_CONTROLLER_BUTTON_DPAD_RIGHT, KeyState::Pressed, std::make_unique<MoveCommand>(Player1.get(), MoveDirection::Right, 50.f));

	inputManager.AddControllerCommand(SDL_CONTROLLER_BUTTON_A, KeyState::Down, std::make_unique<AttackCommand>(Player1.get()));

    Player1->SetRenderLayer(RenderLayer::Player);
    m_Scene.Add(Player1);

    m_Scene.Add(RopeHeadGameObject);
    m_Scene.Add(RopeMiddleGameObject);
    m_Scene.Add(RopeTailGameObject);
}

void Level::SpawnPooka(const glm::vec2& spawnPos) const
{
	auto PookaGameObject = std::make_unique<dae::GameObject>();
	PookaGameObject->AddComponent<SpriteComponent>("Sprites/Pooka/PookaDefaultSprite.png", 2, 5, 0.25f, 0, 1);
	PookaGameObject->GetComponent<dae::Transform>()->SetPosition(spawnPos.x, spawnPos.y);
	PookaGameObject->AddComponent<ColliderComponent>(ENEMY_ENTITY);
	PookaGameObject->AddComponent<Pooka>(m_GridComponent);
    PookaGameObject->GetComponent<Pooka>()->AddObserver(m_SoundHandler);
    PookaGameObject->GetComponent<Pooka>()->AddObserver(m_ScoreHandler);

    PookaGameObject->SetRenderLayer(RenderLayer::Entity);
	m_Scene.Add(PookaGameObject);
}

void Level::SpawnFygar(const glm::vec2& spawnPos) const
{
	auto FygarGameObject = std::make_unique<dae::GameObject>();
	FygarGameObject->AddComponent<SpriteComponent>("Sprites/Fygar/FygarDefaultSprite.png", 2, 8, 0.25f, 0, 1);
	FygarGameObject->GetComponent<dae::Transform>()->SetPosition(spawnPos.x, spawnPos.y);
	FygarGameObject->AddComponent<ColliderComponent>(ENEMY_ENTITY);
    FygarGameObject->AddComponent<Fygar>(m_GridComponent);
    FygarGameObject->GetComponent<Fygar>()->AddObserver(m_SoundHandler);
    FygarGameObject->GetComponent<Fygar>()->AddObserver(m_ScoreHandler);

    FygarGameObject->SetRenderLayer(RenderLayer::Entity);
	m_Scene.Add(FygarGameObject);
}

void Level::SpawnRock(const glm::vec2& spawnPos) const
{
	auto RockGameObject = std::make_unique<dae::GameObject>();
	RockGameObject->AddComponent<SpriteComponent>("Sprites/Misc/EnvironmentSprite.png", 1, 7, 0.25f, 0, 0);
	RockGameObject->GetComponent<dae::Transform>()->SetPosition(spawnPos.x + 8, spawnPos.y);
	RockGameObject->AddComponent<ColliderComponent>(ROCK);
    RockGameObject->AddComponent<Rock>(GetOwner()->GetComponent<GridComponent>());

    m_GridComponent->GetGrid()[m_GridComponent->GetCellIndex(spawnPos)].hasRock = true;

    RockGameObject->SetRenderLayer(RenderLayer::Entity); //May not technically be an entity but should render between ground and player
	m_Scene.Add(RockGameObject);
}

void Level::SpawnDirtYellow(const glm::vec2& spawnPos) const
{
	auto DirtYellowGameObject = std::make_unique<dae::GameObject>();
	DirtYellowGameObject->AddComponent<TextureComponent>("Sprites/Misc/WorldTiles/Yellow.png");
	DirtYellowGameObject->AddComponent<ColliderComponent>(GROUND);
	DirtYellowGameObject->GetComponent<dae::Transform>()->SetPosition(spawnPos.x, spawnPos.y);

	m_GridComponent->GetGrid()[m_GridComponent->GetCellIndex(spawnPos)].hasBeenDug = false;

    DirtYellowGameObject->SetRenderLayer(RenderLayer::Ground);
	m_Scene.Add(DirtYellowGameObject);
}

void Level::SpawnDirtOrangeLight(const glm::vec2& spawnPos) const
{
	auto DirtOrangeLightGameObject = std::make_unique<dae::GameObject>();
	DirtOrangeLightGameObject->AddComponent<TextureComponent>("Sprites/Misc/WorldTiles/OrangeLight.png");
	DirtOrangeLightGameObject->AddComponent<ColliderComponent>(GROUND);
	DirtOrangeLightGameObject->GetComponent<dae::Transform>()->SetPosition(spawnPos.x, spawnPos.y);

    m_GridComponent->GetGrid()[m_GridComponent->GetCellIndex(spawnPos)].hasBeenDug = false;

    DirtOrangeLightGameObject->SetRenderLayer(RenderLayer::Ground);
	m_Scene.Add(DirtOrangeLightGameObject);
}

void Level::SpawnDirtOrangeDark(const glm::vec2& spawnPos) const
{
	auto DirtOrangeDarkGameObject = std::make_unique<dae::GameObject>();
	DirtOrangeDarkGameObject->AddComponent<TextureComponent>("Sprites/Misc/WorldTiles/OrangeDark.png");
	DirtOrangeDarkGameObject->AddComponent<ColliderComponent>(GROUND);
	DirtOrangeDarkGameObject->GetComponent<dae::Transform>()->SetPosition(spawnPos.x, spawnPos.y);

    m_GridComponent->GetGrid()[m_GridComponent->GetCellIndex(spawnPos)].hasBeenDug = false;

    DirtOrangeDarkGameObject->SetRenderLayer(RenderLayer::Ground);
	m_Scene.Add(DirtOrangeDarkGameObject);
}

void Level::SpawnDirtRed(const glm::vec2& spawnPos) const
{
	auto DirtRedGameObject = std::make_unique<dae::GameObject>();
	DirtRedGameObject->AddComponent<TextureComponent>("Sprites/Misc/WorldTiles/Red.png");
	DirtRedGameObject->AddComponent<ColliderComponent>(GROUND);
	DirtRedGameObject->GetComponent<dae::Transform>()->SetPosition(spawnPos.x, spawnPos.y);

    m_GridComponent->GetGrid()[m_GridComponent->GetCellIndex(spawnPos)].hasBeenDug = false;

    DirtRedGameObject->SetRenderLayer(RenderLayer::Ground);
	m_Scene.Add(DirtRedGameObject);
}

std::unique_ptr<dae::GameObject> Level::SpawnEmpty(const glm::vec2& spawnPos) const
{
	auto EmptyGameObject = std::make_unique<dae::GameObject>();
	EmptyGameObject->AddComponent<TextureComponent>("Sprites/Misc/WorldTiles/DiggedArea.png");
	EmptyGameObject->GetComponent<dae::Transform>()->SetPosition(spawnPos.x, spawnPos.y);
    EmptyGameObject->AddComponent<ColliderComponent>(EMPTY_GROUND);

    EmptyGameObject->SetRenderLayer(RenderLayer::Ground);
	return EmptyGameObject;
}

std::unique_ptr<dae::GameObject> Level::SpawnRope(const glm::vec2& spawnPos) const
{
	auto RopeGameObject = std::make_unique<dae::GameObject>();
	RopeGameObject->GetComponent<dae::Transform>()->SetPosition(spawnPos.x, spawnPos.y);

	RopeGameObject->SetRenderLayer(RenderLayer::Ground);
	return RopeGameObject;
}

void Level::SpawnFlower(const glm::vec2& spawnPos) const
{
	auto FlowerGameObject = std::make_unique<dae::GameObject>();
	FlowerGameObject->AddComponent<SpriteComponent>("Sprites/Misc/EnvironmentSprite.png", 1, 7, 0.25f, 5, 5);
	FlowerGameObject->GetComponent<dae::Transform>()->SetPosition(spawnPos.x, spawnPos.y);

    FlowerGameObject->SetRenderLayer(RenderLayer::Ground);
	m_Scene.Add(FlowerGameObject);
}