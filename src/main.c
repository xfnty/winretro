#include <windows.h>

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

    for (MSG msg; GetMessageA(&msg, 0, 0, 0) != 0;)
    {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    DestroyWindow(window);
    UnregisterClassA(WNDCLASS_NAME, instance);
    ExitProcess(0);
}
