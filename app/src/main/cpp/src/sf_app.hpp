#pragma once

#include <sf_platform.hpp>

namespace sf {

    class SF_API App {

    public:
        App();
        virtual ~App();

        void run();

        void on_window_resize(int w, int h);
        void on_window_move(int x, int y);
        void on_cursor_move(double x, double y);
        void on_touch_move(double x, double y);

    private:
        void assert_init();

    protected:
        bool m_running = false;
        Window* m_window = nullptr;
        float m_delta_time = 6;
    };

}