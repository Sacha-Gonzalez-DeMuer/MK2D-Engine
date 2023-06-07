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
	private:
		std::shared_ptr<PacNavigator> m_pNavigator;

		enum class PacNPCState
		{
			Chase,
			Wander
		};

		PacNPCState m_State;


		//std::shared_ptr<PacNPCState> m_pState;
	};
}
