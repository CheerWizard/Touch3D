#pragma once

#include <Types.hpp>

#include <thread>
#include <functional>

enum T3D_THREAD_PRIORITY {
    T3D_THREAD_PRIORITY_LOWEST,
    T3D_THREAD_PRIORITY_NORMAL,
    T3D_THREAD_PRIORITY_HIGHEST,

    T3D_THREAD_PRIORITY_COUNT
};

class Thread final {

public:
    Thread(const char* name, T3D_THREAD_PRIORITY priority) : m_name(name), m_priority(priority) {}

public:
    static void CurrentSleep(u32 millis);
    static u32 GetCurrentID();

public:
    void Run(const std::function<void()>& runnable);

private:
    void SetFormat();

private:
    std::thread::native_handle_type m_handle;
    const char* m_name;
    T3D_THREAD_PRIORITY m_priority;

};