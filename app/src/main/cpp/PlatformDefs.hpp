#pragma once

#if defined(_WIN32) || defined(_WIN64)

#define T3D_WINDOWS
#define T3D_DESKTOP

#elif defined(__APPLE__) || defined(__MACH__)

#include <TargetConditionals.h>

#if TARGET_OS_IPHONE == 1
#define T3D_IOS
#define T3D_PHONE

#elif TARGET_OS_MAC == 1
#define T3D_MACOS
#define T3D_DESKTOP

#endif

#elif defined(__ANDROID__)

#define T3D_ANDROID
#define T3D_PHONE

#elif defined(__linux__)

#define T3D_LINUX
#define T3D_DESKTOP

#else

#endif