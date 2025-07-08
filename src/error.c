#include "error.h"

#include "log.h"

ptr check_result_crash(result_t result)
{
    if (!result.ok) __debugbreak();
    return result.value;
}

result_t create_error(cstr file, i32 line, cstr text, ...)
{
    result_t r = { .error = { .file = file, .line = line }, .ok = false };
    va_list args;
    va_start(args, text);
    vsnprintf(r.error.text, sizeof(r.error.text), text, args);
    return r;
}

cstr get_winapi_error(u32 code, ...)
{
    static c8 buffer[128];

    va_list args;
    va_start(args, code);
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

    SetLastError(code); /* FormatMessageA() also sets last error */
    return buffer;
}

void report_error(result_t result)
{
    print_error(result.error.file, result.error.line, result.error.text);
}
