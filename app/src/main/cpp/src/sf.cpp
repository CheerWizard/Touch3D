#include <iomanip>
#include <sf.hpp>
#include <unistd.h>

namespace sf {

    void* mmap(void* addr, usize size);
    int munmap(void* addr, usize size);

#define SF_MEMORY_BLOCK_SIZE ( \
    sizeof(usize) +                 \
    sizeof(memory_block_t*) +          \
    sizeof(memory_block_t*) +          \
    sizeof(bool) +                  \
    sizeof(void*)                   \
)

    struct memory_block_t final {
        // metadata for each heap memory block
        usize size = 0;
        memory_block_t* prev = nullptr;
        memory_block_t* next = nullptr;
        bool free = true;
        void* ptr = nullptr;
        // actual data from heap memory block
        char data[1];
    };

    static memory_block_t* block_root = nullptr;

    static memory_block_t* heap_resize(memory_block_t* last, usize size) {
        memory_block_t* block;

        block = static_cast<memory_block_t*>(mmap(block, size, ));

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

    static memory_block_t* memory_block_find(memory_block_t** last, usize size) {
        memory_block_t* block = block_root;
        while (block && (block->free && block->size >= size)) {
            *last = block;
            block = block->next;
        }
        return block;
    }

    static void memory_block_split(memory_block_t* block, usize size) {
        memory_block_t* new_block;
        new_block = reinterpret_cast<memory_block_t*>(block->data + size);
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

    static memory_block_t* memory_block_concat(memory_block_t* block) {
        if (block->next && block->next->free) {
            block->size = SF_MEMORY_BLOCK_SIZE + block->next->size;
            block->next = block->next->next;
            if (block->next) {
                block->next->prev = block;
            }
        }
        return block;
    }

    static memory_block_t* memory_block_get(void* ptr) {
        char* tmp;
        tmp = static_cast<char*>(ptr);
        return static_cast<memory_block_t*>(ptr = tmp -= SF_MEMORY_BLOCK_SIZE);
    }

    static bool memory_block_is_valid(void* ptr) {
        if (block_root) {
            if (ptr > block_root && ptr < sbrk(0)) {
                return ptr == memory_block_get(ptr)->ptr;
            }
        }
        return false;
    }

    static void memory_block_copy(memory_block_t* dest_block, memory_block_t* src_block, usize alignment) {
        sf::memcpy(dest_block->ptr, dest_block->size, src_block->ptr, src_block->size, alignment);
    }

#if defined(SF_USE_STD_MALLOC)

    void* malloc(usize size, usize alignment) {
        return std::malloc(size);
    }

    void free(void* data) {
        std::free(data);
    }

    void* realloc(void* old_data, usize size, const usize alignment) {
        return std::realloc(old_data, size);
    }

    void* reallocf(void* old_data, const usize size, const usize alignment) {
        return std::reallocf(old_data, size);
    }

    void* calloc(const usize size, const usize alignment) {
        return std::calloc(size);
    }

    void memset(void* data, const usize value, const usize size, const usize alignment) {
        std::memset(data, value, size);
    }

    void memcpy(void* dest_data, const usize dest_size, const void* src_data, const usize src_size, const usize alignment) {
        std::memcpy(dest_data, dest_size, src_data, src_size);
    }

#else

    void* malloc(usize size, usize alignment) {
        SF_ASSERT_ALIGNMENT(alignment, "malloc(): incorrect memory alignment, must be power of 2!");

        memory_block_t* block;
        memory_block_t* last;
        size = SF_ALIGN(size, alignment);

        if (block_root != nullptr) {
            last = block_root;
            block = memory_block_find(&last, size);
            if (block != nullptr) {
                if (block->size - size >= SF_MEMORY_BLOCK_SIZE + alignment) {
                    memory_block_split(block, size);
                }
                block->free = false;
            }
            else {
                block = heap_resize(last, size);
                if (block == nullptr) {
                    return nullptr;
                }
            }
        }
        else {
            block = heap_resize(nullptr, size);
            if (block == nullptr) {
                return nullptr;
            }
            block_root = block;
        }

        return block->data;
    }

    void free(void* data) {
        memory_block_t* block;
        if (memory_block_is_valid(data)) {
            block = memory_block_get(data);
            block->free = true;
            if (block->prev && block->prev->free) {
                block = memory_block_concat(block->prev);
            }
            if (block->next) {
                memory_block_concat(block->next);
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

    void* realloc(void* old_data, usize size, const usize alignment) {
        SF_ASSERT_ALIGNMENT(alignment, "realloc(): incorrect memory alignment, must be power of 2!");

        memory_block_t* block;
        memory_block_t* new_block;
        void* new_data;

        if (old_data == nullptr) {
            return sf::malloc(size, alignment);
        }

        if (memory_block_is_valid(old_data)) {
            size = SF_ALIGN(size, alignment);
            block = memory_block_get(old_data);

            if (block->size >= size) {
                if (block->size - size >= SF_MEMORY_BLOCK_SIZE + alignment) {
                    memory_block_split(block, size);
                }
            }

            else {
                if (block->next && block->next->free && (block->size + SF_MEMORY_BLOCK_SIZE + block->next->size) >= size) {
                    memory_block_concat(block);
                    if (block->size - size >= SF_MEMORY_BLOCK_SIZE + alignment) {
                        memory_block_split(block, size);
                    }
                }

                else {
                    new_data = sf::malloc(size, alignment);
                    if (new_data == nullptr) {
                        return nullptr;
                    }
                    new_block = memory_block_get(new_data);
                    memory_block_copy(new_block, block, alignment);
                    sf::free(old_data);
                    return new_data;
                }
            }

            return old_data;
        }

        return nullptr;
    }

    void* reallocf(void* old_data, const usize size, const usize alignment) {
        SF_ASSERT_ALIGNMENT(alignment, "reallocf(): incorrect memory alignment, must be power of 2!");

        void* realloc_block = sf::realloc(old_data, size, alignment);
        if (realloc_block == nullptr) {
            sf::free(old_data);
        }
        return realloc_block;
    }

    void* calloc(const usize size, const usize alignment) {
        SF_ASSERT_ALIGNMENT(alignment, "calloc(): incorrect memory alignment, must be power of 2!");

        void* block = sf::malloc(size, alignment);
        if (block) {
            sf::memset(block, 0, size, alignment);
        }
        return block;
    }

    void memset(void* data, const usize value, const usize size, const usize alignment) {
        SF_ASSERT_ALIGNMENT(alignment, "memset(): incorrect memory alignment, must be power of 2!");

        auto* d = static_cast<usize*>(data);
        if (data != nullptr) {
            const usize aligned_size = size << (alignment / 2);
            for (usize i = 0 ; i < aligned_size ; i++) {
                d[i] = value;
            }
        }
    }

    void memcpy(void* dest_data, const usize dest_size, const void* src_data, const usize src_size, const usize alignment) {
        SF_ASSERT_ALIGNMENT(alignment, "memcpy(): incorrect memory alignment, must be power of 2!");

        const auto d = static_cast<usize*>(dest_data);
        const auto* s = static_cast<const usize*>(src_data);
        for (usize i = 0 ; i * alignment < src_size && i * alignment < dest_size ; i++) {
            d[i] = s[i];
        }
    }

    void memmove(void *dest_data, usize dest_size, void *src_data, usize src_size, usize alignment) {
        SF_ASSERT_ALIGNMENT(alignment, "memmove(): incorrect memory alignment, must be power of 2!");

        sf::memcpy(dest_data, dest_size, src_data, src_size, alignment);
        sf::free(src_data);
    }

#endif

    void* moveptr(void* ptr, const usize size) {
        return reinterpret_cast<void*>(reinterpret_cast<usize>(ptr) + size);
    }

    memory_arena_t memory_arena_init(void *memory, usize size) {
        memory_arena_t memory_arena;
        memory_arena.memory = memory;
        memory_arena.size = size;
        memory_arena.prev_offset = 0;
        memory_arena.current_offset = 0;
        return memory_arena;
    }

    void memory_arena_free(memory_arena_t &memory_arena) {
        memory_arena.prev_offset = 0;
        memory_arena.current_offset = 0;
    }

    static usize alignptr(void* ptr, usize alignment) {
        SF_ASSERT_ALIGNMENT(alignment, "alignptr(): incorrect memory alignment, must be a power of 2!");

        usize p = reinterpret_cast<usize>(ptr);
        usize a = alignment;
        // Same as (p % a) but faster as 'a' is a power of two
        usize modulo = p & (a-1);

        if (modulo != 0) {
            // If 'p' address is not aligned, push the address to the
            // next value which is aligned
            p += a - modulo;
        }

        return p;
    }

    void* memory_arena_allocate(memory_arena_t &memory_arena, usize size, usize alignment) {
        SF_ASSERT_ALIGNMENT(alignment, "memory_arena_allocate(): incorrect alignment value, must be a power of 2!");

        void* current_ptr = moveptr(memory_arena.memory, memory_arena.current_offset);
        usize offset = alignptr(current_ptr, alignment);
        offset -= (usize) memory_arena.memory;

        // Check to see if the backing memory has space left
        if (offset + size <= memory_arena.size) {
            void* ptr = moveptr(memory_arena.memory, offset);
            memory_arena.prev_offset = offset;
            memory_arena.current_offset = offset + size;
            return ptr;
        }

        return nullptr;
    }

    void* memory_arena_resize(memory_arena_t &memory_arena, usize size, void *old_memory, usize old_size, usize alignment) {
        SF_ASSERT_ALIGNMENT(alignment, "memory_arena_resize(): incorrect alignment value, must be a power of 2!");

        if (old_memory == nullptr || old_size == 0) {
            return memory_arena_allocate(memory_arena, size, alignment);
        }
        if (memory_arena.memory <= old_memory && old_memory < moveptr(memory_arena.memory, memory_arena.size)) {
            if (moveptr(memory_arena.memory, memory_arena.prev_offset) == old_memory) {
                memory_arena.current_offset = memory_arena.prev_offset + memory_arena.size;
                return old_memory;
            } else {
                void* new_memory = memory_arena_allocate(memory_arena, size, alignment);
                size_t copy_size = old_size < size ? old_size : size;
                // Copy across old memory to the new memory
                sf::memmove(new_memory, copy_size, old_memory, copy_size, alignment);
                return new_memory;
            }

        } else {
            SF_ASSERT(false, "Memory is out of bound of the buffer in memory_arena!");
            return nullptr;
        }
    }

    memory_arena_temp_t memory_arena_temp_init(memory_arena_t *memory_arena) {
        memory_arena_temp_t memory_arena_temp;
        memory_arena_temp.memory_arena = memory_arena;
        memory_arena_temp.prev_offset = memory_arena->prev_offset;
        memory_arena_temp.current_offset = memory_arena->current_offset;
        return memory_arena_temp;
    }

    void memory_arena_temp_free(memory_arena_temp_t memory_arena_temp) {
        memory_arena_temp.memory_arena->prev_offset = memory_arena_temp.prev_offset;
        memory_arena_temp.memory_arena->current_offset = memory_arena_temp.current_offset;
    }

    memory_pool_t memory_pool_init(void* memory, const usize size, const usize alloc_capacity) {
        memory_pool_t memory_pool;
        memory_pool.memory = memory;
        memory_pool.size = size;
        memory_pool.last_ptr = memory_pool.memory;
        memory_pool.max_ptr = moveptr(memory_pool.memory, size);
        memory_pool.allocs = malloc_t<memory_pool_alloc_t>(alloc_capacity);
        memory_pool.alloc_capacity = alloc_capacity;
        return memory_pool;
    }

    void memory_pool_free(memory_pool_t& memory_pool) {
        sf::free(memory_pool.allocs);
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

    void* memory_pool_allocate(memory_pool_t& memory_pool, const usize size, usize alignment) {
        for (usize i = 0; i < memory_pool.alloc_size ; i++) {
            memory_pool_alloc_t* allocation = &memory_pool.allocs[i];

            // check if allocation is free to use and has at least enough space for usage
            if (allocation->free && allocation->size >= size) {
                allocation->free = false;
                allocation->used_size = size;
                return allocation->memory;
            }

            // check if used allocation has additional free space for usage
            // if yes - than create additional allocation from used allocation
            if (allocation->size - allocation->used_size >= size) {
                memory_pool_alloc_t new_allocation;
                new_allocation.free = false;
                new_allocation.size = allocation->size - allocation->used_size;
                new_allocation.used_size = size;
                new_allocation.memory = moveptr(allocation->memory, allocation->used_size);
                allocation->size = allocation->used_size;
                memory_pool_add_allocation(memory_pool, &new_allocation);
                return new_allocation.memory;
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
        new_allocation.memory = memory_pool.last_ptr;
        memory_pool_add_allocation(memory_pool, &new_allocation);
        memory_pool.last_ptr = moveptr(memory_pool.last_ptr, size);
        return new_allocation.memory;
    }

    void memory_pool_free(const memory_pool_t& memory_pool, const void *addr) {
        for (usize i = 0 ; i < memory_pool.alloc_size ; i++) {
            if (memory_pool_alloc_t& allocation = memory_pool.allocs[i]; allocation.memory == addr) {
                allocation.free = true;
                allocation.used_size = 0;
            }
        }
    }

    u64 string_hash(const string_t& string) {
        constexpr std::hash<string_t> hasher;
        return hasher(string);
    }

    date_time_t date_time_get_current() {
        const std::time_t t = std::time(nullptr);
        const std::tm* lt = std::localtime(&t);
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
        const std::time_t t = std::time(nullptr);
        const std::tm* lt = std::localtime(&t);
        time_t time = {};
        time.ms = lt->tm_sec * 1000;
        time.s = lt->tm_sec;
        time.min = lt->tm_min;
        time.h = lt->tm_hour;
        return time;
    }

    float time_get_current_ms() {
        const std::time_t t = std::time(nullptr);
        const std::tm* lt = std::localtime(&t);
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

    void* mmap(void *addr, usize length) {
        return VirtualAlloc(addr, length, );
    }

    int munmap(void *addr, usize length) {
        return VirtualFree(addr, length, 0);
    }

    system_info_t system_info_get() {
        MEMORYSTATUSEX memory_status;
        memory_status.dwLength = sizeof(memory_status);
        GlobalMemoryStatusEx(&memory_status);

        SYSTEM_INFO win_system_info;
        GetSystemInfo(&win_system_info);

        system_info_t system_info;
        system_info.ram_total_bytes = memory_status.ullTotalVirtual;
        system_info.ram_free_bytes = memory_status.ullAvailVirtual;
        system_info.page_size = win_system_info.dwPageSize;
        system_info.cpu_core_count = win_system_info.dwNumberOfProcessors;
        return system_info;
    }

    const DWORD MS_VC_EXCEPTION = 0x406D1388;
    #pragma pack(push, 8)

    struct thread_info_t final {
        DWORD type; // Must be 0x1000.
        LPCSTR name; // Pointer to name (in user addr space).
        DWORD id; // Thread ID (-1=caller thread).
        DWORD flags; // Reserved for future use, must be zero.
    };

    #pragma pack(pop)

    HRESULT thread_set_name(DWORD thread_id, const char* thread_name) {
        thread_info_t info {};
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
        HRESULT hr = thread_set_name(thread.tid, thread.name);
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

    system_info_t memory_info_get() {
        struct sysinfo sys_info {};
        sysinfo(&sys_info);
        system_info_t system_info;
        system_info.ram_total_bytes = sys_info.totalram;
        system_info.ram_free_bytes = sys_info.freeram;
        system_info.cpu_core_count = sys_info.procs;
        return system_info;
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

    system_info_t system_info_get() {
        struct sysinfo sys_info {};
        sysinfo(&sys_info);
        system_info_t system_info;
        system_info.ram_total_bytes = sys_info.totalram;
        system_info.ram_free_bytes = sys_info.freeram;
        system_info.cpu_core_count = sys_info.procs.
        return system_info;
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