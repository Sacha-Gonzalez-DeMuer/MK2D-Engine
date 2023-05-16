#pragma once
class SoundSystem
{
public:
	SoundSystem();
	~SoundSystem();
	SoundSystem(const SoundSystem& other) = delete;
	SoundSystem(SoundSystem&& other) = delete;
	SoundSystem& operator=(const SoundSystem& other) = delete;
	SoundSystem& operator=(SoundSystem&& other) = delete;

	// member functions
	virtual void PlaySound(int soundId);
	virtual void PlayMusic(int musicId);
	virtual void StopAllSounds();
	virtual void StopAllMusic();
	virtual void PauseAllSounds();
	virtual void PauseAllMusic();
	virtual void ResumeAllSounds();
	virtual void ResumeAllMusic();
};

