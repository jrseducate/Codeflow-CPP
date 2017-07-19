#include "TypeDefs.h"

#pragma once

//
// NOTE(Jeremy): Standard Math Functions
//

#define Absolute(Value) ((Value >= 0)? Value : -Value)

inline u16
PowU16(u16 Value, u16 Exp)
{
    Assert(Exp >= 0, "Exponent must be greater or equal to 0!");
    
    u16 Result = (Exp == 0) ? 1 : Value;
    for (u16 i = 1;
        i < Exp && Exp > 0;
        i++)
    {
        Result *= Value;
    }
    
    return (Result);
}

inline s16
PowS16(s16 Value, u16 Exp)
{
    Assert(Exp >= 0, "Exponent must be greater or equal to 0!");
    
    s16 Result = (Exp == 0) ? 1 : Value;
    for (u16 i = 1;
        i < Exp && Exp > 0;
        i++)
    {
        Result *= Value;
    }
    
    return (Result);
}

inline u32
PowU32(u32 Value, u32 Exp)
{
    Assert(Exp >= 0, "Exponent must be greater or equal to 0!");
    
    u32 Result = (Exp == 0) ? 1 : Value;
    for (u32 i = 1;
        i < Exp && Exp > 0;
        i++)
    {
        Result *= Value;
    }
    
    return (Result);
}

inline s32
PowS32(s32 Value, u32 Exp)
{
    Assert(Exp >= 0, "Exponent must be greater or equal to 0!");
    
    s32 Result = (Exp == 0) ? 1 : Value;
    for (u32 i = 1;
        i < Exp && Exp > 0;
        i++)
    {
        Result *= Value;
    }
    
    return (Result);
}

inline u64
PowU64(u64 Value, u64 Exp)
{
    Assert(Exp >= 0, "Exponent must be greater or equal to 0!");
    
    u64 Result = (Exp == 0) ? 1 : Value;
    for (u64 i = 1;
        i < Exp && Exp > 0;
        i++)
    {
        Result *= Value;
    }
    
    return (Result);
}

inline s64
PowS64(s64 Value, u64 Exp)
{
    Assert(Exp >= 0, "Exponent must be greater or equal to 0!");
    
    s64 Result = (Exp == 0) ? 1 : Value;
    for (u64 i = 1;
        i < Exp && Exp > 0;
        i++)
    {
        Result *= Value;
    }
    
    return (Result);
}

inline u16
SafeTruncateU32(u32 Value)
{
    Assert(Value <= MAX_U16, "Value too large to fit in u16!");
    u16 Result = (u16)Value;
    return (Result);
}

inline s16
SafeTruncateS32(s32 Value)
{
    Assert(Value <= MAX_S16 && Value >= MIN_S16, "Value too large or too small to fit in s16!");
    s16 Result = (s16)Value;
    return (Result);
}

inline u32
SafeTruncateU64(u64 Value)
{
    Assert(Value <= MAX_U32, "Value too large to fit in u32!");
    u32 Result = (u32)Value;
    return (Result);
}

inline s32
SafeTruncateS64(s64 Value)
{
    Assert(Value <= MAX_S32 && Value >= MIN_S32, "Value too large or too small to fit in s32!");
    s32 Result = (s32)Value;
    return (Result);
}

inline u16
ClampU16(u16 Min, u16 Val, u16 Max)
{
    u16 Result = Val;
	Result = (Val <= Min) ? Min : (Val >= Max) ? Max : Val;
    return (Result);
}

inline s16
ClampS16(s16 Min, s16 Val, s16 Max)
{
    s16 Result = Val;
	Result = (Val <= Min) ? Min : (Val >= Max) ? Max : Val;
    return (Result);
}

inline u32
ClampU32(u32 Min, u32 Val, u32 Max)
{
    u32 Result = Val;
	Result = (Val <= Min) ? Min : (Val >= Max) ? Max : Val;
    return (Result);
}

inline s32
ClampS32(s32 Min, s32 Val, s32 Max)
{
    s32 Result = Val;
	Result = (Val <= Min) ? Min : (Val >= Max) ? Max : Val;
    return (Result);
}

inline u64
ClampU64(u64 Min, u64 Val, u64 Max)
{
    s64 Result = Val;
	Result = (Val <= Min) ? Min : (Val >= Max) ? Max : Val;
    return (Result);
}

