#pragma once

// #include <stdarg.h>

/* defines */
#define true 1u
#define false 0
#define INVALID_IDX ((u32)-1)


/* typedefs */
typedef char                c8;
typedef char                i8;
typedef short               i16;
typedef int                 i32;
typedef signed long long    i64;
typedef unsigned char       u8;
typedef unsigned short      u16;
typedef unsigned int        u32;
typedef unsigned long long  u64;
typedef float               f32;
typedef double              f64;
typedef const char         *cstr;
typedef void               *ptr;
typedef unsigned char      *va_list;

typedef enum state_t state_t;
enum state_t {
    STATE_UNINITIALIZED = 0,
    STATE_INITIALIZED,
    STATE_ACTIVE,
};


/* macros */
#define KB(_b) ((_b) * 1024)
#define MB(_b) ((_b) * 1024 * 1024)
#define GB(_b) ((_b) * 1024 * 1024 * 1024)
#define min(_a, _b) ((_a) < (_b) ? (_a) : (_b))
#define max(_a, _b) ((_a) > (_b) ? (_a) : (_b))
#define countof(_a) (sizeof(_a)/sizeof((_a)[0]))
#define va_start(_args, _last_arg) (_args = (u8*)&(_last_arg) + 8)
#define va_arg(_list, _T) ( ( sizeof(_T) > 8) ? (**(_T**)((_list += 8) - 8)) : ( *(_T* )((_list += 8) - 8)) )


/* function declarations */
u32  str_equals(cstr a, cstr b, u32 minsize);
u32  str_length(cstr a);
u32  snprintf(c8 *buffer, u32 maxsize, cstr format, ...);
u32  vsnprintf(c8 *buffer, u32 maxsize, cstr format, va_list args);
cstr get_root_directory(void);
f64  get_time(void);
cstr get_filename(cstr full_path);

ptr memset(ptr dst, i32 byte, u64 size);
#pragma intrinsic(memset)
