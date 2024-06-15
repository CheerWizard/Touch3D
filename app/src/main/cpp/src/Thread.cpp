#include <Thread.hpp>

u32 Thread::GetCurrentID()
{
    return std::hash<std::thread::id>{}(std::this_thread::get_id());
}

void Thread::CurrentSleep(u32 millis)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(millis));
}