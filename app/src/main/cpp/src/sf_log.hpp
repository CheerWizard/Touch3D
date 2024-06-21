#pragma once

#include <sf.hpp>
#include <cstdio>
#include <cstring>

#if defined(SF_DEBUG)

#define SF_LOG_OPEN(filepath) sf::Log::Open(filepath)
#define SF_LOG_CLOSE() sf::Log::Close()

#if defined(SF_ANDROID)

#define SF_LOG_VERB(msg, ...) sf::Log::Verbose(msg, 0, ##__VA_ARGS__)
#define SF_LOG_INFO(msg, ...) sf::Log::Info(msg, 0, ##__VA_ARGS__)
#define SF_LOG_DBG(msg, ...) sf::Log::Debug(msg, 0, ##__VA_ARGS__)
#define SF_LOG_WARN(msg, ...) sf::Log::Warning(msg, 0, ##__VA_ARGS__)
#define SF_LOG_ERR(msg, ...) sf::Log::Error(SF_FILENAME, __FUNCTION__, __LINE__, msg, 0, ##__VA_ARGS__)
#define SF_LOG_ASSERT(x, msg, ...) \
{                                \
    if (!(x)) {                  \
        sf::Log::Assert(SF_FILENAME, __FUNCTION__, __LINE__, msg, 0, ##__VA_ARGS__); \
        SF_DEBUG_BREAK(); \
    }\
}

#else

#define SF_LOG_VERB(msg, ...) sf::Log::Verbose(msg, ##__VA_ARGS__)
#define SF_LOG_INFO(msg, ...) sf::Log::Info(msg, ##__VA_ARGS__)
#define SF_LOG_DBG(msg, ...) sf::Log::Debug(msg, ##__VA_ARGS__)
#define SF_LOG_WARN(msg, ...) sf::Log::Warning(msg, ##__VA_ARGS__)
#define SF_LOG_ERR(msg, ...) sf::Log::Error(SF_FILENAME, __FUNCTION__, __LINE__, msg, ##__VA_ARGS__)
#define SF_LOG_ASSERT(x, msg, ...) \
{                                \
    if (!(x)) {                  \
        sf::Log::Assert(SF_FILENAME, __FUNCTION__, __LINE__, msg, ##__VA_ARGS__); \
        SF_DEBUG_BREAK(); \
    }\
}

#endif

#else

#define SF_LOG_OPEN(filepath)
#define SF_LOG_CLOSE()

#define SF_LOG_VERB(msg, ...)
#define SF_LOG_INFO(msg, ...)
#define SF_LOG_DBG(msg, ...)
#define SF_LOG_WARN(msg, ...)
#define SF_LOG_ERR(msg, ...)
#define SF_LOG_ASSERT(x, msg, ...)

#endif

namespace sf {

    enum SF_LOG_COLOR {
        SF_LOG_COLOR_BLACK,
        SF_LOG_COLOR_BLUE,
        SF_LOG_COLOR_GREEN,
        SF_LOG_COLOR_AQUA,
        SF_LOG_COLOR_RED,
        SF_LOG_COLOR_PURPLE,
        SF_LOG_COLOR_YELLOW,
        SF_LOG_COLOR_WHITE,
        SF_LOG_COLOR_GRAY,
        SF_LOG_COLOR_LIGHT_BLUE,
        SF_LOG_COLOR_LIGHT_GREEN,
        SF_LOG_COLOR_LIGHT_AQUA,
        SF_LOG_COLOR_LIGHT_RED,
        SF_LOG_COLOR_LIGHT_PURPLE,
        SF_LOG_COLOR_LIGHT_YELLOW,
        SF_LOG_COLOR_LIGHT_WHITE,

        SF_LOG_COLOR_COUNT
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
                Time time = Time::GetCurrentTime();
#if defined(__LP64__)
                const char* fmt = "\n[%d.%d.%d][%ld:%ld:%ld.%ld] %s";
#else
                const char* fmt = "\n[%d.%d.%d][%lld:%lld:%lld.%lld] %s";
#endif
                sprintf(
                        fmt_buffer,
                        fmt,
                        time.d, time.m, time.y, time.h, time.min, time.s, time.ms,
                        msg
                );
                sprintf(text_buffer, fmt_buffer, args...);
                PrintVerbose(SF_LOG_COLOR_LIGHT_GREEN, text_buffer);
                PrintFile(text_buffer);
            });
        }

        template<typename... Args>
        static void Info(const char* msg, Args... args) {
            s_thread_pool->Add([msg, args...]() {
                char fmt_buffer[256] = {};
                char text_buffer[256] = {};
                Time time = Time::GetCurrentTime();
#if defined(__LP64__)
                const char* fmt = "\n[%d.%d.%d][%ld:%ld:%ld.%ld] %s";
#else
                const char* fmt = "\n[%d.%d.%d][%lld:%lld:%lld.%lld] %s";
#endif
                sprintf(
                        fmt_buffer,
                        fmt,
                        time.d, time.m, time.y, time.h, time.min, time.s, time.ms,
                        msg
                );
                sprintf(text_buffer, fmt_buffer, args...);
                PrintInfo(SF_LOG_COLOR_GREEN, text_buffer);
                PrintFile(text_buffer);
            });
        }

        template<typename... Args>
        static void Debug(const char* msg, Args... args) {
            s_thread_pool->Add([msg, args...]() {
                char fmt_buffer[256] = {};
                char text_buffer[256] = {};
                Time time = Time::GetCurrentTime();
#if defined(__LP64__)
                const char* fmt = "\n[%d.%d.%d][%ld:%ld:%ld.%ld] %s";
#else
                const char* fmt = "\n[%d.%d.%d][%lld:%lld:%lld.%lld] %s";
#endif
                sprintf(
                        fmt_buffer,
                        fmt,
                        time.d, time.m, time.y, time.h, time.min, time.s, time.ms,
                        msg
                );
                sprintf(text_buffer, fmt_buffer, args...);
                PrintDebug(SF_LOG_COLOR_WHITE, text_buffer);
                PrintFile(text_buffer);
            });
        }

        template<typename... Args>
        static void Warning(const char* msg, Args... args) {
            s_thread_pool->Add([msg, args...]() {
                char fmt_buffer[256] = {};
                char text_buffer[256] = {};
                Time time = Time::GetCurrentTime();
#if defined(__LP64__)
                const char* fmt = "\n[%d.%d.%d][%ld:%ld:%ld.%ld] %s";
#else
                const char* fmt = "\n[%d.%d.%d][%lld:%lld:%lld.%lld] %s";
#endif
                sprintf(
                        fmt_buffer,
                        fmt,
                        time.d, time.m, time.y, time.h, time.min, time.s, time.ms,
                        msg
                );
                sprintf(text_buffer, fmt_buffer, args...);
                PrintWarning(SF_LOG_COLOR_YELLOW, text_buffer);
                PrintFile(text_buffer);
            });
        }

        template<typename... Args>
        static void Error(const char* filename, const char* function, int line, const char* msg, Args... args) {
            s_thread_pool->Add([filename, function, line, msg, args...]() {
                char fmt_buffer[256] = {};
                char text_buffer[256] = {};
                Time time = Time::GetCurrentTime();
#if defined(__LP64__)
                const char* fmt = "\n[%d.%d.%d][%ld:%ld:%ld.%ld] Error in %s -> %s(%i line):\n%s";
#else
                const char* fmt = "\n[%d.%d.%d][%lld:%lld:%lld.%lld] Error in %s -> %s(%i line):\n%s";
#endif
                sprintf(
                        fmt_buffer,
                        fmt,
                        time.d, time.m, time.y, time.h, time.min, time.s, time.ms,
                        filename, function, line,
                        msg
                );
                sprintf(text_buffer, fmt_buffer, args...);
                PrintError(SF_LOG_COLOR_RED, text_buffer);
                PrintFile(text_buffer);
            });
        }

        template<typename... Args>
        static void Assert(const char* filename, const char* function, int line, const char* msg, Args... args) {
            s_thread_pool->Add([filename, function, line, msg, args...]() {
                char fmt_buffer[256] = {};
                char text_buffer[256] = {};
                Time time = Time::GetCurrentTime();
#if defined(__LP64__)
                const char* fmt = "\n[%d.%d.%d][%ld:%ld:%ld.%ld] Assertion Failed in %s -> %s(%i line):\n%s";
#else
                const char* fmt = "\n[%d.%d.%d][%lld:%lld:%lld.%lld] Assertion Failed in %s -> %s(%i line):\n%s";
#endif
                sprintf(
                        fmt_buffer,
                        fmt,
                        time.d, time.m, time.y, time.h, time.min, time.s, time.ms,
                        filename, function, line,
                        msg
                );
                sprintf(text_buffer, fmt_buffer, args...);
                PrintAssert(SF_LOG_COLOR_RED, text_buffer);
                PrintFile(text_buffer);
            });
        }

    private:
        static void PrintVerbose(SF_LOG_COLOR log_color, char* log);
        static void PrintInfo(SF_LOG_COLOR log_color, char* log);
        static void PrintDebug(SF_LOG_COLOR log_color, char* log);
        static void PrintWarning(SF_LOG_COLOR log_color, char* log);
        static void PrintError(SF_LOG_COLOR log_color, char* log);
        static void PrintAssert(SF_LOG_COLOR log_color, char* log);

        static void PrintFile(char* log);

    private:
        static FILE* s_file;
        static ThreadPool<1, 10, SF_THREAD_PRIORITY_HIGHEST>* s_thread_pool;
    };

}