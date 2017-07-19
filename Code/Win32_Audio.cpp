#include "DefaultIncludes.h"
#include <dsound.h>

#define DSoundDllName "dsound.dll"

#define DIRECT_SOUND_CREATE(name) HRESULT WINAPI name(LPCGUID pcGuidDevice, LPDIRECTSOUND *ppDS, LPUNKNOWN pUnkOuter)
typedef DIRECT_SOUND_CREATE(direct_sound_create);

#define DIRECT_SOUND_BITS_PER_SAMPLE 16
#define DIRECT_SOUND_CHANNELS 2

struct win32_audio_buffer
{
	LPDIRECTSOUNDBUFFER SecondaryBuffer;
    
	DWORD PlayCursor;
	DWORD WriteCursor;
    DWORD ByteToLock;
    DWORD BytesToWrite;
    DWORD TargetCursor;
    
    DWORD SafetyBytes;
    DWORD ExpectedBytesPerFrame;
    
	u32 BufferSize;
	u32 BytesPerSample;
	u32 SamplesPerSecond;
	u32 RunningSampleIndex;
    
    b32 IsValid;
    b32 IsLowLatency;
    s16* Samples;
};

internal void
Win32InitSound(win32_audio_buffer* AudioBuffer, HWND Window, 
                u16 SamplesPerSecond, u16 BytesPerSample, u16 Volume,
                u32 GameUpdateHz)
{
	u32 BufferSize = SamplesPerSecond * BytesPerSample;

	HMODULE DSoundLibrary = LoadLibrary(DSoundDllName);

	if (DSoundLibrary)
	{
		direct_sound_create* DirectSoundCreate = 
			(direct_sound_create*)GetProcAddress(DSoundLibrary, "DirectSoundCreate");

		LPDIRECTSOUND DirectSound;
		if (DirectSoundCreate && SUCCEEDED(DirectSoundCreate(0, &DirectSound, 0)))
		{
			WAVEFORMATEX WaveFormat = {};
			WaveFormat.cbSize = sizeof(WaveFormat);
			WaveFormat.wFormatTag = WAVE_FORMAT_PCM;
			WaveFormat.wBitsPerSample = DIRECT_SOUND_BITS_PER_SAMPLE;
			WaveFormat.nChannels = DIRECT_SOUND_CHANNELS;
			WaveFormat.nSamplesPerSec = SamplesPerSecond;
			WaveFormat.nBlockAlign = (WaveFormat.nChannels * WaveFormat.wBitsPerSample) / 8;
			WaveFormat.nAvgBytesPerSec = WaveFormat.nSamplesPerSec * WaveFormat.nBlockAlign;

			if (SUCCEEDED(DirectSound->SetCooperativeLevel(Window, DSSCL_PRIORITY)))
			{
				DSBUFFERDESC BufferDescription = {};
				BufferDescription.dwSize = sizeof(BufferDescription);
				BufferDescription.dwFlags = DSBCAPS_PRIMARYBUFFER;

				LPDIRECTSOUNDBUFFER PrimaryBuffer = {};
				if (SUCCEEDED(DirectSound->CreateSoundBuffer(&BufferDescription, &PrimaryBuffer, 0)))
				{
					if (!SUCCEEDED(PrimaryBuffer->SetFormat(&WaveFormat)))
					{
						//NOTE(Jeremy): Handle failure to initialize primary buffer!
					}
				}
			}
			DSBUFFERDESC BufferDescription = {};
			BufferDescription.dwSize = sizeof(BufferDescription);
			BufferDescription.dwFlags = 0;
			BufferDescription.dwBufferBytes = BufferSize;
			BufferDescription.lpwfxFormat = &WaveFormat;

			LPDIRECTSOUNDBUFFER SecondaryBuffer = {};
			if (SUCCEEDED(DirectSound->CreateSoundBuffer(&BufferDescription, &SecondaryBuffer, 0)))
			{
				AudioBuffer->SecondaryBuffer = SecondaryBuffer;
				AudioBuffer->SamplesPerSecond = SamplesPerSecond;
				AudioBuffer->BytesPerSample = BytesPerSample;
				AudioBuffer->BufferSize = BufferSize;
                AudioBuffer->Samples = (s16*)VirtualAlloc(0, AudioBuffer->BufferSize, 
                                                        MEM_COMMIT, PAGE_READWRITE);
                                      
                AudioBuffer->ExpectedBytesPerFrame = (AudioBuffer->SamplesPerSecond 
                                                    * AudioBuffer->BytesPerSample) / GameUpdateHz;                  
                AudioBuffer->SafetyBytes = (AudioBuffer->SamplesPerSecond 
                                                    * AudioBuffer->BytesPerSample) / GameUpdateHz / 2;
			}
		}
	}
}

internal void
Win32ClearAudioBuffer(win32_audio_buffer* AudioBuffer)
{
    void* Region1;
	DWORD Region1Size;
	void* Region2;
	DWORD Region2Size;
	if (SUCCEEDED(AudioBuffer->SecondaryBuffer->Lock(0, AudioBuffer->BufferSize,
		&Region1, &Region1Size,
		&Region2, &Region2Size,
		0)))
	{
        u08* DestSample = (u08*)Region1;
        for (DWORD ByteIndex = 0;
            ByteIndex < Region1Size;
            ByteIndex++)
        {
            *DestSample = 0; 
            DestSample++;
        }
       DestSample = (u08*)Region2;
        for (DWORD ByteIndex = 0;
            ByteIndex < Region2Size;
            ByteIndex++)
        {
            *DestSample = 0; 
            DestSample++;
        }
        
        AudioBuffer->SecondaryBuffer->Unlock(Region1, Region1Size, Region2, Region2Size);
    }
}

