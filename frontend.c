/* defines */
#define true 1u
#define false 0

#define WNDCLASS_NAME                    "libretro-frontend"
#define DRAWWNDCLASS_NAME                "libretro-frontend-draw"

#define INVALID_HANDLE_VALUE             (ptr)-1
#define ATTACH_PARENT_PROCESS            (u32)-1
#define STD_OUTPUT_HANDLE                (u32)-11
#define IDC_ARROW                        ((cstr)32512)
#define WS_OVERLAPPEDWINDOW              0x00CF0000L
#define WS_CHILD                         0x40000000L
#define WS_VISIBLE                       0x10000000L
#define CW_USEDEFAULT                    0x80000000
#define SW_SHOW                          5
#define SW_HIDE                          0
#define WM_CLOSE                         0x0010
#define WM_SIZE                          0x0005
#define PM_REMOVE                        0x0001
#define CS_OWNDC                         0x0020
#define PFD_DOUBLEBUFFER                 0x00000001
#define PFD_DRAW_TO_WINDOW               0x00000004
#define PFD_SUPPORT_OPENGL               0x00000020
#define PFD_TYPE_RGBA                    0
#define PFD_MAIN_PLANE                   0

#define GL_VENDOR                        0x1F00
#define GL_RENDERER                      0x1F01
#define GL_VERSION                       0x1F02
#define GL_COLOR_BUFFER_BIT              0x00004000
#define WGL_CONTEXT_MAJOR_VERSION_ARB    0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB    0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB     0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001
#define WGL_CONTEXT_FLAGS_ARB            0x2094
#define WGL_CONTEXT_DEBUG_BIT_ARB        0x00000001

#define RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY 9
#define RETRO_ENVIRONMENT_GET_LOG_INTERFACE    27
#define RETRO_ENVIRONMENT_GET_SAVE_DIRECTORY   31

#if defined(_MSC_VER)
    #define WINAPI __stdcall
#else
    #define WINAPI __attribute__((stdcall))
#endif


/* typedefs */
typedef char               c8;
typedef char               i8;
typedef short              i16;
typedef int                i32;
typedef signed long long   i64;
typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned long long u64;
typedef float              f32;
typedef double             f64;
typedef const char        *cstr;
typedef void              *ptr;
typedef unsigned char     *va_list;


/* imported typedefs */
typedef i64 (WINAPI *WNDPROC)(ptr hwnd, u32 msg, u64 wp, i64 lp);
typedef struct WNDCLASSA WNDCLASSA;
struct WNDCLASSA {
    u32     style;
    u8      _padding[4];
    WNDPROC lpfnWndProc;
    i32     cbClsExtra;
    i32     cbWndExtra;
    ptr     hInstance;
    ptr     hIcon;
    ptr     hCursor;
    ptr     hbrBackground;
    cstr    lpszMenuName;
    cstr    lpszClassName;
};
typedef struct POINT POINT;
struct POINT {
    i32 x, y;
};
typedef struct MSG MSG;
struct MSG {
    ptr   hwnd;
    u32   message;
    u64   wParam;
    i32   lParam;
    u32   time;
    POINT pt;
    u32   lPrivate;
};
typedef struct RECT RECT;
struct RECT {
    i32 left;
    i32 top;
    i32 right;
    i32 bottom;
};
typedef struct PIXELFORMATDESCRIPTOR PIXELFORMATDESCRIPTOR;
struct PIXELFORMATDESCRIPTOR {
    u16  nSize;
    u16  nVersion;
    u32 dwFlags;
    u8  iPixelType;
    u8  cColorBits;
    u8  cRedBits;
    u8  cRedShift;
    u8  cGreenBits;
    u8  cGreenShift;
    u8  cBlueBits;
    u8  cBlueShift;
    u8  cAlphaBits;
    u8  cAlphaShift;
    u8  cAccumBits;
    u8  cAccumRedBits;
    u8  cAccumGreenBits;
    u8  cAccumBlueBits;
    u8  cAccumAlphaBits;
    u8  cDepthBits;
    u8  cStencilBits;
    u8  cAuxBuffers;
    u8  iLayerType;
    u8  bReserved;
    u32 dwLayerMask;
    u32 dwVisibleMask;
    u32 dwDamageMask;
};
typedef ptr (WINAPI *PFNWGLCREATECONTEXTATTRIBSARBPROC)(
    ptr hDC,
    ptr hShareContext,
    i32 *attribList
);
typedef u32 (WINAPI *PFNWGLSWAPINTERVALEXTPROC)(i32 interval);
typedef i32 (WINAPI *PFNWGLGETSWAPINTERVALEXTPROC)(void);

