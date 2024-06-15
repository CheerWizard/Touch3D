#include <Log.hpp>

static const char* T3D_LOG_COLOR_CODE[T3D_LOG_COLOR_COUNT] {
        "\x1b[30m", // T3D_LOG_COLOR_BLACK
        "\x1b[34m", // T3D_LOG_COLOR_BLUE
        "\x1b[32m", // T3D_LOG_COLOR_GREEN
        "\x1b[36m", // T3D_LOG_COLOR_AQUA
        "\x1b[31m", // T3D_LOG_COLOR_RED
        "\x1b[35m", // T3D_LOG_COLOR_PURPLE
        "\x1b[33m", // T3D_LOG_COLOR_YELLOW
        "\x1b[37m", // T3D_LOG_COLOR_WHITE
        "\x1b[90m", // T3D_LOG_COLOR_GRAY
        "\x1b[94m", // T3D_LOG_COLOR_LIGHT_BLUE
        "\x1b[92m", // T3D_LOG_COLOR_LIGHT_GREEN
        "\x1b[96m", // T3D_LOG_COLOR_LIGHT_AQUA
        "\x1b[91m", // T3D_LOG_COLOR_LIGHT_RED
        "\x1b[95m", // T3D_LOG_COLOR_LIGHT_PURPLE
        "\x1b[93m", // T3D_LOG_COLOR_LIGHT_YELLOW
        "\x1b[97m", // T3D_LOG_COLOR_LIGHT_WHITE
};

void __PrintLog(T3D_LOG_LEVEL log_level, T3D_LOG_COLOR log_color, char* log) {
    char fmt_buffer[256] = {};
    sprintf(fmt_buffer, "%s %s \033[0m", T3D_LOG_COLOR_CODE[log_color], log);
    puts(fmt_buffer);
}