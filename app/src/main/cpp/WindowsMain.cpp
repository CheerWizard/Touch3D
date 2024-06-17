#include <WindowsApp.hpp>

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR cmd_line, int cmd_show) {
    WindowsApp* app = new WindowsApp(instance, cmd_show);
    app->Run();
    delete app;
    return 0;
}