#pragma once

#include "common.h"

void init_gl(ptr dc);
void configure_gl(u32 max_width, u32 max_height);
void free_gl(void);

u64 get_gl_framebuffer(void);
ptr get_gl_proc_address(cstr name);

void set_gl_window_size(u32 w, u32 h);
void set_gl_render_resolution(u32 w, u32 h);

void present_frame(void);
