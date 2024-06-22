#include <sf_log.hpp>

namespace sf {

    FILE* Log::s_file = nullptr;
    ThreadPool<1, 10, SF_THREAD_PRIORITY_HIGHEST>* Log::s_thread_pool = nullptr;

    void Log::open(const char* filepath) {
        s_thread_pool = new ThreadPool<1, 10, SF_THREAD_PRIORITY_HIGHEST>("Log");
        s_thread_pool->add([filepath]() {
            s_file = fopen(filepath, "w+");
            if (s_file == nullptr) {
                printf("Unable to open Log file %s", filepath);
                SF_DEBUG_BREAK();
            }
            log_info("Log file %s is open.", filepath);
        });
    }

    void Log::close() {
        s_thread_pool->add([]() {
            log_info("Log file is closing...");
            fflush(s_file);
            fclose(s_file);
            delete s_thread_pool;
        });
    }

    void Log::write_file(char *log) {
        if (s_file != nullptr) {
            fputs(log, s_file);
        }
    }

}

// SF_WINDOWS_BEGIN
#if defined(SF_WINDOWS)

#include <windows.h>

namespace sf {

    static const int SF_LOG_COLOR_CODE[SF_LOG_COLOR_COUNT] {
        0x00, // SF_LOG_COLOR_BLACK
        0x01, // SF_LOG_COLOR_BLUE
        0x02, // SF_LOG_COLOR_GREEN
        0x03, // SF_LOG_COLOR_AQUA
        0x04, // SF_LOG_COLOR_RED
        0x05, // SF_LOG_COLOR_PURPLE
        0x06, // SF_LOG_COLOR_YELLOW
        0x07, // SF_LOG_COLOR_WHITE
        0x08, // SF_LOG_COLOR_GRAY
        0x09, // SF_LOG_COLOR_LIGHT_BLUE
        0x0A, // SF_LOG_COLOR_LIGHT_GREEN
        0x0B, // SF_LOG_COLOR_LIGHT_AQUA
        0x0C, // SF_LOG_COLOR_LIGHT_RED
        0x0D, // SF_LOG_COLOR_LIGHT_PURPLE
        0x0E, // SF_LOG_COLOR_LIGHT_YELLOW
        0x0F, // SF_LOG_COLOR_LIGHT_WHITE
    };

    static void print(SF_LOG_COLOR log_color, char* log) {
        HANDLE std_out = GetStdHandle(STD_OUT_HANDLE);
        if (std_out != nullptr && std_out != INVALID_HANDLE_VALUE) {
            DWORD written = 0;
            SetConsoleTextAttribute(std_out, SF_LOG_COLOR_CODE[log_color]);
            WriteConsoleA(std_out, log, strlen(log), &written, nullptr);
        }
    }

    void Log::print_verbose(SF_LOG_COLOR log_color, char* log) {
        print(log_color, log);
    }

    void Log::print_info(SF_LOG_COLOR log_color, char* log) {
        print(log_color, log);
    }

    void Log::print_debug(SF_LOG_COLOR log_color, char* log) {
        print(log_color, log);
    }

    void Log::print_warning(SF_LOG_COLOR log_color, char* log) {
        print(log_color, log);
    }

    void Log::print_error(SF_LOG_COLOR log_color, char* log) {
        print(log_color, log);
    }

    void Log::print_assert(SF_LOG_COLOR log_color, char* log) {
        print(log_color, log);
    }

}

#endif
// SF_WINDOWS_END

// SF_LINUX_BEGIN
#if defined(SF_LINUX)

namespace sf {

