#include "ui.h"
#include "common.h"

void _start(void)
{
    init_ui();
    init_gl(g_ui.render_window, (gl_params_t){ .version_major = 4, .version_minor = 0, .debug = true });

    while (!g_ui.was_exit_requested)
    {
        process_ui_events();
    }

    free_gl();
    free_ui();
    ExitProcess(0);
}
