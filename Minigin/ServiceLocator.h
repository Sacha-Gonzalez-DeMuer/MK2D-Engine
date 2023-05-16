#pragma once
#include "SoundSystem.h"
#include <memory>

class ServiceLocator final
{
public:
	ServiceLocator() = default;
	~ServiceLocator() = default;
	ServiceLocator(const ServiceLocator& other) = delete;
	ServiceLocator(ServiceLocator&& other) = delete;
	ServiceLocator& operator=(const ServiceLocator& other) = delete;
	ServiceLocator& operator=(ServiceLocator&& other) = delete;

	static void RegisterSoundSystem(SoundSystem* pSoundSystem);
	static SoundSystem* GetSoundSystem();

private:
	static std::unique_ptr<SoundSystem> m_pSoundSystem;
};