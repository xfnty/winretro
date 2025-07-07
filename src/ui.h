#pragma once

#include "common.h"

typedef enum ui_event_type_t ui_event_type_t;
enum ui_event_type_t {
    UI_EXIT,
    UI_OPEN_CORE,
    UI_OPEN_ROM,
    UI_LOAD_STATE,
    UI_SAVE_STATE,
};

typedef struct ui_event_t ui_event_t;
struct ui_event_t {
    ui_event_type_t type;
    union {
        c8 path[256];
    } value;
};

void init_ui(void);
void free_ui(void);

void ui_display_core_state(state_t state);
void set_ui_status(cstr text);

void poll_ui_events(void);

u8 get_ui_event(ui_event_t *event);
ptr get_ui_device_context(void);
