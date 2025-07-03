#include "windows.h"

cstr get_last_error_text(u32 code, ...)
{
    static c8 buffer[1024];

    va_list args = va_create(code);
    u32 l = FormatMessageA(
        FORMAT_MESSAGE_FROM_SYSTEM,
        0,
        code,
        MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
        buffer,
        countof(buffer) - 1,
        &args
    );
    if (l > 2) buffer[l - 3] = '\0'; /* FormatMessageA() adds ".\n" at the end. */

    return buffer;
}
