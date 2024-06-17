#include <Log.hpp>
#include <android/log.h>

void Log::PrintVerbose(T3D_LOG_COLOR log_color, char *log) {
    __android_log_buf_write(LOG_ID_MAIN, ANDROID_LOG_VERBOSE, "T3D", log);
}

void Log::PrintInfo(T3D_LOG_COLOR log_color, char *log) {
    __android_log_buf_write(LOG_ID_MAIN, ANDROID_LOG_INFO, "T3D", log);
}

void Log::PrintDebug(T3D_LOG_COLOR log_color, char *log) {
    __android_log_buf_write(LOG_ID_MAIN, ANDROID_LOG_DEBUG, "T3D", log);
}

void Log::PrintWarning(T3D_LOG_COLOR log_color, char *log) {
    __android_log_buf_write(LOG_ID_MAIN, ANDROID_LOG_WARN, "T3D", log);
}

void Log::PrintError(T3D_LOG_COLOR log_color, char *log) {
    __android_log_buf_write(LOG_ID_MAIN, ANDROID_LOG_ERROR, "T3D", log);
}

void Log::PrintAssert(T3D_LOG_COLOR log_color, char *log) {
    __android_log_buf_write(LOG_ID_MAIN, ANDROID_LOG_FATAL, "T3D", log);
}