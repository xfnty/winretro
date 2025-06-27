#include "log.h"

#include "miniwindows.h"

static struct {
    HANDLE stdout;
} s_log;

void InitLog(void)
{
    AttachConsole(ATTACH_PARENT_PROCESS);
    s_log.stdout = GetStdHandle(STD_OUTPUT_HANDLE);
}

void LogMessage(LogLevel level, cstr fmt, ...)
{
    cstr level_prefixes[] = {
        [LOG_ERROR] = "error: ",
    };

    char usermsg[1024];
    va_list args;
    va_start(args, fmt);
    u32 ulen = formatv(usermsg, sizeof(usermsg) - 1, fmt, args);
    usermsg[ulen] = '\0';
    va_end(args);

    char msg[1124];
    u32 len = 0;
    if (level_prefixes[level]) len += format(msg + len, sizeof(msg) - 2 - len, level_prefixes[level]);
    len += format(msg + len, sizeof(msg) - 2 - len, usermsg);
    msg[len++] = '\n';

    if (s_log.stdout != INVALID_HANDLE_VALUE)
        WriteConsoleA(s_log.stdout, msg, len, 0, 0);

    msg[len] = '\0';
    OutputDebugStringA(msg);
}
