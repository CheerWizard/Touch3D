#include <AndroidApp.hpp>
#include <Log.hpp>

static AndroidApp* s_app = nullptr;
static ThreadPool<1, 5, T3D_THREAD_PRIORITY_HIGHEST>* s_jni_thread_pool = nullptr;

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    Jni::vm = vm;

    JNIEnv* env = Jni::Get();

    jclass class_local_ref = env->FindClass("com/cheerwizard/touch3d/MainActivity");
    Activity::clazz = (jclass) env->NewGlobalRef(class_local_ref);
    env->DeleteLocalRef(class_local_ref);

    Activity::mid_set_window_flags = env->GetMethodID(Activity::clazz, "setWindowFlags", "(II)V");
    Activity::mid_set_window_format = env->GetMethodID(Activity::clazz, "setWindowFormat", "(I)V");
    Activity::mid_show_input = env->GetMethodID(Activity::clazz, "showInput", "(I)V");
    Activity::mid_hide_input = env->GetMethodID(Activity::clazz, "hideInput", "(I)V");

    s_jni_thread_pool = new ThreadPool<1, 5, T3D_THREAD_PRIORITY_HIGHEST>("JNI");

    return JNI_VERSION_1_6;
}

JNIEXPORT void JNI_OnUnload(JavaVM* vm, void* reserved) {
    delete s_app;
    delete s_jni_thread_pool;
    Jni::Get()->DeleteGlobalRef(Activity::clazz);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_cheerwizard_touch3d_MainActivity_nativeOnCreate(
        JNIEnv* env, jobject thiz
) {
    s_jni_thread_pool->Add([thiz]() {
        if (s_app == nullptr) {
            s_app = new AndroidApp(thiz);
            s_app->OnCreate();
            s_app->Run();
        }
    });
}

extern "C"
JNIEXPORT void JNICALL
Java_com_cheerwizard_touch3d_MainActivity_nativeOnDestroy(
        JNIEnv *env, jobject thiz
) {
    s_app->OnDestroy();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_cheerwizard_touch3d_MainActivity_nativeOnStart(
        JNIEnv *env, jobject thiz
) {
    s_app->OnStart();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_cheerwizard_touch3d_MainActivity_nativeOnResume(
        JNIEnv *env, jobject thiz
) {
    s_app->OnResume();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_cheerwizard_touch3d_MainActivity_nativeOnPause(
        JNIEnv *env, jobject thiz
) {
    s_app->OnPause();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_cheerwizard_touch3d_MainActivity_nativeOnStop(
        JNIEnv *env, jobject thiz
) {
    s_app->OnStop();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_cheerwizard_touch3d_MainActivity_nativeOnConfigurationChanged(
        JNIEnv *env, jobject thiz
) {
    s_app->OnConfigurationChanged();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_cheerwizard_touch3d_MainActivity_nativeOnLowMemory(
        JNIEnv *env, jobject thiz
) {
    s_app->OnLowMemory();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_cheerwizard_touch3d_MainActivity_nativeOnWindowFocusChanged(
        JNIEnv *env, jobject thiz,
        jboolean focused
) {
    s_app->OnWindowFocusChanged();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_cheerwizard_touch3d_MainActivity_nativeOnSurfaceCreated(
        JNIEnv *env, jobject thiz,
        jobject surface
) {
    s_app->OnSurfaceCreated(surface);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_cheerwizard_touch3d_MainActivity_nativeOnSurfaceChanged(
        JNIEnv *env, jobject thiz,
        jobject surface,
        jint format, jint w, jint h
) {
    s_app->OnSurfaceChanged(surface, format, w, h);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_cheerwizard_touch3d_MainActivity_nativeOnSurfaceRedrawNeeded(
        JNIEnv *env, jobject thiz,
        jobject surface
) {
    s_app->OnSurfaceRedrawNeeded(surface);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_cheerwizard_touch3d_MainActivity_nativeOnSurfaceDestroyed(
        JNIEnv *env, jobject thiz
) {
    s_app->OnSurfaceDestroyed();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_cheerwizard_touch3d_MainActivity_nativeOnInputQueueCreated(
        JNIEnv *env, jobject thiz,
        jlong queue
) {
    s_app->OnInputQueueCreated(reinterpret_cast<void*>(queue));
}

extern "C"
JNIEXPORT void JNICALL
Java_com_cheerwizard_touch3d_MainActivity_nativeOnInputQueueDestroyed(
        JNIEnv *env, jobject thiz,
        jlong queue
) {
    s_app->OnInputQueueDestroyed(reinterpret_cast<void*>(queue));
}

extern "C"
JNIEXPORT void JNICALL
Java_com_cheerwizard_touch3d_MainActivity_nativeOnContentRectChanged(
        JNIEnv *env, jobject thiz,
        jint x, jint y,
        jint w, jint h
) {
    s_app->OnContentRectChanged(x, y, w, h);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_cheerwizard_touch3d_NativeLog_v(JNIEnv *env, jobject thiz, jstring tag, jstring log) {
    T3D_LOG_VERB("%s: %s", tag, log);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_cheerwizard_touch3d_NativeLog_i(JNIEnv *env, jobject thiz, jstring tag, jstring log) {
    T3D_LOG_INFO("%s: %s", tag, log);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_cheerwizard_touch3d_NativeLog_d(JNIEnv *env, jobject thiz, jstring tag, jstring log) {
    T3D_LOG_DBG("%s: %s", tag, log);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_cheerwizard_touch3d_NativeLog_w(JNIEnv *env, jobject thiz, jstring tag, jstring log) {
    T3D_LOG_WARN("%s: %s", tag, log);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_cheerwizard_touch3d_NativeLog_e(JNIEnv *env, jobject thiz, jstring tag, jstring log) {
    T3D_LOG_ERR("%s: %s", tag, log);
}