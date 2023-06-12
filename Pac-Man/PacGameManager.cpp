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
#include <fstream>
#include <memory>
#include "SceneManager.h"
#include "TextObject.h"
#include "PacSpawner.h"
#include "Button.h"
#include "MathHelpers.h"
#include "PacImpostorState.h"
#include "PacImpostorVulnerableState.h"
#include "ServiceLocator.h"
#include "Toggle.h"

namespace dae
{

	void PacGameManager::Initialize(const PacData::PacGameData& gameData)
	{
		m_GameData = gameData;

		const auto& dataPath = ResourceManager::Get().GetDataPath();

		ServiceLocator::RegisterSoundSystem(std::make_unique<SDL_SoundSystem>());

		ServiceLocator::GetSoundSystem().AddSound(dataPath + PacData::PacFiles::WAKAWAKAWAKA,
			static_cast<int>(PacData::PacSound::WAKA));

		ServiceLocator::GetSoundSystem().AddSound(dataPath + PacData::PacFiles::PacDeathSound,
			static_cast<int>(PacData::PacSound::DEATH));

		ServiceLocator::GetSoundSystem().AddSound(dataPath + PacData::PacFiles::PacEatSound,
			static_cast<int>(PacData::PacSound::EAT));

		ServiceLocator::GetSoundSystem().AddSound(dataPath + PacData::PacFiles::PacIntro,
			static_cast<int>(PacData::PacSound::INTRO));

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
			{ m_GameData.mode = PacData::PacGameMode::SINGLE; LoadLevel(0); 
		ServiceLocator::GetSoundSystem().PlaySound(static_cast<int>(PacData::PacSound::INTRO), 50); });

		coop_button->OnClick.AddFunction([this]()
			{ m_GameData.mode = PacData::PacGameMode::COOP; LoadLevel(0);		
		ServiceLocator::GetSoundSystem().PlaySound(static_cast<int>(PacData::PacSound::INTRO), 50); });

		versus_button->OnClick.AddFunction([this]()
			{ m_GameData.mode = PacData::PacGameMode::VERSUS; LoadLevel(0); 		
		ServiceLocator::GetSoundSystem().PlaySound(static_cast<int>(PacData::PacSound::INTRO), 50); });

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
		scene.Add(level_go);

		PacData::PacLevelData levelData{};
		levelData.map = map;
		auto level = level_go->AddComponent<PacLevel>(levelData, level_grid);

		level->OnLevelCompleted.AddFunction([this]() {
			LoadNextLevel();
			
			});

		//*************
		// Create HUD
		auto gameover_hud = std::make_shared<GameObject>();
		auto gameover_text = gameover_hud->AddComponent<TextComponent>("GAME OVER", ResourceManager::Get().LoadFont(PacData::PacFiles::PacFont, 36));
		const auto& gameover_text_size = gameover_text->GetSize();
		gameover_hud->GetTransform()->SetLocalPosition(g_WindowSize.x * .5f - (gameover_text_size.x * .5f), g_WindowSize.y * .5f - (gameover_text_size.x * .5f));
		gameover_hud->SetActive(false); 
		scene.Add(gameover_hud);

		auto mute_toggle = std::make_shared<GameObject>();
		auto mute_toggle_comp = mute_toggle->AddComponent<Toggle>(PacData::PacFiles::MutedBtn, PacData::PacFiles::MuteBtn);
		mute_toggle_comp->SetSize({ 50.f, 50.f });
		mute_toggle->GetTransform()->SetLocalPosition({ 50.f, g_WindowSize.y - 150.f });

		std::weak_ptr weak_mute_toggle{ mute_toggle_comp };
		mute_toggle_comp->OnClick.AddFunction([weak_mute_toggle]() {
			if (auto mute_toggle_locked = weak_mute_toggle.lock(); mute_toggle_locked)
			{
				mute_toggle_locked->IsToggled()
					? ServiceLocator::GetSoundSystem().Mute()
					: ServiceLocator::GetSoundSystem().Unmute();
			}}, { weak_mute_toggle });
			
		scene.Add(mute_toggle);
			

		//***************
		// Create players
		auto pacman_go_default = AddPlayer(scene, level, gameover_hud);

		AddSpawner(scene, level);
		switch (m_GameData.mode)
		{
		case PacData::PacGameMode::COOP:
			AddPlayer(scene, level, gameover_hud);
			break;

		case PacData::PacGameMode::VERSUS:
			AddImpostor(scene, level);
			break;
		}

