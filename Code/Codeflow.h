#include "DefaultIncludes.h"

#include "Platform.h"

#pragma once

//
//NOTE(Jeremy): Platform
//

struct offscreen_buffer;
struct sound_buffer;
struct input;

//
//NOTE(Jeremy): Game
//

struct memory_space
{
    memory_index Size;
    u64 Used;
    void* Memory; //NOTE(Jeremy): Must be cleared on allocation
};

struct game_memory
{
	platform_api PlatformApi;
	
    b32 IsValid;
    b32 Initialized;

    memory_space Permament;
    memory_space Transient;

	r32 FPS;
};

struct game_state
{
    u32 ToneHz;
    r32 tSine;
	u32 XOffset;
	u32 YOffset;
	
	s32 PlayerX;
	s32 PlayerY;
};

#define GAME_UPDATE_AND_RENDER(name) void name(game_memory* GameMemory, offscreen_buffer* Buffer, input* PlayerInput)
typedef GAME_UPDATE_AND_RENDER(game_update_and_render);
GAME_UPDATE_AND_RENDER(GameUpdateAndRenderStub){}

#define GAME_GET_SOUND_SAMPLES(name) void name(game_memory* Memory, sound_buffer* SoundBuffer)
typedef GAME_GET_SOUND_SAMPLES(game_get_sound_samples);
GAME_GET_SOUND_SAMPLES(GameGetSoundSamplesStub){}

extern "C"
GAME_UPDATE_AND_RENDER(GameUpdateAndRender);

extern "C"
GAME_GET_SOUND_SAMPLES(GameGetSoundSamples);

struct game_code
{
	b32 IsLoaded;
	game_update_and_render* GameUpdateAndRender;
	game_get_sound_samples* GameGetSoundSamples;
};