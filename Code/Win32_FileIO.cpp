#include "DefaultIncludes.h"

#include <windows.h>

void Win32FreeFileMemory(read_file_result* FileMemory)
{
    if (FileMemory->Memory)
    {
        VirtualFree(FileMemory->Memory, 0, MEM_RELEASE);
    }
}

read_file_result Win32ReadEntireFile(char* FileLocation)
{
    read_file_result Result = {};
	void* Memory;
    
    HANDLE FileHandle = CreateFile(FileLocation, GENERIC_READ, FILE_SHARE_READ, 
        0, OPEN_EXISTING, 0, 0);
    if (FileHandle != INVALID_HANDLE_VALUE)
    {
        LARGE_INTEGER FileSize;
        if (GetFileSizeEx(FileHandle, &FileSize))
        {
            u32 FileSize32 = SafeTruncateU64(FileSize.QuadPart);
			Memory = VirtualAlloc(0, FileSize32, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
            if (Memory)
            {
                DWORD BytesRead;
				if (ReadFile(FileHandle, Memory, FileSize32, &BytesRead, 0)
					&& (FileSize32 == BytesRead))
                {
					Result.Memory = Memory;
					Result.Size = FileSize32;
                }
                else
                {
                    //NOTE(Jeremy): Failed to read file!
					Win32FreeFileMemory(&Result);
                    Result.Memory = 0;
                    Result.Size = 0;
                }
			}
		}
		CloseHandle(FileHandle);
    }
    
    return (Result);
}

b32 Win32WriteFromEndOfFile(char* FileLocation, void* Memory, u64 MemorySize)
{
	b32 Result = false;

    HANDLE FileHandle = CreateFile(FileLocation, FILE_APPEND_DATA, 0,
		0, OPEN_ALWAYS, 0, 0);
    if (FileHandle != INVALID_HANDLE_VALUE)
    {
        DWORD BytesWritten = 0;
        u32 MemorySize32 = SafeTruncateU64(MemorySize);
        if (WriteFile(FileHandle, Memory, MemorySize32, &BytesWritten, 0)
            && (MemorySize32 == BytesWritten))
        {
            Result = true;
        }
        CloseHandle(FileHandle);
    }

	return (Result);
}

b32 Win32WriteEntireFile(char* FileLocation, void* Memory, u64 MemorySize)
{
	b32 Result = false;

    HANDLE ExistingFileHandle = CreateFileA(FileLocation, GENERIC_READ, FILE_SHARE_READ, 
    0, OPEN_EXISTING, 0, 0);
    if (ExistingFileHandle == INVALID_HANDLE_VALUE)
    {
        //NOTE(Jeremy): File doesn't exist, just create one
        HANDLE NewFileHandle = CreateFileA(FileLocation, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
        if (NewFileHandle != INVALID_HANDLE_VALUE)
        {
            DWORD BytesWritten = 0;
            u32 MemorySize32 = SafeTruncateU64(MemorySize);
            if (WriteFile(NewFileHandle, Memory, MemorySize32, &BytesWritten, 0)
                && (MemorySize32 == BytesWritten))
            {
                Result = true;
            }
            CloseHandle(NewFileHandle);
        }
    }
    else
    {
        CloseHandle(ExistingFileHandle);
        //NOTE(Jeremy): File does exist, create temp one then switch
        char TempFileName[256];
        _snprintf_s(TempFileName, sizeof(TempFileName), "%s.tmp", FileLocation);
        HANDLE TempFileHandle = CreateFileA(TempFileName, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
        if (TempFileHandle != INVALID_HANDLE_VALUE)
        {
            DWORD BytesWritten = 0;
            u32 MemorySize32 = SafeTruncateU64(MemorySize);
            if (WriteFile(TempFileHandle, Memory, MemorySize32, &BytesWritten, 0)
                && (MemorySize32 == BytesWritten))
            {
                Result = true;
            }
            CloseHandle(TempFileHandle);
            
            MoveFileEx(TempFileName, FileLocation, MOVEFILE_REPLACE_EXISTING);
        }
    }

	return (Result);
}