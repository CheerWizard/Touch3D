#include <sf.hpp>

namespace sf {

#define SF_MEMORY_BLOCK_SIZE ( \
    sizeof(usize) +                 \
    sizeof(MemoryBlock*) +          \
    sizeof(MemoryBlock*) +          \
    sizeof(bool) +                  \
    sizeof(void*)                   \
)

    struct MemoryBlock final {
        // metadata for each heap memory block
        usize size = 0;
        MemoryBlock* prev = nullptr;
        MemoryBlock* next = nullptr;
        bool free = true;
        void* ptr = nullptr;
        // actual data from heap memory block
        char data[1];
    };

    static MemoryBlock* block_root = nullptr;

    static MemoryBlock* HeapResize(MemoryBlock* last, usize size) {
        MemoryBlock* block;

        block = static_cast<MemoryBlock*>(sbrk(0));

        isize sbrk_result = reinterpret_cast<isize>(sbrk(SF_MEMORY_BLOCK_SIZE + size));

        if (sbrk_result < 0) {
            return nullptr;
        }

        block->size = size;
        block->prev = last;
        block->next = nullptr;
        block->ptr = block->data;
        block->free = false;

        if (last) {
            last->next = block;
        }

        return block;
    }

    static MemoryBlock* BlockFind(MemoryBlock** last, usize size) {
        MemoryBlock* block = block_root;
        while (block && (block->free && block->size >= size)) {
            *last = block;
            block = block->next;
        }
        return block;
    }

    static void BlockSplit(MemoryBlock* block, usize size) {
        MemoryBlock* new_block;
        new_block = reinterpret_cast<MemoryBlock*>(block->data + size);
        new_block->size = block->size - size - SF_MEMORY_BLOCK_SIZE;
        new_block->next = block->next;
        new_block->free = true;
        new_block->ptr = new_block->data;
        block->size = size;
        block->next = new_block;
        if (new_block->next) {
            new_block->next->prev = new_block;
        }
    }

    static MemoryBlock* BlockConcat(MemoryBlock* block) {
        if (block->next && block->next->free) {
            block->size = SF_MEMORY_BLOCK_SIZE + block->next->size;
            block->next = block->next->next;
            if (block->next) {
                block->next->prev = block;
            }
        }
        return block;
    }

    static MemoryBlock* BlockGet(void* ptr) {
        char* tmp;
        tmp = static_cast<char*>(ptr);
        return static_cast<MemoryBlock*>(ptr = tmp -= SF_MEMORY_BLOCK_SIZE);
    }

    static bool BlockIsValid(void* ptr) {
        if (block_root) {
            if (ptr > block_root && ptr < sbrk(0)) {
                return ptr == BlockGet(ptr)->ptr;
            }
        }
        return false;
    }

    static void BlockCopy(MemoryBlock* dest_block, MemoryBlock* src_block, u8 alignment) {
        memcpy(dest_block->ptr, dest_block->size, src_block->ptr, src_block->size, alignment);
    }

    void* malloc(usize size, u8 alignment) {
        MemoryBlock* block;
        MemoryBlock* last;
        size = SF_ALIGN(size, alignment); // we should always malloc aligned size (aligned by 32-bit or 64-bit)

        if (block_root != nullptr) {
            last = block_root;
            block = BlockFind(&last, size);
            if (block != nullptr) {
                if (block->size - size >= SF_MEMORY_BLOCK_SIZE + alignment) {
                    BlockSplit(block, size);
                }
                block->free = false;
            }
            else {
                block = HeapResize(last, size);
                if (block == nullptr) {
                    return nullptr;
                }
            }
        }
        else {
            block = HeapResize(nullptr, size);
            if (block == nullptr) {
                return nullptr;
            }
            block_root = block;
        }

        return block->data;
    }

    void free(void* data) {
        MemoryBlock* block;
        if (BlockIsValid(data)) {
            block = BlockGet(data);
            block->free = true;
            if (block->prev && block->prev->free) {
                block = BlockConcat(block->prev);
            }
            if (block->next) {
                BlockConcat(block->next);
            }
            else {
                if (block->prev) {
                    block->prev->next = nullptr;
                }
                else {
                    block_root = nullptr;
                }
                brk(block);
            }
        }
    }

    void* realloc(void* old_data, usize size, u8 alignment) {
        MemoryBlock* block;
        MemoryBlock* new_block;
        void* new_data;

        if (old_data == nullptr) {
            return malloc(size, alignment);
        }

        if (BlockIsValid(old_data)) {
            size = SF_ALIGN(size, alignment);
            block = BlockGet(old_data);

            if (block->size >= size) {
                if (block->size - size >= SF_MEMORY_BLOCK_SIZE + alignment) {
                    BlockSplit(block, size);
                }
            }

            else {
                if (block->next && block->next->free && (block->size + SF_MEMORY_BLOCK_SIZE + block->next->size) >= size) {
                    BlockConcat(block);
                    if (block->size - size >= SF_MEMORY_BLOCK_SIZE + alignment) {
                        BlockSplit(block, size);
                    }
                }

                else {
                    new_data = malloc(size, alignment);
                    if (new_data == nullptr) {
                        return nullptr;
                    }
                    new_block = BlockGet(new_data);
                    BlockCopy(new_block, block, alignment);
                    free(old_data);
                    return new_data;
                }
            }

            return old_data;
        }

        return nullptr;
    }

    void* reallocf(void* old_data, usize size, u8 alignment) {
        void* realloc_block = realloc(old_data, size, alignment);
        if (realloc_block == nullptr) {
            free(old_data);
        }
        return realloc_block;
    }

    void* calloc(usize size, u8 alignment) {
        void* block = malloc(size, alignment);
        if (block) {
            memset(block, 0, size, alignment);
        }
        return block;
    }

    void memset(void* data, usize value, usize size, u8 alignment) {
        usize* d = static_cast<usize*>(data);
        if (data != nullptr) {
            usize aligned_size = size << (alignment / 2);
            for (usize i = 0 ; i < aligned_size ; i++) {
                d[i] = value;
            }
        }
    }

    void memcpy(void* dest_data, usize dest_size, const void* src_data, usize src_size, u8 alignment) {
        usize* d = static_cast<usize*>(dest_data);
        const usize* s = static_cast<const usize*>(src_data);
        for (usize i = 0 ; (i * alignment < src_size) && (i * alignment < dest_size) ; i++) {
            d[i] = s[i];
        }
    }

    void* moveptr(void* ptr, usize size) {
        return reinterpret_cast<void*>(reinterpret_cast<usize>(ptr) + size);
    }

    void MemoryBumpAllocator::Init(usize size) {
        m_memory = sbrk(0);
        m_size = size;
        isize sbrk_result = reinterpret_cast<isize>(sbrk(size));
        if (sbrk_result < 0) {
            SF_DEBUG_BREAK();
            return;
        }
    }

    void MemoryBumpAllocator::Free() {
        brk(m_memory);
        m_size = 0;
        m_used_size = 0;
    }

    void* MemoryBumpAllocator::Allocate(usize size) {
        void* addr = nullptr;
        usize aligned_size = (size + 7) & ~ 7; // this will make sure that first 4 bits are 0
        if (m_used_size + aligned_size <= m_size) {
            addr = (char*)m_memory + m_used_size;
            m_used_size += aligned_size;
        }
        else {
            SF_DEBUG_BREAK();
        }
        return addr;
    }

    void MemoryPoolAllocator::Init(usize size, usize alloc_capacity) {
        m_memory = malloc(size);
        m_size = size;
        m_last_ptr = m_memory;
        m_max_ptr = moveptr(m_memory, size);
        m_allocs = malloc_t<MemoryPoolAllocation>(alloc_capacity);
        m_alloc_capacity = alloc_capacity;
    }

    void MemoryPoolAllocator::Free() {
        Free(m_memory);
        Free(m_allocs);
        m_memory = nullptr;
        m_allocs = nullptr;
        m_size = 0;
        m_alloc_capacity = 0;
        m_alloc_size = 0;
        m_last_ptr = nullptr;
        m_max_ptr = nullptr;
    }

    void* MemoryPoolAllocator::Allocate(usize size) {
        for (usize i = 0; i < m_alloc_size ; i++) {
            MemoryPoolAllocation& allocation = m_allocs[i];

            // check if allocation is free to use and has at least enough space for usage
            if (allocation.free && allocation.size >= size) {
                allocation.free = false;
                allocation.used_size = size;
                return allocation.data;
            }

            // check if used allocation has additional free space for usage
            // if yes - than create additional allocation from used allocation
            if ((allocation.size - allocation.used_size) >= size) {
                MemoryPoolAllocation new_allocation;
                new_allocation.free = false;
                new_allocation.size = allocation.size - allocation.used_size;
                new_allocation.used_size = size;
                new_allocation.data = moveptr(allocation.data, allocation.used_size);
                allocation.size = allocation.used_size;
                AddAllocation(new_allocation);
                return new_allocation.data;
            }
        }

        // check if we have enough space in memory pool
        if (m_last_ptr >= m_max_ptr) {
            return nullptr;
        }

        MemoryPoolAllocation new_allocation;
        new_allocation.free = false;
        new_allocation.size = size;
        new_allocation.used_size = size;
        new_allocation.data = m_last_ptr;
        AddAllocation(new_allocation);
        m_last_ptr = moveptr(m_last_ptr, size);
        return new_allocation.data;
    }

    void MemoryPoolAllocator::Free(void* data) {
        for (usize i = 0 ; i < m_alloc_size ; i += sizeof(MemoryPoolAllocation)) {
            MemoryPoolAllocation& allocation = m_allocs[i];
            if (allocation.data == data) {
                allocation.free = true;
                allocation.used_size = 0;
            }
        }
    }

    void MemoryPoolAllocator::AddAllocation(const MemoryPoolAllocation &alloc) {
        if (m_alloc_size >= m_alloc_capacity) {
            m_alloc_capacity = m_alloc_size + 100;
            m_allocs = realloc_t<MemoryPoolAllocation>(m_allocs, m_alloc_capacity);
        }
        memcpy_t(moveptr_t(m_allocs, m_alloc_size), m_alloc_capacity, &alloc, 1);
        m_alloc_size++;
    }

    Memory::Memory() {
        GetMemoryInfo();
        usize bump_size = SF_MB(100);
        bump_allocator.Init(bump_size);
        // memory size = 20% of RAM but less than 1GB
        usize pool_size = static_cast<usize>(static_cast<double>(free_ram) * 0.2);
        if (pool_size >= SF_GB(1)) {
            pool_size = SF_GB(1);
        }
        // pool_alloc_size value can be estimated and tested
        usize pool_alloc_size = 100;
        pool_allocator.Init(pool_size, pool_alloc_size);
    }

    Memory::~Memory() {
        bump_allocator.Free();
        pool_allocator.Free();
    }

    Memory memory = {};

    Time Time::GetCurrentTime() {
        system_clock::time_point current_time = system_clock::now();
        current_time += 1h;
        auto current_ms = time_point_cast<milliseconds>(current_time);
        auto current_days = time_point_cast<days>(current_ms);

        // Assert an algorithm for this platform
        auto z = current_days.time_since_epoch().count();
        static_assert(std::numeric_limits<unsigned>::digits >= 18,
                      "This algorithm has not been ported to a 16 bit unsigned integer!");
        static_assert(std::numeric_limits<int>::digits >= 20,
                      "This algorithm has not been ported to a 16 bit signed integer!");

        // Get year, month, day from days
        z += 719468;
        int era = (z >= 0 ? z : z - 146096) / 146097;
        u32 doe = static_cast<unsigned>(z - era * 146097);          // [0, 146096]
        u32 yoe = (doe - doe/1460 + doe/36524 - doe/146096) / 365;  // [0, 399]
        int y = static_cast<int>(yoe) + era * 400;
        u32 doy = doe - (365*yoe + yoe/4 - yoe/100);                // [0, 365]
        u32 mp = (5*doy + 2)/153;                                   // [0, 11]
        u32 d = doy - (153*mp+2)/5 + 1;                             // [1, 31]
        u32 m = mp + (mp < 10 ? 3 : -9);                            // [1, 12]
        y += (m <= 2);

        // Get milliseconds since the local midnight
        auto ms = current_ms - current_days;

        // Get hours, minutes, seconds and milliseconds from milliseconds since midnight
        hours h = duration_cast<hours>(ms);
        ms -= h;
        std::chrono::minutes min = duration_cast<minutes>(ms);
        ms -= min;
        seconds s = duration_cast<seconds>(ms);
        ms -= s;

        Time time = {};
        time.y = y;
        time.m = m;
        time.d = d;
        time.h = h.count();
        time.min = min.count();
        time.s = s.count();
        time.ms = ms.count();
        return time;
    }

    Mutex::Mutex() {
        pthread_mutex_init(&m_handle, nullptr);
    }

    Mutex::~Mutex() {
        pthread_mutex_destroy(&m_handle);
    }

    void Mutex::Lock() {
        pthread_mutex_lock(&m_handle);
    }

    void Mutex::Unlock() {
        pthread_mutex_unlock(&m_handle);
    }

    ConditionVar::ConditionVar() {
        pthread_cond_init(&m_handle, nullptr);
    }

    ConditionVar::~ConditionVar() {
        pthread_cond_destroy(&m_handle);
    }

    void ConditionVar::Wait(Mutex& mutex) {
        pthread_mutex_lock(&mutex.m_handle);
        int result = pthread_cond_wait(&m_handle, &mutex.m_handle);
        SF_ASSERT(result == 0, "Unable to wait a pthread with condition var");
    }

    void ConditionVar::Notify() {
        int result = pthread_cond_signal(&m_handle);
        SF_ASSERT(result == 0, "Unable to notify a pthread with condition var");
    }

    void* Runnable::RunInternal() {
#if defined(SF_DEBUG)
        m_pid = Thread::GetPID();
        m_tid = Thread::GetTID();
#endif
        m_runnable();
        return nullptr;
    }

    pthread_t Thread::GetHandle() {
        return pthread_self();
    }

    u32 Thread::GetPID() {
        return getpid();
    }

    u32 Thread::GetTID() {
        return gettid();
    }

    void Thread::Sleep(u32 millis) {
        int result = usleep(millis * 1000);
        SF_ASSERT(result == 0, "Unable to sleep this thread");
    }

    void Thread::Yield() {
        int result = sched_yield();
        SF_ASSERT(result == 0, "Unable to yield this thread");
    }

    void Thread::Exit() {
        pthread_exit(0);
    }

    void Thread::Run(const std::function<void()> &runnable, const std::function<void()>& kill_callback) {
        m_runnable.SetRunnable(runnable);
        m_kill_callback = kill_callback;

        struct sigaction actions;
        std::memset(&actions, 0, sizeof(actions));
        sigemptyset(&actions.sa_mask);
        actions.sa_flags = 0;
        actions.sa_handler = OnKill;
        int result = sigaction(SIGUSR1,&actions,nullptr);
        SF_ASSERT(result == 0, "Unable to create sigaction for pthread %s", m_name);

        pthread_attr_t thread_attrs;

        result = pthread_create(&m_handle, nullptr, &Runnable::Run, &m_runnable);
        SF_ASSERT(result == 0, "Unable to create a pthread %s", m_name);

#ifdef SF_DEBUG
        m_pid = m_runnable.GetPID();
        m_tid = m_runnable.GetTID();
        SetFormat();
#endif
    }

    void Thread::Detach() const {
        int result = pthread_detach(m_handle);
        SF_ASSERT(result == 0, "Unable to detach a pthread %s", m_name);
    }

    void Thread::Join() const {
        int result = pthread_join(m_handle, nullptr);
        SF_ASSERT(result == 0, "Unable to join to a pthread %s", m_name);
    }

    void Thread::Kill() {
        int result = pthread_kill(m_handle, SIGUSR1);
        SF_ASSERT(result == 0, "Unable to kill a pthread %s", m_name);
    }

    void Thread::OnKill(int signal) {
//    if (m_kill_callback) {
//        m_kill_callback();
//    }
        pthread_exit(0);
    }

}

