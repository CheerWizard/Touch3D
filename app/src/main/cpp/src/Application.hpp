#pragma once

#include <Window.hpp>

#if defined(T3D_WINDOWS)

#elif defined(T3D_ANDROID)

#include <poll.h>
#include <pthread.h>
#include <sched.h>

#include "src/android/T3DActivity.hpp"
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

    virtual void OnCreate();
    virtual void OnStart();
    virtual void OnResume();
    virtual void OnPause();
    virtual void OnStop();
    virtual void OnDestroy();

    virtual MainActivitySavedState OnSaveInstanceState();
    virtual void OnRestoreInstanceState(const MainActivitySavedState& saved_state);

    virtual void OnConfigurationChanged();

    virtual void OnLowMemory();

    virtual void OnWindowFocusChanged();

    virtual void OnSurfaceCreated(void* surface);
    virtual void OnSurfaceChanged(void* surface, int format, int w, int h);
    virtual void OnSurfaceRedrawNeeded(void* surface);
    virtual void OnSurfaceDestroyed();

    virtual void OnInputQueueCreated(void* input_queue);
    virtual void OnInputQueueDestroyed(void* input_queue);

    virtual void OnContentRectChanged(int x, int y, int w, int h);

protected:
    Window* m_window;
    IOBuffer m_io_buffer;

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