#pragma once

#include "common.h"

typedef struct gl_params_t gl_params_t;
struct gl_params_t {
    u32 width;
    u32 height;
    u8 version_major;
    u8 version_minor;
    u8 debug;
};

typedef struct g_gl_t g_gl_t;
struct g_gl_t {
    gl_params_t params;
    ptr window;
    ptr device_context;
    ptr opengl_context;
};
extern g_gl_t g_gl;

u8   init_gl(ptr hwnd, gl_params_t params);
void free_gl(void);
