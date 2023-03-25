#pragma once
#include "Datatypes.h"
#include <vector>

class Keybindings final
{
public:
	Keybindings();

	Keybindings(const Keybindings& other) = delete;
	Keybindings& operator=(const Keybindings& other) = delete;
	Keybindings(Keybindings&& other) = delete;
	Keybindings& operator=(const Keybindings&& other) = delete;

	void AddKeybinding(const dae::Keybinding& keybinding) { m_Keybindings.push_back(keybinding); }
	const std::vector<dae::Keybinding>& GetKeybindings() const { return m_Keybindings; };

private:
	std::vector<dae::Keybinding> m_Keybindings;
};

