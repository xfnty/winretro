#include "core.h"

#include "logger.h"
#include "miniwindows.h"

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
    u16 base_width;
    u16 base_height;
    u16 max_width;
    u16 max_height;
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

typedef u8    (*retro_environment_t)(u16 cmd, ptr data);
typedef void  (*retro_video_callback_t)(const ptr data, u16 width, u16 height, usize pitch);
typedef void  (*retro_audio_sample_t)(i16 left, i16 right);
typedef usize (*retro_audio_sample_batch_t)(const i16 *data, usize frames);
typedef void  (*retro_input_poll_t)(void);
typedef i16   (*retro_input_state_t)(u16 port, u16 device, u16 index, u16 id);

#define RETRO_API_DECL_LIST \
    _X(void,  retro_set_environment,            retro_environment_t callback) \
    _X(void,  retro_set_video_refresh,          retro_video_callback_t callback) \
    _X(void,  retro_set_audio_sample,           retro_audio_sample_t callback) \
    _X(void,  retro_set_audio_sample_batch,     retro_audio_sample_batch_t callback) \
    _X(void,  retro_set_input_poll,             retro_input_poll_t callback) \
    _X(void,  retro_set_input_state,            retro_input_state_t callback) \
    _X(void,  retro_init,                       void) \
    _X(void,  retro_deinit,                     void) \
    _X(u16,   retro_api_version,                void) \
    _X(void,  retro_get_system_info,            retro_system_info* info) \
    _X(void,  retro_get_system_av_info,         retro_system_av_info* avinfo) \
    _X(void,  retro_set_controller_port_device, u16 port, u16 device) \
    _X(void,  retro_reset,                      void) \
    _X(void,  retro_run,                        void) \
    _X(usize, retro_serialize_size,             void) \
    _X(u8,    retro_serialize,                  ptr data, usize len) \
    _X(u8,    retro_unserialize,                const ptr data, usize len) \
    _X(void,  retro_cheat_reset,                void) \
    _X(void,  retro_cheat_set,                  u16 index, u8 enabled, cstr code) \
    _X(u8,    retro_load_game,                  const retro_game_info *info) \
    _X(u8,    retro_load_game_special,          u16 type, const retro_game_info *info, usize count) \
    _X(void,  retro_unload_game,                void) \
    _X(u16,   retro_get_region,                 void) \
    _X(ptr,   retro_get_memory_data,            u16 type) \
    _X(usize, retro_get_memory_size,            u16 type)

typedef struct CorePrivate CorePrivate;
struct CorePrivate {
    Core base;
    Logger *logger;
    HMODULE dll;
    retro_system_info    info;
    retro_system_av_info avinfo;
    struct {
        #define _X(_ret, _name, _arg1, ...) _ret (*_name)(_arg1, ##__VA_ARGS__);
        RETRO_API_DECL_LIST
        #undef _X
    } api;
};

Core *CreateCore(cstr path)
{
    CorePrivate *core = HeapAlloc(
        GetProcessHeap(),
        HEAP_GENERATE_EXCEPTIONS | HEAP_ZERO_MEMORY,
        sizeof(*core)
    );

    core->logger = CreateLogger((LoggerParams){ .name = "core" });
    core->dll = LoadLibraryA(path);
    if (!core->logger || !core->dll) goto Failure;

    struct {
        cstr symbol;
        ptr *func;
    } load_list[] = {
        #define _X(_ret, _name, _arg1, ...) { #_name, (ptr)&core->api._name },
        RETRO_API_DECL_LIST
        #undef _X
    };
    for (i32 i = 0; i < countof(load_list); i++)
    {
        *load_list[i].func = (ptr)GetProcAddress(core->dll, load_list[i].symbol);
        if (!(*load_list[i].func))
        {
            LogError(core->logger, "symbol \"%s\" not found", load_list[i].symbol);
            goto Failure;
        }
    }

    if (core->api.retro_api_version() != 1)
    {
        LogError(core->logger, "surprisingly, given core does not use Libretro API version 1");
        goto Failure;
    }

    core->api.retro_get_system_info(&core->info);
    core->api.retro_get_system_av_info(&core->avinfo);
    core->base.name = core->info.library_name;
    return (Core*)core;

    Failure:
    FreeCore((Core**)&core);
    return 0;
}

void FreeCore(Core **core)
{
    if (!core) return;
    CorePrivate *p = (CorePrivate*)(*core);
    if (!p) return;
    FreeLogger(&p->logger);
    FreeLibrary(p->dll);
    HeapFree(GetProcessHeap(), 0, p);
    *core = 0;
}
