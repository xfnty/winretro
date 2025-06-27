#include "core.h"

#include "log.h"
#include "miniwindows.h"

#define RETRO_NUM_CORE_OPTION_VALUES_MAX 128
#define RETRO_ENVIRONMENT_EXPERIMENTAL 0x10000
#define RETRO_ENVIRONMENT_PRIVATE 0x20000
#define RETRO_ENVIRONMENT_GET_LOG_INTERFACE 27

typedef struct retro_system_info retro_system_info;
struct retro_system_info {
    cstr library_name;
    cstr library_version;
    cstr valid_extensions;
    u8   need_fullpath;
    u8   block_extract;
};

typedef struct retro_game_geometry retro_game_geometry;
struct retro_game_geometry {
    u32 base_width;
    u32 base_height;
    u32 max_width;
    u32 max_height;
    f32 aspect_ratio;
};

typedef struct retro_system_timing retro_system_timing;
struct retro_system_timing {
    f64 fps;
    f64 sample_rate;
};

typedef struct retro_system_av_info retro_system_av_info;
struct retro_system_av_info {
    retro_game_geometry geometry;
    retro_system_timing timing;
};

typedef struct retro_game_info retro_game_info;
struct retro_game_info {
    cstr  path;
    ptr   data;
    usize size;
    cstr  meta;
};

typedef enum retro_log_level retro_log_level;
enum retro_log_level {
   RETRO_LOG_DEBUG = 0,
   RETRO_LOG_INFO,
   RETRO_LOG_WARN,
   RETRO_LOG_ERROR,
};

typedef void  (*retro_log_printf_t)(retro_log_level level, const char *fmt, ...);
typedef u8    (*retro_environment_t)(u32 cmd, ptr data);
typedef void  (*retro_video_callback_t)(const ptr data, u32 width, u32 height, usize pitch);
typedef void  (*retro_audio_sample_t)(i16 left, i16 right);
typedef usize (*retro_audio_sample_batch_t)(const i16 *data, usize frames);
typedef void  (*retro_input_poll_t)(void);
typedef i16   (*retro_input_state_t)(u32 port, u32 device, u32 index, u32 id);

typedef struct retro_log_callback retro_log_callback;
struct retro_log_callback {
   retro_log_printf_t log;
};

#define RETRO_API_DECL_LIST \
    _X(void,  retro_set_environment,            retro_environment_t callback) \
    _X(void,  retro_set_video_refresh,          retro_video_callback_t callback) \
    _X(void,  retro_set_audio_sample,           retro_audio_sample_t callback) \
    _X(void,  retro_set_audio_sample_batch,     retro_audio_sample_batch_t callback) \
    _X(void,  retro_set_input_poll,             retro_input_poll_t callback) \
    _X(void,  retro_set_input_state,            retro_input_state_t callback) \
    _X(void,  retro_init,                       void) \
    _X(void,  retro_deinit,                     void) \
    _X(u32,   retro_api_version,                void) \
    _X(void,  retro_get_system_info,            retro_system_info* info) \
    _X(void,  retro_get_system_av_info,         retro_system_av_info* avinfo) \
    _X(void,  retro_set_controller_port_device, u32 port, u32 device) \
    _X(void,  retro_reset,                      void) \
    _X(void,  retro_run,                        void) \
    _X(usize, retro_serialize_size,             void) \
    _X(u8,    retro_serialize,                  ptr data, usize len) \
    _X(u8,    retro_unserialize,                const ptr data, usize len) \
    _X(void,  retro_cheat_reset,                void) \
    _X(void,  retro_cheat_set,                  u32 index, u8 enabled, cstr code) \
    _X(u8,    retro_load_game,                  const retro_game_info *info) \
    _X(u8,    retro_load_game_special,          u32 type, const retro_game_info *info, usize count) \
    _X(void,  retro_unload_game,                void) \
    _X(u32,   retro_get_region,                 void) \
    _X(ptr,   retro_get_memory_data,            u32 type) \
    _X(usize, retro_get_memory_size,            u32 type)

static struct {
    HMODULE dll;
    retro_system_info    info;
    retro_system_av_info avinfo;
    struct {
        #define _X(_ret, _name, _arg1, ...) _ret (*_name)(_arg1, ##__VA_ARGS__);
        RETRO_API_DECL_LIST
        #undef _X
    } api;
    f32 inputs[_CORE_AXIS_COUNT];
} s_core;

