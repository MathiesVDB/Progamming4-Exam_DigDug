//---------------------------
// Include Files
//---------------------------
#include "fstream"
#include "Level.h"

#include "ColliderComponent.h"
#include "Command.h"
#include "DamageSound.h"
#include "HealthDisplayer.h"
#include "InputManager.h"
#include "ResourceManager.h" 
#include "SpriteComponent.h"
#include "Enemies/Pooka.h"

//---------------------------
// Constructor & Destructor
//---------------------------
Level::Level(dae::GameObject* owner, const std::string& sceneName)
	:   Component(owner),
		m_Scene(dae::SceneManager::GetInstance().CreateScene(sceneName))
{
}

void Level::LoadLevel(const std::string& fileName)
{
    auto gridComponent = GetOwner()->GetComponent<GridComponent>();

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
            if (tile == ' ' || tile == 'P' || tile == 'p' || tile == 'F' || tile == 'R') continue; // Skip spawns

            float x = static_cast<float>(col) * GridComponent::CELL_SIZE;
            float y = static_cast<float>(row) * GridComponent::CELL_SIZE;
            int index = gridComponent->GetCellIndex({ x, y });
            if (index < 0 || index >= GridComponent::ROWS * GridComponent::COLUMNS) continue;

            Point2f spawnPos = gridComponent->GetGrid()[index].spawnPosition;

            switch (tile)
            {
            case '1': SpawnDirtYellow(spawnPos);      break;
            case '2': SpawnDirtOrangeLight(spawnPos); break;
            case '3': SpawnDirtOrangeDark(spawnPos);  break;
            case '4': SpawnDirtRed(spawnPos);         break;
            case 'f': SpawnFlower(spawnPos);         break;
            case '#': SpawnEmpty(spawnPos);           break;
            default:
                std::cerr << "Unknown tile: " << tile << std::endl;
                break;
            }
        }
        ++row;
    }

    // --- Parse "spawn" array ---
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

        if (index < 0 || index >= GridComponent::ROWS * GridComponent::COLUMNS) continue;

        Point2f spawnPos = gridComponent->GetGrid()[index].spawnPosition;

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



void Level::SpawnPlayer(const Point2f& spawnPos) const
{
    auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
    auto textGameObject = std::make_unique<dae::GameObject>();
    textGameObject->AddComponent<dae::TextObject>("Programming 4 Assignment", font);
    textGameObject->GetComponent<dae::Transform>()->SetPosition(80, 20, 0);
    m_Scene.Add(textGameObject);

    auto Player1 = std::make_unique<dae::GameObject>();
    Player1->AddComponent<SpriteComponent>("Sprites/Player/WalkingSprite.bmp", 1, 8, 0.25f, 0, 1);
    Player1->GetComponent<dae::Transform>()->SetPosition(spawnPos.x, spawnPos.y, 0);
    Player1->AddComponent<HealthComponent>(3);

    int player1Lives = Player1->GetComponent<HealthComponent>()->GetLives();

    auto lifeDisplay1GameObject = std::make_unique<dae::GameObject>();
    lifeDisplay1GameObject->AddComponent<dae::TextObject>("Lives: " + std::to_string(player1Lives), font);
    lifeDisplay1GameObject->GetComponent<dae::Transform>()->SetPosition(10, 150, 0);

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

    m_Scene.Add(Player1);
    m_Scene.Add(lifeDisplay1GameObject);
}

void Level::SpawnPooka(const Point2f& spawnPos) const
{
	auto PookaGameObject = std::make_unique<dae::GameObject>();
	PookaGameObject->AddComponent<SpriteComponent>("Sprites/Pooka/PookaDefaultSprite.bmp", 2, 5, 0.25f, 0, 1);
	PookaGameObject->GetComponent<dae::Transform>()->SetPosition(spawnPos.x, spawnPos.y, 0);
	PookaGameObject->AddComponent<Pooka>();
	m_Scene.Add(PookaGameObject);
}

