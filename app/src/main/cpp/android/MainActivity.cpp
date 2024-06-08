#include <android/MainActivity.hpp>

MainActivity::MainActivity(JNIEnv* env, jobject handle) : m_env(env), m_this(handle) {
    m_class = env->FindClass("com/cheerwizard/touch3d/MainActivity");

    m_on_create = env->GetMethodID(m_class, "onCreate", "void(V)");
    m_on_start = env->GetMethodID(m_class, "onStart", "void(V)");
    m_on_resume = env->GetMethodID(m_class, "onResume", "void(V)");
    m_on_pause = env->GetMethodID(m_class, "onPause", "void(V)");
    m_on_stop = env->GetMethodID(m_class, "onStop", "void(V)");
    m_on_destroy = env->GetMethodID(m_class, "onDestroy", "void(V)");

    m_set_window_flags = env->GetMethodID(m_class, "setWindowFlags", "void(Int, Int)");
    m_set_window_format = env->GetMethodID(m_class, "setWindowFormat", "void(Int)");
    m_show_imm = env->GetMethodID(m_class, "showIMM", "void(Int)");
    m_hide_imm = env->GetMethodID(m_class, "hideIMM", "void(Int)");
}

void MainActivity::OnCreate() {
    m_env->CallVoidMethod(m_this, m_on_create);
}

void MainActivity::OnStart() {
    m_env->CallVoidMethod(m_this, m_on_start);
}

void MainActivity::OnResume() {
    m_env->CallVoidMethod(m_this, m_on_resume);
}

void MainActivity::OnPause() {
    m_env->CallVoidMethod(m_this, m_on_pause);
}

void MainActivity::OnStop() {
    m_env->CallVoidMethod(m_this, m_on_stop);
}

void MainActivity::OnDestroy() {
    m_env->CallVoidMethod(m_this, m_on_destroy);
}

void MainActivity::SetWindowFlags(int flags, int mask) {
    m_env->CallVoidMethod(m_this, m_set_window_flags, flags, mask);
}

void MainActivity::SetWindowFormat(int format) {
    m_env->CallVoidMethod(m_this, m_set_window_format, format);
}

void MainActivity::ShowIMM(int mode) {
    m_env->CallVoidMethod(m_this, m_show_imm, mode);
}

void MainActivity::HideIMM(int mode) {
    m_env->CallVoidMethod(m_this, m_hide_imm, mode);
}
