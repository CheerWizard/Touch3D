#include <android/MainActivity.hpp>

#include <Log.hpp>

jint MainActivity::Load(JavaVM *vm) {
    JNIEnv* env = GetEnv();

    jclass class_local_ref = env->FindClass("com/cheerwizard/touch3d/MainActivity");
    s_class = (jclass) env->NewGlobalRef(class_local_ref);
    env->DeleteLocalRef(class_local_ref);

    s_set_window_flags = env->GetMethodID(s_class, "setWindowFlags", "(II)V");
    s_set_window_format = env->GetMethodID(s_class, "setWindowFormat", "(I)V");
    s_show_input = env->GetMethodID(s_class, "showInput", "(I)V");
    s_hide_input = env->GetMethodID(s_class, "hideInput", "(I)V");

    return JNI_VERSION_1_6;
}

void MainActivity::Unload(JavaVM *vm) {
    GetEnv()->DeleteGlobalRef(s_class);
}

MainActivity::MainActivity(jobject thiz) {
    JNIEnv* env = GetEnv();
    m_this = env->NewGlobalRef(thiz);
    env->DeleteLocalRef(thiz);
}

MainActivity::~MainActivity() {
    GetEnv()->DeleteGlobalRef(m_this);
}

bool MainActivity::IsOpen() const {
    return lifecycle != T3D_LIFECYCLE_DESTROY;
}

void MainActivity::SetWindowFlags(int flags, int mask) {
    GetEnv()->CallVoidMethod(m_this, s_set_window_flags, flags, mask);
}

void MainActivity::SetWindowFormat(int format) {
    GetEnv()->CallVoidMethod(m_this, s_set_window_format, format);
}

void MainActivity::ShowInput(int mode) {
    GetEnv()->CallVoidMethod(m_this, s_show_input, mode);
}

void MainActivity::HideInput(int mode) {
    GetEnv()->CallVoidMethod(m_this, s_hide_input, mode);
}

JNIEnv* MainActivity::GetEnv() {
    JNIEnv* env;
    if (s_vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        LogAssert(false, "Failed to get JNIEnv!", "");
    }
    return env;
}