typedef struct retro_system_info retro_system_info;
struct retro_system_info {
    cstr library_name;
    cstr library_version;
    cstr valid_extensions;
    u8   need_fullpath;
    u8   block_extract;
};

typedef struct retro_game_geometry retro_game_geometry;
struct retro_game_geometry {
    u32 base_width;
    u32 base_height;
    u32 max_width;
    u32 max_height;
    f32 aspect_ratio;
};

typedef struct retro_system_timing retro_system_timing;
struct retro_system_timing {
    f64 fps;
    f64 sample_rate;
};

typedef struct retro_system_av_info retro_system_av_info;
struct retro_system_av_info {
    retro_game_geometry geometry;
    retro_system_timing timing;
};

typedef struct retro_game_info retro_game_info;
struct retro_game_info {
    cstr path;
    ptr  data;
    u64  size;
    cstr meta;
};

typedef void (*retro_log_printf_t)(u32 level, cstr fmt, ...);
typedef u8   (*retro_environment_t)(u32 cmd, ptr data);
typedef void (*retro_video_callback_t)(ptr data, u32 width, u32 height, u64 pitch);
typedef void (*retro_audio_sample_t)(i16 left, i16 right);
typedef u64  (*retro_audio_sample_batch_t)(i16 *data, u64 frames);
typedef void (*retro_input_poll_t)(void);
typedef i16  (*retro_input_state_t)(u32 port, u32 device, u32 index, u32 id);

typedef struct retro_log_callback retro_log_callback;
struct retro_log_callback {
   retro_log_printf_t log;
};


/* macros */
#define countof(_a) (sizeof(_a)/sizeof((_a)[0]))
#define va_start(_list, _last_arg) do { _list = (u8*)&(_last_arg) + 8; } while (0)
#define va_arg(_list, _T) ( ( sizeof(_T) > 8) ? (**(_T**)((_list += 8) - 8)) : ( *(_T* )((_list += 8) - 8)) )

#if defined(_MSC_VER)
    #define assert(_x, _m, ...) do { \
            if (!(_x)) { \
                print("error: " _m " (%s():%d)", ##__VA_ARGS__, __func__, __LINE__); \
                __debugbreak(); \
            } \
        } while (0)
#else
    #define assert(_x, _m, ...) do { \
            if (!(_x)) { \
                print("error: " _m " (%s():%d)", ##__VA_ARGS__, __func__, __LINE__); \
                __asm__("int3"); \
                __builtin_unreachable(); \
            } \
        } while (0)
#endif

#define RETRO_API_DECL_LIST \
    _X(void,  retro_set_environment,            retro_environment_t callback) \
    _X(void,  retro_set_video_refresh,          retro_video_callback_t callback) \
    _X(void,  retro_set_audio_sample,           retro_audio_sample_t callback) \
    _X(void,  retro_set_audio_sample_batch,     retro_audio_sample_batch_t callback) \
    _X(void,  retro_set_input_poll,             retro_input_poll_t callback) \
    _X(void,  retro_set_input_state,            retro_input_state_t callback) \
    _X(void,  retro_init,                       void) \
    _X(void,  retro_deinit,                     void) \
    _X(u32,   retro_api_version,                void) \
    _X(void,  retro_get_system_info,            retro_system_info* info) \
    _X(void,  retro_get_system_av_info,         retro_system_av_info* avinfo) \
    _X(void,  retro_set_controller_port_device, u32 port, u32 device) \
    _X(void,  retro_reset,                      void) \
    _X(void,  retro_run,                        void) \
    _X(u64,   retro_serialize_size,             void) \
    _X(u8,    retro_serialize,                  ptr data, u64 len) \
    _X(u8,    retro_unserialize,                const ptr data, u64 len) \
    _X(void,  retro_cheat_reset,                void) \
    _X(void,  retro_cheat_set,                  u32 index, u8 enabled, cstr code) \
    _X(u8,    retro_load_game,                  const retro_game_info *info) \
    _X(u8,    retro_load_game_special,          u32 type, const retro_game_info *info, u64 count) \
    _X(void,  retro_unload_game,                void) \
    _X(u32,   retro_get_region,                 void) \
    _X(ptr,   retro_get_memory_data,            u32 type) \
    _X(u64,   retro_get_memory_size,            u32 type)



