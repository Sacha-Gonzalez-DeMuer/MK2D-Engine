#pragma once
#include "PlayerController.h"
#include "GridNavComponent.h"
#include "Commands.h"
#include "Subject.h"
namespace dae
{
	enum class PacEvent
	{
		Death,
		Win
	};

	class PacController : public PlayerController, public Subject<PacController>
	{
	public:
		PacController(std::shared_ptr<GridNavComponent> pNavigator, const float speed);
		~PacController() = default;

		void Update() override;
	};
}
