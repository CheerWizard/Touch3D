#include <Window.hpp>
#include <Log.hpp>

static LRESULT CALLBACK WindowProc(HWND handle, UINT msg, WPARAM w_param, LPARAM l_param) {
    IOBuffer* io_buffer;

    // only called when window is created first time
    if (msg == WM_CREATE) {
        CREATESTRUCT *pCreate = reinterpret_cast<CREATESTRUCT*>(l_param);
        io_buffer = reinterpret_cast<IOBuffer*>(pCreate->lpCreateParams);
        SetWindowLongPtr(handle, GWLP_USERDATA, (LONG_PTR) io_buffer);
    }
    else {
        LONG_PTR ptr = GetWindowLongPtr(handle, GWLP_USERDATA);
        io_buffer = reinterpret_cast<IOBuffer*>(ptr);
    }

    switch (msg) {
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(handle, &ps);
            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW + 1));
            EndPaint(handle, &ps);
            return 0;
        }

        case WM_SIZE:
        {
            int2 size = { LOWORD(l_param), HIWORD(l_param) };
            if (io_buffer != nullptr) {
                for (OnWindowResized on_window_resized : io_buffer->on_window_resized) {
                    on_window_resized(size);
                }
            }
            break;
        }

        case WM_DESTROY:
        {
            PostQuitMessage(0);
            break;
        }
    }

    return DefWindowProc(handle, msg, w_param, l_param);
}

Window::Window(HINSTANCE instance, int cmd_show, const char* title, int2 position, int2 size) {
    WNDCLASS window_class = {};
    window_class.hInstance = instance;
    window_class.lpszClassName = "Win32_Window";
    window_class.lpfnWndProc = WindowProc;
    RegisterClass(&window_class);

    m_title = title;
    m_position = position;
    m_size = size;

    m_window = CreateWindowEx(
            0,
            window_class.lpszClassName,
            title,
            WS_OVERLAPPEDWINDOW,
            position.x,
            position.y,
            size.x,
            size.y,
            nullptr,
            nullptr,
            instance,
            nullptr
    );

    if (m_window == nullptr) {
        LogError("Failed to create Win32 window!");
    }

    ShowWindow(m_window, cmd_show);

    DEVMODE devmode;
    EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &devmode);
    m_refresh_rate = devmode.dmDisplayFrequency;
}

Window::~Window() {
    DestroyWindow(m_window);
    m_window = nullptr;
}

void Window::SetIOBuffer(IOBuffer* io_buffer) {
    SetWindowLongPtr(m_handle, GWLP_USERDATA, (LONG_PTR) io_buffer);
}

IOBuffer* Window::GetIOBuffer() {
    return reinterpret_cast<IOBuffer*>(GetWindowLongPtr(handle, GWLP_USERDATA));
}

void Window::Update() {
    MSG msg = {};
    if (GetMessage(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

bool Window::IsOpen() const {
    return IsWindow(m_window);
}

bool Window::IsWindowed() {
    return true;
}

bool Window::IsFullscreen() {
    return false;
}