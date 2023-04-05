#pragma once
#include "Component.h"
#include "IObserver.h"

namespace dae
{
	class TextObject;
	class HealthComponent;

	class HealthHUD final : public Component, public IObserver<HealthComponent>
	{
	public:
		HealthHUD(std::shared_ptr<TextObject> healthText, std::shared_ptr<TextObject> livesText, std::shared_ptr<HealthComponent> health);

		virtual ~HealthHUD() = default;
		HealthHUD(const HealthHUD& other) = delete;
		HealthHUD(HealthHUD&& other) = delete;
		HealthHUD& operator=(const HealthHUD& other) = delete;
		HealthHUD& operator=(HealthHUD&& other) = delete;

		void Render() const override;
		void Notify(const HealthComponent& healthComponent) override;

	private:
		std::shared_ptr<HealthComponent> m_health;
		std::shared_ptr<TextObject> m_healthText;
		std::shared_ptr<TextObject> m_livesText;
	};
}

