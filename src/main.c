#include "ui.h"
#include "core.h"
#include "logger.h"
#include "miniwindows.h"

void main(void)
{
    Logger *logger = CreateLogger((LoggerParams){ .name = "main" });
    Core *core = 0;
    Ui *ui = CreateUi((UiParams){ .title = "Libretro Frontend" });

    for (u8 running = true; running; )
    {
        Ui_ProcessEvents(ui);

        for (UiEvent e; Ui_GetEvent(ui, &e); )
        {
            switch (e.type)
            {
                case UI_EXIT:
                    running = false;
                    break;

                case UI_LOAD_STATE:
                    LogInfo(logger, "load state \"%s\"", e.value.path);
                    break;

                case UI_SAVE_STATE:
                    LogInfo(logger, "save state \"%s\"", e.value.path);
                    break;

                case UI_OPEN_ROM:
                    LogInfo(logger, "open rom \"%s\"", e.value.path);
                    if (core && Core_SetRom(core, e.value.path))
                    {
                        Ui_SetRomLoaded(ui, e.value.path);
                    }
                    break;

                case UI_OPEN_CORE:
                    LogInfo(logger, "open core \"%s\"", e.value.path);
                    Core *new_core = CreateCore(e.value.path);
                    if (new_core)
                    {
                        FreeCore(&core);
                        core = new_core;
                        Ui_SetCoreLoaded(ui, core->name);
                        Ui_SetRomLoaded(ui, 0);
                    }
                    break;

                case UI_INPUT:
                    if (core) Core_SetInput(core, e.value.input);
                    break;
            }
        }
    }

    FreeUi(&ui);
    FreeCore(&core);
    FreeLogger(&logger);
    ExitProcess(0);
}
