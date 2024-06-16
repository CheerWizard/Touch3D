#pragma once

#include <IOBuffer.hpp>

#if defined(T3D_WINDOWS)

#include <Windows.h>

#elif defined(T3D_ANDROID)

#include <Activity.hpp>
#include <android/native_window.h>
#include <android/input.h>
#include <android/rect.h>
#include <android/configuration.h>
#include <android/looper.h>
#include <android/native_activity.h>

#elif defined(T3D_LINUX)

#include <X11/Xlib.h>
#include <err.h>

#endif

class Window final {

public:

#if defined(T3D_WINDOWS)

    Window(HINSTANCE handle, int cmd_show, const char* title, int2 position, int2 size);

#elif defined(T3D_ANDROID)

    Window(Activity* activity);

#else

    Window();

#endif

    ~Window();

    void SetIOBuffer(IOBuffer* ioBuffer);
    IOBuffer* GetIOBuffer();

    void Update();
    bool IsOpen() const;

#if defined(T3D_DESKTOP)

    bool IsWindowed();
    bool IsFullscreen();

    [[nodiscard]] inline int2 GetSize() const {
        return m_size;
    }

    [[nodiscard]] inline u32 GetRefreshRate() const {
        return m_refresh_rate;
    }

#endif

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

private:

#if defined(T3D_DESKTOP)

    const char* m_title;
    int2 m_position;
    int2 m_size;
    u32 m_refresh_rate;

#endif

#if defined(T3D_WINDOWS)

    HWND m_window;

#elif defined(T3D_ANDROID)

    Activity* m_activity;
    ALooper* m_looper;
    AConfiguration* m_config;
    pthread_mutex_t m_mutex;
    pthread_cond_t m_cond;
    pthread_t m_thread;
    int m_msg_read;
    int m_msg_write;

    ANativeWindow* m_window;
    ANativeWindow* m_pending_window;

    AInputQueue* m_input_queue;
    AInputQueue* m_pending_input_queue;

    ARect m_content_rect;
    ARect m_pending_content_rect;

#elif defined(T3D_LINUX)

    Display* m_display;
    Window m_root;
    int m_screen;

#endif
};