#include <sf_main.hpp>

using namespace sf;

class LinuxApp : public App {};

App* new_app() {
    return new LinuxApp();
}