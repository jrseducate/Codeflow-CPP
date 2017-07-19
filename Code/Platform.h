#include "DefaultIncludes.h"

#pragma once

//
//NOTE(Jeremy): Input
//

enum controllers
{
    CONTROLLER_ONE,
    CONTROLLER_TWO,
    CONTROLLER_THREE,
    CONTROLLER_FOUR,
    CONTROLLER_COUNT,
};

struct rumble
{
	u16 LeftScale;
	u16 RightScale;
    b32 Rumbling;
};

struct button
{
	b32 JustPressed;
	b32 Pressed;
	b32 JustReleased;
};

struct analog_stick
{
	r32 X;
	r32 Y;
    
	r32 PrevX;
	r32 PrevY;
};

struct trigger
{
    r32 PrevValue;
    r32 Value;
};

struct controller
{    
    button Start;
    button Back;

	button ButtonA;
	button ButtonB;
	button ButtonX;
	button ButtonY;

	button DPadUp;
	button DPadDown;
	button DPadLeft;
	button DPadRight;
    
	analog_stick LeftStick;
	analog_stick RightStick;
    
    button LeftBumper;
    button RightBumper;
    
    trigger LeftTrigger;
    trigger RightTrigger;
    
	rumble Rumble;
};

struct keyboard
{
	button Keys[256];

	button* Up;
	button* Down;
	button* Left;
	button* Right;
	button* Space;

	button* F1;
	button* F2;
	button* F3;
	button* F4;
	button* F5;
	button* F6;
	button* F7;
	button* F8;
	button* F9;
	button* F10;
	button* F11;
	button* F12;

	button* Shift;
	button* Alt;
	button* Ctrl;
	button* Tab;
};

struct input
{
    keyboard* Keyboard;
    
    controller* Controllers;
    u32 ControllerCount;
};

internal void
SetGamepadRumbleState(rumble* Rumble, u16 LeftScale, u16 RightScale)
{
	Rumble->LeftScale = LeftScale;
	Rumble->RightScale = RightScale;
}

//
//NOTE(Jeremy): Audio
//

struct sound_buffer
{
	s16* Samples;
    u32 SampleCount;
    u32 SamplesPerSecond;
};

//
//NOTE(Jeremy): Rendering
//

struct offscreen_buffer
{
	u32 Width;
	u32 Height;
	u32 BytesPerPixel;
	u32 Pitch;
	void* Memory;
};

//
//NOTE(Jeremy): File IO
//

struct read_file_result
{
	void* Memory;
	u32 Size;
};

//
//NOTE(Jeremy): Platform Functions
//

#define FREE_FILE_MEMORY(name) void name(void* Memory)
typedef FREE_FILE_MEMORY(free_file_memory);
FREE_FILE_MEMORY(FreeFileMemoryStub){}

#define READ_ENTIRE_FILE(name) read_file_result name(char* FileLocation)
typedef READ_ENTIRE_FILE(read_entire_file);
READ_ENTIRE_FILE(ReadEntireFileStub){return {};}

#define WRITE_ENTIRE_FILE(name) b32 name(char* FileLocation, void* Memory, u64 MemorySize)
typedef WRITE_ENTIRE_FILE(write_entire_file);
WRITE_ENTIRE_FILE(WriteEntireFileStub){return {};}

struct platform_api
{
	free_file_memory* FreeFileMemory;
	read_entire_file* ReadEntireFile;
	write_entire_file* WriteEntireFile;
};