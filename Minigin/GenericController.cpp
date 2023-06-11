#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Xinput.h>
#pragma comment(lib, "xinput.lib")

#include "GenericController.h"
#include <cmath>

using namespace dae;

class GenericController::GenericControllerImpl
{
	XINPUT_STATE previousState{};
	XINPUT_STATE currentState{};

	WORD buttonsPressedThisFrame;
	WORD buttonsReleasedThisFrame;

	unsigned int _controllerIndex;
	JoystickState m_JoystickState;

	float deadzoneX = 0.2f;
	float deadzoneY = 0.2f;

	bool isActive{ true };

public:
	GenericControllerImpl(unsigned int controllerIndex)
		: _controllerIndex(controllerIndex)
	{
		ZeroMemory(&previousState, sizeof(XINPUT_STATE));
		ZeroMemory(&currentState, sizeof(XINPUT_STATE));
	}
	~GenericControllerImpl() = default;

	void Update()
	{
		if (!isActive) return;

		CopyMemory(&previousState, &currentState, sizeof(XINPUT_STATE)); 
		ZeroMemory(&currentState, sizeof(XINPUT_STATE)); 
		XInputGetState(_controllerIndex, &currentState); 

		auto buttonChanges = currentState.Gamepad.wButtons ^ previousState.Gamepad.wButtons;
		buttonsPressedThisFrame = buttonChanges & currentState.Gamepad.wButtons;
		buttonsReleasedThisFrame = buttonChanges & (~currentState.Gamepad.wButtons);

		// Read joystick state
		float LX = fmaxf(-1, (float)currentState.Gamepad.sThumbLX / 32767);
		float LY = fmaxf(-1, (float)currentState.Gamepad.sThumbLY / 32767);

		// if axis value is lower than deadzone, set it to 0
		// otherwise normalize it
		m_JoystickState.x = (abs(LX) < deadzoneX ? 0 : (abs(LX) - deadzoneX) * (LX / abs(LX)));
		m_JoystickState.y = (abs(LY) < deadzoneY ? 0 : (abs(LY) - deadzoneY) * (LY / abs(LY)));
	}

	unsigned int GetControllerIndex() const {return _controllerIndex;}
	void SetActive(bool active) { isActive = active; };

	bool IsDownThisFrame(unsigned int button) const {return buttonsPressedThisFrame & button;}
	bool IsReleased(unsigned int button) const {return buttonsReleasedThisFrame & button;}
	bool IsDown(unsigned int button) const {return currentState.Gamepad.wButtons & button;}
};

void dae::GenericController::Update()
{
	pImpl->Update();
}

bool dae::GenericController::IsDown(const ControllerButton& button) const
{
	return pImpl->IsDown(static_cast<unsigned int>(button));
}

bool dae::GenericController::IsDownThisFrame(const ControllerButton& button) const
{
	return pImpl->IsDownThisFrame(static_cast<unsigned int>(button));
}

bool dae::GenericController::IsReleased(const ControllerButton& button) const
{
	return pImpl->IsReleased(static_cast<unsigned int>(button));
}

void dae::GenericController::SetActive(bool active)
{
	pImpl->SetActive(active);
}

unsigned int dae::GenericController::GetControllerIndex() const
{
	return pImpl->GetControllerIndex();
}

dae::GenericController::GenericController(int controllerIndex)
	: pImpl(new GenericControllerImpl(controllerIndex))
{
}

dae::GenericController::~GenericController()
{
	delete pImpl;
	pImpl = nullptr;
}
