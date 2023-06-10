#include "PacGameManager.h"
#include "PacGrid.h"
#include "Scene.h"
#include "RenderComponent.h"
#include "AStarPathFinder.h"
#include "PacNavigator.h"
#include "PacNPC.h"
#include "PacLevel.h"
#include "PacData.h"
#include "GameObject.h"
#include "PacJSONParser.h"
#include "CircleCollider.h"
#include "PacScoreComponent.h"
#include "PacScoreHUD.h"
#include "PacHealthHUD.h"
#include "PacHealthComponent.h"
#include "PacController.h"
#include "PacNPCState.h"
#include "GameObject.h"
#include <iostream>
#include <memory>
#include "SceneManager.h"
#include "TextObject.h"
#include "PacSpawner.h"
#include "Button.h"
#include "MathHelpers.h"


namespace dae
{

	void PacGameManager::Initialize(const PacData::PacGameData& gameData)
	{
		m_GameData = gameData;

	}

	void PacGameManager::LoadGamemodeSelect()
	{
		auto& scene_manager = SceneManager::Get();
		bool scene_exists = scene_manager.HasScene("GamemodeSelect");
		if (scene_exists)
		{
			scene_manager.SetActiveScene("GamemodeSelect");
			return;
		}

		auto& scene = scene_manager.CreateScene("GamemodeSelect");
		scene_manager.SetActiveScene("GamemodeSelect");

		auto single_go = std::make_shared<GameObject>();
		auto coop_go = std::make_shared<GameObject>();
		auto versus_go = std::make_shared<GameObject>();

		auto single_button = single_go->AddComponent<Button>();
		auto coop_button = coop_go->AddComponent<Button>();
		auto versus_button = versus_go->AddComponent<Button>();


		single_button->SetTexture(PacData::PacFiles::SinglePlayerBtn);
		coop_button->SetTexture(PacData::PacFiles::CoopBtn);
		versus_button->SetTexture(PacData::PacFiles::VersusBtn);

		single_button->OnClick.AddFunction([this]()
			{ m_GameData.mode = PacData::PacGameMode::SINGLE; LoadLevel(0); });

		coop_button->OnClick.AddFunction([this]()
			{ m_GameData.mode = PacData::PacGameMode::COOP; LoadLevel(0); });

		versus_button->OnClick.AddFunction([this]()
			{ m_GameData.mode = PacData::PacGameMode::VERSUS; LoadLevel(0); });

		constexpr float half_width = g_WindowSize.x * .5f;
		constexpr float half_height = g_WindowSize.y * .5f;
		constexpr glm::vec2 btn_size{ 128.f };
		constexpr float margin = 10.f;

		single_button->SetSize(btn_size);
		coop_button->SetSize(btn_size);
		versus_button->SetSize(btn_size);

		single_go->GetTransform()->SetLocalPosition({ half_width - margin - btn_size.x, half_height });
		coop_go->GetTransform()->SetLocalPosition({ half_width, half_height });
		versus_go->GetTransform()->SetLocalPosition({ half_width + margin + btn_size.x, half_height });

		scene.Add(single_go);
		scene.Add(coop_go);
		scene.Add(versus_go);
	}

	void PacGameManager::LoadLevel(int idx)
	{
		m_CurrentLevelIdx = idx;

		const std::string scene_name{ std::to_string(idx) };
		auto& scene_manager = SceneManager::Get();
		bool scene_exists = scene_manager.HasScene(scene_name);

		if (scene_exists)
		{
			scene_manager.SetActiveScene(scene_name);
			return;
		}

		auto& scene = scene_manager.CreateScene(scene_name);
		scene_manager.SetActiveScene(scene_name);

		const auto& map = m_GameData.maps[idx];

		//*************
		// Create level
		auto level_go = std::make_shared<GameObject>();
		auto level_grid = level_go->AddComponent<PacGrid>(map);

		PacData::PacLevelData levelData{};
		levelData.map = map;
		auto level = level_go->AddComponent<PacLevel>(levelData, level_grid);

		auto pacman_go_default = AddPlayer(level, scene);
		AddSpawner(level, scene);

		switch (m_GameData.mode)
		{
		case PacData::PacGameMode::COOP:
			AddPlayer(level, scene);
			break;

		case PacData::PacGameMode::VERSUS:
			break;
		}

		scene.Add(level_go);

		scene.Start();
	}

	void PacGameManager::LoadNextLevel(bool unloadCurrent)
	{
		// unload last level
		if (unloadCurrent) SceneManager::Get().RemoveScene(std::to_string(m_CurrentLevelIdx));
		if (m_CurrentLevelIdx >= m_GameData.maps.size() - 1) return;

		LoadLevel(++m_CurrentLevelIdx);
	}

