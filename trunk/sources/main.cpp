#include <sdkddkver.h>
#include <windows.h>

#include <ui/Window.hpp>

int __stdcall WinMain(
    HINSTANCE hinst,
    HINSTANCE hinstUsused,
    LPSTR     pCommandLine,
    int       eShow
    )
{
    HeapSetInformation(NULL, HeapEnableTerminationOnCorruption, NULL, 0);

    ui::Idler  idler;
    ui::Window* pWindow = ui::Window::instance();
    pWindow->doLoop(idler);
    pWindow->free();

    return 0;
}