#pragma once

#include <Types.hpp>
#include <PlatformDefs.hpp>

#include <cstdio>

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
        __debugbreak(); \
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

enum LogColor {
    T3D_LOG_BLACK            = 0x00,
    T3D_LOG_BLUE             = 0x01,
    T3D_LOG_GREEN            = 0x02,
    T3D_LOG_AQUA             = 0x03,
    T3D_LOG_RED              = 0x04,
    T3D_LOG_PURPLE           = 0x05,
    T3D_LOG_YELLOW           = 0x06,
    T3D_LOG_WHITE            = 0x07,
    T3D_LOG_GRAY             = 0x08,
    T3D_LOG_LIGHT_BLUE       = 0x09,
    T3D_LOG_LIGHT_GREEN      = 0x0A,
    T3D_LOG_LIGHT_AQUA       = 0x0B,
    T3D_LOG_LIGHT_RED        = 0x0C,
    T3D_LOG_LIGHT_PURPLE     = 0x0D,
    T3D_LOG_LIGHT_YELLOW     = 0x0E,
    T3D_LOG_LIGHT_WHITE      = 0x0F,
};

enum LogLevel {
    T3D_LOG_VERBOSE,
    T3D_LOG_INFO,
    T3D_LOG_DEBUG,
    T3D_LOG_WARNING,
    T3D_LOG_ERROR,
    T3D_LOG_ASSERTION,
};

void __PrintLog(LogLevel logLevel, char* log);

template<typename... Args>
void __LogVerbose(const char* msg, Args... args) {
    char fmt_buffer[8192] = {};
    char text_buffer[8192] = {};
    sprintf(fmt_buffer, "%s", msg);
    sprintf(text_buffer, fmt_buffer, args...);
    __PrintLog(T3D_LOG_VERBOSE, text_buffer);
}

template<typename... Args>
void __LogInfo(const char* msg, Args... args) {
    char fmt_buffer[8192] = {};
    char text_buffer[8192] = {};
    sprintf(fmt_buffer, "%s", msg);
    sprintf(text_buffer, fmt_buffer, args...);
    __PrintLog(T3D_LOG_INFO, text_buffer);
}

template<typename... Args>
void __LogDebug(const char* msg, Args... args) {
    char fmt_buffer[8192] = {};
    char text_buffer[8192] = {};
    sprintf(fmt_buffer, "%s", msg);
    sprintf(text_buffer, fmt_buffer, args...);
    __PrintLog(T3D_LOG_DEBUG, text_buffer);
}

template<typename... Args>
void __LogWarning(const char* msg, Args... args) {
    char fmt_buffer[8192] = {};
    char text_buffer[8192] = {};
    sprintf(fmt_buffer, "%s", msg);
    sprintf(text_buffer, fmt_buffer, args...);
    __PrintLog(T3D_LOG_WARNING, text_buffer);
}

template<typename... Args>
void __LogError(const char* filename, const char* function, int line, const char* msg, Args... args) {
    char fmt_buffer[8192] = {};
    char text_buffer[8192] = {};
    sprintf(fmt_buffer, "Error in %s -> %s(%i line):\n%s", filename, function, line, msg);
    sprintf(text_buffer, fmt_buffer, args...);
    __PrintLog(T3D_LOG_ERROR, text_buffer);
}

template<typename... Args>
void __LogAssert(const char* filename, const char* function, int line, const char* msg, Args... args) {
    char fmt_buffer[8192] = {};
    char text_buffer[8192] = {};
    sprintf(fmt_buffer, "Assertion Failed in %s -> %s(%i line):\n%s", filename, function, line, msg);
    sprintf(text_buffer, fmt_buffer, args...);
    __PrintLog(T3D_LOG_ASSERTION, text_buffer);
}