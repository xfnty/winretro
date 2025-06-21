#include "ui.h"

void Ui_Create(HWND window, HINSTANCE instance)
{
    HWND toolbar = CreateWindowExA(
        0,
        TOOLBARCLASSNAME,
        0,
        WS_CHILD | TBSTYLE_WRAPABLE,
        0, 0, 0, 0,
        window,
        0,
        instance,
        0
    );
    if (!toolbar)
    {
        error("failed to create toolbar (%d)", GetLastError());
    }
    ShowWindow(toolbar, 1);
}
