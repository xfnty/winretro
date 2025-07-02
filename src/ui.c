#include "ui.h"

#define WINDOW_CLASS_NAME                    "winretro"
#define RENDER_WINDOW_CLASS_NAME             "winretro-render"

#define MENU_OPEN_CORE_STR    "Open Core"
#define MENU_OPEN_ROM_STR     "Open ROM"
#define MENU_LOAD_STATE_STR   "Load State"
#define MENU_SAVE_STATE_STR   "Save State"
#define MENU_EXIT_STR         "Exit"
#define MENU_OPEN_WEBSITE_STR "GitHub Repository"
#define MENU_OPEN_CORE_ID     0
#define MENU_OPEN_ROM_ID      1
#define MENU_LOAD_STATE_ID    2
#define MENU_SAVE_STATE_ID    3
#define MENU_EXIT_ID          4
#define MENU_OPEN_WEBSITE_ID  5

g_ui_t g_ui;

static i64 WINAPI window_event_handler(ptr hwnd, u32 msg, u64 wp, i64 lp);
static u8 open_file_dialog(u8 save, cstr title, cstr filename, cstr extensions, c8 *path, u32 pathmax);

void init_ui(void)
{
    g_ui.instance = GetModuleHandleA(0);

    ptr file_menu = CreateMenu();
    assert(AppendMenuA(file_menu, 0, MENU_OPEN_CORE_ID,  MENU_OPEN_CORE_STR));
    assert(AppendMenuA(file_menu, 0, MENU_OPEN_ROM_ID,   MENU_OPEN_ROM_STR));
    assert(AppendMenuA(file_menu, 0, MENU_LOAD_STATE_ID, MENU_LOAD_STATE_STR));
    assert(AppendMenuA(file_menu, 0, MENU_SAVE_STATE_ID, MENU_SAVE_STATE_STR));
    assert(AppendMenuA(file_menu, MF_SEPARATOR, 0, 0));
    assert(AppendMenuA(file_menu, 0, MENU_EXIT_ID, "Exit"));
    ptr help_menu = CreateMenu();
    assert(AppendMenuA(help_menu, 0, MENU_OPEN_WEBSITE_ID, MENU_OPEN_WEBSITE_STR));
    g_ui.menu = CreateMenu();
    assert(AppendMenuA(g_ui.menu, MF_POPUP, (u64)file_menu, "File"));
    assert(AppendMenuA(g_ui.menu, MF_POPUP, (u64)help_menu, "Help"));

    ModifyMenuA(g_ui.menu, MENU_OPEN_ROM_ID,   MF_DISABLED, MENU_OPEN_ROM_ID,   MENU_OPEN_ROM_STR);
    ModifyMenuA(g_ui.menu, MENU_LOAD_STATE_ID, MF_DISABLED, MENU_LOAD_STATE_ID, MENU_LOAD_STATE_STR);
    ModifyMenuA(g_ui.menu, MENU_SAVE_STATE_ID, MF_DISABLED, MENU_SAVE_STATE_ID, MENU_SAVE_STATE_STR);

    WNDCLASSA window_class = {
        .hInstance = g_ui.instance,
        .lpszClassName = WINDOW_CLASS_NAME,
        .lpfnWndProc = window_event_handler,
        .hCursor = LoadCursorA(0, IDC_ARROW),
    };
    assert(RegisterClassA(&window_class));
    g_ui.window = CreateWindowExA(
        0,
        WINDOW_CLASS_NAME,
        "Tiny Libretro Frontend",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
        0,
        g_ui.menu,
        g_ui.instance,
        0
    );
    assert(g_ui.window);

    WNDCLASSA render_window_class = {
        .style = CS_OWNDC,
        .hInstance = g_ui.instance,
        .lpszClassName = RENDER_WINDOW_CLASS_NAME,
        .lpfnWndProc = DefWindowProcA,
        .hCursor = LoadCursorA(0, IDC_ARROW),
        .hbrBackground = GetStockObject(WHITE_BRUSH),
    };
    assert(RegisterClassA(&render_window_class));
    g_ui.render_window = CreateWindowExA(
        0,
        RENDER_WINDOW_CLASS_NAME,
        0,
        WS_CHILD | WS_VISIBLE,
        0, 0, 0, 0,
        g_ui.window,
        0,
        g_ui.instance,
        0
    );
    assert(g_ui.window);
    
    ShowWindow(g_ui.window, SW_SHOW);
}

