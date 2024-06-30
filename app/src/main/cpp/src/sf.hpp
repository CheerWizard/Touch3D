#pragma once

#include <cstdint>
#include <cassert>
#include <csignal>
#include <cstdio>
#include <functional>
#include <pthread.h>
#include <cstring>
#include <string>
#include <utility>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <map>
#include <set>

using u8 = uint8_t;
using i8 = int8_t;
using u16 = uint16_t;
using i16 = int16_t;
using u32 = uint32_t;
using i32 = int32_t;
using u64 = uint64_t;
using i64 = int64_t;

#if defined(__arm__) \
|| defined(__thumb__)    \
|| defined(__TARGET_ARCH_ARM)     \
|| defined(__TARGET_ARCH_THUMB)   \
|| defined(_ARM)    \
|| defined(_M_ARM)  \
|| defined(_M_ARMT)    \
|| defined(__arm)    \
|| defined(_ARM)    \
|| defined(__bfin)    \
|| defined(__BFIN__)    \
|| defined(i386)    \
|| defined(__i386)    \
|| defined(__i386__)  \
|| defined(__IA32__)  \
|| defined(_M_I86)  \
|| defined(_M_IX86)  \
|| defined(__X86__)  \
|| defined(_X86_)  \
|| defined(__THW_INTEL__)  \
|| defined(__INTEL__) \
|| defined(__386)

#define SF_32

#elif defined(__amd64__) \
|| defined(__amd64)   \
|| defined(__x86_64__)     \
|| defined(__x86_64)\
|| defined(_M_X64)   \
|| defined(_M_AMD64) \
|| defined(__alpha__) \
|| defined(__alpha) \
|| defined(_M_ALPHA) \
|| defined(__aarch64__)\
|| defined(__ia64__)  \
|| defined(_IA64)  \
|| defined(__IA64__)  \
|| defined(__ia64)  \
|| defined(_M_IA64)  \
|| defined(__itanium__)

#define SF_64

#else

#error "Unsupported CPU architecture"

#endif

#if defined(_WIN32)

#if defined(_WIN64)

#define SF_WINDOWS

#else

#error "Windows x86 is not supported!"

#endif

#elif defined(__APPLE__) \
|| defined(__MACH__)

#include <TargetConditionals.h>

#if TARGET_IPHONE_SIMULATOR == 1

#define SF_IOS_SIMULATOR

#elif TARGET_OS_IPHONE == 1

#define SF_IOS

#elif TARGET_OS_MAC == 1

#define SF_MACOS

#else

#error "Unknown Apple platform!"

#endif

#elif defined(__ANDROID__)

#define SF_ANDROID

#elif defined(__linux__)

#define SF_LINUX

#else

#error "Unsupported platform!"

#endif

#if defined(SF_16)

using isize = i16;
using usize = u16;

#elif defined(SF_32)

using isize = i32;
using usize = u32;

#elif defined(SF_64)

using isize = i64;
using usize = u64;

#endif

#if defined(_MSC_VER)

#define SF_DLL_EXPORT __declspec(dllexport)
#define SF_DLL_IMPORT __declspec(dllimport)

#elif defined(__GNUC__)

#define SF_DLL_EXPORT __attribute__((visibility("default")))
#define SF_DLL_IMPORT __attribute__((visibility("default")))

#else

// Most compilers export/import all the symbols by default. We hope for the best here.
#define SF_DLL_EXPORT
#define SF_DLL_IMPORT
#pragma warning Unknown dynamic link import/export semantics.

#endif

// used only during build of library itself into dll
#if defined(SF_DLL_BUILD)

#define SF_API SF_DLL_EXPORT

// enable to use library as a dynamically linked library
#elif defined(SF_DLL)

#define SF_API SF_DLL_IMPORT

#else

// by default library is used as a statically linked library or directly included as a header
#define SF_API

#endif

#if defined(SF_WINDOWS)
#define SF_FILENAME (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else
#define SF_FILENAME (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif

#ifdef _MSC_VER

#define SF_DEBUG_BREAK() __debugbreak

#else

