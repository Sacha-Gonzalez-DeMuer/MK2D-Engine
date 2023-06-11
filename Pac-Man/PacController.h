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
	class GenericController;
	class PacController : public Component
	{
	public:
		PacController(std::shared_ptr<PacNavigator> pNavigator, int playerIdx);
		~PacController() = default;

		Delegate<float> OnPowerup;

		void PowerUp(float duration);
		void Update() override;

		void SetGenericControllerActive(bool active);

	private:
		std::shared_ptr<GenericController> m_pGenericController;
		std::shared_ptr<PacNavigator> m_pNavigator;
		void InitializeInput(unsigned int playerIdx);

		int m_PlayerIdx;
		float m_PowerUpTimer;
	};
}
