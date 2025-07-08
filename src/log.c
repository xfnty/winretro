#include "log.h"

#include "error.h"
#include "windows.h"

typedef enum g_log_state_t g_log_state_t;
enum g_log_state_t {
    LOG_UNINITIALIZED = 0,
    LOG_INITIALIZED,
    LOG_ACTIVE,
};

static struct {
    g_log_state_t state;
    ptr stdout;
    c8 filepath[256];
    ptr file;
    c8 buffer[1024];
} g_log;

void init_log(void)
{
    free_log();
    
    u32 l = snprintf(g_log.filepath, sizeof(g_log.filepath), "%s\\logs", get_root_directory());
    CreateDirectoryA(g_log.filepath, 0);

    SYSTEMTIME time;
    GetSystemTime(&time);
    snprintf(
        g_log.filepath + l,
        sizeof(g_log.filepath) - l,
        "\\%hu-%hu-%hu_%hu-%hu-%hu.%hu.txt",
        time.wYear, time.wMonth, time.wDay,
        time.wHour, time.wMinute, time.wSecond, time.wMilliseconds
    );
    
    AttachConsole(ATTACH_PARENT_PROCESS);
    g_log.stdout = GetStdHandle(STD_OUTPUT_HANDLE);

    g_log.file = CreateFileA(g_log.filepath, GENERIC_WRITE, FILE_SHARE_READ, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
    if (g_log.file == INVALID_HANDLE_VALUE && g_log.stdout != INVALID_HANDLE_VALUE)
    {
        l = snprintf(
            g_log.buffer,
            sizeof(g_log.buffer),
            "%s:%d: error: failed to create log file \"%s\" (%d)",
            get_filename(__FILE__),
            __LINE__,
            g_log.filepath,
            GetLastError()
        );
        WriteConsoleA(g_log.stdout, g_log.buffer, l, 0, 0);
    }

    u8 has_sink = g_log.file != INVALID_HANDLE_VALUE || g_log.stdout != INVALID_HANDLE_VALUE || IsDebuggerPresent();
    g_log.state = (has_sink) ? (LOG_ACTIVE) : (LOG_INITIALIZED);
}

void free_log(void)
{
    if (g_log.state == LOG_INITIALIZED)
    {
        FlushFileBuffers(g_log.file);
    }

    CloseHandle(g_log.file);
    RtlZeroMemory(&g_log, sizeof(g_log));
    
    SetLastError(0);
}

void print(cstr format, ...)
{
    assert_report(g_log.state >= LOG_INITIALIZED);
    check_return(g_log.state == LOG_ACTIVE);

    va_list args;
    va_start(args, format);
    u32 len = vsnprintf(g_log.buffer, sizeof(g_log.buffer) - 2, format, args);
    g_log.buffer[len++] = '\n';

    if (g_log.stdout != INVALID_HANDLE_VALUE) WriteConsoleA(g_log.stdout, g_log.buffer, len, 0, 0);
    if (g_log.file != INVALID_HANDLE_VALUE)   WriteFile(g_log.file, g_log.buffer, len, 0, 0);
    
    g_log.buffer[len++] = '\0';
    OutputDebugStringA(g_log.buffer);
}

void print_error(cstr file, i32 line, cstr format, ...)
{
    assert_report(g_log.state >= LOG_INITIALIZED);
    check_return(g_log.state == LOG_ACTIVE);

    u32 len = snprintf(g_log.buffer, sizeof(g_log.buffer) - 2, "%s:%d: error: ", get_filename(file), line);

    va_list args;
    va_start(args, format);
    len += vsnprintf(g_log.buffer + len, sizeof(g_log.buffer) - len - 2, format, args);
    g_log.buffer[len++] = '\n';

    if (g_log.stdout != INVALID_HANDLE_VALUE) WriteConsoleA(g_log.stdout, g_log.buffer, len, 0, 0);
    if (g_log.file != INVALID_HANDLE_VALUE)   WriteFile(g_log.file, g_log.buffer, len, 0, 0);
    
    g_log.buffer[len++] = '\0';
    OutputDebugStringA(g_log.buffer);
}
