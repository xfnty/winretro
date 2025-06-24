#pragma once

#include "common.h"

typedef struct Core Core;
struct Core {
    cstr name;
};

Core *CreateCore(cstr path);
void FreeCore(Core **core);
