#include "core.h"

#include <windows.h>

#include "common.h"

typedef struct retro_system_info retro_system_info;
struct retro_system_info
{
   const char *library_name;
   const char *library_version;
   const char *valid_extensions;
   bool        need_fullpath;
   bool        block_extract;
};

typedef struct Core Core;
struct Core {
    HMODULE dll;
    struct {
        unsigned (*get_version)(void);
        void     (*get_system_info)(retro_system_info*);
    } api;
};

static Core core;

bool Core_Load(const char *path)
{
    Core_Free();

    core.dll = LoadLibraryA(path);
    if (!core.dll) return false;

    struct {
        const char *symbol;
        void **func;
    } load_list[] = {
        { "retro_api_version", (void*)&core.api.get_version },
        { "retro_get_system_info", (void*)&core.api.get_system_info },
    };
    for (int i = 0; i < countof(load_list); i++)
    {
        *load_list[i].func = (void*)GetProcAddress(core.dll, load_list[i].symbol);
        if (!(*load_list[i].func))
        {
            error("symbol \"%s\" not found", load_list[i].symbol);
            return false;
        }
    }

    retro_system_info inf;
    core.api.get_system_info(&inf);
    info("loaded %s %s", inf.library_name, inf.library_version);
    return true;
}

void Core_Free(void)
{
    FreeLibrary(core.dll);
    core = (Core){0};
}
