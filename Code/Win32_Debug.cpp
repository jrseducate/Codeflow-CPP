#include "DefaultIncludes.h"

#pragma once

struct debug_win32_audio_time_marker
{
	DWORD FlipPlayCursor;
	DWORD FlipWriteCursor;
	DWORD OutputPlayCursor;
	DWORD OutputWriteCursor;
	DWORD OutputLocation;
	DWORD OutputSizeInBytes;
};

struct debug_win32_state
{
	HANDLE RecordingHandle;
	u32 InputRecordingIndex;
	b32 IsRecording;

	HANDLE PlaybackHandle;
	u32 InputPlaybackIndex;
	b32 IsPlaying;

	read_file_result InputFile;
	u32 InputFrame;
};

struct debug_win32_memory_buffer
{
	void* Buffer;
	u08* Iter;
	memory_index Size;
	memory_index Used;
};

struct debug_win32_game
{
	HMODULE Dll;
	FILETIME LastModifiedTime;
	FILETIME NewModifiedTime;
	game_code Code;
};

internal void
DEBUGPushToMemoryBuffer_(debug_win32_memory_buffer* DebugWin32MemoryBuffer, void* Value, memory_index ValueSize)
{
	Assert(DebugWin32MemoryBuffer->Used + ValueSize <= DebugWin32MemoryBuffer->Size);
	for (u32 i = 0;
		i < ValueSize;
		i++)
	{
		*DebugWin32MemoryBuffer->Iter = *((u08*)Value + i);
		DebugWin32MemoryBuffer->Iter++;
	}
	DebugWin32MemoryBuffer->Used += ValueSize;
}
#define DEBUGPushToMemoryBuffer(DebugWin32MemoryBuffer, Value) DEBUGPushToMemoryBuffer_(&DebugWin32MemoryBuffer, &Value, sizeof(Value))
#define DEBUGPushPointerToMemoryBuffer(DebugWin32MemoryBuffer, Value, Size) DEBUGPushToMemoryBuffer_(&DebugWin32MemoryBuffer, Value, Size)

internal void
DEBUGPullFromMemoryBuffer_(debug_win32_memory_buffer* DebugWin32MemoryBuffer, void* Value, memory_index ValueSize)
{
	Assert(DebugWin32MemoryBuffer->Used - ValueSize >= 0);
	for (u32 i = 0;
		i < ValueSize;
		i++)
	{
		*((u08*)Value + i) = *DebugWin32MemoryBuffer->Iter;
		DebugWin32MemoryBuffer->Iter++;
	}
	DebugWin32MemoryBuffer->Used -= ValueSize;
}
#define DEBUGPullFromMemoryBuffer(DebugWin32MemoryBuffer, Value) DEBUGPullFromMemoryBuffer_(&DebugWin32MemoryBuffer, &Value, sizeof(Value))
#define DEBUGPullPointerFromMemoryBuffer(DebugWin32MemoryBuffer, Value, Size) DEBUGPullFromMemoryBuffer_(&DebugWin32MemoryBuffer, Value, Size)

internal void
Win32GetExecutablePath(char* Buffer);