#ifdef __cplusplus
extern "C" {
#endif

#define SF_DEBUG_BREAK_USE_TRAP_INSTRUCTION 1
#define SF_DEBUG_BREAK_USE_BUILTIN_TRAP     2
#define SF_DEBUG_BREAK_USE_SIGTRAP          3

#if defined(__i386__) || defined(__x86_64__)

#define SF_DEBUG_BREAK_IMPL SF_DEBUG_BREAK_USE_TRAP_INSTRUCTION

__inline__ static void trap_instruction()
{
    __asm__ volatile("int $0x03");
}

#elif defined(__thumb__)
#define SF_DEBUG_BREAK_IMPL SF_DEBUG_BREAK_USE_TRAP_INSTRUCTION
__attribute__((always_inline))
__inline__ static void trap_instruction()
{
	/* See 'arm-linux-tdep.c' in GDB source.
	 * Both instruction sequences below work. */
#if 1
	/* 'eabi_linux_thumb_le_breakpoint' */
	__asm__ volatile(".inst 0xde01");
#else
	/* 'eabi_linux_thumb2_le_breakpoint' */
	__asm__ volatile(".inst.w 0xf7f0a000");
#endif

	/* Known problem:
	 * After a breakpoint hit, can't 'stepi', 'step', or 'continue' in GDB.
	 * 'step' would keep getting stuck on the same instruction.
	 *
	 * Workaround: use the new GDB commands 'debugbreak-step' and
	 * 'debugbreak-continue' that become available
	 * after you source the script from GDB:
	 *
	 * $ gdb -x debugbreak-gdb.py <... USUAL ARGUMENTS ...>
	 *
	 * 'debugbreak-step' would jump over the breakpoint instruction with
	 * roughly equivalent of:
	 * (gdb) set $instruction_len = 2
	 * (gdb) tbreak *($pc + $instruction_len)
	 * (gdb) jump   *($pc + $instruction_len)
	 */
}
#elif defined(__arm__) && !defined(__thumb__)
	#define SF_DEBUG_BREAK_IMPL SF_DEBUG_BREAK_USE_TRAP_INSTRUCTION
__attribute__((always_inline))
__inline__ static void trap_instruction()
{
	/* See 'arm-linux-tdep.c' in GDB source,
	 * 'eabi_linux_arm_le_breakpoint' */
	__asm__ volatile(".inst 0xe7f001f0");
	/* Known problem:
	 * Same problem and workaround as Thumb mode */
}
#elif defined(__aarch64__) && defined(__APPLE__)
	#define SF_DEBUG_BREAK_IMPL SF_DEBUG_BREAK_USE_BULTIN_DEBUGTRAP
#elif defined(__aarch64__)
	#define SF_DEBUG_BREAK_IMPL SF_DEBUG_BREAK_USE_TRAP_INSTRUCTION
__attribute__((always_inline))
__inline__ static void trap_instruction()
{
	/* See 'aarch64-tdep.c' in GDB source,
	 * 'aarch64_default_breakpoint' */
	__asm__ volatile(".inst 0xd4200000");
}
#elif defined(__powerpc__)
	/* PPC 32 or 64-bit, big or little endian */
	#define SF_DEBUG_BREAK_IMPL SF_DEBUG_BREAK_USE_TRAP_INSTRUCTION
__attribute__((always_inline))
__inline__ static void trap_instruction()
{
	/* See 'rs6000-tdep.c' in GDB source,
	 * 'rs6000_breakpoint' */
	__asm__ volatile(".4byte 0x7d821008");

	/* Known problem:
	 * After a breakpoint hit, can't 'stepi', 'step', or 'continue' in GDB.
	 * 'Step' stuck on the same instruction ("twge r2,r2").
	 *
	 * The workaround is the same as ARM Thumb mode: use debugbreak-gdb.py
	 * or manually jump over the instruction. */
}
#elif defined(__riscv)
	/* RISC-V 32 or 64-bit, whether the "C" extension
	 * for compressed, 16-bit instructions are supported or not */
	#define SF_DEBUG_BREAK_IMPL SF_DEBUG_BREAK_USE_TRAP_INSTRUCTION
__attribute__((always_inline))
__inline__ static void trap_instruction()
{
	/* See 'riscv-tdep.c' in GDB source,
	 * 'riscv_sw_breakpoint_from_kind' */
	__asm__ volatile(".4byte 0x00100073");
}
#else
	#define SF_DEBUG_BREAK_IMPL SF_DEBUG_BREAK_USE_SIGTRAP
#endif


#ifndef SF_DEBUG_BREAK_IMPL
#error "SF_DEBUG_BREAK is not supported on this target"
#elif SF_DEBUG_BREAK_IMPL == SF_DEBUG_BREAK_USE_TRAP_INSTRUCTION
__attribute__((always_inline))
__inline__ static void SF_DEBUG_BREAK()
{
    trap_instruction();
}
#elif SF_DEBUG_BREAK_IMPL == SF_DEBUG_BREAK_USE_BULTIN_DEBUGTRAP
__attribute__((always_inline))
__inline__ static void SF_DEBUG_BREAK()
{
	__builtin_debugtrap();
}
#elif SF_DEBUG_BREAK_IMPL == SF_DEBUG_BREAK_USE_BUILTIN_TRAP
__attribute__((always_inline))
__inline__ static void SF_DEBUG_BREAK()
{
	__builtin_trap();
}
#elif SF_DEBUG_BREAK_IMPL == SF_DEBUG_BREAK_USE_SIGTRAP
#include <signal.h>
__attribute__((always_inline))
__inline__ static void SF_DEBUG_BREAK()
{
	raise(SIGTRAP);
}
#else
#error "Invalid SF_DEBUG_BREAK_IMPL value"
#endif

#ifdef __cplusplus
}
#endif

