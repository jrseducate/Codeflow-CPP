#include "DefaultIncludes.h"

struct color
{
	r32 R;
	r32 G;
	r32 B;
	r32 A;
};

inline color
Color(r32 R, r32 G, r32 B, r32 A)
{
	color Result = {};

	Result.R = R;
	Result.G = G;
	Result.B = B;
	Result.A = A;

	return (Result);
}

inline color
ColorModulate(color A, color B)
{
	color Result = A;

	Result.R *= B.R;
	Result.G *= B.G;
	Result.B *= B.B;
	Result.A *= B.A;

	return(Result);

}

#define ColorRGB(R, G, B, A) Color(((r32)R / 255), ((r32)G / 255), ((r32)B / 255), ((r32)A / 255))

inline color
U32ToColor(u32 Val)
{
	u08* ValPtr = (u08*)&Val;
	color Result = ColorRGB(ValPtr[2],
		ValPtr[1],
		ValPtr[0],
		ValPtr[3]);
	return (Result);
}

struct bitmap
{
	u32* Pixels;
	u16 Width;
	u16 Height;
};

#include "AsciiBitmaps.h"