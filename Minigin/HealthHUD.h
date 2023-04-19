#pragma once
#include "Component.h"
#include "IObserver.h"

namespace dae
{
	class TextComponent;
	class HealthComponent;
	class Font;

	class HealthHUD final 
		: public Component
		, public IObserver<HealthComponent>
		, public  std::enable_shared_from_this<HealthHUD>
	{
	public:
		HealthHUD(std::shared_ptr<HealthComponent> health
			, std::shared_ptr<TextComponent> m_livesText);

		virtual ~HealthHUD() = default;
		HealthHUD(const HealthHUD& other) = delete;
		HealthHUD(HealthHUD&& other) = delete;
		HealthHUD& operator=(const HealthHUD& other) = delete;
		HealthHUD& operator=(HealthHUD&& other) = delete;

		void Update() override;
		void Render() const override;
		void Notify(const HealthComponent& healthComponent) override;

	private:
		std::shared_ptr<HealthComponent> m_healthComponent;
		std::shared_ptr<TextComponent> m_livesText;

		void Start() override;
	};
}