		scene.Start();
	}

	void PacGameManager::LoadNextLevel(bool unloadCurrent)
	{
		if (unloadCurrent) SceneManager::Get().RemoveScene(std::to_string(m_CurrentLevelIdx));
		if (m_CurrentLevelIdx >= static_cast<int>(m_GameData.maps.size()) - 1) return;

		m_pPlayers.clear();

		LoadLevel(++m_CurrentLevelIdx);
	}

	std::shared_ptr<GameObject> PacGameManager::AddPlayer(Scene& scene, std::shared_ptr<PacLevel> level, std::shared_ptr<GameObject> gameoverHUD)
	{
		//**************
		// Create pacman
		auto pacman_go = std::make_shared<GameObject>();
		pacman_go->SetTag("PacMan");
		pacman_go->AddComponent<RenderComponent>()->SetTexture(PacData::PacFiles::PacMan);

		// Add components
		auto pac_navigator = pacman_go->AddComponent<PacNavigator>(level->GetPacGrid());
		auto pac_controller = pacman_go->AddComponent<PacController>(pac_navigator, static_cast<int>(m_pPlayers.size()));
		auto pac_score = pacman_go->AddComponent<PacScoreComponent>(0);
		auto pac_health = pacman_go->AddComponent<PacHealthComponent>(3);
		pacman_go->AddComponent<CircleCollider>(static_cast<float>(level->GetGrid()->GetCellSize() * .5f));


		std::weak_ptr weak_navigator = pac_navigator;
		pac_health->OnDeath.AddFunction([weak_navigator]()
			{
				if (auto weak_navigator_locked = weak_navigator.lock(); weak_navigator_locked)
				{
					weak_navigator_locked->SetCurrentNode(weak_navigator_locked->GetSpawnNode());
				}
			}, { weak_navigator });

		// Tell pacman what to do when he arrives at a node
		std::weak_ptr weak_score = pac_score;
		std::weak_ptr weak_controller = pac_controller;
		std::weak_ptr weak_level = level;
		pac_navigator->OnArriveAtTarget.AddFunction([weak_score, weak_controller, weak_level](int idx, std::shared_ptr<PacGrid> grid)
			{
				// Collect Items
				const auto& info = grid->GetPacNodeInfo(idx);
				if (!info.hasItem) return;

				auto weak_score_locked = weak_score.lock();
				auto weak_controller_locked = weak_controller.lock();
				auto weak_level_locked = weak_level.lock();

				switch (info.type)
				{
				case PacData::PacNodeType::DOT:
					if (weak_score_locked) weak_score_locked->AddScore(PacData::PacDotScore);
					grid->SetPacNodeInfo(idx, PacData::PacNodeType::DOT, false);
					if (weak_level_locked) weak_level_locked->RemoveDot();
					break;

				case PacData::PacNodeType::POWERUP:
					if (weak_score_locked) weak_score_locked->AddScore(PacData::PacPowerUpScore);
					grid->SetPacNodeInfo(idx, PacData::PacNodeType::POWERUP, false);
					if (weak_controller_locked) weak_controller_locked->PowerUp(5.f);
					if (weak_level_locked) weak_level_locked->RemovePowerup();
					break;
				default:
					break;
				}

			}, { weak_score, weak_controller });


		// Sound
		pac_health->OnHitTaken.AddFunction([]()
			{
				ServiceLocator::GetSoundSystem().PlaySound(static_cast<int>(PacData::PacSound::DEATH), 50);
			});


		scene.Add(pacman_go);
		m_pPlayers.emplace_back(pacman_go);

		if(!gameoverHUD) return pacman_go;

		// Create HUD
		const float margin = g_WindowSize.x * .02f;
		const float offset = 100.f;

		auto player_hud = std::make_shared<GameObject>();

		auto score_hud = std::make_shared<GameObject>();
		score_hud->AddComponent<PacScoreHUD>(pac_score);
		score_hud->GetTransform()->SetLocalPosition(margin, margin);

		auto health_hud = std::make_shared<GameObject>();
		health_hud->AddComponent<PacHealthHUD>(pac_health);
		health_hud->GetTransform()->SetLocalPosition(margin, margin + 50.f);

		player_hud->AddChild(score_hud);
		player_hud->AddChild(health_hud);
		player_hud->GetTransform()->SetLocalPosition(margin, margin + (offset * m_pPlayers.size()));

		std::weak_ptr weak_pacman = pacman_go;
		if (gameoverHUD)
		{
			pac_health->OnDeath.AddFunction([gameoverHUD, weak_pacman]() {

				if (auto weak_pacman_locked = weak_pacman.lock())
				{
					gameoverHUD->SetActive(true);
					weak_pacman_locked->SetActive(false);
				}}, { weak_pacman });
		}
		pac_health->OnHitTaken.AddFunction([weak_pacman]() {
			if (auto weak_pacman_locked = weak_pacman.lock())
			{
				weak_pacman_locked->GetComponent<PacNavigator>()->SetSpawn(-1);
			}
			}, { weak_pacman });


		scene.Add(score_hud);
		scene.Add(health_hud);

		return pacman_go;
	}

	std::shared_ptr<GameObject> PacGameManager::AddImpostor(Scene& scene, std::shared_ptr<PacLevel> level)
	{
		AddPlayer(scene, level, nullptr);
		auto impostor_player = m_pPlayers[1];
		auto impostor_renderer = impostor_player->GetComponent<RenderComponent>();
		impostor_renderer->SetTexture(PacData::PacFiles::ImpostorGhost);

		float size = static_cast<float>(level->GetGrid()->GetCellSize());
		impostor_renderer->SetSize({ size, size });
		impostor_player->SetTag("Ghost");

		auto impostor_navigator = impostor_player->GetComponent<PacNavigator>();
		impostor_navigator->SetSpawn(level->GetPacGrid()->GetNPCSpawnIdxs()[0], true);

		auto astar_pathfinder = std::make_shared<AStarPathFinder>(level->GetGrid());
		impostor_navigator->SetPathFinder(astar_pathfinder);

		auto impostor_npc = impostor_player->AddComponent<PacNPC>(impostor_navigator);
		impostor_npc->SetState(std::make_shared<PacImpostorState>(nullptr));
		impostor_npc->SetDefaultState(std::make_shared<PacImpostorState>(nullptr));

		std::weak_ptr weak_impostor_npc = impostor_npc;
		m_pPlayers[0]->GetComponent<PacController>()->OnPowerup.AddFunction(
			[weak_impostor_npc](float time) {
			if (auto weak_impostor_locked = weak_impostor_npc.lock(); weak_impostor_locked)
			{
				auto gameobject = weak_impostor_locked->GetOwner();
				weak_impostor_locked->SetState(std::make_shared<PacImpostorVulnerableState>(gameobject, time));
			}}, { weak_impostor_npc });

		std::weak_ptr weak_brain = impostor_npc;
		impostor_navigator->OnArriveAtTarget.AddFunction([weak_brain](int, std::shared_ptr<PacGrid>) {
			if (auto ghost_brain_locked = weak_brain.lock()) {
				weak_brain.lock()->GetState()->OnArrive(*weak_brain.lock());
			}}, { weak_brain });


		// Sound
		impostor_npc->OnNPCDeath.AddFunction([]()
			{
				ServiceLocator::GetSoundSystem().PlaySound(static_cast<int>(PacData::PacSound::EAT), 50);
			});

		return impostor_player;
	}

	std::shared_ptr<PacSpawner> PacGameManager::AddSpawner(Scene& scene, std::shared_ptr<PacLevel> level)
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

	void PacGameManager::SerializeHighscore(const PacData::PacPlayerInfo& playerInfo)
	{
		const auto& dataPath = dae::ResourceManager::Get().GetDataPath();

		std::ofstream ofs{ dataPath + "Highscore.txt" };
		if (ofs.is_open())
		{
			ofs << playerInfo.name << " " << playerInfo.score;
		}

		ofs.write(reinterpret_cast<const char*>(&playerInfo), sizeof(PacData::PacPlayerInfo));

		ofs.close();
	}

	PacData::PacPlayerInfo PacGameManager::DeserializeHighscore()
	{
		const auto& dataPath = dae::ResourceManager::Get().GetDataPath();

		std::ifstream ifs{ dataPath + "Highscore.txt" };
		if (ifs.is_open())
		{
			PacData::PacPlayerInfo playerInfo{};
			ifs >> playerInfo.name >> playerInfo.score;
		}

		PacData::PacPlayerInfo playerInfo{};
		ifs.read(reinterpret_cast<char*>(&playerInfo), sizeof(PacData::PacPlayerInfo));

		ifs.close();

		return playerInfo;
	}

}