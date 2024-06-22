#pragma once

#include <Thread.hpp>
#include <EventBuffer.hpp>

#include <Windows.h>

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
    HWND m_window;
    const char* m_title;
};