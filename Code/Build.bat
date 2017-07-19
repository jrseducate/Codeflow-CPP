@echo off

IF NOT EXIST ..\Build MKDIR ..\Build
PUSHD ..\Build

SET DateTimeNow=%date:~7,2%-%date:~4,2%-%date:~10,4%_%time:~0,2%_%time:~3,2%_%time:~6,2%
SET DateTimeNow=%DateTimeNow: =0%
ECHO %DateTimeNow%

REM To prevent from embedding typedef data, add -GR- -EHa-
REM For building debugger data, add -Oi
REM For seeing all warnings, add -W4
REM For removing warning messaged, add -wd[warning_number]
REM For minimal rebuild -Zi -Gm
SET CompilerFlags=-nologo -Z7 -GR- -EHa- -Od -Oi -W4 -wd4505 -wd4189 -wd4281 -wd4201 -wd4100 -MT -FC
SET CustomFlags=-DCODEFLOW_INTERNAL=1

REM For compiling Windows XP, add [/link -subsystem:windows,5.1]
REM For removing unused references, add [/link -opt:ref]
SET LinkerFlags=-opt:ref -incremental:no

REM All of the libraries to be included
SET IncludeLibraries=user32.lib gdi32.lib winmm.lib

REM x64 or x86 Compile target
SET CompileTarget=x64

DEL *.pdb > NUL 2> NUL

call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" %CompileTarget%
CL %CompilerFlags% %CustomFlags% ..\Code\Codeflow.cpp %IncludeLibraries% -LD /link %LinkerFlags% /PDB:Codeflow_%DateTimeNow%.pdb /EXPORT:GameUpdateAndRender /EXPORT:GameGetSoundSamples

REM Build Call
call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" %CompileTarget%
CL %CompilerFlags% %CustomFlags% ..\Code\Win32_Codeflow.cpp %IncludeLibraries% /link %LinkerFlags%

POPD

ECHO Done!