/* imports */
u32  WINAPI AttachConsole(u32 pid);
ptr  WINAPI GetStdHandle(u32 id);
void ExitProcess(u32 code);
u32  GetLastError(void);
u32  WriteConsoleA(ptr console, ptr buffer, u32 bytes, u32 *written, ptr _reserved);
void OutputDebugStringA(cstr str);
ptr  GetModuleHandleA(cstr name);
ptr  LoadCursorA(ptr hinst, cstr name);
u16  RegisterClassA(WNDCLASSA *cls);
i64  DefWindowProcA(ptr hwnd, u32 msg, u64 wp, i64 lp);
ptr CreateWindowExA(
    u32  exstyle,
    cstr cls,
    cstr title,
    u32  style,
    i32  x,
    i32  y,
    i32  w,
    i32  h,
    ptr  hwnd_parent,
    ptr  hmenu,
    ptr  hinst,
    ptr  lp
);
u32  ShowWindow(ptr hwnd, i32 show);
u32  PeekMessageA(MSG *msg, ptr hwnd, u32 fmin, u32 fmax, u32 flags);
u32  TranslateMessage(MSG *msg);
u32  DispatchMessageA(MSG *msg);
u32  GetClientRect(ptr hwnd, RECT *r);
u32  SetWindowPos(ptr hwnd, ptr insert_type, i32 x, i32 y, i32 w, i32 h, u32 flags);
ptr  GetDC(ptr hwnd);
i32  ChoosePixelFormat(ptr hdc, PIXELFORMATDESCRIPTOR *pfd);
i32  DescribePixelFormat(ptr hdc, i32 pixel_format, u32 pfd_size, PIXELFORMATDESCRIPTOR *pfd);
u32  SetPixelFormat(ptr hdc, i32 pixel_format, PIXELFORMATDESCRIPTOR *pfd);
ptr  WINAPI wglCreateContext(ptr hdc);
u32  WINAPI wglDeleteContext(ptr hglrc);
ptr  WINAPI wglGetProcAddress(cstr symbol);
u32  WINAPI wglMakeCurrent(ptr hdc, ptr hglrc);
u32  WINAPI SwapBuffers(ptr hdc);
void WINAPI glClear(u32 mask);
void WINAPI glClearColor(f32 r, f32 g, f32 b, f32 a);
cstr WINAPI glGetString(u32 name);
ptr  LoadLibraryA(cstr name);
u32  FreeLibrary(ptr module);
ptr  GetProcAddress(ptr module, cstr symbol);
void RtlZeroMemory(ptr buffer, u64 size);


/* function declarations */
void _start(void);
void init_logging(void);
void init_ui(void);
void load_core(cstr path);
void unload_core(void);
void load_game(cstr path);
void unload_game(void);
void process_ui_events(void);
void present_frame(void);
i64  WINAPI window_event_handler(ptr hwnd, u32 msg, u64 wp, i64 lp);
void core_log_callback(u32 level, cstr format, ...);
u8   core_environment_callback(u32 cmd, ptr data);
void core_video_callback(ptr data, u32 width, u32 height, u64 pitch);
void core_audio_sample_callback(i16 left, i16 right);
u64  core_audio_batch_callback(i16 *data, u64 frames);
void core_input_poll_callback(void);
i16  core_input_state_callback(u32 port, u32 device, u32 index, u32 id);
void print(cstr format, ...);
u32  snprintf(c8 *buffer, u32 maxsize, cstr format, ...);
u32  vsnprintf(c8 *buffer, u32 maxsize, cstr format, va_list args);


/* variables */
u32 _fltused=1;
ptr g_stdout;

struct {
    ptr hwnd;
    ptr draw_hwnd;
    ptr hinst;
    ptr hdc;
    ptr hglrc;
    u32 was_exit_requested;
} g_ui;

struct {
    ptr module;
    retro_system_info info;
    retro_system_av_info avinfo;
    struct {
        #define _X(_ret, _name, _arg1, ...) _ret (*_name)(_arg1, ##__VA_ARGS__);
        RETRO_API_DECL_LIST
        #undef _X
    } api;
} g_core;


