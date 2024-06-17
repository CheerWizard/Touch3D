#include <LinuxApp.hpp>
#include <Log.hpp>

static ulong RGB(u8 r, u8 g, u8 b) {
    return b + (g<<8) + (r<<16);
}

LinuxApp::LinuxApp() {
    LogOpen("T3D_Linux.log");

    m_display = XOpenDisplay(nullptr);
    if (m_display == nullptr) {
        LogAssert(false, "Unable to open X11 display for Linux!");
    }

    m_title = "T3D Linux";
    m_color_black = BlackPixel(m_display, m_screen);
    m_color_white = WhitePixel(m_display, m_screen);
    m_color_red = RGB(255, 0, 0);
    m_color_blue = RGB(0, 0, 255);

    m_screen = DefaultScreen(m_display);

    m_window = XCreateSimpleWindow(
            m_display,
            DefaultRootWindow(m_display),
            400, 300,
            800, 600,
            5,
            m_color_black,
            m_color_white
    );

    XSetStandardProperties(
            m_display,
            m_window,
            m_title,
            m_title,
            None,
            nullptr,
            0,
            nullptr
    );

    XSelectInput(
            m_display,
            m_window,
            ExposureMask |
            ButtonPressMask |
            KeyPressMask
    );

    m_gc = XCreateGC(m_display, m_window, 0, 0);

    XSetBackground(m_display, m_gc, m_color_white);
    XSetForeground(m_display, m_gc, m_color_black);
    XClearWindow(m_display, m_window);
    XMapRaised(m_display, m_window);
}

LinuxApp::~LinuxApp() {
    XFreeGC(m_display, m_gc);
    XDestroyWindow(m_display, m_window);
    XCloseDisplay(m_display);
    LogClose();
}

void LinuxApp::Run() {
    m_running = true;
    while (m_running) {
        UpdateWindow();
        UpdateEvents();
    }
}

void LinuxApp::UpdateWindow() {
    LogInfo("UpdateWindow()");
}

void LinuxApp::UpdateEvents() {
    XEvent event;
    m_running = XNextEvent(m_display, &event) == 0;
    if (event.type == Expose && event.xexpose.count == 0) {
        XClearWindow(m_display, m_window);
    }
}