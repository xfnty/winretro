#pragma once

#if !defined(_MSC_VER) || !defined(_WIN64)
    #error Platform or compiler not supported
#endif

void __debugbreak(void);

#define countof(_x) (size)(sizeof(_x)/sizeof(*(_x)))
#define assert(_x) do { if (!(_x)) __debugbreak(); } while(0)
