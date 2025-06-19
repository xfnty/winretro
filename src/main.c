#include <windows.h>
#include <commdlg.h>

#include "common.h"

#define WNDCLASS_NAME "tiny-libretro-frontend"

static HANDLE stdout;

LRESULT CALLBACK WindowEventHandler(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    switch (msg)
    {
    case WM_CLOSE:
    case WM_DESTROY:
        PostQuitMessage(0);
        return 1;
    }

    return DefWindowProcA(hwnd, msg, wp, lp);
}

void main(void)
{
    HINSTANCE instance = GetModuleHandle(0);
    WNDCLASSA class = {
        .style = CS_OWNDC,
        .hInstance = instance,
        .lpszClassName = WNDCLASS_NAME,
        .lpfnWndProc = WindowEventHandler,
        .hCursor = LoadCursor(0, IDC_ARROW),
        .hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH),
    };
    assert(RegisterClassA(&class));
    HWND window = CreateWindowExA(
        0,
        WNDCLASS_NAME,
        "Tiny Libretro Frontend",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
        0,
        0,
        instance,
        0
    );
    assert(window);
    ShowWindow(window, SW_SHOW);

    char file[260] = {0};
    OPENFILENAMEA open_dialog = {
        .lStructSize = sizeof(open_dialog),
        .hwndOwner = window,
        .lpstrFilter = ".DLL",
        .lpstrFile = file,
        .nMaxFile = sizeof(file),
        .Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST,
    };
    if (GetOpenFileNameA(&open_dialog))
    {
        Core_Load(open_dialog.lpstrFile);
    }

    for (MSG msg; GetMessageA(&msg, 0, 0, 0) != 0;)
    {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    DestroyWindow(window);
    UnregisterClassA(WNDCLASS_NAME, instance);
    ExitProcess(0);
}
