#include "DefaultIncludes.h"

#include <windows.h>

#define WIN32_BITMAP_BITS_PER_PIXEL 32

struct win32_bitmap
{
	BITMAPINFO Info;
	u32 Width;
	u32 Height;
	u32 BytesPerPixel;
    u32 Pitch;
	void* Memory;
};

//
// NOTE(Jeremy): Win32 Functions
//

internal void
Win32ResizeDIBSection(win32_bitmap* Bitmap, u32 Width, u32 Height)
{
	if (Bitmap->Memory)
	{
		VirtualFree(Bitmap->Memory, 0, MEM_RELEASE);
	}

	Bitmap->Width = Width;
	Bitmap->Height = Height;
	Bitmap->BytesPerPixel = 4;
    Bitmap->Pitch = (Bitmap->Width * Bitmap->BytesPerPixel);

	Bitmap->Info.bmiHeader.biSize = sizeof(Bitmap->Info.bmiHeader);
	Bitmap->Info.bmiHeader.biWidth = Width;
	//NOTE(Jeremy): Negative biHeight indicates origin is top left corner
	//				and goes top-down
	Bitmap->Info.bmiHeader.biHeight = -((s32)Height);
	Bitmap->Info.bmiHeader.biPlanes = 1;
	Bitmap->Info.bmiHeader.biBitCount = WIN32_BITMAP_BITS_PER_PIXEL;
	Bitmap->Info.bmiHeader.biCompression = BI_RGB;

	u32 BitmapMemorySize = (Bitmap->Width * Bitmap->Height * Bitmap->BytesPerPixel);
	Bitmap->Memory = VirtualAlloc(0, BitmapMemorySize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
}

internal void
Win32UpdateWindow(HDC DeviceContext, win32_bitmap* Bitmap)
{
	StretchDIBits(DeviceContext,
		0, 0, Bitmap->Width, Bitmap->Height,
		0, 0, Bitmap->Width, Bitmap->Height,
		Bitmap->Memory, &Bitmap->Info,
		DIB_RGB_COLORS, SRCCOPY);
}