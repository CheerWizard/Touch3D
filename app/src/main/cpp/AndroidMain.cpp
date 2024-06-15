#include <Application.hpp>
#include <Log.hpp>

static Application* s_app = nullptr;
static ThreadPool* s_thread_pool = nullptr;

Application::Application(Activity* activity)
: m_activity(activity) {
    m_window = new Window(m_io_buffer);
}

Application::~Application() {
    delete m_activity;
    delete m_window;
}

void Application::RunLoop() {
    m_running = true;
    while (m_running) {
        m_window->Update();
    }
}

void Application::OnCreate() {
    LogDebug("OnCreate()");
    // TODO(cheerwizard): consider how to simplify the code
    ThreadPool::Get().SubmitTask([this]() {
         // do something
    });
}

void Application::OnStart() {
    LogDebug("OnStart()");
}

void Application::OnResume() {
    LogDebug("OnResume()");
}

void Application::OnPause() {
    LogDebug("OnPause()");
}

void Application::OnStop() {
    LogDebug("OnStop()");
}

void Application::OnDestroy() {
    LogDebug("OnDestroy()");
}

void Application::OnConfigurationChanged() {

}

void Application::OnLowMemory() {

}

void Application::OnWindowFocusChanged() {

}

void Application::OnSurfaceCreated(void *surface) {

}

void Application::OnSurfaceChanged(void *surface, int format, int w, int h) {

}

void Application::OnSurfaceRedrawNeeded(void *surface) {

}

void Application::OnSurfaceDestroyed() {

}

void Application::OnInputQueueCreated(void *input_queue) {

}

void Application::OnInputQueueDestroyed(void *input_queue) {

}

void Application::OnContentRectChanged(int x, int y, int w, int h) {

}

JNIEXPORT jint JNI_OnLoad(JavaVM* vm, void* reserved) {
    Jni::vm = vm;

    JNIEnv* env = Jni::Get();

    jclass class_local_ref = env->FindClass("com/cheerwizard/touch3d/MainActivity");
    MainActivity::clazz = (jclass) env->NewGlobalRef(class_local_ref);
    env->DeleteLocalRef(class_local_ref);

    MainActivity::mid_set_window_flags = env->GetMethodID(MainActivity::clazz, "setWindowFlags", "(II)V");
    MainActivity::mid_set_window_format = env->GetMethodID(MainActivity::clazz, "setWindowFormat", "(I)V");
    MainActivity::mid_show_input = env->GetMethodID(MainActivity::clazz, "showInput", "(I)V");
    MainActivity::mid_hide_input = env->GetMethodID(MainActivity::clazz, "hideInput", "(I)V");

    s_thread_pool = new ThreadPool(4, 100, "Native", T3D_THREAD_PRIORITY_NORMAL);

    return JNI_VERSION_1_6;
}

JNIEXPORT void JNI_OnUnload(JavaVM* vm, void* reserved) {
    delete s_app;
    delete s_thread_pool;
    Jni::Get()->DeleteGlobalRef(MainActivity::clazz);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_cheerwizard_touch3d_MainActivity_nativeOnCreate(
        JNIEnv* env, jobject thiz
) {
    ThreadPool::Get().SubmitTask([thiz]() {
        if (s_app == nullptr) {
            s_app = new Application(new MainActivity(thiz));
            s_app->OnCreate();
            s_app->RunLoop();
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