#include "HealthHUD.h"
#include "TextObject.h"
#include "HealthComponent.h"
#include "Minigin.h"

namespace dae
{
	HealthHUD::HealthHUD(std::shared_ptr<HealthComponent> health
		, std::shared_ptr<TextComponent> livesText)
		: m_healthComponent{ health }
		, m_livesText{ livesText }
	{
	}

	void HealthHUD::Update()
	{
		m_livesText->Update();
	}

	void HealthHUD::Render() const
	{
		m_livesText->Render();
	}

	void HealthHUD::Notify(const HealthComponent& health)
	{
		m_livesText->SetText("Lives: " + std::to_string( health.GetRemainingLives()));
	}

	void HealthHUD::Start()
	{
		m_healthComponent->AddObserver(shared_from_this());
	}
}


