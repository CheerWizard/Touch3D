#include <sf_app.hpp>
#include <sf_log.hpp>

namespace sf {

    App::App() {
        SF_LOG_OPEN("App.log");
        m_window = new Window("SF App", 400, 300, 800, 600, true);
        m_window->events.event_on_window_resize = { this, sf::on_window_resize<App>, nullptr };
    }

    App::~App() {
        SF_LOG_CLOSE();
        delete m_window;
    }

    void App::run() {
        assert_init();
        m_running = true;
        while (m_running) {
            float begin_time_ms = get_current_time_millis();

            m_running = m_window->update();

            float end_time_ms = get_current_time_millis();
            m_delta_time = end_time_ms - begin_time_ms;
        }
    }

    void App::assert_init() {
        SF_ASSERT(m_window != nullptr, "Window was not created!");
    }

    void App::on_window_resize(int w, int h) {
    }

    void App::on_window_move(int x, int y) {
    }

    void App::on_cursor_move(double x, double y) {
    }

    void App::on_touch_move(double x, double y) {
    }

}
