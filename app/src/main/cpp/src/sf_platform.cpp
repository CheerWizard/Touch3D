#include <sf_platform.hpp>

namespace sf {



}

// SF_WINDOWS_BEGIN
#if defined(SF_WINDOWS)

#include <windows.h>

namespace sf {

    #define SF_WINDOW_WIN (HWND) window.handle

    static LRESULT CALLBACK WindowProc(HWND handle, UINT msg, WPARAM w_param, LPARAM l_param) {
        window_t::desktop_events_t* events;

        // only called when window is created first time
        if (msg == WM_CREATE) {
            CREATESTRUCT *pCreate = reinterpret_cast<CREATESTRUCT*>(l_param);
            events = reinterpret_cast<window_t::desktop_events_t*>(pCreate->lpCreateParams);
            SetWindowLongPtr(handle, GWLP_USERDATA, (LONG_PTR) events);
        }
        else {
            LONG_PTR ptr = GetWindowLongPtr(handle, GWLP_USERDATA);
            events = reinterpret_cast<window_t::desktop_events_t*>(ptr);
        }

        SF_ASSERT(events != nullptr, "DesktopEvents should not be null here!");

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
                events->event_on_window_resize(w, h);
                break;
            }

            case WM_MOUSEMOVE:
            {

                break;
            }

            case WM_LBUTTONDOWN:
            {

                break;
            }

            case WM_LBUTTONUP:
            {
                break;
            }

            case WM_LBUTTONDBLCLK:
            {

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

    window_t window_init(const char *title, int x, int y, int w, int h, bool sync) {
        window_t window;

        HINSTANCE instance = (HINSTANCE) GetCurrentProcess();

        WNDCLASS window_class = {};
        window_class.hInstance = instance;
        window_class.lpszClassName = "Windows_Window";
        window_class.lpfnWndProc = WindowProc;
        RegisterClass(&window_class);

        window.handle = (void*) CreateWindowEx(
                0,
                window_class.lpszClassName,
                title,
                WS_OVERLAPPEDWINDOW,
                x, y,
                w, h,
                nullptr,
                nullptr,
                instance,
                &window.desktop_events
        );

        if (window.handle == nullptr) {
            SF_ASSERT(false, "Failed to create window for Windows!");
        }

        ShowWindow(SF_WINDOW_WIN, SW_SHOWDEFAULT);

        DEVMODE devmode;
        EnumDisplaySettings(nullptr, ENUM_CURRENT_SETTINGS, &devmode);
        window.refresh_rate = devmode.dmDisplayFrequency;

        return window;
    }

    void window_free(const window_t& window) {
        DestroyWindow(SF_WINDOW_WIN);
    }

    bool window_update(window_t& window) {
        MSG msg = {};
        if (GetMessage(&msg, nullptr, 0, 0) > 0) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        return IsWindow(SF_WINDOW_WIN);
    }

    bool key_is_pressed(SF_KEYCODE keycode) {
        return GetAsyncKeyState(keycode) & 0b1;
    }

    bool mouse_is_pressed(SF_MOUSE_CODE mouse_code) {
        return GetAsyncKeyState(mouse_code) & 0b1;
    }

    bool gamepad_is_pressed(SF_GAMEPAD_CODE gamepad_code) {
        return false;
    }

    cursor_t cursor_get() {
        POINT p;
        GetCursorPos(&p);
        return { p.x, p.y };
    }

}

#endif
// SF_WINDOWS_END

// SF_LINUX_BEGIN
#if defined(SF_LINUX)

#include <X11/Xlib.h>
#include <X11/Xos.h>
#include <X11/Xutil.h>
#include <X11/extensions/Xrandr.h>

namespace sf {

    #define SF_WINDOW_X11 (XID) window.handle

    static Display* s_display = nullptr;
    static int s_screen;
    static GC s_context;

    window_t window_init(const char *title, int x, int y, int w, int h, bool sync) {
        window_t window;

        s_display = XOpenDisplay(nullptr);
        if (s_display == nullptr) {
            SF_ASSERT(false, "Unable to open X11 display for Linux!");
        }

        ulong black = BlackPixel(s_display, s_screen);
        ulong white = WhitePixel(s_display, s_screen);
        ulong red = SF_RGB(255, 0, 0);
        ulong blue = SF_RGB(0, 0, 255);

        s_screen = DefaultScreen(s_display);

        window.handle = reinterpret_cast<void*>(XCreateSimpleWindow(
            s_display,
            DefaultRootWindow(s_display),
            x, y,
            w, h,
            5,
            black,
            white
        ));

        XSetStandardProperties(
                s_display,
                SF_WINDOW_X11,
                title,
                title,
                None,
                nullptr,
                0,
                nullptr
        );

        XSelectInput(
                s_display,
                SF_WINDOW_X11,
                ExposureMask |
                ButtonPressMask |
                KeyPressMask
        );

        s_context = XCreateGC(s_display, SF_WINDOW_X11, 0, 0);

        XSetBackground(s_display, s_context, white);
        XSetForeground(s_display, s_context, black);
        XSync(s_display, sync);
        int num_sizes;
        XRRScreenSize* xrrs = XRRSizes(s_display, 0, &num_sizes);
        XRRScreenConfiguration* conf = XRRGetScreenInfo(s_display, SF_WINDOW_X11);
        window.refresh_rate = XRRConfigCurrentRate(conf);
        XClearWindow(s_display, SF_WINDOW_X11);
        XMapRaised(s_display, SF_WINDOW_X11);

        return window;
    }

    void window_free(const window_t &window) {
        XFreeGC(s_display, s_context);
        XDestroyWindow(s_display, SF_WINDOW_X11);
        XCloseDisplay(s_display);
    }

    static void handle_event(window_t& window, const XEvent& event) {
        window_t::desktop_events_t& events = window.desktop_events;
        switch (event.type) {
            case Expose: {
                if (event.xexpose.count == 0) {
                    XClearWindow(s_display, SF_WINDOW_X11);
                }
                break;
            }
            case KeyPress: {
                events.event_key_press(static_cast<SF_KEYCODE>(event.xkey.keycode));
                break;
            }
            case KeyRelease: {
                events.event_key_release(static_cast<SF_KEYCODE>(event.xkey.keycode));
                break;
            }
            case ButtonPress: {
                events.event_mouse_press(static_cast<SF_MOUSE_CODE>(event.xbutton.button));
                break;
            }
            case ButtonRelease: {
                events.event_mouse_release(static_cast<SF_MOUSE_CODE>(event.xbutton.button));
                break;
            }
        }
    }

    bool window_update(window_t &window) {
        XEvent event;
        bool updated = XNextEvent(s_display, &event) == 0;
        handle_event(window, event);
        return updated;
    }

    bool key_is_pressed(SF_KEYCODE keycode) {
    }

    bool mouse_is_pressed(SF_MOUSE_CODE mouse_code) {
    }

    bool gamepad_is_pressed(SF_GAMEPAD_CODE gamepad_code) {
    }

    cursor_t cursor_get() {
    }

}

#endif
// SF_LINUX_END

// SF_ANDROID_BEGIN
#if defined(SF_ANDROID)

namespace sf {



}

#endif
// SF_ANDROID_END