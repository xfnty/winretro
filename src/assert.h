#pragma once

#include "log.h"
#include "windows.h"

#if defined(_MSC_VER)
    #define crash() do { __debugbreak(); } while(0)
#else
    #define crash() do { __asm__("int3"); __builtin_unreachable(); } while(0)
#endif

#define assert(_x) do { if (!(_x)) crash(); } while (0)
#define assertp(_x, _fmt, ...) do { if (!(_x)) { print(_fmt, ##__VA_ARGS__); crash(); } } while (0)

#define check_goto(_x, _l)                   do { if (!(_x)) goto _l } while (0)
#define check_return_value(_x, _v)           do { if (!(_x)) return (_v); } while (0)
#define checkp_goto(_x, _l, _m, ...)         do { if (!(_x)) { print(_m, ##__VA_ARGS__); goto _l; } } while (0)
#define checkp_return(_x, _m, ...)           do { if (!(_x)) { print(_m, ##__VA_ARGS__); return; } } while (0)
#define checkp_return_value(_x, _v, _m, ...) do { if (!(_x)) { print(_m, ##__VA_ARGS__); return (_v); } } while (0)

#define _assert_winapi_body(_var, _cond, _f, _fmt, ...) \
        _var = _f(__VA_ARGS__); \
        if (_cond) break; \
        u32 __e = GetLastError(); \
        SetLastError(__e); \
        print(#_f "(" _fmt ") failed: %s (%d) [%s():%d]", ##__VA_ARGS__, get_last_error_text(__e, ##__VA_ARGS__), __e, __func__, __LINE__);

#define assert_winapi(_var, _cond, _f, _fmt, ...) do { \
        _assert_winapi_body(_var, _cond, _f, _fmt, ##__VA_ARGS__); \
        crash(); \
    } while (0)
#define assert_winapi_goto(_var, _cond, _label, _f, _fmt, ...) do { \
        _assert_winapi_body(_var, _cond, _f, _fmt, ##__VA_ARGS__); \
        goto _label; \
    } while (0)
