#pragma once

#include <Types.hpp>
#include <DebugBreak.hpp>
#include <Time.hpp>
#include <ThreadPool.hpp>

#include <cstdio>
#include <cstring>
#include <mutex>

#if defined(T3D_WINDOWS)
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

#if defined(T3D_DEBUG)

#define LogOpen(filepath) Log::Open(filepath)
#define LogClose() Log::Close()

#if defined(T3D_ANDROID)

#define LogVerbose(msg, ...) Log::Verbose(msg, 0, ##__VA_ARGS__)
#define LogInfo(msg, ...) Log::Info(msg, 0, ##__VA_ARGS__)
#define LogDebug(msg, ...) Log::Debug(msg, 0, ##__VA_ARGS__)
#define LogWarning(msg, ...) Log::Warning(msg, 0, ##__VA_ARGS__)
#define LogError(msg, ...) Log::Error(__FILENAME__, __FUNCTION__, __LINE__, msg, 0, ##__VA_ARGS__)
#define LogAssert(x, msg, ...) \
{                                \
    if (!(x)) {                  \
        Log::Assert(__FILENAME__, __FUNCTION__, __LINE__, msg, 0, ##__VA_ARGS__); \
        T3D_DEBUGBREAK(); \
    }\
}

#else

#define LogVerbose(msg, ...) Log::Verbose(msg, ##__VA_ARGS__)
#define LogInfo(msg, ...) Log::Info(msg, ##__VA_ARGS__)
#define LogDebug(msg, ...) Log::Debug(msg, ##__VA_ARGS__)
#define LogWarning(msg, ...) Log::Warning(msg, ##__VA_ARGS__)
#define LogError(msg, ...) Log::Error(__FILENAME__, __FUNCTION__, __LINE__, msg, ##__VA_ARGS__)
#define LogAssert(x, msg, ...) \
{                                \
    if (!(x)) {                  \
        Log::Assert(__FILENAME__, __FUNCTION__, __LINE__, msg, ##__VA_ARGS__); \
        T3D_DEBUGBREAK(); \
    }\
}

#endif

#else

#define LogOpen(filepath)
#define LogClose()

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

class Log final {

public:

    static void Open(const char* filepath);
    static void Close();

    template<typename... Args>
    static void Verbose(const char* msg, Args... args) {
        s_thread_pool->Add([msg, args...]() {
            char fmt_buffer[256] = {};
            char text_buffer[256] = {};
            ClockTime time = Clock::GetTime();
            sprintf(
                    fmt_buffer,
                    "\n[%d.%d.%d][%d:%d:%d.%d] %s\0",
                    time.d, time.m, time.y, time.h, time.min, time.s, time.ms,
                    msg
            );
            sprintf(text_buffer, fmt_buffer, args...);
            PrintVerbose(T3D_LOG_COLOR_LIGHT_GREEN, text_buffer);
            PrintFile(text_buffer);
        });
    }

    template<typename... Args>
    static void Info(const char* msg, Args... args) {
        s_thread_pool->Add([msg, args...]() {
            char fmt_buffer[256] = {};
            char text_buffer[256] = {};
            ClockTime time = Clock::GetTime();
            sprintf(
                    fmt_buffer,
                    "\n[%d.%d.%d][%d:%d:%d.%d] %s\0",
                    time.d, time.m, time.y, time.h, time.min, time.s, time.ms,
                    msg
            );
            sprintf(text_buffer, fmt_buffer, args...);
            PrintInfo(T3D_LOG_COLOR_GREEN, text_buffer);
            PrintFile(text_buffer);
        });
    }

    template<typename... Args>
    static void Debug(const char* msg, Args... args) {
        s_thread_pool->Add([msg, args...]() {
            char fmt_buffer[256] = {};
            char text_buffer[256] = {};
            ClockTime time = Clock::GetTime();
            sprintf(
                    fmt_buffer,
                    "\n[%d.%d.%d][%d:%d:%d.%d] %s\0",
                    time.d, time.m, time.y, time.h, time.min, time.s, time.ms,
                    msg
            );
            sprintf(text_buffer, fmt_buffer, args...);
            PrintDebug(T3D_LOG_COLOR_WHITE, text_buffer);
            PrintFile(text_buffer);
        });
    }

    template<typename... Args>
    static void Warning(const char* msg, Args... args) {
        s_thread_pool->Add([msg, args...]() {
            char fmt_buffer[256] = {};
            char text_buffer[256] = {};
            ClockTime time = Clock::GetTime();
            sprintf(
                    fmt_buffer,
                    "\n[%d.%d.%d][%d:%d:%d.%d] %s\0",
                    time.d, time.m, time.y, time.h, time.min, time.s, time.ms,
                    msg
            );
            sprintf(text_buffer, fmt_buffer, args...);
            PrintWarning(T3D_LOG_COLOR_YELLOW, text_buffer);
            PrintFile(text_buffer);
        });
    }

    template<typename... Args>
    static void Error(const char* filename, const char* function, int line, const char* msg, Args... args) {
        s_thread_pool->Add([filename, function, line, msg, args...]() {
            char fmt_buffer[256] = {};
            char text_buffer[256] = {};
            ClockTime time = Clock::GetTime();
            sprintf(
                    fmt_buffer,
                    "\n[%d.%d.%d][%d:%d:%d.%d] Error in %s -> %s(%i line):\n%s\0",
                    time.d, time.m, time.y, time.h, time.min, time.s, time.ms,
                    filename, function, line,
                    msg
            );
            sprintf(text_buffer, fmt_buffer, args...);
            PrintError(T3D_LOG_COLOR_RED, text_buffer);
            PrintFile(text_buffer);
        });
    }

    template<typename... Args>
    static void Assert(const char* filename, const char* function, int line, const char* msg, Args... args) {
        s_thread_pool->Add([filename, function, line, msg, args...]() {
            char fmt_buffer[256] = {};
            char text_buffer[256] = {};
            ClockTime time = Clock::GetTime();
            sprintf(
                    fmt_buffer,
                    "\n[%d.%d.%d][%d:%d:%d.%d] Assertion Failed in %s -> %s(%i line):\n%s\0",
                    time.d, time.m, time.y, time.h, time.min, time.s, time.ms,
                    filename, function, line,
                    msg
            );
            sprintf(text_buffer, fmt_buffer, args...);
            PrintAssert(T3D_LOG_COLOR_RED, text_buffer);
            PrintFile(text_buffer);
        });
    }

private:
    static void PrintVerbose(T3D_LOG_COLOR log_color, char* log);
    static void PrintInfo(T3D_LOG_COLOR log_color, char* log);
    static void PrintDebug(T3D_LOG_COLOR log_color, char* log);
    static void PrintWarning(T3D_LOG_COLOR log_color, char* log);
    static void PrintError(T3D_LOG_COLOR log_color, char* log);
    static void PrintAssert(T3D_LOG_COLOR log_color, char* log);

    static void PrintFile(char* log);

private:
    static FILE* s_file;
    static std::mutex s_mutex;
    static ThreadPool<10>* s_thread_pool;

};