/* function definitions */
void _start(void)
{
    init_logging();
    load_core(".ignore\\swanstation_libretro.dll");
    load_game(".ignore\\game.chd");
    init_ui();

    while (!g_ui.was_exit_requested)
    {
        process_ui_events();
        present_frame();
    }

    unload_game();
    unload_core();
    ExitProcess(0);
}

void init_logging(void)
{
    AttachConsole(ATTACH_PARENT_PROCESS);
    g_stdout = GetStdHandle(STD_OUTPUT_HANDLE);
}

void init_ui(void)
{
    g_ui.hinst = GetModuleHandleA(0);

    WNDCLASSA wndclass = {
        .hInstance = g_ui.hinst,
        .lpszClassName = WNDCLASS_NAME,
        .lpfnWndProc = window_event_handler,
        .hCursor = LoadCursorA(0, IDC_ARROW),
    };
    assert(RegisterClassA(&wndclass), "RegisterClassA() failed (%d)", GetLastError());
    g_ui.hwnd = CreateWindowExA(
        0,
        WNDCLASS_NAME,
        "Tiny Libretro Frontend",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
        0,
        0,
        g_ui.hinst,
        0
    );
    assert(g_ui.hwnd, "CreateWindowExA() failed (%d)", GetLastError());

    WNDCLASSA drawwndclass = {
        .style = CS_OWNDC,
        .hInstance = g_ui.hinst,
        .lpszClassName = DRAWWNDCLASS_NAME,
        .lpfnWndProc = DefWindowProcA,
        .hCursor = LoadCursorA(0, IDC_ARROW),
    };
    assert(RegisterClassA(&drawwndclass), "RegisterClassA() failed (%d)", GetLastError());
    g_ui.draw_hwnd = CreateWindowExA(
        0,
        DRAWWNDCLASS_NAME,
        0,
        WS_CHILD | WS_VISIBLE,
        0, 0, 0, 0,
        g_ui.hwnd,
        0,
        g_ui.hinst,
        0
    );
    assert(g_ui.hwnd, "CreateWindowExA() failed (%d)", GetLastError());

    g_ui.hdc = GetDC(g_ui.draw_hwnd);
    assert(g_ui.hdc, "GetDC() failed (%d)", GetLastError());
    PIXELFORMATDESCRIPTOR pfd = {0};
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.iLayerType = PFD_MAIN_PLANE;
    int pixel_format = ChoosePixelFormat(g_ui.hdc, &pfd);
    assert(DescribePixelFormat(g_ui.hdc, pixel_format, sizeof(pfd), &pfd), "DescribePixelFormat() failed (%d)", GetLastError());
    assert(SetPixelFormat(g_ui.hdc, pixel_format, &pfd), "SetPixelFormat() failed (%d)", GetLastError());
    ptr tmp_gl = wglCreateContext(g_ui.hdc);
    assert(tmp_gl, "wglCreateContext() failed (%d)", GetLastError());
    assert(wglMakeCurrent(g_ui.hdc, tmp_gl), "wglMakeCurrent() failed (%d)", GetLastError());
    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB
        = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
    assert(wglCreateContextAttribsARB, "wglGetProcAddress(\"wglCreateContextAttribsARB\") failed (%d)", GetLastError());
    PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT
        = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
    assert(wglSwapIntervalEXT, "wglGetProcAddress(\"wglSwapIntervalEXT\") failed (%d)", GetLastError());
    assert(wglMakeCurrent(0, 0), "wglMakeCurrent() failed (%d)", GetLastError());
    assert(wglDeleteContext(tmp_gl), "wglDeleteContext() failed (%d)", GetLastError());
    g_ui.hglrc = wglCreateContextAttribsARB(g_ui.hdc, 0, (i32[]){
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 3,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
        0
    });
    assert(g_ui.hglrc, "wglCreateContextAttribsARB() failed (%d)", GetLastError());
    assert(wglMakeCurrent(g_ui.hdc, g_ui.hglrc), "wglMakeCurrent() failed (%d)", GetLastError());
    assert(wglSwapIntervalEXT(1), "wglSwapIntervalEXT() failed (%d)", GetLastError());
    print("using %s on %s", glGetString(GL_VERSION), glGetString(GL_RENDERER));

    ShowWindow(g_ui.hwnd, SW_SHOW);
}

