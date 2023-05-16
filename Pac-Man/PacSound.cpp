#pragma once
#include "PacSound.h"
//#include "ServiceLocator.h"

void dae::PacSound::Notify(PacController*, PacEvent)
{
	//ServiceLocator::GetSoundSystem().PlaySound(0, 100, -1);
}

void dae::PacSound::OnSubjectDestroy()
{
}