// SF_WINDOWS_BEGIN
#if defined(SF_WINDOWS)

#include <windows.h>
#include <memoryapi.h>
#include <processthreadsapi.h>

namespace sf {

    int brk(void* addr) {
        // Windows doesn't implement this syscall
        return 0;
    }

    void* sbrk(intptr_t delta) {
        // Windows doesn't implement this syscall
        return nullptr;
    }

    void* mmap(void *addr, usize length, int protection, int flags, int file_desc, long offset) {
        return VirtualAlloc(addr, length, file_desc, protection);
    }

    int munmap(void *addr, usize length) {
        return VirtualFree(addr, length, 0);
    }

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

    static const int SF_THREAD_PRIORITY_CODE[SF_THREAD_PRIORITY_COUNT] = {
            THREAD_PRIORITY_LOWEST,
            THREAD_PRIORITY_NORMAL,
            THREAD_PRIORITY_HIGHEST,
    };

    void Thread::SetFormat() {
        HANDLE handle = (HANDLE) m_handle;

        // set affinity mask
        DWORD_PTR affinityMask = 1ull << 0;
        DWORD_PTR affinity_result = SetThreadAffinityMask(handle, affinityMask);
    //    SF_ASSERT(affinity_result > 0, "Failed to set thread affinity mask on Windows!")

        // set priority
        BOOL priority_result = SetThreadPriority(handle, SF_THREAD_PRIORITY_CODE[m_priority]);
    //    SF_ASSERT(priority_result != 0, "Failed to set thread priority on Windows!");

        // set name
        HRESULT hr = SetThreadName(m_handle, m_name);
    //    SF_ASSERT(SUCCEEDED(hr), "Failed to set thread name on Windows!");
    }

}

