#include "input.h"

u8 map_ui_key_to_core_input(ui_key_t key, core_input_t *input)
{
    switch (key & UI_KEY_MASK)
    {
        /* AC1 layout */
        case UI_KEY_BACKSPACE: *input = CORE_INPUT_SELECT; return true;
        case UI_KEY_RETURN:    *input = CORE_INPUT_START;  return true;
        case UI_KEY_W:         *input = CORE_INPUT_UP;     return true;
        case UI_KEY_S:         *input = CORE_INPUT_DOWN;   return true;
        case UI_KEY_I:         *input = CORE_INPUT_L2;     return true;
        case UI_KEY_K:         *input = CORE_INPUT_R2;     return true;
        case UI_KEY_J:         *input = CORE_INPUT_LEFT;   return true;
        case UI_KEY_L:         *input = CORE_INPUT_RIGHT;  return true;
        case UI_KEY_A:         *input = CORE_INPUT_L;      return true;
        case UI_KEY_D:         *input = CORE_INPUT_R;      return true;
        case UI_KEY_N:         *input = CORE_INPUT_A;      return true;
        case UI_KEY_M:         *input = CORE_INPUT_Y;      return true;
        case UI_KEY_TAB:       *input = CORE_INPUT_X;      return true;
        case UI_KEY_SPACE:     *input = CORE_INPUT_B;      return true;
    }

    return false;
}
