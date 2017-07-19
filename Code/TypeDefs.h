#include <stdint.h>
#include <stddef.h>
#include <limits.h>
#include <float.h>

#pragma once

//
// NOTE(Jeremy): Constants
//

#define MIN_S08 (-MAX_S08 - 1)
#define MAX_S08 (127)
#define MIN_U08 (0)
#define MAX_U08 (256)

#define MIN_S16 (-MAX_S16 - 1)
#define MAX_S16 (32767)
#define MIN_U16 (0)
#define MAX_U16 (65535)

#define MIN_S32 (-MAX_S32 - 1)
#define MAX_S32 (2147483647)
#define MIN_U32 (0)
#define MAX_U32 (4294967295)

#define MIN_S64 (-MAX_S64 - 1)
#define MAX_S64 (9223372036854775807)
#define MIN_U64 (0)
#define MAX_U64 (18446744073709551615)

#define PI32 (3.14159265359f)

//
// NOTE(Jeremy): Standard Typedefs
//

#define internal static
#define local_persist static
#define global_variable static

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef int32 bool32;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

typedef float real32;
typedef double real64;

typedef intptr_t intptr;
typedef uintptr_t uintptr;

typedef size_t memory_index;

typedef int8 s8;
typedef int8 s08;
typedef int16 s16;
typedef int32 s32;
typedef int64 s64;
typedef bool32 b32;

typedef uint8 u8;
typedef uint8 u08;
typedef uint16 u16;
typedef uint32 u32;
typedef uint64 u64;

typedef real32 r32;
typedef real64 r64;