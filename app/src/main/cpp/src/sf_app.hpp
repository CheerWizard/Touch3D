#pragma once

#include <sf_platform.hpp>

namespace sf {

    class SF_API App {

    public:
        App();
        ~App();

        void run();

    private:
        void assert_init();

    protected:
        bool m_running = false;
        Window* m_window = nullptr;
        float m_delta_time = 6;
    };

}