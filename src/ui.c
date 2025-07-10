#include "ui.h"

#include "error.h"
#include "windows.h"

#define WINDOW_CLASS_NAME        "winretro"
#define RENDER_WINDOW_CLASS_NAME "winretro-render"

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

static struct {
    state_t state;
    state_t core_state;
    ptr instance;
    ptr window;
    ptr dev;
    ptr statusbar;
    ptr menu;
    ptr render_window;
    struct {
        ui_event_t array[64];
        u32 head;
        u32 count;
    } events;
} g_ui;

static i64 WINAPI window_event_handler(ptr hwnd, u32 msg, u64 wp, i64 lp);
static u8 open_file_dialog(u8 save, cstr title, cstr filename, cstr extensions, c8 *path, u32 pathmax);
static void enqueue_event(ui_event_t event);

void init_ui(void)
{
    free_ui();

    assert_winapi_retval_report(g_ui.instance, g_ui.instance, GetModuleHandleA, "\"%s\"", 0);

    ptr file_menu;
    assert_winapi_retval_report(file_menu, file_menu, CreateMenu, "");
    assert_winapi_report(AppendMenuA, "%p, %u, %p, \"%s\"", file_menu, 0, MENU_OPEN_CORE_ID,  MENU_OPEN_CORE_STR);
    assert_winapi_report(AppendMenuA, "%p, %u, %p, \"%s\"", file_menu, 0, MENU_OPEN_ROM_ID,   MENU_OPEN_ROM_STR);
    assert_winapi_report(AppendMenuA, "%p, %u, %p, \"%s\"", file_menu, 0, MENU_LOAD_STATE_ID, MENU_LOAD_STATE_STR);
    assert_winapi_report(AppendMenuA, "%p, %u, %p, \"%s\"", file_menu, 0, MENU_SAVE_STATE_ID, MENU_SAVE_STATE_STR);
    assert_winapi_report(AppendMenuA, "%p, %u, %p, \"%s\"", file_menu, MF_SEPARATOR, 0, 0);
    assert_winapi_report(AppendMenuA, "%p, %u, %p, \"%s\"", file_menu, 0, MENU_EXIT_ID, "Exit");

    ptr help_menu;
    assert_winapi_retval_report(help_menu, help_menu, CreateMenu, "");
    assert_winapi_report(AppendMenuA, "%p, %u, %p, \"%s\"", help_menu, 0, MENU_OPEN_WEBSITE_ID, MENU_OPEN_WEBSITE_STR);

    assert_winapi_retval_report(g_ui.menu, g_ui.menu, CreateMenu, "");
    assert_winapi_report(AppendMenuA, "%p, %u, %p, \"%s\"", g_ui.menu, MF_POPUP, (u64)file_menu, "File");
    assert_winapi_report(AppendMenuA, "%p, %u, %p, \"%s\"", g_ui.menu, MF_POPUP, (u64)help_menu, "Help");

    assert_winapi_report(ModifyMenuA, "%p, %u, %u, %p, \"%s\"", g_ui.menu, MENU_OPEN_ROM_ID,   MF_DISABLED, MENU_OPEN_ROM_ID,   MENU_OPEN_ROM_STR);
    assert_winapi_report(ModifyMenuA, "%p, %u, %u, %p, \"%s\"", g_ui.menu, MENU_LOAD_STATE_ID, MF_DISABLED, MENU_LOAD_STATE_ID, MENU_LOAD_STATE_STR);
    assert_winapi_report(ModifyMenuA, "%p, %u, %u, %p, \"%s\"", g_ui.menu, MENU_SAVE_STATE_ID, MF_DISABLED, MENU_SAVE_STATE_ID, MENU_SAVE_STATE_STR);

    WNDCLASSA window_class = {
        .hInstance = g_ui.instance,
        .lpszClassName = WINDOW_CLASS_NAME,
        .lpfnWndProc = window_event_handler,
        .hCursor = LoadCursorA(0, IDC_ARROW),
    };
    assert_winapi_report(RegisterClassA, "%p", &window_class);

    assert_winapi_retval_report(
        g_ui.window,
        g_ui.window,
        CreateWindowExA,
        "%u, \"%s\", \"%s\", %u, %d, %d, %d, %d, %p, %p, %p, %p",
        0,
        WINDOW_CLASS_NAME,
        "WinRetro",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
        0,
        g_ui.menu,
        g_ui.instance,
        0
    );

    WNDCLASSA render_window_class = {
        .style = CS_OWNDC,
        .hInstance = g_ui.instance,
        .lpszClassName = RENDER_WINDOW_CLASS_NAME,
        .lpfnWndProc = DefWindowProcA,
        .hCursor = LoadCursorA(0, IDC_ARROW),
        .hbrBackground = GetStockObject(BLACK_BRUSH),
    };
    assert_winapi_report(RegisterClassA, "%p", &render_window_class);

    assert_winapi_retval_report(
        g_ui.render_window,
        g_ui.render_window,
        CreateWindowExA,
        "%u, \"%s\", \"%s\", %u, %d, %d, %d, %d, %p, %p, %p, %p",
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

    assert_winapi_retval_report(g_ui.dev, g_ui.dev, GetDC, "%p", g_ui.render_window);

    INITCOMMONCONTROLSEX inf = {
        .dwSize = sizeof(inf),
        .dwICC = ICC_STANDARD_CLASSES,
    };
    assert_report(InitCommonControlsEx(&inf));

    assert_winapi_retval_report(
        g_ui.statusbar,
        g_ui.statusbar,
        CreateWindowExA,
        "%u, \"%s\", \"%s\", %u, %d, %d, %d, %d, %p, %p, %p, %p",
        0,
        STATUSCLASSNAME,
        0,
        WS_CHILD | WS_VISIBLE,
        0, 0, 0, 0,
        g_ui.window,
        0,
        g_ui.instance,
        0
    );
    SendMessageA(g_ui.statusbar, SB_SETPARTS, 1, (i64)(i32[]){ -1 });
    SendMessageA(g_ui.statusbar, SB_SETTEXTA, 0 | SBT_NOBORDERS, (i64)"");

    ShowWindow(g_ui.window, SW_SHOW);
    g_ui.state = STATE_INITIALIZED;
}

void free_ui(void)
{
    check_return(g_ui.state != STATE_UNINITIALIZED);

    ReleaseDC(g_ui.render_window, g_ui.dev);
    DestroyWindow(g_ui.render_window);
    DestroyWindow(g_ui.window);
    UnregisterClassA(RENDER_WINDOW_CLASS_NAME, g_ui.instance);
    UnregisterClassA(WINDOW_CLASS_NAME, g_ui.instance);
    RtlZeroMemory(&g_ui, sizeof(g_ui));
    
    SetLastError(0);
}

void ui_display_core_state(state_t state)
{
    assert_report(g_ui.state == STATE_INITIALIZED);
    check_return(g_ui.core_state != state);
    
    g_ui.core_state = state;
    assert_winapi_report(ModifyMenuA, "%p, %u, %u, %p, \"%s\"", g_ui.menu, MENU_OPEN_ROM_ID,   (state >= STATE_INITIALIZED) ? (MF_ENABLED) : (MF_DISABLED), MENU_OPEN_ROM_ID,   MENU_OPEN_ROM_STR);
    assert_winapi_report(ModifyMenuA, "%p, %u, %u, %p, \"%s\"", g_ui.menu, MENU_LOAD_STATE_ID, (state == STATE_ACTIVE)      ? (MF_ENABLED) : (MF_DISABLED), MENU_LOAD_STATE_ID, MENU_LOAD_STATE_STR);
    assert_winapi_report(ModifyMenuA, "%p, %u, %u, %p, \"%s\"", g_ui.menu, MENU_SAVE_STATE_ID, (state == STATE_ACTIVE)      ? (MF_ENABLED) : (MF_DISABLED), MENU_SAVE_STATE_ID, MENU_SAVE_STATE_STR);
}

void set_ui_status(cstr text)
{
    assert_report(g_ui.state == STATE_INITIALIZED);
    SendMessageA(g_ui.statusbar, SB_SETTEXTA, 0 | SBT_NOBORDERS, (i64)text);
}

void poll_ui_events(void)
{
    assert_report(g_ui.state == STATE_INITIALIZED);
    for (MSG msg; PeekMessageA(&msg, g_ui.window, 0, 0, PM_REMOVE) != 0;)
    {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
}

u8 get_ui_event(ui_event_t *event)
{
    assert_report(g_ui.state == STATE_INITIALIZED);
    assert_report(event);
    check_return_value(g_ui.events.count, false);
    *event = g_ui.events.array[(g_ui.events.head++) % countof(g_ui.events.array)];
    g_ui.events.count--;
    return true;
}

ptr get_ui_device_context(void)
{
    assert_report(g_ui.state == STATE_INITIALIZED);
    return g_ui.dev;
}

i64 WINAPI window_event_handler(ptr hwnd, u32 msg, u64 wp, i64 lp)
{
    check_return_value(hwnd == g_ui.window, DefWindowProcA(hwnd, msg, wp, lp));

    ui_event_t e;

    switch (msg)
    {
    case WM_CLOSE:
        e.type = UI_EXIT;
        enqueue_event(e);
        return 1;

    case WM_SIZE:
        SendMessageA(g_ui.statusbar, WM_SIZE, 0, 0);

        RECT wr, sbr;
        assert_report(GetClientRect(g_ui.window, &wr));
        assert_report(GetClientRect(g_ui.statusbar, &sbr));
        assert_report(GetClientRect(g_ui.window, &wr));

        e.type = UI_RESIZE;
        e.value.size.x = wr.right;
        e.value.size.y = wr.bottom - sbr.bottom;
        enqueue_event(e);

        assert_report(SetWindowPos(g_ui.render_window, 0, 0, 0, e.value.size.x, e.value.size.y, 0));
        break;

    case WM_COMMAND:
        switch ((u16)wp)
        {
        case MENU_EXIT_ID:
            SendMessageA(g_ui.window, WM_CLOSE, 0, 0);
            break;

        case MENU_OPEN_CORE_ID:
                if (open_file_dialog(false, MENU_OPEN_CORE_STR, 0, "Libretro Core (*.dll)\0*.DLL\0", e.value.path, sizeof(e.value.path)))
                {
                    e.type = UI_OPEN_CORE;
                    enqueue_event(e);
                }
            break;

        case MENU_OPEN_ROM_ID:
            if (open_file_dialog(false, MENU_OPEN_ROM_STR, 0, "Game Rom (*.chd)\0*.CHD\0Game Rom (*.bin)\0*.BIN\0All Files (*.*)\0*.*\0", e.value.path, sizeof(e.value.path)))
            {
                e.type = UI_OPEN_ROM;
                enqueue_event(e);
            }
            break;

        case MENU_LOAD_STATE_ID:
            if (open_file_dialog(false, MENU_LOAD_STATE_STR, 0, "Game State (*.bin)\0*.BIN\0All Files (*.*)\0*.*\0", e.value.path, sizeof(e.value.path)))
            {
                e.type = UI_LOAD_STATE;
                enqueue_event(e);
            }
            break;

        case MENU_SAVE_STATE_ID:
            if (open_file_dialog(true, MENU_SAVE_STATE_STR, "save.bin", "Game State (*.bin)\0*.BIN\0All Files (*.*)\0*.*\0", e.value.path, sizeof(e.value.path)))
            {
                e.type = UI_SAVE_STATE;
                enqueue_event(e);
            }
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

void enqueue_event(ui_event_t event)
{
    assert_report(g_ui.events.count + 1 <= countof(g_ui.events.array));
    g_ui.events.array[(g_ui.events.head + (g_ui.events.count++)) % countof(g_ui.events.array)]
        = event;
}
