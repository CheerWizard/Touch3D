#include <Thread.hpp>

u32 Thread::GetCurrentID()
{
    return std::hash<std::thread::id>{}(std::this_thread::get_id());
}

void Thread::CurrentSleep(u32 millis)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(millis));
}

void Thread::Run(const std::function<void()> &runnable) {
    std::thread thread(runnable);
    thread.detach();
    m_handle = thread.native_handle();
#ifdef T3D_DEBUG
    SetFormat();
#endif
}
