#pragma once

#include "core.h"
#include "common.h"

typedef struct UiParams UiParams;
struct UiParams {
    cstr title;
};

typedef struct Ui Ui;
struct Ui {
    UiParams params;
};

typedef enum UiEventType UiEventType;
enum UiEventType {
    UI_EXIT,
    UI_OPEN_CORE,
    UI_OPEN_ROM,
    UI_LOAD_STATE,
    UI_SAVE_STATE,
    UI_INPUT,
};

typedef struct UiEvent UiEvent;
struct UiEvent {
    UiEventType type;
    union {
        char *path;
        CoreInputAxisState input;
    } value;
};

Ui *CreateUi(UiParams params);
void FreeUi(Ui **ui);

void Ui_ProcessEvents(Ui *ui);
u8 Ui_GetEvent(Ui *ui, UiEvent *event);

void Ui_SetCoreLoaded(Ui *ui, cstr name);
void Ui_SetRomLoaded(Ui *ui, cstr name);
