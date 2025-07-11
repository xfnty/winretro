#pragma once

#include "common.h"

typedef enum core_input_t core_input_t;
enum core_input_t {
    CORE_INPUT_B      = 0,
    CORE_INPUT_Y      = 1,
    CORE_INPUT_SELECT = 2,
    CORE_INPUT_START  = 3,
    CORE_INPUT_UP     = 4,
    CORE_INPUT_DOWN   = 5,
    CORE_INPUT_LEFT   = 6,
    CORE_INPUT_RIGHT  = 7,
    CORE_INPUT_A      = 8,
    CORE_INPUT_X      = 9,
    CORE_INPUT_L      = 10,
    CORE_INPUT_R      = 11,
    CORE_INPUT_L2     = 12,
    CORE_INPUT_R2     = 13,
    CORE_INPUT_L3     = 14,
    CORE_INPUT_R3     = 15,
    _CORE_INPUT_COUNT
};

void init_core(cstr dll);
void free_core(void);

void load_game(cstr rom);
void unload_game(void);

void run_frame(void);

void load_game_state(cstr path);
void save_game_state(cstr path);

state_t get_core_state(void);
f32     get_target_fps(void);
cstr    get_core_system_directory(void);
cstr    get_core_save_dirrectory(void);

void set_core_input(core_input_t input, i16 value);
