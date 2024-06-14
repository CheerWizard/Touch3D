#pragma once

#include <Window.hpp>

#if defined(T3D_WINDOWS)

#elif defined(T3D_ANDROID)

#include <poll.h>
#include <pthread.h>
#include <sched.h>

#include <android/Activity.hpp>
#include <android/configuration.h>
#include <android/looper.h>
#include <android/native_activity.h>

#endif

class Application final {

public:

#if defined(T3D_ANDROID)

    Application(T3DActivity* main_activity);

#elif defined(T3D_WINDOWS)

    Application(HINSTANCE instance, int cmd_show);

#else

    Application();

#endif

    ~Application();

    void RunLoop();

#if defined(T3D_ANDROID)

    void OnCreate();
    void OnStart();
    void OnResume();
    void OnPause();
    void OnStop();
    void OnDestroy();

    void OnConfigurationChanged();

    void OnLowMemory();

    void OnWindowFocusChanged();

    void OnSurfaceCreated(void* surface);
    void OnSurfaceChanged(void* surface, int format, int w, int h);
    void OnSurfaceRedrawNeeded(void* surface);
    void OnSurfaceDestroyed();

    void OnInputQueueCreated(void* input_queue);
    void OnInputQueueDestroyed(void* input_queue);

    void OnContentRectChanged(int x, int y, int w, int h);

#endif

protected:
    Window* m_window;
    IOBuffer m_io_buffer;
    bool m_running;

#if defined(T3D_ANDROID)
    T3DActivity* m_main_activity;
    ALooper* m_looper;
    AConfiguration* m_config;
    pthread_mutex_t m_mutex;
    pthread_cond_t m_cond;
    pthread_t m_thread;
    int m_msg_read;
    int m_msg_write;
#endif

};