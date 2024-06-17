#include <Log.hpp>

FILE* Log::s_file = nullptr;
std::mutex Log::s_mutex;
char Log::s_buffer[256] = {};

void Log::Open(const char* filepath) {
    s_file = fopen(filepath, "wd");
    if (s_file == nullptr) {
        printf("Unable to open Log file %s", filepath);
        T3D_DEBUGBREAK();
    }
    Info("Log file %s is open.", filepath);
}

void Log::Close() {
    Info("Log file is closing...");
    fflush(s_file);
    fclose(s_file);
}