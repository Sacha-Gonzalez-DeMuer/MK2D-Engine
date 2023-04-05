#pragma once
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Xinput.h>
#pragma comment(lib, "xinput.lib")

#include "Xbox360Controller.h"
#include <cmath>

using namespace dae;

class XBox360Controller::XBox360ControllerImpl
{
	XINPUT_STATE previousState{};
	XINPUT_STATE currentState{};

	WORD buttonsPressedThisFrame;
	WORD buttonsReleasedThisFrame;

	unsigned int _controllerIndex;
	JoystickState m_JoystickState;

	float deadzoneX = 0.2f;
	float deadzoneY = 0.2f;

public:
	XBox360ControllerImpl(unsigned int controllerIndex)
		: _controllerIndex(controllerIndex)
	{
		ZeroMemory(&previousState, sizeof(XINPUT_STATE));
		ZeroMemory(&currentState, sizeof(XINPUT_STATE));
	}
	~XBox360ControllerImpl() = default;

	void Update()
	{
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

	bool IsDownThisFrame(unsigned int button) const {return buttonsPressedThisFrame & button;}
	bool IsReleased(unsigned int button) const {return buttonsReleasedThisFrame & button;}
	bool IsDown(unsigned int button) const {return currentState.Gamepad.wButtons & button;}
};

void dae::XBox360Controller::Update()
{
	pImpl->Update();
}

bool dae::XBox360Controller::IsDown(const ControllerButton& button) const
{
	return pImpl->IsDown(static_cast<unsigned int>(button));
}

bool dae::XBox360Controller::IsDownThisFrame(const ControllerButton& button) const
{
	return pImpl->IsDownThisFrame(static_cast<unsigned int>(button));
}

bool dae::XBox360Controller::IsReleased(const ControllerButton& button) const
{
	return pImpl->IsReleased(static_cast<unsigned int>(button));
}

unsigned int dae::XBox360Controller::GetControllerIndex() const
{
	return pImpl->GetControllerIndex();
}

dae::XBox360Controller::XBox360Controller(int controllerIndex)
	: pImpl(new XBox360ControllerImpl(controllerIndex))
{
}

dae::XBox360Controller::~XBox360Controller()
{
	delete pImpl;
	pImpl = nullptr;
}
