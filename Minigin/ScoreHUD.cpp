#include "ScoreHUD.h"
#include "ScoreComponent.h"
#include "TextComponent.h"
// constructor

namespace dae
{

	dae::ScoreHUD::ScoreHUD(std::shared_ptr<ScoreComponent> scoreComponent, std::shared_ptr<TextComponent> m_scoreText)
		: m_scoreComponent{ scoreComponent }
		, m_scoreText{ m_scoreText }
	{
	}

	void dae::ScoreHUD::Update()
	{
		m_scoreText->Update();
	}

	void dae::ScoreHUD::Render() const
	{
		m_scoreText->Render();
	}

	void ScoreHUD::Notify(const int& /*score*/)
	{
		m_scoreText->SetText("Score: " + std::to_string(m_scoreComponent->GetScore()));
	}


	void dae::ScoreHUD::Start()
	{
		m_scoreComponent->AddObserver(shared_from_this());
	}

}