#pragma once

#include "common.h"

typedef enum LogLevel LogLevel;
enum LogLevel {
    LOG_INFO,
    LOG_ERROR,
};

void InitLog(void);
void LogMessage(LogLevel level, cstr format, ...);
