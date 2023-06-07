#pragma once
#include "Component.h"
#include <memory>

namespace dae
{
	class PacNavigator;
	//class PacNPCState;
	class PacNPC final : public Component
	{
	public:
		explicit PacNPC(std::shared_ptr<PacNavigator> pNavigator);
		virtual ~PacNPC() = default;

		virtual void Update() override;

		void SetTarget(std::shared_ptr<GameObject> target);
	private:
		std::shared_ptr<PacNavigator> m_pNavigator;
		std::shared_ptr<GameObject> m_pTarget;

		enum class PacNPCState
		{
			Chase,
			Wander
		};

		PacNPCState m_State;
		void OnArrive();

		void ChaseLogic();
		void WanderLogic();
	};
}