internal void
Win32FillAudioBuffer(win32_audio_buffer* AudioBuffer, sound_buffer* GameSoundBuffer)
{
    DWORD& ByteToLock = AudioBuffer->ByteToLock;
    DWORD& BytesToWrite = AudioBuffer->BytesToWrite;
    
	void* Region1;
	DWORD Region1Size;
	void* Region2;
	DWORD Region2Size;
	if (SUCCEEDED(AudioBuffer->SecondaryBuffer->Lock(ByteToLock, BytesToWrite,
		&Region1, &Region1Size,
		&Region2, &Region2Size,
		0)))
	{

		DWORD Region1SampleCount = Region1Size / AudioBuffer->BytesPerSample;
        s16* SourceSample = GameSoundBuffer->Samples;
		s16* DestSample = (s16*)Region1;
		for (DWORD SampleIndex = 0;
			SampleIndex < Region1SampleCount;
			SampleIndex++)
		{
			*DestSample = *SourceSample; 
            DestSample++; SourceSample++;
            *DestSample = *SourceSample; 
            DestSample++; SourceSample++;
			AudioBuffer->RunningSampleIndex++;
		}

		DWORD Region2SampleCount = Region2Size / AudioBuffer->BytesPerSample;
		DestSample = (s16*)Region2;
		for (DWORD SampleIndex = 0;
			SampleIndex < Region2SampleCount;
			SampleIndex++)
		{
			*DestSample = *SourceSample; 
            DestSample++; SourceSample++;
            *DestSample = *SourceSample; 
            DestSample++; SourceSample++;
			AudioBuffer->RunningSampleIndex++;
		}

		AudioBuffer->SecondaryBuffer->Unlock(Region1, Region1Size, Region2, Region2Size);
	}
}

internal void
Win32UpdateAudio(win32_audio_buffer* AudioBuffer)
{
	DWORD& PlayCursor = AudioBuffer->PlayCursor;
	DWORD& WriteCursor = AudioBuffer->WriteCursor;

    if (SUCCEEDED(AudioBuffer->SecondaryBuffer->GetCurrentPosition(&PlayCursor, &WriteCursor)))
    {
        if (!AudioBuffer->IsValid)
        {
            AudioBuffer->RunningSampleIndex = WriteCursor / AudioBuffer->BytesPerSample;
            AudioBuffer->IsValid = true;
        }
        
        DWORD& ByteToLock = AudioBuffer->ByteToLock;
        DWORD& BytesToWrite = AudioBuffer->BytesToWrite;
        DWORD& TargetCursor = AudioBuffer->TargetCursor;
        DWORD& SafetyBytes = AudioBuffer->SafetyBytes;
        DWORD& ExpectedBytesPerFrame = AudioBuffer->ExpectedBytesPerFrame;
        
        b32& IsLowLatency = AudioBuffer->IsLowLatency;
          
        DWORD ExpectedFrameByte = PlayCursor + ExpectedBytesPerFrame;
        
        DWORD SafeWriteCursor = WriteCursor;
        if (SafeWriteCursor < PlayCursor)
        {
            SafeWriteCursor += AudioBuffer->BufferSize;
        }
		Assert(SafeWriteCursor >= PlayCursor);
        SafeWriteCursor += SafetyBytes;
        
        IsLowLatency = (SafeWriteCursor < ExpectedFrameByte);
        
        ByteToLock = (AudioBuffer->RunningSampleIndex * AudioBuffer->BytesPerSample)
            % AudioBuffer->BufferSize;
        
		TargetCursor = 0;
        if (IsLowLatency)
        {
            TargetCursor = (ExpectedFrameByte + ExpectedBytesPerFrame);
        }
        else
        {
            TargetCursor = (WriteCursor + ExpectedBytesPerFrame + SafetyBytes);
        }
        TargetCursor = (TargetCursor % AudioBuffer->BufferSize);
        
        BytesToWrite = 0;
        if (ByteToLock > TargetCursor)
        {
            BytesToWrite = AudioBuffer->BufferSize - ByteToLock;
            BytesToWrite += TargetCursor;
        }
        else
        {
            BytesToWrite = TargetCursor - ByteToLock;
        }
        
    }
    else
    {
        AudioBuffer->IsValid = false;
    }
}

internal void
Win32UpdateAudioLatency(win32_audio_buffer* AudioBuffer)
{
    DWORD TestPlayCursor = 0;
    DWORD TestWriteCursor = 0;
    DWORD LatencyBytes = 0;
    r32 LatencySeconds  = 0;
    
    if (SUCCEEDED(AudioBuffer->SecondaryBuffer->GetCurrentPosition(&TestPlayCursor, 
                                                    &TestWriteCursor)))
    {        
        DWORD UnwrappedWriteCursor = TestWriteCursor;
        if (UnwrappedWriteCursor < TestPlayCursor)
        {
            UnwrappedWriteCursor += AudioBuffer->BufferSize;
        }
        LatencyBytes = UnwrappedWriteCursor - TestPlayCursor;
        LatencySeconds = ((r32)LatencyBytes / (r32)AudioBuffer->BytesPerSample)
                                    / (r32)AudioBuffer->SamplesPerSecond;
    }
    
    char TEMP_TextBuffer[256] = {};
    _snprintf_s(TEMP_TextBuffer, sizeof(TEMP_TextBuffer), "\tPC:%u, \tWC:%u, \tBBC:%u, \tALS:%0.04f\n",
        TestPlayCursor, TestWriteCursor, LatencyBytes,
        LatencySeconds);
    OutputDebugString(TEMP_TextBuffer);
}