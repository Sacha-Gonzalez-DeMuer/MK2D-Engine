#include <stdexcept>
#define WIN32_LEAN_AND_MEAN 
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "MK2D.h"
#include "Input.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "GameTime.h"
#include "Debug.h"
#include "Datatypes.h"
SDL_Window* g_window{};

void PrintSDLVersion()
{
	SDL_version version{};
	SDL_VERSION(&version);
	printf("We compiled against SDL version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	SDL_GetVersion(&version);
	printf("We are linking against SDL version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_IMAGE_VERSION(&version);
	printf("We compiled against SDL_image version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *IMG_Linked_Version();
	printf("We are linking against SDL_image version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_TTF_VERSION(&version)
	printf("We compiled against SDL_ttf version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *TTF_Linked_Version();
	printf("We are linking against SDL_ttf version %u.%u.%u.\n",
		version.major, version.minor, version.patch);
}

dae::MK2D::MK2D(const std::string &dataPath)
{
	PrintSDLVersion();
	
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	g_window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		static_cast<int>(g_WindowSize.x),
		static_cast<int>(g_WindowSize.y),
		SDL_WINDOW_OPENGL
	);

	if (g_window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::Get().Init(g_window);

	ResourceManager::Get().Init(dataPath);
}

dae::MK2D::~MK2D()
{
	Renderer::Get().Destroy();
	SDL_DestroyWindow(g_window);
	g_window = nullptr;
	SDL_Quit();
}

void dae::MK2D::Run(const std::function<void()>& load)
{
	load();
	srand(static_cast<unsigned int>(time(nullptr)));

	constexpr float desired_FPS{ 144.0f };
	constexpr float frame_time{ 1000 / desired_FPS };

	auto& renderer = Renderer::Get();
	auto& scene_manager = SceneManager::Get();
	auto& input = Input::Get();
	auto& time{ dae::GameTime::Get() };

	scene_manager.Start();

	bool exit = false;
	while (!exit)
	{
		time.Update();
		exit = input.HandleInput();
		scene_manager.Update();
		renderer.Render();
		scene_manager.DeleteObjectsMarkedForDestruction();

		const float sleep_time{ frame_time - time.DeltaTime() };
		const auto& sleep_duration{ std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::duration<float, std::milli>(sleep_time)) };
		if (sleep_time > 0)
			std::this_thread::sleep_for(sleep_duration);
	}
}
