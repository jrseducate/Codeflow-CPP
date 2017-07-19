#include "DefaultIncludes.h"

#include <windows.h>
#include <Xinput.h>

char* XInputDllNames[] {"xinput1_4.dll", "xinput1_3.dll"};

#define X_INPUT_GET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex, XINPUT_STATE *pState)
#define X_INPUT_SET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex, XINPUT_VIBRATION *pVibration)

typedef X_INPUT_GET_STATE(x_input_get_state);
typedef X_INPUT_SET_STATE(x_input_set_state);

X_INPUT_GET_STATE(XInputGetStateStub)
{
	return(ERROR_DEVICE_NOT_CONNECTED);
}
X_INPUT_SET_STATE(XInputSetStateStub)
{
	return(ERROR_DEVICE_NOT_CONNECTED);
}

static x_input_get_state* XInputGetState_ = XInputGetStateStub;
static x_input_set_state* XInputSetState_ = XInputSetStateStub;

#define XInputGetState XInputGetState_
#define XInputSetState XInputSetState_

#define RUMBLE_DEADZONE 1024

#define LEFT_ANALOG_STICK_DEADZONE 1024
#define RIGHT_ANALOG_STICK_DEADZONE 1024

#define LEFT_BUMPER_DEADZONE 0
#define RIGHT_BUMPER_DEADZONE 0

internal void
Win32InitXInput()
{
	HMODULE XInputLib = 0;
	for (u32 i = 0;
		i < ArrayCount(XInputDllNames) && !XInputLib;
		i++)
	{
		XInputLib = LoadLibrary(XInputDllNames[i]);
	}

	if (XInputLib)
	{
		XInputGetState = (x_input_get_state*)GetProcAddress(XInputLib, "XInputGetState");
		XInputSetState = (x_input_set_state*)GetProcAddress(XInputLib, "XInputSetState");
	}
}

//
// NOTE(Jeremy): Win32 Input Functions
//

internal void
UpdateButtonState(button* Button, b32 Pressed)
{
	b32 WasPressed = Button->Pressed;
	b32 IsPressed = Pressed;

	Button->Pressed = IsPressed;
	if (!WasPressed && IsPressed)
	{
		Button->JustPressed = true;
	}
	else
	{
		Button->JustPressed = false;
	}

	if (WasPressed && !IsPressed)
	{
		Button->JustReleased = true;
	}
	else
	{
		Button->JustReleased = false;
	}
}

internal void
UpdateGamepadAnalogStickState(analog_stick* Stick, s16 X, s16 Y, u16 Deadzone = 0)
{
	Stick->PrevX = Stick->X;
	Stick->PrevY = Stick->Y;

	s16 StickX = ((Absolute(X) > Deadzone) ? X : 0);
	s16 StickY = ((Absolute(Y) > Deadzone) ? Y : 0);

	Stick->X = (StickX < 0) ? -((r32)StickX / (r32)MIN_S16) : ((r32)StickX / (r32)MAX_S16);
	Stick->Y = (StickY < 0) ? -((r32)StickY / (r32)MIN_S16) : ((r32)StickY / (r32)MAX_S16);
}

internal void
Win32UpdateGamepadButtonState(button* Button, XINPUT_GAMEPAD* Gamepad, WORD GamepadButton)
{
	WORD GamepadButtons = Gamepad->wButtons;
	b32 Pressed = GamepadButtons & GamepadButton;

	UpdateButtonState(Button, Pressed);
}

internal void
Win32UpdateGamepadRumbleState(DWORD GamepadRumble, rumble* Rumble)
{
	if (Rumble->LeftScale > RUMBLE_DEADZONE || Rumble->RightScale > RUMBLE_DEADZONE)
	{
		XINPUT_VIBRATION Vibration;

		Vibration.wLeftMotorSpeed = Rumble->LeftScale;
		Vibration.wRightMotorSpeed = Rumble->RightScale;

		XInputSetState(GamepadRumble, &Vibration);

		Rumble->Rumbling = true;
	}
	else if (Rumble->Rumbling)
	{
		XINPUT_VIBRATION Vibration;

		Vibration.wLeftMotorSpeed = 0;
		Vibration.wRightMotorSpeed = 0;

		XInputSetState(GamepadRumble, &Vibration);

		Rumble->Rumbling = false;
	}

	SetGamepadRumbleState(Rumble, 0, 0);
}

internal void
Win32UpdateGamepadAnalogStickStates(analog_stick* LeftAnalogStick, analog_stick* RightAnalogStick, XINPUT_GAMEPAD* Gamepad)
{
	UpdateGamepadAnalogStickState(LeftAnalogStick, Gamepad->sThumbLX, Gamepad->sThumbLY, LEFT_ANALOG_STICK_DEADZONE);
	UpdateGamepadAnalogStickState(RightAnalogStick, Gamepad->sThumbRX, Gamepad->sThumbRY, RIGHT_ANALOG_STICK_DEADZONE);
}

internal void
Win32UpdateGamepadTriggerState(trigger* Trigger, u08 TriggerValue)
{
    Trigger->PrevValue = Trigger->Value;
    
    r32 NewValue = ((r32)TriggerValue / (r32)MAX_U08);
    Trigger->Value = (TriggerValue > LEFT_BUMPER_DEADZONE) ? NewValue : 0;
}

#define LOW_ASCII 0x30
#define HIGH_ASCII 0x5A

