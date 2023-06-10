#include "PacScoreHUD.h"
#include "PacScoreComponent.h"
#include "TextObject.h"
#include <memory>
#include "ResourceManager.h"
#include "PacData.h"
namespace dae
{
	PacScoreHUD::PacScoreHUD(std::shared_ptr<PacScoreComponent> scoreComponent)
		: m_scoreComponent{ scoreComponent }, m_scoreText{ nullptr }
	{
	}

	void dae::PacScoreHUD::Update()
	{
		m_scoreText->Update();
	}

	void dae::PacScoreHUD::Render() const
	{
		m_scoreText->Render();
	}

	void PacScoreHUD::UpdateText() const
	{
		m_scoreText->SetText("Score: " + std::to_string(m_scoreComponent->GetScore()));
	}

	void dae::PacScoreHUD::Start()
	{
		m_scoreComponent->OnScoreChanged.AddFunction([this]() { UpdateText(); });

		auto font = ResourceManager::Get().LoadFont(PacData::PacFont, 36);
		m_scoreText = GetOwner()->AddComponent<TextComponent>("Score: " + std::to_string(m_scoreComponent->GetScore()), font);
	} 
}