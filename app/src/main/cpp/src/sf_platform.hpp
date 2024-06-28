#pragma once

#include <sf.hpp>

enum SF_KEYCODE
{
    SF_KEYCODE_NONE                = 0,
    SF_KEYCODE_SPACE               = 32,
    SF_KEYCODE_APOSTROPHE          = 39, /* ' */
    SF_KEYCODE_COMMA               = 44, /* , */
    SF_KEYCODE_MINUS               = 45, /* - */
    SF_KEYCODE_PERIOD              = 46, /* . */
    SF_KEYCODE_SLASH               = 47, /* / */

    SF_KEYCODE_D0                  = 48, /* 0 */
    SF_KEYCODE_D1                  = 49, /* 1 */
    SF_KEYCODE_D2                  = 50, /* 2 */
    SF_KEYCODE_D3                  = 51, /* 3 */
    SF_KEYCODE_D4                  = 52, /* 4 */
    SF_KEYCODE_D5                  = 53, /* 5 */
    SF_KEYCODE_D6                  = 54, /* 6 */
    SF_KEYCODE_D7                  = 55, /* 7 */
    SF_KEYCODE_D8                  = 56, /* 8 */
    SF_KEYCODE_D9                  = 57, /* 9 */

    SF_KEYCODE_SEMICOLON           = 59, /* ; */
    SF_KEYCODE_EQUAL               = 61, /* = */

    SF_KEYCODE_A                   = 65,
    SF_KEYCODE_B                   = 66,
    SF_KEYCODE_C                   = 67,
    SF_KEYCODE_D                   = 68,
    SF_KEYCODE_E                   = 69,
    SF_KEYCODE_F                   = 70,
    SF_KEYCODE_G                   = 71,
    SF_KEYCODE_H                   = 72,
    SF_KEYCODE_I                   = 73,
    SF_KEYCODE_J                   = 74,
    SF_KEYCODE_K                   = 75,
    SF_KEYCODE_L                   = 76,
    SF_KEYCODE_M                   = 77,
    SF_KEYCODE_N                   = 78,
    SF_KEYCODE_O                   = 79,
    SF_KEYCODE_P                   = 80,
    SF_KEYCODE_Q                   = 81,
    SF_KEYCODE_R                   = 82,
    SF_KEYCODE_S                   = 83,
    SF_KEYCODE_T                   = 84,
    SF_KEYCODE_U                   = 85,
    SF_KEYCODE_V                   = 86,
    SF_KEYCODE_W                   = 87,
    SF_KEYCODE_X                   = 88,
    SF_KEYCODE_Y                   = 89,
    SF_KEYCODE_Z                   = 90,

    SF_KEYCODE_LEFT_BRACKET         = 91,  /* [ */
    SF_KEYCODE_BACKSLASH           = 92,  /* \ */
    SF_KEYCODE_RIGHT_BRACKET        = 93,  /* ] */
    SF_KEYCODE_GRAVE_ACCENT         = 96,  /* ` */

    SF_KEYCODE_WORLD1              = 161, /* non-US #1 */
    SF_KEYCODE_WORLD2              = 162, /* non-US #2 */

