#include <sf_app.hpp>
#include <sf_log.hpp>

namespace sf {

    App::App() {
        SF_LOG_OPEN("App.log");
    }

    App::~App() {
        SF_LOG_CLOSE();
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

}