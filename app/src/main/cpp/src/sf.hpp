#pragma once

#include <cstdint>
#include <cstddef>
#include <chrono>
#include <functional>
#include <pthread.h>
#include <csignal>
#include <cassert>
#include <sstream>
#include <cstring>

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

#define SF_KB(c) ((c) * 1024)
#define SF_MB(c) SF_KB(1024)
#define SF_GB(c) SF_MB(1024)
#define SF_TB(c) SF_GB(1024)

// x - target value to align
// a - alignment in bytes
#define SF_ALIGN(x,a) (((((x)-1)>>((a)/2))<<((a)/2))+(a))
#define SF_ALIGNMENT sizeof(void*)

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

    SF_API void* malloc(usize size, u8 alignment = SF_ALIGNMENT);
    SF_API void free(void* data);
    SF_API void* realloc(void* old_data, usize size, u8 alignment = SF_ALIGNMENT);
    SF_API void* reallocf(void* old_data, usize size, u8 alignment = SF_ALIGNMENT);
    SF_API void* calloc(usize size, u8 alignment = SF_ALIGNMENT);
    SF_API void memset(void* data, usize value, usize size, u8 alignment = SF_ALIGNMENT);
    SF_API void memcpy(void* dest_data, usize dest_size, const void* src_data, usize src_size, u8 alignment = SF_ALIGNMENT);
    SF_API void* moveptr(void* ptr, usize size);

    template<typename T>
    inline T* malloc_t(usize count) {
        return static_cast<T*>(malloc(sizeof(T) * count));
    }

    template<typename T>
    inline T* realloc_t(T* data, usize count) {
        return static_cast<T*>(realloc(data, sizeof(T) * count));
    }

    template<typename T>
    inline T* reallocf_t(T* data, usize count) {
        return static_cast<T*>(reallocf(data, sizeof(T) * count));
    }

    template<typename T>
    inline T* calloc_t(usize count) {
        return static_cast<T*>(calloc(sizeof(T) * count));
    }

    template<typename T>
    inline void memset_t(T* data, usize value, usize count) {
        memset(data, value, sizeof(T) * count);
    }

    template<typename T>
    inline void memcpy_t(T* dest_data, usize dest_count, const T* src_data, usize src_count) {
        memcpy(dest_data, sizeof(T) * dest_count, src_data, sizeof(T) * src_count);
    }

    template<typename T>
    inline T* moveptr_t(T* ptr, usize count) {
        return static_cast<T*>(moveptr(ptr, sizeof(T) * count));
    }

    class SF_API MemoryBumpAllocator final {

        friend class Memory;

    public:
        void* allocate(usize size);

    private:
        void init(usize size);
        void free();

    private:
        void* m_memory = nullptr;
        usize m_size = 0;
        usize m_used_size = 0;
    };

    struct SF_API MemoryPoolAllocation final {
        bool free = true;
        usize size = 0;
        usize used_size = 0;
        void* data = nullptr;
    };

    class SF_API MemoryPoolAllocator final {

        friend class Memory;

    public:
        void* allocate(usize size);
        void free(void* data);

    private:
        void init(usize size, usize alloc_capacity);
        void free();
        void add_allocation(const MemoryPoolAllocation& alloc);

    private:
        void* m_memory = nullptr;
        usize m_size = 0;
        void* m_last_ptr = nullptr;
        void* m_max_ptr = nullptr;
        MemoryPoolAllocation* m_allocs = nullptr;
        usize m_alloc_size = 0;
        usize m_alloc_capacity = 0;
    };

    class SF_API Memory final {

    public:
        usize total_ram;   // Total usable main memory size
        usize free_ram;	   // Available memory size
        usize shared_ram;  // Amount of shared memory
        usize buffer_ram;  // Memory used by buffers
        usize total_high;  // Total high memory size
        usize free_high;   // Available high memory size

        MemoryBumpAllocator bump_allocator;
        MemoryPoolAllocator pool_allocator;

    public:
        Memory();
        ~Memory();

    private:
        void get_memory_info();
    };