    /* Function keys */
    SF_KEYCODE_ESC                 = 256,
    SF_KEYCODE_ENTER               = 257,
    SF_KEYCODE_TAB                 = 258,
    SF_KEYCODE_BACKSPACE           = 259,
    SF_KEYCODE_INSERT              = 260,
    SF_KEYCODE_DELETE              = 261,
    SF_KEYCODE_RIGHT               = 262,
    SF_KEYCODE_LEFT                = 263,
    SF_KEYCODE_DOWN                = 264,
    SF_KEYCODE_UP                  = 265,
    SF_KEYCODE_PAGE_UP              = 266,
    SF_KEYCODE_PAGE_DOWN            = 267,
    SF_KEYCODE_HOME                = 268,
    SF_KEYCODE_END                 = 269,
    SF_KEYCODE_CAPS_LOCK            = 280,
    SF_KEYCODE_SCROLL_LOCK          = 281,
    SF_KEYCODE_NUM_LOCK             = 282,
    SF_KEYCODE_PRINT_SCREEN         = 283,
    SF_KEYCODE_PAUSE               = 284,
    SF_KEYCODE_F1                  = 290,
    SF_KEYCODE_F2                  = 291,
    SF_KEYCODE_F3                  = 292,
    SF_KEYCODE_F4                  = 293,
    SF_KEYCODE_F5                  = 294,
    SF_KEYCODE_F6                  = 295,
    SF_KEYCODE_F7                  = 296,
    SF_KEYCODE_F8                  = 297,
    SF_KEYCODE_F9                  = 298,
    SF_KEYCODE_F10                 = 299,
    SF_KEYCODE_F11                 = 300,
    SF_KEYCODE_F12                 = 301,
    SF_KEYCODE_F13                 = 302,
    SF_KEYCODE_F14                 = 303,
    SF_KEYCODE_F15                 = 304,
    SF_KEYCODE_F16                 = 305,
    SF_KEYCODE_F17                 = 306,
    SF_KEYCODE_F18                 = 307,
    SF_KEYCODE_F19                 = 308,
    SF_KEYCODE_F20                 = 309,
    SF_KEYCODE_F21                 = 310,
    SF_KEYCODE_F22                 = 311,
    SF_KEYCODE_F23                 = 312,
    SF_KEYCODE_F24                 = 313,
    SF_KEYCODE_F25                 = 314,

    /* Keypad */
    SF_KEYCODE_KP0                 = 320,
    SF_KEYCODE_KP1                 = 321,
    SF_KEYCODE_KP2                 = 322,
    SF_KEYCODE_KP3                 = 323,
    SF_KEYCODE_KP4                 = 324,
    SF_KEYCODE_KP5                 = 325,
    SF_KEYCODE_KP6                 = 326,
    SF_KEYCODE_KP7                 = 327,
    SF_KEYCODE_KP8                 = 328,
    SF_KEYCODE_KP9                 = 329,
    SF_KEYCODE_KP_DECIMAL           = 330,
    SF_KEYCODE_KP_DIVIDE            = 331,
    SF_KEYCODE_KP_MULTIPLY          = 332,
    SF_KEYCODE_KP_SUBTRACT          = 333,
    SF_KEYCODE_KP_ADD               = 334,
    SF_KEYCODE_KP_ENTER             = 335,
    SF_KEYCODE_KP_EQUAL             = 336,

    SF_KEYCODE_LEFT_SHIFT           = 340,
    SF_KEYCODE_LEFT_CTRL         = 341,
    SF_KEYCODE_LEFT_ALT             = 342,
    SF_KEYCODE_LEFT_SUPER           = 343,
    SF_KEYCODE_RIGHT_SHIFT          = 344,
    SF_KEYCODE_RIGHT_CTRL        = 345,
    SF_KEYCODE_RIGHT_ALT            = 346,
    SF_KEYCODE_RIGHT_SUPER          = 347,
    SF_KEYCODE_MENU                = 348
};

enum SF_MOUSE_CODE
{
    SF_MOUSE_CODE_NONE                = 16,

    SF_MOUSE_CODE_BTN0                = 0,
    SF_MOUSE_CODE_BTN1                = 1,
    SF_MOUSE_CODE_BTN2                = 2,
    SF_MOUSE_CODE_BTN3                = 3,
    SF_MOUSE_CODE_BTN4                = 4,
    SF_MOUSE_CODE_BTN5                = 5,
    SF_MOUSE_CODE_BTN6                = 6,
    SF_MOUSE_CODE_BTN7                = 7,

    SF_MOUSE_CODE_BTN_LEFT            = SF_MOUSE_CODE_BTN0,
    SF_MOUSE_CODE_BTN_RIGHT           = SF_MOUSE_CODE_BTN1,
    SF_MOUSE_CODE_BTN_MIDDLE          = SF_MOUSE_CODE_BTN2,
    SF_MOUSE_CODE_BTN_LAST            = SF_MOUSE_CODE_BTN7,
};

