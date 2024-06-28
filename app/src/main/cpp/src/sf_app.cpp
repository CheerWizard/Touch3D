#include <sf_app.hpp>
#include <sf_log.hpp>

namespace sf {

    app_t s_app = {};

    void app_init() {
        g_stl_memory_pool = memory_pool_init(sf::malloc(1_MB), 1_MB, 100);
        SF_LOG_OPEN("App.log");
        s_app.window = window_init("SF App", 400, 300, 800, 600, true);
        s_app.window.desktop_events.event_on_window_resize = app_on_window_resize;
        s_app.window.desktop_events.event_on_key = app_on_key;
    }

    void app_free() {
        window_free(s_app.window);
        SF_LOG_CLOSE();
        memory_pool_free(g_stl_memory_pool);
        sf::free(g_stl_memory_pool.memory);
    }

    void app_run() {
        s_app.running = true;
        while (s_app.running) {
            float begin_time_ms = time_get_current_ms();

            s_app.running = window_update(s_app.window);

            float end_time_ms = time_get_current_ms();
            s_app.delta_time = end_time_ms - begin_time_ms;
        }
    }

    void app_on_window_resize(int w, int h) {
    }

    void app_on_window_move(int x, int y) {
    }

    void app_on_key(SF_KEYCODE keycode, bool pressed) {
        if (pressed) {
            switch (keycode) {
                case SF_KEYCODE_ESC: {
                    s_app.running = false;
                }
            }
        }
    }

    void app_on_cursor_move(double x, double y) {
    }

    void app_on_touch_move(double x, double y) {
    }

}