    static const char* SF_LOG_COLOR_CODE[SF_LOG_COLOR_COUNT] {
        "\x1b[30m", // SF_LOG_COLOR_BLACK
        "\x1b[34m", // SF_LOG_COLOR_BLUE
        "\x1b[32m", // SF_LOG_COLOR_GREEN
        "\x1b[36m", // SF_LOG_COLOR_AQUA
        "\x1b[31m", // SF_LOG_COLOR_RED
        "\x1b[35m", // SF_LOG_COLOR_PURPLE
        "\x1b[33m", // SF_LOG_COLOR_YELLOW
        "\x1b[37m", // SF_LOG_COLOR_WHITE
        "\x1b[90m", // SF_LOG_COLOR_GRAY
        "\x1b[94m", // SF_LOG_COLOR_LIGHT_BLUE
        "\x1b[92m", // SF_LOG_COLOR_LIGHT_GREEN
        "\x1b[96m", // SF_LOG_COLOR_LIGHT_AQUA
        "\x1b[91m", // SF_LOG_COLOR_LIGHT_RED
        "\x1b[95m", // SF_LOG_COLOR_LIGHT_PURPLE
        "\x1b[93m", // SF_LOG_COLOR_LIGHT_YELLOW
        "\x1b[97m", // SF_LOG_COLOR_LIGHT_WHITE
    };

    void Log::print_verbose(SF_LOG_COLOR log_color, char* log) {
        char fmt_buffer[256] = {};
        sprintf(fmt_buffer, "%s %s \033[0m", SF_LOG_COLOR_CODE[log_color], log);
        puts(fmt_buffer);
    }

    void Log::print_info(SF_LOG_COLOR log_color, char* log) {
        char fmt_buffer[256] = {};
        sprintf(fmt_buffer, "%s %s \033[0m", SF_LOG_COLOR_CODE[log_color], log);
        puts(fmt_buffer);
    }

    void Log::print_debug(SF_LOG_COLOR log_color, char* log) {
        char fmt_buffer[256] = {};
        sprintf(fmt_buffer, "%s %s \033[0m", SF_LOG_COLOR_CODE[log_color], log);
        puts(fmt_buffer);
    }

    void Log::print_warning(SF_LOG_COLOR log_color, char* log) {
        char fmt_buffer[256] = {};
        sprintf(fmt_buffer, "%s %s \033[0m", SF_LOG_COLOR_CODE[log_color], log);
        puts(fmt_buffer);
    }

    void Log::print_error(SF_LOG_COLOR log_color, char* log) {
        char fmt_buffer[256] = {};
        sprintf(fmt_buffer, "%s %s \033[0m", SF_LOG_COLOR_CODE[log_color], log);
        puts(fmt_buffer);
    }

    void Log::print_assert(SF_LOG_COLOR log_color, char* log) {
        char fmt_buffer[256] = {};
        sprintf(fmt_buffer, "%s %s \033[0m", SF_LOG_COLOR_CODE[log_color], log);
        puts(fmt_buffer);
    }

}

#endif
// SF_LINUX_END

// SF_ANDROID_BEGIN
#if defined(SF_ANDROID)

#include <android/log.h>

namespace sf {

    void Log::print_verbose(SF_LOG_COLOR log_color, char *log) {
        __android_log_buf_write(LOG_ID_MAIN, ANDROID_LOG_VERBOSE, "SF_ANDROID", log);
    }

    void Log::print_info(SF_LOG_COLOR log_color, char *log) {
        __android_log_buf_write(LOG_ID_MAIN, ANDROID_LOG_INFO, "SF_ANDROID", log);
    }

    void Log::print_debug(SF_LOG_COLOR log_color, char *log) {
        __android_log_buf_write(LOG_ID_MAIN, ANDROID_LOG_DEBUG, "SF_ANDROID", log);
    }

    void Log::print_warning(SF_LOG_COLOR log_color, char *log) {
        __android_log_buf_write(LOG_ID_MAIN, ANDROID_LOG_WARN, "SF_ANDROID", log);
    }

    void Log::print_error(SF_LOG_COLOR log_color, char *log) {
        __android_log_buf_write(LOG_ID_MAIN, ANDROID_LOG_ERROR, "SF_ANDROID", log);
    }

    void Log::print_assert(SF_LOG_COLOR log_color, char *log) {
        __android_log_buf_write(LOG_ID_MAIN, ANDROID_LOG_FATAL, "SF_ANDROID", log);
    }

}

#endif // SF_ANDROID_END