void load_core(cstr path)
{
    g_core.module = LoadLibraryA(path);
    if (!g_core.module)
    {
        print("error: LoadLibraryA(\"%s\") failed (%d)", path, GetLastError());
        return;
    }

    struct {
        cstr symbol;
        ptr *func;
    } load_list[] = {
        #define _X(_ret, _name, _arg1, ...) { #_name, (ptr)&g_core.api._name },
        RETRO_API_DECL_LIST
        #undef _X
    };
    u8 failed = false;
    for (u32 i = 0; i < countof(load_list); i++)
    {
        *load_list[i].func = (ptr)GetProcAddress(g_core.module, load_list[i].symbol);
        if (!(*load_list[i].func))
        {
            print("error: symbol \"%s\" not found", load_list[i].symbol);
            failed = true;
        }
    }
    if (failed) return;

    u32 api = g_core.api.retro_api_version();
    if (api != 1)
    {
        print("error: core uses unsupported API version %d", api);
        unload_core();
        return;
    }

    g_core.api.retro_get_system_info(&g_core.info);
    g_core.api.retro_get_system_av_info(&g_core.avinfo);

    g_core.api.retro_set_environment(core_environment_callback);
    g_core.api.retro_set_video_refresh(core_video_callback);
    g_core.api.retro_set_audio_sample(core_audio_sample_callback);
    g_core.api.retro_set_audio_sample_batch(core_audio_batch_callback);
    g_core.api.retro_set_input_poll(core_input_poll_callback);
    g_core.api.retro_set_input_state(core_input_state_callback);

    g_core.api.retro_init();

    print(
        "loaded %s (%d FPS, %dx%d)",
        g_core.info.library_name,
        (u64)g_core.avinfo.timing.fps,
        g_core.avinfo.geometry.max_width,
        g_core.avinfo.geometry.max_height
    );
}

void unload_core(void)
{
    if (g_core.module)
    {
        g_core.api.retro_unload_game();
        g_core.api.retro_deinit();
    }

    FreeLibrary(g_core.module);
    RtlZeroMemory(&g_core, sizeof(g_core));
}

void load_game(cstr path)
{
    assert(g_core.module, "attempted to load game when core was unloaded");

    retro_game_info info = { .path = path };
    if (!g_core.api.retro_load_game(&info))
    {
        print("error: failed to load game \"%s\"", path);
    }
}

void unload_game(void)
{
    assert(g_core.module, "attempted to unload game when core was unloaded");
    g_core.api.retro_unload_game();
}

void process_ui_events(void)
{
    for (MSG msg; PeekMessageA(&msg, g_ui.hwnd, 0, 0, PM_REMOVE) != 0;)
    {
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
}

i64 window_event_handler(ptr hwnd, u32 msg, u64 wp, i64 lp)
{
    if (hwnd != g_ui.hwnd)
        return DefWindowProcA(hwnd, msg, wp, lp);

    switch (msg)
    {
    case WM_CLOSE:
        g_ui.was_exit_requested = true;
        ShowWindow(g_ui.hwnd, SW_HIDE);
        break;

    case WM_SIZE:
        RECT wr;
        assert(GetClientRect(g_ui.hwnd, &wr), "GetClientRect() failed (%d)", GetLastError());
        RECT r = {
            0, 0, wr.right, wr.bottom,
        };
        assert(SetWindowPos(g_ui.draw_hwnd, 0, r.left, r.top, r.right, r.bottom, 0), "SetWindowPos() failed (%d)", GetLastError());
        break;
    }
    
    return DefWindowProcA(hwnd, msg, wp, lp);
}

void core_log_callback(u32 level, cstr format, ...)
{
    (void)level;

    c8 buffer[1024];

    va_list args;
    va_start(args, format);
    u32 len = vsnprintf(buffer, sizeof(buffer), format, args);

    for (u32 i = 0; i < len; i++) if (buffer[i] < ' ' || buffer[i] > '~') buffer[i] = ' ';

    print("%s", buffer);
}

u8 core_environment_callback(u32 cmd, ptr data)
{
    switch (cmd)
    {
    case RETRO_ENVIRONMENT_GET_LOG_INTERFACE:
        ((retro_log_callback*)data)->log = core_log_callback;
        return true;

    case RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY:
        *(cstr*)data = "system";
        return true;

    case RETRO_ENVIRONMENT_GET_SAVE_DIRECTORY:
        *(cstr*)data = "save";
        return true;
    }

    print("unhandled core command %u (%p)", cmd, data);
    return false;
}

void core_video_callback(ptr data, u32 width, u32 height, u64 pitch)
{
    (void)data; (void)width; (void)height; (void)pitch;
}

void core_audio_sample_callback(i16 left, i16 right)
{
    (void)left; (void)right;
}

u64 core_audio_batch_callback(i16 *data, u64 frames)
{
    (void)data; (void)frames;
    return 0;
}

void core_input_poll_callback(void)
{
}

i16 core_input_state_callback(u32 port, u32 device, u32 index, u32 id)
{
    (void)port; (void)device; (void)index; (void)id;
    return 0;
}

void present_frame(void)
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    SwapBuffers(g_ui.hdc);
}