internal void
Win32UpdateKeyboardButtonState(keyboard* Keyboard, u32 VKeyCode, b32 Pressed)
{
	button* Button;
    switch (VKeyCode)
    {
        case VK_UP:			{Button = Keyboard->Up; } break;
        case VK_DOWN:		{Button = Keyboard->Down; } break;
        case VK_LEFT:		{Button = Keyboard->Left; } break;
        case VK_RIGHT:		{Button = Keyboard->Right; } break;
        case VK_SPACE:		{Button = Keyboard->Space; } break;

        case VK_F1:			{Button = Keyboard->F1; } break;
        case VK_F2:			{Button = Keyboard->F2; } break;
        case VK_F3:			{Button = Keyboard->F3; } break;
        case VK_F4:			{Button = Keyboard->F4; } break;
        case VK_F5:			{Button = Keyboard->F5; } break;
        case VK_F6:			{Button = Keyboard->F6; } break;
        case VK_F7:			{Button = Keyboard->F7; } break;
        case VK_F8:			{Button = Keyboard->F8; } break;
        case VK_F9:			{Button = Keyboard->F9; } break;
        case VK_F10:		{Button = Keyboard->F10; } break;
        case VK_F11:		{Button = Keyboard->F11; } break;
        case VK_F12:		{Button = Keyboard->F12; } break;

        case VK_SHIFT:		{Button = Keyboard->Shift; } break;
        case VK_MENU:		{Button = Keyboard->Alt; } break;
        case VK_CONTROL:	{Button = Keyboard->Ctrl; } break;
        case VK_TAB:		{Button = Keyboard->Tab; } break;
        
        default:            {Button =  Keyboard->Keys + VKeyCode;} break;
    }

	UpdateButtonState(Button, Pressed);
}

internal void
Win32UpdateControllers(controller* Controllers)
{
	for (DWORD ControllerIndex = 0;
		ControllerIndex < XUSER_MAX_COUNT;
		ControllerIndex++)
	{
		XINPUT_STATE ControllerState = {};
		if (XInputGetState(ControllerIndex, &ControllerState) == ERROR_SUCCESS)
		{
			XINPUT_GAMEPAD* Gamepad = &ControllerState.Gamepad;
			controller* Controller = Controllers + ControllerIndex;

			Win32UpdateGamepadRumbleState(ControllerIndex, &Controller->Rumble);

			Win32UpdateGamepadButtonState(&Controller->ButtonA, Gamepad, XINPUT_GAMEPAD_A);
			Win32UpdateGamepadButtonState(&Controller->ButtonB, Gamepad, XINPUT_GAMEPAD_B);
			Win32UpdateGamepadButtonState(&Controller->ButtonX, Gamepad, XINPUT_GAMEPAD_X);
			Win32UpdateGamepadButtonState(&Controller->ButtonY, Gamepad, XINPUT_GAMEPAD_Y);
            
			Win32UpdateGamepadButtonState(&Controller->DPadUp, Gamepad, XINPUT_GAMEPAD_DPAD_UP);
			Win32UpdateGamepadButtonState(&Controller->DPadDown, Gamepad, XINPUT_GAMEPAD_DPAD_DOWN);
			Win32UpdateGamepadButtonState(&Controller->DPadLeft, Gamepad, XINPUT_GAMEPAD_DPAD_LEFT);
			Win32UpdateGamepadButtonState(&Controller->DPadRight, Gamepad, XINPUT_GAMEPAD_DPAD_RIGHT);
            
			Win32UpdateGamepadButtonState(&Controller->LeftBumper, Gamepad, XINPUT_GAMEPAD_LEFT_SHOULDER);
			Win32UpdateGamepadButtonState(&Controller->RightBumper, Gamepad, XINPUT_GAMEPAD_RIGHT_SHOULDER);
            
			Win32UpdateGamepadButtonState(&Controller->Start, Gamepad, XINPUT_GAMEPAD_START);
			Win32UpdateGamepadButtonState(&Controller->Back, Gamepad, XINPUT_GAMEPAD_BACK);
            
            Win32UpdateGamepadTriggerState(&Controller->LeftTrigger, Gamepad->bLeftTrigger);
            Win32UpdateGamepadTriggerState(&Controller->RightTrigger, Gamepad->bRightTrigger);

			Win32UpdateGamepadAnalogStickStates(&Controller->LeftStick, &Controller->RightStick, Gamepad);
		}
	}
}

internal void
Win32InitKeyboard(keyboard* Keyboard)
{
	Keyboard->Up = Keyboard->Keys + VK_UP;
	Keyboard->Down = Keyboard->Keys + VK_DOWN;
	Keyboard->Left = Keyboard->Keys + VK_LEFT;
	Keyboard->Right = Keyboard->Keys + VK_RIGHT;
	Keyboard->Space = Keyboard->Keys + VK_SPACE;
	
	Keyboard->F1 = Keyboard->Keys + VK_F1;
	Keyboard->F2 = Keyboard->Keys + VK_F2;
	Keyboard->F3 = Keyboard->Keys + VK_F3;
	Keyboard->F4 = Keyboard->Keys + VK_F4;
	Keyboard->F5 = Keyboard->Keys + VK_F5;
	Keyboard->F6 = Keyboard->Keys + VK_F6;
	Keyboard->F7 = Keyboard->Keys + VK_F7;
	Keyboard->F8 = Keyboard->Keys + VK_F8;
	Keyboard->F9 = Keyboard->Keys + VK_F9;
	Keyboard->F10 = Keyboard->Keys + VK_F10;
	Keyboard->F11 = Keyboard->Keys + VK_F11;
	Keyboard->F12 = Keyboard->Keys + VK_F12;
	
	Keyboard->Shift = Keyboard->Keys + VK_SHIFT;
	Keyboard->Alt = Keyboard->Keys + VK_MENU;
	Keyboard->Ctrl = Keyboard->Keys + VK_CONTROL;
	Keyboard->Tab = Keyboard->Keys + VK_TAB;
}