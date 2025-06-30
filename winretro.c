/* defines */
#define true 1u
#define false 0

#define WNDCLASS_NAME                    "libretro-frontend"
#define DRAWWNDCLASS_NAME                "libretro-frontend-draw"

#define GENERIC_READ                     0x80000000
#define GENERIC_WRITE                    0x40000000
#define FILE_SHARE_READ                  0x00000001
#define CREATE_ALWAYS                    2
#define OPEN_EXISTING                    3
#define OPEN_ALWAYS                      4
#define FILE_ATTRIBUTE_NORMAL            0x00000080
#define FILE_ATTRIBUTE_DIRECTORY         0x00000010
#define INVALID_FILE_SIZE                0xFFFFFFFFu
#define ERROR_FILE_NOT_FOUND             2
#define ERROR_ALREADY_EXISTS             183
#define HEAP_GENERATE_EXCEPTIONS         0x00000004
#define HEAP_ZERO_MEMORY                 0x00000008
#define INVALID_HANDLE_VALUE             (ptr)-1
#define ATTACH_PARENT_PROCESS            (u32)-1
#define STD_OUTPUT_HANDLE                (u32)-11
#define IDC_ARROW                        ((cstr)32512)
#define WS_OVERLAPPEDWINDOW              0x00CF0000L
#define WS_CHILD                         0x40000000L
#define WS_VISIBLE                       0x10000000L
#define CW_USEDEFAULT                    0x80000000
#define SW_HIDE                          0
#define SW_SHOWNORMAL                    1
#define SW_SHOW                          5
#define WM_CLOSE                         0x0010
#define WM_SIZE                          0x0005
#define WM_COMMAND                       0x0111
#define PM_REMOVE                        0x0001
#define CS_OWNDC                         0x0020
#define PFD_DOUBLEBUFFER                 0x00000001
#define PFD_DRAW_TO_WINDOW               0x00000004
#define PFD_SUPPORT_OPENGL               0x00000020
#define PFD_TYPE_RGBA                    0
#define PFD_MAIN_PLANE                   0
#define OFN_PATHMUSTEXIST                0x00000800
#define OFN_FILEMUSTEXIST                0x00001000
#define OFN_OVERWRITEPROMPT              0x00000002

#define MF_POPUP                         0x00000010L
#define MF_ENABLED                       0x00000000L
#define MF_DISABLED                      0x00000002L
#define MF_BYCOMMAND                     0x00000000L
#define MF_SEPARATOR                     0x00000800L

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

#define RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY    9
#define RETRO_ENVIRONMENT_GET_VARIABLE            15
#define RETRO_ENVIRONMENT_SET_VARIABLES           16
#define RETRO_ENVIRONMENT_GET_LOG_INTERFACE       27
#define RETRO_ENVIRONMENT_GET_SAVE_DIRECTORY      31
#define RETRO_ENVIRONMENT_GET_PREFERRED_HW_RENDER 56
#define RETRO_ENVIRONMENT_EXPERIMENTAL            0x10000

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

#define INVALID_IDX ((u32)-1)

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
typedef struct OPENFILENAMEA OPENFILENAMEA;
struct OPENFILENAMEA {
    u32  lStructSize;
    ptr  hwndOwner;
    ptr  hInstance;
    cstr lpstrFilter;
    c8  *lpstrCustomFilter;
    u32  nMaxCustFilter;
    u32  nFilterIndex;
    c8  *lpstrFile;
    u32  nMaxFile;
    c8  *lpstrFileTitle;
    u32  nMaxFileTitle;
    cstr lpstrInitialDir;
    cstr lpstrTitle;
    u32  Flags;
    u16  nFileOffset;
    u16  nFileExtension;
    cstr lpstrDefExt;
    i64  lCustData;
    ptr  lpfnHook;
    cstr lpTemplateName;
    ptr  pvReserved;
    u32  dwReserved;
    u32  FlagsEx;
};

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

