#include "ui.h"

#include "log.h"
#include "miniwindows.h"

#define WNDCLASS_NAME "libretro-frontend"
#define PRESENT_WNDCLASS_NAME "libretro-frontend-present-window"

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

static struct {
    UiParams params;
    cstr core;
    cstr rom;
    HDC dc;
    HGLRC gl;
    HWND window;
    HWND present_window;
    HWND statusbar;
    HMENU menu;
    HINSTANCE instance;
    RECT present_rect;
    char dialog_path[260];
    struct {
        UiEvent array[64];
        u32 head;
        u32 count;
    } events;
    DWORD bindings[_CORE_AXIS_COUNT];
} s_ui;

LRESULT CALLBACK WindowEventHandler(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);
static void EnqueueEvent(UiEvent event);
static u8 OpenFileDialog(u8 save, cstr title, cstr filename, cstr extensions);

void InitUi(UiParams params)
{
    FreeUi();

    s_ui.params = params;
    s_ui.bindings[CORE_AXIS_START] = 0x0D;
    s_ui.bindings[CORE_AXIS_SELECT] = ' ';
    s_ui.bindings[CORE_AXIS_LEFT] = 'A';
    s_ui.bindings[CORE_AXIS_RIGHT] = 'D';
    s_ui.bindings[CORE_AXIS_UP] = 'W';
    s_ui.bindings[CORE_AXIS_DOWN] = 'S';
    s_ui.bindings[CORE_AXIS_A] = 'K';
    s_ui.bindings[CORE_AXIS_B] = 'L';
    s_ui.bindings[CORE_AXIS_X] = 'I';
    s_ui.bindings[CORE_AXIS_Y] = 'O';
    s_ui.bindings[CORE_AXIS_L] = 'Q';
    s_ui.bindings[CORE_AXIS_R] = 'E';

    HMENU sysmenu = CreateMenu();
    assert(AppendMenuA(sysmenu, 0, MENU_SYS_OPEN_CORE,  MENU_OPEN_CORE_STR));
    assert(AppendMenuA(sysmenu, 0, MENU_SYS_OPEN_ROM,   MENU_OPEN_ROM_STR));
    assert(AppendMenuA(sysmenu, 0, MENU_SYS_LOAD_STATE, MENU_LOAD_STATE_STR));
    assert(AppendMenuA(sysmenu, 0, MENU_SYS_SAVE_STATE, MENU_SAVE_STATE_STR));
    assert(AppendMenuA(sysmenu, MF_SEPARATOR, 0, 0));
    assert(AppendMenuA(sysmenu, 0, MENU_SYS_EXIT, "Exit"));
    HMENU helpmenu = CreateMenu();
    assert(AppendMenuA(helpmenu, 0, MENU_HELP_WEBSITE, "Project website"));
    s_ui.menu = CreateMenu();
    assert(AppendMenuA(s_ui.menu, MF_POPUP, (UINT_PTR)sysmenu, "System"));
    assert(AppendMenuA(s_ui.menu, MF_POPUP, (UINT_PTR)helpmenu, "Help"));

    s_ui.instance = GetModuleHandleA(0);
    WNDCLASSA class = {
        .hInstance = s_ui.instance,
        .lpszClassName = WNDCLASS_NAME,
        .lpfnWndProc = WindowEventHandler,
        .hCursor = LoadCursorA(0, IDC_ARROW),
    };
    assert(RegisterClassA(&class));
    s_ui.window = CreateWindowExA(
        0,
        WNDCLASS_NAME,
        s_ui.params.title,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
        0,
        s_ui.menu,
        s_ui.instance,
        0
    );
    assert(s_ui.window);
    WNDCLASSA present_class = {
        .style = CS_OWNDC,
        .hInstance = s_ui.instance,
        .lpszClassName = PRESENT_WNDCLASS_NAME,
        .lpfnWndProc = DefWindowProcA,
        .hCursor = LoadCursorA(0, IDC_ARROW),
        // .hbrBackground = GetStockObject(BLACK_BRUSH),
    };
    assert(RegisterClassA(&present_class));
    s_ui.present_window = CreateWindowExA(
        0,
        PRESENT_WNDCLASS_NAME,
        "",
        WS_CHILD | WS_VISIBLE,
        0, 0, 0, 0,
        s_ui.window,
        0,
        s_ui.instance,
        0
    );
    assert(s_ui.present_window);

    s_ui.dc = GetDC(s_ui.present_window);
    assert(s_ui.dc);
    PIXELFORMATDESCRIPTOR pfd = {0};
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.iLayerType = PFD_MAIN_PLANE;
    int pixel_format = ChoosePixelFormat(s_ui.dc, &pfd);
    assert(DescribePixelFormat(s_ui.dc, pixel_format, sizeof(pfd), &pfd));
    assert(SetPixelFormat(s_ui.dc, pixel_format, &pfd));
    HGLRC tmp_gl = wglCreateContext(s_ui.dc);
    assert(tmp_gl);
    assert(wglMakeCurrent(s_ui.dc, tmp_gl));
    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB
        = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
    assert(wglCreateContextAttribsARB);
    PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT
        = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
    assert(wglSwapIntervalEXT);
    assert(wglMakeCurrent(0, 0));
    assert(wglDeleteContext(tmp_gl));
    s_ui.gl = wglCreateContextAttribsARB(s_ui.dc, 0, (i32[]){
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 3,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
        0
    });
    assert(s_ui.gl);
    assert(wglMakeCurrent(s_ui.dc, s_ui.gl));
    assert(wglSwapIntervalEXT(1));
    LogMessage(LOG_INFO, "using %s on %s", glGetString(GL_VERSION), glGetString(GL_RENDERER));

    INITCOMMONCONTROLSEX inf = {
        .dwSize = sizeof(inf),
        .dwICC = ICC_STANDARD_CLASSES | ICC_BAR_CLASSES | ICC_PROGRESS_CLASS,
    };
    assert(InitCommonControlsEx(&inf));

    s_ui.statusbar = CreateWindowExA(
        0,
        STATUSCLASSNAME,
        0,
        WS_CHILD | WS_VISIBLE,
        0, 0, 0, 0,
        s_ui.window,
        0,
        s_ui.instance,
        0
    );
    assert(s_ui.statusbar);
    SendMessageA(s_ui.statusbar, SB_SETPARTS, 1, (LPARAM)(i32[]){ -1 });

    Ui_SetCoreLoaded(0);
    Ui_SetRomLoaded(0);
    
    ShowWindow(s_ui.window, SW_SHOW);
}

