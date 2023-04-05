#include "HealthHUD.h"
#include "TextObject.h"
#include "HealthComponent.h"

dae::HealthHUD::HealthHUD(std::shared_ptr<TextObject> healthText, std::shared_ptr<TextObject> livesText, std::shared_ptr<HealthComponent> health)
	: m_healthText(healthText)
	, m_livesText(livesText)
	, m_health(health)
{
	m_health->AddObserver(this);
}

void dae::HealthHUD::Render() const
{
	m_healthText->Render();
	m_livesText->Render();
}

void dae::HealthHUD::Notify(const HealthComponent& health)
{
	m_healthText->SetText(
	"Health: " + std::to_string(m_health->GetCurrentHealth()));

	m_livesText->SetText("Remaining lives: " + std::to_string(m_health->GetRemainingLives()));
}
