#pragma once
#include "Singleton.h"
#include "GenericController.h"
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
		using ControllerKey = std::pair<unsigned, GenericController::ControllerButton>;
		using ControllerCommandsMap = std::map<ControllerKey, std::shared_ptr<Command>>;

		using KeyboardCommandsMap = std::map<SDL_Scancode, std::shared_ptr<Command>>;

		Input() {};
		~Input() = default;
		Input(const Input& other) = delete;
		Input(Input&& other) = delete;
		Input& operator=(const Input& other) = delete;
		Input& operator=(Input&& other) = delete;

		void AddController(std::shared_ptr<GenericController> controller);
		std::shared_ptr<dae::GenericController> AddController();
		void AddCommand(const ControllerKey& key, std::shared_ptr<Command> command);
		void AddCommand(const SDL_Scancode& key, std::shared_ptr<Command> command);
		unsigned int GetNrControllers() const;
		bool HandleInput() const;

		void SetControllerActive(unsigned int idx, bool active);

	private:
		ControllerCommandsMap m_ControllerCommands{};
		KeyboardCommandsMap m_KeyboardCommands{};
		std::vector<std::shared_ptr<GenericController>> m_Controllers{};
	};
}

