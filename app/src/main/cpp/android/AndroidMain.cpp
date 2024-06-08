#include <PlatformDefs.hpp>

#ifdef T3D_ANDROID

#include <Application.hpp>
#include <Log.hpp>

#include <android/MainActivity.hpp>

static Application* s_app = nullptr;

Application::Application(MainActivity* main_activity)
: m_main_activity(main_activity) {
    m_window = new Window(m_io_buffer);
}

Application::~Application() {
    delete m_main_activity;
    delete m_window;
}

void Application::RunLoop() {
    m_state = T3D_APP_STARTED;
    while (m_state != T3D_APP_DESTROYED) {
        m_window->Update();
        if (!m_window->IsOpen()) {
            m_state = T3D_APP_DESTROYED;
            break;
        }
    }
}

void Application::OnCreate() {
    LogDebug("OnCreate()", "");
    m_state = T3D_APP_CREATED;
}

void Application::OnStart() {
    LogDebug("OnStart()", "");
    m_state = T3D_APP_STARTED;
}

void Application::OnResume() {
    LogDebug("OnResume()", "");
    m_state = T3D_APP_RESUMED;
}

void Application::OnPause() {
    LogDebug("OnPause()", "");
    m_state = T3D_APP_PAUSED;
}

void Application::OnStop() {
    LogDebug("OnStop()", "");
    m_state = T3D_APP_STOPPED;
}

void Application::OnDestroy() {
    LogDebug("OnDestroy()", "");
    m_state = T3D_APP_DESTROYED;
}

MainActivitySavedState Application::OnSaveInstanceState() {
    LogDebug("OnSaveInstanceState()", "");
    return m_main_activity->saved_state;
}

void Application::OnRestoreInstanceState(const MainActivitySavedState& saved_state) {
    LogDebug("OnRestoreInstanceState()", "");
    m_main_activity->saved_state = saved_state;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_cheerwizard_touch3d_MainActivity_nativeOnCreate(
        JNIEnv *env, jobject thiz
) {
    s_app = new Application(new MainActivity(env, thiz));
    s_app->OnCreate();
    s_app->RunLoop();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_cheerwizard_touch3d_MainActivity_nativeOnDestroy(
        JNIEnv *env, jobject thiz
) {
    s_app->OnDestroy();
    delete s_app;
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_cheerwizard_touch3d_MainActivity_nativeGetError(
        JNIEnv *env, jobject thiz
) {
    return nullptr;
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
JNIEXPORT jbyteArray JNICALL
Java_com_cheerwizard_touch3d_MainActivity_nativeOnSaveInstanceState(
        JNIEnv *env, jobject thiz
) {
    MainActivitySavedState saved_state = s_app->OnSaveInstanceState();
    jbyteArray state = env->NewByteArray(saved_state.size);
    memcpy(state, saved_state.state, saved_state.size);
    return state;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_cheerwizard_touch3d_MainActivity_nativeOnRestoreInstanceState(
        JNIEnv *env, jobject thiz,
        jbyteArray state
) {
    MainActivitySavedState saved_state;
    saved_state.state = state;
    saved_state.size = 0;
    s_app->OnRestoreInstanceState(saved_state);
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

#endif // T3D_ANDROID