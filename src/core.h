#pragma once

#include "common.h"

typedef enum CoreInputAxis CoreInputAxis;
enum CoreInputAxis {
    CORE_AXIS_B = 0,
    CORE_AXIS_Y = 1,
    CORE_AXIS_SELECT = 2,
    CORE_AXIS_START = 3,
    CORE_AXIS_UP = 4,
    CORE_AXIS_DOWN = 5,
    CORE_AXIS_LEFT = 6,
    CORE_AXIS_RIGHT = 7,
    CORE_AXIS_A = 8,
    CORE_AXIS_X = 9,
    CORE_AXIS_L = 10,
    CORE_AXIS_R = 11,
    CORE_AXIS_L2 = 12,
    CORE_AXIS_R2 = 13,
    CORE_AXIS_L3 = 14,
    CORE_AXIS_R3 = 15,
    _CORE_AXIS_COUNT
};

typedef enum CorePixelFormat CorePixelFormat;
enum CorePixelFormat {
    CORE_PIXEL_FORMAT_UNDEFINED = 0,
    CORE_PIXEL_FORMAT_0RGB1555,
    CORE_PIXEL_FORMAT_XRGB8888,
    CORE_PIXEL_FORMAT_RGB565,
};

typedef struct CoreOptions CoreOptions;
struct CoreOptions {
    struct {
        cstr save;
        cstr system;
    } dirs;
};

typedef struct CoreInputAxisState CoreInputAxisState;
struct CoreInputAxisState {
    CoreInputAxis axis;
    f32 value;
};

u8 Core_Load(cstr path);
u8 Core_LoadGame(cstr path);
void Core_Free(void);

cstr Core_GetName(void);
CorePixelFormat Core_GetPixelFormat(void);

u8 Core_SetInput(CoreInputAxisState input);
void Core_SetOptions(CoreOptions options);
