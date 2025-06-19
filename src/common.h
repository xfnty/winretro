#pragma once

#if !defined(_MSC_VER) || !defined(_WIN64)
    #error Platform or compiler not supported
#endif

#include <stdarg.h>
#include <stdint.h>

#ifndef ssize_t
    typedef __int64 ssize_t;
#endif

void __debugbreak(void);

#define countof(_x) (ssize_t)(sizeof(_x)/sizeof(*(_x)))
#define assert(_x) do { if (!(_x)) __debugbreak(); } while(0)

void info(const char *format, ...);
void error(const char *format, ...);

unsigned int format(char *buffer, unsigned int size, const char *fmt, ...);
unsigned int formatv(char *buffer, unsigned int size, const char *fmt, va_list args);
