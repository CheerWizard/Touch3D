#include <sf_main.hpp>

using namespace sf;

class LinuxApp : public App {

public:

    LinuxApp() : App() {
        m_window = new Window("Linux", 400, 300, 800, 600, true);
    }

    ~LinuxApp() {
        delete m_window;
    }

};

App* new_app() {
    return new LinuxApp();
}