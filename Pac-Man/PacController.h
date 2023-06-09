#pragma once
#include "Subject.h"
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
	class PacController : public Component, public Subject<PacController>
	{
	public:
		PacController(std::shared_ptr<PacNavigator> pNavigator);
		~PacController() = default;

		Delegate<> OnPlayerDeath;

		void PowerUp(float duration);
		void Update() override;

	private:
		std::shared_ptr<PacNavigator> m_pNavigator;
		void InitializeInput();
		void OnCollision(ICollider& other) override;

		float m_PowerUpTimer;
	};
}
