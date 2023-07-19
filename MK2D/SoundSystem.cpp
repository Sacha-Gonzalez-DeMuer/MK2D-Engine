#include "SoundSystem.h"
#include <map>
#include <queue>
#include <SDL_mixer.h>
#include <stdexcept>
#include <mutex>
#include <thread>

namespace dae
{
	class SDL_SoundSystem::SDL_SoundSysImpl final
	{
		struct PlayRequest
		{
			float volume;
			int loops;
			sound_id soundId;

			PlayRequest(const sound_id soundId, const float volume, const int loops = 0)
				: volume{ volume }
				, loops{ loops }
				, soundId{ soundId }
			{}
		};

	public:
		SDL_SoundSysImpl()
			: m_Sounds{}
			, m_Music{}
		{
			m_Running = true;
			m_SoundThread = std::jthread(&SDL_SoundSysImpl::UpdateQueue, this);

			Mix_Init(MIX_INIT_MP3);
			Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 8, 2048);
		}

		~SDL_SoundSysImpl()
		{
			for (auto& sound : m_Sounds)
				Mix_FreeChunk(sound.second);
			for (auto& music : m_Music)
				Mix_FreeMusic(music.second);
			Mix_CloseAudio();
			Mix_Quit();

			m_Running = false;
			m_qCondtionVariable.notify_all();
			m_SoundThread.join();
		}

		void PlaySound(const PlayRequest& request)
		{
			PlaySound(request.soundId, request.volume, request.loops);
		}

		void PlaySound(const sound_id soundId, const float volume, const int loops)
		{
			m_Pending.push({ soundId, volume, loops });
			m_qCondtionVariable.notify_one();
		}

		void StopSound(const sound_id soundId)
		{
			Mix_Chunk* sound = GetSound(soundId);
			if (sound == nullptr) return;

			int numChannels = Mix_AllocateChannels(-1);  // Get the total number of available channels
			for (int channel = 0; channel < numChannels; ++channel) {
				if (Mix_GetChunk(channel) == sound) {
					Mix_HaltChannel(channel);
					return;
				}
			}
		}

		void PlayMusic(const sound_id musicId, const float volume, const int loops)
		{
			Mix_Music* music = GetMusic(musicId);
			if (music != nullptr) {
				Mix_VolumeMusic(static_cast<Uint8>(volume));
				Mix_PlayMusic(music, loops);
			}
		}

		void MuteAllSounds()
		{
			int numChannels = Mix_AllocateChannels(-1);
			for (int channel = 0; channel < numChannels; ++channel)
			{
				m_PreviousSoundVolumes[channel] = Mix_Volume(channel, -1);
				Mix_Volume(channel, 0);
			}
		}

		void UnmuteAllSounds()
		{
			for (const auto& [channel, volume] : m_PreviousSoundVolumes)
			{
				Mix_Volume(channel, volume);
			}
		}

		void MuteAllMusic()
		{
			m_PreviousMusicVolume = Mix_VolumeMusic(-1);
			Mix_VolumeMusic(0);
		}

		void UnmuteAllMusic()
		{
			Mix_VolumeMusic(m_PreviousMusicVolume);
		}

		void Mute()
		{
			MuteAllSounds();
			MuteAllMusic();
		}

		void Unmute()
		{
			UnmuteAllSounds();
			UnmuteAllMusic();
		}


		void StopAllSounds()
		{
			Mix_HaltChannel(-1);
		}

		void StopAllMusic()
		{
			Mix_HaltMusic();
		}

		void PauseAllSounds()
		{
			Mix_Pause(-1);
		}

		void PauseAllMusic()
		{
			Mix_PauseMusic();
		}

		void ResumeAllSounds()
		{
			Mix_Resume(-1);
		}

		void ResumeAllMusic()
		{
			Mix_ResumeMusic();
		}

		void AddSound(const std::string& filePath, const sound_id soundId)
		{
			std::unique_lock<std::mutex> lock(m_qMutex);

			if (auto sound = Mix_LoadWAV(filePath.c_str()))
				m_Sounds[soundId] = sound;
			else
				throw std::runtime_error("Failed to load sound: " + filePath);
		}

