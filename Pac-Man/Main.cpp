#include <SDL.h>

#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "PacJSONParser.h"
#include "PacData.h"
#include "PacGameManager.h"

#include "PacCommands.h"
#include "Input.h"
namespace dae
{
	void LoadPacMan()
	{
		auto jsonParser = std::make_shared<PacJSONParser>(PacData::PacJSONpath);
		PacData::PacGameData game_data{};
		jsonParser->LoadGameData(game_data);

		auto skipLevel = std::make_shared<PacSkipLevelCommand>();
		Input::Get().AddCommand(SDL_SCANCODE_L, skipLevel);

		PacGameManager::Get().Initialize(game_data);
		PacGameManager::Get().LoadLevel(0);
	}
}


int main(int, char* []) {
	dae::Minigin engine("../Data/");
	engine.Run(dae::LoadPacMan);
	return 0;
}