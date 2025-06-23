#include "common.h"

#include "miniwindows.h"

typedef enum LoggingState LoggingState;
enum LoggingState {
    LOGGING_UNINITIALIZED = 0,
    LOGGING_DISABLED = 1,
    LOGGING_DEBUGGER = 2,
    LOGGING_STDOUT   = 3,
};

typedef struct Common Common;
struct Common {
    LoggingState logging_state;
    HANDLE stdout;
};

static Common *GetCommon(void)
{
    static Common c = { .stdout = INVALID_HANDLE_VALUE };
    return &c;
}

void InitDebugOutput(void)
{
    assert(GetCommon()->logging_state == LOGGING_UNINITIALIZED);

    if (AttachConsole(ATTACH_PARENT_PROCESS))
    {
        GetCommon()->logging_state = LOGGING_STDOUT;
        GetCommon()->stdout = GetStdHandle(STD_OUTPUT_HANDLE);
        assert(GetCommon()->stdout != INVALID_HANDLE_VALUE);
        DWORD mode;
        assert(GetConsoleMode(GetCommon()->stdout, &mode));
        assert(SetConsoleMode(GetCommon()->stdout, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING));
    }
    else if (IsDebuggerPresent())
    {
        GetCommon()->logging_state = LOGGING_DEBUGGER;
    }
    else
    {
        GetCommon()->logging_state = LOGGING_DISABLED;
    }
}

// FIXME: code duplication
void info(cstr fmt, ...)
{
    if (GetCommon()->logging_state <= LOGGING_DISABLED) return;

    c8 buffer[1024];
    u32 i = 0;
    va_list args;
    va_start(args, fmt);

    if (GetCommon()->logging_state == LOGGING_STDOUT)
    {
        i +=  format(buffer + i, sizeof(buffer) - 1 - i, "\x1b[0m");
        i += formatv(buffer + i, sizeof(buffer) - 1 - i, fmt, args);
        i +=  format(buffer + i, sizeof(buffer) - 1 - i, "\x1b[0m\n");
        assert(WriteConsoleA(GetCommon()->stdout, buffer, i, 0, 0));
    }
    else
    {
        i += formatv(buffer + i, sizeof(buffer) - 2 - i, fmt, args);
        buffer[i++] = '\n';
        buffer[i] = '\0';
        OutputDebugStringA(buffer);
    }

    va_end(args);
}

void error(cstr fmt, ...)
{
    if (GetCommon()->logging_state <= LOGGING_DISABLED) return;

    c8 buffer[1024];
    u32 i = 0;
    va_list args;
    va_start(args, fmt);

    if (GetCommon()->logging_state == LOGGING_STDOUT)
    {
        i +=  format(buffer + i, sizeof(buffer) - 1 - i, "\x1b[1m\x1b[1;31merror\x1b[0m: ");
        i += formatv(buffer + i, sizeof(buffer) - 1 - i, fmt, args);
        i +=  format(buffer + i, sizeof(buffer) - 1 - i, "\x1b[0m\n");
        assert(WriteConsoleA(GetCommon()->stdout, buffer, i, 0, 0));
    }
    else
    {
        i +=  format(buffer + i, sizeof(buffer) - 2 - i, "error: ");
        i += formatv(buffer + i, sizeof(buffer) - 2 - i, fmt, args);
        buffer[i++] = '\n';
        buffer[i] = '\0';
        OutputDebugStringA(buffer);
    }

    va_end(args);
}

void die(i32 code)
{
    __debugbreak();
    ExitProcess(code);
}

u32 format(c8 *buffer, u32 maxsize, cstr fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    u32 r = formatv(buffer, maxsize, fmt, args);
    va_end(args);
    return r;
}

static u32 format_str(c8 *buffer, u32 maxsize, cstr v)
{
    assert(buffer);
    u32 bi = 0;
    while (bi < maxsize && *v) buffer[bi++] = *(v++);
    return bi;
}

static u32 format_int(c8 *buffer, u32 maxsize, u32 base, i32 v)
{
    assert(buffer);

    const c8 table[] = {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
    };

    u32 bi = 0;

    if (bi < maxsize && v < 0)
    {
        buffer[bi++] = '-';
        v *= -1;
    }

    i32 d = 1;
    while ((u32)(v / d) >= base)
    {
        d *= base;
    }
    while (bi < maxsize && d != 0)
    {
        u32 table_idx = (u32)(v / d);
        assert(table_idx < countof(table));
        buffer[bi++] = table[table_idx];
        v %= d;
        d /= base;
    }

    return bi;
}

u32 formatv(c8 *buffer, u32 maxsize, cstr fmt, va_list args)
{
    assert(buffer);
    assert(fmt);

    u32 bi = 0;
    u32 fi = 0;
    while (bi < maxsize && fmt[fi])
    {
        c8 c = fmt[fi];

        if (c != '%')
        {
            buffer[bi++] = fmt[fi++];
        }
        else
        {
            fi++;

            if (bi < maxsize && fmt[fi])
            {
                c = fmt[fi++];

                switch (c)
                {
                case '%':
                    buffer[bi++] = '%';
                    break;

                case 's':
                    bi += format_str(buffer + bi, maxsize - bi, va_arg(args, cstr));
                    break;

                case 'b':
                    bi += format_int(buffer + bi, maxsize - bi, 2, va_arg(args, i32));
                    break;

                case 'd':
                    bi += format_int(buffer + bi, maxsize - bi, 10, va_arg(args, i32));
                    break;

                case 'x':
                    bi += format_int(buffer + bi, maxsize - bi, 16, va_arg(args, i32));
                    break;
                }
            }
        }
    }

    return bi;
}
