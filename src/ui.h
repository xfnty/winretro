#pragma once

#include "common.h"

typedef enum ui_key_t ui_key_t;
enum ui_key_t {
    UI_KEY_NONE = 0,
    UI_KEY_BACKSPACE,
    UI_KEY_TAB,
    UI_KEY_RETURN,
    UI_KEY_LSHIFT,
    UI_KEY_LCONTROL,
    UI_KEY_LALT,
    UI_KEY_RSHIFT,
    UI_KEY_RCONTROL,
    UI_KEY_RALT,
    UI_KEY_ESCAPE,
    UI_KEY_SPACE,
    UI_KEY_LEFT,
    UI_KEY_RIGHT,
    UI_KEY_UP,
    UI_KEY_DOWN,
    UI_KEY_0,
    UI_KEY_1,
    UI_KEY_2,
    UI_KEY_3,
    UI_KEY_4,
    UI_KEY_5,
    UI_KEY_6,
    UI_KEY_7,
    UI_KEY_8,
    UI_KEY_9,
    UI_KEY_A,
    UI_KEY_B,
    UI_KEY_C,
    UI_KEY_D,
    UI_KEY_E,
    UI_KEY_F,
    UI_KEY_G,
    UI_KEY_H,
    UI_KEY_I,
    UI_KEY_J,
    UI_KEY_K,
    UI_KEY_L,
    UI_KEY_M,
    UI_KEY_N,
    UI_KEY_O,
    UI_KEY_P,
    UI_KEY_Q,
    UI_KEY_R,
    UI_KEY_S,
    UI_KEY_T,
    UI_KEY_U,
    UI_KEY_V,
    UI_KEY_W,
    UI_KEY_X,
    UI_KEY_Y,
    UI_KEY_Z,
    UI_KEY_ADD,
    UI_KEY_SUB,
    UI_KEY_F1,
    UI_KEY_F2,
    UI_KEY_F3,
    UI_KEY_F4,
    UI_KEY_F5,
    UI_KEY_F6,
    UI_KEY_F7,
    UI_KEY_F8,
    UI_KEY_F9,
    UI_KEY_F10,
    UI_KEY_F11,
    UI_KEY_F12,
    UI_KEY_MASK = 0xff,
    UI_KEY_STATE_MASK = 0x100,
};

typedef enum ui_event_type_t ui_event_type_t;
enum ui_event_type_t {
    UI_EXIT,
    UI_RESIZE,
    UI_OPEN_CORE,
    UI_OPEN_ROM,
    UI_LOAD_STATE,
    UI_SAVE_STATE,
    UI_INPUT,
};

typedef struct ui_event_t ui_event_t;
struct ui_event_t {
    ui_event_type_t type;
    union {
        c8 path[256];
        struct {
            u32 x;
            u32 y;
        } size;
        struct {
            ui_key_t key;
        } input;
    } value;
};

void init_ui(void);
void free_ui(void);

void ui_display_core_state(state_t state);
void set_ui_status(cstr text);

void poll_ui_events(void);

u8 get_ui_event(ui_event_t *event);
ptr get_ui_device_context(void);
