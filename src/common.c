#include "common.h"

g_log_t g_log;
g_mem_t g_mem;

u32 str_equals(cstr a, cstr b, u32 minsize)
{
    for (u32 i = 0; i < minsize && a[i] == b[i]; i++)
        if (!a[i]) return true;
    return false;
}

void print(cstr format, ...)
{
    if (!g_log.initialized)
    {
        AttachConsole(ATTACH_PARENT_PROCESS);
        g_log.stdout = GetStdHandle(STD_OUTPUT_HANDLE);
        g_log.enabled = g_log.stdout != INVALID_HANDLE_VALUE || IsDebuggerPresent();
        g_log.initialized = true;
    }

    if (!g_log.enabled)
        return;

    c8 buffer[1024];

    u32 len = vsnprintf(buffer, sizeof(buffer) - 2, format, va_create(format));
    buffer[len++] = '\n';
    WriteConsoleA(g_log.stdout, buffer, len, 0, 0);
    buffer[len++] = '\0';
    OutputDebugStringA(buffer);
}

u32 snprintf(c8 *buffer, u32 maxsize, cstr format, ...)
{
    return vsnprintf(buffer, maxsize, format, va_create(format));
}

u32 vsnprintf(c8 *buffer, u32 maxsize, cstr format, va_list args)
{
    if (!maxsize) return 0;

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
                str = (str) ? (str) : ("(null)");
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

ptr mem_alloc(u32 size)
{
    if (!size) return 0;
    
    if (!g_mem.data)
    {
        SYSTEM_INFO info;
        GetSystemInfo(&info);

        g_mem.reserved = GB(1);
        if ((g_mem.reserved / info.dwPageSize) * info.dwPageSize < g_mem.reserved)
            g_mem.reserved += info.dwPageSize;
        
        g_mem.data = VirtualAlloc(0, g_mem.reserved, MEM_RESERVE, PAGE_READWRITE);
        assert(g_mem.data);
    }

    assert(g_mem.used + size <= g_mem.reserved);

    ptr p = VirtualAlloc(g_mem.data + g_mem.used, size, MEM_COMMIT, PAGE_READWRITE);
    assert(p);
    RtlFillMemory(p, size, 0xCC);

    g_mem.used += size;
    return p;
}

void mem_reset(ptr p)
{
    if (!p)
    {
        VirtualFree(g_mem.data, 0, MEM_RELEASE);
        RtlZeroMemory(&g_mem, sizeof(g_mem));
        return;
    }

    assert(g_mem.data);
    assert((u64)p >= (u64)g_mem.data);

    u64 p_start_offset = (u64)p - (u64)g_mem.data;
    assert(p_start_offset < g_mem.used);

    RtlFillMemory(p, g_mem.used - p_start_offset, 0xCC);
    g_mem.used = p_start_offset;
}
