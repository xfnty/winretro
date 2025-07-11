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
    init_core("C:\\Dev\\personal\\cpp\\winretro\\.ignore\\swanstation_libretro.dll");
    load_game("C:\\Dev\\personal\\cpp\\winretro\\.ignore\\game.chd");
    ui_display_core_state(get_core_state());

    f64 last_frame = 0;
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
                case UI_INPUT:
                    {
                        core_input_t inp;
                        if (map_ui_key_to_core_input(e.value.input.key, &inp))
                            set_core_input(inp, ((e.value.input.key & UI_KEY_STATE_MASK) != 0));
                    }
                    break;
            }
            ui_display_core_state(get_core_state());
        }

        //                                      FIXME: looks like get_time() is incorrect
        if (get_core_state() == STATE_ACTIVE && (get_time() - last_frame >= 0.25f / get_target_fps()))
        {
            f64 frame_start = get_time();
            run_frame();
            f64 frame_end = get_time();
            last_frame = frame_end;

            f64 draw_start = get_time();
            present_frame();
            f64 draw_end = get_time();

            c8 text[64];
            snprintf(
                text,
                sizeof(text),
                "Update: %d ms, Draw: %d ms",
                (i32)((frame_end - frame_start) * 1000),
                (i32)((draw_end - draw_start) * 1000)
            );
            set_ui_status(text);
        }
    }

    free_core();
    free_gl();
    free_ui();
    free_log();
    ExitProcess(0);
}
