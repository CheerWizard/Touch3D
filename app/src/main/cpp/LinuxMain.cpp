#include <LinuxApp.hpp>

static Display* display;
static int screen;
static Window window_root;

int main() {
    LinuxApp* app = new LinuxApp();
    app->Run();
    delete app;
    return 0;
}