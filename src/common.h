#pragma once

/* defines */
#define true 1u
#define false 0
#define INVALID_IDX ((u32)-1)

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
#define MEM_COMMIT                       0x00001000
#define MEM_RESERVE                      0x00002000
#define MEM_DECOMMIT                     0x00004000
#define MEM_RELEASE                      0x00008000
#define PAGE_READWRITE                   0x04
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
#define WM_ERASEBKGND                    0x0014
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
#define WHITE_BRUSH                      0

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


/* macros */
#define KB(_b) ((_b) * 1024)
#define MB(_b) ((_b) * 1024 * 1024)
#define GB(_b) ((_b) * 1024 * 1024 * 1024)
#define min(_a, _b) ((_a) < (_b) ? (_a) : (_b))
#define max(_a, _b) ((_a) > (_b) ? (_a) : (_b))
#define countof(_a) (sizeof(_a)/sizeof((_a)[0]))
#define va_create(_last_arg) ((u8*)&(_last_arg) + 8)
#define va_arg(_list, _T) ( ( sizeof(_T) > 8) ? (**(_T**)((_list += 8) - 8)) : ( *(_T* )((_list += 8) - 8)) )

#if defined(_MSC_VER)
    #define assert(_x) do { if (!(_x)) { __debugbreak(); } } while (0)
#else
    #define assert(_x) do { if (!(_x)) { __asm__("int3"); __builtin_unreachable(); } } while (0)
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
typedef struct PAINTSTRUCT PAINTSTRUCT;
struct PAINTSTRUCT {
    ptr  hdc;
    u32  fErase;
    RECT rcPaint;
    u32  fRestore;
    u32  fIncUpdate;
    u8   rgbReserved[32];
};
typedef struct SYSTEM_INFO SYSTEM_INFO;
struct SYSTEM_INFO {
    union {
        u32 dwOemId;
        struct {
            u16 wProcessorArchitecture;
            u16 wReserved;
        } DUMMYSTRUCTNAME;
    } DUMMYUNIONNAME;
    u32 dwPageSize;
    ptr lpMinimumApplicationAddress;
    ptr lpMaximumApplicationAddress;
    u64 dwActiveProcessorMask;
    u32 dwNumberOfProcessors;
    u32 dwProcessorType;
    u32 dwAllocationGranularity;
    u16 wProcessorLevel;
    u16 wProcessorRevision;
};


/* imports */
/* kernel32.dll */
void GetSystemInfo(SYSTEM_INFO *info);
u32 WINAPI AttachConsole(u32 pid);
ptr WINAPI GetStdHandle(u32 id);
u32 IsDebuggerPresent(void);
void ExitProcess(u32 code);
u32 GetLastError(void);
u32 WriteConsoleA(ptr con, const ptr buf, u32 bytes, u32 *written, ptr _);
u32 OutputDebugStringA(cstr str);
ptr GetModuleHandleA(cstr name);
u32 FreeLibrary(ptr module);
void RtlFillMemory(ptr buffer, u64 size, i32 byte);
void RtlZeroMemory(ptr buffer, u64 size);
ptr CreateFileA(cstr path, u32 access, u32 share, ptr security, u32 create, u32 flags, ptr template);
u32 ReadFile(ptr file, ptr buffer, u32 bytes_to_read, u32 *bytes_read, ptr overlapped);
u32 WriteFile(ptr file, ptr buffer, u32 bytes_to_write, u32 *bytes_written, ptr overlapped);
u32 GetFileSize(ptr file, u32 *msdword);
u32 CloseHandle(ptr handle);
ptr VirtualAlloc(ptr addr, u64 size, u32 flags, u32 protect);
u32 VirtualFree(ptr addr, u64 size, u32 flags);
u32 GetModuleFileNameA(ptr module, c8 *path, u32 pathmax);
u32 GetFileAttributesA(cstr path);
u32 CreateDirectoryA(cstr path, ptr security);

/* user32.dll */
ptr CreateMenu(void);
ptr LoadCursorA(ptr hinst, cstr name);
u16 RegisterClassA(WNDCLASSA *cls);
i64 DefWindowProcA(ptr hwnd, u32 msg, u64 wp, i64 lp);
ptr CreateWindowExA(u32 exstyle, cstr cls, cstr title, u32 style, i32 x, i32 y, i32 w, i32 h, ptr hwnd_parent, ptr hmenu, ptr hinst, ptr lp);
i64 SendMessageA(ptr hwnd, u32 msg, u64 wp, i64 lp);
u32 ShowWindow(ptr hwnd, i32 show);
u32 PeekMessageA(MSG *msg, ptr hwnd, u32 fmin, u32 fmax, u32 flags);
u32 TranslateMessage(MSG *msg);
u32 DispatchMessageA(MSG *msg);
u32 GetClientRect(ptr hwnd, RECT *r);
u32 AppendMenuA(ptr hmenu, u32 flags, u64 item, cstr str);
u32 ModifyMenuA(ptr hmenu, u32 id, u32 flags, u64 item, cstr str);
u32 SetWindowPos(ptr hwnd, ptr insert_type, i32 x, i32 y, i32 w, i32 h, u32 flags);
ptr GetDC(ptr hwnd);
i32 ReleaseDC(ptr hwnd, ptr hdc);
u32 DestroyWindow(ptr hwnd);
u32 UnregisterClassA(cstr cls, ptr hinst);
ptr BeginPaint(ptr hwnd, PAINTSTRUCT *paint);
u32 EndPaint(ptr hwnd, const PAINTSTRUCT *paint);

/* gdi32.dll */
i32 ChoosePixelFormat(ptr hdc, PIXELFORMATDESCRIPTOR *pfd);
i32 DescribePixelFormat(ptr hdc, i32 pixel_format, u32 pfd_size, PIXELFORMATDESCRIPTOR *pfd);
u32 SetPixelFormat(ptr hdc, i32 pixel_format, PIXELFORMATDESCRIPTOR *pfd);
ptr GetStockObject(i32 id);
u32 SwapBuffers(ptr hdc);

/* opengl32.dll */
ptr WINAPI wglCreateContext(ptr hdc);
u32 WINAPI wglDeleteContext(ptr hglrc);
ptr WINAPI wglGetProcAddress(cstr symbol);
u32 WINAPI wglMakeCurrent(ptr hdc, ptr hglrc);
u32 WINAPI glGetError(void);
void WINAPI glClear(u32 mask);
cstr WINAPI glGetString(u32 id);
cstr WINAPI glClearColor(f32 r, f32 g, f32 b, f32 a);

/* comdlg32.dll */
u32 WINAPI GetOpenFileNameA(OPENFILENAMEA *ofn);
u32 WINAPI GetSaveFileNameA(OPENFILENAMEA *ofn);

/* shell32.dll */
ptr WINAPI ShellExecuteA(ptr hwnd, cstr op, cstr file, cstr params, cstr dir, i32 show);


/* function declarations */
u32  str_equals(cstr a, cstr b, u32 minsize);
void print(cstr format, ...);
u32  snprintf(c8 *buffer, u32 maxsize, cstr format, ...);
u32  vsnprintf(c8 *buffer, u32 maxsize, cstr format, va_list args);
ptr  mem_alloc(u32 size);
void mem_reset(ptr p);


/* variables */
typedef struct g_log_t g_log_t;
struct g_log_t {
    ptr stdout;
    u8 initialized;
    u8 enabled;
};
extern g_log_t g_log;

typedef struct g_mem_t g_mem_t;
struct g_mem_t {
    u8 *data;
    u64 reserved;
    u64 used;
};
extern g_mem_t g_mem;
