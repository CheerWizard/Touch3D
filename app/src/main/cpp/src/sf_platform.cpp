#include <sf_platform.hpp>

namespace sf {



}

// SF_WINDOWS_BEGIN
#if defined(SF_WINDOWS)

namespace sf {



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