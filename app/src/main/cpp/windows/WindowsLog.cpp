#include <Log.hpp>
#include <Windows.h>

static const int T3D_LOG_COLOR_CODE[T3D_LOG_COLOR_COUNT] {
    0x00, // T3D_LOG_COLOR_BLACK
    0x01, // T3D_LOG_COLOR_BLUE
    0x02, // T3D_LOG_COLOR_GREEN
    0x03, // T3D_LOG_COLOR_AQUA
    0x04, // T3D_LOG_COLOR_RED
    0x05, // T3D_LOG_COLOR_PURPLE
    0x06, // T3D_LOG_COLOR_YELLOW
    0x07, // T3D_LOG_COLOR_WHITE
    0x08, // T3D_LOG_COLOR_GRAY
    0x09, // T3D_LOG_COLOR_LIGHT_BLUE
    0x0A, // T3D_LOG_COLOR_LIGHT_GREEN
    0x0B, // T3D_LOG_COLOR_LIGHT_AQUA
    0x0C, // T3D_LOG_COLOR_LIGHT_RED
    0x0D, // T3D_LOG_COLOR_LIGHT_PURPLE
    0x0E, // T3D_LOG_COLOR_LIGHT_YELLOW
    0x0F, // T3D_LOG_COLOR_LIGHT_WHITE
};

void Log::PrintVerbose(T3D_LOG_COLOR log_color, char* log) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), T3D_LOG_COLOR_CODE[log_color]);
    puts(log);
}

void Log::PrintInfo(T3D_LOG_COLOR log_color, char* log) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), T3D_LOG_COLOR_CODE[log_color]);
    puts(log);
}

void Log::PrintDebug(T3D_LOG_COLOR log_color, char* log) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), T3D_LOG_COLOR_CODE[log_color]);
    puts(log);
}

void Log::PrintWarning(T3D_LOG_COLOR log_color, char* log) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), T3D_LOG_COLOR_CODE[log_color]);
    puts(log);
}

void Log::PrintError(T3D_LOG_COLOR log_color, char* log) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), T3D_LOG_COLOR_CODE[log_color]);
    puts(log);
}

void Log::PrintAssert(T3D_LOG_COLOR log_color, char* log) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), T3D_LOG_COLOR_CODE[log_color]);
    puts(log);
}