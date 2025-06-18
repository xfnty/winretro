@echo off
pushd "%~dp0"

if "%1"=="dist" (
    set buildtype=%1
    set cflags=/O2 /fp:fast
    set lflags=/nodefaultlib
) else if "%1"=="release" (
    set buildtype=%1
    set cflags=/O2 /fp:fast
    set lflags=/debug
) else if "%1"=="" (
    set buildtype=debug
    set cflags=/Od /GS /RTC1
    set lflags=/debug ucrtd.lib msvcrtd.lib vcruntimed.lib
) else (
    echo Unknown build type "%1"
    exit /b 1
)

set out=out\%buildtype%
set tmp=out\.temp\%buildtype%

echo ----- Running "%buildtype%" -----
%out%\frontend.exe %*
if not errorlevel 0 echo ----- Exit code %ERRORLEVEL% -----

popd