	std::shared_ptr<GameObject> PacGameManager::AddPlayer(std::shared_ptr<PacLevel> level, Scene& scene)
	{
		//**************
		// Create pacman
		auto pacman_go = std::make_shared<GameObject>();
		pacman_go->SetTag("PacMan");
		pacman_go->AddComponent<RenderComponent>()->SetTexture(PacData::PacFiles::PacMan);

		// Add components
		auto pac_navigator = pacman_go->AddComponent<PacNavigator>(level->GetPacGrid());
		auto pac_controller = pacman_go->AddComponent<PacController>(pac_navigator);
		auto pac_score = pacman_go->AddComponent<PacScoreComponent>(0);
		auto pac_health = pacman_go->AddComponent<PacHealthComponent>(3);
		pacman_go->AddComponent<CircleCollider>(static_cast<float>(level->GetGrid()->GetCellSize() * .5f));

		// Tell pacman what to do when he arrives at a node
		std::weak_ptr weak_score = pac_score;
		std::weak_ptr weak_controller = pac_controller;
		pac_navigator->OnArriveAtTarget.AddFunction([weak_score, weak_controller](int idx, std::shared_ptr<PacGrid> grid)
			{
				// Collect Items
				const auto& info = grid->GetPacNodeInfo(idx);
				if (!info.hasItem) return;

				auto weak_score_locked = weak_score.lock();
				auto weak_controller_locked = weak_controller.lock();

				switch (info.type)
				{
				case PacData::PacNodeType::DOT:
					if (weak_score_locked) weak_score_locked->AddScore(PacData::PacDotScore);
					grid->SetPacNodeInfo(idx, PacData::PacNodeType::DOT, false);
					break;

				case PacData::PacNodeType::POWERUP:
					if (weak_score_locked) weak_score_locked->AddScore(PacData::PacPowerUpScore);
					grid->SetPacNodeInfo(idx, PacData::PacNodeType::POWERUP, false);
					if (weak_controller_locked) weak_controller_locked->PowerUp(5.f);

					break;
				default:
					break;
				}
			}, { weak_score, weak_controller });


		// Create HUD
		const float margin = g_WindowSize.x * .02f;
		auto score_hud = std::make_shared<GameObject>();
		score_hud->AddComponent<PacScoreHUD>(pac_score);
		score_hud->GetTransform()->SetLocalPosition(margin, margin);

		auto health_hud = std::make_shared<GameObject>();
		health_hud->AddComponent<PacHealthHUD>(pac_health);
		health_hud->GetTransform()->SetLocalPosition(margin, margin + 50.f);

		auto gameover_hud = std::make_shared<GameObject>();
		auto gameover_text = gameover_hud->AddComponent<TextComponent>("GAME OVER", ResourceManager::Get().LoadFont(PacData::PacFiles::PacFont, 36));
		const auto& gameover_text_size = gameover_text->GetSize();
		gameover_hud->GetTransform()->SetLocalPosition(g_WindowSize.x * .5f - (gameover_text_size.x * .5f), g_WindowSize.y * .5f - (gameover_text_size.x * .5f));
		gameover_hud->SetActive(false);

		std::weak_ptr weak_pacman = pacman_go;
		pac_health->OnDeath.AddFunction([gameover_hud, weak_pacman]() {

			if (auto weak_pacman_locked = weak_pacman.lock())
			{
				gameover_hud->SetActive(true);
				weak_pacman_locked->SetActive(false);
			}}, { weak_pacman });

		m_pPlayers.emplace_back(pacman_go);

		scene.Add(pacman_go);
		scene.Add(score_hud);
		scene.Add(health_hud);
		scene.Add(gameover_hud);

		return pacman_go;
	}

	std::shared_ptr<PacSpawner> PacGameManager::AddSpawner(std::shared_ptr<PacLevel> level, Scene& scene)
	{
		// Spawner initializes ghosts	
		auto spawner_go = std::make_shared<GameObject>();
		auto spawner = spawner_go->AddComponent<PacSpawner>(level);
		spawner->Initialize();

		// Set up ghost events
		auto ghosts = spawner_go->GetComponent<PacSpawner>()->GetNPCs();
		ghosts[static_cast<int>(PacData::PacGhosts::BLINKY)]
			->GetComponent<PacNPC>()->SetTarget(
				m_pPlayers[MathHelpers::GenerateRandomRange(0, static_cast<int>(m_pPlayers.size()) - 1)]); // do this here instead of injecting a dependency in the spawner

		for (auto pGhost : ghosts)
		{
			std::weak_ptr weak_ghost = pGhost;
			if (auto weak_ghost_locked = weak_ghost.lock())
			{
				for (auto pPlayer : m_pPlayers)
				{
					auto pac_controller = pPlayer->GetComponent<PacController>();

					pac_controller->OnPowerup.AddFunction([weak_ghost](float duration) {
						if (auto weak_ghost_locked = weak_ghost.lock()) {
							weak_ghost_locked->GetComponent<PacNPC>()->SetFrightened(duration);
						}}, { weak_ghost });
				}
			}
		}

		scene.Add(spawner_go);

		return spawner;
	}

}