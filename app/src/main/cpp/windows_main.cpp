#include <sf_main.hpp>

using namespace sf;

class WindowsApp : public App {

public:

    WindowsApp() : App() {
        m_window = new Window("Windows", 400, 300, 800, 600, true);
    }

    ~WindowsApp() {
        delete m_window;
    }

};

App* new_app() {
    return new WindowsApp();
}