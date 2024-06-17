#include <Activity.hpp>
#include <Log.hpp>

jclass Activity::clazz;
jmethodID Activity::mid_set_window_flags;
jmethodID Activity::mid_set_window_format;
jmethodID Activity::mid_show_input;
jmethodID Activity::mid_hide_input;

Activity::Activity(jobject thiz) {
    JNIEnv* env = Jni::Get();
    m_this = env->NewGlobalRef(thiz);
    env->DeleteLocalRef(thiz);
}

Activity::~Activity() {
    Jni::Get()->DeleteGlobalRef(m_this);
}

void Activity::SetWindowFlags(int flags, int mask) {
    Jni::Get()->CallVoidMethod(m_this, mid_set_window_flags, flags, mask);
}

void Activity::SetWindowFormat(int format) {
    Jni::Get()->CallVoidMethod(m_this, mid_set_window_format, format);
}

void Activity::ShowInput(int mode) {
    Jni::Get()->CallVoidMethod(m_this, mid_show_input, mode);
}

void Activity::HideInput(int mode) {
    Jni::Get()->CallVoidMethod(m_this, mid_hide_input, mode);
}