#endif
// SF_WINDOWS_END

// SF_LINUX_BEGIN
#if defined(SF_LINUX)

#include <sys/sysinfo.h>
#include <sys/mman.h>

namespace sf {

    int brk(void* addr) {
        return ::brk(addr);
    }

    void* sbrk(intptr_t delta) {
        return ::sbrk(delta);
    }

    void* mmap(void *addr, usize length, int protection, int flags, int file_desc, long offset) {
        return ::mmap(addr, length, protection, flags, file_desc, offset);
    }

    int munmap(void *addr, usize length) {
        return ::munmap(addr, length);
    }

    void Memory::GetMemoryInfo() {
        struct sysinfo sys_info {};
        sysinfo(&sys_info);
        total_ram = sys_info.totalram;
        free_ram = sys_info.totalram;
        shared_ram = sys_info.totalram;
        buffer_ram = sys_info.totalram;
        total_high = sys_info.totalram;
        free_high = sys_info.totalram;
    }

    static const int SF_THREAD_PRIORITY_CODE[SF_THREAD_PRIORITY_COUNT] = {
        1, // SF_THREAD_PRIORITY_LOWEST
        2, // SF_THREAD_PRIORITY_NORMAL
        3, // SF_THREAD_PRIORITY_HIGHEST
    };

