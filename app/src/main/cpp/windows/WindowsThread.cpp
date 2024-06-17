#include <Thread.hpp>
#include <Log.hpp>

#define NOMINMAX
#include <Windows.h>
#include <processthreadsapi.h>
#include <pthread.h>

#include <sstream>

const DWORD MS_VC_EXCEPTION = 0x406D1388;
#pragma pack(push, 8)

struct ThreadInfo final {
    DWORD type; // Must be 0x1000.
    LPCSTR name; // Pointer to name (in user addr space).
    DWORD id; // Thread ID (-1=caller thread).
    DWORD flags; // Reserved for future use, must be zero.
};

#pragma pack(pop)

HRESULT SetThreadName(DWORD thread_id, const char* thread_name) {
    ThreadInfo info {};
    info.type = 0x1000;
    info.name = thread_name;
    info.id = thread_id;
    info.flags = 0;
#pragma warning(push)
#pragma warning(disable: 6320 6322)
    __try {
        RaiseException(MS_VC_EXCEPTION, 0, sizeof(info) / sizeof(ULONG_PTR), (ULONG_PTR*)&info);
    }
    catch (...) {}
#pragma warning(pop)
    return 1;
}

static const int T3D_THREAD_PRIORITY_CODE[T3D_THREAD_PRIORITY_COUNT] = {
        THREAD_PRIORITY_LOWEST,
        THREAD_PRIORITY_NORMAL,
        THREAD_PRIORITY_HIGHEST,
};

void Thread::SetFormat()
{
    HANDLE handle = (HANDLE) m_handle;

    // set affinity mask
    DWORD_PTR affinityMask = 1ull << m_handle;
    DWORD_PTR affinity_result = SetThreadAffinityMask(handle, affinityMask);
    LogAssert(affinity_result > 0, "Failed to set thread affinity mask on Windows!")

    // set priority
    BOOL priority_result = SetThreadPriority(handle, T3D_THREAD_PRIORITY_CODE[m_priority]);
    LogAssert(priority_result != 0, "Failed to set thread priority on Windows!");

    // set name
    HRESULT hr = SetThreadName(m_handle, m_name);
    LogAssert(SUCCEEDED(hr), "Failed to set thread name on Windows!");
}