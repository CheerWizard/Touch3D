#pragma once

#include <ThreadPool.hpp>
#include <EventBuffer.hpp>
#include <Activity.hpp>

#include <android/native_window.h>
#include <android/input.h>
#include <android/rect.h>
#include <android/configuration.h>
#include <android/looper.h>
#include <android/native_activity.h>

class AndroidApp final {

public:
    AndroidApp(jobject activity);
    ~AndroidApp();

    void Run();

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

private:
    void UpdateWindow();
    void UpdateEvents();

private:
    bool m_running;
    EventBuffer m_event_buffer;
    ThreadPool* m_thread_pool;

    Activity* m_activity;
    ALooper* m_looper;
    AConfiguration* m_config;

    ANativeWindow* m_window;
    ANativeWindow* m_pending_window;

    AInputQueue* m_input_queue;
    AInputQueue* m_pending_input_queue;

    ARect m_content_rect;
    ARect m_pending_content_rect;

};