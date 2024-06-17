#pragma once

#include <Types.hpp>

#include <Jni.hpp>

class Activity {

public:
    static jclass clazz;

    static jmethodID mid_set_window_flags;
    static jmethodID mid_set_window_format;
    static jmethodID mid_show_input;
    static jmethodID mid_hide_input;

public:
    Activity(jobject thiz);
    ~Activity();

    void SetWindowFlags(int flags, int mask);
    void SetWindowFormat(int format);

    void ShowInput(int mode);
    void HideInput(int mode);

private:
    jobject m_this;
};