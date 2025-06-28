/* typedefs */
typedef char               c8;
typedef char               i8;
typedef short              i16;
typedef int                i32;
typedef signed long long   i64;
typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned long long u64;
typedef float              f32;
typedef double             f64;
typedef const char        *cstr;
typedef void              *ptr;
typedef unsigned char     *va_list;

/* defines */
#define true 1u
#define false 0

#if defined(_MSC_VER)
    #define WINAPI __stdcall
#else
    #define WINAPI __attribute__((stdcall))
#endif

#define INVALID_HANDLE_VALUE  (ptr)-1
#define ATTACH_PARENT_PROCESS (u32)-1
#define STD_OUTPUT_HANDLE     (u32)-11

/* macros */
#define countof(_a) (sizeof(_a)/sizeof((_a)[0]))
#define va_start(_list, _last_arg) do { _list = (u8*)&(_last_arg) + 8; } while (0)
#define va_arg(_list, _T) ( ( sizeof(_T) > 8) ? (**(_T**)((_list += 8) - 8)) : ( *(_T* )((_list += 8) - 8)) )

#if defined(_MSC_VER)
    #define assert(_x) do { if (!(_x)) __debugbreak(); } while (0)
#else
    #define assert(_x) do { if (!(_x)) { asm("int3"); __builtin_unreachable(); } } while (0)
#endif

/* imports */
void ExitProcess(u32 code);
u32 GetLastError(void);
u32 WINAPI AttachConsole(u32 pid);
ptr WINAPI GetStdHandle(u32 id);
u32 WriteConsoleA(ptr console, ptr buffer, u32 bytes, u32 *written, ptr _reserved);
void OutputDebugStringA(cstr str);

/* function declarations */
void _start(void);
void init_logging(void);
void info(cstr format, ...);
u32 snprintf(c8 *buffer, u32 maxsize, cstr format, ...);
u32 vsnprintf(c8 *buffer, u32 maxsize, cstr format, va_list args);

/* variables */
ptr stdout;
int _fltused=1;

/* function definitions */
void _start(void)
{
    init_logging();
    info("Hello, %llu %hhu %hx %s %f %llf", -1LL, 256, 0x10AB0, "text", -1.1234f, 1.7976931348623157E+16);

    ExitProcess(0);
}

void init_logging(void)
{
    AttachConsole(ATTACH_PARENT_PROCESS);
    stdout = GetStdHandle(STD_OUTPUT_HANDLE);
}

void info(cstr format, ...)
{
    c8 buffer[1024];

    va_list args;
    va_start(args, format);
    u32 len = vsnprintf(buffer, sizeof(buffer), format, args);

    WriteConsoleA(stdout, buffer, len, 0, 0);
    OutputDebugStringA(buffer);
}

u32 snprintf(c8 *buffer, u32 maxsize, cstr format, ...)
{
    va_list args;
    va_start(args, format);
    return vsnprintf(buffer, maxsize, format, args);
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
