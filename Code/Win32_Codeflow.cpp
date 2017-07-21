#include "DefaultIncludes.h"

#include <windows.h>

#include "Platform.h"
#include "Codeflow.h"

#include "Win32_Input.cpp"
#include "Win32_Render.cpp"
#include "Win32_Audio.cpp"
#include "Win32_FileIO.cpp"

#if CODEFLOW_INTERNAL
#include "Win32_Debug.cpp"
#else
#include "Codeflow.cpp"
#endif


global_variable WINDOWPLACEMENT GlobalWindowPosition = {sizeof(GlobalWindowPosition)};

internal void
Win32ToggleFullscreen(HWND Window)
{
	DWORD Style = GetWindowLong(Window, GWL_STYLE);
	
	if (Style & WS_OVERLAPPEDWINDOW)
	{
		MONITORINFO MonitorInfo = {sizeof(MonitorInfo)};

		if (GetWindowPlacement(Window, &GlobalWindowPosition) &&
			GetMonitorInfo(MonitorFromWindow(Window, MONITOR_DEFAULTTOPRIMARY), &MonitorInfo))
		{
			SetWindowLong(Window, GWL_STYLE, Style & ~WS_OVERLAPPEDWINDOW);
			SetWindowPos(Window, HWND_TOP,
						 MonitorInfo.rcMonitor.left, MonitorInfo.rcMonitor.top,
						 MonitorInfo.rcMonitor.right - MonitorInfo.rcMonitor.left,
						 MonitorInfo.rcMonitor.bottom - MonitorInfo.rcMonitor.top,
						 SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
		}
	}
	else
	{
		SetWindowLong(Window, GWL_STYLE, Style | WS_OVERLAPPEDWINDOW);
		SetWindowPlacement(Window, &GlobalWindowPosition);
		SetWindowPos(Window, 0, 0, 0, 0, 0,
					 SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER |
					 SWP_NOOWNERZORDER | SWP_FRAMECHANGED);
	}
}


struct win32_params
{
	b32 Paused;
	b32 Running;
	win32_bitmap Bitmap;
	s64 PerfCountFrequency;
};

global_variable win32_params* GlobalParams = 0;

internal void
Win32GetExecutablePath(char* Buffer)
{
	u32 FilePathLength = GetModuleFileName(0, Buffer, MAX_PATH);

	char* FileName = Buffer;
	for (char* Scan = Buffer;
		*Scan;
		Scan++)
	{
		if (*Scan == '\\')
		{
			FileName = Scan + 1;
		}
	}
	*FileName = '\0';
}

internal void
Win32DrawRectangle(win32_bitmap* Bitmap, u32 X, u32 Y,
	u32 Width, u32 Height, u32 BitmapPixel)
{
	ClampU32(0, Width, Bitmap->Width);
	ClampU32(0, Height, Bitmap->Height);
	ClampU32(0, X, Width - 1);
	ClampU32(0, Y, Height - 1);

	for (u32 YIter = Y;
		YIter < Y + Height;
		YIter++)
	{
		u08* Pixel = (u08*)Bitmap->Memory +
			(YIter * Bitmap->Pitch) +
			(X * Bitmap->BytesPerPixel);
		for (u32 XIter = X;
			XIter < X + Width;
			XIter++)
		{
			if (XIter < Bitmap->Width && YIter < Bitmap->Height)
				*(u32*)Pixel = BitmapPixel;

			Pixel++;
		}
	}
}

inline void
Win32QueryPerfCountFrequency()
{
	LARGE_INTEGER ResultPerfCountFrequency;
	QueryPerformanceFrequency(&ResultPerfCountFrequency);
	GlobalParams->PerfCountFrequency = ResultPerfCountFrequency.QuadPart;
}

inline LARGE_INTEGER
Win32GetWallClock()
{
	LARGE_INTEGER Result;
	QueryPerformanceCounter(&Result);
	return(Result);
}

inline r32
Win32GetSecondsElapsed(LARGE_INTEGER Start, LARGE_INTEGER End)
{
	r32 Result = ((r32)(End.QuadPart - Start.QuadPart) / 
	(r32)GlobalParams->PerfCountFrequency);
	return(Result);
}

LRESULT CALLBACK
Win32MainWindowCallback(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	LRESULT Result = 0;

	if (GlobalParams)
	{
		switch (uMsg)
		{
		case WM_QUIT:
		case WM_CLOSE:
			{
				GlobalParams->Running = false;
			} break;

		case WM_SIZE:
		{
			RECT ClientRect = {};
			GetClientRect(hWnd, &ClientRect);
			s32 Width = ClientRect.right - ClientRect.left;
			s32 Height = ClientRect.bottom - ClientRect.top;
			Win32ResizeDIBSection(&GlobalParams->Bitmap, Width, Height);
		} break;

		case WM_PAINT:
			{
				PAINTSTRUCT Paint;
				HDC DeviceContext = BeginPaint(hWnd, &Paint);
				Win32UpdateWindow(DeviceContext, &GlobalParams->Bitmap);
				EndPaint(hWnd, &Paint);
			} break;

		default:
			{
				Result = DefWindowProc(hWnd, uMsg, wParam, lParam);
			}
		}
	}
	else
	{
		Result = DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	
	return(Result);
}

int __stdcall
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	//NOTE(Jeremy): Win32 Global Params
	win32_params Params = {};
	GlobalParams = &Params;

	u32 DesiredSchedulerMS = 1;
	b32 SleepIsGranular = (timeBeginPeriod(DesiredSchedulerMS) == TIMERR_NOERROR);
	
	//NOTE(Jeremy): Win32 MainWindow Class
	char* WindowName = "CodeFlow";
	WNDCLASS WndClass = {};
	WndClass.hInstance = hInstance;
	WndClass.lpfnWndProc = Win32MainWindowCallback;
	WndClass.lpszClassName = "Win32_CodeFlow";

	ATOM WndClassRegistered = RegisterClass(&WndClass);
	if(WndClassRegistered)
	{

		//NOTE(Jeremy): Win32 MainWindow
		HWND hWnd = CreateWindow(WndClass.lpszClassName,
		WindowName,
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		0,
		0,
		hInstance,
		0);

		if(hWnd)
		{

			//NOTE(Jeremy): Misc
			b32& Running = Params.Running;
			b32& Paused = Params.Paused;

			r32 FrameMs = 0;
			
#define MonitorRefreshHz 60
#define GameUpdateHz (MonitorRefreshHz / 2)
			r32 TargetSecondsPerFrame = 1.0f / (r32)GameUpdateHz;
			
			//NOTE(Jeremy): Rendering
			win32_bitmap* Bitmap = &Params.Bitmap;

			//NOTE(Jeremy): Input
			Win32InitXInput();
			
			keyboard Keyboard = {};
			Win32InitKeyboard(&Keyboard);
			
			controller Controllers[CONTROLLER_COUNT] = {};
			u32 ControllerCount = ArrayCount(Controllers);

			//NOTE(Jeremy): Audio
			win32_audio_buffer AudioBuffer = {};
			u16 Volume = 8000;
			u16 SamplesPerSecond = 48000;
			u16 BytesPerSample = sizeof(s16) * 2;
			Win32InitSound(&AudioBuffer, hWnd, SamplesPerSecond, 
			BytesPerSample, Volume, GameUpdateHz);
			Win32ClearAudioBuffer(&AudioBuffer);
			AudioBuffer.SecondaryBuffer->Play(0, 0, DSBPLAY_LOOPING);

			//NOTE(Jeremy): Time Counters
			Win32QueryPerfCountFrequency();
			LARGE_INTEGER LastCounter = Win32GetWallClock();
			
#if CODEFLOW_INTERNAL
			LPVOID BaseAddress = (LPVOID)Terabytes((u64)2);
#else
			LPVOID BaseAddress = 0;
#endif

			platform_api PlatformApi = {};
			PlatformApi.FreeFileMemory = (free_file_memory*)&Win32FreeFileMemory;
			PlatformApi.ReadEntireFile = (read_entire_file*)&Win32ReadEntireFile;
			PlatformApi.WriteEntireFile = (write_entire_file*)&Win32WriteEntireFile;

			//Game Memory
			game_memory GameMemory = {};
			GameMemory.PlatformApi = PlatformApi;
			GameMemory.Permament.Size = Megabytes(64);
			GameMemory.Transient.Size = Gigabytes((u64)1);
			
			u64 TotalSize = GameMemory.Permament.Size + GameMemory.Transient.Size;
			
			void* GameMemoryBaseAddress = VirtualAlloc(BaseAddress, (size_t)TotalSize, 
			MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
			GameMemory.Permament.Memory = GameMemoryBaseAddress;
			GameMemory.Transient.Memory = (u08*)GameMemoryBaseAddress + GameMemory.Permament.Size;
			GameMemory.IsValid = true;
			
#if CODEFLOW_INTERNAL
			//NOTE(Jeremy): Processor Cycles
			u64 LastCycleCount = __rdtsc();

			u32 DEBUGAudioTimeMarkerIndex = 0;
			debug_win32_audio_time_marker DEBUGAudioTimeMarkers[GameUpdateHz / 2] = {};
			debug_win32_audio_time_marker* DEBUGAudioTimeMarker = 0;

			debug_win32_game DebugWin32Game = {};
			debug_win32_state DebugWin32State = {};
#endif
			
			//Main Loop
			Running = true;
			while (Running)
			{
				//NOTE(Jeremy): Game Input
				input PlayerInput = {};
				PlayerInput.Keyboard = &Keyboard;
				PlayerInput.Controllers = Controllers;
				PlayerInput.ControllerCount = ControllerCount;

#if CODEFLOW_INTERNAL
				if (DEBUGWin32GameChanged(&DebugWin32Game))
				{
					DEBUGWin32UnloadGameCode(&DebugWin32Game);
					DEBUGWin32LoadGameCode(&DebugWin32Game);
				}

				if (DebugWin32State.InputRecordingIndex > 0)
				{
					if (!DebugWin32State.IsRecording)
					{
						DEBUGWin32SaveGameMemory(&DebugWin32State, &GameMemory);
						DEBUGWin32InitRecordInput(&DebugWin32State);
					}
					else
					{
						DEBUGWin32RecordInput(&DebugWin32State, &PlayerInput);
					}
				}
				else
				{
					DEBUGWin32EndRecordInput(&DebugWin32State);
				}
#endif

				for (u32 i = 0;
				i < 255;
				i++)
				{
					Win32UpdateKeyboardButtonState(&Keyboard, 
					i, Keyboard.Keys[i].Pressed);
				}
				
				//NOTE(Jeremy): Win32 Message Handling,
				// gets handled in Win32MainWindowCallback
				MSG Message = {};
				while (PeekMessage(&Message, hWnd, 0, 0, PM_REMOVE))
				{            
					switch(Message.message)
					{
						case WM_SYSKEYDOWN:
						case WM_SYSKEYUP:
						case WM_KEYDOWN:
						case WM_KEYUP:
						{
							u32 VKeyCode = (u32)Message.wParam;
							u32 IsDown = !(Message.lParam & (1 << 31));

							Win32UpdateKeyboardButtonState(&Keyboard, 
							VKeyCode, IsDown);
						} break;
						
						default:
						{
							TranslateMessage(&Message);
							DispatchMessage(&Message);
						} break;
					}
				}

				//NOTE(Jeremy): Input Handling
				Win32UpdateControllers(Controllers);

#if CODEFLOW_INTERNAL
				for (u32 i = '0';
					i <= '9';
					i++)
				{
					if (Keyboard.Ctrl->Pressed && Keyboard.Keys[i].JustPressed)
					{
						if (DebugWin32State.InputRecordingIndex <= 0)
						{
							DebugWin32State.InputRecordingIndex = i - '0';
						}
						else
						{
							DebugWin32State.InputRecordingIndex = 0;
						}
					}

					if (!Keyboard.Ctrl->Pressed && Keyboard.Keys[i].JustPressed)
					{
						if (DebugWin32State.InputPlaybackIndex <= 0)
						{
							DebugWin32State.InputPlaybackIndex = i - '0';
						}
						else
						{
							DebugWin32State.InputPlaybackIndex = 0;
						}
					}
				}

				if (DebugWin32State.InputPlaybackIndex > 0)
				{
					if (!DebugWin32State.IsPlaying)
					{
						DEBUGWin32InitPlaybackInput(&DebugWin32State);
					}
					else
					{
						if (DebugWin32State.InputFrame == 0)
						{
							DEBUGWin32ReloadGameMemory(&DebugWin32State, &GameMemory);
						}
						DEBUGWin32PlaybackInput(&DebugWin32State, &PlayerInput);
					}
				}
				else
				{
					DEBUGWin32EndPlaybackInput(&DebugWin32State);
				}
#endif

				//NOTE(Jeremy): Alt-F4 Kills Window
				if (Keyboard.Alt->Pressed && Keyboard.F4->JustReleased)
				{
					Running = false;
				}
				
				if (Keyboard.Space->JustPressed)
				{
					Paused = !Paused;
				}
				
				if (Keyboard.F11->JustPressed)
				{
					Win32ToggleFullscreen(hWnd);
				}
				
				if (!Paused)
				{

					//NOTE(Jeremy): Game Rendering
					offscreen_buffer OffscreenBuffer = {};
					OffscreenBuffer.Width = Bitmap->Width;
					OffscreenBuffer.Height = Bitmap->Height;
					OffscreenBuffer.BytesPerPixel = Bitmap->BytesPerPixel;
					OffscreenBuffer.Pitch = Bitmap->Pitch;
					OffscreenBuffer.Memory = Bitmap->Memory;

					GameMemory.FPS = 1 / FrameMs;

#if CODEFLOW_INTERNAL
					DebugWin32Game.Code.GameUpdateAndRender(&GameMemory, &OffscreenBuffer, &PlayerInput);
#else
					GameUpdateAndRender(&GameMemory, &OffscreenBuffer, &PlayerInput);
#endif
					
					if (SUCCEEDED(AudioBuffer.SecondaryBuffer->GetCurrentPosition(&AudioBuffer.PlayCursor, &AudioBuffer.WriteCursor)))
					{
						if (!AudioBuffer.IsValid)
						{
							AudioBuffer.RunningSampleIndex = AudioBuffer.WriteCursor / AudioBuffer.BytesPerSample;
							AudioBuffer.IsValid = true;
						}
						
						Win32UpdateAudio(&AudioBuffer);
						
						//NOTE(Jeremy): Game Audio
						sound_buffer SoundBuffer = {};
						SoundBuffer.SamplesPerSecond = AudioBuffer.SamplesPerSecond;
						SoundBuffer.SampleCount = AudioBuffer.BytesToWrite / AudioBuffer.BytesPerSample;
						SoundBuffer.Samples = AudioBuffer.Samples;

#if CODEFLOW_INTERNAL
						DEBUGAudioTimeMarker = &DEBUGAudioTimeMarkers[DEBUGAudioTimeMarkerIndex];
						DEBUGAudioTimeMarker->OutputPlayCursor = AudioBuffer.PlayCursor;
						DEBUGAudioTimeMarker->OutputWriteCursor = AudioBuffer.WriteCursor;
						DEBUGAudioTimeMarker->OutputLocation = AudioBuffer.ByteToLock;
						DEBUGAudioTimeMarker->OutputSizeInBytes = AudioBuffer.BytesToWrite;

						DebugWin32Game.Code.GameGetSoundSamples(&GameMemory, &SoundBuffer);
#else
						GameGetSoundSamples(&GameMemory, &SoundBuffer);
#endif
						Win32FillAudioBuffer(&AudioBuffer, &SoundBuffer);
					}
					else
					{
						AudioBuffer.IsValid = false;
					}

					//
					//NOTE(Jeremy): Debug Timing Handling
					//
					
					//NOTE(Jeremy): Time Counters
					LARGE_INTEGER GameCounter = Win32GetWallClock();
					
					r32 SecondsElapsedForWork = Win32GetSecondsElapsed(LastCounter, GameCounter);
					r32 SecondsElapsedForFrame = SecondsElapsedForWork;
					if (SecondsElapsedForFrame < TargetSecondsPerFrame)
					{
						if (SleepIsGranular)
						{
							DWORD SleepMS = (DWORD)(1000.0f * (TargetSecondsPerFrame - SecondsElapsedForFrame));
							Sleep(SleepMS);
						}
						else
						{
							while (SecondsElapsedForFrame < TargetSecondsPerFrame)
							{
								SecondsElapsedForFrame = Win32GetSecondsElapsed(LastCounter, Win32GetWallClock());
							}
						}
					}

#if CODEFLOW_INTERNAL
					//NOTE(Jeremy): Update Counters and Cycles
					u64 EndCycleCount = __rdtsc();
					u32 CyclesElapsed = (u32)(EndCycleCount - LastCycleCount);
					LastCycleCount = EndCycleCount;

					//NOTE(Jeremy): Audio Markers
					AudioBuffer.SecondaryBuffer->GetCurrentPosition(&DEBUGAudioTimeMarker->FlipPlayCursor,
					&DEBUGAudioTimeMarker->FlipWriteCursor);
#if 0
					DEBUGWin32DebugSyncDisplay(&Params.Bitmap, &AudioBuffer,
					DEBUGAudioTimeMarkers, ArrayCount(DEBUGAudioTimeMarkers),
					TargetSecondsPerFrame, DEBUGAudioTimeMarkerIndex - 1);
#endif
#endif

					HDC DeviceContext = GetDC(hWnd);
					Win32UpdateWindow(DeviceContext, &Params.Bitmap);
					ReleaseDC(hWnd, DeviceContext);

					LARGE_INTEGER EndCounter = Win32GetWallClock();
					FrameMs = Win32GetSecondsElapsed(LastCounter, EndCounter);

#if CODEFLOW_INTERNAL
					DEBUGAudioTimeMarkerIndex++;
					if (DEBUGAudioTimeMarkerIndex >= ArrayCount(DEBUGAudioTimeMarkers))
					{
						DEBUGAudioTimeMarkerIndex = 0;
					}
					r32 DEBUGFPS = 1 / Win32GetSecondsElapsed(LastCounter, EndCounter);
					r32 DEBUGMilliSeconds = 1000.0f * DEBUGFPS;
					r32 DEBUGMegaCyclesElapsed = ((r32)CyclesElapsed / (1000.0f * 1000.0f));

					char DEBUGTextBuffer[256] = {};
					_snprintf_s(DEBUGTextBuffer, sizeof(DEBUGTextBuffer), "%0.02f ms, \t%0.02f fps, \t%0.02f mc\n",
					DEBUGMilliSeconds, DEBUGFPS, DEBUGMegaCyclesElapsed);
					OutputDebugString(DEBUGTextBuffer);
#endif

					LastCounter = EndCounter;
				}
			}
		}
		else
		{
			//LOG: "Failed to create hWnd(Window)"
		}
	}
	else
	{
		//LOG: "Failed to register WndClass(Window Class)"
	}
	
	return (0);
}