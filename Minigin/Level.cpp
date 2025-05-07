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
	auto grid = GetOwner()->GetComponent<GridComponent>()->GetGrid();

	std::string filePath = "../Data/Levels/" + fileName;
	std::ifstream file(filePath);
	if (!file.is_open())
	{
		std::cerr << "Failed to open level file: " << fileName << std::endl;
		return;
	}

	std::string line;
	bool inLevelArray = false;
	int row = 0;

	while (std::getline(file, line))
	{
		if (!inLevelArray)
		{
			if (line.find("\"level\"") != std::string::npos)
				inLevelArray = true;

			continue;
		}

		if (line.find("]") != std::string::npos)
			break;

		line.erase(remove(line.begin(), line.end(), '\"'), line.end());
		line.erase(remove(line.begin(), line.end(), ','), line.end());
		line.erase(remove(line.begin(), line.end(), '\n'), line.end());
		line.erase(remove(line.begin(), line.end(), '\r'), line.end());

		for (size_t col = 0; col < line.length(); ++col)
		{
			char tile = line[col];
			int x = static_cast<int>(col * 16); 
			int y = static_cast<int>(row * 16);

			int index = y * GridComponent::COLUMNS + x;

			if (index < 0 || index >= GridComponent::ROWS * GridComponent::COLUMNS) continue;

			Point2f spawnPos = grid[index].spawnPosition;

			switch (tile)
			{
			case 'R': SpawnRock(spawnPos);			 break;
			case '1': SpawnDirtYellow(spawnPos);	 break;
			case '2': SpawnDirtOrangeLight(spawnPos);break;
			case '3': SpawnDirtOrangeDark(spawnPos); break;
			case '4': SpawnDirtRed(spawnPos);		 break;
			case 'p': SpawnPlayer(spawnPos);		 break;
			case 'P': SpawnPooka(spawnPos);			 break;
			case 'F': SpawnFygar(spawnPos);			 break;
			case '#': SpawnEmpty(spawnPos);			 break;
			default:
				std::cerr << "Unknown tile type: " << tile << std::endl;
				break;
			}
		}
		row++;
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
	PookaGameObject->AddComponent<SpriteComponent>("Sprites/Pooka/DefaultSprite.bmp", 2, 5, 0.25f, 0, 1);
	PookaGameObject->GetComponent<dae::Transform>()->SetPosition(spawnPos.x, spawnPos.y, 0);
	PookaGameObject->AddComponent<Pooka>();
	m_Scene.Add(PookaGameObject);
}

void Level::SpawnFygar(const Point2f& spawnPos) const
{
	auto FygarGameObject = std::make_unique<dae::GameObject>();
	FygarGameObject->AddComponent<SpriteComponent>("Sprites/Fygar/DefaultSprite.bmp", 2, 8, 0.25f, 0, 1);
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
	DirtYellowGameObject->AddComponent<TextureComponent>("Sprites/WorldTiles/Yellow.png");
	DirtYellowGameObject->AddComponent<ColliderComponent>();
	DirtYellowGameObject->GetComponent<dae::Transform>()->SetPosition(spawnPos.x, spawnPos.y, 0);
	m_Scene.Add(DirtYellowGameObject);
}

void Level::SpawnDirtOrangeLight(const Point2f& spawnPos) const
{
	auto DirtOrangeLightGameObject = std::make_unique<dae::GameObject>();
	DirtOrangeLightGameObject->AddComponent<TextureComponent>("Sprites/WorldTiles/OrangeLight.png");
	DirtOrangeLightGameObject->AddComponent<ColliderComponent>();
	DirtOrangeLightGameObject->GetComponent<dae::Transform>()->SetPosition(spawnPos.x, spawnPos.y, 0);
	m_Scene.Add(DirtOrangeLightGameObject);
}

void Level::SpawnDirtOrangeDark(const Point2f& spawnPos) const
{
	auto DirtOrangeDarkGameObject = std::make_unique<dae::GameObject>();
	DirtOrangeDarkGameObject->AddComponent<TextureComponent>("Sprites/WorldTiles/OrangeDark.png");
	DirtOrangeDarkGameObject->AddComponent<ColliderComponent>();
	DirtOrangeDarkGameObject->GetComponent<dae::Transform>()->SetPosition(spawnPos.x, spawnPos.y, 0);
	m_Scene.Add(DirtOrangeDarkGameObject);
}

void Level::SpawnDirtRed(const Point2f& spawnPos) const
{
	auto DirtRedGameObject = std::make_unique<dae::GameObject>();
	DirtRedGameObject->AddComponent<TextureComponent>("Sprites/WorldTiles/Red.png");
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