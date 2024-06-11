#pragma once

#include <Types.hpp>

#include <android/Jni.hpp>

enum Lifecycle {
    T3D_LIFECYCLE_CREATE,
    T3D_LIFECYCLE_START,
    T3D_LIFECYCLE_RESUME,
    T3D_LIFECYCLE_PAUSE,
    T3D_LIFECYCLE_STOP,
    T3D_LIFECYCLE_DESTROY,
};

class Activity final {

public:
    static jclass clazz;
    static jmethodID mid_set_window_flags;
    static jmethodID mid_set_window_format;
    static jmethodID mid_show_input;
    static jmethodID mid_hide_input;

public:
    Lifecycle lifecycle;

    Activity(jobject thiz);
    ~Activity();

    bool IsOpen() const;

    void SetWindowFlags(int flags, int mask);
    void SetWindowFormat(int format);

    void ShowInput(int mode);
    void HideInput(int mode);

private:
    jobject m_this;
};