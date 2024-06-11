#include <android/Activity.hpp>
#include <Log.hpp>

Activity::Activity(jobject thiz) {
    JNIEnv* env = Jni::Get();
    m_this = env->NewGlobalRef(thiz);
    env->DeleteLocalRef(thiz);
}

Activity::~Activity() {
    Jni::Get()->DeleteGlobalRef(m_this);
}

bool Activity::IsOpen() const {
    return lifecycle != T3D_LIFECYCLE_DESTROY;
}

void Activity::SetWindowFlags(int flags, int mask) {
    Jni::Get()->CallVoidMethod(m_this, s_set_window_flags, flags, mask);
}

void Activity::SetWindowFormat(int format) {
    Jni::Get()->CallVoidMethod(m_this, s_set_window_format, format);
}

void Activity::ShowInput(int mode) {
    Jni::Get()->CallVoidMethod(m_this, s_show_input, mode);
}

void Activity::HideInput(int mode) {
    Jni::Get()->CallVoidMethod(m_this, s_hide_input, mode);
}