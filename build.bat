@echo off
setlocal enableextensions enabledelayedexpansion

if "%1"=="help" (
    echo build [config="debug"] [run]
    echo   config: "debug" / "dist"
    echo   output: "%~dp0out\<config>"
    exit /b 0
)

set "compile_start=%time%"

for /F "tokens=1,2 delims=#" %%a in ('"prompt #$H#$E# & echo on & for %%b in (1) do rem"') do set esc=%%b

for %%a in (%*) do set "%%a=1"
if not "%dist%"=="1" if not "%debug%"=="1" set "debug=1"
if "%dist%"=="1" set "buildtype=dist"
if "%debug%"=="1" set "buildtype=debug"
set "src=%~dp0src\"
set "out=%~dp0out\%buildtype%\"
set "tmp=%~dp0out\.temp\%buildtype%\"
set "libraries=kernel32 user32 comdlg32 comctl32 gdi32 shell32 opengl32"
set unity=%tmp%unity.c
set "exe=%out%winretro.exe"


echo ----- Building "%buildtype%" -----

mkdir %out% 2> nul
mkdir %tmp% 2> nul

where /q cl.exe || (
    echo !esc![1;91merror!esc![0m: "cl.exe" not found.
    echo.
    echo To fix the issue install Build Tools for Visual Studio and MSVC if you haven't already:
    echo !esc![1;30mhttps://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2022!esc![0m
    echo And use 64-bit Command Prompt:
    echo !esc![1;30mhttps://learn.microsoft.com/en-us/cpp/build/how-to-enable-a-64-bit-visual-cpp-toolset-on-the-command-line!esc![0m
    goto CompileFailed
)

set cflags=/nologo /options:strict /std:c11 /Wall /wd4711 /wd4820 /wd5045 /wd4152 /Oi- /GS- /fp:fast /I %src%.
set lflags=/nodefaultlib /entry:_start /subsystem:windows
if "%dist%"=="1" (
    set cflags=!cflags! /O2
) else (
    set cflags=!cflags! /Z7 /Ob2
    set lflags=!lflags! /debug
)
set links=
for %%a in (%libraries%) do set "links=%%a.lib !links!"

set compile=cl.exe !cflags! %unity% /Fe:%exe% /Fo:%unity%.obj /link !lflags! !links!

echo Generating unity.c
echo /*> %unity%
echo     buildtype: %buildtype%>> %unity%
echo     command: %compile%>> %unity%
echo */>> %unity%
echo.>> %unity%
for /r %src% %%c in (*.c) do (
    echo #include "%%c">> %unity%
)
echo int _fltused=1;>> %unity%

echo Compiling
%compile% || goto CompileFailed
set "compile_end=%time%"

echo.
for %%g in (%exe%) do ^
for /f "tokens=1,2" %%s in ('robocopy %%~dpg. %%~dpg. %%~nxg /l /nocopy /is /njh /njs /ndl /nc') do (
    echo Size: !esc![1;96m%%s!esc![0m bytes
)
for /f "tokens=1-4 delims=:.," %%a in ("%compile_start%") do set start_h=%%a&set /a start_m=100%%b %% 100&set /a start_s=100%%c %% 100&set /a start_ms=100%%d %% 100
for /f "tokens=1-4 delims=:.," %%a in ("%compile_end%") do set end_h=%%a&set /a end_m=100%%b %% 100&set /a end_s=100%%c %% 100&set /a end_ms=100%%d %% 100
set /a hours=%end_h%-%start_h%
set /a mins=%end_m%-%start_m%
set /a secs=%end_s%-%start_s%
set /a ms=%end_ms%-%start_ms%
if %ms% lss 0 set /a secs = %secs% - 1 & set /a ms = 100%ms%
if %secs% lss 0 set /a mins = %mins% - 1 & set /a secs = 60%secs%
if %mins% lss 0 set /a hours = %hours% - 1 & set /a mins = 60%mins%
if %hours% lss 0 set /a hours = 24%hours%
if 1%ms% lss 100 set ms=0%ms%
set /a totalsecs = %hours%*3600 + %mins%*60 + %secs%
echo Time: !esc![1;96m%totalsecs%.%ms%!esc![0m seconds

if "%run%"=="1" (
    echo ----- Running -----
    start /wait %exe% && if !ERRORLEVEL! NEQ 0 goto RunFailed
)

exit /b 0

:CompileFailed
echo !esc![1;91mCompilation failed.!esc![0m
exit /b 1

:RunFailed
echo ----- Failed with code !esc![1;91m!ERRORLEVEL!!esc![0m -----
exit /b 1
