#include "DefaultIncludes.h"

#include <math.h>

#include "Codeflow.h"
#include "Render.h"

#include "Render.cpp"
#include "SpaceInvaders.cpp"

internal void
OutputSound(game_state* GameState, sound_buffer* SoundBuffer)
{
    u32 ToneVolume = 3000;
    
	s16* SampleOut = SoundBuffer->Samples;
	u32 SampleCount = SoundBuffer->SampleCount;
	u32 WavePeriod = SoundBuffer->SamplesPerSecond / GameState->ToneHz;
    
    for (u32 SampleIndex = 0;
        SampleIndex < SampleCount;
        SampleIndex++)
    {
        r32 SineValue = sinf(GameState->tSine);
        s16 SampleValue = (s16)(SineValue * ToneVolume);
        
        *SampleOut = SampleValue; 
		SampleOut++;
        *SampleOut = SampleValue; 
		SampleOut++;

        GameState->tSine += 2.0f * PI32 * (1.0f / (r32)WavePeriod);
    }
}

extern "C"
GAME_UPDATE_AND_RENDER(GameUpdateAndRender)
{
	UpdateAndRender_SpaceInvaders(GameMemory, Buffer, PlayerInput);
}

extern "C"
GAME_GET_SOUND_SAMPLES(GameGetSoundSamples)
{
	game_state* GameState = (game_state*)Memory->Permament.Memory;
    //OutputSound(GameState, SoundBuffer);
}