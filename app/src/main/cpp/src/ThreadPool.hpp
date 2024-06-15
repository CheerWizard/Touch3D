#pragma once

#include <Thread.hpp>
#include <RingBuffer.hpp>

#include <functional>
#include <condition_variable>
#include <atomic>

class ThreadPool final {

public:
    ThreadPool(u32 thread_count, usize reserved_task_count, const char* name, T3D_THREAD_PRIORITY priority);
    ~ThreadPool();

public:
    static ThreadPool& Get();

public:
    void SubmitTask(const std::function<void()>& task);

    inline bool IsBusy();

    inline u32 GetThreadCount() const;

protected:
    // wakes only one thread
    // allows caller-thread to be rescheduled by OS
    inline void Poll();

    virtual void CreateThread(u32 id, const char* name, T3D_THREAD_PRIORITY priority);

private:
    static ThreadPool* s_instance;

    RingBuffer<std::function<void()>> m_task_buffer;
    std::condition_variable m_wake_condition;
    std::mutex m_wake_mutex;
    u64 m_todo_task_count;
    std::atomic<u64> m_done_task_count;
    u32 m_thread_count;
    bool m_running;
};