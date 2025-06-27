#pragma once

#include "core.h"
#include "common.h"

typedef struct UiParams UiParams;
struct UiParams {
    cstr title;
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

void InitUi(UiParams params);
void FreeUi(void);

void Ui_ProcessEvents(void);
u8 Ui_GetEvent(UiEvent *event);

void Ui_SetCoreLoaded(cstr name);
void Ui_SetRomLoaded(cstr name);