#endif // _MSC_VER

constexpr unsigned long long operator ""_KB(unsigned long long bytes) {
    return bytes * 1024;
}

constexpr unsigned long long operator ""_MB(unsigned long long bytes) {
    return bytes * 1024_KB;
}

constexpr unsigned long long operator ""_GB(unsigned long long bytes) {
    return bytes * 1024_MB;
}

constexpr unsigned long long operator ""_TB(unsigned long long bytes) {
    return bytes * 1024_GB;
}

// x - target value to align
// a - alignment in bytes
#define SF_ALIGN(x,a) (((((x)-1)>>((a)/2))<<((a)/2))+(a))
#define SF_ALIGNMENT sizeof(void*)
#define SF_ASSERT_ALIGNMENT(x, ...) SF_ASSERT((x & (x-1)) == 0, ##__VA_ARGS__)

#if defined(SF_DEBUG)

#if defined(SF_ANDROID)

#define SF_ASSERT(x, ...) sf::__assert(SF_FILENAME, __FUNCTION__, __LINE__, x, 0, ##__VA_ARGS__)

#else

#define SF_ASSERT(x, ...) sf::__assert(SF_FILENAME, __FUNCTION__, __LINE__,  x, ##__VA_ARGS__)

#endif

#else

#define SF_ASSERT(x, ...)

#endif

#define SF_RGB(r, g, b) (b) + ((g)<<8) + ((r)<<16)

enum SF_THREAD_PRIORITY {
    SF_THREAD_PRIORITY_LOWEST,
    SF_THREAD_PRIORITY_NORMAL,
    SF_THREAD_PRIORITY_HIGHEST,

    SF_THREAD_PRIORITY_COUNT
};

namespace sf {

    template<typename... Args>
    void __assert(const char* filename, const char* function, int line, bool condition, const char* msg, Args... args) {
        char fmt_buffer[256] = {};
        char text_buffer[256] = {};
        const char* fmt = "\nAssertion Failed in %s -> %s(%i line):\n%s";
        sprintf(
                fmt_buffer,
                fmt,
                filename, function, line,
                msg
        );
        sprintf(text_buffer, fmt_buffer, args...);
        assert(condition && text_buffer);
    }

    SF_API int brk(void* addr);
    SF_API void* sbrk(intptr_t delta);
    SF_API void* mmap(void* addr, usize length, int protection, int flags, int file_desc, long offset);
    SF_API int munmap(void* addr, usize length);

    /**
     * You can define SF_USE_STD_MALLOC macro in order to switch to std::malloc implementation of allocation functions.
     */

