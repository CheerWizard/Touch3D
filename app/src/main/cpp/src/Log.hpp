#pragma once

#include <Types.hpp>

#include <DebugBreak.hpp>

#include <cstdio>
#include <cstring>

#ifdef T3D_WINDOWS
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

#ifdef T3D_DEBUG

#define LogVerbose(msg, ...) __LogVerbose(msg, ##__VA_ARGS__)
#define LogInfo(msg, ...) __LogInfo(msg, ##__VA_ARGS__)
#define LogDebug(msg, ...) __LogDebug(msg, ##__VA_ARGS__)
#define LogWarning(msg, ...) __LogWarning(msg, ##__VA_ARGS__)
#define LogError(msg, ...) __LogError(__FILENAME__, __FUNCTION__, __LINE__, msg, ##__VA_ARGS__)
#define LogAssert(x, msg, ...) \
{                                \
    if (!(x)) {                  \
        __LogAssert(__FILENAME__, __FUNCTION__, __LINE__, msg, ##__VA_ARGS__); \
        T3D_DEBUGBREAK(); \
    }\
}

#else

#define LogVerbose(msg, ...)
#define LogInfo(msg, ...)
#define LogDebug(msg, ...)
#define LogWarning(msg, ...)
#define LogError(msg, ...)
#define LogAssert(x, msg, ...)

#endif

enum T3D_LOG_COLOR {
    T3D_LOG_COLOR_BLACK,
    T3D_LOG_COLOR_BLUE,
    T3D_LOG_COLOR_GREEN,
    T3D_LOG_COLOR_AQUA,
    T3D_LOG_COLOR_RED,
    T3D_LOG_COLOR_PURPLE,
    T3D_LOG_COLOR_YELLOW,
    T3D_LOG_COLOR_WHITE,
    T3D_LOG_COLOR_GRAY,
    T3D_LOG_COLOR_LIGHT_BLUE,
    T3D_LOG_COLOR_LIGHT_GREEN,
    T3D_LOG_COLOR_LIGHT_AQUA,
    T3D_LOG_COLOR_LIGHT_RED,
    T3D_LOG_COLOR_LIGHT_PURPLE,
    T3D_LOG_COLOR_LIGHT_YELLOW,
    T3D_LOG_COLOR_LIGHT_WHITE,

    T3D_LOG_COLOR_COUNT
};

enum T3D_LOG_LEVEL {
    T3D_LOG_LEVEL_VERBOSE,
    T3D_LOG_LEVEL_INFO,
    T3D_LOG_LEVEL_DEBUG,
    T3D_LOG_LEVEL_WARNING,
    T3D_LOG_LEVEL_ERROR,
    T3D_LOG_LEVEL_ASSERTION,
};

void __PrintLog(T3D_LOG_LEVEL log_level, T3D_LOG_COLOR log_color, char* log);

template<typename... Args>
void __LogVerbose(const char* msg, Args... args) {
    char fmt_buffer[1024] = {};
    char text_buffer[1024] = {};
    sprintf(fmt_buffer, "%s", msg);
    sprintf(text_buffer, fmt_buffer, args...);
    __PrintLog(T3D_LOG_LEVEL_VERBOSE, T3D_LOG_COLOR_WHITE, text_buffer);
}

template<typename... Args>
void __LogInfo(const char* msg, Args... args) {
    char fmt_buffer[1024] = {};
    char text_buffer[1024] = {};
    sprintf(fmt_buffer, "%s", msg);
    sprintf(text_buffer, fmt_buffer, args...);
    __PrintLog(T3D_LOG_LEVEL_INFO, T3D_LOG_COLOR_GREEN, text_buffer);
}

template<typename... Args>
void __LogDebug(const char* msg, Args... args) {
    char fmt_buffer[1024] = {};
    char text_buffer[1024] = {};
    sprintf(fmt_buffer, "%s", msg);
    sprintf(text_buffer, fmt_buffer, args...);
    __PrintLog(T3D_LOG_LEVEL_DEBUG, T3D_LOG_COLOR_LIGHT_GREEN, text_buffer);
}

template<typename... Args>
void __LogWarning(const char* msg, Args... args) {
    char fmt_buffer[1024] = {};
    char text_buffer[1024] = {};
    sprintf(fmt_buffer, "%s", msg);
    sprintf(text_buffer, fmt_buffer, args...);
    __PrintLog(T3D_LOG_LEVEL_WARNING, T3D_LOG_COLOR_YELLOW, text_buffer);
}

template<typename... Args>
void __LogError(const char* filename, const char* function, int line, const char* msg, Args... args) {
    char fmt_buffer[1024] = {};
    char text_buffer[1024] = {};
    sprintf(fmt_buffer, "Error in %s -> %s(%i line):\n%s", filename, function, line, msg);
    sprintf(text_buffer, fmt_buffer, args...);
    __PrintLog(T3D_LOG_LEVEL_ERROR, T3D_LOG_COLOR_RED, text_buffer);
}

template<typename... Args>
void __LogAssert(const char* filename, const char* function, int line, const char* msg, Args... args) {
    char fmt_buffer[1024] = {};
    char text_buffer[1024] = {};
    sprintf(fmt_buffer, "Assertion Failed in %s -> %s(%i line):\n%s", filename, function, line, msg);
    sprintf(text_buffer, fmt_buffer, args...);
    __PrintLog(T3D_LOG_LEVEL_ASSERTION, T3D_LOG_COLOR_RED, text_buffer);
}