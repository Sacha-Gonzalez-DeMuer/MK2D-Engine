#pragma once
#include <memory>
#include <string>

namespace dae
{
	using sound_id = unsigned short;
	class ISoundSystem
	{
	public:
		ISoundSystem() = default;
		virtual ~ISoundSystem() = default;
		ISoundSystem(const ISoundSystem& other) = delete;
		ISoundSystem(ISoundSystem&& other) = delete;
		ISoundSystem& operator=(const ISoundSystem& other) = delete;
		ISoundSystem& operator=(ISoundSystem&& other) = delete;

		// member functions
		virtual void AddSound(const std::string& filePath, const sound_id soundId) = 0;
		virtual void PlaySound(const sound_id soundId, const float volume, const int loops = 0) = 0;
		virtual void StopSound(const sound_id soundId) = 0;
		virtual void PlayMusic(const sound_id musicId, const float volume, const int loops = 0) = 0;
		virtual void StopAllSounds() = 0;
		virtual void StopAllMusic() = 0;
		virtual void PauseAllSounds() = 0;
		virtual void PauseAllMusic() = 0;
		virtual void ResumeAllSounds() = 0;
		virtual void ResumeAllMusic() = 0;
		virtual bool IsSoundPlaying(const sound_id soundId) const = 0;
	};

	class NULL_SoundSystem : public ISoundSystem
	{
		NULL_SoundSystem() = default;
	public:
		~NULL_SoundSystem() = default;
		//overrides
		void AddSound(const std::string&, const sound_id) override {};
		void PlaySound(const sound_id, const float, const int) override {};
		void StopSound(const sound_id) override {};
		void PlayMusic(const sound_id, const float, const int) override {};
		void StopAllSounds() override {};
		void StopAllMusic() override {};
		void PauseAllSounds() override {};
		void PauseAllMusic() override {};
		void ResumeAllSounds() override {};
		void ResumeAllMusic() override {};
		bool IsSoundPlaying(const sound_id) const override {};
	};

	class SDL_SoundSystem final : public ISoundSystem
	{
	public:
		SDL_SoundSystem();
		~SDL_SoundSystem();

		//overrides
		void AddSound(const std::string& filePath, const sound_id soundId) override;
		void PlaySound(const sound_id soundId, const float volume, const int loops = 0) override;
		void StopSound(const sound_id soundId) override;
		void PlayMusic(const sound_id musicId, const float volume, const int loops = 0) override;
		void StopAllSounds() override;
		void StopAllMusic() override;
		void PauseAllSounds() override;
		void PauseAllMusic() override;
		void ResumeAllSounds() override;
		void ResumeAllMusic() override;
		bool IsSoundPlaying(const sound_id soundId) const override;

	private:
		class SDL_SoundSysImpl;
		SDL_SoundSysImpl* m_pImpl;
	};
}