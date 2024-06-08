#pragma once

#include <Types.hpp>

#include <jni.h>

struct MainActivitySavedState {
    void* state;
    usize size;
};

class MainActivity final {

public:
    MainActivitySavedState saved_state;
    void* user_data;

    MainActivity(JNIEnv* env, jobject handle);

    void OnCreate();
    void OnStart();
    void OnResume();
    void OnPause();
    void OnStop();
    void OnDestroy();

    void SetWindowFlags(int flags, int mask);
    void SetWindowFormat(int format);
    void ShowIMM(int mode);
    void HideIMM(int mode);

private:
    JNIEnv* m_env;
    jobject m_this;
    jclass m_class;

    jmethodID m_on_create;
    jmethodID m_on_start;
    jmethodID m_on_resume;
    jmethodID m_on_pause;
    jmethodID m_on_stop;
    jmethodID m_on_destroy;

    jmethodID m_set_window_flags;
    jmethodID m_set_window_format;
    jmethodID m_show_imm;
    jmethodID m_hide_imm;
};