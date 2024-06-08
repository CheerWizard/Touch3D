#include <PlatformDefs.hpp>

#ifdef T3D_WINDOWS

#include <Application.hpp>

#include <Windows.h>

static HINSTANCE s_instance;
static int s_cmd_show;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR cmd_line, int cmd_show) {
    Application* application = new Application();
    application->Run();
    delete application;
    return 0;
}

Application::Application() {
    m_window = new Window(s_instance, s_cmd_show, m_io_buffer, "Touch3D", 400, 300, 800, 600);
}

Application::~Application() {
    delete m_window;
}

void Application::Run() {
    m_state = T3D_APP_STARTED;
    while (m_state != T3D_APP_DESTROYED) {
        m_window->Update();

        if (!m_window->IsOpen()) {
            m_state = T3D_APP_DESTROYED;
            break;
        }
    }
}

#endif // T3D_WINDOWS