void process_ui_events(void)
{
    for (MSG msg; PeekMessageA(&msg, g_ui.window, 0, 0, PM_REMOVE) != 0;)
    {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
}

void free_ui(void)
{
    DestroyWindow(g_ui.render_window);
    DestroyWindow(g_ui.window);
    UnregisterClassA(RENDER_WINDOW_CLASS_NAME, g_ui.instance);
    UnregisterClassA(WINDOW_CLASS_NAME, g_ui.instance);
    RtlZeroMemory(&g_ui, sizeof(g_ui));
}

i64 WINAPI window_event_handler(ptr hwnd, u32 msg, u64 wp, i64 lp)
{
    if (hwnd != g_ui.window)
        return DefWindowProcA(hwnd, msg, wp, lp);

    c8 path[256];

    switch (msg)
    {
    case WM_CLOSE:
        ShowWindow(g_ui.window, SW_HIDE);
        g_ui.was_exit_requested = true;
        break;

    case WM_SIZE:
        RECT wr;
        assert(GetClientRect(g_ui.window, &wr));
        RECT r = {
            0, 0, wr.right, wr.bottom,
        };
        assert(SetWindowPos(g_ui.render_window, 0, r.left, r.top, r.right, r.bottom, 0));
        break;

    case WM_COMMAND:
        switch ((u16)wp)
        {
        case MENU_EXIT_ID:
            SendMessageA(g_ui.window, WM_CLOSE, 0, 0);
            break;

        case MENU_OPEN_CORE_ID:
            if (open_file_dialog(false, MENU_OPEN_CORE_STR, 0, "Libretro Core (*.dll)\0*.DLL\0", path, sizeof(path)))
                ;
            break;

        case MENU_OPEN_ROM_ID:
            if (open_file_dialog(false, MENU_OPEN_ROM_STR, 0, "Game Rom (*.chd)\0*.CHD\0Game Rom (*.bin)\0*.BIN\0All Files (*.*)\0*.*\0", path, sizeof(path)))
                ;
            break;

        case MENU_LOAD_STATE_ID:
            if (open_file_dialog(false, MENU_LOAD_STATE_STR, 0, "Game State (*.bin)\0*.BIN\0All Files (*.*)\0*.*\0", path, sizeof(path)))
                ;
            break;

        case MENU_SAVE_STATE_ID:
            if (open_file_dialog(true, MENU_SAVE_STATE_STR, "save.bin", "Game State (*.bin)\0*.BIN\0All Files (*.*)\0*.*\0", path, sizeof(path)))
                ;
            break;

        case MENU_OPEN_WEBSITE_ID:
            ShellExecuteA(0, "open", "https://github.com/xfnty/winretro", 0, 0, SW_SHOWNORMAL);
            break;
        }
        break;
    }
    
    return DefWindowProcA(hwnd, msg, wp, lp);
}

u8 open_file_dialog(u8 save, cstr title, cstr filename, cstr extensions, c8 *path, u32 pathmax)
{
    path[0] = '\0';
    if (filename) snprintf(path, pathmax, "%s", filename);

    OPENFILENAMEA info = {
        .lStructSize = sizeof(info),
        .lpstrTitle = title,
        .hwndOwner = g_ui.window,
        .lpstrFilter = extensions,
        .nFilterIndex = 1,
        .lpstrFile = path,
        .nMaxFile = pathmax,
        .Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | ((save) ? (OFN_OVERWRITEPROMPT) : (0)),
    };

    return (u8)((save) ? (GetSaveFileNameA(&info)) : (GetOpenFileNameA(&info)));
}
