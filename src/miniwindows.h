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
#define WS_OVERLAPPEDWINDOW                0x00CF0000L
#define OFN_PATHMUSTEXIST                  0x00000800
#define OFN_FILEMUSTEXIST                  0x00001000

#define WINAPI   __stdcall
#define CALLBACK __stdcall
#define APIENTRY __stdcall

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
typedef u32 UINT;
typedef char *LPSTR;
typedef const char *LPCSTR;
typedef LRESULT LPARAM;
typedef u64 WPARAM;
typedef u64 UINT_PTR;
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
BOOL GetMessageA(MSG *msg, HWND window, UINT fmin, UINT fmax);
BOOL TranslateMessage(const MSG *msg);
BOOL DispatchMessageA(const MSG *msg);
HCURSOR LoadCursorA(HINSTANCE instance, cstr name);
LRESULT DefWindowProcA(HWND window, UINT msg, WPARAM wp, LPARAM lp);
void PostQuitMessage(int code);

/* gdi32.dll */
HGDIOBJ GetStockObject(int id);

/* comdlg32.dll */
BOOL APIENTRY GetOpenFileNameA(OPENFILENAMEA *ofn);
DWORD CommDlgExtendedError(void);
