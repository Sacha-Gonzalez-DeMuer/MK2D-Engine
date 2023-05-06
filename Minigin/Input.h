#pragma once
#include "Singleton.h"
#include "XBox360Controller.h"
#include "Commands.h"
#include <SDL.h>
#include <map>
#include <memory>
#include <vector>

namespace dae
{
	class Input final : public Singleton<Input>
	{
	public:
		using ControllerKey = std::pair<unsigned, XBox360Controller::ControllerButton>;
		using ControllerCommandsMap = std::map<ControllerKey, std::shared_ptr<Command>>;

		using KeyboardKey = std::pair<SDL_Scancode, SDL_EventType>;
		using KeyboardCommandsMap = std::map<KeyboardKey, std::shared_ptr<Command>>;

		Input() {};
		~Input() = default;
		Input(const Input& other) = delete;
		Input(Input&& other) = delete;
		Input& operator=(const Input& other) = delete;
		Input& operator=(Input&& other) = delete;

		void AddController(std::shared_ptr<XBox360Controller> controller);
		unsigned int Add360Controller(); // returns controller index
		void AddCommand(const ControllerKey& key, std::shared_ptr<Command> command);
		void AddCommand(const KeyboardKey& key, std::shared_ptr<Command> command);

		bool HandleInput() const;

	private:
		ControllerCommandsMap m_ControllerCommands{};
		KeyboardCommandsMap m_KeyboardCommands{};
		std::vector<std::shared_ptr<XBox360Controller>> m_Controllers{};
	};
}

