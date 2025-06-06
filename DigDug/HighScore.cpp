#include "HighScore.h"
#include <fstream>
#include <sstream>

#include "Command.h"
#include "InputManager.h"
#include "ResourceManager.h"
#include "Scene.h"
#include "SceneManager.h"
#include "SceneSwitcher.h"
#include "ScoreHandler.h"

HighScore::HighScore(std::shared_ptr<ScoreHandler> scoreHandler)
	:	m_SaveScene			{ dae::SceneManager::GetInstance().CreateScene("HighScoreSaver") },
		m_HighScoresScene	{ dae::SceneManager::GetInstance().CreateScene("HighScoreShow") },
		m_ScoreHandler      { scoreHandler }
{
}

void HighScore::LoadSaveScene()
{
    auto& scene = m_SaveScene;

    auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

    auto titleGO = std::make_unique<dae::GameObject>();
    titleGO->AddComponent<dae::TextObject>("Choose name!", font);
    titleGO->SetLocalPosition({ dae::Minigin::WINDOW_WIDTH / 2.f - 100, 100.f });
    titleGO->SetRenderLayer(RenderLayer::Entity);
    scene.Add(titleGO);

    for (int iterator = 0; iterator < 3; ++iterator)
    {
        auto letterGO = std::make_unique<dae::GameObject>();
        letterGO->AddComponent<dae::TextObject>("A", font);
        letterGO->SetLocalPosition({ dae::Minigin::WINDOW_WIDTH / 2.f - 70 + iterator * 50.f, 200.f });
        letterGO->SetRenderLayer(RenderLayer::Entity);

        m_LetterObjects[iterator] = letterGO.get(); 
        scene.Add(letterGO);
    }

    auto& inputManager = InputManager::GetInstance();

    // Keyboard commands
    inputManager.AddCommand(SDL_SCANCODE_W, KeyState::Down, std::make_unique<LetterUpCommand>(this));
    inputManager.AddCommand(SDL_SCANCODE_S, KeyState::Down, std::make_unique<LetterDownCommand>(this));

    inputManager.AddCommand(SDL_SCANCODE_C, KeyState::Down, std::make_unique<LetterConfirmCommand>(this));

    // Controller commands
    inputManager.AddControllerCommand(SDL_CONTROLLER_BUTTON_DPAD_UP, KeyState::Down, std::make_unique<LetterUpCommand>(this));
    inputManager.AddControllerCommand(SDL_CONTROLLER_BUTTON_DPAD_DOWN, KeyState::Down, std::make_unique<LetterDownCommand>(this));

    inputManager.AddControllerCommand(SDL_CONTROLLER_BUTTON_A, KeyState::Down, std::make_unique<LetterConfirmCommand>(this));

    auto& letterGO = m_LetterObjects[m_SelectedLetterIndex];
    glm::vec2 pos = letterGO->GetWorldPosition();

    // Up Arrow
    auto upArrow = std::make_unique<dae::GameObject>();
    auto texture = upArrow->AddComponent<TextureComponent>("Sprites/Misc/Highlight/Arrow_Up.png");
    upArrow->SetLocalPosition({ pos.x, pos.y - texture->GetHeight() - 5.f});
    upArrow->SetRenderLayer(RenderLayer::Entity);
    m_UpArrowMarker = upArrow.get();
    scene.Add(upArrow);

    // Down Arrow
    auto downArrow = std::make_unique<dae::GameObject>();
    downArrow->AddComponent<TextureComponent>("Sprites/Misc/Highlight/Arrow_Down.png");
    downArrow->SetLocalPosition({ pos.x, pos.y + FONT_SIZE });
    downArrow->SetRenderLayer(RenderLayer::Entity);
    m_DownArrowMarker = downArrow.get();
    scene.Add(downArrow);

    dae::SceneManager::GetInstance().SetActiveScene(m_SaveScene);
}

void HighScore::LoadHighScoreScene()
{
    auto& scene = m_HighScoresScene;

    auto font = dae::ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);

    auto titleGO = std::make_unique<dae::GameObject>();
    titleGO->AddComponent<dae::TextObject>("High Scores", font);
    titleGO->SetLocalPosition({ 100.f, 100.f });
    titleGO->SetRenderLayer(RenderLayer::Entity);
    scene.Add(titleGO);

    for (size_t index = 0; index < m_HighScores.size(); ++index)
    {
	    constexpr float lineHeight  {  40.f };
	    constexpr float startY      { 160.f };

	    const auto& [name, score] = m_HighScores[index];
        std::stringstream ss;
        ss << index + 1 << ". " << std::setw(3) << std::left << name << "     " << score;

        auto scoreGO = std::make_unique<dae::GameObject>();
        scoreGO->AddComponent<dae::TextObject>(ss.str(), font);
        scoreGO->SetLocalPosition({ 100.f, startY + index * lineHeight });
        scoreGO->SetRenderLayer(RenderLayer::Entity);
        scene.Add(scoreGO);
    }

    dae::SceneManager::GetInstance().SetActiveScene(m_SaveScene);
}


void HighScore::FinishEntry()
{
    m_EntryComplete = true;

    std::string finalName(m_NameLetters.begin(), m_NameLetters.end());

    LoadHighScores(finalName);
    SaveHighScores();

    dae::SceneSwitcher::GetInstance().QueueSceneChange([this]() {
        LoadHighScoreScene();
        dae::SceneManager::GetInstance().SetActiveScene(m_HighScoresScene);
        });
}

void HighScore::UpdateLetterDisplay()
{
     if (auto* text = m_LetterObjects[m_SelectedLetterIndex]->GetComponent<dae::TextObject>())
     {
         text->SetText(std::string(1, m_NameLetters[m_SelectedLetterIndex]));
     }

     auto& letterGO = m_LetterObjects[m_SelectedLetterIndex];
     glm::vec2 pos = letterGO->GetWorldPosition();

     if (m_UpArrowMarker)	m_UpArrowMarker->SetLocalPosition({ pos.x, pos.y - 35.f });

     if (m_DownArrowMarker)	m_DownArrowMarker->SetLocalPosition({ pos.x, pos.y + FONT_SIZE });
}

void HighScore::LoadHighScores(std::string chosenName)
{
    m_HighScores.clear();

    m_HighScores.emplace_back(std::pair<std::string, int>{chosenName, m_ScoreHandler->GetScore() });

    std::ifstream inFile("HighScores.txt");
    if (!inFile)
    {
        std::cerr << "Failed to open HighScores.txt for reading\n";
        return;
    }

    std::string line;
    while (std::getline(inFile, line))
    {
        std::istringstream iss(line);
        std::string name;
        int score;

        if (!(iss >> name >> score))
        {
            std::cerr << "Invalid line format: " << line << '\n';
            continue;
        }

        m_HighScores.emplace_back(name, score);
    }

    std::sort(m_HighScores.begin(), m_HighScores.end(),
        [](const auto& a, const auto& b) {
            return a.second > b.second;
        });

    //Trim to top 8
    if (m_HighScores.size() > 8)
        m_HighScores.resize(8);
}


void HighScore::SaveHighScores()
{
    std::ofstream outFile("HighScores.txt", std::ios::trunc);
    if (!outFile)
    {
        std::cerr << "Failed to open HighScores.txt for writing\n";
        return;
    }

    for (const auto& [name, score] : m_HighScores)
    {
        outFile << name << ' ' << score << '\n';
    }
}
