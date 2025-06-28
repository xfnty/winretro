/* defines */
#define true 1u
#define false 0
#define WNDCLASS_NAME "libretro-frontend"
#define INVALID_HANDLE_VALUE  (ptr)-1
#define ATTACH_PARENT_PROCESS (u32)-1
#define STD_OUTPUT_HANDLE     (u32)-11
#define IDC_ARROW             ((cstr)32512)
#define WS_OVERLAPPEDWINDOW   0x00CF0000L
#define CW_USEDEFAULT         0x80000000
#define SW_SHOW               5
#define SW_HIDE               0
#define WM_CLOSE              0x0010u
#define PM_REMOVE             0x0001u

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


/* macros */
#define countof(_a) (sizeof(_a)/sizeof((_a)[0]))
#define va_start(_list, _last_arg) do { _list = (u8*)&(_last_arg) + 8; } while (0)
#define va_arg(_list, _T) ( ( sizeof(_T) > 8) ? (**(_T**)((_list += 8) - 8)) : ( *(_T* )((_list += 8) - 8)) )

#if defined(_MSC_VER)
    #define assert(_x, _m, ...) do { \
            if (!(_x)) { \
                print("error: " _m, ##__VA_ARGS__); \
                __debugbreak(); \
            } \
        } while (0)
#else
    #define assert(_x, _m, ...) do { \
            if (!(_x)) { \
                print("error: " _m, ##__VA_ARGS__); \
                __asm__("int3"); \
                __builtin_unreachable(); \
            } \
        } while (0)
#endif


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
u32 ShowWindow(ptr hwnd, i32 show);
u32 PeekMessageA(MSG *msg, ptr hwnd, u32 fmin, u32 fmax, u32 flags);
u32 TranslateMessage(MSG *msg);
u32 DispatchMessageA(MSG *msg);


/* function declarations */
void _start(void);
void init_logging(void);
void init_ui(void);
void process_ui_events(void);
i64 WINAPI window_event_handler(ptr hwnd, u32 msg, u64 wp, i64 lp);
void print(cstr format, ...);
void die(cstr format, ...);
u32 snprintf(c8 *buffer, u32 maxsize, cstr format, ...);
u32 vsnprintf(c8 *buffer, u32 maxsize, cstr format, va_list args);


/* variables */
u32 _fltused=1;
ptr g_stdout;
struct {
    ptr hwnd;
    ptr hinst;
    u32 was_exit_requested;
} g_ui;


/* function definitions */
void _start(void)
{
    init_logging();
    init_ui();

    while (!g_ui.was_exit_requested)
    {
        process_ui_events();
    }

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

    ShowWindow(g_ui.hwnd, SW_SHOW);
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
    }
    
    return DefWindowProcA(hwnd, msg, wp, lp);
}

void print(cstr format, ...)
{
    c8 buffer[1024];

    va_list args;
    va_start(args, format);
    u32 len = vsnprintf(buffer, sizeof(buffer) - 2, format, args);
    buffer[len++] = '\n';
    buffer[len++] = '\0';

    WriteConsoleA(g_stdout, buffer, len, 0, 0);
    OutputDebugStringA(buffer);
}

void die(cstr format, ...)
{
    c8 buffer[1024];

    u32 len = snprintf(buffer, sizeof(buffer), "error: ");

    va_list args;
    va_start(args, format);
    len = vsnprintf(buffer + len, sizeof(buffer) - len - 2, format, args);
    buffer[len++] = '\n';
    buffer[len++] = '\0';

    WriteConsoleA(g_stdout, buffer, len, 0, 0);
    OutputDebugStringA(buffer);

    ExitProcess(1);
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