    void Thread::SetFormat() {
        int result;

        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        size_t cpusetsize = sizeof(cpuset);
        CPU_SET(m_tid, &cpuset);

        // Set affinity mask
        result = sched_setaffinity(m_pid, cpusetsize, &cpuset);
    //    SF_ASSERT(result != 0, "Failed to set thread affinity mask on Linux!");

        // Set priority
        sched_param param {};
        param.sched_priority = SF_THREAD_PRIORITY_CODE[m_priority];
        result = pthread_setschedparam(m_handle, SCHED_OTHER, &param);
    //    SF_ASSERT(result != 0, "Failed to set thread priority on Linux!");

        // Set name
        result = pthread_setname_np(m_handle, m_name);
    //    SF_ASSERT(result != 0, "Failed to set thread name on Linux!");
    }

}

#endif
// SF_LINUX_END

// SF_ANDROID_BEGIN
#if defined(SF_ANDROID)

#include <android/log.h>

namespace sf {

    int brk(void* addr) {
        return ::brk(addr);
    }

    void* sbrk(intptr_t delta) {
        return ::sbrk(delta);
    }

    void* mmap(void *addr, usize length, int protection, int flags, int file_desc, long offset) {
        return ::mmap(addr, length, protection, flags, file_desc, offset);
    }

