#include <WindowsApp.hpp>
#include <Log.hpp>

static LRESULT CALLBACK WindowProc(HWND handle, UINT msg, WPARAM w_param, LPARAM l_param) {
    EventBuffer* event_buffer;

    // only called when window is created first time
    if (msg == WM_CREATE) {
        CREATESTRUCT *pCreate = reinterpret_cast<CREATESTRUCT*>(l_param);
        event_buffer = reinterpret_cast<EventBuffer*>(pCreate->lpCreateParams);
        SetWindowLongPtr(handle, GWLP_USERDATA, (LONG_PTR) event_buffer);
    }
    else {
        LONG_PTR ptr = GetWindowLongPtr(handle, GWLP_USERDATA);
        event_buffer = reinterpret_cast<EventBuffer*>(ptr);
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
            if (event_buffer != nullptr) {
                for (OnWindowResized on_window_resized : event_buffer->on_window_resized) {
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

WindowsApp::WindowsApp(HINSTANCE instance, int cmd_show) {
    WNDCLASS window_class = {};
    window_class.hInstance = instance;
    window_class.lpszClassName = "T3D_Window";
    window_class.lpfnWndProc = WindowProc;
    RegisterClass(&window_class);

    m_window.title = "T3D Windows";
    m_window.position = { 400, 300 };
    m_window.size = { 800, 600 };

    m_window.handle = CreateWindowEx(
            0,
            window_class.lpszClassName,
            m_window.title,
            WS_OVERLAPPEDWINDOW,
            m_window.position.x,
            m_window.position.y,
            m_window.size.x,
            m_window.size.y,
            nullptr,
            nullptr,
            instance,
            &m_event_buffer
    );

    if (m_window.handle == nullptr) {
        LogAssert(false, "Failed to create window for Windows!");
    }

    ShowWindow(m_window.handle, cmd_show);

    DEVMODE devmode;
    EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &devmode);
    m_window.refresh_rate = devmode.dmDisplayFrequency;
}

WindowsApp::~WindowsApp() {
    DestroyWindow(m_window.handle);
}

void WindowsApp::Run() {
    m_running = true;
    while (m_running) {
        UpdateWindow();
        UpdateEvents();
    }
}

void WindowsApp::UpdateWindow() {
    LogInfo("UpdateWindow()");
    MSG msg = {};
    if (GetMessage(&msg, nullptr, 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

void WindowsApp::UpdateEvents() {
    m_running = IsWindow(m_window.handle);
}