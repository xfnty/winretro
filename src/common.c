#include "common.h"

#include "error.h"
#include "windows.h"

u32 str_equals(cstr a, cstr b, u32 minsize)
{
    assert(a && b);
    for (u32 i = 0; i < minsize && a[i] == b[i]; i++)
        if (!a[i]) return true;
    return false;
}

u32 str_length(cstr a)
{
    assert(a);
    u32 l = 0;
    while (*(a++)) l++;
    return l;
}

u32 snprintf(c8 *buffer, u32 maxsize, cstr format, ...)
{
    assert(buffer && format);
    va_list args;
    va_start(args, format);
    return vsnprintf(buffer, maxsize, format, args);
}

u32 vsnprintf(c8 *buffer, u32 maxsize, cstr format, va_list args)
{
    assert(buffer && format && args);
    check_return_value(maxsize, 0);

    u32 fi = 0, bi = 0;

    while (bi < maxsize - 1 && format[fi])
    {
        if (format[fi] != '%')
        {
            buffer[bi++] = format[fi++];
        }
        else
        {
            fi++;

            cstr str;
            u64 ui, ud;
            i64 si, sd;
            f64 f;
            u8 size = 4, base = 10;

            const c8 table[] = {
                '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
            };

            switch (format[fi])
            {
            case 'h':
                size = 2;
                fi++;
                if (format[fi] == 'h') { size = 1; fi++; }
                break;

            case 'l':
                size = 4;
                fi++;
                if (format[fi] == 'l') { size = 8; fi++; }
                break;
            }

            switch (format[fi])
            {
            case '%':
                buffer[bi++] = '%';
                fi++;
                break;

            case 's':
                str = va_arg(args, cstr);
                if (!str) str = "(null)";
                else if (!str[0]) str = "(empty)";
                while (bi < maxsize - 1 && *str) buffer[bi++] = *(str++);
                fi++;
                break;

            case 'i':
            case 'd':
                si = va_arg(args, i64);
                if      (size == 1) si = (i8)si;
                else if (size == 2) si = (i16)si;
                else if (size == 4) si = (i32)si;
                if (si < 0 && bi < maxsize - 1) { buffer[bi++] = '-'; si *= -1; }
                for (sd = 1; si / sd >= base; sd *= base);
                while (bi < maxsize - 1 && sd > 0) { buffer[bi++] = table[si / sd]; si %= sd; sd /= 10; }
                fi++;
                break;

            case 'x':
            case 'p':
            case 'b':
                base = (format[fi] == 'b') ? (2) : (16);

            case 'u':
                ui = va_arg(args, u64);
                if      (size == 1) ui = (u8)ui;
                else if (size == 2) ui = (u16)ui;
                else if (size == 4) ui = (u32)ui;
                for (ud = 1; ui / ud >= base; ud *= base);
                while (bi < maxsize - 1 && ud > 0) { buffer[bi++] = table[ui / ud]; ui %= ud; ud /= base; }
                fi++;
                break;

            case 'f':
                f = va_arg(args, f64);
                if (size == 4) f = (f32)f;
                if (f < 0 && bi < maxsize - 1) { buffer[bi++] = '-'; f *= -1; }
                ui = (u64)(f * 1000.0f);
                for (ud = 1; ui / ud >= 10; ud *= 10);
                while (bi < maxsize - 1 && ud > 0) 
                {
                    buffer[bi++] = table[ui / ud]; 
                    ui %= ud;
                    ud /= 10;
                    if (ud == 100 && bi < maxsize - 1) buffer[bi++] = '.';
                }
                fi++;
                break;
            }
        }
    }
    buffer[bi] = '\0';

    return bi;
}

cstr get_root_directory(void)
{
    static c8 path[256] = {'\0'};

    if (!path[0])
    {
        u32 l = GetModuleFileNameA(0, path, sizeof(path) - 1);
        assert(l);
        while (path[l] != '\\') l--;
        path[l] = '\0';
    }

    return path;
}

f64 get_time(void)
{
    i64 f, c;
    assert(QueryPerformanceFrequency(&f));
    assert(QueryPerformanceCounter(&c));
    return (f64)c / f;
}

cstr get_filename(cstr path)
{
    check_return_value(path, 0);
    for (u32 i = 0; path[i + 1]; i++) if (path[i] == '\\') { path += i + 1; i = 0; }
    return path;
}

#pragma function(memset)
ptr memset(ptr dst, i32 byte, u64 size)
{
    RtlFillMemory(dst, size, byte);
    return dst;
}
