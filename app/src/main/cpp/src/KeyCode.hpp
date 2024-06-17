#pragma once

enum T3D_KEYCODE
{
    T3D_KEYCODE_NONE                = 0,
    T3D_KEYCODE_SPACE               = 32,
    T3D_KEYCODE_APOSTROPHE          = 39, /* ' */
    T3D_KEYCODE_COMMA               = 44, /* , */
    T3D_KEYCODE_MINUS               = 45, /* - */
    T3D_KEYCODE_PERIOD              = 46, /* . */
    T3D_KEYCODE_SLASH               = 47, /* / */

    T3D_KEYCODE_D0                  = 48, /* 0 */
    T3D_KEYCODE_D1                  = 49, /* 1 */
    T3D_KEYCODE_D2                  = 50, /* 2 */
    T3D_KEYCODE_D3                  = 51, /* 3 */
    T3D_KEYCODE_D4                  = 52, /* 4 */
    T3D_KEYCODE_D5                  = 53, /* 5 */
    T3D_KEYCODE_D6                  = 54, /* 6 */
    T3D_KEYCODE_D7                  = 55, /* 7 */
    T3D_KEYCODE_D8                  = 56, /* 8 */
    T3D_KEYCODE_D9                  = 57, /* 9 */

    T3D_KEYCODE_SEMICOLON           = 59, /* ; */
    T3D_KEYCODE_EQUAL               = 61, /* = */

    T3D_KEYCODE_A                   = 65,
    T3D_KEYCODE_B                   = 66,
    T3D_KEYCODE_C                   = 67,
    T3D_KEYCODE_D                   = 68,
    T3D_KEYCODE_E                   = 69,
    T3D_KEYCODE_F                   = 70,
    T3D_KEYCODE_G                   = 71,
    T3D_KEYCODE_H                   = 72,
    T3D_KEYCODE_I                   = 73,
    T3D_KEYCODE_J                   = 74,
    T3D_KEYCODE_K                   = 75,
    T3D_KEYCODE_L                   = 76,
    T3D_KEYCODE_M                   = 77,
    T3D_KEYCODE_N                   = 78,
    T3D_KEYCODE_O                   = 79,
    T3D_KEYCODE_P                   = 80,
    T3D_KEYCODE_Q                   = 81,
    T3D_KEYCODE_R                   = 82,
    T3D_KEYCODE_S                   = 83,
    T3D_KEYCODE_T                   = 84,
    T3D_KEYCODE_U                   = 85,
    T3D_KEYCODE_V                   = 86,
    T3D_KEYCODE_W                   = 87,
    T3D_KEYCODE_X                   = 88,
    T3D_KEYCODE_Y                   = 89,
    T3D_KEYCODE_Z                   = 90,

    T3D_KEYCODE_LEFT_BRACKET         = 91,  /* [ */
    T3D_KEYCODE_BACKSLASH           = 92,  /* \ */
    T3D_KEYCODE_RIGHT_BRACKET        = 93,  /* ] */
    T3D_KEYCODE_GRAVE_ACCENT         = 96,  /* ` */

    T3D_KEYCODE_WORLD1              = 161, /* non-US #1 */
    T3D_KEYCODE_WORLD2              = 162, /* non-US #2 */

    /* Function keys */
    T3D_KEYCODE_ESC                 = 256,
    T3D_KEYCODE_ENTER               = 257,
    T3D_KEYCODE_TAB                 = 258,
    T3D_KEYCODE_BACKSPACE           = 259,
    T3D_KEYCODE_INSERT              = 260,
    T3D_KEYCODE_DELETE              = 261,
    T3D_KEYCODE_RIGHT               = 262,
    T3D_KEYCODE_LEFT                = 263,
    T3D_KEYCODE_DOWN                = 264,
    T3D_KEYCODE_UP                  = 265,
    T3D_KEYCODE_PAGE_UP              = 266,
    T3D_KEYCODE_PAGE_DOWN            = 267,
    T3D_KEYCODE_HOME                = 268,
    T3D_KEYCODE_END                 = 269,
    T3D_KEYCODE_CAPS_LOCK            = 280,
    T3D_KEYCODE_SCROLL_LOCK          = 281,
    T3D_KEYCODE_NUM_LOCK             = 282,
    T3D_KEYCODE_PRINT_SCREEN         = 283,
    T3D_KEYCODE_PAUSE               = 284,
    T3D_KEYCODE_F1                  = 290,
    T3D_KEYCODE_F2                  = 291,
    T3D_KEYCODE_F3                  = 292,
    T3D_KEYCODE_F4                  = 293,
    T3D_KEYCODE_F5                  = 294,
    T3D_KEYCODE_F6                  = 295,
    T3D_KEYCODE_F7                  = 296,
    T3D_KEYCODE_F8                  = 297,
    T3D_KEYCODE_F9                  = 298,
    T3D_KEYCODE_F10                 = 299,
    T3D_KEYCODE_F11                 = 300,
    T3D_KEYCODE_F12                 = 301,
    T3D_KEYCODE_F13                 = 302,
    T3D_KEYCODE_F14                 = 303,
    T3D_KEYCODE_F15                 = 304,
    T3D_KEYCODE_F16                 = 305,
    T3D_KEYCODE_F17                 = 306,
    T3D_KEYCODE_F18                 = 307,
    T3D_KEYCODE_F19                 = 308,
    T3D_KEYCODE_F20                 = 309,
    T3D_KEYCODE_F21                 = 310,
    T3D_KEYCODE_F22                 = 311,
    T3D_KEYCODE_F23                 = 312,
    T3D_KEYCODE_F24                 = 313,
    T3D_KEYCODE_F25                 = 314,

    /* Keypad */
    T3D_KEYCODE_KP0                 = 320,
    T3D_KEYCODE_KP1                 = 321,
    T3D_KEYCODE_KP2                 = 322,
    T3D_KEYCODE_KP3                 = 323,
    T3D_KEYCODE_KP4                 = 324,
    T3D_KEYCODE_KP5                 = 325,
    T3D_KEYCODE_KP6                 = 326,
    T3D_KEYCODE_KP7                 = 327,
    T3D_KEYCODE_KP8                 = 328,
    T3D_KEYCODE_KP9                 = 329,
    T3D_KEYCODE_KP_DECIMAL           = 330,
    T3D_KEYCODE_KP_DIVIDE            = 331,
    T3D_KEYCODE_KP_MULTIPLY          = 332,
    T3D_KEYCODE_KP_SUBTRACT          = 333,
    T3D_KEYCODE_KP_ADD               = 334,
    T3D_KEYCODE_KP_ENTER             = 335,
    T3D_KEYCODE_KP_EQUAL             = 336,

    T3D_KEYCODE_LEFT_SHIFT           = 340,
    T3D_KEYCODE_LEFT_CTRL         = 341,
    T3D_KEYCODE_LEFT_ALT             = 342,
    T3D_KEYCODE_LEFT_SUPER           = 343,
    T3D_KEYCODE_RIGHT_SHIFT          = 344,
    T3D_KEYCODE_RIGHT_CTRL        = 345,
    T3D_KEYCODE_RIGHT_ALT            = 346,
    T3D_KEYCODE_RIGHT_SUPER          = 347,
    T3D_KEYCODE_MENU                = 348
};