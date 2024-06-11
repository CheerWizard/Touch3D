#include <Window.hpp>

Window::Window(IOBuffer& io_buffer) : m_io_buffer(io_buffer) {
    m_io_buffer.on_touch_moved.reserve(1);
}

Window::~Window() {
}

void Window::Update() {
}

bool Window::IsOpen() const {
    return true;
}