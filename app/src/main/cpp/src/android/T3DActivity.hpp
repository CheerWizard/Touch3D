#pragma once

#include <Types.hpp>

#include <jni.h>

struct MainActivitySavedState {
    void* state;
    usize size;
};

enum Lifecycle {
    T3D_LIFECYCLE_CREATE,
    T3D_LIFECYCLE_START,
    T3D_LIFECYCLE_RESUME,
    T3D_LIFECYCLE_PAUSE,
    T3D_LIFECYCLE_STOP,
    T3D_LIFECYCLE_DESTROY,
};

class T3DActivity final {

public:

    static jint Load(JavaVM* vm);
    static void Unload(JavaVM* vm);

    Lifecycle lifecycle;
    MainActivitySavedState saved_state;
    void* user_data;

    T3DActivity(jobject thiz);
    ~T3DActivity();

    bool IsOpen() const;

    void SetWindowFlags(int flags, int mask);
    void SetWindowFormat(int format);

    void ShowInput(int mode);
    void HideInput(int mode);

private:
    static JNIEnv* GetEnv();

private:
    static JavaVM* s_vm;
    static jclass s_class;
    static jmethodID s_set_window_flags;
    static jmethodID s_set_window_format;
    static jmethodID s_show_input;
    static jmethodID s_hide_input;

    jobject m_this;
};