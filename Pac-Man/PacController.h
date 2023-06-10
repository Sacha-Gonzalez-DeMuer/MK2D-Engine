#pragma once
#include "Component.h"
#include "Delegate.h"

namespace dae
{
	enum class PacEvent
	{
		Death,
		Win
	};

	class PacNavigator;
	class PacController : public Component
	{
	public:
		PacController(std::shared_ptr<PacNavigator> pNavigator);
		~PacController() = default;

		Delegate<float> OnPowerup;

		void PowerUp(float duration);
		void Update() override;

	private:
		std::shared_ptr<PacNavigator> m_pNavigator;
		void InitializeInput();

		float m_PowerUpTimer;
	};
}
