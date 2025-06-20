#include "common.h"

#include "miniwindows.h"

static HANDLE stdout = INVALID_HANDLE_VALUE;

static void CheckOrInitStdout(void)
{
    if (stdout != INVALID_HANDLE_VALUE)
    {
        return;
    }
    else if(!AttachConsole(ATTACH_PARENT_PROCESS))
    {
        #if BUILD_DEBUG == BUILD_DEBUG
            if (!AllocConsole()) return;
        #else
            return;
        #endif
    }

    stdout = GetStdHandle(STD_OUTPUT_HANDLE);
    assert(stdout != INVALID_HANDLE_VALUE);

    DWORD mode;
    assert(GetConsoleMode(stdout, &mode));
    assert(SetConsoleMode(stdout, mode | ENABLE_VIRTUAL_TERMINAL_PROCESSING));
}

void info(cstr fmt, ...)
{
    CheckOrInitStdout();

    if (stdout == INVALID_HANDLE_VALUE)
    {
        return;
    }

    c8 buffer[1024];
    u32 i = format(buffer, sizeof(buffer), "\x1b[0m");
    va_list args;
    va_start(args, fmt);
    i += formatv(buffer + i, sizeof(buffer) - i, fmt, args);
    va_end(args);
    i += format(buffer + i, sizeof(buffer) - i, "\x1b[0m\n");
    assert(WriteConsoleA(stdout, buffer, i, 0, 0));
}

void error(cstr fmt, ...)
{
    CheckOrInitStdout();

    if (stdout == INVALID_HANDLE_VALUE)
    {
        return;
    }
    
    c8 buffer[1024];
    u32 i = format(buffer, sizeof(buffer), "\x1b[1m\x1b[1;31merror\x1b[0m: ");
    va_list args;
    va_start(args, fmt);
    i += formatv(buffer + i, sizeof(buffer) - i, fmt, args);
    va_end(args);
    i += format(buffer + i, sizeof(buffer) - i, "\x1b[0m\n");
    assert(WriteConsoleA(stdout, buffer, i, 0, 0));
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
