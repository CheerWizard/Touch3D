#include <iomanip>
#include <sf.hpp>
#include <unistd.h>

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

    void* realloc(void* old_data, usize size, const u8 alignment) {
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

    void* reallocf(void* old_data, const usize size, const u8 alignment) {
        void* realloc_block = realloc(old_data, size, alignment);
        if (realloc_block == nullptr) {
            free(old_data);
        }
        return realloc_block;
    }

    void* calloc(const usize size, const u8 alignment) {
        void* block = malloc(size, alignment);
        if (block) {
            memset(block, 0, size, alignment);
        }
        return block;
    }

    void memset(void* data, const usize value, const usize size, const u8 alignment) {
        auto* d = static_cast<usize*>(data);
        if (data != nullptr) {
            const usize aligned_size = size << (alignment / 2);
            for (usize i = 0 ; i < aligned_size ; i++) {
                d[i] = value;
            }
        }
    }

    void memcpy(void* dest_data, const usize dest_size, const void* src_data, const usize src_size, const u8 alignment) {
        const auto d = static_cast<usize*>(dest_data);
        const auto* s = static_cast<const usize*>(src_data);
        for (usize i = 0 ; i * alignment < src_size && i * alignment < dest_size ; i++) {
            d[i] = s[i];
        }
    }

    void* moveptr(void* ptr, const usize size) {
        return reinterpret_cast<void*>(reinterpret_cast<usize>(ptr) + size);
    }

    memory_bump_t memory_bump_init(const usize size) {
        memory_bump_t memory_bump;
        memory_bump.memory = sbrk(0);
        memory_bump.size = size;
        if (const auto sbrk_result = reinterpret_cast<isize>(sbrk(size)); sbrk_result < 0) {
            SF_DEBUG_BREAK();
        }
        return memory_bump;
    }

    void memory_bump_free(memory_bump_t& memory_bump) {
        brk(memory_bump.memory);
        memory_bump.size = 0;
        memory_bump.used_size = 0;
    }

    void* memory_bump_allocate(memory_bump_t& memory_bump, const usize size) {
        void* addr = nullptr;
        if (const usize aligned_size = (size + 7) & ~ 7; memory_bump.used_size + aligned_size <= memory_bump.size) {
            addr = static_cast<char*>(memory_bump.memory) + memory_bump.used_size;
            memory_bump.used_size += aligned_size;
        }
        else {
            SF_DEBUG_BREAK();
        }
        return addr;
    }

    memory_pool_t memory_pool_init(const usize size, const usize alloc_capacity) {
        memory_pool_t memory_pool;
        memory_pool.memory = malloc(size);
        memory_pool.size = size;
        memory_pool.last_ptr = memory_pool.memory;
        memory_pool.max_ptr = moveptr(memory_pool.memory, size);
        memory_pool.allocs = malloc_t<memory_pool_alloc_t>(alloc_capacity);
        memory_pool.alloc_capacity = alloc_capacity;
        return memory_pool;
    }

    void memory_pool_free(memory_pool_t& memory_pool) {
        free(memory_pool.memory);
        free(memory_pool.allocs);
        memory_pool.memory = nullptr;
        memory_pool.allocs = nullptr;
        memory_pool.size = 0;
        memory_pool.alloc_capacity = 0;
        memory_pool.alloc_size = 0;
        memory_pool.last_ptr = nullptr;
        memory_pool.max_ptr = nullptr;
    }

    static void memory_pool_add_allocation(memory_pool_t& memory_pool, const memory_pool_alloc_t* allocation) {
        if (memory_pool.alloc_size >= memory_pool.alloc_capacity) {
            memory_pool.alloc_capacity = memory_pool.alloc_size + 100;
            memory_pool.allocs = realloc_t<memory_pool_alloc_t>(memory_pool.allocs, memory_pool.alloc_capacity);
        }
        memcpy_t(moveptr_t(memory_pool.allocs, memory_pool.alloc_size), memory_pool.alloc_capacity, allocation, 1);
        memory_pool.alloc_size++;
    }

    void* memory_pool_allocate(memory_pool_t& memory_pool, const usize size) {
        for (usize i = 0; i < memory_pool.alloc_size ; i++) {
            memory_pool_alloc_t* allocation = &memory_pool.allocs[i];

            // check if allocation is free to use and has at least enough space for usage
            if (allocation->free && allocation->size >= size) {
                allocation->free = false;
                allocation->used_size = size;
                return allocation->data;
            }

            // check if used allocation has additional free space for usage
            // if yes - than create additional allocation from used allocation
            if (allocation->size - allocation->used_size >= size) {
                memory_pool_alloc_t new_allocation;
                new_allocation.free = false;
                new_allocation.size = allocation->size - allocation->used_size;
                new_allocation.used_size = size;
                new_allocation.data = moveptr(allocation->data, allocation->used_size);
                allocation->size = allocation->used_size;
                memory_pool_add_allocation(memory_pool, &new_allocation);
                return new_allocation.data;
            }
        }

        // check if we have enough space in memory pool
        if (memory_pool.last_ptr >= memory_pool.max_ptr) {
            return nullptr;
        }

        memory_pool_alloc_t new_allocation;
        new_allocation.free = false;
        new_allocation.size = size;
        new_allocation.used_size = size;
        new_allocation.data = memory_pool.last_ptr;
        memory_pool_add_allocation(memory_pool, &new_allocation);
        memory_pool.last_ptr = moveptr(memory_pool.last_ptr, size);
        return new_allocation.data;
    }

    void memory_pool_free(const memory_pool_t& memory_pool, const void *addr) {
        for (usize i = 0 ; i < memory_pool.alloc_size ; i++) {
            if (memory_pool_alloc_t& allocation = memory_pool.allocs[i]; allocation.data == addr) {
                allocation.free = true;
                allocation.used_size = 0;
            }
        }
    }

    memory_t global_memory;

    void memory_init() {
        auto [ram_total_bytes, ram_free_bytes] = memory_info_get();

        constexpr usize bump_size = SF_MB(1);
        global_memory.memory_bump = memory_bump_init(bump_size);

        // memory size = 20% of RAM but less than 1GB
        auto pool_size = static_cast<usize>(static_cast<double>(ram_free_bytes) * 0.2);
        if (pool_size >= SF_MB(1)) {
            pool_size = SF_MB(1);
        }

        // pool_alloc_size value can be estimated and tested
        constexpr usize pool_alloc_size = 100;
        global_memory.memory_pool = memory_pool_init(pool_size, pool_alloc_size);
    }

    void memory_free() {
        memory_bump_free(global_memory.memory_bump);
        memory_pool_free(global_memory.memory_pool);
    }

    u64 string_hash(const string_t& string) {
        constexpr std::hash<string_t> hasher;
        return hasher(string);
    }

    date_time_t date_time_get_current() {
        const ::time_t t = time(nullptr);
        const tm* lt = localtime(&t);
        date_time_t date_time = {};
        date_time.y = lt->tm_year;
        date_time.m = lt->tm_mon;
        date_time.d = lt->tm_mday;
        date_time.h = lt->tm_hour;
        date_time.min = lt->tm_min;
        date_time.s = lt->tm_sec;
        date_time.ms = lt->tm_sec * 1000;
        return date_time;
    }

    time_t time_get_current() {
        const ::time_t t = time(nullptr);
        const tm* lt = localtime(&t);
        time_t time = {};
        time.ms = lt->tm_sec * 1000;
        time.s = lt->tm_sec;
        time.min = lt->tm_min;
        time.h = lt->tm_hour;
        return time;
    }

    float time_get_current_ms() {
        const ::time_t t = time(nullptr);
        const tm* lt = localtime(&t);
        return lt->tm_sec * 1000;
    }

    mutex_t mutex_init() {
        mutex_t mutex;
        pthread_mutex_init(&mutex.handle, nullptr);
        return mutex;
    }

    void mutex_free(mutex_t& mutex) {
        pthread_mutex_destroy(&mutex.handle);
    }

    void mutex_lock(mutex_t& mutex) {
        pthread_mutex_lock(&mutex.handle);
    }

    void mutex_unlock(mutex_t& mutex) {
        pthread_mutex_unlock(&mutex.handle);
    }

    condition_var_t condition_var_init() {
        condition_var_t condition_var;
        pthread_cond_init(&condition_var.handle, nullptr);
        return condition_var;
    }

    void condition_var_free(condition_var_t& condition_var) {
        pthread_cond_destroy(&condition_var.handle);
    }

    void condition_var_wait(condition_var_t& condition_var, mutex_t& mutex) {
        pthread_mutex_lock(&mutex.handle);
        int result = pthread_cond_wait(&condition_var.handle, &mutex.handle);
        SF_ASSERT(result == 0, "Unable to wait a pthread with condition var");
    }

    void condition_var_notify(condition_var_t& condition_var) {
        int result = pthread_cond_signal(&condition_var.handle);
        SF_ASSERT(result == 0, "Unable to notify a pthread with condition var");
    }

    u32 thread_get_pid() {
        return getpid();
    }

    u32 thread_get_tid() {
        return pthread_self();
    }

    void thread_sleep(u32 ms) {
        int result = usleep(ms * 1000);
        SF_ASSERT(result == 0, "Unable to sleep this thread");
    }

    void thread_yield() {
        int result = sched_yield();
        SF_ASSERT(result == 0, "Unable to yield this thread");
    }

    void thread_exit() {
        pthread_exit(0);
    }

    thread_t thread_init(const char *name, SF_THREAD_PRIORITY priority) {
        thread_t thread;
        thread.name = name;
        thread.priority = priority;
        signal(SIGABRT, [](int) {
            pthread_exit(0);
        });
        return thread;
    }

    void thread_free(const thread_t &thread) {
        int result = pthread_kill(thread.handle, SIGABRT);
        SF_ASSERT(result == 0, "Unable to free a Thread=%s", thread.name);
        signal(SIGABRT, SIG_DFL);
    }

    static void* thread_run_function(void* thread) {
        thread_t& t = *static_cast<thread_t*>(thread);
#if defined(SF_DEBUG)
        t.pid = thread_get_pid();
        t.tid = thread_get_tid();
        thread_set_info(t);
#endif
        t.run_function(t.run_args);
        return nullptr;
    }

    void thread_run(thread_t &thread) {
        int result = pthread_create(&thread.handle, nullptr, thread_run_function, &thread);
        SF_ASSERT(result == 0, "Unable to create a Thread=%s", thread.name);
    }

    void thread_detach(const thread_t &thread) {
        int result = pthread_detach(thread.handle);
        SF_ASSERT(result == 0, "Unable to detach a pthread %s", thread.name);
    }

    void thread_join(const thread_t &thread) {
        int result = pthread_join(thread.handle, nullptr);
        SF_ASSERT(result == 0, "Unable to join to a pthread %s", thread.name);
    }

    thread_pool_t thread_pool_init(const usize thread_size, const usize task_size, const char* name, const SF_THREAD_PRIORITY priority) {
        thread_pool_t thread_pool;
        thread_pool.threads = static_cast<thread_t*>(memory_pool_allocate(global_memory.memory_pool, sizeof(thread_t) * thread_size));
        thread_pool.thread_size = thread_size;
        thread_pool.tasks = circular_buffer_init<task_t>(task_size);
        thread_pool.mutex_wake = mutex_init();
        thread_pool.condition_var_wake = condition_var_init();
        for (int i = 0; i < thread_size ; i++) {
            thread_t& thread = thread_pool.threads[i];
            thread = thread_init(name, priority);
        }
        return thread_pool;
    }

    void thread_pool_run(thread_pool_t& thread_pool) {
        thread_pool.running = true;
        const usize thread_size = thread_pool.thread_size;
        for (int i = 0 ; i < thread_size ; i++) {
            thread_t& thread = thread_pool.threads[i];
            thread.run_args = &thread_pool;
            thread.run_function = [] (void* args) {
                auto& thread_pool = *static_cast<thread_pool_t*>(args);
                while (thread_pool.running) {
                    if (task_t task; circular_buffer_pop(thread_pool.tasks, task)) {
                        task();
                    }
                    else {
                        // because we don't want to overhead cpu core with thread while loop
                        // it's better to simply put thread into wait, until it is notified by outer thread with wake condition variable
                        condition_var_wait(thread_pool.condition_var_wake, thread_pool.mutex_wake);
                    }
                }
            };
            thread_run(thread);
            thread_detach(thread);
        }
    }

    void thread_pool_free(thread_pool_t& thread_pool) {
        thread_pool.running = false;
        const usize thread_size = thread_pool.thread_size;
        for (int i = 0 ; i < thread_size ; i++) {
            thread_free(thread_pool.threads[i]);
        }
    }

    void thread_pool_add(thread_pool_t& thread_pool, const task_t& task) {
        // try to push a new task until it is pushed
        while (!circular_buffer_push<task_t>(thread_pool.tasks, task)) {
            condition_var_notify(thread_pool.condition_var_wake);
            thread_yield();
        }
        condition_var_notify(thread_pool.condition_var_wake);
    }

}

