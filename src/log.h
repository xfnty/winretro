#pragma once

#include "common.h"

void init_log(void);
void free_log(void);

void print(cstr format, ...);
void error(cstr format, ...);
