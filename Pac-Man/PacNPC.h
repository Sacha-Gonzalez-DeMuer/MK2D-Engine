#pragma once
#include "Component.h"
#include <memory>
#include "Delegate.h"
namespace dae
{
	class PacNavigator;
	class PacNPCState;
	//class PacNPCState;
	class PacNPC final : public Component
	{
	public:
		explicit PacNPC(std::shared_ptr<PacNavigator> pNavigator);
		virtual ~PacNPC() {};

		virtual void Update() override;

		void SetTarget(std::shared_ptr<GameObject> target);
		std::shared_ptr<GameObject> GetTarget() const { return m_pTarget; }
		std::shared_ptr<PacNavigator> GetNavigator() const { return m_pNavigator; }

		Delegate<> OnNPCDeath;

		std::shared_ptr<PacNPCState> GetState() const { return m_State; };


	private:
		std::shared_ptr<PacNPCState> m_State;
		std::shared_ptr<PacNavigator> m_pNavigator;
		std::shared_ptr<GameObject> m_pTarget;

		bool m_Vulnerable;
		void OnCollision(ICollider& other);
	};
}