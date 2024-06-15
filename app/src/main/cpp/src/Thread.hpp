#pragma once

#include <Types.hpp>

#include <thread>

enum T3D_THREAD_PRIORITY {
    T3D_THREAD_PRIORITY_LOWEST,
    T3D_THREAD_PRIORITY_NORMAL,
    T3D_THREAD_PRIORITY_HIGHEST,

    T3D_THREAD_PRIORITY_COUNT
};

class Thread final {

public:
    Thread(u32 id, std::thread& thread) : m_id(id), m_thread(thread) {}

public:
    static void CurrentSleep(u32 millis);
    static u32 GetCurrentID();

public:
    void SetFormat(const char* name, T3D_THREAD_PRIORITY priority);

private:
    u32 m_id;
    std::thread& m_thread;
};