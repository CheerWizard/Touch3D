#include <LinuxApp.hpp>

int main() {
    LinuxApp* app = new LinuxApp();
    app->Run();
    delete app;
    return 0;
}