#include "core.h"

#include "gl.h"
#include "log.h"
#include "error.h"
#include "windows.h"

#define CORE_SETTINGS_FILENAME_FORMAT "%s (Variables).ini"
#define CORE_INPUTS_FILENAME_FORMAT   "%s (Inputs).ini"

#define RETRO_HW_FRAME_BUFFER_VALID                                ((ptr)-1)
#define RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY                     9
#define RETRO_ENVIRONMENT_SET_PIXEL_FORMAT                         10
#define RETRO_ENVIRONMENT_SET_INPUT_DESCRIPTORS                    11
#define RETRO_ENVIRONMENT_SET_DISK_CONTROL_INTERFACE               13
#define RETRO_ENVIRONMENT_SET_HW_RENDER                            14
#define RETRO_ENVIRONMENT_GET_VARIABLE                             15
#define RETRO_ENVIRONMENT_SET_VARIABLES                            16
#define RETRO_ENVIRONMENT_GET_VARIABLE_UPDATE                      17
#define RETRO_ENVIRONMENT_GET_RUMBLE_INTERFACE                     23
#define RETRO_ENVIRONMENT_GET_LOG_INTERFACE                        27
#define RETRO_ENVIRONMENT_GET_SAVE_DIRECTORY                       31
#define RETRO_ENVIRONMENT_SET_CONTROLLER_INFO                      35
#define RETRO_ENVIRONMENT_GET_VFS_INTERFACE                        (45 | RETRO_ENVIRONMENT_EXPERIMENTAL)
#define RETRO_ENVIRONMENT_GET_CORE_OPTIONS_VERSION                 52
#define RETRO_ENVIRONMENT_SET_CORE_OPTIONS_DISPLAY                 55
#define RETRO_ENVIRONMENT_GET_PREFERRED_HW_RENDER                  56
#define RETRO_ENVIRONMENT_GET_DISK_CONTROL_INTERFACE_VERSION       57
#define RETRO_ENVIRONMENT_GET_MESSAGE_INTERFACE_VERSION            59
#define RETRO_ENVIRONMENT_SET_CORE_OPTIONS_UPDATE_DISPLAY_CALLBACK 69
#define RETRO_ENVIRONMENT_EXPERIMENTAL                             0x10000
#define RETRO_ENVIRONMENT_GET_CURRENT_SOFTWARE_FRAMEBUFFER        (40 | RETRO_ENVIRONMENT_EXPERIMENTAL)

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
    cstr path;
    ptr  data;
    u64  size;
    cstr meta;
};

typedef struct retro_variable retro_variable;
struct retro_variable {
    cstr key;
    cstr value;
};

typedef void (*retro_log_printf_t)(u32 level, cstr fmt, ...);
typedef u8   (*retro_environment_t)(u32 cmd, ptr data);
typedef void (*retro_video_callback_t)(ptr data, u32 width, u32 height, u64 pitch);
typedef void (*retro_audio_sample_t)(i16 left, i16 right);
typedef u64  (*retro_audio_sample_batch_t)(i16 *data, u64 frames);
typedef void (*retro_input_poll_t)(void);
typedef i16  (*retro_input_state_t)(u32 port, u32 device, u32 index, u32 id);

typedef struct retro_log_callback retro_log_callback;
struct retro_log_callback {
   retro_log_printf_t log;
};

typedef enum retro_hw_context_type retro_hw_context_type;
enum retro_hw_context_type {
   RETRO_HW_CONTEXT_NONE             = 0,
   RETRO_HW_CONTEXT_OPENGL           = 1,
   RETRO_HW_CONTEXT_OPENGLES2        = 2,
   RETRO_HW_CONTEXT_OPENGL_CORE      = 3,
   RETRO_HW_CONTEXT_OPENGLES3        = 4,
   RETRO_HW_CONTEXT_OPENGLES_VERSION = 5,
   RETRO_HW_CONTEXT_VULKAN           = 6,
   RETRO_HW_CONTEXT_D3D11            = 7,
   RETRO_HW_CONTEXT_D3D10            = 8,
   RETRO_HW_CONTEXT_D3D12            = 9,
   RETRO_HW_CONTEXT_D3D9             = 10,
};

