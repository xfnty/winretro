Compile using MSVC in 64-bit mode (debug, release):
```bat
cl.exe /nologo /std:c11 /Wall /wd4711 /wd4820 /wd5045 /Oi- /Ob2 /GS- /fp:fast /Z7 winretro.c kernel32.lib user32.lib gdi32.lib opengl32.lib comdlg32.lib shell32.lib /Fe:winretro.exe /Fo:winretro.obj /link /debug /entry:_start /nodefaultlib /subsystem:windows /INCREMENTAL:NO
cl.exe /nologo /std:c11 /Wall /wd4711 /wd4820 /wd5045 /Oi- /O2  /GS- /fp:fast winretro.c kernel32.lib user32.lib gdi32.lib opengl32.lib comdlg32.lib shell32.lib /Fe:winretro.exe /Fo:winretro.obj /link /entry:_start /nodefaultlib /subsystem:windows /INCREMENTAL:NO
```

Compile using MinGW (debug, release):
```bat
gcc -std=c99 -Wall -Wpedantic -ffreestanding -nostdlib -nostartfiles -O2 -fno-inline -g -ffunction-sections -Wl,--gc-sections -Wl,-e_start winretro.c -o winretro.exe -lkernel32 -luser32 -lgdi32 -lopengl32 -lcomdlg32 -lshell32
gcc -std=c99 -Wall -Wpedantic -ffreestanding -nostdlib -nostartfiles -O2 -ffunction-sections -Wl,-s -Wl,--gc-sections -Wl,-e_start winretro.c -o winretro.exe -lkernel32 -luser32 -lgdi32 -lopengl32 -lcomdlg32 -lshell32
```

Run:
```bat
start /b /wait winretro.exe
```

Clean:
```bat
del /q *.exe *.ilk *.obj *.pdb *.rdi
```
