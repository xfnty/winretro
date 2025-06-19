#pragma once

#include <stdbool.h>

bool Core_Load(const char *path);
void Core_Free(void);

const char *Core_GetName(void);
