#pragma once

#include <Window.hpp>

#if defined(T3D_WINDOWS)

#elif defined(T3D_ANDROID)

#include <poll.h>
#include <pthread.h>
#include <sched.h>

#include <android/MainActivity.hpp>
#include <android/configuration.h>
#include <android/looper.h>
#include <android/native_activity.h>

#endif

enum AppState {
    T3D_APP_CREATED,
    T3D_APP_STARTED,
    T3D_APP_RESUMED,
    T3D_APP_PAUSED,
    T3D_APP_STOPPED,
    T3D_APP_DESTROYED,

    T3D_APP_REDRAW_NEEDED,
    T3D_APP_STATE_SAVED,

    T3D_APP_RUNNING,
};

class Application final {

public:

#if defined(T3D_ANDROID)

    Application(MainActivity* main_activity);

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

    int OnInputEvent(AInputEvent* event);
    int32_t (*onInputEvent)(AInputEvent* event);

protected:
    int m_state;
    Window* m_window;
    IOBuffer m_io_buffer;

#if defined(T3D_ANDROID)
    MainActivity* m_main_activity;
    ANativeWindow* m_window;
    ANativeWindow* m_pending_window;
    ALooper* m_looper;
    AConfiguration* m_config;
    AInputQueue* m_input_queue;
    AInputQueue* m_pending_input_queue;
    ARect m_content_rect;
    ARect m_pending_content_rect;
    pthread_mutex_t m_mutex;
    pthread_cond_t m_cond;
    pthread_t m_thread;
    int m_msg_read;
    int m_msg_write;
#endif

};