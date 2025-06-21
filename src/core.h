#pragma once

#include "common.h"

typedef enum CoreState CoreState;
enum CoreState {
    CORE_UNINITIALIZED,
    CORE_LOADED,
    CORE_PLAYING,
};

typedef struct Core Core;
struct Core {
    CoreState state;
    cstr name;
};

Core *Core_Load(cstr path);
void Core_Free(Core **core);
