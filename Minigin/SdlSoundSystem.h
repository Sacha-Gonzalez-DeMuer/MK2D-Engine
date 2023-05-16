#pragma once
#include "SoundSystem.h"
class SdlSoundSystem final : public SoundSystem
{
public:
	SdlSoundSystem() = default;
	~SdlSoundSystem();

	void PlaySound(int soundId) override;
};

