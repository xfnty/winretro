#pragma once

#include "common.h"

#define IDC_ARROW                          ((cstr)32512)
#define WHITE_BRUSH                        0
#define CS_OWNDC                           0x0020
#define CW_USEDEFAULT                      0x80000000
#define SW_HIDE                            0
#define SW_SHOW                            5
#define INVALID_HANDLE_VALUE               (ptr)-1
#define ATTACH_PARENT_PROCESS              (u32)-1
#define STD_OUTPUT_HANDLE                  (u32)-11
#define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004u
#define WM_CLOSE                           0x0010u
#define WM_DESTROY                         0x0002u
#define WM_SIZE                            0x0005u
#define WM_CREATE                          0x0001u
#define WM_USER                            0x0400u
#define WS_OVERLAPPEDWINDOW                0x00CF0000L
#define WS_CHILD                           0x40000000L
#define WS_VISIBLE                         0x10000000L
#define TBSTYLE_WRAPABLE                   0x0200
#define OFN_PATHMUSTEXIST                  0x00000800
#define OFN_FILEMUSTEXIST                  0x00001000
#define TOOLBARCLASSNAME                   "ToolbarWindow32"
#define STATUSCLASSNAME                    "msctls_statusbar32"
#define GWLP_USERDATA                      (-21)
#define GWL_STYLE                          (-16)
#define HEAP_GENERATE_EXCEPTIONS           0x00000004
#define HEAP_ZERO_MEMORY                   0x00000008
#define ICC_BAR_CLASSES                    0x00000004
#define ICC_PROGRESS_CLASS                 0x00000020
#define ICC_STANDARD_CLASSES               0x00000020
#define SBARS_SIZEGRIP                     0x0100
#define SBARS_TOOLTIPS                     0x0800
#define SB_SETTEXTA                        (WM_USER+1)
#define SB_SETPARTS                        (WM_USER+4)
#define SBT_NOBORDERS                      0x0100
#define SBT_POPOUT                         0x0200

#define LOWORD(_x) ((SHORT)(_x))
#define HIWORD(_x) ((SHORT)((DWORD)(_x) >> sizeof(SHORT)))

#define WINAPI   __stdcall
#define CALLBACK __stdcall
#define APIENTRY __stdcall

typedef ptr HDC;
typedef ptr HWND;
typedef ptr HICON;
typedef ptr HMENU;
typedef ptr HANDLE;
typedef ptr HBRUSH;
typedef ptr HCURSOR;
typedef ptr HMODULE;
typedef ptr HGDIOBJ;
typedef ptr HINSTANCE;
typedef i32 BOOL;
typedef i64 LRESULT;
typedef i64 LONG_PTR;
typedef u32 UINT;
typedef char *LPSTR;
typedef const char *LPCSTR;
typedef LRESULT LPARAM;
typedef u64 WPARAM;
typedef u64 UINT_PTR;
typedef u16 SHORT;
typedef u32 DWORD;
typedef u64 DWORD64;
typedef u16 WORD;
typedef WORD ATOM;

typedef LRESULT (CALLBACK *WNDPROC)(HWND window, UINT msg, WPARAM wp, LPARAM lp);

typedef struct WNDCLASSA WNDCLASSA;
struct WNDCLASSA {
    UINT      style;
    WNDPROC   lpfnWndProc;
    int       cbClsExtra;
    int       cbWndExtra;
    HINSTANCE hInstance;
    HICON     hIcon;
    HCURSOR   hCursor;
    HBRUSH    hbrBackground;
    LPCSTR    lpszMenuName;
    LPCSTR    lpszClassName;
};

typedef struct POINT POINT;
struct POINT {
    i32 x, y;
};

typedef struct MSG MSG;
struct MSG {
    HWND hwnd;
    u32    message;
    u64    wParam;
    i32    lParam;
    u32    time;
    POINT  pt;
    u32    lPrivate;
};

typedef UINT_PTR (*LPOFNHOOKPROC)(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp);

typedef struct EDITMENU EDITMENU;
struct EDITMENU {
   HMENU   hmenu;
   WORD    idEdit;
   WORD    idCut;
   WORD    idCopy;
   WORD    idPaste;
   WORD    idClear;
   WORD    idUndo;
};

