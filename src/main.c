#include "ui.h"
#include "log.h"
#include "core.h"
#include "miniwindows.h"

void main(void)
{
    InitLog();
    InitUi((UiParams){ .title = "Libretro Frontend" });

    for (u8 running = true; running; )
    {
        Ui_ProcessEvents();

        for (UiEvent e; Ui_GetEvent(&e); )
        {
            switch (e.type)
            {
                case UI_EXIT:
                    running = false;
                    break;

                case UI_LOAD_STATE:
                    break;

                case UI_SAVE_STATE:
                    break;

                case UI_OPEN_ROM:
                    if (Core_SetRom(e.value.path))
                    {
                        Ui_SetRomLoaded(e.value.path);
                    }
                    break;

                case UI_OPEN_CORE:
                    if (Core_Load(e.value.path))
                    {
                        Ui_SetCoreLoaded(Core_GetName());
                        Ui_SetRomLoaded(0);
                    }
                    break;

                case UI_INPUT:
                    Core_SetInput(e.value.input);
                    break;
            }
        }
    }

    FreeUi();
    ExitProcess(0);
}
