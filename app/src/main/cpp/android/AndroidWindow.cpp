#include <PlatformDefs.hpp>

#ifdef T3D_ANDROID

#include <Window.hpp>

Window::Window(IOBuffer& io_buffer) : m_io_buffer(io_buffer) {

}

Window::~Window() {

}

void Window::Update() {

}

bool Window::IsOpen() const {
    return true;
}

#endif // T3D_ANDROID