static void  LogCallback(retro_log_level level, const char *format, ...);
static u8    EnvironmentCallback(u32 cmd, ptr data);
static void  VideoCallback(const ptr data, u32 width, u32 height, usize pitch);
static void  AudioSampleCallback(i16 left, i16 right);
static usize AudioBatchCallback(const i16 *data, usize frames);
static void  InputPollCallback(void);
static i16   InputStateCallback(u32 port, u32 device, u32 index, u32 id);

u8 Core_Load(cstr path)
{
    assert(path);

    Core_Free();

    if (!(s_core.dll = LoadLibraryA(path)))
    {
        LogMessage(LOG_ERROR, "LoadLibraryA() failed (%d)", GetLastError());
        goto Failure;
    }

    struct {
        cstr symbol;
        ptr *func;
    } load_list[] = {
        #define _X(_ret, _name, _arg1, ...) { #_name, (ptr)&s_core.api._name },
        RETRO_API_DECL_LIST
        #undef _X
    };
    for (i32 i = 0; i < countof(load_list); i++)
    {
        *load_list[i].func = (ptr)GetProcAddress(s_core.dll, load_list[i].symbol);
        if (!(*load_list[i].func))
        {
            LogMessage(LOG_ERROR, "symbol \"%s\" not found", load_list[i].symbol);
            goto Failure;
        }
    }

    if (s_core.api.retro_api_version() != 1)
    {
        LogMessage(LOG_ERROR, "surprisingly, given core does not use Libretro API version 1");
        goto Failure;
    }

    s_core.api.retro_get_system_info(&s_core.info);
    s_core.api.retro_get_system_av_info(&s_core.avinfo);

    s_core.api.retro_set_environment(EnvironmentCallback);
    s_core.api.retro_set_video_refresh(VideoCallback);
    s_core.api.retro_set_audio_sample(AudioSampleCallback);
    s_core.api.retro_set_audio_sample_batch(AudioBatchCallback);
    s_core.api.retro_set_input_poll(InputPollCallback);
    s_core.api.retro_set_input_state(InputStateCallback);

    LogMessage(LOG_ERROR, "loaded %s %s", s_core.info.library_name, s_core.info.library_version);
    return true;

    Failure:
    LogMessage(LOG_ERROR, "failed to load \"%s\"", path);
    Core_Free();
    return false;
}

void Core_Free(void)
{
    FreeLibrary(s_core.dll);
    RtlZeroMemory(&s_core, sizeof(s_core));
}

cstr Core_GetName(void)
{
    return s_core.info.library_name;
}

u8 Core_SetRom(cstr path)
{
    assert(path);
    
    if (!s_core.dll) return false;

    retro_game_info info = {
        .path = path
    };
    return s_core.api.retro_load_game(&info);
}

u8 Core_SetInput(CoreInputAxisState input)
{
    assert((u32)input.axis < _CORE_AXIS_COUNT);

    if (!s_core.dll) return false;

    if (s_core.inputs[input.axis] != input.value)
    {
        LogMessage(LOG_INFO, "input %d %d", (i32)input.axis, (i32)input.value);
    }

    s_core.inputs[input.axis] = input.value;
    return true;
}

void LogCallback(retro_log_level level, const char *format, ...)
{
    (void)level;
    assert(s_core.dll);

    char buffer[1024];
    va_list args;
    va_start(args, format);
    formatv(buffer, sizeof(buffer), format, args);
    va_end(args);

    LogMessage(LOG_INFO, buffer);
}

u8 EnvironmentCallback(u32 cmd, ptr data)
{
    assert(s_core.dll);

    if (cmd & (RETRO_ENVIRONMENT_EXPERIMENTAL | RETRO_ENVIRONMENT_PRIVATE))
    {
        return false;
    }

    switch (cmd)
    {
    case RETRO_ENVIRONMENT_GET_LOG_INTERFACE:
        {
            ((retro_log_callback*)data)->log = LogCallback;
            return true;
        }
    }

    LogMessage(LOG_INFO, "ignored unknown core command %d", (i32)cmd);
    return false;
}

void VideoCallback(const ptr data, u32 width, u32 height, usize pitch)
{
    (void)data;
    (void)width;
    (void)height;
    (void)pitch;
}

void AudioSampleCallback(i16 left, i16 right)
{
    (void)left;
    (void)right;
}

usize AudioBatchCallback(const i16 *data, usize frames)
{
    (void)data;
    (void)frames;
    return 0;
}

void InputPollCallback(void)
{
}

i16 InputStateCallback(u32 port, u32 device, u32 index, u32 id)
{
    (void)port;
    (void)device;
    (void)index;
    (void)id;
    return 0;
}
