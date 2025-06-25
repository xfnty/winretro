#include "logger.h"

#include "miniwindows.h"

typedef struct LoggerPrivate LoggerPrivate;
struct LoggerPrivate {
    Logger base;
    HANDLE stdout;
};

Logger *CreateLogger(LoggerParams params)
{
    LoggerPrivate *logger = HeapAlloc(
        GetProcessHeap(),
        HEAP_GENERATE_EXCEPTIONS | HEAP_ZERO_MEMORY,
        sizeof(*logger)
    );

    logger->base.params = params;
    assert(params.name);

    AttachConsole(ATTACH_PARENT_PROCESS);
    logger->stdout = GetStdHandle(STD_OUTPUT_HANDLE);

    return (Logger*)logger;
}

void FreeLogger(Logger **logger)
{
    assert(logger);
    if (!(*logger)) return;
    HeapFree(GetProcessHeap(), 0, *logger);
    *logger = 0;
}

static u32 FormatMessage(char *buffer, u32 size, cstr name, cstr prefix, cstr fmt, va_list args)
{
    u32 i = 0;
    if (name) i += format(buffer + i, size - 2 - i, "[%s] ", name);
    if (prefix) i += format(buffer + i, size - 2 - i, prefix);
    i += formatv(buffer + i, size - 2 - i, fmt, args);
    buffer[i++] = '\n';
    buffer[i++] = '\0';
    return i;
}

void LogInfo(Logger *logger, cstr fmt, ...)
{
    assert(logger);

    LoggerPrivate *loggerp = (LoggerPrivate*)logger;

    char buffer[1024];
    va_list args;
    va_start(args, fmt);
    u32 len = FormatMessage(buffer, sizeof(buffer), loggerp->base.params.name, "", fmt, args);
    va_end(args);

    WriteConsoleA(loggerp->stdout, buffer, len, 0, 0);
    OutputDebugStringA(buffer);
}

void LogError(Logger *logger, cstr fmt, ...)
{
    assert(logger);

    char buffer[1024];
    va_list args;
    va_start(args, fmt);
    u32 len = FormatMessage(buffer, sizeof(buffer), logger->params.name, "error: ", fmt, args);
    va_end(args);

    WriteConsoleA(((LoggerPrivate*)logger)->stdout, buffer, len, 0, 0);
    OutputDebugStringA(buffer);
}