typedef enum retro_pixel_format retro_pixel_format;
enum retro_pixel_format {
   RETRO_PIXEL_FORMAT_0RGB1555 = 0,
   RETRO_PIXEL_FORMAT_XRGB8888 = 1,
   RETRO_PIXEL_FORMAT_RGB565   = 2,
};

typedef void (*retro_hw_context_reset_t)(void);
typedef u64  (*retro_hw_get_current_framebuffer_t)(void);
typedef ptr  (*retro_hw_get_proc_address_t)(cstr sym);

typedef struct retro_hw_render_callback retro_hw_render_callback;
struct retro_hw_render_callback {
   retro_hw_context_type context_type;
   retro_hw_context_reset_t context_reset;
   retro_hw_get_current_framebuffer_t get_current_framebuffer;
   retro_hw_get_proc_address_t get_proc_address;
   u8 depth;
   u8 stencil;
   u8 bottom_left_origin;
   u32 version_major;
   u32 version_minor;
   u8 cache_context;
   retro_hw_context_reset_t context_destroy;
   u8 debug_context;
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
    _X(u64,   retro_serialize_size,             void) \
    _X(u8,    retro_serialize,                  ptr data, u64 len) \
    _X(u8,    retro_unserialize,                const ptr data, u64 len) \
    _X(void,  retro_cheat_reset,                void) \
    _X(void,  retro_cheat_set,                  u32 index, u8 enabled, cstr code) \
    _X(u8,    retro_load_game,                  const retro_game_info *info) \
    _X(u8,    retro_load_game_special,          u32 type, const retro_game_info *info, u64 count) \
    _X(void,  retro_unload_game,                void) \
    _X(u32,   retro_get_region,                 void) \
    _X(ptr,   retro_get_memory_data,            u32 type) \
    _X(u64,   retro_get_memory_size,            u32 type)

static struct {
    state_t state;
    ptr module;
    retro_system_info info;
    retro_system_av_info avinfo;
    retro_hw_render_callback *hw;
    struct {
        #define _X(_ret, _name, _arg1, ...) _ret (*_name)(_arg1, ##__VA_ARGS__);
        RETRO_API_DECL_LIST
        #undef _X
    } api;
    struct {
        u32 count;
        u32 allocated;
        struct {
            c8 key[64];
            c8 value[32];
        } *array;
    } vars;
    struct {
        c8 save[256];
        c8 system[256];
        c8 settings[256];
        c8 settings_file[256];
    } paths;
} g_core;

static void load_core_variables(void);
static void save_core_variables(void);
static cstr get_core_variable(cstr key);
static void update_core_variable(cstr key, cstr value);
static u32  get_core_variable_idx(cstr key);
static void core_log_callback(u32 level, cstr format, ...);
static u8   core_environment_callback(u32 cmd, ptr data);
static void core_video_callback(ptr data, u32 width, u32 height, u64 pitch);
static void core_audio_sample_callback(i16 left, i16 right);
static u64  core_audio_batch_callback(i16 *data, u64 frames);
static void core_input_poll_callback(void);
static i16  core_input_state_callback(u32 port, u32 device, u32 index, u32 id);

void init_core(cstr dll)
{
    assert_report(dll);

    free_core();

    struct { c8 *str; cstr format; u32 size; } paths[] = {
        { g_core.paths.save, "%s\\save", sizeof(g_core.paths.save) },
        { g_core.paths.system, "%s\\system", sizeof(g_core.paths.system) },
        { g_core.paths.settings, "%s\\settings", sizeof(g_core.paths.settings) },
    };
    for (u32 i = 0; i < countof(paths); i++)
    {
        snprintf(paths[i].str, paths[i].size, paths[i].format, get_root_directory());
        check_print_error_goto(
            CreateDirectoryA(paths[i].str, 0) || GetLastError() == ERROR_ALREADY_EXISTS,
            Failure,
            "CreateDirectoryA(\"%s\", %p) failed: %s (%d)",
            get_winapi_error(GetLastError(), paths[i].str, 0),
            GetLastError()
        );
        SetLastError(0);
    }

    check_winapi_retval_report_goto(g_core.module, g_core.module, Failure, LoadLibraryA, "\"%s\"", dll);

    struct {
        cstr symbol;
        ptr *func;
    } load_list[] = {
        #define _X(_ret, _name, _arg1, ...) { #_name, (ptr)&g_core.api._name },
        RETRO_API_DECL_LIST
        #undef _X
    };
    for (u32 i = 0; i < countof(load_list); i++)
    {
        check_winapi_retval_report_goto(
            *load_list[i].func,
            *load_list[i].func,
            Failure,
            GetProcAddress,
            "%p, \"%s\"",
            g_core.module, load_list[i].symbol
        );
    }

    u32 api = g_core.api.retro_api_version();
    check_print_error_goto(api == 1, Failure, "core uses unsupported API version %d", api);

    g_core.api.retro_get_system_info(&g_core.info);

    snprintf(g_core.paths.settings_file, sizeof(g_core.paths.settings_file), "%s\\" CORE_SETTINGS_FILENAME_FORMAT, g_core.paths.settings, g_core.info.library_name);
    load_core_variables();

    g_core.api.retro_set_environment(core_environment_callback);
    g_core.api.retro_set_video_refresh(core_video_callback);
    g_core.api.retro_set_audio_sample(core_audio_sample_callback);
    g_core.api.retro_set_audio_sample_batch(core_audio_batch_callback);
    g_core.api.retro_set_input_poll(core_input_poll_callback);
    g_core.api.retro_set_input_state(core_input_state_callback);
    g_core.api.retro_init();

    g_core.api.retro_get_system_av_info(&g_core.avinfo);
    configure_gl(g_core.avinfo.geometry.max_width, g_core.avinfo.geometry.max_height);

    g_core.state = STATE_INITIALIZED;

    print(
        "loaded %s (%d FPS, %dx%d)",
        g_core.info.library_name,
        (u32)g_core.avinfo.timing.fps,
        g_core.avinfo.geometry.max_width,
        g_core.avinfo.geometry.max_height
    );

    return;
    Failure:
    free_core();
    return;
}

void free_core(void)
{
    if (g_core.state >= STATE_INITIALIZED)
    {
        unload_game();
        save_core_variables();
        g_core.api.retro_deinit();
        print("unloaded %s", g_core.info.library_name);
        check_winapi_report(FreeLibrary, "%p", g_core.module);
    }

    RtlZeroMemory(&g_core, sizeof(g_core));
    SetLastError(0);
}

void load_game(cstr rom)
{
    check_return(g_core.state >= STATE_INITIALIZED);
    if (g_core.state == STATE_ACTIVE) unload_game();

    retro_game_info info = { .path = rom };
    if (g_core.api.retro_load_game(&info))
    {
        g_core.state = STATE_ACTIVE;
        print("loaded game \"%s\"", rom);
    }
}

void unload_game(void)
{
    check_return(g_core.state == STATE_ACTIVE);
    g_core.api.retro_unload_game();
    g_core.state = STATE_INITIALIZED;
    print("unloaded game");
}

void run_frame(void)
{
    assert_report(g_core.state == STATE_ACTIVE);
    g_core.api.retro_run();
}

void load_game_state(cstr path)
{
    check_return(g_core.state == STATE_ACTIVE);
    
    c8 *contents = 0;

    ptr file = INVALID_HANDLE_VALUE;
    check_winapi_retval_report_goto(
        file,
        file != INVALID_HANDLE_VALUE,
        Failure,
        CreateFileA,
        "\"%s\", %u, %u, %p, %u, %u, %p",
        path, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0
    );

    u32 size;
    check_winapi_retval_report_goto(
        size,
        size != INVALID_FILE_SIZE,
        Failure,
        GetFileSize,
        "%p, %p",
        file, 0
    );
    u64 expected_size = g_core.api.retro_serialize_size();
    check_print_error_goto(size == expected_size, Failure, "wrong save state size (%u, %u expected)", size, expected_size);

    contents = HeapAlloc(GetProcessHeap(), HEAP_GENERATE_EXCEPTIONS, size);
    u32 ok, bytes_read = 0;
    check_winapi_retval_report_goto(
        ok,
        ok && bytes_read == size,
        Failure,
        ReadFile,
        "%p, %p, %u, %p, %p",
        file, contents, size, &bytes_read, 0
    );
    
    check_print_error_goto(g_core.api.retro_unserialize(contents, size), Failure, "retro_unserialize() failed");

    print("loaded game state from \"%s\"", path);

    Failure:
    check_winapi_report(CloseHandle, "%p", file);
    check_winapi_report(HeapFree, "%p, %u, %p", GetProcessHeap(), 0, contents);
    return;
}

void save_game_state(cstr path)
{
    check_return(g_core.state == STATE_ACTIVE);

    c8 *contents = 0;

    ptr file = INVALID_HANDLE_VALUE;
    check_winapi_retval_report_goto(
        file,
        file != INVALID_HANDLE_VALUE,
        Failure,
        CreateFileA,
        "\"%s\", %u, %u, %p, %u, %u, %p",
        path, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0
    );

    u32 size = (u32)g_core.api.retro_serialize_size();;
    contents = HeapAlloc(GetProcessHeap(), HEAP_GENERATE_EXCEPTIONS, size);

    check_print_error_goto(g_core.api.retro_serialize(contents, size), Failure, "retro_serialize() failed");

    u32 ok, bytes_written = 0;
    check_winapi_retval_report_goto(
        ok,
        ok && bytes_written == size,
        Failure,
        WriteFile,
        "%p, %p, %u, %p, %p",
        file, contents, size, &bytes_written, 0
    );

    print("saved game state to \"%s\"", path);

    Failure:
    check_winapi_report(CloseHandle, "%p", file);
    check_winapi_report(HeapFree, "%p, %u, %p", GetProcessHeap(), 0, contents);
    return;
}

state_t get_core_state(void)
{
    return g_core.state;
}

cstr get_core_system_directory(void)
{
    return g_core.paths.system;
}

cstr get_core_save_directory(void)
{
    return g_core.paths.save;
}

void load_core_variables(void)
{
    if (!g_core.vars.array)
    {
        g_core.vars.allocated = 8;
        g_core.vars.array = HeapAlloc(
            GetProcessHeap(),
            HEAP_GENERATE_EXCEPTIONS | HEAP_ZERO_MEMORY,
            sizeof(g_core.vars.array[0]) * g_core.vars.allocated
        );
    }

    c8 *contents = 0;

    ptr file;
    check_winapi_retval_report_goto(
        file,
        file != INVALID_HANDLE_VALUE,
        Failure,
        CreateFileA,
        "\"%s\", %u, %u, %p, %u, %u, %p",
        g_core.paths.settings_file,
        GENERIC_READ,
        FILE_SHARE_READ,
        0,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        0
    );
    if (GetLastError() == ERROR_ALREADY_EXISTS) SetLastError(0);

    u32 size;
    check_winapi_retval_report_goto(size, size != INVALID_FILE_SIZE, Failure, GetFileSize, "%p, %p", file, 0);

    contents = HeapAlloc(GetProcessHeap(), HEAP_GENERATE_EXCEPTIONS, size);
    u32 bytes_read = 0, ok;
    check_winapi_retval_report_goto(
        ok,
        ok && bytes_read == size,
        Failure,
        ReadFile,
        "%p, %p, %u, %p, %p",
        file, contents, size, &bytes_read, 0
    );

    for (u32 i = 0; i < size; )
    {
        c8 key[256];
        c8 value[256];

        while (i < size && (contents[i] == ' ' || contents[i] == '\n')) i++;

        u32 j;
        for (j = 0; j < countof(key) - 1 && i < size && contents[i] != '='; j++, i++)
            key[j] = contents[i];
        key[j] = '\0';
        i++;

        for (j = 0; j < countof(value) - 1 && i < size && contents[i] != '\n'; j++, i++)
            value[j] = contents[i];
        value[j] = '\0';
        i++;

        if (!key[0]) continue;

        update_core_variable(key, value);
    }

    print("loaded core settings from \"%s\"", g_core.paths.settings_file);

    Failure:
    check_winapi_report(CloseHandle, "%p", file);
    check_winapi_report(HeapFree, "%p, %u, %p", GetProcessHeap(), 0, contents);
    return;
}

void save_core_variables(void)
{
    c8 *contents = 0;

    ptr file;
    check_winapi_retval_report_goto(
        file,
        file != INVALID_HANDLE_VALUE,
        Failure,
        CreateFileA,
        "\"%s\", %u, %u, %p, %u, %u, %p",
        g_core.paths.settings_file,
        GENERIC_WRITE,
        0,
        0,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        0
    );
    if (GetLastError() == ERROR_ALREADY_EXISTS) SetLastError(0);

    u32 size = sizeof(g_core.vars.array[0]) * g_core.vars.count + 1;
    contents = HeapAlloc(GetProcessHeap(), HEAP_GENERATE_EXCEPTIONS, size);

    u32 bytes_to_write = 0;
    for (u32 j = 0; j < g_core.vars.count; j++)
    {
        bytes_to_write += snprintf(
            contents + bytes_to_write,
            size - bytes_to_write,
            "%s=%s\n",
            g_core.vars.array[j].key,
            g_core.vars.array[j].value
        );
        assert_report(bytes_to_write <= size);
    }

    u32 bytes_written = 0, ok;
    check_winapi_retval_report_goto(
        ok,
        ok && bytes_written == bytes_to_write,
        Failure,
        WriteFile,
        "%p, %p, %u, %p, %p",
        file, contents, bytes_to_write, &bytes_written, 0
    );

    print("saved core settings to \"%s\"", g_core.paths.settings_file);

    Failure:
    check_winapi_report(CloseHandle, "%p", file);
    check_winapi_report(HeapFree, "%p, %u, %p", GetProcessHeap(), 0, contents);
    return;
}

u32 get_core_variable_idx(cstr key)
{
    for (u32 i = 0; i < g_core.vars.count; i++)
        if (str_equals(key, g_core.vars.array[i].key, sizeof(g_core.vars.array[i].key)))
            return i;
    return INVALID_IDX;
}

cstr get_core_variable(cstr key)
{
    u32 i = get_core_variable_idx(key);
    return (i != INVALID_IDX) ? (g_core.vars.array[i].value) : (0);
}

void update_core_variable(cstr key, cstr value)
{
    u32 var_idx = get_core_variable_idx(key);
    if (var_idx == INVALID_IDX) var_idx = g_core.vars.count;

    if (g_core.vars.count == g_core.vars.allocated)
    {
        g_core.vars.allocated <<= 1;
        g_core.vars.array = HeapReAlloc(
            GetProcessHeap(),
            HEAP_GENERATE_EXCEPTIONS | HEAP_ZERO_MEMORY,
            g_core.vars.array,
            sizeof(g_core.vars.array[0]) * g_core.vars.allocated
        );
    }

    snprintf(g_core.vars.array[var_idx].key, sizeof(g_core.vars.array[var_idx].key), "%s", key);
    snprintf(g_core.vars.array[var_idx].value, sizeof(g_core.vars.array[var_idx].value), "%s", value);

    if (var_idx == g_core.vars.count)
        g_core.vars.count++;
}

void core_log_callback(u32 level, cstr format, ...)
{
    (void)level;
    c8 buffer[1024];

    va_list args;
    va_start(args, format);
    u32 l = vsnprintf(buffer, sizeof(buffer) - 1, format, args);
    for (u32 i = 0; i < l; i++) if (buffer[i] < ' ' || buffer[i] > '~') buffer[i] = ' ';
    buffer[l] = '\0';

    print("%s", buffer);
}

u8 core_environment_callback(u32 cmd, ptr data)
{
    switch (cmd)
    {
    case RETRO_ENVIRONMENT_GET_LOG_INTERFACE:
        ((retro_log_callback*)data)->log = core_log_callback;
        return true;

    case RETRO_ENVIRONMENT_GET_SYSTEM_DIRECTORY:
        *(cstr*)data = g_core.paths.system;
        return true;

    case RETRO_ENVIRONMENT_GET_SAVE_DIRECTORY:
        *(cstr*)data = g_core.paths.save;
        return true;

    case RETRO_ENVIRONMENT_SET_PIXEL_FORMAT:
        return *(retro_pixel_format*)data == RETRO_PIXEL_FORMAT_XRGB8888;

    case RETRO_ENVIRONMENT_GET_PREFERRED_HW_RENDER:
        *(u32*)data = RETRO_HW_CONTEXT_OPENGL_CORE;
        return true;

    case RETRO_ENVIRONMENT_SET_HW_RENDER:
        g_core.hw = data;
        print("core attempts to set hardware renderer to %u", g_core.hw->context_type);
        check_return_value(
            g_core.hw->context_type == RETRO_HW_CONTEXT_OPENGL_CORE
            && g_core.hw->version_major == 3
            && g_core.hw->version_minor == 3,
            false
        );
        g_core.hw->get_proc_address = get_gl_proc_address;
        g_core.hw->get_current_framebuffer = get_gl_framebuffer;
        g_core.hw->context_reset();
        return true;

    case RETRO_ENVIRONMENT_SET_VARIABLES:
        for (retro_variable *v = data; v->key; v++)
        {
            if (get_core_variable(v->key))
                continue;

            c8 value[64] = {0};
            u32 i = 0, j = 0;
            while (v->value[i] && v->value[i] != ';') i++;
            i++;
            while (v->value[i] && v->value[i] == ' ') i++;
            while (v->value[i] && v->value[i] != '|' && j < sizeof(value) - 1) value[j++] = v->value[i++];
            update_core_variable(v->key, value);
        }
        return true;

    case RETRO_ENVIRONMENT_GET_VARIABLE:
        retro_variable *v = data;
        return (v->value = get_core_variable(v->key)) != 0;

    case RETRO_ENVIRONMENT_GET_RUMBLE_INTERFACE:
    case RETRO_ENVIRONMENT_GET_DISK_CONTROL_INTERFACE_VERSION:
    case RETRO_ENVIRONMENT_GET_MESSAGE_INTERFACE_VERSION:
    case RETRO_ENVIRONMENT_SET_DISK_CONTROL_INTERFACE:
    case RETRO_ENVIRONMENT_SET_CONTROLLER_INFO:
    case RETRO_ENVIRONMENT_GET_VFS_INTERFACE:
    case RETRO_ENVIRONMENT_GET_CORE_OPTIONS_VERSION:
    case RETRO_ENVIRONMENT_SET_CORE_OPTIONS_DISPLAY:
    case RETRO_ENVIRONMENT_SET_CORE_OPTIONS_UPDATE_DISPLAY_CALLBACK:
    case RETRO_ENVIRONMENT_SET_INPUT_DESCRIPTORS:
    case RETRO_ENVIRONMENT_GET_VARIABLE_UPDATE:
    case RETRO_ENVIRONMENT_GET_CURRENT_SOFTWARE_FRAMEBUFFER:
        return false;
    }

    print("ignored core command %u (%u, %p)", cmd, cmd & (~RETRO_ENVIRONMENT_EXPERIMENTAL), data);
    return false;
}

void core_video_callback(ptr data, u32 width, u32 height, u64 pitch)
{
    (void)data; (void)width; (void)height; (void)pitch;
}

void core_audio_sample_callback(i16 left, i16 right)
{
    (void)left; (void)right;
}

u64 core_audio_batch_callback(i16 *data, u64 frames)
{
    (void)data; (void)frames;
    return 0;
}

void core_input_poll_callback(void)
{
}

i16 core_input_state_callback(u32 port, u32 device, u32 index, u32 id)
{
    (void)port; (void)device; (void)index; (void)id;
    return 0;
}
