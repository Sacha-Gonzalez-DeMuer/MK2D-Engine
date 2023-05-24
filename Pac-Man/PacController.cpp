#include "PacController.h"
#include "PacFilePaths.h"

namespace dae
{
	PacController::PacController(std::shared_ptr<GridNavComponent> pNavigator, const float speed)
		: PlayerController(pNavigator, speed)
	{
	}

	void PacController::Update()
	{
	
	}
}

