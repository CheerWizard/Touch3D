#include <Application.hpp>

static Display* display;
static int screen;
static Window window_root;

int main() {
    display = XOpenDisplay(nullptr);
    if (display == nullptr) {
        LogAssert(false, "Unable to open X11 display for Linux!");
    }

    screen = DefaultScreen(display);
    window_root = RootWindow(display, screen);
    Window window = XCreateSimpleWindow(display, root, 400, 300, 800, 600, 15, BlackPixel(display, screen), WhitePixel(display, screen));
    XMapWindow(display, window);

    bool running = true;
    while (running) {
        XEvent event;
        running = XNextEvent(display, &event) == 0;
    }

    XUnmapWindow(display, window);
    XDestroyWindow(display, window);
    XCloseDisplay(display);

    return 0;
}