#pragma once

#include <Thread.hpp>
#include <RingBuffer.hpp>

#include <functional>
#include <condition_variable>
#include <atomic>
#include <sstream>

template<usize task_size>
class ThreadPool final {

public:
    ThreadPool(u32 thread_count, const char* name, T3D_THREAD_PRIORITY priority);
    ~ThreadPool();

public:
    void Add(const std::function<void()>& task);

    inline u32 GetThreadCount() const;

protected:
    // wakes only one thread
    // allows caller-thread to be rescheduled by OS
    inline void Poll();

    virtual void CreateThread(const char* name, T3D_THREAD_PRIORITY priority);

private:
    RingBuffer<std::function<void()>, task_size> m_task_buffer;
    std::condition_variable m_wake_condition;
    std::mutex m_wake_mutex;
    u32 m_thread_count;
    bool m_running;
};

template<usize task_size>
ThreadPool<task_size>::ThreadPool(u32 thread_count, const char* name, T3D_THREAD_PRIORITY priority)
{
    m_thread_count = thread_count;
    m_running = true;
    std::stringstream fmt_name;
    for (int i = 0; i < thread_count ; i++)
    {
        fmt_name << name << "-" << i;
        CreateThread(fmt_name.str().c_str(), priority);
    }
}

template<usize task_size>
ThreadPool<task_size>::~ThreadPool() {
    m_running = false;
}

template<usize task_size>
void ThreadPool<task_size>::Add(const std::function<void()> &task) {
    // try to push a new task until it is pushed
    while (!m_task_buffer.Push(task))
    {
        Poll();
    }
    m_wake_condition.notify_one();
}

template<usize task_size>
u32 ThreadPool<task_size>::GetThreadCount() const {
    return m_thread_count;
}

template<usize task_size>
void ThreadPool<task_size>::Poll() {
    m_wake_condition.notify_one();
    std::this_thread::yield();
}

template<usize task_size>
void ThreadPool<task_size>::CreateThread(const char* name, T3D_THREAD_PRIORITY priority)
{
    Thread(name, priority).Run([this]()
    {
       std::function<void()> task;
       while (m_running) {
           if (m_task_buffer.Pop(task)) {
               task();
           }
           else {
               // because we don't want to overhead cpu core with thread while loop
               // it's better to simply put thread into wait, until it is notified by outer thread with wake condition variable
               std::unique_lock<std::mutex> lock(m_wake_mutex);
               m_wake_condition.wait(lock);
           }
       }
    });
}