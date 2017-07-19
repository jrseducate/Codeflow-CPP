#include "DefaultIncludes.h"

#pragma once

internal void
ClearScreen(offscreen_buffer* Bitmap, color BitmapPixel)
{
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
			u08 R = (u08)(BitmapPixel.R * 255);
			u08 G = (u08)(BitmapPixel.G * 255);
			u08 B = (u08)(BitmapPixel.B * 255);
			u08 A = (u08)(BitmapPixel.A * 255);

			*Pixel = (R << 16) |
					(G << 8) |
					(B << 0) |
					(A << 24);
			Pixel++;
		}
		Row += Pitch;
	}
}

internal void
RenderBitmap(offscreen_buffer* Buffer, bitmap Bitmap, s32 X, s32 Y, u32 Scale, color Color = {1, 1, 1, 1})
{
	u32 BitmapWidth = Bitmap.Width * Scale;
	u32 BitmapHeight = Bitmap.Height * Scale;

	rect2 ScreenRect = Rect2MinMax(V2(0, 0), V2(Buffer->Width, Buffer->Height));
	rect2 BitmapRect = Rect2(V2(X, Y), V2(BitmapWidth, BitmapHeight));

	if (Rect2ContainsRect2(ScreenRect, BitmapRect))
	{

		v2 BitmapOffset = V2((X < 0) ? (-X) : (0),
							(Y < 0) ? (-Y) : (0));

		color* BitmapPixels = (color*)Bitmap.Pixels;
		u32 BitmapPitch = Bitmap.Width;

		for (u32 BufferY = (u32)Max(0, Y), BitmapYIter = BitmapOffset.Y;
			BufferY < (u32)Min(Rect2MaxCorner(BitmapRect).Y, Buffer->Height);
			BufferY++, BitmapYIter++)
		{
			for (u32 BufferX = (u32)Max(0, X), BitmapXIter = BitmapOffset.X;
				BufferX < (u32)Min(Rect2MaxCorner(BitmapRect).X, Buffer->Width);
				BufferX++, BitmapXIter++)
			{
				u08* Pixel = (u08*)Buffer->Memory +
					(BufferY * Buffer->Pitch) +
					(BufferX * Buffer->BytesPerPixel);

				u32 BitmapX = ((BitmapXIter - (BitmapXIter % Scale)) / Scale);
				u32 BitmapY = ((BitmapYIter - (BitmapYIter % Scale)) / Scale);

				//
				// Note(Jeremy): Modulating the (bitmap)'s colors by the (color) _parameter_
				//

				color BitmapPixel = (*(BitmapPixels + (BitmapY * BitmapPitch) + (BitmapX)));
				BitmapPixel = ColorModulate(BitmapPixel, Color);

				//
				// Note(Jeremy): Modulating the (buffer)'s colors by the (bitmap)'s colors
				//

#if 0
				color BufferPixel = *(color*)Pixel;
				BufferPixel.R *= BufferPixel.A;
				BufferPixel.G *= BufferPixel.A;
				BufferPixel.B *= BufferPixel.A;
				BufferPixel.A *= BufferPixel.A;

				BitmapPixel.R *= BitmapPixel.A;
				BitmapPixel.G *= BitmapPixel.A;
				BitmapPixel.B *= BitmapPixel.A;
				BitmapPixel.A *= BitmapPixel.A;

				BufferPixel.R *= BitmapPixel.R;
				BufferPixel.G *= BitmapPixel.G;
				BufferPixel.B *= BitmapPixel.B;
				BufferPixel.A *= BitmapPixel.A;

				BufferPixel.R /= BufferPixel.A;
				BufferPixel.G /= BufferPixel.A;
				BufferPixel.B /= BufferPixel.A;
				BufferPixel.A /= BufferPixel.A;
#else
				color BufferPixel = U32ToColor(*(u32*)Pixel);
				color BufferPixelBefore = BufferPixel;

				r32 NewAlpha = 1 - (1 - BitmapPixel.A) * (1 - BufferPixel.A);
				BufferPixel.R = (BitmapPixel.R * BitmapPixel.A / NewAlpha) + 
								(BufferPixel.R * BufferPixel.A * (1 - BitmapPixel.A) / NewAlpha);
				BufferPixel.G = (BitmapPixel.G * BitmapPixel.A / NewAlpha) +
								(BufferPixel.G * BufferPixel.A * (1 - BitmapPixel.A) / NewAlpha);
				BufferPixel.B = (BitmapPixel.B * BitmapPixel.A / NewAlpha) +
								(BufferPixel.B * BufferPixel.A * (1 - BitmapPixel.A) / NewAlpha);
				BufferPixel.A = NewAlpha;

#endif



				//
				// Note(Jeremy): Updating (buffer) with (buffer)
				//
#if 1
				u08 R = (u08)BufferPixel.R * 255;
				u08 G = (u08)BufferPixel.G * 255;
				u08 B = (u08)BufferPixel.B * 255;
				u08 A = (u08)BufferPixel.A * 255;
#else
				u08 R = (u08)BitmapPixel.R * 255;
				u08 G = (u08)BitmapPixel.G * 255;
				u08 B = (u08)BitmapPixel.B * 255;
				u08 A = (u08)BitmapPixel.A * 255;
#endif

				*(u32*)Pixel = (R << 16) |
					(G << 8) |
					(B << 0) |
					(A << 24);
			}
		}
	}
}

internal void
RenderText(offscreen_buffer* Buffer, char* Text, s32 X, s32 Y, u32 Scale, color Color = {1, 1, 1, 1}, u32 Padding = 5)
{
	u32 XIter = X;
	u32 YIter = Y;

	for (char* TextIter = Text;
		*TextIter;
		TextIter++)
	{
		bitmap Bitmap = {};
		b32 NewLine = false;

		if (IsNumeric(*TextIter))
		{
			u32 Index = *TextIter - '0';
			Bitmap = Number_Bitmaps[Index];
		}
		else if (IsLowerCase(*TextIter))
		{
			u32 Index = *TextIter - 'a';
			Bitmap = Letter_Bitmaps[Index];
		}
		else if (IsUpperCase(*TextIter))
		{
			u32 Index = *TextIter - 'A';
			Bitmap = Letter_Bitmaps[Index];
		}
		else
		{
			Bitmap = {};
		}

		RenderBitmap(Buffer, Bitmap, XIter, YIter, Scale, Color);

		char* NextChar = TextIter + 1;
		switch(*NextChar)
		{
			case ('\n'):
			{
				YIter += (Bitmap.Height * Scale) + Padding;
				XIter = X;
			} break;

			case (' '):
			{
				XIter += (2 * (Bitmap.Width * Scale) + Padding);
			} break;

			default:
			{
				XIter += (Bitmap.Width * Scale) + Padding;
			} break;
		}
	}
}