    int munmap(void *addr, usize length) {
        return ::munmap(addr, length);
    }

    void Memory::GetMemoryInfo() {
        struct sysinfo sys_info {};
        sysinfo(&sys_info);
        total_ram = sys_info.totalram;
        free_ram = sys_info.totalram;
        shared_ram = sys_info.totalram;
        buffer_ram = sys_info.totalram;
        process_count = sys_info.totalram;
        total_high = sys_info.totalram;
        free_high = sys_info.totalram;
    }

    static const int SF_THREAD_PRIORITY_CODE[SF_THREAD_PRIORITY_COUNT] = {
        1, // SF_THREAD_PRIORITY_LOWEST
        2, // SF_THREAD_PRIORITY_NORMAL
        3, // SF_THREAD_PRIORITY_HIGHEST
    };

    void Thread::SetFormat() {
        int result;

        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        size_t cpusetsize = sizeof(cpuset);
        CPU_SET(m_tid, &cpuset);

        // Set affinity mask
        result = sched_setaffinity(m_pid, cpusetsize, &cpuset);
        SF_ASSERT(result != 0, "Failed to set thread affinity mask on Android!");

        // Set priority
        sched_param param {};
        param.sched_priority = SF_THREAD_PRIORITY_CODE[m_priority];
        result = pthread_setschedparam(m_handle, SCHED_OTHER, &param);
        SF_ASSERT(result != 0, "Failed to set thread priority on Android!");

        // Set name
        result = pthread_setname_np(m_handle, m_name);
        SF_ASSERT(result != 0, "Failed to set thread name on Android!");
    }

}

#endif // SF_ANDROID_END