		bool IsSoundPlaying(const sound_id soundId)
		{
			std::lock_guard<std::mutex> lock(m_qMutex);

			Mix_Chunk* sound = GetSound(soundId);
			if (sound == nullptr) return false;

			int numChannels = Mix_AllocateChannels(-1);  // Get the total number of available channels
			for (int channel = 0; channel < numChannels; ++channel)
				if (Mix_Playing(channel) && Mix_GetChunk(channel) == sound)
					return true;  // Sound is playing on this channel

			return false;
		}

	private:
		std::map<sound_id, Mix_Chunk*> m_Sounds;
		std::map<sound_id, Mix_Music*> m_Music;

		std::queue<PlayRequest> m_Pending;

		std::mutex m_qMutex;
		std::condition_variable m_qCondtionVariable;
		std::jthread m_SoundThread;
		bool m_Running;

		std::map<int, int> m_PreviousSoundVolumes;
		int m_PreviousMusicVolume;

		void UpdateQueue()
		{
			while (m_Running)
			{
				std::unique_lock<std::mutex> lock(m_qMutex);
				m_qCondtionVariable.wait(lock, [&]() { return !m_Pending.empty() || !m_Running; });

				if (!m_Pending.empty())
				{
					PlayRequest request = m_Pending.front();
					Dequeue(request);
				}
			}
		}

		Mix_Chunk* GetSound(const sound_id soundId) const
		{
			if (m_Sounds.count(soundId) > 0) {
				return m_Sounds.at(soundId);
			}
			return nullptr;
		}

		Mix_Music* GetMusic(const sound_id musicId) const
		{
			if (m_Music.count(musicId) > 0) {
				return m_Music.at(musicId);
			}
			return nullptr;
		}

		inline void Dequeue(const PlayRequest& request)
		{
			Mix_Chunk* sound = GetSound(request.soundId);
			if (sound != nullptr) {
				sound->volume = static_cast<Uint8>(request.volume);
				Mix_PlayChannel(-1, sound, request.loops);
			};

			m_Pending.pop();
		}
	};

#pragma region SDl_pImpl
	SDL_SoundSystem::SDL_SoundSystem()
		: m_pImpl{ new SDL_SoundSysImpl() }
	{}

	SDL_SoundSystem::~SDL_SoundSystem()
	{
		delete m_pImpl;
		m_pImpl = nullptr;
	}

	void SDL_SoundSystem::AddSound(const std::string& filePath, const sound_id soundId)
	{
		m_pImpl->AddSound(filePath, soundId);
	}

	void SDL_SoundSystem::PlaySound(const sound_id soundId, const float volume, const int loops)
	{
		m_pImpl->PlaySound(soundId, volume, loops);
	}

	void SDL_SoundSystem::StopSound(const sound_id soundId)
	{
		m_pImpl->StopSound(soundId);
	}

	void SDL_SoundSystem::PlayMusic(const sound_id musicId, const float volume, const int loops)
	{
		m_pImpl->PlayMusic(musicId, volume, loops);
	}
	void SDL_SoundSystem::StopAllSounds()
	{
		m_pImpl->StopAllSounds();
	}
	void SDL_SoundSystem::StopAllMusic()
	{
		m_pImpl->StopAllMusic();
	}
	void SDL_SoundSystem::PauseAllSounds()
	{
		m_pImpl->PauseAllSounds();
	}
	void SDL_SoundSystem::PauseAllMusic()
	{
		m_pImpl->PauseAllMusic();
	}
	void SDL_SoundSystem::ResumeAllSounds()
	{
		m_pImpl->ResumeAllSounds();
	}
	void SDL_SoundSystem::ResumeAllMusic()
	{
		m_pImpl->ResumeAllMusic();
	}
	bool SDL_SoundSystem::IsSoundPlaying(const sound_id soundId) const
	{
		return m_pImpl->IsSoundPlaying(soundId);
	}
	void SDL_SoundSystem::MuteAllSounds()
	{
		m_pImpl->MuteAllSounds();
	}
	void SDL_SoundSystem::MuteAllMusic()
	{
		m_pImpl->MuteAllMusic();
	}
	void SDL_SoundSystem::UnmuteAllMusic()
	{
		m_pImpl->UnmuteAllSounds();
	}
	void SDL_SoundSystem::UnmuteAllSounds()
	{
		m_pImpl->UnmuteAllSounds();
	}
	void SDL_SoundSystem::Mute()
	{
		m_pImpl->Mute();
	}
	void SDL_SoundSystem::Unmute()
	{
		m_pImpl->Unmute();
	}
#pragma endregion
}