// SF_WINDOWS_BEGIN
#if defined(SF_WINDOWS)

#include <windows.h>
#include <memoryapi.h>
#include <processthreadsapi.h>
#include <sysinfoapi.h>

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

    memory_info_t memory_info_get() {
        memory_info_t memory_info;
        MEMORYSTATUSEX memory_status;
        memory_status.dwLength = sizeof(memory_status);
        GlobalMemoryStatusEx(&memory_status);
        memory_info.ram_total_bytes = memory_status.ullTotalVirtual;
        memory_info.ram_free_bytes = memory_status.ullAvailVirtual;
        return memory_info;
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

    void thread_set_info(thread_t& thread) {
        HANDLE handle = (HANDLE) thread.handle;

        // set affinity mask
        DWORD_PTR affinityMask = 1ull << 0;
        DWORD_PTR affinity_result = SetThreadAffinityMask(handle, affinityMask);
    //    SF_ASSERT(affinity_result > 0, "Failed to set thread affinity mask on Windows!")

        // set priority
        BOOL priority_result = SetThreadPriority(handle, SF_THREAD_PRIORITY_CODE[thread.priority]);
    //    SF_ASSERT(priority_result != 0, "Failed to set thread priority on Windows!");

        // set name
        HRESULT hr = SetThreadName(handle, thread.name);
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

    memory_info_t memory_info_get() {
        struct sysinfo sys_info {};
        sysinfo(&sys_info);
        memory_info_t memory_info;
        memory_info.ram_total_bytes = sys_info.totalram;
        memory_info.ram_free_bytes = sys_info.freeram;
        return memory_info;
    }

    static const int SF_THREAD_PRIORITY_CODE[SF_THREAD_PRIORITY_COUNT] = {
        1, // SF_THREAD_PRIORITY_LOWEST
        2, // SF_THREAD_PRIORITY_NORMAL
        3, // SF_THREAD_PRIORITY_HIGHEST
    };

    void thread_set_info(thread_t& thread) {
        int result;

        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        size_t cpusetsize = sizeof(cpuset);
        CPU_SET(thread.tid, &cpuset);

        // Set affinity mask
        result = sched_setaffinity(thread.pid, cpusetsize, &cpuset);
    //    SF_ASSERT(result != 0, "Failed to set thread affinity mask on Linux!");

        // Set priority
        sched_param param {};
        param.sched_priority = SF_THREAD_PRIORITY_CODE[thread.priority];
        result = pthread_setschedparam(thread.handle, SCHED_OTHER, &param);
    //    SF_ASSERT(result != 0, "Failed to set thread priority on Linux!");

        // Set name
        result = pthread_setname_np(thread.handle, thread.name);
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

    MemoryInfo Memory::get_memory_info() {
        struct sysinfo sys_info {};
        sysinfo(&sys_info);
        MemoryInfo memory_info;
        memory_info.ram_total_bytes = sys_info.totalram;
        memory_info.ram_free_bytes = sys_info.freeram;
        return memory_info;
    }

    static const int SF_THREAD_PRIORITY_CODE[SF_THREAD_PRIORITY_COUNT] = {
        1, // SF_THREAD_PRIORITY_LOWEST
        2, // SF_THREAD_PRIORITY_NORMAL
        3, // SF_THREAD_PRIORITY_HIGHEST
    };

    void Thread::set_thread_info() {
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