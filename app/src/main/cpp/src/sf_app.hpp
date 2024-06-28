#pragma once

#include <sf_platform.hpp>

namespace sf {

    struct SF_API app_t final {
        bool running = false;
        float delta_time = 6;
        window_t window;
    };

    SF_API void app_init();
    SF_API void app_free();
    SF_API void app_run();

    SF_API void app_on_window_resize(int w, int h);
    SF_API void app_on_window_move(int x, int y);
    SF_API void app_on_key(SF_KEYCODE keycode, bool pressed);
    SF_API void app_on_cursor_move(double x, double y);
    SF_API void app_on_touch_move(double x, double y);

}