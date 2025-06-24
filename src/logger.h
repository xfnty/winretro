#pragma once

#include "common.h"

typedef struct LoggerParams LoggerParams;
struct LoggerParams {
    cstr name;
};

typedef struct Logger Logger;
struct Logger {
    LoggerParams params;
};

Logger *CreateLogger(LoggerParams params);
void FreeLogger(Logger **logger);

void LogInfo(Logger *logger, cstr format, ...);
void LogError(Logger *logger, cstr format, ...);
