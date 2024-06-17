#include <LinuxApp.hpp>
#include <Log.hpp>

LinuxApp::LinuxApp() {
    m_window.display = XOpenDisplay(nullptr);
    if (m_window.display == nullptr) {
        LogAssert(false, "Unable to open X11 display for Linux!");
    }

    m_window.title = "T3D Linux";
    m_window.position = { 400, 300 };
    m_window.size = { 800, 600 };
    m_window.border_width = 15;
    m_window.border_color = BlackPixel(m_window.display, m_window.screen);
    m_window.background_color = WhitePixel(m_window.display, m_window.screen);

    m_window.screen = DefaultScreen(m_window.display);
    m_window.root = RootWindow(m_window.display, m_window.screen);

    m_window.handle = XCreateSimpleWindow(
            m_window.display, m_window.root,
            m_window.position.x, m_window.position.y,
            m_window.size.x, m_window.size.y,
            m_window.border_width,
            m_window.border_color,
            m_window.background_color
    );

    XMapWindow(m_window.display, m_window.handle);
}

LinuxApp::~LinuxApp() {
    XUnmapWindow(m_window.display, m_window.handle);
    XDestroyWindow(m_window.display, m_window.handle);
    XCloseDisplay(m_window.display);
}

void LinuxApp::Run() {
    m_running = true;
    while (m_running) {
        UpdateWindow();
        UpdateEvents();
    }
}

void LinuxApp::UpdateWindow() {

}

void LinuxApp::UpdateEvents() {
    XEvent event;
    m_running = XNextEvent(m_window.display, &event) == 0;
}