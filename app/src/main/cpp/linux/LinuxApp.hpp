#pragma once

#include <ThreadPool.hpp>
#include <EventBuffer.hpp>

#include <X11/Xlib.h>
#include <err.h>

struct LinuxWindow final {
    const char* title;
    int2 position;
    int2 size;
    int border_width;
    ulong border_color;
    ulong background_color;
    u32 refresh_rate;
    Display* display;
    int screen;
    Window root;
    Window handle;
};

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
    IOBuffer m_io_buffer;
    ThreadPool* m_thread_pool;
    LinuxWindow m_window;
};