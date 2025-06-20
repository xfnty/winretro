#pragma once

#if !defined(_MSC_VER) || !defined(_WIN64)
    #error Only 64-bit Windows and MSVC are supported
#endif

typedef char c8;
typedef __int8  i8;
typedef __int16 i16;
typedef __int32 i32;
typedef __int64 i64;
typedef unsigned __int8  u8;
typedef unsigned __int16 u16;
typedef unsigned __int32 u32;
typedef unsigned __int64 u64;
typedef __int64 size;
typedef unsigned __int64 usize;
typedef const char *cstr;
typedef float  f32;
typedef double f64;
typedef void *ptr;
typedef char *va_list;

#ifndef true
    #define true 1u
#endif
#ifndef false
    #define false 0
#endif

void __debugbreak(void);
void __cdecl __va_start(va_list* , ...);

#define countof(_x) (size)(sizeof(_x)/sizeof(*(_x)))
#define assert(_x) do { if (!(_x)) __debugbreak(); } while(0)
#define va_start(_list, _arg) ((void)(__va_start(&_list, _arg)))
#define va_arg(_list, _T) ( \
        (sizeof(_T) > sizeof(__int64) || (sizeof(_T) & (sizeof(_T) - 1)) != 0) \
        ? ( **(_T**)((_list += sizeof(__int64)) - sizeof(__int64)) ) \
        : (  *(_T* )((_list += sizeof(__int64)) - sizeof(__int64)) ) \
    )
#define va_end(_list) ((void)(_list = (va_list)0));

void info(cstr format, ...);
void error(cstr format, ...);

u32 format(c8 *buffer, u32 maxsize, cstr fmt, ...);
u32 formatv(c8 *buffer, u32 maxsize, cstr fmt, va_list args);