void FreeUi(void)
{
    DestroyWindow(s_ui.window);
    UnregisterClassA(WNDCLASS_NAME, s_ui.instance);
    RtlZeroMemory(&s_ui, sizeof(s_ui));
}

void Ui_ProcessEvents(void)
{
    for (MSG msg; PeekMessageA(&msg, s_ui.window, 0, 0, PM_REMOVE) != 0;)
    {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
}

u8 Ui_GetEvent(UiEvent *event)
{
    assert(event);

    if (!s_ui.events.count)
        return false;

    *event = s_ui.events.array[(s_ui.events.head++) % countof(s_ui.events.array)];
    s_ui.events.count--;
    return true;
}

void Ui_SetCoreLoaded(cstr name)
{
    s_ui.core = name;

    if (s_ui.core)
    {
        ModifyMenuA(s_ui.menu, MENU_SYS_OPEN_CORE,  MF_ENABLED,  MENU_SYS_OPEN_CORE,  MENU_OPEN_CORE_STR);
        ModifyMenuA(s_ui.menu, MENU_SYS_OPEN_ROM,   MF_ENABLED,  MENU_SYS_OPEN_ROM,   MENU_OPEN_ROM_STR);
        ModifyMenuA(s_ui.menu, MENU_SYS_LOAD_STATE, MF_DISABLED, MENU_SYS_LOAD_STATE, MENU_LOAD_STATE_STR);
        ModifyMenuA(s_ui.menu, MENU_SYS_SAVE_STATE, MF_DISABLED, MENU_SYS_SAVE_STATE, MENU_SAVE_STATE_STR);
    }
    else
    {
        ModifyMenuA(s_ui.menu, MENU_SYS_OPEN_CORE,  MF_ENABLED,  MENU_SYS_OPEN_CORE,  MENU_OPEN_CORE_STR);
        ModifyMenuA(s_ui.menu, MENU_SYS_OPEN_ROM,   MF_DISABLED, MENU_SYS_OPEN_ROM,   MENU_OPEN_ROM_STR);
        ModifyMenuA(s_ui.menu, MENU_SYS_LOAD_STATE, MF_DISABLED, MENU_SYS_LOAD_STATE, MENU_LOAD_STATE_STR);
        ModifyMenuA(s_ui.menu, MENU_SYS_SAVE_STATE, MF_DISABLED, MENU_SYS_SAVE_STATE, MENU_SAVE_STATE_STR);
    }

    SendMessageA(s_ui.statusbar, SB_SETTEXTA, 0 | SBT_NOBORDERS, (LPARAM)s_ui.core);
}

void Ui_SetRomLoaded(cstr name)
{
    if (name)
    {
        if (!s_ui.core)
        {
            LogMessage(LOG_ERROR, "attempted to set ROM name to \"%s\" when Core is empty", name);
            return;
        }

        ModifyMenuA(s_ui.menu, MENU_SYS_OPEN_CORE,  MF_ENABLED, MENU_SYS_OPEN_CORE,  MENU_OPEN_CORE_STR);
        ModifyMenuA(s_ui.menu, MENU_SYS_OPEN_ROM,   MF_ENABLED, MENU_SYS_OPEN_ROM,   MENU_OPEN_ROM_STR);
        ModifyMenuA(s_ui.menu, MENU_SYS_LOAD_STATE, MF_ENABLED, MENU_SYS_LOAD_STATE, MENU_LOAD_STATE_STR);
        ModifyMenuA(s_ui.menu, MENU_SYS_SAVE_STATE, MF_ENABLED, MENU_SYS_SAVE_STATE, MENU_SAVE_STATE_STR);
    }
    else
    {
        Ui_SetCoreLoaded(s_ui.core);
    }
}

void Ui_PresentFrame(void)
{
    static f32 i = 0;
    i += 1;
    if (i > 100) i = 0;
    glClearColor(i / 100.0f, i / 100.0f, i / 100.0f, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    SwapBuffers(s_ui.dc);
}

LRESULT CALLBACK WindowEventHandler(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp)
{
    if (s_ui.window != hwnd)
    {
        return DefWindowProcA(hwnd, msg, wp, lp);
    }

    switch (msg)
    {
    case WM_CLOSE:
        ShowWindow(s_ui.window, SW_HIDE);
        LogMessage(LOG_INFO, "exit requested");
        EnqueueEvent((UiEvent){ .type = UI_EXIT });
        return 1;

    case WM_SIZE:
        SendMessageA(s_ui.statusbar, WM_SIZE, 0, 0);
        RECT wr, sbr;
        assert(GetClientRect(s_ui.window, &wr));
        assert(GetClientRect(s_ui.statusbar, &sbr));
        s_ui.present_rect = (RECT){
            0, 0, wr.right, wr.bottom - sbr.bottom,
        };
        assert(
            SetWindowPos(
                s_ui.present_window,
                HWND_TOP,
                s_ui.present_rect.left,
                s_ui.present_rect.top,
                s_ui.present_rect.right,
                s_ui.present_rect.bottom,
                0
            )
        );
        glViewport(
            s_ui.present_rect.left,
            s_ui.present_rect.top,
            s_ui.present_rect.right,
            s_ui.present_rect.bottom
        );
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        SwapBuffers(s_ui.dc);
        break;

    case WM_COMMAND:
        {
            i32 cmd = LOWORD(wp);
            switch (cmd)
            {
            case MENU_SYS_EXIT:
                {
                    SendMessageA(s_ui.window, WM_CLOSE, 0, 0);
                }
                break;

            case MENU_SYS_OPEN_CORE:
                {
                    if (OpenFileDialog(false, MENU_OPEN_CORE_STR, 0, "Libretro Core (*.dll)\0*.DLL\0"))
                    {
                        LogMessage(LOG_INFO, "load core from \"%s\"", s_ui.dialog_path);
                        EnqueueEvent((UiEvent){ .type = UI_OPEN_CORE, .value.path = s_ui.dialog_path });
                    }
                }
                break;

            case MENU_SYS_OPEN_ROM:
                {
                    if (
                        OpenFileDialog(
                            false,
                            MENU_OPEN_ROM_STR,
                            0,
                            "Game Rom (*.chd)\0*.CHD\0"
                            "Game Rom (*.bin)\0*.BIN\0"
                            "All Files (*.*)\0*.*\0"
                        )
                    )
                    {
                        LogMessage(LOG_INFO, "load ROM from \"%s\"", s_ui.dialog_path);
                        EnqueueEvent((UiEvent){ .type = UI_OPEN_ROM, .value.path = s_ui.dialog_path });
                    }
                }
                break;

            case MENU_SYS_LOAD_STATE:
                {
                    if (OpenFileDialog(false, MENU_LOAD_STATE_STR, 0, "Game State (*.bin)\0*.BIN\0All Files (*.*)\0*.*\0"))
                    {
                        LogMessage(LOG_INFO, "load state from \"%s\"", s_ui.dialog_path);
                        EnqueueEvent((UiEvent){ .type = UI_LOAD_STATE, .value.path = s_ui.dialog_path });
                    }
                }
                break;

            case MENU_SYS_SAVE_STATE:
                {
                    if (OpenFileDialog(true, MENU_SAVE_STATE_STR, "save.bin", "Game State (*.bin)\0*.BIN\0All Files (*.*)\0*.*\0"))
                    {
                        LogMessage(LOG_INFO, "save state to \"%s\"", s_ui.dialog_path);
                        EnqueueEvent((UiEvent){ .type = UI_SAVE_STATE, .value.path = s_ui.dialog_path });
                    }
                }
                break;

            case MENU_HELP_WEBSITE:
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
                        LogMessage(LOG_ERROR, "ShellExecuteA() failed (%d)", (i32)err);
                    }
                }
                break;
            }
        }
        break;

    case WM_KEYUP:
    case WM_KEYDOWN:
        for (i32 i = 0; i < countof(s_ui.bindings); i++)
        {
            if (s_ui.bindings[i] == wp)
            {
                EnqueueEvent(
                    (UiEvent){ 
                        .type = UI_INPUT,
                        .value.input = (CoreInputAxisState){
                            (CoreInputAxis)i,
                            (f32)(msg == WM_KEYDOWN)
                        } 
                    }
                );
                break;
            }
        }
        break;
    }

    return DefWindowProcA(hwnd, msg, wp, lp);
}

void EnqueueEvent(UiEvent event)
{
    assert(s_ui.events.count + 1 <= countof(s_ui.events.array));

    s_ui.events.array[(s_ui.events.head + (s_ui.events.count++)) % countof(s_ui.events.array)]
        = event;
}

u8 OpenFileDialog(u8 save, cstr title, cstr filename, cstr extensions)
{
    if (filename)
    {
        u32 len = 0;
        for (; filename[len]; len++);
        RtlCopyMemory((ptr)s_ui.dialog_path, (const ptr)filename, len + 1);
    }
    else
    {
        RtlZeroMemory(s_ui.dialog_path, sizeof(s_ui.dialog_path));
    }

    OPENFILENAMEA info = {
        .lStructSize = sizeof(info),
        .lpstrTitle = title,
        .hwndOwner = s_ui.window,
        .lpstrFilter = extensions,
        .nFilterIndex = 1,
        .lpstrFile = s_ui.dialog_path,
        .nMaxFile = sizeof(s_ui.dialog_path),
        .Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | ((save) ? (OFN_OVERWRITEPROMPT) : (0)),
    };

    return (u8)((save) ? (GetSaveFileNameA(&info)) : (GetOpenFileNameA(&info)));
}
