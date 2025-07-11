#pragma once

#include "common.h"

/* defines */
#define GENERIC_READ                       0x80000000
#define GENERIC_WRITE                      0x40000000
#define FILE_SHARE_READ                    0x00000001
#define CREATE_ALWAYS                      2
#define OPEN_EXISTING                      3
#define OPEN_ALWAYS                        4
#define FILE_ATTRIBUTE_NORMAL              0x00000080
#define FILE_ATTRIBUTE_DIRECTORY           0x00000010
#define INVALID_FILE_SIZE                  0xFFFFFFFFu
#define HEAP_GENERATE_EXCEPTIONS           0x00000004
#define HEAP_ZERO_MEMORY                   0x00000008
#define ERROR_FILE_NOT_FOUND               2
#define ERROR_ALREADY_EXISTS               183
#define MEM_COMMIT                         0x00001000
#define MEM_RESERVE                        0x00002000
#define MEM_DECOMMIT                       0x00004000
#define MEM_RELEASE                        0x00008000
#define PAGE_READWRITE                     0x04
#define INVALID_HANDLE_VALUE               (ptr)-1
#define ATTACH_PARENT_PROCESS              (u32)-1
#define STD_OUTPUT_HANDLE                  (u32)-11
#define IDC_ARROW                          ((cstr)32512)
#define WS_OVERLAPPEDWINDOW                0x00CF0000L
#define WS_CHILD                           0x40000000L
#define WS_VISIBLE                         0x10000000L
#define CW_USEDEFAULT                      0x80000000
#define SW_HIDE                            0
#define SW_SHOWNORMAL                      1
#define SW_SHOW                            5
#define WM_CLOSE                           0x0010
#define WM_SIZE                            0x0005
#define WM_COMMAND                         0x0111
#define WM_ERASEBKGND                      0x0014
#define WM_USER                            0x0400
#define WM_KEYDOWN                         0x0100
#define WM_KEYUP                           0x0101
#define PM_REMOVE                          0x0001
#define CS_OWNDC                           0x0020
#define PFD_DOUBLEBUFFER                   0x00000001
#define PFD_DRAW_TO_WINDOW                 0x00000004
#define PFD_SUPPORT_OPENGL                 0x00000020
#define PFD_TYPE_RGBA                      0
#define PFD_MAIN_PLANE                     0
#define OFN_PATHMUSTEXIST                  0x00000800
#define OFN_FILEMUSTEXIST                  0x00001000
#define OFN_OVERWRITEPROMPT                0x00000002
#define WHITE_BRUSH                        0
#define BLACK_BRUSH                        4
#define FORMAT_MESSAGE_FROM_SYSTEM         0x00001000
#define FORMAT_MESSAGE_IGNORE_INSERTS      0x00000200
#define LANG_ENGLISH                       0x09
#define SUBLANG_ENGLISH_US                 0x01
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004u
#define STATUSCLASSNAME                    "msctls_statusbar32"

#define MF_POPUP                           0x00000010L
#define MF_ENABLED                         0x00000000L
#define MF_DISABLED                        0x00000002L
#define MF_BYCOMMAND                       0x00000000L
#define MF_SEPARATOR                       0x00000800L

#define ICC_BAR_CLASSES                    0x00000004
#define ICC_PROGRESS_CLASS                 0x00000020
#define ICC_STANDARD_CLASSES               0x00000020
#define SBARS_SIZEGRIP                     0x0100
#define SBARS_TOOLTIPS                     0x0800
#define SB_SETTEXTA                        (WM_USER+1)
#define SB_SETPARTS                        (WM_USER+4)
#define SBT_NOBORDERS                      0x0100
#define SBT_POPOUT                         0x0200

#if defined(_MSC_VER)
    #define WINAPI __stdcall
#else
    #define WINAPI __attribute__((stdcall))
#endif

#define MAKELANGID(p, s) ((((u16)(s)) << 10) | (u16)(p))


/* typedefs */
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
typedef struct SYSTEMTIME SYSTEMTIME;
struct SYSTEMTIME {
    u16 wYear;
    u16 wMonth;
    u16 wDayOfWeek;
    u16 wDay;
    u16 wHour;
    u16 wMinute;
    u16 wSecond;
    u16 wMilliseconds;
};
typedef struct INITCOMMONCONTROLSEX INITCOMMONCONTROLSEX;
struct INITCOMMONCONTROLSEX {
  u32 dwSize;
  u32 dwICC;
};
typedef struct SIZE SIZE;
struct SIZE {
    i32 cx;
    i32 cy;
};


/* kernel32.dll */
u32 FormatMessageA(u32 flags, ptr source, u32 msg, u32 lang, c8 *buf, u32 size, va_list *args);
void GetSystemInfo(SYSTEM_INFO *info);
u32 WINAPI AttachConsole(u32 pid);
ptr WINAPI GetStdHandle(u32 id);
u32 WINAPI GetConsoleMode(ptr console, u32 *mode);
u32 WINAPI SetConsoleMode(ptr console, u32 mode);
u32 IsDebuggerPresent(void);
void ExitProcess(u32 code);
u32 GetLastError(void);
void SetLastError(u32 code);
u32 WriteConsoleA(ptr con, const ptr buf, u32 bytes, u32 *written, ptr _);
u32 OutputDebugStringA(cstr str);
ptr GetModuleHandleA(cstr name);
ptr LoadLibraryA(cstr name);
ptr GetProcAddress(ptr module, cstr symbol);
u32 FreeLibrary(ptr module);
void RtlCopyMemory(ptr dst, const ptr src, u64 size);
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
ptr GetProcessHeap(void);
ptr HeapAlloc(ptr heap, u32 flags, u64 size);
ptr HeapReAlloc(ptr heap, u32 flags, ptr memory, u64 size);
u32 HeapFree(ptr heap, u32 flags, ptr memory);
u32 GetCurrentDirectory(u32 nBufferLength, cstr lpBuffer);
u32 SetCurrentDirectory(cstr path);
void GetSystemTime(SYSTEMTIME *time);
u32 FlushFileBuffers(ptr file);
u32 QueryPerformanceFrequency(i64 *freq);
u32 QueryPerformanceCounter(i64 *counter);
u16 WINAPI CaptureStackBackTrace(u32 skip, u32 capture, ptr *bt, u32 *hash);

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
u32 GetTextExtentPoint32A(ptr hdc, cstr text, i32 len, SIZE *size);

/* comdlg32.dll */
u32 WINAPI GetOpenFileNameA(OPENFILENAMEA *ofn);
u32 WINAPI GetSaveFileNameA(OPENFILENAMEA *ofn);

/* shell32.dll */
ptr WINAPI ShellExecuteA(ptr hwnd, cstr op, cstr file, cstr params, cstr dir, i32 show);

/* comctl32.dll */
u32 InitCommonControlsEx(const INITCOMMONCONTROLSEX *inf);
