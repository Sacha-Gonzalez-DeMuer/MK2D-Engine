#pragma once
#include "SoundSystem.h"
#include <memory>

namespace dae
{
	class ServiceLocator final 
	{
	public:
		static void RegisterSoundSystem(std::unique_ptr<ISoundSystem>&& pSoundSystem)
		{ m_pSoundSystem = std::move(pSoundSystem); }

		static ISoundSystem& GetSoundSystem()
		{ 	return *m_pSoundSystem; }


	private:
		static std::unique_ptr<ISoundSystem> m_pSoundSystem;
		static NULL_SoundSystem m_nullSoundSystem;
	};

	std::unique_ptr<ISoundSystem> ServiceLocator::m_pSoundSystem = nullptr;
}
