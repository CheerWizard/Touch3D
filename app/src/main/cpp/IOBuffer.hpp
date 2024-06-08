#pragma once

#include <Math.hpp>

#include <vector>

typedef void (*OnWindowResized)(int2 size);
typedef void (*OnWindowMoved)(int2 size);

typedef void (*OnCursorMoved)(double2 position);

typedef void (*OnTouchMoved)(double2 position);

struct IOBuffer final {
    std::vector<OnWindowResized> on_window_resized;
    std::vector<OnWindowMoved> on_window_moved;
    std::vector<OnCursorMoved> on_cursor_moved;
    std::vector<OnTouchMoved> on_touch_moved;
};