void print(cstr format, ...)
{
    c8 buffer[1024];

    va_list args;
    va_start(args, format);
    u32 len = vsnprintf(buffer, sizeof(buffer) - 2, format, args);
    buffer[len++] = '\n';

    WriteConsoleA(g_stdout, buffer, len, 0, 0);

    buffer[len++] = '\0';
    OutputDebugStringA(buffer);
}

u32 snprintf(c8 *buffer, u32 maxsize, cstr format, ...)
{
    va_list args;
    va_start(args, format);
    return vsnprintf(buffer, maxsize, format, args);
}

u32 vsnprintf(c8 *buffer, u32 maxsize, cstr format, va_list args)
{
    if (!maxsize) return 0;

    u32 fi = 0, bi = 0;

    while (bi < maxsize - 1 && format[fi])
    {
        if (format[fi] != '%')
        {
            buffer[bi++] = format[fi++];
        }
        else
        {
            fi++;

            cstr str;
            u64 ui, ud;
            i64 si, sd;
            f64 f;
            u8 size = 4, base = 10;

            const c8 table[] = {
                '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
            };

            switch (format[fi])
            {
            case 'h':
                size = 2;
                fi++;
                if (format[fi] == 'h') { size = 1; fi++; }
                break;

            case 'l':
                size = 4;
                fi++;
                if (format[fi] == 'l') { size = 8; fi++; }
                break;
            }

            switch (format[fi])
            {
            case '%':
                buffer[bi++] = '%';
                fi++;
                break;

            case 's':
                str = va_arg(args, cstr);
                str = (str) ? (str) : ("(null)");
                while (bi < maxsize - 1 && *str) buffer[bi++] = *(str++);
                fi++;
                break;

            case 'i':
            case 'd':
                si = va_arg(args, i64);
                if      (size == 1) si = (i8)si;
                else if (size == 2) si = (i16)si;
                else if (size == 4) si = (i32)si;
                if (si < 0 && bi < maxsize - 1) { buffer[bi++] = '-'; si *= -1; }
                for (sd = 1; si / sd >= base; sd *= base);
                while (bi < maxsize - 1 && sd > 0) { buffer[bi++] = table[si / sd]; si %= sd; sd /= 10; }
                fi++;
                break;

            case 'x':
            case 'p':
            case 'b':
                base = (format[fi] == 'b') ? (2) : (16);

            case 'u':
                ui = va_arg(args, u64);
                if      (size == 1) ui = (u8)ui;
                else if (size == 2) ui = (u16)ui;
                else if (size == 4) ui = (u32)ui;
                for (ud = 1; ui / ud >= base; ud *= base);
                while (bi < maxsize - 1 && ud > 0) { buffer[bi++] = table[ui / ud]; ui %= ud; ud /= base; }
                fi++;
                break;

            case 'f':
                f = va_arg(args, f64);
                if (size == 4) f = (f32)f;
                if (f < 0 && bi < maxsize - 1) { buffer[bi++] = '-'; f *= -1; }
                ui = (u64)(f * 1000.0f);
                for (ud = 1; ui / ud >= 10; ud *= 10);
                while (bi < maxsize - 1 && ud > 0) 
                {
                    buffer[bi++] = table[ui / ud]; 
                    ui %= ud;
                    ud /= 10;
                    if (ud == 100 && bi < maxsize - 1) buffer[bi++] = '.';
                }
                fi++;
                break;
            }
        }
    }
    buffer[bi] = '\0';

    return bi;
}
