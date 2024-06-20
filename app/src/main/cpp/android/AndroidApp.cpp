#include <AndroidApp.hpp>
#include <Log.hpp>

AndroidApp::AndroidApp(jobject activity) {
    m_activity = new Activity(activity);
}

AndroidApp::~AndroidApp() {
    delete m_activity;
}

void AndroidApp::Run() {
    m_running = true;
    while (m_running) {
        UpdateWindow();
        UpdateEvents();
    }
}

void AndroidApp::UpdateWindow() {

}

void AndroidApp::UpdateEvents() {

}

void AndroidApp::OnCreate() {
    T3D_LOG_INFO("OnCreate()");
}

void AndroidApp::OnStart() {
    T3D_LOG_INFO("OnStart()");
}

void AndroidApp::OnResume() {
    T3D_LOG_INFO("OnResume()");
}

void AndroidApp::OnPause() {
    T3D_LOG_INFO("OnPause()");
}

void AndroidApp::OnStop() {
    T3D_LOG_INFO("OnStop()");
}

void AndroidApp::OnDestroy() {
    T3D_LOG_INFO("OnDestroy()");
}

void AndroidApp::OnConfigurationChanged() {
    T3D_LOG_INFO("OnConfigurationChanged()");
}

void AndroidApp::OnLowMemory() {
    T3D_LOG_INFO("OnLowMemory()");
}

void AndroidApp::OnWindowFocusChanged() {
    T3D_LOG_INFO("OnWindowFocusChanged()");
}

void AndroidApp::OnSurfaceCreated(void *surface) {
    T3D_LOG_INFO("OnSurfaceCreated()");
}

void AndroidApp::OnSurfaceChanged(void *surface, int format, int w, int h) {
    T3D_LOG_INFO("OnSurfaceChanged()");
}

void AndroidApp::OnSurfaceRedrawNeeded(void *surface) {
    T3D_LOG_INFO("OnSurfaceRedrawNeeded()");
}

void AndroidApp::OnSurfaceDestroyed() {
    T3D_LOG_INFO("OnSurfaceDestroyed()");
}

void AndroidApp::OnInputQueueCreated(void *input_queue) {
    T3D_LOG_INFO("OnInputQueueCreated()");
}

void AndroidApp::OnInputQueueDestroyed(void *input_queue) {
    T3D_LOG_INFO("OnInputQueueDestroyed()");
}

void AndroidApp::OnContentRectChanged(int x, int y, int w, int h) {
    T3D_LOG_INFO("OnContentRectChanged()");
}