#include <sf_app.hpp>
#include <sf_log.hpp>

namespace sf {

    app_t global_app;

    void app_init() {
        memory_init();
        SF_LOG_OPEN("App.log");
        global_app.window = window_init("SF App", 400, 300, 800, 600, true);
        global_app.window.desktop_events.event_on_window_resize = app_on_window_resize;
        global_app.window.desktop_events.event_on_key = app_on_key;
    }

    void app_free() {
        window_free(global_app.window);
        SF_LOG_CLOSE();
        memory_free();
    }

    void app_run() {
        global_app.running = true;
        while (global_app.running) {
            float begin_time_ms = time_get_current_ms();

            global_app.running = window_update(global_app.window);

            float end_time_ms = time_get_current_ms();
            global_app.delta_time = end_time_ms - begin_time_ms;
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
                    global_app.running = false;
                }
            }
        }
    }

    void app_on_cursor_move(double x, double y) {
    }

    void app_on_touch_move(double x, double y) {
    }

}
