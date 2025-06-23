#include "ui.h"

#include "common.h"

typedef struct Ui Ui;
struct Ui {
    HWND window;
    HINSTANCE instance;
    HWND statusbar;
};

Ui *GetUi(void)
{
    static Ui ui;
    return &ui;
}

void Ui_Init(HWND window, HINSTANCE instance)
{
    Ui_Free();

    Ui *ui = GetUi();
    ui->window = window;
    ui->instance = instance;

    INITCOMMONCONTROLSEX inf = {
        .dwSize = sizeof(inf),
        .dwICC = ICC_STANDARD_CLASSES | ICC_BAR_CLASSES | ICC_PROGRESS_CLASS,
    };
    if (!InitCommonControlsEx(&inf))
    {
        error("InitCommonControlsEx() failed (%d)", GetLastError());
        die(GetLastError());
    }

    ui->statusbar = CreateWindowExA(
        0,
        STATUSCLASSNAME,
        0,
        WS_CHILD | WS_VISIBLE,
        0, 0, 0, 0,
        window,
        0,
        instance,
        0
    );
    if (!ui->statusbar)
    {
        error("failed to create status bar (%d)", GetLastError());
        return;
    }

    const char *strings[] = {
        "Swanstation",
        "Armored Core (USA)",
        "OpenGL 3.3 Core",
        "60 FPS",
        "35%",
    };
    
    int parts[countof(strings)];
    HDC dc = GetDC(window);
    assert(dc);
    for (int i = 0; i < countof(strings); i++)
    {
        int l = 0; for (; strings[i][l]; l++);
        SIZE s;
        assert(GetTextExtentPoint32A(dc, strings[i], l, &s));
        parts[i] = ((i > 0) ? (parts[i - 1]) : (0)) + s.cx;
    }
    SendMessageA(ui->statusbar, SB_SETPARTS, countof(parts), (LPARAM)parts);

    for (int i = 0; i < countof(strings); i++)
    {
        SendMessageA(ui->statusbar, SB_SETTEXTA, i | SBT_NOBORDERS, (LPARAM)strings[i]);
    }

    ShowWindow(ui->statusbar, 1);

    LONG_PTR currentStyle = GetWindowLongPtrA(ui->statusbar, GWL_STYLE);
    LONG_PTR newStyle = currentStyle & (~SBARS_SIZEGRIP);
    SetWindowLongPtrA(ui->statusbar, GWL_STYLE, newStyle);
}

void Ui_Free(void)
{
    RtlZeroMemory(GetUi(), sizeof(Ui));
}

void Ui_Resize(int w, int h)
{
    (void)w;
    (void)h;

    Ui *ui = GetUi();

    const char *strings[] = {
        "Swanstation",
        "Armored Core (USA)",
        "OpenGL 3.3 Core",
        "60 FPS",
        "35%",
    };
    int parts[countof(strings)];
    HDC dc = GetDC(ui->window);
    assert(dc);
    for (int i = 0; i < countof(strings); i++)
    {
        int l = 0; for (; strings[i][l]; l++);
        SIZE s;
        assert(GetTextExtentPoint32A(dc, strings[i], l, &s));
        parts[i] = ((i > 0) ? (parts[i - 1]) : (0)) + s.cx;
    }
    SendMessageA(ui->statusbar, SB_SETPARTS, countof(parts), (LPARAM)parts);

    SendMessageA(GetUi()->statusbar, WM_SIZE, 0, 0);
}
