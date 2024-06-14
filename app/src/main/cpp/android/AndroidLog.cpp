#include <Log.hpp>
#include <android/log.h>

void __PrintLog(T3D_LOG_LEVEL log_level, T3D_LOG_COLOR log_color, char* log) {
    switch (log_level) {

        case T3D_LOG_VERBOSE:
        {
            __android_log_buf_write(LOG_ID_MAIN, ANDROID_LOG_VERBOSE, "T3D", log);
            break;
        }

        case T3D_LOG_INFO:
        {
            __android_log_buf_write(LOG_ID_MAIN, ANDROID_LOG_INFO, "T3D", log);
            break;
        }

        case T3D_LOG_DEBUG:
        {
            __android_log_buf_write(LOG_ID_MAIN, ANDROID_LOG_DEBUG, "T3D", log);
            break;
        }

        case T3D_LOG_WARNING:
        {
            __android_log_buf_write(LOG_ID_MAIN, ANDROID_LOG_WARN, "T3D", log);
            break;
        }

        case T3D_LOG_ERROR:
        {
            __android_log_buf_write(LOG_ID_MAIN, ANDROID_LOG_ERROR, "T3D", log);
            break;
        }

        case T3D_LOG_ASSERTION:
        {
            __android_log_buf_write(LOG_ID_MAIN, ANDROID_LOG_FATAL, "T3D", log);
            break;
        }

    }
}