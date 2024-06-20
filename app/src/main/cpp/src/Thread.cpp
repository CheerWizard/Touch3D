#include <Thread.hpp>
#include <Log.hpp>
#include <unistd.h>

Mutex::Mutex() {
    pthread_mutex_init(&m_handle, nullptr);
}

Mutex::~Mutex() {
    pthread_mutex_destroy(&m_handle);
}

void Mutex::Lock() {
    pthread_mutex_lock(&m_handle);
}

void Mutex::Unlock() {
    pthread_mutex_unlock(&m_handle);
}

ConditionVar::ConditionVar() {
    pthread_cond_init(&m_handle, nullptr);
}

ConditionVar::~ConditionVar() {
    pthread_cond_destroy(&m_handle);
}

void ConditionVar::Wait(Mutex& mutex) {
    pthread_mutex_lock(&mutex.m_handle);
    int result = pthread_cond_wait(&m_handle, &mutex.m_handle);
    T3D_ASSERT(result == 0, "Unable to wait a pthread with condition var");
}

void ConditionVar::Notify() {
    int result = pthread_cond_signal(&m_handle);
    T3D_ASSERT(result == 0, "Unable to notify a pthread with condition var");
}

void* Runnable::RunInternal() {
#if defined(T3D_DEBUG)
    m_pid = Thread::GetPID();
    m_tid = Thread::GetTID();
#endif
    m_runnable();
    return nullptr;
}

pthread_t Thread::GetHandle() {
    return pthread_self();
}

u32 Thread::GetPID() {
    return getpid();
}

u32 Thread::GetTID() {
    return gettid();
}

void Thread::Sleep(u32 millis) {
    int result = usleep(millis * 1000);
    T3D_ASSERT(result == 0, "Unable to sleep this thread");
}

void Thread::Yield() {
    int result = sched_yield();
    T3D_ASSERT(result == 0, "Unable to yield this thread");
}

void Thread::Exit() {
    pthread_exit(0);
}

void Thread::Run(const std::function<void()> &runnable, const std::function<void()>& kill_callback) {
    m_runnable.SetRunnable(runnable);
    m_kill_callback = kill_callback;

    struct sigaction actions;
    memset(&actions, 0, sizeof(actions));
    sigemptyset(&actions.sa_mask);
    actions.sa_flags = 0;
    actions.sa_handler = OnKill;
    int result = sigaction(SIGUSR1,&actions,nullptr);
    T3D_ASSERT(result == 0, "Unable to create sigaction for pthread %s", m_name);

    pthread_attr_t thread_attrs;

    result = pthread_create(&m_handle, nullptr, &Runnable::Run, &m_runnable);
    T3D_ASSERT(result == 0, "Unable to create a pthread %s", m_name);

#ifdef T3D_DEBUG
    m_pid = m_runnable.GetPID();
    m_tid = m_runnable.GetTID();
    SetFormat();
#endif
}

void Thread::Detach() const {
    int result = pthread_detach(m_handle);
    T3D_ASSERT(result == 0, "Unable to detach a pthread %s", m_name);
}

void Thread::Join() const {
    int result = pthread_join(m_handle, nullptr);
    T3D_ASSERT(result == 0, "Unable to join to a pthread %s", m_name);
}

void Thread::Kill() {
    int result = pthread_kill(m_handle, SIGUSR1);
    T3D_ASSERT(result == 0, "Unable to kill a pthread %s", m_name);
}

void Thread::OnKill(int signal) {
    T3D_LOG_INFO("OnKill(%d)", signal);
//    if (m_kill_callback) {
//        m_kill_callback();
//    }
    pthread_exit(0);
}