inline s64
ClampS64(s64 Min, s64 Val, s64 Max)
{
    s64 Result = Val;
	Result = (Val <= Min) ? Min : (Val >= Max) ? Max : Val;
    return (Result);
}

//
// NOTE(Jeremy): Custom Types
//

//
// NOTE(Jeremy): V2
//

struct v2
{
    union
    {
        struct
        {
            u32 X, Y;
        };
    };
};

//NOTE(Jeremy): V2 Constructors
inline v2
V2(u32 X, u32 Y)
{
    v2 Result = {};
    Result.X = X;
    Result.Y = Y;
    return (Result);
}

//NOTE(Jeremy): V2 Operators
inline bool operator==(const v2& A, const v2& B){ return ((A.X == B.X) && (A.Y == B.Y)); }
inline bool operator!=(const v2& A, const v2& B){ return !(A == B); }
inline v2 operator+(const v2& A, const v2& B){ return(V2(A.X + B.X, A.Y + B.Y)); }
inline v2 operator-(const v2& A, const v2& B){ return(V2(A.X - B.X, A.Y - B.Y)); }

//NOTE(Jeremy): V2 Functions

//
// NOTE(Jeremy): Rect2
//

struct rect2
{
    union
    {
        struct
        {
            v2 Pos;
            v2 Dim;
        };
    };
};

//NOTE(Jeremy): Rect2 Constructors
inline rect2
Rect2(u32 PosX, u32 PosY, u32 DimX, u32 DimY)
{
    rect2 Result = {};
    Result.Pos = V2(PosX, PosY);
    Result.Dim = V2(DimX, DimY);
    return (Result);
}

inline rect2
Rect2(v2 Pos, v2 Dim)
{
    rect2 Result = {};
    Result.Pos = Pos;
    Result.Dim = Dim;
    return (Result);
}

inline rect2
Rect2MinMax(u32 MinX, u32 MinY, u32 MaxX, u32 MaxY)
{
    rect2 Result = {};
    Result.Pos = V2(MinX, MinY);
    Result.Dim = V2(MinX + MaxX, MinY + MaxY);
    return (Result);
}

inline rect2
Rect2MinMax(v2 Min, v2 Max)
{
    rect2 Result = {};
    Result.Pos = Min;
    Result.Dim = Min + Max;
    return (Result);
}

//NOTE(Jeremy): Rect2 Operators
inline bool operator==(const rect2& A, const rect2& B){ return ((A.Pos == B.Pos) && (A.Dim == B.Dim)); }
inline bool operator!=(const rect2& A, const rect2& B){ return !(A == B); }

//NOTE(Jeremy): Rect2 Functions
inline v2
Rect2GetSize(rect2 Rect)
{
	v2 Result = Rect.Dim;
	return (Result);
}

inline v2
Rect2GetPosition(rect2 Rect)
{
	v2 Result = Rect.Pos;
	return (Result);
}

inline v2
Rect2RightCorner(rect2 Rect)
{
	v2 Result = Rect.Pos + V2(Rect.Dim.X, 0);
	return (Result);
}

inline v2
Rect2BottomCorner(rect2 Rect)
{
	v2 Result = Rect.Pos + V2(0, Rect.Dim.Y);
	return (Result);
}

inline v2
Rect2MaxCorner(rect2 Rect)
{
    v2 Result = Rect.Pos + Rect.Dim;
    return (Result);
}

inline b32
Rect2ContainsV2(rect2 Rect, v2 Point)
{
    b32 Result = false;
    
    v2 MinCorner = Rect.Pos;
    v2 MaxCorner = Rect2MaxCorner(Rect);
    
    Result = (Point.X >= MinCorner.X &&
            Point.X <= MaxCorner.X &&
            Point.Y >= MinCorner.Y &&
            Point.Y <= MaxCorner.Y);
    
    return (Result);
}

inline b32
Rect2ContainsRect2(rect2 A, rect2 B)
{
	b32 Result = false;
	
	Result = Rect2ContainsV2(A, Rect2GetPosition(B)) ||
			Rect2ContainsV2(A, Rect2MaxCorner(B)) ||
			Rect2ContainsV2(A, Rect2RightCorner(B)) ||
			Rect2ContainsV2(A, Rect2BottomCorner(B));

	return (Result);
}