    SF_API void* malloc(usize size, usize alignment = SF_ALIGNMENT);
    SF_API void free(void* data);
    SF_API void* realloc(void* old_data, usize size, usize alignment = SF_ALIGNMENT);
    SF_API void* reallocf(void* old_data, usize size, usize alignment = SF_ALIGNMENT);
    SF_API void* calloc(usize size, usize alignment = SF_ALIGNMENT);
    SF_API void memset(void* data, usize value, usize size, usize alignment = SF_ALIGNMENT);
    SF_API void memcpy(void* dest_data, usize dest_size, const void* src_data, usize src_size, usize alignment = SF_ALIGNMENT);
    SF_API void memmove(void* dest_data, usize dest_size, void* src_data, usize src_size, usize alignment = SF_ALIGNMENT);

    SF_API void* moveptr(void* ptr, usize size);

    template<typename T>
    constexpr T* malloc_t(usize count, usize alignment = SF_ALIGNMENT) {
        return static_cast<T*>(sf::malloc(sizeof(T) * count, alignment));
    }

    template<typename T>
    constexpr T* realloc_t(T* data, usize count, usize alignment = SF_ALIGNMENT) {
        return static_cast<T*>(sf::realloc(data, sizeof(T) * count, alignment));
    }

    template<typename T>
    constexpr T* reallocf_t(T* data, usize count, usize alignment = SF_ALIGNMENT) {
        return static_cast<T*>(sf::reallocf(data, sizeof(T) * count, alignment));
    }

    template<typename T>
    constexpr T* calloc_t(usize count, usize alignment = SF_ALIGNMENT) {
        return static_cast<T*>(sf::calloc(sizeof(T) * count, alignment));
    }

    template<typename T>
    constexpr void memset_t(T* data, usize value, usize count, usize alignment = SF_ALIGNMENT) {
        sf::memset(data, value, sizeof(T) * count, alignment);
    }

    template<typename T>
    constexpr void memcpy_t(T* dest_data, usize dest_count, const T* src_data, usize src_count, usize alignment = SF_ALIGNMENT) {
        sf::memcpy(dest_data, sizeof(T) * dest_count, src_data, sizeof(T) * src_count, alignment);
    }

    template<typename T>
    constexpr void memmove_t(T* dest_data, usize dest_count, T* src_data, usize src_count, usize alignment = SF_ALIGNMENT) {
        sf::memmove(dest_data, sizeof(T) * dest_count, src_data, sizeof(T) * src_count, alignment);
    }

    template<typename T>
    constexpr T* moveptr_t(T* ptr, usize count) {
        return static_cast<T*>(moveptr(ptr, sizeof(T) * count));
    }

    struct SF_API memory_arena_t final {
        void* memory = nullptr;
        usize prev_offset = 0;
        usize current_offset = 0;
        usize size = 0;
    };

    SF_API memory_arena_t memory_arena_init(void* memory, usize size);
    SF_API void memory_arena_free(memory_arena_t& memory_arena);
    SF_API void* memory_arena_allocate(memory_arena_t& memory_arena, usize size, usize alignment = SF_ALIGNMENT);
    SF_API void* memory_arena_resize(memory_arena_t& memory_arena, usize size, void* old_memory, usize old_size, usize alignment = SF_ALIGNMENT);

    struct SF_API memory_arena_temp_t final {
        memory_arena_t* memory_arena = nullptr;
        usize prev_offset = 0;
        usize current_offset = 0;
    };

    SF_API memory_arena_temp_t memory_arena_temp_init(memory_arena_t* memory_arena);
    SF_API void memory_arena_temp_free(memory_arena_temp_t memory_arena_temp);

    struct SF_API memory_pool_alloc_t final {
        bool free = true;
        usize size = 0;
        usize used_size = 0;
        void* memory = nullptr;
    };

    struct SF_API memory_pool_t final {
        void* memory = nullptr;
        usize size = 0;
        void* last_ptr = nullptr;
        void* max_ptr = nullptr;
        memory_pool_alloc_t* allocs = nullptr;
        usize alloc_size = 0;
        usize alloc_capacity = 0;
    };

    SF_API memory_pool_t memory_pool_init(void* memory, usize size, usize alloc_capacity);
    SF_API void memory_pool_free(memory_pool_t& memory_pool);
    SF_API void* memory_pool_allocate(memory_pool_t& memory_pool, usize size, usize alignment = SF_ALIGNMENT);
    SF_API void memory_pool_free(const memory_pool_t& memory_pool, const void* addr);

