#pragma once

#include "common.h"

typedef enum CoreState CoreState;
enum CoreState {
    CORE_UNINITIALIZED,
    CORE_LOADED,
    CORE_PLAYING,
};

u8   Core_Load(cstr path);
void Core_Free(void);

cstr Core_GetName(void);
CoreState Core_GetState(void);
