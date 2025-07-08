#pragma once

#include "common.h"

typedef struct result_t result_t;
struct result_t {
    union {
        ptr value;
        struct {
            c8   text[128];
            cstr file;
            i32  line;
        } error;
    };
    u8 ok;
};
#define result(_value) ((result_t){ .value = (ptr)(_value), .ok = true })

result_t create_error(cstr file, i32 line, cstr text, ...);
#define error(_text, ...) create_error(__FILE__, __LINE__, _text, ##__VA_ARGS__);

#define crash() __debugbreak()

#define assert(_condition)                                     do { if (!(_condition)) __debugbreak(); } while(0)
#define assert_report(_condition)                              do { if (!(_condition)) { print_error(__FILE__, __LINE__, "assertion \"" #_condition"\" failed"); __debugbreak(); } } while(0)
#define check_report(_condition)                               do { if (!(_condition)) print_error(__FILE__, __LINE__, "check \"" #_condition"\" failed"); } while(0)
#define check_return(_condition)                               do { if (!(_condition)) return; } while(0)
#define check_return_value(_condition, _value)                 do { if (!(_condition)) return _value; } while(0)
#define check_return_error(_condition, _text, ...)             do { if (!(_condition)) return error(_text, ##__VA_ARGS__); } while(0)
#define check_goto(_condition, _label)                         do { if (!(_condition)) goto _label; } while(0)
#define assert_print_error(_condition, _text, ...)             do { if (!(_condition)) { print_error(__FILE__, __LINE__, _text, ##__VA_ARGS__); __debugbreak(); } } while(0)
#define check_print_error_goto(_condition, _label, _text, ...) do { if (!(_condition)) { print_error(__FILE__, __LINE__, _text, ##__VA_ARGS__); goto _label; } } while(0)

ptr     assert_result(result_t result);
#define assert_result_report(_result)                  do { result_t _r = (_result); if (!_r.ok) { report_error(_r); __debugbreak(); } } while(0)
#define check_result_return(_result)                   check_return((_result).ok)
#define check_result_return_value(_result, _value)     check_return_value((_result).ok, _value)
#define check_result_return_error(_result, _text, ...) check_return_error((_result).ok, _text, ##__VA_ARGS__)
#define check_result_goto(_result, _label)             check_goto((_result).ok, _label)
#define check_result_propagate(_result)                do { result_t _r = _result; if (!_r.ok) return _r; } while(0)

cstr get_winapi_error(u32 code, ...);

#define _check_winapi_report_body(_func, _args_fmt, ...) \
    SetLastError(0); \
    _func(__VA_ARGS__); \
    u32 __e = GetLastError(); \
    if (!__e) break; \
    print_error( \
        __FILE__, \
        __LINE__, \
        "%s(" _args_fmt ") failed: %s (%u)", \
        #_func, \
        ##__VA_ARGS__, \
        get_winapi_error(__e, ##__VA_ARGS__), \
        __e \
    ); \
    SetLastError(0);
#define check_winapi_report(_func, _args_fmt, ...) do { \
        _check_winapi_report_body(_func, _args_fmt, __VA_ARGS__) \
    } while (0)
#define check_winapi_report_goto(_label, _func, _args_fmt, ...) do { \
        _check_winapi_report_body(_func, _args_fmt, __VA_ARGS__) \
        goto _label; \
    } while (0)
#define assert_winapi_report(_func, _args_fmt, ...) do { \
        _check_winapi_report_body(_func, _args_fmt, __VA_ARGS__) \
        crash(); \
    } while (0)

#define _check_winapi_retval_report_body(_var, _cond, _func, _args_fmt, ...) \
    SetLastError(0); \
    _var = _func(__VA_ARGS__); \
    u32 __e = GetLastError(); \
    if (_cond) break; \
    print_error( \
        __FILE__, \
        __LINE__, \
        "%s(" _args_fmt ") failed: %s (%u)", \
        #_func, \
        ##__VA_ARGS__, \
        get_winapi_error(__e, ##__VA_ARGS__), \
        __e \
    ); \
    SetLastError(0);
#define check_winapi_retval_report(_var, _cond, _func, _args_fmt, ...) do { \
        _check_winapi_retval_report_body(_var, _cond, _func, _args_fmt, ##__VA_ARGS__) \
    } while (0)
#define check_winapi_retval_report_goto(_var, _cond, _label, _func, _args_fmt, ...) do { \
        _check_winapi_retval_report_body(_var, _cond, _func, _args_fmt, ##__VA_ARGS__) \
        goto _label; \
    } while (0)
#define assert_winapi_retval_report(_var, _cond, _func, _args_fmt, ...) do { \
        _check_winapi_retval_report_body(_var, _cond, _func, _args_fmt, ##__VA_ARGS__) \
        crash(); \
    } while (0)

#define result_as(_T, _result) ((_T)check_result_crash(_result))

void report_error(result_t result);
