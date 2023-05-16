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
		//bool isMoveSoundPlaying = ServiceLocator::GetSoundSystem().IsSoundPlaying(0);

		//if (m_pNavigator->IsMoving() && !isMoveSoundPlaying)
		//	ServiceLocator::GetSoundSystem().PlaySound(0, 100, -1);
		//else if (!m_pNavigator->IsMoving() && isMoveSoundPlaying)
		//	ServiceLocator::GetSoundSystem().StopSound(0);
	}
}

