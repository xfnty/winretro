#include "common.h"
#include "miniwindows.h"

#define WNDCLASS_NAME "tiny-libretro-frontend"

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
    InitDebugOutput();

    volatile int e;
    HINSTANCE instance = GetModuleHandleA(0);
    WNDCLASSA class = {
        .style = CS_OWNDC,
        .hInstance = instance,
        .lpszClassName = WNDCLASS_NAME,
        .lpfnWndProc = WindowEventHandler,
        .hCursor = LoadCursorA(0, IDC_ARROW),
        .hbrBackground = GetStockObject(WHITE_BRUSH),
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
    e = GetLastError();
    assert(window);
    ShowWindow(window, SW_SHOW);

    char file[260];
    OPENFILENAMEA open_dialog = {
        .lStructSize = sizeof(open_dialog),
        .hwndOwner = window,
        .lpstrFilter = "Libretro Core (*.dll)\0*.DLL\0All Files (*.*)\0*.*\0",
        .nFilterIndex = 1,
        .lpstrFile = file,
        .nMaxFile = sizeof(file),
        .Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST,
    };
    if (GetOpenFileNameA(&open_dialog) && Core_Load(open_dialog.lpstrFile))
    {
        info("core: %s", Core_GetName());
    }
    else
    {
        error("OFN: %x, last error: %d", CommDlgExtendedError(), GetLastError());
    }

    for (MSG msg; GetMessageA(&msg, 0, 0, 0) != 0;)
    {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    ExitProcess(0);
}