typedef struct retro_variable retro_variable;
struct retro_variable {
    cstr key;
    cstr value;
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

typedef enum retro_hw_context_type retro_hw_context_type;
enum retro_hw_context_type {
   RETRO_HW_CONTEXT_NONE             = 0,
   RETRO_HW_CONTEXT_OPENGL           = 1,
   RETRO_HW_CONTEXT_OPENGLES2        = 2,
   RETRO_HW_CONTEXT_OPENGL_CORE      = 3,
   RETRO_HW_CONTEXT_OPENGLES3        = 4,
   RETRO_HW_CONTEXT_OPENGLES_VERSION = 5,
};


/* macros */
#define countof(_a) (sizeof(_a)/sizeof((_a)[0]))
#define va_create(_last_arg) ((u8*)&(_last_arg) + 8)
#define va_arg(_list, _T) ( ( sizeof(_T) > 8) ? (**(_T**)((_list += 8) - 8)) : ( *(_T* )((_list += 8) - 8)) )

#if defined(_MSC_VER)
    #define assert(_x) do { \
            if (!(_x)) { \
                print("assertion " #_x " failed (%s():%d)", __func__, __LINE__); \
                __debugbreak(); \
            } \
        } while (0)
#else
    #define assert(_x) do { \
            if (!(_x)) { \
                print("assertion " #_x " failed (%s():%d)", __func__, __LINE__); \
                __asm__("int3"); \
                __builtin_unreachable(); \
            } \
        } while (0)
#endif
#define assertp(_x, _m, ...) do { \
        if (!(_x)) { \
            print(_m, ##__VA_ARGS__); \
            assert(0); \
        } \
    } while (0)
#define check_goto(_x, _label) do { if(!(_x)) goto _label } while (0)
#define checkp_goto(_x, _label, _m, ...) do { \
        if (!(_x)) { \
            print(_m, ##__VA_ARGS__); \
            goto _label; \
        } \
    } while (0)
#define checkp_return(_x, _label, _m, ...) do { \
        if (!(_x)) { \
            print(_m, ##__VA_ARGS__); \
            return; \
        } \
    } while (0)

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
u32  IsDebuggerPresent(void);
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
i64  SendMessageA(ptr hwnd, u32 msg, u64 wp, i64 lp);
u32  ShowWindow(ptr hwnd, i32 show);
u32  PeekMessageA(MSG *msg, ptr hwnd, u32 fmin, u32 fmax, u32 flags);
u32  TranslateMessage(MSG *msg);
u32  DispatchMessageA(MSG *msg);
u32  GetClientRect(ptr hwnd, RECT *r);
u32  AppendMenuA(ptr hmenu, u32 flags, u64 item, cstr str);
u32  ModifyMenuA(ptr hmenu, u32 id, u32 flags, u64 item, cstr str);
u32  SetWindowPos(ptr hwnd, ptr insert_type, i32 x, i32 y, i32 w, i32 h, u32 flags);
ptr  GetDC(ptr hwnd);
u32  DestroyWindow(ptr hwnd);
u32  UnregisterClassA(cstr cls, ptr hinst);
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
ptr  CreateFileA(cstr path, u32 access, u32 share, ptr security, u32 create, u32 flags, ptr template);
u32  ReadFile(ptr file, ptr buffer, u32 bytes_to_read, u32 *bytes_read, ptr overlapped);
u32  WriteFile(ptr file, ptr buffer, u32 bytes_to_write, u32 *bytes_written, ptr overlapped);
u32  GetFileSize(ptr file, u32 *msdword);
u32  CloseHandle(ptr handle);
ptr  GetProcessHeap(void);
ptr  HeapAlloc(ptr heap, u32 flags, u64 size);
ptr  HeapReAlloc(ptr heap, u32 flags, ptr memory, u64 size);
u32  HeapFree(ptr heap, u32 flags, ptr memory);
ptr  CreateMenu(void);
u32  WINAPI GetOpenFileNameA(OPENFILENAMEA *ofn);
u32  WINAPI GetSaveFileNameA(OPENFILENAMEA *ofn);
ptr  ShellExecuteA(ptr hwnd, cstr op, cstr file, cstr params, cstr dir, i32 show);
u32  GetModuleFileNameA(ptr module, c8 *path, u32 pathmax);
u32  GetFileAttributesA(cstr path);
u32  CreateDirectoryA(cstr path, ptr security);


/* function declarations */
void _start(void);
void init_paths(void);
void init_logging(void);
void init_ui(void);
void free_ui(void);
void load_core(cstr path);
void load_core_variables(void);
void save_core_variables(void);
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
u32  str_equals(cstr a, cstr b, u32 minsize);
cstr get_core_variable(cstr key);
void update_core_variable(cstr key, cstr value);
u32  get_core_variable_idx(cstr key);
void print(cstr format, ...);
u8   open_file_dialog(u8 save, cstr title, cstr filename, cstr extensions, c8 *path, u32 pathmax);
u32  snprintf(c8 *buffer, u32 maxsize, cstr format, ...);
u32  vsnprintf(c8 *buffer, u32 maxsize, cstr format, va_list args);


/* variables */
u32 _fltused=1;

struct {
    u32 enabled;
    ptr stdout;
} g_log;

struct {
    ptr hwnd;
    ptr draw_hwnd;
    ptr hinst;
    ptr hmenu;
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
    struct {
        u32 count;
        u32 allocated;
        struct {
            c8 key[64];
            c8 value[32];
        } *array;
    } vars;
} g_core;

struct {
    c8 root[512];
    c8 save[512];
    c8 bios[512];
    c8 settings[512];
    c8 settings_file[512];
} g_paths;


/* function definitions */
void _start(void)
{
    init_logging();
    init_paths();
    init_ui();

    while (!g_ui.was_exit_requested)
    {
        process_ui_events();
        present_frame();
    }

    unload_core();
    free_ui();
    ExitProcess(0);
}

void init_paths(void)
{
    u32 l = GetModuleFileNameA(0, g_paths.root, sizeof(g_paths.root) - 1);
    while (g_paths.root[l] != '\\') l--;
    g_paths.root[l] = '\0';

    snprintf(g_paths.save, sizeof(g_paths.save), "%s\\save", g_paths.root);
    snprintf(g_paths.bios, sizeof(g_paths.bios), "%s\\bios", g_paths.root);
    snprintf(g_paths.settings, sizeof(g_paths.settings), "%s\\settings", g_paths.root);
    g_paths.settings_file[0] = '\0';

    cstr array[] = { g_paths.save, g_paths.bios, g_paths.settings };
    for (u32 i = 0; i < countof(array); i++)
    {
        if (!CreateDirectoryA(array[i], 0) && GetLastError() != ERROR_ALREADY_EXISTS)
            print("CreateDirectoryA(\"%s\") failed (%d)", array[i], GetLastError());
    }
}

void init_logging(void)
{
    AttachConsole(ATTACH_PARENT_PROCESS);
    g_log.stdout = GetStdHandle(STD_OUTPUT_HANDLE);
    g_log.enabled = g_log.stdout != INVALID_HANDLE_VALUE || IsDebuggerPresent();
}

void init_ui(void)
{
    g_ui.hinst = GetModuleHandleA(0);

    ptr file_menu = CreateMenu();
    assert(AppendMenuA(file_menu, 0, MENU_OPEN_CORE_ID,  MENU_OPEN_CORE_STR));
    assert(AppendMenuA(file_menu, 0, MENU_OPEN_ROM_ID,   MENU_OPEN_ROM_STR));
    assert(AppendMenuA(file_menu, 0, MENU_LOAD_STATE_ID, MENU_LOAD_STATE_STR));
    assert(AppendMenuA(file_menu, 0, MENU_SAVE_STATE_ID, MENU_SAVE_STATE_STR));
    assert(AppendMenuA(file_menu, MF_SEPARATOR, 0, 0));
    assert(AppendMenuA(file_menu, 0, MENU_EXIT_ID, "Exit"));
    ptr help_menu = CreateMenu();
    assert(AppendMenuA(help_menu, 0, MENU_OPEN_WEBSITE_ID, MENU_OPEN_WEBSITE_STR));
    g_ui.hmenu = CreateMenu();
    assert(AppendMenuA(g_ui.hmenu, MF_POPUP, (u64)file_menu, "File"));
    assert(AppendMenuA(g_ui.hmenu, MF_POPUP, (u64)help_menu, "Help"));

    ModifyMenuA(g_ui.hmenu, MENU_OPEN_ROM_ID,   MF_DISABLED, MENU_OPEN_ROM_ID,   MENU_OPEN_ROM_STR);
    ModifyMenuA(g_ui.hmenu, MENU_LOAD_STATE_ID, MF_DISABLED, MENU_LOAD_STATE_ID, MENU_LOAD_STATE_STR);
    ModifyMenuA(g_ui.hmenu, MENU_SAVE_STATE_ID, MF_DISABLED, MENU_SAVE_STATE_ID, MENU_SAVE_STATE_STR);

    WNDCLASSA wndclass = {
        .hInstance = g_ui.hinst,
        .lpszClassName = WNDCLASS_NAME,
        .lpfnWndProc = window_event_handler,
        .hCursor = LoadCursorA(0, IDC_ARROW),
    };
    assert(RegisterClassA(&wndclass));
    g_ui.hwnd = CreateWindowExA(
        0,
        WNDCLASS_NAME,
        "Tiny Libretro Frontend",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
        0,
        g_ui.hmenu,
        g_ui.hinst,
        0
    );
    assert(g_ui.hwnd);

    WNDCLASSA drawwndclass = {
        .style = CS_OWNDC,
        .hInstance = g_ui.hinst,
        .lpszClassName = DRAWWNDCLASS_NAME,
        .lpfnWndProc = DefWindowProcA,
        .hCursor = LoadCursorA(0, IDC_ARROW),
    };
    assert(RegisterClassA(&drawwndclass));
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
    assert(g_ui.hwnd);

    g_ui.hdc = GetDC(g_ui.draw_hwnd);
    assert(g_ui.hdc);
    PIXELFORMATDESCRIPTOR pfd = {0};
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.iLayerType = PFD_MAIN_PLANE;
    int pixel_format = ChoosePixelFormat(g_ui.hdc, &pfd);
    assert(DescribePixelFormat(g_ui.hdc, pixel_format, sizeof(pfd), &pfd));
    assert(SetPixelFormat(g_ui.hdc, pixel_format, &pfd));
    ptr tmp_gl = wglCreateContext(g_ui.hdc);
    assert(tmp_gl);
    assert(wglMakeCurrent(g_ui.hdc, tmp_gl));
    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB
        = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
    assert(wglCreateContextAttribsARB);
    PFNWGLSWAPINTERVALEXTPROC wglSwapIntervalEXT
        = (PFNWGLSWAPINTERVALEXTPROC)wglGetProcAddress("wglSwapIntervalEXT");
    assert(wglSwapIntervalEXT);
    assert(wglMakeCurrent(0, 0));
    assert(wglDeleteContext(tmp_gl));
    g_ui.hglrc = wglCreateContextAttribsARB(g_ui.hdc, 0, (i32[]){
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 3,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
        0
    });
    assert(g_ui.hglrc);
    assert(wglMakeCurrent(g_ui.hdc, g_ui.hglrc));
    assert(wglSwapIntervalEXT(1));
    print("using %s on %s", glGetString(GL_VERSION), glGetString(GL_RENDERER));

    ShowWindow(g_ui.hwnd, SW_SHOW);
}

void free_ui(void)
{
    DestroyWindow(g_ui.draw_hwnd);
    DestroyWindow(g_ui.hwnd);
    UnregisterClassA(DRAWWNDCLASS_NAME, g_ui.hinst);
    UnregisterClassA(WNDCLASS_NAME, g_ui.hinst);
    RtlZeroMemory(&g_ui, sizeof(g_ui));
}

void load_core(cstr path)
{
    unload_core();

    g_core.vars.allocated = 8;
    g_core.vars.array = HeapAlloc(
        GetProcessHeap(),
        HEAP_GENERATE_EXCEPTIONS | HEAP_ZERO_MEMORY,
        sizeof(g_core.vars.array[0]) * g_core.vars.allocated
    );

    g_core.module = LoadLibraryA(path);
    checkp_goto(g_core.module, Failure, "LoadLibraryA(\"%s\") failed (%d)", path, GetLastError());

    struct {
        cstr symbol;
        ptr *func;
    } load_list[] = {
        #define _X(_ret, _name, _arg1, ...) { #_name, (ptr)&g_core.api._name },
        RETRO_API_DECL_LIST
        #undef _X
    };
    for (u32 i = 0; i < countof(load_list); i++)
    {
        *load_list[i].func = (ptr)GetProcAddress(g_core.module, load_list[i].symbol);
        checkp_goto(*load_list[i].func, Failure, "symbol \"%s\" not found", load_list[i].symbol);
    }

    u32 api = g_core.api.retro_api_version();
    checkp_goto(api == 1, Failure, "core uses unsupported API version %d", api);

    g_core.api.retro_get_system_info(&g_core.info);

    snprintf(g_paths.settings_file, sizeof(g_paths.settings_file), "%s\\%s.ini", g_paths.settings, g_core.info.library_name);
    load_core_variables();

    g_core.api.retro_set_environment(core_environment_callback);
    g_core.api.retro_set_video_refresh(core_video_callback);
    g_core.api.retro_set_audio_sample(core_audio_sample_callback);
    g_core.api.retro_set_audio_sample_batch(core_audio_batch_callback);
    g_core.api.retro_set_input_poll(core_input_poll_callback);
    g_core.api.retro_set_input_state(core_input_state_callback);

    g_core.api.retro_init();

    g_core.api.retro_get_system_av_info(&g_core.avinfo);

    ModifyMenuA(g_ui.hmenu, MENU_OPEN_ROM_ID, MF_ENABLED,  MENU_OPEN_ROM_ID,   MENU_OPEN_ROM_STR);

    print(
        "loaded %s (%d FPS, %dx%d)",
        g_core.info.library_name,
        (u64)g_core.avinfo.timing.fps,
        g_core.avinfo.geometry.max_width,
        g_core.avinfo.geometry.max_height
    );

    return;
    Failure:
    unload_core();
}

void load_core_variables(void)
{
    c8 *contents = 0;

    ptr file = CreateFileA(
        g_paths.settings_file,
        GENERIC_READ,
        FILE_SHARE_READ,
        0,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        0
    );
    checkp_goto(file != INVALID_HANDLE_VALUE, Failure, "CreateFileA(\"%s\") failed (%d)", g_paths.settings_file, GetLastError());

    u32 size = GetFileSize(file, 0);
    checkp_goto(size != INVALID_FILE_SIZE, Failure, "GetFileSize() failed (%d)", GetLastError());

    contents = HeapAlloc(GetProcessHeap(), HEAP_GENERATE_EXCEPTIONS, size);
    u32 bytes_read = 0;
    u32 ok = ReadFile(file, contents, size, &bytes_read, 0);
    checkp_goto(ok && bytes_read == size, Failure, "ReadFile(\"%s\") failed (%d, %u/%u B)", g_paths.settings_file, GetLastError(), bytes_read, size);

    for (u32 i = 0; i < size; )
    {
        c8 key[256];
        c8 value[256];

        while (i < size && (contents[i] == ' ' || contents[i] == '\n')) i++;

        u32 j;
        for (j = 0; j < countof(key) - 1 && i < size && contents[i] != '='; j++, i++)
            key[j] = contents[i];
        key[j] = '\0';
        i++;

        for (j = 0; j < countof(value) - 1 && i < size && contents[i] != '\n'; j++, i++)
            value[j] = contents[i];
        value[j] = '\0';
        i++;

        if (!key[0]) continue;

        update_core_variable(key, value);
    }

    print("loaded core settings from \"%s\"", g_paths.settings_file);

    Failure:
    CloseHandle(file);
    HeapFree(GetProcessHeap(), 0, contents);
    return;
}

void save_core_variables(void)
{
    c8 *contents = 0;

    ptr file = CreateFileA(
        g_paths.settings_file,
        GENERIC_WRITE,
        FILE_SHARE_READ,
        0,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        0
    );
    checkp_goto(file != INVALID_HANDLE_VALUE, Failure, "CreateFileA(\"%s\") failed (%d)", g_paths.settings_file, GetLastError());

    u32 size = sizeof(g_core.vars.array[0]) * g_core.vars.count + 1;
    contents = HeapAlloc(GetProcessHeap(), HEAP_GENERATE_EXCEPTIONS, size);

    u32 bytes_to_write = 0;
    for (u32 j = 0; j < g_core.vars.count; j++)
    {
        bytes_to_write += snprintf(
            contents + bytes_to_write,
            size - bytes_to_write,
            "%s=%s\n",
            g_core.vars.array[j].key,
            g_core.vars.array[j].value
        );
        assert(bytes_to_write <= size);
    }

    u32 bytes_written = 0;
    u32 ok = WriteFile(file, contents, bytes_to_write, &bytes_written, 0);
    checkp_goto(ok && bytes_written == bytes_to_write, Failure, "WriteFile(\"%s\") failed (%d, %u/%u B)", g_paths.settings_file, GetLastError(), bytes_written, bytes_to_write);

    print("saved core settings to \"%s\"", g_paths.settings_file);

    Failure:
    CloseHandle(file);
    HeapFree(GetProcessHeap(), 0, contents);
    return;
}

void unload_core(void)
{
    ModifyMenuA(g_ui.hmenu, MENU_OPEN_ROM_ID,   MF_DISABLED, MENU_OPEN_ROM_ID,   MENU_OPEN_ROM_STR);
    ModifyMenuA(g_ui.hmenu, MENU_LOAD_STATE_ID, MF_DISABLED, MENU_LOAD_STATE_ID, MENU_LOAD_STATE_STR);
    ModifyMenuA(g_ui.hmenu, MENU_SAVE_STATE_ID, MF_DISABLED, MENU_SAVE_STATE_ID, MENU_SAVE_STATE_STR);

    if (g_core.module)
    {
        save_core_variables();
        g_core.api.retro_unload_game();
        g_core.api.retro_deinit();
    }

    g_paths.settings_file[0] = '\0';
    HeapFree(GetProcessHeap(), 0, g_core.vars.array);
    FreeLibrary(g_core.module);
    RtlZeroMemory(&g_core, sizeof(g_core));
}

void load_game(cstr path)
{
    assertp(g_core.module, "attempted to load game when core was unloaded");

    unload_game();

    retro_game_info info = { .path = path };
    checkp_return(g_core.api.retro_load_game(&info), "failed to load game \"%s\"", path);

    ModifyMenuA(g_ui.hmenu, MENU_LOAD_STATE_ID, MF_ENABLED, MENU_LOAD_STATE_ID, MENU_LOAD_STATE_STR);
    ModifyMenuA(g_ui.hmenu, MENU_SAVE_STATE_ID, MF_ENABLED, MENU_SAVE_STATE_ID, MENU_SAVE_STATE_STR);
}

void unload_game(void)
{
    ModifyMenuA(g_ui.hmenu, MENU_LOAD_STATE_ID, MF_DISABLED, MENU_LOAD_STATE_ID, MENU_LOAD_STATE_STR);
    ModifyMenuA(g_ui.hmenu, MENU_SAVE_STATE_ID, MF_DISABLED, MENU_SAVE_STATE_ID, MENU_SAVE_STATE_STR);

    if (!g_core.module)
        return;

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

    c8 path[256];

    switch (msg)
    {
    case WM_CLOSE:
        g_ui.was_exit_requested = true;
        ShowWindow(g_ui.hwnd, SW_HIDE);
        break;

    case WM_SIZE:
        RECT wr;
        assert(GetClientRect(g_ui.hwnd, &wr));
        RECT r = {
            0, 0, wr.right, wr.bottom,
        };
        assert(SetWindowPos(g_ui.draw_hwnd, 0, r.left, r.top, r.right, r.bottom, 0));
        break;

    case WM_COMMAND:
        switch ((u16)wp)
        {
        case MENU_EXIT_ID:
            SendMessageA(g_ui.hwnd, WM_CLOSE, 0, 0);
            break;

        case MENU_OPEN_CORE_ID:
            if (open_file_dialog(false, MENU_OPEN_CORE_STR, 0, "Libretro Core (*.dll)\0*.DLL\0", path, sizeof(path)))
                load_core(path);
            break;

        case MENU_OPEN_ROM_ID:
            if (open_file_dialog(false, MENU_OPEN_ROM_STR, 0, "Game Rom (*.chd)\0*.CHD\0Game Rom (*.bin)\0*.BIN\0All Files (*.*)\0*.*\0", path, sizeof(path)))
                load_game(path);
            break;

        case MENU_LOAD_STATE_ID:
            if (open_file_dialog(false, MENU_LOAD_STATE_STR, 0, "Game State (*.bin)\0*.BIN\0All Files (*.*)\0*.*\0", path, sizeof(path)))
                print("load state from \"%s\"", path);
            break;

        case MENU_SAVE_STATE_ID:
            if (open_file_dialog(true, MENU_SAVE_STATE_STR, "save.bin", "Game State (*.bin)\0*.BIN\0All Files (*.*)\0*.*\0", path, sizeof(path)))
                print("save state to \"%s\"", path);
            break;

        case MENU_OPEN_WEBSITE_ID:
            ShellExecuteA(0, "open", "https://github.com/xfnty/tiny-libretro-frontend", 0, 0, SW_SHOWNORMAL);
            break;
        }
        break;
    }
    
    return DefWindowProcA(hwnd, msg, wp, lp);
}

void core_log_callback(u32 level, cstr format, ...)
{
    (void)level;
    c8 buffer[1024];
    u32 len = vsnprintf(buffer, sizeof(buffer), format, va_create(format));
    for (u32 i = 0; i < len; i++) if (buffer[i] < ' ' || buffer[i] > '~') buffer[i] = ' ';
    print("%s", buffer);
}

u8 core_environment_callback(u32 cmd, ptr data)
{
    cmd &= ~RETRO_ENVIRONMENT_EXPERIMENTAL;
    
    switch (cmd)
    {
    case RETRO_ENVIRONMENT_GET_LOG_INTERFACE:
        ((retro_log_callback*)data)->log = core_log_callback;
        return true;

    case RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY:
        *(cstr*)data = g_paths.bios;
        return true;

    case RETRO_ENVIRONMENT_GET_SAVE_DIRECTORY:
        *(cstr*)data = g_paths.save;
        return true;

    case RETRO_ENVIRONMENT_GET_PREFERRED_HW_RENDER:
        *(u32*)data = RETRO_HW_CONTEXT_OPENGL_CORE;
        return true;

    case RETRO_ENVIRONMENT_SET_VARIABLES:
        for (retro_variable *v = data; v->key; v++)
        {
            if (get_core_variable(v->key))
                continue;

            c8 value[64] = {0};
            u32 i = 0, j = 0;
            while (v->value[i] && v->value[i] != ';') i++;
            i++;
            while (v->value[i] && v->value[i] == ' ') i++;
            while (v->value[i] && v->value[i] != '|' && j < sizeof(value) - 1) value[j++] = v->value[i++];
            update_core_variable(v->key, value);
        }
        return true;

    case RETRO_ENVIRONMENT_GET_VARIABLE:
        retro_variable *v = data;
        return (v->value = get_core_variable(v->key)) != 0;
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

u32 str_equals(cstr a, cstr b, u32 minsize)
{
    for (u32 i = 0; i < minsize && a[i] == b[i]; i++)
        if (!a[i]) return true;
    return false;
}

u32 get_core_variable_idx(cstr key)
{
    for (u32 i = 0; i < g_core.vars.count; i++)
        if (str_equals(key, g_core.vars.array[i].key, sizeof(g_core.vars.array[i].key)))
            return i;
    return INVALID_IDX;
}

cstr get_core_variable(cstr key)
{
    u32 i = get_core_variable_idx(key);
    return (i != INVALID_IDX) ? (g_core.vars.array[i].value) : (0);
}

void update_core_variable(cstr key, cstr value)
{
    u32 var_idx = get_core_variable_idx(key);
    if (var_idx == INVALID_IDX) var_idx = g_core.vars.count;

    if (g_core.vars.count == g_core.vars.allocated)
    {
        g_core.vars.allocated <<= 1;
        g_core.vars.array = HeapReAlloc(
            GetProcessHeap(),
            HEAP_GENERATE_EXCEPTIONS | HEAP_ZERO_MEMORY,
            g_core.vars.array,
            sizeof(g_core.vars.array[0]) * g_core.vars.allocated
        );
    }

    snprintf(g_core.vars.array[var_idx].key, sizeof(g_core.vars.array[var_idx].key), "%s", key);
    snprintf(g_core.vars.array[var_idx].value, sizeof(g_core.vars.array[var_idx].value), "%s", value);

    if (var_idx == g_core.vars.count)
        g_core.vars.count++;
}

void present_frame(void)
{
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);
    SwapBuffers(g_ui.hdc);
}

u8 open_file_dialog(u8 save, cstr title, cstr filename, cstr extensions, c8 *path, u32 pathmax)
{
    path[0] = '\0';
    if (filename) snprintf(path, pathmax, "%s", filename);

    OPENFILENAMEA info = {
        .lStructSize = sizeof(info),
        .lpstrTitle = title,
        .hwndOwner = g_ui.hwnd,
        .lpstrFilter = extensions,
        .nFilterIndex = 1,
        .lpstrFile = path,
        .nMaxFile = pathmax,
        .Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | ((save) ? (OFN_OVERWRITEPROMPT) : (0)),
    };

    return (u8)((save) ? (GetSaveFileNameA(&info)) : (GetOpenFileNameA(&info)));
}

void print(cstr format, ...)
{
    if (!g_log.enabled)
        return;

    c8 buffer[1024];

    u32 len = vsnprintf(buffer, sizeof(buffer) - 2, format, va_create(format));
    buffer[len++] = '\n';
    WriteConsoleA(g_log.stdout, buffer, len, 0, 0);
    buffer[len++] = '\0';
    OutputDebugStringA(buffer);
}

u32 snprintf(c8 *buffer, u32 maxsize, cstr format, ...)
{
    return vsnprintf(buffer, maxsize, format, va_create(format));
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