// TODO(cheerwizard): memory should be initialized statically first, before anything else.
//  it will allow to use memory global variable during static initialization of any MemoryPoolObject.
    extern SF_API Memory memory;

    class SF_API MemoryPoolObject {
    public:
        void* operator new(usize size) {
            return memory.pool_allocator.allocate(size);
        }
        void operator delete(void* addr) {
            memory.pool_allocator.free(addr);
        }
    };

    class SF_API MemoryBumpObject {
    public:
        void* operator new(usize size) {
            return memory.bump_allocator.allocate(size);
        }
        void operator delete(void* addr) {
            // do nothing, bump allocator has no free functionality
        }
    };

    class SF_API String final {

    public:
        String(const char* string);

    private:
        void resize(usize size);

    private:
        char* m_chars = nullptr;
        usize m_size = 0;
        usize m_capacity = 0;
    };

    using namespace std::chrono;
    using days = std::chrono::duration<int, std::ratio<86400>>;

    struct SF_API DateTime final {
        u32 y;   // year
        u32 m;   // month
        u32 d;   // day
        i64 h;   // hour
        i64 min; // minute
        i64 s;   // second
        i64 ms;  // millisecond
    };

    struct SF_API Time final {
        i64 h;   // hour
        i64 min; // minute
        i64 s;   // second
        i64 ms;  // millisecond
    };

    SF_API DateTime get_current_date_time();
    SF_API Time get_current_time();
    SF_API float get_current_time_millis();

    template<typename T, usize size>
    class CircularBuffer final {

    public:
        bool push(const T& item);
        bool pop(T& item);

    private:
        T m_elements[size] = {};
        usize m_tail = 0;
        usize m_head = 0;
    };

    template<typename T, usize size>
    bool CircularBuffer<T, size>::push(const T &item)
    {
        bool pushed = false;

        usize next = (m_head + 1) % size;
        if (next != m_tail)
        {
            m_elements[m_head] = item;
            m_head = next;
            pushed = true;
        }

        return pushed;
    }

    template<typename T, usize size>
    bool CircularBuffer<T, size>::pop(T &item)
    {
        bool popped = false;

        if (m_tail != m_head)
        {
            item = m_elements[m_tail];
            m_tail = (m_tail + 1) % size;
            popped = true;
        }

        return popped;
    }

    enum SF_THREAD_PRIORITY {
        SF_THREAD_PRIORITY_LOWEST,
        SF_THREAD_PRIORITY_NORMAL,
        SF_THREAD_PRIORITY_HIGHEST,

        SF_THREAD_PRIORITY_COUNT
    };

    class SF_API Mutex final {

        friend class ConditionVar;

    public:
        Mutex();
        ~Mutex();

        void lock();
        void unlock();

    private:
        pthread_mutex_t m_handle;
    };

    class SF_API ConditionVar final {

    public:
        ConditionVar();
        ~ConditionVar();

        void wait(Mutex& mutex);
        void notify();

    private:
        pthread_cond_t m_handle;
    };

    class SF_API Runnable final {

    public:
        Runnable(const std::function<void()>& runnable = {}) : m_runnable(runnable) {}

    public:
        static void* run(void* thiz) {
            return static_cast<Runnable*>(thiz)->run();
        }

    public:
        inline void set_runnable(const std::function<void()>& runnable) {
            m_runnable = runnable;
        }

        [[nodiscard]] inline const pid_t& get_pid() const {
            return m_pid;
        }
        [[nodiscard]] inline const pid_t& get_tid() const {
            return m_tid;
        }

    private:
        void* run();

    private:
        std::function<void()> m_runnable = {};
        pid_t m_pid;
        pid_t m_tid;

    };

    class SF_API Thread final {

    public:
        Thread() = default;
        Thread(const char* name, SF_THREAD_PRIORITY priority) : m_name(name), m_priority(priority) {}

    public:
        static u32 get_pid();
        static u32 get_tid();
        static void sleep(u32 millis);
        static void yield();
        static void exit();

    public:
        void run(const std::function<void()>& runnable, const std::function<void()>& kill_callback = {});
        void detach() const;
        void join() const;
        void kill();

    private:
        void set_thread_info();
        static void on_kill(int signal);

    private:
        Runnable m_runnable;
        pthread_t m_handle;
        pid_t m_pid;
        pid_t m_tid;
        const char* m_name;
        SF_THREAD_PRIORITY m_priority;
        std::function<void()> m_kill_callback;

    };

    template<usize thread_buffer_size, usize task_buffer_size, SF_THREAD_PRIORITY thread_priority>
    class ThreadPool final {

    public:
        ThreadPool(const char* thread_name);
        ~ThreadPool();

    public:
        void add(const std::function<void()>& task);

    protected:
        // wakes only one thread
        // allows caller-thread to be rescheduled by OS
        inline void poll();

    private:
        CircularBuffer<std::function<void()>, task_buffer_size> m_task_buffer;
        Thread m_thread_buffer[thread_buffer_size] = {};
        Mutex m_wake_mutex;
        ConditionVar m_wake_condition_var;
        bool m_running = false;
    };

    template<usize thread_buffer_size, usize task_buffer_size, SF_THREAD_PRIORITY thread_priority>
    ThreadPool<thread_buffer_size, task_buffer_size, thread_priority>::ThreadPool(const char* thread_name)
    {
        m_running = true;
        for (int i = 0; i < thread_buffer_size ; i++)
        {
            Thread& thread = m_thread_buffer[i];
            thread = Thread(thread_name, thread_priority);
            thread.run([this]() {
                std::function<void()> task;
                while (m_running) {
                    if (m_task_buffer.pop(task)) {
                        task();
                    }
//                    else {
//                        // because we don't want to overhead cpu core with thread while loop
//                        // it's better to simply put thread into wait, until it is notified by outer thread with wake condition variable
//                        m_wake_condition_var.wait(m_wake_mutex);
//                    }
                }
            });
            thread.detach();
        }
    }

    template<usize thread_buffer_size, usize task_buffer_size, SF_THREAD_PRIORITY thread_priority>
    ThreadPool<thread_buffer_size, task_buffer_size, thread_priority>::~ThreadPool() {
        m_running = false;
        for (int i = 0 ; i < thread_buffer_size ; i++) {
            m_thread_buffer[i].kill();
        }
    }

    template<usize thread_buffer_size, usize task_buffer_size, SF_THREAD_PRIORITY thread_priority>
    void ThreadPool<thread_buffer_size, task_buffer_size, thread_priority>::add(const std::function<void()> &task) {
        // try to push a new task until it is pushed
        while (!m_task_buffer.push(task)) {
            poll();
        }
        m_wake_condition_var.notify();
    }

    template<usize thread_buffer_size, usize task_buffer_size, SF_THREAD_PRIORITY thread_priority>
    void ThreadPool<thread_buffer_size, task_buffer_size, thread_priority>::poll() {
        m_wake_condition_var.notify();
        Thread::yield();
    }

}