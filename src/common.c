#include "common.h"

#include <stdarg.h>
#include <windows.h>

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

void info(const char *fmt, ...)
{
    CheckOrInitStdout();

    if (stdout == INVALID_HANDLE_VALUE)
    {
        return;
    }

    char buffer[1024];
    unsigned int i = format(buffer, sizeof(buffer), "\x1b[0m");
    va_list args;
    va_start(args, fmt);
    i += formatv(buffer + i, sizeof(buffer) - i, fmt, args);
    va_end(args);
    i += format(buffer + i, sizeof(buffer) - i, "\x1b[0m\n");
    assert(WriteConsoleA(stdout, buffer, i, 0, 0));
}

void error(const char *fmt, ...)
{
    CheckOrInitStdout();

    if (stdout == INVALID_HANDLE_VALUE)
    {
        return;
    }
    
    char buffer[1024];
    unsigned int i = format(buffer, sizeof(buffer), "\x1b[1m\x1b[1;31merror\x1b[0m: ");
    va_list args;
    va_start(args, fmt);
    i += formatv(buffer + i, sizeof(buffer) - i, fmt, args);
    va_end(args);
    i += format(buffer + i, sizeof(buffer) - i, "\x1b[0m\n");
    assert(WriteConsoleA(stdout, buffer, i, 0, 0));
}

unsigned int format(char *buffer, unsigned int size, const char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    unsigned int r = formatv(buffer, size, fmt, args);
    va_end(args);
    return r;
}

static unsigned int format_str(char *buffer, unsigned int size, const char *v)
{
    assert(buffer);
    unsigned int bi = 0;
    while (bi < size && *v) buffer[bi++] = *(v++);
    return bi;
}

static unsigned int format_int(char *buffer, unsigned int size, unsigned int base, int v)
{
    assert(buffer);

    const char table[] = {
        '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
    };

    unsigned int bi = 0;

    if (bi < size && v < 0)
    {
        buffer[bi++] = '-';
        v *= -1;
    }

    int d = 1;
    while ((unsigned int)(v / d) >= base)
    {
        d *= base;
    }
    while (bi < size && d != 0)
    {
        unsigned int table_idx = (unsigned int)(v / d);
        assert(table_idx < countof(table));
        buffer[bi++] = table[table_idx];
        v %= d;
        d /= base;
    }

    return bi;
}

unsigned int formatv(char *buffer, unsigned int size, const char *fmt, va_list args)
{
    assert(buffer);
    assert(fmt);

    unsigned int bi = 0;
    unsigned int fi = 0;
    while (bi < size && fmt[fi])
    {
        char c = fmt[fi];

        if (c != '%')
        {
            buffer[bi++] = fmt[fi++];
        }
        else
        {
            fi++;

            if (bi < size && fmt[fi])
            {
                c = fmt[fi++];

                switch (c)
                {
                case '%':
                    buffer[bi++] = '%';
                    break;

                case 's':
                    bi += format_str(buffer + bi, size - bi, va_arg(args, const char*));
                    break;

                case 'b':
                    bi += format_int(buffer + bi, size - bi, 2, va_arg(args, int));
                    break;

                case 'd':
                    bi += format_int(buffer + bi, size - bi, 10, va_arg(args, int));
                    break;

                case 'x':
                    bi += format_int(buffer + bi, size - bi, 16, va_arg(args, int));
                    break;
                }
            }
        }
    }

    return bi;
}