enum SF_GAMEPAD_CODE
{
    SF_GAMEPAD_CODE_PAD_A = 0,
    SF_GAMEPAD_CODE_PAD_B = 1,
    SF_GAMEPAD_CODE_PAD_X = 2,
    SF_GAMEPAD_CODE_PAD_Y = 3,

    SF_GAMEPAD_CODE_PAD_LEFT_BUMPER = 4,
    SF_GAMEPAD_CODE_PAD_RIGHT_BUMPER = 5,

    SF_GAMEPAD_CODE_PAD_BACK = 6,
    SF_GAMEPAD_CODE_PAD_START = 7,
    SF_GAMEPAD_CODE_PAD_GUIDE = 8,

    SF_GAMEPAD_CODE_PAD_LEFT_THUMB = 9,
    SF_GAMEPAD_CODE_PAD_RIGHT_THUMB = 10,

    SF_GAMEPAD_CODE_PAD_UP = 11,
    SF_GAMEPAD_CODE_PAD_RIGHT = 12,
    SF_GAMEPAD_CODE_PAD_DOWN = 13,
    SF_GAMEPAD_CODE_PAD_LEFT = 14,

    SF_GAMEPAD_CODE_PAD_LAST = SF_GAMEPAD_CODE_PAD_LEFT,

    SF_GAMEPAD_CODE_PAD_CROSS = SF_GAMEPAD_CODE_PAD_A,
    SF_GAMEPAD_CODE_PAD_CIRCLE = SF_GAMEPAD_CODE_PAD_B,
    SF_GAMEPAD_CODE_PAD_SQUARE = SF_GAMEPAD_CODE_PAD_X,
    SF_GAMEPAD_CODE_PAD_TRIANGLE = SF_GAMEPAD_CODE_PAD_Y
};

enum SF_GAMEPAD_AXIS
{
    SF_GAMEPAD_AXIS_LEFT_X = 0,
    SF_GAMEPAD_AXIS_LEFT_Y = 1,
    SF_GAMEPAD_AXIS_RIGHT_X = 2,
    SF_GAMEPAD_AXIS_RIGHT_Y = 3,

    SF_GAMEPAD_AXIS_LEFT_TRIGGER = 4,
    SF_GAMEPAD_AXIS_RIGHT_TRIGGER = 5,

    SF_GAMEPAD_AXIS_LAST = SF_GAMEPAD_AXIS_RIGHT_TRIGGER
};

namespace sf {

    typedef void (*event_on_window_resize_t)(int w, int h);
    typedef void (*event_on_window_move_t)(int x, int y);
    typedef void (*event_on_key_t)(SF_KEYCODE keycode, bool pressed);
    typedef void (*event_on_mouse_t)(SF_MOUSE_CODE mouse_code, bool pressed);
    typedef void (*event_on_gamepad_t)(SF_GAMEPAD_CODE gamepad_code, bool pressed);
    typedef void (*event_on_cursor_move_t)(double x, double y);
    typedef void (*event_on_touch_move_t)(double x, double y);

    struct SF_API window_t final {

        struct SF_API desktop_events_t final {
            event_on_window_resize_t event_on_window_resize;
            event_on_window_move_t event_on_window_move;
            event_on_key_t event_on_key;
            event_on_mouse_t event_on_mouse;
            event_on_gamepad_t event_on_gamepad;
            event_on_cursor_move_t event_on_cursor_move;
        };

        struct SF_API phone_events_t final {
            event_on_touch_move_t event_on_touch_move;
        };

        desktop_events_t desktop_events;
        phone_events_t phone_events;

        void* handle;
        u32 refresh_rate;

    };

    SF_API window_t window_init(const char* title, int x, int y, int w, int h, bool sync);
    SF_API void window_free(const window_t& window);
    SF_API bool window_update(window_t& window);

}