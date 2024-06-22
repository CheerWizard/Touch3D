#include <sf_platform.hpp>

namespace sf {



}

// SF_WINDOWS_BEGIN
#if defined(SF_WINDOWS)

#include <windows.h>

namespace sf {

    #define SF_WINDOW (HWND) m_handle

    static LRESULT CALLBACK WindowProc(HWND handle, UINT msg, WPARAM w_param, LPARAM l_param) {
        Events* events;

        // only called when window is created first time
        if (msg == WM_CREATE) {
            CREATESTRUCT *pCreate = reinterpret_cast<CREATESTRUCT*>(l_param);
            events = reinterpret_cast<Events*>(pCreate->lpCreateParams);
            SetWindowLongPtr(handle, GWLP_USERDATA, (LONG_PTR) events);
        }
        else {
            LONG_PTR ptr = GetWindowLongPtr(handle, GWLP_USERDATA);
            events = reinterpret_cast<Events*>(ptr);
        }

        SF_ASSERT(events != nullptr, "Events should not be null here!");

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
                int w = LOWORD(l_param);
                int h = HIWORD(l_param);
                if (events->window_resize) {
                    events->window_resize(w, h);
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

    Window::Window(const char *title, int x, int y, int w, int h, bool sync) {
        HINSTANCE instance = (HINSTANCE) GetCurrentProcess();

        WNDCLASS window_class = {};
        window_class.hInstance = instance;
        window_class.lpszClassName = "Windows_Window";
        window_class.lpfnWndProc = WindowProc;
        RegisterClass(&window_class);

        m_handle = (void*) CreateWindowEx(
                0,
                window_class.lpszClassName,
                title,
                WS_OVERLAPPEDWINDOW,
                x, y,
                w, h,
                nullptr,
                nullptr,
                instance,
                &events
        );

        if (m_handle == nullptr) {
            SF_ASSERT(false, "Failed to create window for Windows!");
        }

        ShowWindow(SF_WINDOW, 0);

        DEVMODE devmode;
        EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &devmode);
        m_refresh_rate = devmode.dmDisplayFrequency;
    }

    Window::~Window() {
        DestroyWindow(SF_WINDOW);
    }

    bool Window::update() {
        MSG msg = {};
        if (GetMessage(&msg, nullptr, 0, 0) > 0) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        return IsWindow(SF_WINDOW);
    }

}

#endif
// SF_WINDOWS_END

// SF_LINUX_BEGIN
#if defined(SF_LINUX)

#include <X11/Xlib.h>
#include <X11/Xos.h>
#include <X11/Xutil.h>

namespace sf {

    #define SF_WINDOW (XID) m_handle

    static Display* s_display = nullptr;
    static int s_screen;
    static GC s_context;

    Window::Window(const char *title, int x, int y, int w, int h, bool sync) {
        s_display = XOpenDisplay(nullptr);
        if (s_display == nullptr) {
            SF_ASSERT(false, "Unable to open X11 display for Linux!");
        }

        ulong black = BlackPixel(s_display, s_screen);
        ulong white = WhitePixel(s_display, s_screen);
        ulong red = SF_RGB(255, 0, 0);
        ulong blue = SF_RGB(0, 0, 255);

        s_screen = DefaultScreen(s_display);

        m_handle = (void*) XCreateSimpleWindow(
                s_display,
                DefaultRootWindow(s_display),
                400, 300,
                800, 600,
                5,
                black,
                white
        );

        XSetStandardProperties(
                s_display,
                SF_WINDOW,
                title,
                title,
                None,
                nullptr,
                0,
                nullptr
        );

        XSelectInput(
                s_display,
                SF_WINDOW,
                ExposureMask |
                ButtonPressMask |
                KeyPressMask
        );

        s_context = XCreateGC(s_display, SF_WINDOW, 0, 0);

        XSetBackground(s_display, s_context, white);
        XSetForeground(s_display, s_context, black);
        XClearWindow(s_display, SF_WINDOW);
        XMapRaised(s_display, SF_WINDOW);
    }

    Window::~Window() {
        XFreeGC(s_display, s_context);
        XDestroyWindow(s_display, SF_WINDOW);
        XCloseDisplay(s_display);
    }

    bool Window::update() {
        XEvent event;
        bool updated = XNextEvent(s_display, &event) == 0;
        if (event.type == Expose && event.xexpose.count == 0) {
            XClearWindow(s_display, SF_WINDOW);
        }
        return updated;
    }

}

#endif
// SF_LINUX_END

// SF_ANDROID_BEGIN
#if defined(SF_ANDROID)

namespace sf {



}

#endif // SF_ANDROID_END