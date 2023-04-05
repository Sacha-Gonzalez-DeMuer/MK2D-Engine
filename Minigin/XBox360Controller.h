#pragma once

namespace dae
{
	class XBox360Controller final
	{
		class XBox360ControllerImpl;
		XBox360ControllerImpl* pImpl;
	public:
		enum class ControllerButton
		{
			DPadUp = 0x0001,
			DPadDown = 0x0002,
			DPadLeft = 0x0004,
			DPadRight = 0x0008,
			Start = 0x0010,
			Back = 0x0020,
			LeftThumb = 0x0040,
			RightThumb = 0x0080,
			LeftShoulder = 0x0100,
			RightShoulder = 0x0200,
			A = 0x1000,
			B = 0x2000,
			X = 0x4000,
			Y = 0x8000
		};

		struct JoystickState
		{
			float x = 0.0f;
			float y = 0.0f;
		};

		void Update();

		bool IsDown(const ControllerButton& button) const;
		bool IsDownThisFrame(const ControllerButton& button) const;
		bool IsReleased(const ControllerButton& button) const;

		unsigned int GetControllerIndex() const;

		explicit XBox360Controller(int controllerIndex);
		~XBox360Controller();
	};

}

