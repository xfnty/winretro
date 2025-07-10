#include "ui.h"
#include "gl.h"
#include "log.h"
#include "core.h"
#include "error.h"
#include "common.h"
#include "windows.h"

void _start(void)
{
    init_log();

    init_ui();
    init_gl(get_ui_device_context());
    // init_core("C:\\Dev\\personal\\cpp\\winretro\\.ignore\\swanstation_libretro.dll");
    // load_game("C:\\Dev\\personal\\cpp\\winretro\\.ignore\\game.chd");
    // ui_display_core_state(get_core_state());
    configure_gl(1024, 512);

    for (u8 running = true; running; )
    {
        poll_ui_events();

        for (ui_event_t e; get_ui_event(&e); )
        {
            switch (e.type)
            {
                case UI_EXIT:       running = false; break;
                case UI_RESIZE:     set_gl_window_size(e.value.size.x, e.value.size.y); break;
                case UI_OPEN_CORE:  init_core(e.value.path); save_core_variables(); break;
                case UI_OPEN_ROM:   load_game(e.value.path); break;
                case UI_LOAD_STATE: load_game_state(e.value.path); break;
                case UI_SAVE_STATE: save_game_state(e.value.path); break;
            }
            ui_display_core_state(get_core_state());
        }

        // if (get_core_state() == STATE_ACTIVE)
        // {
        //     f64 frame_start = get_time();
        //     run_frame();
        //     f64 frame_end = get_time();

        //     f64 draw_start = get_time();
            present_frame();
        //     f64 draw_end = get_time();

        //     c8 text[64];
        //     snprintf(
        //         text,
        //         sizeof(text),
        //         "Update: %d ms, Draw: %d ms",
        //         (i32)((frame_end - frame_start) * 1000),
        //         (i32)((draw_end - draw_start) * 1000)
        //     );
        //     set_ui_status(text);
        // }
    }

    free_core();
    free_gl();
    free_ui();
    free_log();
    ExitProcess(0);
}
