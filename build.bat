@echo off
setlocal enableextensions enabledelayedexpansion
pushd "%~dp0"

if "%1"=="dist" (
    set buildtype=%1
    set cflags=/O2 /fp:fast
    set lflags=/nodefaultlib
) else if "%1"=="" (
    set buildtype=debug
    set cflags=/Z7 /O2 /fp:fast
    set lflags=/nodefaultlib /debug
) else (
    echo Unknown build type "%1"
    exit /b 1
)

set out=out\%buildtype%
set tmp=out\.temp\%buildtype%

echo ----- Compiling "%buildtype%" -----
set start=%time%

if not exist %out% mkdir %out%
if not exist %tmp% mkdir %tmp%

echo int _fltused=1;> %tmp%\unity.c
for /r src %%c in (*.c) do (
    echo #include "%%c">> %tmp%\unity.c
)

cl.exe ^
/nologo /options:strict /Wall /Oi %cflags% ^
/D WIN32_LEAN_AND_MEAN /D NOMINMAX /D NOGDICAPMASKS /D NOSYSMETRICS /D NOMENUS /D NOICONS ^
/D NOKEYSTATES /D NOSYSCOMMANDS /D NORASTEROPS /D OEMRESOURCE /D NOATOM /D NOCLIPBOARD /D NOCOLOR ^
/D NOCTLMGR /D NODRAWTEXT /D NONLS /D NOMEMMGR /D NOMB /D NOMETAFILE /D NOOPENFILE /D NOSCROLL ^
/D NOSERVICE /D NOSOUND /D NOTEXTMETRIC /D NOWH /D NOWINOFFSETS /D NOCOMM /D NOKANJI /D NOHELP ^
/D NOPROFILER /D NODEFERWINDOWPOS /D NOMCX /D NOCRYPT /D NOIME ^
/D WINVER=_WIN32_WINNT_WIN7 /D _WIN32_WINNT=_WIN32_WINNT_WIN7 ^
/D _WIN32_WINNT_WIN10_TH2=0 /D _WIN32_WINNT_WIN10_RS1=0 /D _WIN32_WINNT_WIN10_RS2=0 ^
/D _WIN32_WINNT_WIN10_RS3=0 /D _WIN32_WINNT_WIN10_RS4=0 /D _WIN32_WINNT_WIN10_RS5=0 ^
/I %~dp0src ^
%tmp%\unity.c ^
/Fe:%out%\frontend.exe /Fo:%tmp%\unity.o ^
/link /entry:main /subsystem:windows %lflags% kernel32.lib user32.lib gdi32.lib
if %ERRORLEVEL% NEQ 0 goto failure
set end=%time%

echo.
for %%g in (%out%\frontend.exe) do ^
for /f "tokens=1,2" %%s in ('robocopy %%~dpg. %%~dpg. %%~nxg /l /nocopy /is /njh /njs /ndl /nc') do (
    echo Size: %%s bytes
)
for /f "tokens=1-4 delims=:.," %%a in ("%start%") do set start_h=%%a&set /a start_m=100%%b %% 100&set /a start_s=100%%c %% 100&set /a start_ms=100%%d %% 100
for /f "tokens=1-4 delims=:.," %%a in ("%end%") do set end_h=%%a&set /a end_m=100%%b %% 100&set /a end_s=100%%c %% 100&set /a end_ms=100%%d %% 100
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
echo Time: %totalsecs%.%ms% seconds

popd
exit /b 0

:failure
exit /b 1
popd
