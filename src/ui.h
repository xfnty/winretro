#pragma once

#include "common.h"

typedef struct g_ui_t g_ui_t;
struct g_ui_t {
    ptr instance;
    ptr window;
    ptr menu;
    ptr render_window;
    u8 was_exit_requested;
};
extern g_ui_t g_ui;

void init_ui(void);
void process_ui_events(void);
void free_ui(void);
