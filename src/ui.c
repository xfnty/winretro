#include "ui.h"

#include "logger.h"
#include "miniwindows.h"

#define WNDCLASS_NAME "libretro-frontend"

#define MENU_OPEN_CORE_STR   "Open Core"
#define MENU_OPEN_ROM_STR    "Open ROM"
#define MENU_LOAD_STATE_STR  "Load State"
#define MENU_SAVE_STATE_STR  "Save State"

enum {
    MENU_SYS_OPEN_CORE,
    MENU_SYS_OPEN_ROM,
    MENU_SYS_LOAD_STATE,
    MENU_SYS_SAVE_STATE,
    MENU_SYS_EXIT,
    MENU_HELP_WEBSITE,
};

typedef struct UiPrivate UiPrivate;
struct UiPrivate {
    Ui base;
    UiState state;
    Logger *logger;
    HWND window;
    HWND statusbar;
    HMENU menu;
    HINSTANCE instance;
    char dialog_path[260];
    struct {
        UiEvent array[64];
        u32 head;
        u32 count;
    } events;
};

static DWORD WINAPI WindowThreadProc(Ui *ui);
LRESULT CALLBACK WindowEventHandler(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
static void EnqueueEvent(UiPrivate *uip, UiEvent event);
static u8 OpenFileDialog(UiPrivate *uip, u8 save, cstr title, cstr filename, cstr extensions);

Ui *CreateUi(UiParams params)
{
    UiPrivate *ui = HeapAlloc(
        GetProcessHeap(),
        HEAP_GENERATE_EXCEPTIONS | HEAP_ZERO_MEMORY,
        sizeof(*ui)
    );

    ui->base.params = params;
    ui->logger = CreateLogger((LoggerParams){ .name = "ui" });
    assert(ui->logger);

    ui->instance = GetModuleHandleA(0);
    WNDCLASSA class = {
        .style = CS_OWNDC,
        .hInstance = ui->instance,
        .lpszClassName = WNDCLASS_NAME,
        .lpfnWndProc = WindowEventHandler,
        .hCursor = LoadCursorA(0, IDC_ARROW),
        .hbrBackground = GetStockObject(WHITE_BRUSH),
    };
    assert(RegisterClassA(&class));
    ui->window = CreateWindowExA(
        0,
        WNDCLASS_NAME,
        params.title,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
        0,
        0,
        ui->instance,
        0
    );
    assert(ui->window);
    SetWindowLongPtrA(ui->window, GWLP_USERDATA, (LONG_PTR)ui);

    INITCOMMONCONTROLSEX inf = {
    .dwSize = sizeof(inf),
    .dwICC = ICC_STANDARD_CLASSES | ICC_BAR_CLASSES | ICC_PROGRESS_CLASS,
    };
    assert(InitCommonControlsEx(&inf));

    ui->statusbar = CreateWindowExA(
        0,
        STATUSCLASSNAME,
        0,
        WS_CHILD | WS_VISIBLE,
        0, 0, 0, 0,
        ui->window,
        0,
        ui->instance,
        0
    );
    assert(ui->statusbar);
    SendMessageA(ui->statusbar, SB_SETPARTS, 1, (LPARAM)(i32[]){ -1 });

    HMENU sysmenu = CreateMenu();
    assert(AppendMenuA(sysmenu, 0, MENU_SYS_OPEN_CORE,  MENU_OPEN_CORE_STR));
    assert(AppendMenuA(sysmenu, 0, MENU_SYS_OPEN_ROM,   MENU_OPEN_ROM_STR));
    assert(AppendMenuA(sysmenu, 0, MENU_SYS_LOAD_STATE, MENU_LOAD_STATE_STR));
    assert(AppendMenuA(sysmenu, 0, MENU_SYS_SAVE_STATE, MENU_SAVE_STATE_STR));
    assert(AppendMenuA(sysmenu, 0x00000800L, 0, 0));
    assert(AppendMenuA(sysmenu, 0, MENU_SYS_EXIT, "Exit"));
    HMENU helpmenu = CreateMenu();
    assert(AppendMenuA(helpmenu, 0, MENU_HELP_WEBSITE, "Project website"));
    ui->menu = CreateMenu();
    assert(AppendMenuA(ui->menu, MF_POPUP, (UINT_PTR)sysmenu, "System"));
    assert(AppendMenuA(ui->menu, MF_POPUP, (UINT_PTR)helpmenu, "Help"));
    assert(SetMenu(ui->window, ui->menu));

    Ui_SetState((Ui*)ui, UI_INITIAL);
    
    ShowWindow(ui->window, SW_SHOW);

    return (Ui*)ui;
}

void FreeUi(Ui **ui)
{
    assert(ui);
    DestroyWindow(((UiPrivate*)*ui)->window);
    UnregisterClassA(WNDCLASS_NAME, ((UiPrivate*)*ui)->instance);
    HeapFree(GetProcessHeap(), 0, *ui);
    *ui = 0;
}

void Ui_ProcessEvents(Ui *ui)
{
    for (MSG msg; PeekMessageA(&msg, ((UiPrivate*)ui)->window, 0, 0, PM_REMOVE) != 0;)
    {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
}

u8 Ui_GetEvent(Ui *ui, UiEvent *event)
{
    assert(ui);
    assert(event);

    UiPrivate *uip = (UiPrivate*)ui;

    if (!uip->events.count) return 0;

    *event = uip->events.array[(uip->events.head++) % countof(uip->events.array)];
    uip->events.count--;
    return 1;
}

void Ui_SetCoreName(Ui *ui, cstr name)
{
    UiPrivate *uip = (UiPrivate*)ui;
    assert(uip);

    SendMessageA(uip->statusbar, SB_SETTEXTA, 0 | SBT_NOBORDERS, (LPARAM)name);
}

void Ui_SetState(Ui *ui, UiState state)
{
    UiPrivate *uip = (UiPrivate*)ui;
    assert(uip);

    uip->state = state;

    switch (state)
    {
    case UI_INITIAL:
        ModifyMenuA(uip->menu, MENU_SYS_OPEN_CORE,  MF_ENABLED,  MENU_SYS_OPEN_CORE,  MENU_OPEN_CORE_STR);
        ModifyMenuA(uip->menu, MENU_SYS_OPEN_ROM,   MF_DISABLED, MENU_SYS_OPEN_ROM,   MENU_OPEN_ROM_STR);
        ModifyMenuA(uip->menu, MENU_SYS_LOAD_STATE, MF_DISABLED, MENU_SYS_LOAD_STATE, MENU_LOAD_STATE_STR);
        ModifyMenuA(uip->menu, MENU_SYS_SAVE_STATE, MF_DISABLED, MENU_SYS_SAVE_STATE, MENU_SAVE_STATE_STR);
        break;

    case UI_CORE_LOADED:
        ModifyMenuA(uip->menu, MENU_SYS_OPEN_CORE,  MF_ENABLED,  MENU_SYS_OPEN_CORE,  MENU_OPEN_CORE_STR);
        ModifyMenuA(uip->menu, MENU_SYS_OPEN_ROM,   MF_ENABLED,  MENU_SYS_OPEN_ROM,   MENU_OPEN_ROM_STR);
        ModifyMenuA(uip->menu, MENU_SYS_LOAD_STATE, MF_DISABLED, MENU_SYS_LOAD_STATE, MENU_LOAD_STATE_STR);
        ModifyMenuA(uip->menu, MENU_SYS_SAVE_STATE, MF_DISABLED, MENU_SYS_SAVE_STATE, MENU_SAVE_STATE_STR);
        break;

    case UI_GAMEPLAY:
        ModifyMenuA(uip->menu, MENU_SYS_OPEN_CORE,  MF_ENABLED, MENU_SYS_OPEN_CORE,  MENU_OPEN_CORE_STR);
        ModifyMenuA(uip->menu, MENU_SYS_OPEN_ROM,   MF_ENABLED, MENU_SYS_OPEN_ROM,   MENU_OPEN_ROM_STR);
        ModifyMenuA(uip->menu, MENU_SYS_LOAD_STATE, MF_ENABLED, MENU_SYS_LOAD_STATE, MENU_LOAD_STATE_STR);
        ModifyMenuA(uip->menu, MENU_SYS_SAVE_STATE, MF_ENABLED, MENU_SYS_SAVE_STATE, MENU_SAVE_STATE_STR);
        break;
    }
}

LRESULT CALLBACK WindowEventHandler(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    UiPrivate *uip = (UiPrivate*)GetWindowLongPtrA(hwnd, GWLP_USERDATA);
    if (!uip || uip->window != hwnd)
    {
        return DefWindowProcA(hwnd, msg, wp, lp);
    }

    switch (msg)
    {
    case WM_CLOSE:
    case WM_DESTROY:
        EnqueueEvent(uip, (UiEvent){ .type = UI_EXIT });
        return 1;

    case WM_SIZE:
        SendMessageA(uip->statusbar, WM_SIZE, 0, 0);
        break;

    case WM_COMMAND:
        i32 cmd = LOWORD(wp);
        if (cmd == MENU_SYS_EXIT)
        {
            SendMessageA(uip->window, WM_CLOSE, 0, 0);
        }
        else if (cmd == MENU_SYS_OPEN_CORE)
        {
            if (OpenFileDialog(uip, false, MENU_OPEN_CORE_STR, 0, "Libretro Core (*.dll)\0*.DLL\0"))
            {
                EnqueueEvent(uip, (UiEvent){ .type = UI_OPEN_CORE, .value.path = uip->dialog_path });
            }
        }
        else if (cmd == MENU_SYS_OPEN_ROM)
        {
            if (
                OpenFileDialog(
                    uip,
                    false,
                    MENU_OPEN_ROM_STR,
                    0,
                    "Game Rom (*.chd)\0*.CHD\0"
                    "Game Rom (*.bin)\0*.BIN\0"
                    "All Files (*.*)\0*.*\0"
                )
            )
            {
                EnqueueEvent(uip, (UiEvent){ .type = UI_OPEN_ROM, .value.path = uip->dialog_path });
            }
        }
        else if (cmd == MENU_SYS_LOAD_STATE)
        {
            if (OpenFileDialog(uip, false, MENU_LOAD_STATE_STR, 0, "Game State (*.bin)\0*.BIN\0All Files (*.*)\0*.*\0"))
            {
                EnqueueEvent(uip, (UiEvent){ .type = UI_LOAD_STATE, .value.path = uip->dialog_path });
            }
        }
        else if (cmd == MENU_SYS_SAVE_STATE)
        {
            if (OpenFileDialog(uip, true, MENU_SAVE_STATE_STR, "save.bin", "Game State (*.bin)\0*.BIN\0All Files (*.*)\0*.*\0"))
            {
                EnqueueEvent(uip, (UiEvent){ .type = UI_SAVE_STATE, .value.path = uip->dialog_path });
            }
        }
        else if (cmd == MENU_HELP_WEBSITE)
        {
            INT_PTR err = (INT_PTR)ShellExecuteA(
                0,
                "open",
                "https://github.com/xfnty/tiny-libretro-frontend",
                0,
                0,
                SW_SHOWNORMAL
            );
            if (err <= 32)
            {
                LogError(uip->logger, "ShellExecuteA() failed (%d)", (i32)err);
            }
        }
        break;
    }

    return DefWindowProcA(hwnd, msg, wp, lp);
}

void EnqueueEvent(UiPrivate *uip, UiEvent event)
{
    assert(uip);
    assert(uip->events.count + 1 <= countof(uip->events.array));

    uip->events.array[(uip->events.head + (uip->events.count++)) % countof(uip->events.array)]
        = event;
}

u8 OpenFileDialog(UiPrivate *uip, u8 save, cstr title, cstr filename, cstr extensions)
{
    assert(uip);

    if (filename)
    {
        u32 len = 0;
        for (; filename[len]; len++);
        RtlCopyMemory(uip->dialog_path, "save.bin", len + 1);
    }
    else
    {
        RtlZeroMemory(uip->dialog_path, sizeof(uip->dialog_path));
    }

    OPENFILENAMEA info = {
        .lStructSize = sizeof(info),
        .lpstrTitle = title,
        .hwndOwner = uip->window,
        .lpstrFilter = extensions,
        .nFilterIndex = 1,
        .lpstrFile = uip->dialog_path,
        .nMaxFile = sizeof(uip->dialog_path),
        .Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST,
    };

    return (u8)((save) ? (GetSaveFileNameA(&info)) : (GetOpenFileNameA(&info)));
}
