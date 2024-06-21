#pragma once

#include <sf.hpp>

#include <X11/Xlib.h>
#include <X11/Xos.h>
#include <X11/Xutil.h>

using namespace sf;

class LinuxApp final {

public:
    LinuxApp();
    ~LinuxApp();

    void Run();

private:
    void UpdateWindow();
    void UpdateEvents();

protected:
    bool m_running;
    ThreadPool<1, 10, SF_THREAD_PRIORITY_HIGHEST>* m_thread_pool;

    const char* m_title;
    ulong m_color_black;
    ulong m_color_white;
    ulong m_color_red;
    ulong m_color_blue;
    u32 m_refresh_rate;
    Display* m_display;
    int m_screen;
    Window m_window;
    GC m_gc;
};