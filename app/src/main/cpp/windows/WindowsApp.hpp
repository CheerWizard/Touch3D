#pragma once

#include <ThreadPool.hpp>
#include <EventBuffer.hpp>

#include <Windows.h>

struct WindowsWindow final {
    const char* title;
    int2 position;
    int2 size;
    u32 refresh_rate;
    HWND handle;
};

class WindowsApp final {

public:
    WindowsApp(HINSTANCE instance, int cmd_show);
    ~WindowsApp();

    void Run();

private:
    void UpdateWindow();
    void UpdateEvents();

protected:
    bool m_running;
    ThreadPool* m_thread_pool;
    EventBuffer m_event_buffer;
    WindowsWindow m_window;

};