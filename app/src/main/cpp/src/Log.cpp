#include <Log.hpp>

FILE* Log::s_file = nullptr;
std::mutex Log::s_mutex;
ThreadPool<10>* Log::s_thread_pool = nullptr;

void Log::Open(const char* filepath) {
    s_thread_pool = new ThreadPool<10>(1, "Log", T3D_THREAD_PRIORITY_HIGHEST);
    fopen_s(&s_file, filepath, "w+");
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
    delete s_thread_pool;
}

void Log::PrintFile(char *log) {
    s_mutex.lock();
    if (s_file != nullptr) {
        fputs(log, s_file);
    }
    s_mutex.unlock();
}