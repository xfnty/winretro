#pragma once

#include "miniwindows.h"

void Ui_Init(HWND parent, HINSTANCE instance);
void Ui_Free(void);

void Ui_Resize(int w, int h);
