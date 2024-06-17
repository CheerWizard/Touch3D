#include <ThreadPool.hpp>

#include <sstream>

ThreadPool::ThreadPool(u32 thread_count, usize reserved_task_count, const char* name, T3D_THREAD_PRIORITY priority)
{
    m_todo_task_count = 0;
    m_done_task_count.store(0);
    m_thread_count = thread_count;
    m_task_buffer.Reserve(reserved_task_count);
    m_running = true;
    std::stringstream fmt_name;
    for (int i = 0; i < thread_count ; i++)
    {
        fmt_name << name << "-" << i;
        CreateThread(fmt_name.str().c_str(), priority);
    }
}

ThreadPool::~ThreadPool() {
    m_running = false;
}

void ThreadPool::Add(const std::function<void()> &task) {
    m_todo_task_count += 1;
    // try to push a new task until it is pushed
    while (!m_task_buffer.Push(task))
    {
        Poll();
    }
    m_wake_condition.notify_one();
}

bool ThreadPool::IsBusy() {
    return m_done_task_count.load() < m_todo_task_count;
}

u32 ThreadPool::GetThreadCount() const {
    return m_thread_count;
}

void ThreadPool::Poll() {
    m_wake_condition.notify_one();
    std::this_thread::yield();
}

void ThreadPool::CreateThread(const char* name, T3D_THREAD_PRIORITY priority)
{
    Thread(name, priority).Run([this]()
    {
        std::function<void()> task;
        while (m_running)
        {
            if (m_task_buffer.Pop(task))
            {
                // do task and update worker state
                task();
                m_done_task_count.fetch_add(1);
            }
            else
            {
                // no task, put thread to sleep
                std::unique_lock<std::mutex> lock(m_wake_mutex);
                m_wake_condition.wait(lock);
            }
        }
    });
}