#include "ui.h"
#include "log.h"
#include "core.h"
#include "common.h"
#include "windows.h"

void _start(void)
{
    init_log();
    init_ui();
    init_core("C:\\Dev\\personal\\cpp\\winretro\\.ignore\\swanstation_libretro.dll");
    load_game("C:\\Dev\\personal\\cpp\\winretro\\.ignore\\game.chd");
    load_game_state("C:\\Dev\\personal\\cpp\\winretro\\.ignore\\save.bin");
    save_game_state("C:\\Dev\\personal\\cpp\\winretro\\.ignore\\save.bin");

    for (u8 running = true; running; )
    {
        poll_ui_events();

        for (ui_event_t e; get_ui_event(&e); )
        {
            switch (e.type)
            {
                case UI_EXIT:       running = false; break;
                case UI_OPEN_CORE:  init_core(e.value.path); break;
                case UI_OPEN_ROM:   load_game(e.value.path); break;
                case UI_LOAD_STATE: load_game_state(e.value.path); break;
                case UI_SAVE_STATE: save_game_state(e.value.path); break;
            }
        }

        ui_display_core_state(get_core_state());
    }
    
    free_core();
    free_ui();
    free_log();
    ExitProcess(0);
}
