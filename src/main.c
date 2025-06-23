#include "ui.h"
#include "core.h"
#include "common.h"
#include "miniwindows.h"

#define WNDCLASS_NAME "tiny-libretro-frontend"

typedef struct App App;
struct App {
    Core *core;
    HWND window;
    HINSTANCE instance;
};

LRESULT CALLBACK WindowEventHandler(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    App *app = (App*)GetWindowLongPtrA(hwnd, GWLP_USERDATA);
    if (!app || app->window != hwnd)
    {
        return DefWindowProcA(hwnd, msg, wp, lp);
    }

    switch (msg)
    {
    case WM_CLOSE:
    case WM_DESTROY:
        PostQuitMessage(0);
        return 1;

    case WM_SIZE:
        Ui_Resize(LOWORD(lp), HIWORD(lp));
        break;
    }

    return DefWindowProcA(hwnd, msg, wp, lp);
}

void main(void)
{
    static App app;

    InitDebugOutput();

    app.instance = GetModuleHandleA(0);
    WNDCLASSA class = {
        .style = CS_OWNDC,
        .hInstance = app.instance,
        .lpszClassName = WNDCLASS_NAME,
        .lpfnWndProc = WindowEventHandler,
        .hCursor = LoadCursorA(0, IDC_ARROW),
        .hbrBackground = GetStockObject(WHITE_BRUSH),
    };
    assert(RegisterClassA(&class));
    app.window = CreateWindowExA(
        0,
        WNDCLASS_NAME,
        "Tiny Libretro Frontend",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
        0,
        0,
        app.instance,
        0
    );
    assert(app.window);
    SetWindowLongPtrA(app.window, GWLP_USERDATA, (LONG_PTR)&app);
    Ui_Init(app.window, app.instance);
    ShowWindow(app.window, SW_SHOW);

    // char file[260];
    // OPENFILENAMEA open_dialog = {
    //     .lStructSize = sizeof(open_dialog),
    //     .hwndOwner = app.window,
    //     .lpstrFilter = "Libretro Core (*.dll)\0*.DLL\0All Files (*.*)\0*.*\0",
    //     .nFilterIndex = 1,
    //     .lpstrFile = file,
    //     .nMaxFile = sizeof(file),
    //     .Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST,
    // };
    // if (GetOpenFileNameA(&open_dialog) && (app.core = Core_Load(open_dialog.lpstrFile)))
    // {
    //     info("core: %s", app.core->name);
    // }
    // else
    // {
    //     error("OFN: %x, last error: %d", CommDlgExtendedError(), GetLastError());
    // }

    for (MSG msg; GetMessageA(&msg, 0, 0, 0) != 0;)
    {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }

    Core_Free(&app.core);
    ExitProcess(0);
}