internal void
DEBUGWin32SaveGameMemory(debug_win32_state* DebugWin32State, game_memory* GameMemory)
{
	char SaveFilePath[MAX_PATH];
	Win32GetExecutablePath(SaveFilePath);
	sprintf_s(SaveFilePath, "%sgamememory_%d.mem",
		SaveFilePath,
		DebugWin32State->InputRecordingIndex);

	debug_win32_memory_buffer DebugWin32MemoryBuffer = {};
	DebugWin32MemoryBuffer.Size = GameMemory->Permament.Used + GameMemory->Transient.Used + (2 * sizeof(u64));
	DebugWin32MemoryBuffer.Buffer = VirtualAlloc(0, DebugWin32MemoryBuffer.Size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	DebugWin32MemoryBuffer.Iter = (u08*)DebugWin32MemoryBuffer.Buffer;

	DEBUGPushToMemoryBuffer(DebugWin32MemoryBuffer, GameMemory->Permament.Used);
	DEBUGPushPointerToMemoryBuffer(DebugWin32MemoryBuffer, GameMemory->Permament.Memory, GameMemory->Permament.Used);
	DEBUGPushToMemoryBuffer(DebugWin32MemoryBuffer, GameMemory->Transient.Used);
	DEBUGPushPointerToMemoryBuffer(DebugWin32MemoryBuffer, GameMemory->Transient.Memory, GameMemory->Transient.Used);

	Win32WriteEntireFile(SaveFilePath, DebugWin32MemoryBuffer.Buffer, DebugWin32MemoryBuffer.Used);
}

internal void
DEBUGWin32ReloadGameMemory(debug_win32_state* DebugWin32State, game_memory* GameMemory)
{
	char ReloadFilePath[MAX_PATH];
	Win32GetExecutablePath(ReloadFilePath);
	sprintf_s(ReloadFilePath, "%sgamememory_%d.mem",
		ReloadFilePath,
		DebugWin32State->InputPlaybackIndex);

	read_file_result ReloadFile = Win32ReadEntireFile(ReloadFilePath);

	debug_win32_memory_buffer DebugWin32MemoryBuffer = {};
	DebugWin32MemoryBuffer.Size = ReloadFile.Size;
	DebugWin32MemoryBuffer.Buffer = ReloadFile.Memory;
	DebugWin32MemoryBuffer.Iter = (u08*)DebugWin32MemoryBuffer.Buffer;

	if (DebugWin32MemoryBuffer.Size > 0)
	{
		DEBUGPullFromMemoryBuffer(DebugWin32MemoryBuffer, GameMemory->Permament.Used);
		DEBUGPullPointerFromMemoryBuffer(DebugWin32MemoryBuffer, GameMemory->Permament.Memory, GameMemory->Permament.Used);
		DEBUGPullFromMemoryBuffer(DebugWin32MemoryBuffer, GameMemory->Transient.Used);
		DEBUGPullPointerFromMemoryBuffer(DebugWin32MemoryBuffer, GameMemory->Transient.Memory, GameMemory->Transient.Used);
	}
	else
	{
		DebugWin32State->InputPlaybackIndex = 0;
	}
}

#define RECORD_INPUT_FRAME_SIZE 3876

internal void
DEBUGWin32InitRecordInput(debug_win32_state* DebugWin32State)
{
	char RecordFilePath[MAX_PATH];
	Win32GetExecutablePath(RecordFilePath);
	sprintf_s(RecordFilePath, "%sinput_%d.inp",
		RecordFilePath,
		DebugWin32State->InputRecordingIndex);

	Win32WriteEntireFile(RecordFilePath, 0, 0);
	DebugWin32State->IsRecording = true;
}

internal void
DEBUGWin32RecordInput(debug_win32_state* DebugWin32State, input* PlayerInput)
{
	char RecordFilePath[MAX_PATH];
	Win32GetExecutablePath(RecordFilePath);
	sprintf_s(RecordFilePath, "%sinput_%d.inp",
		RecordFilePath,
		DebugWin32State->InputRecordingIndex);

	debug_win32_memory_buffer DebugWin32MemoryBuffer = {};
	DebugWin32MemoryBuffer.Size = sizeof(DebugWin32MemoryBuffer.Size) + sizeof(PlayerInput->ControllerCount) + (sizeof(*PlayerInput->Controllers) * PlayerInput->ControllerCount);
	DebugWin32MemoryBuffer.Buffer = VirtualAlloc(0, DebugWin32MemoryBuffer.Size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	DebugWin32MemoryBuffer.Iter = (u08*)DebugWin32MemoryBuffer.Buffer;

	DEBUGPushToMemoryBuffer(DebugWin32MemoryBuffer, DebugWin32MemoryBuffer.Size);

#if 0
	for (u32 i = 0;
		i < ArrayCount(PlayerInput->Keyboard->Keys);
		i++)
	{
		DEBUGPushToMemoryBuffer(DebugWin32MemoryBuffer, PlayerInput->Keyboard->Keys[i]);
	}
#endif

	DEBUGPushToMemoryBuffer(DebugWin32MemoryBuffer, PlayerInput->ControllerCount);

	for (u32 i = 0;
		i < PlayerInput->ControllerCount;
		i++)
	{
		DEBUGPushToMemoryBuffer(DebugWin32MemoryBuffer, PlayerInput->Controllers[i]);
	}

	Win32WriteFromEndOfFile(RecordFilePath, DebugWin32MemoryBuffer.Buffer, DebugWin32MemoryBuffer.Used);
}

internal void
DEBUGWin32EndRecordInput(debug_win32_state* DebugWin32State)
{
	DebugWin32State->IsRecording = false;
}

internal void
DEBUGWin32InitPlaybackInput(debug_win32_state* DebugWin32State)
{
	char PlaybackFilePath[MAX_PATH];
	Win32GetExecutablePath(PlaybackFilePath);
	sprintf_s(PlaybackFilePath, "%sinput_%d.inp",
		PlaybackFilePath,
		DebugWin32State->InputPlaybackIndex);

	DebugWin32State->InputFile = Win32ReadEntireFile(PlaybackFilePath);
	DebugWin32State->IsPlaying = true;
}

internal void
DEBUGWin32PlaybackInput(debug_win32_state* DebugWin32State, input* PlayerInput)
{
	u32 BufferSize = DebugWin32State->InputFile.Size;

	if (BufferSize > 0)
	{
		debug_win32_memory_buffer DebugWin32MemoryBuffer = {};
		DebugWin32MemoryBuffer.Buffer = DebugWin32State->InputFile.Memory;
		DebugWin32MemoryBuffer.Iter = (u08*)DebugWin32MemoryBuffer.Buffer;
		for (u32 i = 0;
			i < DebugWin32State->InputFrame;
			i++)
		{
			DEBUGPullFromMemoryBuffer(DebugWin32MemoryBuffer, DebugWin32MemoryBuffer.Size);
			DebugWin32MemoryBuffer.Iter += DebugWin32MemoryBuffer.Size - sizeof(DebugWin32MemoryBuffer.Size);
		}
		DEBUGPullFromMemoryBuffer(DebugWin32MemoryBuffer, DebugWin32MemoryBuffer.Size);


#if 0
		for (u32 i = 0;
			i < ArrayCount(PlayerInput->Keyboard->Keys);
			i++)
		{
			DEBUGPullFromMemoryBuffer(DebugWin32MemoryBuffer, *(PlayerInput->Keyboard->Keys + i));
		}
#endif

		DEBUGPullFromMemoryBuffer(DebugWin32MemoryBuffer, PlayerInput->ControllerCount);

		for (u32 i = 0;
			i < PlayerInput->ControllerCount;
			i++)
		{
			DEBUGPullFromMemoryBuffer(DebugWin32MemoryBuffer, *(PlayerInput->Controllers + i));
		}

		DebugWin32State->InputFrame++;
		if (DebugWin32MemoryBuffer.Iter - (u08*)DebugWin32MemoryBuffer.Buffer >= DebugWin32State->InputFile.Size)
		{
			DebugWin32State->InputFrame = 0;
		}
	}
	else
	{
		DebugWin32State->InputPlaybackIndex = 0;
	}
}

internal void
DEBUGWin32EndPlaybackInput(debug_win32_state* DebugWin32State)
{
	Win32FreeFileMemory(&DebugWin32State->InputFile);
	DebugWin32State->InputFile = {};
	DebugWin32State->InputFrame = 0;
	DebugWin32State->IsPlaying = false;
}

internal b32
DEBUGWin32GameChanged(debug_win32_game* DebugWin32Game)
{
	b32 Result = false;

	char ExecutableDirectoryPath[MAX_PATH] = {};
	Win32GetExecutablePath(ExecutableDirectoryPath);

	char CodeflowDllPath[MAX_PATH] = {};
	sprintf_s(CodeflowDllPath, "%s%s",
		ExecutableDirectoryPath,
		"Codeflow.dll");

	HANDLE FileHandle = CreateFileA(CodeflowDllPath, GENERIC_READ, FILE_SHARE_READ,
		0, OPEN_EXISTING, 0, 0);

	if (FileHandle != INVALID_HANDLE_VALUE)
	{
		if (GetFileTime(FileHandle, 0, 0, &DebugWin32Game->NewModifiedTime))
		{
			s32 CompareResult = CompareFileTime(&DebugWin32Game->LastModifiedTime, &DebugWin32Game->NewModifiedTime);
			Result = ((CompareResult != 0) || (!DebugWin32Game->Dll));
		}

		CloseHandle(FileHandle);
	}

	return (Result);
}

internal void
DEBUGWin32UnloadGameCode(debug_win32_game* DebugWin32Game)
{
	DebugWin32Game->Code.GameUpdateAndRender = GameUpdateAndRenderStub;
	DebugWin32Game->Code.GameGetSoundSamples = GameGetSoundSamplesStub;
	DebugWin32Game->Code.IsLoaded = false;
	if (DebugWin32Game->Dll)
	{
		FreeLibrary(DebugWin32Game->Dll);
	}
}

internal void
DEBUGWin32LoadGameCode(debug_win32_game* DebugWin32Game)
{
	debug_win32_game Result = {};

	char ExecutableDirectoryPath[MAX_PATH] = {};
	Win32GetExecutablePath(ExecutableDirectoryPath);

	char CodeflowDllPath[MAX_PATH] = {};
	sprintf_s(CodeflowDllPath, "%s%s",
		ExecutableDirectoryPath,
		"Codeflow.dll");

	char CodeflowTempDllPath[MAX_PATH] = {};
	sprintf_s(CodeflowTempDllPath, "%s%s",
		ExecutableDirectoryPath,
		"Codeflow_tmp.dll");

	b32 CopySuccess = CopyFile(CodeflowDllPath, CodeflowTempDllPath, FALSE);

	Assert(CopySuccess, "There is an error in copying the DLL!");
	DebugWin32Game->Dll = LoadLibrary(CodeflowTempDllPath);

	if (DebugWin32Game->Dll)
	{
		DebugWin32Game->LastModifiedTime = DebugWin32Game->NewModifiedTime;
		DebugWin32Game->Code.GameUpdateAndRender = (game_update_and_render*)GetProcAddress(DebugWin32Game->Dll, "GameUpdateAndRender");
		DebugWin32Game->Code.GameGetSoundSamples = (game_get_sound_samples*)GetProcAddress(DebugWin32Game->Dll, "GameGetSoundSamples");
		DebugWin32Game->Code.IsLoaded = (DebugWin32Game->Code.GameUpdateAndRender && DebugWin32Game->Code.GameGetSoundSamples);
	}
	else
	{
		DEBUGWin32UnloadGameCode(DebugWin32Game);
	}
}

internal void Win32DrawRectangle(win32_bitmap* Bitmap, u32 X, u32 Y,
	u32 Width, u32 Height, u32 BitmapPixel);

internal void
DEBUGWin32DebugSyncDisplay(win32_bitmap* Bitmap, win32_audio_buffer* AudioBuffer,
	debug_win32_audio_time_marker* DebugAudioTimeMarkers, u32 AudioTimeMarkerCount,
	r32 SecondsPerFrame, u32 CurrentMarkerIndex)
{
	u32 PadX = 16;
	u32 PadY = 16;

	r32 C = (r32)(Bitmap->Width - (PadX * 2)) / (r32)AudioBuffer->BufferSize;
	for (u32 AudioTimeMarkerIndex = 0;
		AudioTimeMarkerIndex < AudioTimeMarkerCount;
		AudioTimeMarkerIndex++)
	{
		debug_win32_audio_time_marker DebugAudioTimeMarker = DebugAudioTimeMarkers[AudioTimeMarkerIndex];
		Assert(DebugAudioTimeMarker.OutputPlayCursor < AudioBuffer->BufferSize);
		Assert(DebugAudioTimeMarker.OutputWriteCursor < AudioBuffer->BufferSize);
		Assert(DebugAudioTimeMarker.OutputLocation < AudioBuffer->BufferSize);
		Assert(DebugAudioTimeMarker.OutputSizeInBytes < AudioBuffer->BufferSize);
		Assert(DebugAudioTimeMarker.FlipPlayCursor < AudioBuffer->BufferSize);
		Assert(DebugAudioTimeMarker.FlipWriteCursor < AudioBuffer->BufferSize);

		u32 PlayBitmapPixel = 0xFFFFFFFF;
		u32 WriteBitmapPixel = 0xFFFF0000;

		u32 X = 0;
		u32 Y = 32;
		u32 Width = 2;
		u32 Height = 64;

		if (AudioTimeMarkerIndex == CurrentMarkerIndex)
		{
			Y += Height;

			X = PadX + (u32)(C * (r32)DebugAudioTimeMarker.OutputPlayCursor);
			Win32DrawRectangle(Bitmap, X, Y, Width, Height, PlayBitmapPixel);

			X = PadX + (u32)(C * (r32)DebugAudioTimeMarker.OutputWriteCursor);
			Win32DrawRectangle(Bitmap, X, Y, Width, Height, WriteBitmapPixel);

			Y += Height;

			X = PadX + (u32)(C * (r32)DebugAudioTimeMarker.OutputLocation);
			Win32DrawRectangle(Bitmap, X, Y, Width, Height, PlayBitmapPixel);

			X = PadX + (u32)(C * (r32)(DebugAudioTimeMarker.OutputLocation + DebugAudioTimeMarker.OutputSizeInBytes));
			Win32DrawRectangle(Bitmap, X, Y, Width, Height, WriteBitmapPixel);

			Y += Height;
		}

		X = PadX + (u32)(C * (r32)DebugAudioTimeMarker.FlipPlayCursor);
		Win32DrawRectangle(Bitmap, X, Y, Width, Height, PlayBitmapPixel);

		X = PadX + (u32)(C * (r32)DebugAudioTimeMarker.FlipWriteCursor);
		Win32DrawRectangle(Bitmap, X, Y, Width, Height, WriteBitmapPixel);
	}
}