    struct system_info_t final {
        usize ram_total_bytes = 0;
        usize ram_free_bytes = 0;
        u8 cpu_core_count = 0;
    };

    SF_API system_info_t system_info_get();

    inline memory_pool_t g_stl_memory_pool = {};

    template<typename T>
    struct stl_allocator_t {
        typedef T value_type;

        stl_allocator_t() = default;

        template<class U>
        constexpr stl_allocator_t(const stl_allocator_t<U>&) noexcept {}

        [[nodiscard]] T* allocate(std::size_t count) {
            SF_ASSERT(g_stl_memory_pool.memory != nullptr, "stl_allocator::allocate(): stl_memory_pool is not initialized!");
            return static_cast<T*>(memory_pool_allocate(g_stl_memory_pool, sizeof(T) * count));
        }

        void deallocate(T* element, std::size_t count) noexcept {
            SF_ASSERT(g_stl_memory_pool.memory != nullptr, "stl_allocator::deallocate(): stl_memory_pool is not initialized!");
            memory_pool_free(g_stl_memory_pool, element);
        }
    };

    template<class T, class U>
    bool operator==(const stl_allocator_t<T>&, const stl_allocator_t<U>&) { return true; }

    template<class T, class U>
    bool operator!=(const stl_allocator_t<T>&, const stl_allocator_t<U>&) { return false; }

    typedef std::basic_string<char, std::char_traits<char>, stl_allocator_t<char>> string_t;

    SF_API u64 string_hash(const string_t& string);

    typedef std::basic_stringstream<char, std::char_traits<char>, stl_allocator_t<char>> sstream_t;

    template<typename T>
    using vector_t = std::vector<T, stl_allocator_t<T>>;

    template<typename K, typename V>
    using map_t = std::map<K, V, std::equal_to<K>, stl_allocator_t<std::pair<K, V>>>;

    template<typename K, typename Comparator = std::less<K>>
    using set_t = std::set<K, Comparator, stl_allocator_t<K>>;

    template<typename K>
    using hash_set_t = std::unordered_set<K, std::hash<K>, std::equal_to<K>, stl_allocator_t<K>>;

    template<typename K, typename V>
    using hash_map_t = std::unordered_map<K, V, std::hash<K>, std::equal_to<K>, stl_allocator_t<std::pair<K, V>>>;

    struct SF_API date_time_t final {
        u32 y;   // year
        u32 m;   // month
        u32 d;   // day
        i64 h;   // hour
        i64 min; // minute
        i64 s;   // second
        i64 ms;  // millisecond
    };

    SF_API date_time_t date_time_get_current();

    struct SF_API time_t final {
        i64 h;   // hour
        i64 min; // minute
        i64 s;   // second
        i64 ms;  // millisecond
    };

    SF_API time_t time_get_current();
    SF_API float time_get_current_ms();

    template<typename T, typename A>
    struct circular_buffer_t final {
        T* elements = nullptr;
        usize size = 0;
        usize tail = 0;
        usize head = 0;
    };

    template<typename T, typename A>
    circular_buffer_t<T, A> circular_buffer_init(const usize size) {
        circular_buffer_t<T, A> circular_buffer;
        circular_buffer.elements = static_cast<T*>(A().allocate(sizeof(T) * size));
        circular_buffer.size = size;
        return circular_buffer;
    }

    template<typename T, typename A>
    void circular_buffer_free(const circular_buffer_t<T, A>& circular_buffer) {
        A().deallocate(circular_buffer.elements);
    }

    template<typename T, typename A>
    bool circular_buffer_push(circular_buffer_t<T, A>& circular_buffer, const T &item) {
        bool pushed = false;

        usize next = (circular_buffer.head + 1) % circular_buffer.size;
        if (next != circular_buffer.tail) {
            circular_buffer.elements[circular_buffer.head] = item;
            circular_buffer.head = next;
            pushed = true;
        }

        return pushed;
    }

