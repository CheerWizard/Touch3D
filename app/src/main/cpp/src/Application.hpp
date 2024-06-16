#pragma once

#include <Window.hpp>
#include <ThreadPool.hpp>

class Application final {

public:
    Application();
    ~Application();

    void RunLoop();

protected:
    Window* m_window;
    bool m_running;

};