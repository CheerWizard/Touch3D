#pragma once

#include <sf.hpp>

#if defined(SF_DEBUG)

#define SF_LOG_OPEN(filepath) sf::log_file_open(filepath)
#define SF_LOG_CLOSE() sf::log_file_close()

#if defined(SF_ANDROID)

#define SF_LOG_VERB(msg, ...) sf::log_verbose(msg, 0, ##__VA_ARGS__)
#define SF_LOG_INFO(msg, ...) sf::log_info(msg, 0, ##__VA_ARGS__)
#define SF_LOG_DBG(msg, ...) sf::log_debug(msg, 0, ##__VA_ARGS__)
#define SF_LOG_WARN(msg, ...) sf::log_warning(msg, 0, ##__VA_ARGS__)
#define SF_LOG_ERR(msg, ...) sf::log_error(SF_FILENAME, __FUNCTION__, __LINE__, msg, 0, ##__VA_ARGS__)
#define SF_LOG_ASSERT(x, msg, ...) \
{                                \
    if (!(x)) {                  \
        sf::log_assert(SF_FILENAME, __FUNCTION__, __LINE__, msg, 0, ##__VA_ARGS__); \
        SF_DEBUG_BREAK(); \
    }\
}

#else

#define SF_LOG_VERB(msg, ...) sf::log_verbose(msg, ##__VA_ARGS__)
#define SF_LOG_INFO(msg, ...) sf::log_info(msg, ##__VA_ARGS__)
#define SF_LOG_DBG(msg, ...) sf::log_debug(msg, ##__VA_ARGS__)
#define SF_LOG_WARN(msg, ...) sf::log_warning(msg, ##__VA_ARGS__)
#define SF_LOG_ERR(msg, ...) sf::log_error(SF_FILENAME, __FUNCTION__, __LINE__, msg, ##__VA_ARGS__)
#define SF_LOG_ASSERT(x, msg, ...) \
{                                \
    if (!(x)) {                  \
        sf::log_assert(SF_FILENAME, __FUNCTION__, __LINE__, msg, ##__VA_ARGS__); \
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

    struct SF_API log_allocator_t final {
        void* allocate(usize size, usize alignment = SF_ALIGNMENT);
        void deallocate(void* addr);
    };

    inline FILE* g_log_file = nullptr;
    inline thread_pool_t<log_allocator_t> g_log_thread_pool = {};

    SF_API void log_file_open(const char* filepath);
    SF_API void log_file_close();
    SF_API void log_file_write(const char* log);

    SF_API void log_console_verbose(SF_LOG_COLOR log_color, char* log);
    SF_API void log_console_info(SF_LOG_COLOR log_color, char* log);
    SF_API void log_console_debug(SF_LOG_COLOR log_color, char* log);
    SF_API void log_console_warning(SF_LOG_COLOR log_color, char* log);
    SF_API void log_console_error(SF_LOG_COLOR log_color, char* log);
    SF_API void log_console_assert(SF_LOG_COLOR log_color, char* log);

    template<typename... Args>
    static void log_verbose(const char* msg, Args &&... args) {
        task_t task_log;
        task_log.args = nullptr;
        task_log.function = [msg, args...](void* task_args) {
            char fmt_buffer[256] = {};
            char text_buffer[256] = {};
            date_time_t date_time = date_time_get_current();
#if defined(__LP64__)
            const char *fmt = "\n[%d.%d.%d][%ld:%ld:%ld.%ld][VERBOSE] %s";
#else
            const char* fmt = "\n[%d.%d.%d][%lld:%lld:%lld.%lld] %s";
#endif
            sprintf(
                    fmt_buffer,
                    fmt,
                    date_time.d, date_time.m, date_time.y, date_time.h, date_time.min, date_time.s, date_time.ms,
                    msg
            );
            sprintf(text_buffer, fmt_buffer, args...);
            log_console_verbose(SF_LOG_COLOR_LIGHT_GREEN, text_buffer);
            log_file_write(text_buffer);
        };
    }

    template<typename... Args>
    static void log_info(const char* msg, Args &&... args) {
        task_t task_log;
        task_log.args = nullptr;
        // TODO: need to figure out how to pass a function by lambda with input list params and args expansion
        auto function = [=](void* task_args) {
            char fmt_buffer[256] = {};
            char text_buffer[256] = {};
            date_time_t date_time = date_time_get_current();
#if defined(__LP64__)
            const char *fmt = "\n[%d.%d.%d][%ld:%ld:%ld.%ld][INFO] %s";
#else
            const char* fmt = "\n[%d.%d.%d][%lld:%lld:%lld.%lld] %s";
#endif
            sprintf(
                    fmt_buffer,
                    fmt,
                    date_time.d, date_time.m, date_time.y, date_time.h, date_time.min, date_time.s, date_time.ms,
                    msg
            );
            sprintf(text_buffer, fmt_buffer, args...);
            log_console_verbose(SF_LOG_COLOR_GREEN, text_buffer);
            log_file_write(text_buffer);
        };
        task_log.function = function();
    }

    template<typename... Args>
    static void log_debug(const char* msg, Args &&... args) {
        task_t task_log;
        task_log.args = nullptr;
        task_log.function = [msg, args...](void* task_args) {
            char fmt_buffer[256] = {};
            char text_buffer[256] = {};
            date_time_t date_time = date_time_get_current();
#if defined(__LP64__)
            const char *fmt = "\n[%d.%d.%d][%ld:%ld:%ld.%ld][DEBUG] %s";
#else
            const char* fmt = "\n[%d.%d.%d][%lld:%lld:%lld.%lld] %s";
#endif
            sprintf(
                    fmt_buffer,
                    fmt,
                    date_time.d, date_time.m, date_time.y, date_time.h, date_time.min, date_time.s, date_time.ms,
                    msg
            );
            sprintf(text_buffer, fmt_buffer, args...);
            log_console_verbose(SF_LOG_COLOR_WHITE, text_buffer);
            log_file_write(text_buffer);
        };
    }

    template<typename... Args>
    static void log_warning(const char* msg, Args &&... args) {
        task_t task_log;
        task_log.args = nullptr;
        task_log.function = [msg, args...](void* task_args) {
            char fmt_buffer[256] = {};
            char text_buffer[256] = {};
            date_time_t date_time = date_time_get_current();
#if defined(__LP64__)
            const char *fmt = "\n[%d.%d.%d][%ld:%ld:%ld.%ld][WARNING] %s";
#else
            const char* fmt = "\n[%d.%d.%d][%lld:%lld:%lld.%lld] %s";
#endif
            sprintf(
                    fmt_buffer,
                    fmt,
                    date_time.d, date_time.m, date_time.y, date_time.h, date_time.min, date_time.s, date_time.ms,
                    msg
            );
            sprintf(text_buffer, fmt_buffer, args...);
            log_console_verbose(SF_LOG_COLOR_YELLOW, text_buffer);
            log_file_write(text_buffer);
        };
    }

    template<typename... Args>
    static void log_error(const char* filename, const char* function, int line, const char* msg, Args &&... args) {
        task_t task_log;
        task_log.args = nullptr;
        task_log.function = [filename, function, line, msg, args...](void* task_args) {
            char fmt_buffer[256] = {};
            char text_buffer[256] = {};
            date_time_t date_time = date_time_get_current();
#if defined(__LP64__)
            const char *fmt = "\n[%d.%d.%d][%ld:%ld:%ld.%ld][ERROR] log_error in %s -> %s(%i line):\n%s";
#else
            const char* fmt = "\n[%d.%d.%d][%lld:%lld:%lld.%lld] Error in %s -> %s(%i line):\n%s";
#endif
            sprintf(
                    fmt_buffer,
                    fmt,
                    date_time.d, date_time.m, date_time.y, date_time.h, date_time.min, date_time.s, date_time.ms,
                    filename, function, line,
                    msg
            );
            sprintf(text_buffer, fmt_buffer, args...);
            log_console_error(SF_LOG_COLOR_RED, text_buffer);
            log_file_write(text_buffer);
        };
    }

    template<typename... Args>
    static void log_assert(const char* filename, const char* function, int line, const char* msg, Args &&... args) {
        task_t task_log;
        task_log.args = nullptr;
        task_log.function = [filename, function, line, msg, args...](void* task_args) {
            char fmt_buffer[256] = {};
            char text_buffer[256] = {};
            date_time_t date_time = date_time_get_current();
#if defined(__LP64__)
            const char *fmt = "\n[%d.%d.%d][%ld:%ld:%ld.%ld][ASSERTION] Assertion Failed in %s -> %s(%i line):\n%s";
#else
            const char* fmt = "\n[%d.%d.%d][%lld:%lld:%lld.%lld] Assertion Failed in %s -> %s(%i line):\n%s";
#endif
            sprintf(
                    fmt_buffer,
                    fmt,
                    date_time.d, date_time.m, date_time.y, date_time.h, date_time.min, date_time.s, date_time.ms,
                    filename, function, line,
                    msg
            );
            sprintf(text_buffer, fmt_buffer, args...);
            log_console_error(SF_LOG_COLOR_RED, text_buffer);
            log_file_write(text_buffer);
        };
    }

}