    template<typename T, typename A>
    bool circular_buffer_pop(circular_buffer_t<T, A>& circular_buffer, T &item) {
        bool popped = false;

        if (circular_buffer.tail != circular_buffer.head) {
            item = circular_buffer.elements[circular_buffer.tail];
            circular_buffer.tail = (circular_buffer.tail + 1) % circular_buffer.size;
            popped = true;
        }

        return popped;
    }

    struct SF_API mutex_t final {
        pthread_mutex_t handle;
    };

    SF_API mutex_t mutex_init();
    SF_API void mutex_free(mutex_t& mutex);
    SF_API void mutex_lock(mutex_t& mutex);
    SF_API void mutex_unlock(mutex_t& mutex);

    struct SF_API condition_var_t final {
        pthread_cond_t handle;
    };

    SF_API condition_var_t condition_var_init();
    SF_API void condition_var_free(condition_var_t& condition_var);
    SF_API void condition_var_wait(condition_var_t& condition_var, mutex_t& mutex);
    SF_API void condition_var_notify(condition_var_t& condition_var);

    typedef void (*thread_run_function_t) (void* args);

    struct SF_API thread_t final {
        pthread_t handle = 0;
        void* run_args = nullptr;
        thread_run_function_t run_function = nullptr;
        pid_t pid = 0;
        pid_t tid = 0;
        const char* name = nullptr;
        SF_THREAD_PRIORITY priority = SF_THREAD_PRIORITY_NORMAL;
    };

    SF_API u32 thread_get_pid();
    SF_API u32 thread_get_tid();
    SF_API void thread_sleep(u32 ms);
    SF_API void thread_yield();
    SF_API void thread_exit();
    SF_API thread_t thread_init(const char* name, SF_THREAD_PRIORITY priority);
    SF_API void thread_free(const thread_t& thread);
    SF_API void thread_run(thread_t& thread);
    SF_API void thread_detach(const thread_t& thread);
    SF_API void thread_join(const thread_t& thread);
    SF_API void thread_set_info(thread_t& thread);

    typedef std::function<void()> task_t;

    template<typename A>
    struct SF_API thread_pool_t final {
        bool running = false;
        thread_t* threads = nullptr;
        usize thread_size = 0;
        circular_buffer_t<task_t, A> tasks;
        mutex_t mutex_wake = {};
        condition_var_t condition_var_wake = {};
    };

    template<typename A>
    thread_pool_t<A> thread_pool_init(usize thread_size, usize task_size, const char* name, SF_THREAD_PRIORITY priority) {
        thread_pool_t<A> thread_pool;
        thread_pool.threads = static_cast<thread_t*>(A().allocate(sizeof(thread_t) * thread_size));
        thread_pool.thread_size = thread_size;
        thread_pool.tasks = circular_buffer_init<task_t, A>(task_size);
        thread_pool.mutex_wake = mutex_init();
        thread_pool.condition_var_wake = condition_var_init();
        for (int i = 0; i < thread_size ; i++) {
            thread_t& thread = thread_pool.threads[i];
            thread = thread_init(name, priority);
        }
        return thread_pool;
    }

    template<typename A>
    void thread_pool_run(thread_pool_t<A>& thread_pool) {
        thread_pool.running = true;
        const usize thread_size = thread_pool.thread_size;
        for (int i = 0 ; i < thread_size ; i++) {
            thread_t& thread = thread_pool.threads[i];
            thread.run_args = &thread_pool;
            thread.run_function = [] (void* args) {
                auto& thread_pool = *static_cast<thread_pool_t<A>*>(args);
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

    template<typename A>
    void thread_pool_free(thread_pool_t<A>& thread_pool) {
        thread_pool.running = false;
        const usize thread_size = thread_pool.thread_size;
        for (int i = 0 ; i < thread_size ; i++) {
            thread_free(thread_pool.threads[i]);
        }
        A().deallocate(thread_pool.threads);
        circular_buffer_free(thread_pool.tasks);
    }

    template<typename A>
    void thread_pool_add_task(thread_pool_t<A>& thread_pool, const task_t& task) {
        // try to push a new task until it is pushed
        while (!circular_buffer_push<task_t>(thread_pool.tasks, task)) {
            condition_var_notify(thread_pool.condition_var_wake);
            thread_yield();
        }
        condition_var_notify(thread_pool.condition_var_wake);
    }

}