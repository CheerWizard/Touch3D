#pragma once

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
    T3D_LOG_COLOR_BLACK            = 0x00,
    T3D_LOG_COLOR_BLUE             = 0x01,
    T3D_LOG_COLOR_GREEN            = 0x02,
    T3D_LOG_COLOR_AQUA             = 0x03,
    T3D_LOG_COLOR_RED              = 0x04,
    T3D_LOG_COLOR_PURPLE           = 0x05,
    T3D_LOG_COLOR_YELLOW           = 0x06,
    T3D_LOG_COLOR_WHITE            = 0x07,
    T3D_LOG_COLOR_GRAY             = 0x08,
    T3D_LOG_COLOR_LIGHT_BLUE       = 0x09,
    T3D_LOG_COLOR_LIGHT_GREEN      = 0x0A,
    T3D_LOG_COLOR_LIGHT_AQUA       = 0x0B,
    T3D_LOG_COLOR_LIGHT_RED        = 0x0C,
    T3D_LOG_COLOR_LIGHT_PURPLE     = 0x0D,
    T3D_LOG_COLOR_LIGHT_YELLOW     = 0x0E,
    T3D_LOG_COLOR_LIGHT_WHITE      = 0x0F,
};

enum T3D_LOG_LEVEL {
    T3D_LOG_LEVEL_VERBOSE,
    T3D_LOG_LEVEL_INFO,
    T3D_LOG_LEVEL_DEBUG,
    T3D_LOG_LEVEL_WARNING,
    T3D_LOG_LEVEL_ERROR,
    T3D_LOG_LEVEL_ASSERTION,
};

void __PrintLog(T3D_LOG_LEVEL logLevel, char* log);

template<typename... Args>
void __LogVerbose(const char* msg, Args... args) {
    char fmt_buffer[1024] = {};
    char text_buffer[1024] = {};
    sprintf(fmt_buffer, "%s", msg);
    sprintf(text_buffer, fmt_buffer, args...);
    __PrintLog(T3D_LOG_LEVEL_VERBOSE, text_buffer);
}

template<typename... Args>
void __LogInfo(const char* msg, Args... args) {
    char fmt_buffer[1024] = {};
    char text_buffer[1024] = {};
    sprintf(fmt_buffer, "%s", msg);
    sprintf(text_buffer, fmt_buffer, args...);
    __PrintLog(T3D_LOG_LEVEL_INFO, text_buffer);
}

template<typename... Args>
void __LogDebug(const char* msg, Args... args) {
    char fmt_buffer[1024] = {};
    char text_buffer[1024] = {};
    sprintf(fmt_buffer, "%s", msg);
    sprintf(text_buffer, fmt_buffer, args...);
    __PrintLog(T3D_LOG_LEVEL_DEBUG, text_buffer);
}

template<typename... Args>
void __LogWarning(const char* msg, Args... args) {
    char fmt_buffer[1024] = {};
    char text_buffer[1024] = {};
    sprintf(fmt_buffer, "%s", msg);
    sprintf(text_buffer, fmt_buffer, args...);
    __PrintLog(T3D_LOG_LEVEL_WARNING, text_buffer);
}

template<typename... Args>
void __LogError(const char* filename, const char* function, int line, const char* msg, Args... args) {
    char fmt_buffer[1024] = {};
    char text_buffer[1024] = {};
    sprintf(fmt_buffer, "Error in %s -> %s(%i line):\n%s", filename, function, line, msg);
    sprintf(text_buffer, fmt_buffer, args...);
    __PrintLog(T3D_LOG_LEVEL_ERROR, text_buffer);
}

template<typename... Args>
void __LogAssert(const char* filename, const char* function, int line, const char* msg, Args... args) {
    char fmt_buffer[1024] = {};
    char text_buffer[1024] = {};
    sprintf(fmt_buffer, "Assertion Failed in %s -> %s(%i line):\n%s", filename, function, line, msg);
    sprintf(text_buffer, fmt_buffer, args...);
    __PrintLog(T3D_LOG_LEVEL_ASSERTION, text_buffer);
}