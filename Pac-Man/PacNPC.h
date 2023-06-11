#pragma once
#include "Component.h"
#include "Delegate.h"
#include <memory>
#include <vector>

namespace dae
{
	class PacNavigator;
	class PacNPCState;
	//class PacNPCState;
	class PacNPC final : public Component
	{
		using Target = std::shared_ptr<GameObject>;
		using TargetVector = std::vector<Target>;

	public:
		explicit PacNPC(std::shared_ptr<PacNavigator> pNavigator);
		virtual ~PacNPC() {};

		virtual void Update() override;

		void SetDefaultState(std::shared_ptr<PacNPCState> state, bool apply = true);
		void SetState(std::shared_ptr<PacNPCState> state, bool onExit = true);
		void ResetState(bool onExit = true);

		void SetTarget(std::shared_ptr<GameObject> target);
		void AddTarget(std::shared_ptr<GameObject> target);
		Target GetTarget() const { return m_pTarget; }
		std::shared_ptr<PacNavigator> GetNavigator() const { return m_pNavigator; }
		void SetFrightened(float duration);

		Delegate<> OnNPCDeath;

		std::shared_ptr<PacNPCState> GetState() const { return m_State; };
	private:
		std::shared_ptr<PacNPCState> m_State;
		std::shared_ptr<PacNPCState> m_DefaultState;
		std::shared_ptr<PacNavigator> m_pNavigator;

		Target m_pTarget;
		TargetVector m_pTargets;

		void OnCollisionEnter(ICollider& other) override;
	};
}