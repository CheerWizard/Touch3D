#include <Log.hpp>
#include <Windows.h>

void __PrintLog(T3D_LOG_LEVEL logLevel, char* log) {
    switch (logLevel) {

        case T3D_LOG_VERBOSE: {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), T3D_LOG_LIGHT_GREEN);
            puts(log);
            break;
        }

        case T3D_LOG_INFO: {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), T3D_LOG_GREEN);
            puts(log);
            break;
        }

        case T3D_LOG_DEBUG: {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), T3D_LOG_WHITE);
            puts(log);
            break;
        }

        case T3D_LOG_WARNING: {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), T3D_LOG_YELLOW);
            puts(log);
            break;
        }

        case T3D_LOG_ERROR: {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), T3D_LOG_RED);
            puts(log);
            break;
        }

        case T3D_LOG_ASSERTION: {
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), T3D_LOG_RED);
            puts(log);
            break;
        }

    }
}