#pragma once
#include "Subject.h"
#include "Component.h"
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

		
	private:
		std::shared_ptr<PacNavigator> m_pNavigator;
		void InitializeInput();
	};
}
