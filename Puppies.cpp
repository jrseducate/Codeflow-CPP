#include "DefaultIncludes.h"

#include <math.h>

#include "Codeflow.h"

internal void
OutputSound(game_state* GameState, sound_buffer* SoundBuffer)
{
    u32 ToneVolume = 3000;
    
	s16* SampleOut = SoundBuffer->Samples;
	u32 SampleCount = SoundBuffer->SampleCount;
    u32 WavePeriod = SoundBuffer->SamplesPerSecond/GameState->ToneHz;
    
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

internal void
RenderWeirdGradient(game_state* GameState, offscreen_buffer* Bitmap)
{    
	u32 XOffset = GameState->XOffset;
	u32 YOffset = GameState->YOffset;

	GameState->XOffset += 5;
	GameState->YOffset += 5;
	
	u32 Pitch = (Bitmap->Width * Bitmap->BytesPerPixel);
	u08* Row = (u08*)Bitmap->Memory;
	for (u32 Y = 0;
		Y < Bitmap->Height;
		Y++)
	{        
		u32* Pixel = (u32*)Row;
		for (u32 X = 0;
			X < Bitmap->Width;
			X++)
		{
			u08 R = (u08)(Y + YOffset);
			u08 G = (u08)(X + XOffset);
			u08 B = (u08)(0);
            
			*Pixel = (R << 16) |
					(G << 8) |
					(B);
			Pixel++;
		}
		Row += Pitch;
	}
}

internal void
RenderPlayer(offscreen_buffer* Buffer, u32 X, u32 Y)
{
	u32 Width = 16;
	u32 Height = 16;
	u32 Color = ((0 << 16) | (255 << 8) | (255 << 0));
	
	ClampU(0, Width, Buffer->Width);
	ClampU(0, Height, Buffer->Height);
	ClampU(0, X, Width - 1);
	ClampU(0, Y, Height - 1);
	
	for (u32 YIter = Y;
		YIter < (Y + Height);
		YIter++)
	{
		u08* Pixel = (u08*)Buffer->Memory + 
					(YIter * Buffer->Pitch) +
					(X * Buffer->BytesPerPixel);
		for (u32 XIter = X;
			XIter < (X + Width);
			XIter++)
		{
			if (XIter < Buffer->Width && YIter < Buffer->Height)
			{
				*(u32*)Pixel = Color;
			}
			
			Pixel += 4;
		}
	}
}

extern "C"
GAME_UPDATE_AND_RENDER(GameUpdateAndRender)
{	
	game_state* GameState = (game_state*)GameMemory->Permament.Memory;
    
    if (!GameMemory->Initialized)
    {
		char* FileName = __FILE__;
        read_file_result LoadedFile = GameMemory->PlatformApi.ReadEntireFile(FileName);
        if (LoadedFile.Memory)
        {
			GameMemory->PlatformApi.WriteEntireFile("C:\\Work\\Codeflow\\Puppies.cpp", LoadedFile.Memory, LoadedFile.Size);
			GameMemory->PlatformApi.FreeFileMemory(LoadedFile.Memory);
        }

		GameMemory->Initialized = true;
		
		GameState->ToneHz = 512;
		GameState->tSine = 0.0f;
		GameState->PlayerX = 100;
		GameState->PlayerY = 100;

		GameMemory->Permament.Size += sizeof(game_state);
    }
    
	GameState->PlayerX += (s32)(PlayerInput->Controllers[CONTROLLER_ONE].LeftStick.X * 10.0f);
	GameState->PlayerY += (s32)(PlayerInput->Controllers[CONTROLLER_ONE].LeftStick.Y * -10.0f) + (u32)(sinf(GameState->PlayerJumpTimer) * -10.0f);
	
	if (PlayerInput->Controllers[CONTROLLER_ONE].ButtonA.JustPressed)
	{
		GameState->PlayerJumpTimer = 1.0f;
	}
	
	if (GameState->PlayerJumpTimer > 0.0f)
	{
		GameState->PlayerJumpTimer -= 0.033f;
	}
	else
	{
		GameState->PlayerJumpTimer = 0.0f;
	}

	RenderWeirdGradient(GameState, Buffer);
	RenderPlayer(Buffer, GameState->PlayerX, GameState->PlayerY);
}

;

extern "C"
GAME_GET_SOUND_SAMPLES(GameGetSoundSamples)
{
	game_state* GameState = (game_state*)Memory->Permament.Memory;
    OutputSound(GameState, SoundBuffer);
}