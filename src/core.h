#pragma once

#include "common.h"

void init_core(cstr dll);
void free_core(void);

void load_game(cstr rom);
void unload_game(void);

void run_frame(void);

void load_game_state(cstr path);
void save_game_state(cstr path);

state_t get_core_state(void);

cstr get_core_system_directory(void);
cstr get_core_save_dirrectory(void);
