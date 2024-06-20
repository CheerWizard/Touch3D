#include <Thread.hpp>
#include <Log.hpp>

#include <pthread.h>

static const int T3D_THREAD_PRIORITY_CODE[T3D_THREAD_PRIORITY_COUNT] = {
        1, // T3D_THREAD_PRIORITY_LOWEST
        2, // T3D_THREAD_PRIORITY_NORMAL
        3, // T3D_THREAD_PRIORITY_HIGHEST
};

void Thread::SetFormat()
{
    int result;

    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    size_t cpusetsize = sizeof(cpuset);
    CPU_SET(m_tid, &cpuset);

    // Set affinity mask
    result = sched_setaffinity(m_pid, cpusetsize, &cpuset);
    LogAssert(result != 0, "Failed to set thread affinity mask on Android!");

    // Set priority
    sched_param param {};
    param.sched_priority = T3D_THREAD_PRIORITY_CODE[m_priority];
    result = pthread_setschedparam(m_handle, SCHED_OTHER, &param);
    LogAssert(result != 0, "Failed to set thread priority on Android!");

    // Set name
    result = pthread_setname_np(m_handle, m_name);
    LogAssert(result != 0, "Failed to set thread name on Android!");
}