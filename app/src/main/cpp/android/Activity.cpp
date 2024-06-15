#include <Activity.hpp>
#include <Log.hpp>

Activity::Activity(jobject thiz) {
    JNIEnv* env = Jni::Get();
    m_this = env->NewGlobalRef(thiz);
    env->DeleteLocalRef(thiz);
}

Activity::~Activity() {
    Jni::Get()->DeleteGlobalRef(m_this);
}

jclass MainActivity::clazz;
jmethodID MainActivity::mid_set_window_flags;
jmethodID MainActivity::mid_set_window_format;
jmethodID MainActivity::mid_show_input;
jmethodID MainActivity::mid_hide_input;

void MainActivity::SetWindowFlags(int flags, int mask) {
    Jni::Get()->CallVoidMethod(m_this, mid_set_window_flags, flags, mask);
}

void MainActivity::SetWindowFormat(int format) {
    Jni::Get()->CallVoidMethod(m_this, mid_set_window_format, format);
}

void MainActivity::ShowInput(int mode) {
    Jni::Get()->CallVoidMethod(m_this, mid_show_input, mode);
}

void MainActivity::HideInput(int mode) {
    Jni::Get()->CallVoidMethod(m_this, mid_hide_input, mode);
}