typedef struct OPENFILENAMEA OPENFILENAMEA;
struct OPENFILENAMEA {
    DWORD         lStructSize;
    HWND          hwndOwner;
    HINSTANCE     hInstance;
    LPCSTR        lpstrFilter;
    LPSTR         lpstrCustomFilter;
    DWORD         nMaxCustFilter;
    DWORD         nFilterIndex;
    LPSTR         lpstrFile;
    DWORD         nMaxFile;
    LPSTR         lpstrFileTitle;
    DWORD         nMaxFileTitle;
    LPCSTR        lpstrInitialDir;
    LPCSTR        lpstrTitle;
    DWORD         Flags;
    WORD          nFileOffset;
    WORD          nFileExtension;
    LPCSTR        lpstrDefExt;
    LPARAM        lCustData;
    LPOFNHOOKPROC lpfnHook;
    LPCSTR        lpTemplateName;
    void         *pvReserved;
    DWORD         dwReserved;
    DWORD         FlagsEx;
};

typedef struct INITCOMMONCONTROLSEX INITCOMMONCONTROLSEX;
struct INITCOMMONCONTROLSEX {
  DWORD dwSize;
  DWORD dwICC;
};

typedef struct RECT RECT;
struct RECT {
    long left;
    long top;
    long right;
    long bottom;
};

typedef struct SIZE SIZE;
struct SIZE {
    long cx;
    long cy;
};

/* kernel32.dll */
void ExitProcess(UINT code);
HMODULE GetModuleHandleA(LPCSTR name);
HMODULE LoadLibraryA(LPCSTR name);
BOOL FreeLibrary(HMODULE module);
ptr GetProcAddress(HMODULE module, LPCSTR symbol);
BOOL WINAPI AttachConsole(DWORD pid);
BOOL WINAPI AllocConsole(void);
HANDLE WINAPI GetStdHandle(DWORD id);
BOOL WINAPI GetConsoleMode(HANDLE console, DWORD *mode);
BOOL WINAPI SetConsoleMode(HANDLE console, DWORD mode);
BOOL WriteConsoleA(HANDLE console, const ptr buffer, DWORD bytes, DWORD *written, ptr _reserved);
DWORD GetLastError(void);
void OutputDebugStringA(LPCSTR str);
BOOL IsDebuggerPresent(void);
void RtlZeroMemory(void *buffer, DWORD64 size);
ptr WINAPI HeapAlloc(HANDLE heap, DWORD flags, DWORD64 size);
BOOL HeapFree(HANDLE heap, DWORD flags, ptr data);
HANDLE GetProcessHeap(void);

/* user32.dll */
ATOM RegisterClassA(WNDCLASSA *cls);
HWND CreateWindowExA(
    DWORD exstyle,
    LPCSTR cls,
    LPCSTR title,
    DWORD style,
    int x,
    int y,
    int w,
    int h,
    HWND parent,
    HMENU menu,
    HINSTANCE instance,
    ptr lp
);
BOOL ShowWindow(HWND window, int show);
LRESULT SendMessageA(HWND wnd, UINT msg, WPARAM wp, LPARAM lp);
BOOL GetMessageA(MSG *msg, HWND window, UINT fmin, UINT fmax);
BOOL TranslateMessage(const MSG *msg);
BOOL DispatchMessageA(const MSG *msg);
HCURSOR LoadCursorA(HINSTANCE instance, cstr name);
LRESULT DefWindowProcA(HWND window, UINT msg, WPARAM wp, LPARAM lp);
void PostQuitMessage(int code);
LONG_PTR GetWindowLongPtrA(HWND hwnd, int idx);
LONG_PTR SetWindowLongPtrA(HWND hwnd, int idx, LONG_PTR ptr);
BOOL GetClientRect(HWND wnd, RECT *r);
HDC GetDC(HWND wnd);

/* gdi32.dll */
HGDIOBJ GetStockObject(int id);
BOOL GetTextExtentPoint32A(HDC dc, LPCSTR str, int length, SIZE *size);

/* comdlg32.dll */
BOOL APIENTRY GetOpenFileNameA(OPENFILENAMEA *ofn);
DWORD CommDlgExtendedError(void);

/* comctl32.dll */
BOOL InitCommonControlsEx(const INITCOMMONCONTROLSEX *inf);