void Level::SpawnFygar(const Point2f& spawnPos) const
{
	auto FygarGameObject = std::make_unique<dae::GameObject>();
	FygarGameObject->AddComponent<SpriteComponent>("Sprites/Fygar/FygarDefaultSprite.bmp", 2, 8, 0.25f, 0, 1);
	FygarGameObject->GetComponent<dae::Transform>()->SetPosition(spawnPos.x, spawnPos.y, 0);
	m_Scene.Add(FygarGameObject);
}

void Level::SpawnRock(const Point2f& spawnPos) const
{
	auto RockGameObject = std::make_unique<dae::GameObject>();
	RockGameObject->AddComponent<SpriteComponent>("Sprites/Misc/EnvironmentSprite.bmp", 1, 7, 0.25f, 0, 0);
	RockGameObject->GetComponent<dae::Transform>()->SetPosition(spawnPos.x, spawnPos.y, 0);
	m_Scene.Add(RockGameObject);
}

void Level::SpawnDirtYellow(const Point2f& spawnPos) const
{
	auto DirtYellowGameObject = std::make_unique<dae::GameObject>();
	DirtYellowGameObject->AddComponent<TextureComponent>("Sprites/Misc/WorldTiles/Yellow.png");
	DirtYellowGameObject->AddComponent<ColliderComponent>();
	DirtYellowGameObject->GetComponent<dae::Transform>()->SetPosition(spawnPos.x, spawnPos.y, 0);
	m_Scene.Add(DirtYellowGameObject);
}

void Level::SpawnDirtOrangeLight(const Point2f& spawnPos) const
{
	auto DirtOrangeLightGameObject = std::make_unique<dae::GameObject>();
	DirtOrangeLightGameObject->AddComponent<TextureComponent>("Sprites/Misc/WorldTiles/OrangeLight.png");
	DirtOrangeLightGameObject->AddComponent<ColliderComponent>();
	DirtOrangeLightGameObject->GetComponent<dae::Transform>()->SetPosition(spawnPos.x, spawnPos.y, 0);
	m_Scene.Add(DirtOrangeLightGameObject);
}

void Level::SpawnDirtOrangeDark(const Point2f& spawnPos) const
{
	auto DirtOrangeDarkGameObject = std::make_unique<dae::GameObject>();
	DirtOrangeDarkGameObject->AddComponent<TextureComponent>("Sprites/Misc/WorldTiles/OrangeDark.png");
	DirtOrangeDarkGameObject->AddComponent<ColliderComponent>();
	DirtOrangeDarkGameObject->GetComponent<dae::Transform>()->SetPosition(spawnPos.x, spawnPos.y, 0);
	m_Scene.Add(DirtOrangeDarkGameObject);
}

void Level::SpawnDirtRed(const Point2f& spawnPos) const
{
	auto DirtRedGameObject = std::make_unique<dae::GameObject>();
	DirtRedGameObject->AddComponent<TextureComponent>("Sprites/Misc/WorldTiles/Red.png");
	DirtRedGameObject->AddComponent<ColliderComponent>();
	DirtRedGameObject->GetComponent<dae::Transform>()->SetPosition(spawnPos.x, spawnPos.y, 0);
	m_Scene.Add(DirtRedGameObject);
}

void Level::SpawnEmpty(const Point2f& spawnPos) const
{
	auto EmptyGameObject = std::make_unique<dae::GameObject>();
	EmptyGameObject->AddComponent<TextureComponent>("Sprites/Misc/WorldTiles/DiggedArea.png");
	EmptyGameObject->GetComponent<dae::Transform>()->SetPosition(spawnPos.x, spawnPos.y, 0);
	m_Scene.Add(EmptyGameObject);
}

void Level::SpawnFlower(const Point2f& spawnPos) const
{
	auto FlowerGameObject = std::make_unique<dae::GameObject>();
	FlowerGameObject->AddComponent<SpriteComponent>("Sprites/Misc/EnvironmentSprite.bmp", 1, 7, 0.25f, 5, 5);
	FlowerGameObject->GetComponent<dae::Transform>()->SetPosition(spawnPos.x, spawnPos.y, 0);
	m_Scene.Add(FlowerGameObject);
}