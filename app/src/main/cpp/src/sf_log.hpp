#pragma once

#include <sf.hpp>
#include <cstdio>
#include <cstring>

#if defined(SF_DEBUG)

#define SF_LOG_OPEN(filepath) sf::Log::open(filepath)
#define SF_LOG_CLOSE() sf::Log::close()

#if defined(SF_ANDROID)

#define SF_LOG_VERB(msg, ...) sf::Log::log_verbose(msg, 0, ##__VA_ARGS__)
#define SF_LOG_INFO(msg, ...) sf::Log::log_info(msg, 0, ##__VA_ARGS__)
#define SF_LOG_DBG(msg, ...) sf::Log::log_debug(msg, 0, ##__VA_ARGS__)
#define SF_LOG_WARN(msg, ...) sf::Log::log_warning(msg, 0, ##__VA_ARGS__)
#define SF_LOG_ERR(msg, ...) sf::Log::log_error(SF_FILENAME, __FUNCTION__, __LINE__, msg, 0, ##__VA_ARGS__)
#define SF_LOG_ASSERT(x, msg, ...) \
{                                \
    if (!(x)) {                  \
        sf::Log::log_assert(SF_FILENAME, __FUNCTION__, __LINE__, msg, 0, ##__VA_ARGS__); \
        SF_DEBUG_BREAK(); \
    }\
}

#else

#define SF_LOG_VERB(msg, ...) sf::Log::log_verbose(msg, ##__VA_ARGS__)
#define SF_LOG_INFO(msg, ...) sf::Log::log_info(msg, ##__VA_ARGS__)
#define SF_LOG_DBG(msg, ...) sf::Log::log_debug(msg, ##__VA_ARGS__)
#define SF_LOG_WARN(msg, ...) sf::Log::log_warning(msg, ##__VA_ARGS__)
#define SF_LOG_ERR(msg, ...) sf::Log::log_error(SF_FILENAME, __FUNCTION__, __LINE__, msg, ##__VA_ARGS__)
#define SF_LOG_ASSERT(x, msg, ...) \
{                                \
    if (!(x)) {                  \
        sf::Log::log_assert(SF_FILENAME, __FUNCTION__, __LINE__, msg, ##__VA_ARGS__); \
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

namespace sf {

    class SF_API Log final {

    public:
        static void open(const char* filepath);
        static void close();

        template<typename... Args>
        static void log_verbose(const char* msg, Args... args) {
            s_thread_pool->add([msg, args...]() {
                char fmt_buffer[256] = {};
                char text_buffer[256] = {};
                DateTime time = get_current_date_time();
#if defined(__LP64__)
                const char *fmt = "\n[%d.%d.%d][%ld:%ld:%ld.%ld] %s";
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
                print_verbose(SF_LOG_COLOR_LIGHT_GREEN, text_buffer);
                write_file(text_buffer);
            });
        }

        template<typename... Args>
        static void log_info(const char* msg, Args... args) {
            s_thread_pool->add([msg, args...]() {
                char fmt_buffer[256] = {};
                char text_buffer[256] = {};
                DateTime time = get_current_date_time();
#if defined(__LP64__)
                const char *fmt = "\n[%d.%d.%d][%ld:%ld:%ld.%ld] %s";
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
                print_info(SF_LOG_COLOR_GREEN, text_buffer);
                write_file(text_buffer);
            });
        }

        template<typename... Args>
        static void log_debug(const char* msg, Args... args) {
            s_thread_pool->add([msg, args...]() {
                char fmt_buffer[256] = {};
                char text_buffer[256] = {};
                DateTime time = get_current_date_time();
#if defined(__LP64__)
                const char *fmt = "\n[%d.%d.%d][%ld:%ld:%ld.%ld] %s";
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
                print_debug(SF_LOG_COLOR_WHITE, text_buffer);
                write_file(text_buffer);
            });
        }

        template<typename... Args>
        static void log_warning(const char* msg, Args... args) {
            s_thread_pool->add([msg, args...]() {
                char fmt_buffer[256] = {};
                char text_buffer[256] = {};
                DateTime time = get_current_date_time();
#if defined(__LP64__)
                const char *fmt = "\n[%d.%d.%d][%ld:%ld:%ld.%ld] %s";
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
                print_warning(SF_LOG_COLOR_YELLOW, text_buffer);
                write_file(text_buffer);
            });
        }

        template<typename... Args>
        static void log_error(const char* filename, const char* function, int line, const char* msg, Args... args) {
            s_thread_pool->add([filename, function, line, msg, args...]() {
                char fmt_buffer[256] = {};
                char text_buffer[256] = {};
                DateTime time = get_current_date_time();
#if defined(__LP64__)
                const char *fmt = "\n[%d.%d.%d][%ld:%ld:%ld.%ld] log_error in %s -> %s(%i line):\n%s";
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
                print_error(SF_LOG_COLOR_RED, text_buffer);
                write_file(text_buffer);
            });
        }

        template<typename... Args>
        static void log_assert(const char* filename, const char* function, int line, const char* msg, Args... args) {
            s_thread_pool->add([filename, function, line, msg, args...]() {
                char fmt_buffer[256] = {};
                char text_buffer[256] = {};
                DateTime time = get_current_date_time();
#if defined(__LP64__)
                const char *fmt = "\n[%d.%d.%d][%ld:%ld:%ld.%ld] Assertion Failed in %s -> %s(%i line):\n%s";
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
                print_assert(SF_LOG_COLOR_RED, text_buffer);
                write_file(text_buffer);
            });
        }

    private:
        static void print_verbose(SF_LOG_COLOR log_color, char* log);
        static void print_info(SF_LOG_COLOR log_color, char* log);
        static void print_debug(SF_LOG_COLOR log_color, char* log);
        static void print_warning(SF_LOG_COLOR log_color, char* log);
        static void print_error(SF_LOG_COLOR log_color, char* log);
        static void print_assert(SF_LOG_COLOR log_color, char* log);

        static void write_file(char* log);

    private:
        static FILE* s_file;
        static ThreadPool<1, 10, SF_THREAD_PRIORITY_HIGHEST>* s_thread_pool;
    };

}