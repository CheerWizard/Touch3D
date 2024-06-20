#include <Log.hpp>

FILE* Log::s_file = nullptr;
std::mutex Log::s_mutex;
ThreadPool<1, 10, T3D_THREAD_PRIORITY_HIGHEST>* Log::s_thread_pool = nullptr;

void Log::Open(const char* filepath) {
    s_thread_pool = new ThreadPool<1, 10, T3D_THREAD_PRIORITY_HIGHEST>("Log");
    s_file = fopen(filepath, "w+");
    if (s_file == nullptr) {
        printf("Unable to open Log file %s", filepath);
        T3D_DEBUG_BREAK();
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