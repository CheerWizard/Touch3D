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
    LogDebug("OnCreate()");
}

void AndroidApp::OnStart() {
    LogDebug("OnStart()");
}

void AndroidApp::OnResume() {
    LogDebug("OnResume()");
}

void AndroidApp::OnPause() {
    LogDebug("OnPause()");
}

void AndroidApp::OnStop() {
    LogDebug("OnStop()");
}

void AndroidApp::OnDestroy() {
    LogDebug("OnDestroy()");
}

void AndroidApp::OnConfigurationChanged() {
    LogDebug("OnConfigurationChanged()");
}

void AndroidApp::OnLowMemory() {
    LogDebug("OnLowMemory()");
}

void AndroidApp::OnWindowFocusChanged() {
    LogDebug("OnWindowFocusChanged()");
}

void AndroidApp::OnSurfaceCreated(void *surface) {
    LogDebug("OnSurfaceCreated()");
}

void AndroidApp::OnSurfaceChanged(void *surface, int format, int w, int h) {
    LogDebug("OnSurfaceChanged()");
}

void AndroidApp::OnSurfaceRedrawNeeded(void *surface) {
    LogDebug("OnSurfaceRedrawNeeded()");
}

void AndroidApp::OnSurfaceDestroyed() {
    LogDebug("OnSurfaceDestroyed()");
}

void AndroidApp::OnInputQueueCreated(void *input_queue) {
    LogDebug("OnInputQueueCreated()");
}

void AndroidApp::OnInputQueueDestroyed(void *input_queue) {
    LogDebug("OnInputQueueDestroyed()");
}

void AndroidApp::OnContentRectChanged(int x, int y, int w, int h) {
    LogDebug("OnContentRectChanged()");
}