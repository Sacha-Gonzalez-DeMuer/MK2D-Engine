#include "PacHealthHUD.h"
#include "TextObject.h"
#include "PacHealthComponent.h"
#include "Minigin.h"
#include "ResourceManager.h"
#include <memory>

namespace dae
{
	PacHealthHUD::PacHealthHUD(std::shared_ptr<PacHealthComponent> health)
		: m_healthComponent{ health }
		, m_livesText{ nullptr }
	{
	}

	void PacHealthHUD::Update()
	{
		m_livesText->Update();
	}

	void PacHealthHUD::Render() const
	{
		m_livesText->Render();
	}

	void PacHealthHUD::UpdateHUD() const
	{
		m_livesText->SetText("Lives: " + std::to_string(m_healthComponent->GetRemainingLives()));
	}

	void PacHealthHUD::Start()
	{
		m_healthComponent->OnHitTaken.AddFunction([this]() { UpdateHUD(); });
		m_healthComponent->OnDeath.AddFunction([this]() { UpdateHUD(); });

		auto font = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
		m_livesText = GetOwner()->AddComponent<TextComponent>("Lives: " + std::to_string(m_healthComponent->GetRemainingLives()), font);
	}
}


