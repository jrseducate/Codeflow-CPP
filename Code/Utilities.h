#include "TypeDefs.h"

#pragma once

#if CODEFLOW_INTERNAL
    #define Assert(Condition, ...) if (!(Condition)) {*(int *)0 = 0; }
#else
    #define Assert(Condition, ...) //{Condition;}
#endif

//TODO(Jeremy): Sort these into cleaner order
#define ArrayCount(Array) sizeof(Array) / sizeof(*(Array))
#define SecondsToMilliseconds(Time) ((u32)((r32)Time * 1000.0f))
#define Swap(A, B, Type) {Type Temp = A; A = B; B = Temp;}

#define Kilobytes(x) (x * 1024)
#define Megabytes(x) (Kilobytes(x) * 1024)
#define Gigabytes(x) (Megabytes(x) * (u64)1024)
#define Terabytes(x) (Gigabytes(x) * (u64)1024)

#define Min(A, B) ((A < B) ? A : B)
#define Max(A, B) ((A > B) ? A : B)

#define InitVarArgs(LastParameter, VarArgs) void* VarArgs; {VarArgs = (void*)((u64)&LastParameter + sizeof(LastParameter));}
inline void*
GetVarArg_(void** VarArgs, u64 Shift)
{
    void* Result = *VarArgs;
    void* NewArgs = (void*)((u64)(*VarArgs) + Shift);
    *VarArgs = NewArgs;
    return (Result);
}
#define GetVarArg(VarArgs, type) *(type*)GetVarArg_(&VarArgs, sizeof(type*));

//
//NOTE(Jeremy): String Functions
//

b32 StringsMatch(char* A, char* B)
{
	b32 Result = (A == B);
	if (!Result)
	{
		Result = *A == *B;
		while (Result && *A != 0)
		{
			A++;
			B++;
			Result = (*A == *B);
		}
	}

	return (Result);
}

b32 StringsMatch(char* A, char* B, u32 Length)
{
	b32 Result = (A == B);
	if (!Result)
	{
		Result = *A == *B;
        Length--;
		while (Result && *A != 0 && Length > 0)
		{
			A++;
			B++;
			Result = (*A == *B);
            Length--;
		}
	}

	return (Result);
}

b32 IsAlpha(char Val)
{
	return ((Val >= 'a' && Val <= 'z') || (Val >= 'A' && Val <= 'Z'));
}

b32 IsLowerCase(char Val)
{
	return (Val >= 'a' && Val <= 'z');
}

b32 IsUpperCase(char Val)
{
	return (Val >= 'A' && Val <= 'Z');
}

b32 IsNumeric(char Val)
{
	return (Val >= '0' && Val <= '9');
}

b32 StringIsAlpha(char* Val)
{
	b32 Result = true;
	
	while (Result && *Val)
	{
		Result = IsAlpha(*Val);
		Val++;
	}

	return (Result);
}

b32 StringIsNumeric(char* Val)
{
	b32 Result = true;

	while (Result && *Val)
	{
		Result = IsNumeric(*Val);
		Val++;
	}

	return (Result);
}

b32 StringIsAlphaNumeric(char* Val)
{
	b32 Result = true;

	while (Result && *Val)
	{
		Result = IsAlpha(*Val) || IsNumeric(*Val);

		Val++;
	}

	return (Result);
}

u32 StringLength(char* Val)
{
	u32 Length = 0;
	while (*Val)
	{
		Length++;
		Val++;
	}
	return (Length);
}