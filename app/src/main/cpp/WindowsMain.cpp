#include <Application.hpp>
#include <Windows.h>

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR cmd_line, int cmd_show) {
    Application* application = new Application(instance, cmd_show);
    application->RunLoop();
    delete application;
    return 0;
}

Application::Application(HINSTANCE instance, int cmd_show) {
    m_window = new Window(instance, cmd_show, m_io_buffer, "Touch3D", { 400, 300 }, { 800, 600 });
}

Application::~Application() {
    delete m_window;
}

void Application::RunLoop() {
    m_state = T3D_APP_STARTED;
    while (m_state != T3D_APP_DESTROYED) {
        m_window->Update();

        if (!m_window->IsOpen()) {
            m_state = T3D_APP_DESTROYED;
            break;
        }
    }
}