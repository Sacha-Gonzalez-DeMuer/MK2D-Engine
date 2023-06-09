#include "PacScoreHUD.h"
#include "PacScoreComponent.h"
#include "TextComponent.h"
// constructor
#include <memory>
#include "ResourceManager.h"

namespace dae
{
	PacScoreHUD::PacScoreHUD(std::shared_ptr<PacScoreComponent> scoreComponent)
		: m_scoreComponent{ scoreComponent }, m_scoreText{ nullptr }
	{
		m_scoreComponent->OnScoreChanged.AddFunction([this]() { UpdateText(); });
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
	}

}