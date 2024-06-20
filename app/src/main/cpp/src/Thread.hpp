#pragma once

#include <RingBuffer.hpp>

#include <sstream>
#include <functional>
#include <pthread.h>
#include <csignal>

enum T3D_THREAD_PRIORITY {
    T3D_THREAD_PRIORITY_LOWEST,
    T3D_THREAD_PRIORITY_NORMAL,
    T3D_THREAD_PRIORITY_HIGHEST,

    T3D_THREAD_PRIORITY_COUNT
};

class Mutex final {

    friend class ConditionVar;

public:
    Mutex();
    ~Mutex();

    void Lock();
    void Unlock();

private:
    pthread_mutex_t m_handle;

};

class ConditionVar final {

public:
    ConditionVar();
    ~ConditionVar();

    void Wait(Mutex& mutex);
    void Notify();

private:
    pthread_cond_t m_handle;

};

class Runnable final {

public:
    Runnable(const std::function<void()>& runnable = {}) : m_runnable(runnable) {}

public:
    static void* Run(void* thiz) {
        return static_cast<Runnable*>(thiz)->RunInternal();
    }

public:
    inline void SetRunnable(const std::function<void()>& runnable) {
        m_runnable = runnable;
    }

    [[nodiscard]] inline const pid_t& GetPID() const {
        return m_pid;
    }
    [[nodiscard]] inline const pid_t& GetTID() const {
        return m_tid;
    }

private:
    void* RunInternal();

private:
    std::function<void()> m_runnable = {};
    pid_t m_pid;
    pid_t m_tid;

};

class Thread final {

public:
    Thread() = default;
    Thread(const char* name, T3D_THREAD_PRIORITY priority) : m_name(name), m_priority(priority) {}

public:
    static pthread_t GetHandle();
    static u32 GetPID();
    static u32 GetTID();
    static void Sleep(u32 millis);
    static void Yield();
    static void Exit();

public:
    void Run(const std::function<void()>& runnable, const std::function<void()>& kill_callback = {});
    void Detach() const;
    void Join() const;
    void Kill();

private:
    void SetFormat();
    static void OnKill(int signal);

private:
    Runnable m_runnable;
    pthread_t m_handle;
    pid_t m_pid;
    pid_t m_tid;
    const char* m_name;
    T3D_THREAD_PRIORITY m_priority;
    std::function<void()> m_kill_callback;

};

template<usize thread_buffer_size, usize task_buffer_size, T3D_THREAD_PRIORITY thread_priority>
class ThreadPool final {

public:
    ThreadPool(const char* thread_name);
    ~ThreadPool();

public:
    void Add(const std::function<void()>& task);

protected:
    // wakes only one thread
    // allows caller-thread to be rescheduled by OS
    inline void Poll();

private:
    RingBuffer<std::function<void()>, task_buffer_size> m_task_buffer;
    Thread m_thread_buffer[thread_buffer_size] = {};
    Mutex m_wake_mutex;
    ConditionVar m_wake_condition_var;
};

template<usize thread_buffer_size, usize task_buffer_size, T3D_THREAD_PRIORITY thread_priority>
ThreadPool<thread_buffer_size, task_buffer_size, thread_priority>::ThreadPool(const char* thread_name)
{
    for (int i = 0; i < thread_buffer_size ; i++)
    {
        Thread& thread = m_thread_buffer[i];
        thread = Thread(thread_name, thread_priority);
        thread.Run([this]()
                   {
                       std::function<void()> task;
                       while (true) {
                           if (m_task_buffer.Pop(task)) {
                               task();
                           }
                           else {
                               // because we don't want to overhead cpu core with thread while loop
                               // it's better to simply put thread into wait, until it is notified by outer thread with wake condition variable
                               m_wake_condition_var.Wait(m_wake_mutex);
                           }
                       }
                   });
        thread.Detach();
    }
}

template<usize thread_buffer_size, usize task_buffer_size, T3D_THREAD_PRIORITY thread_priority>
ThreadPool<thread_buffer_size, task_buffer_size, thread_priority>::~ThreadPool() {
    for (int i = 0 ; i < thread_buffer_size ; i++) {
        m_thread_buffer[i].Kill();
    }
}

template<usize thread_buffer_size, usize task_buffer_size, T3D_THREAD_PRIORITY thread_priority>
void ThreadPool<thread_buffer_size, task_buffer_size, thread_priority>::Add(const std::function<void()> &task) {
    // try to push a new task until it is pushed
    while (!m_task_buffer.Push(task))
    {
        Poll();
    }
    m_wake_condition_var.Notify();
}

template<usize thread_buffer_size, usize task_buffer_size, T3D_THREAD_PRIORITY thread_priority>
void ThreadPool<thread_buffer_size, task_buffer_size, thread_priority>::Poll() {
    m_wake_condition_var.Notify();
    Thread::Yield();
}