#pragma once

#include <Types.hpp>
#include <DebugBreak.hpp>

#define T3D_KB(c) ((c) * 1024)
#define T3D_MB(c) T3D_KB(1024)
#define T3D_GB(c) T3D_MB(1024)
#define T3D_TB(c) T3D_GB(1024)

// defined by OS specific syscall implementation
int Brk(void* addr);
void* SBrk(intptr_t delta);
void* MMap(void* addr, usize length, int protection, int flags, int file_desc, long offset);
int MUnmap(void* addr, usize length);
// defined by OS specific syscall implementation

// x - target value to align
// a - alignment in bytes
#define T3D_ALIGN(x,a) (((((x)-1)>>((a)/2))<<((a)/2))+(a))
#define T3D_ALIGNMENT sizeof(void*)

void* Malloc(usize size, u8 alignment = T3D_ALIGNMENT);
void Free(void* data);
void* Realloc(void* old_data, usize size, u8 alignment = T3D_ALIGNMENT);
void* Reallocf(void* old_data, usize size, u8 alignment = T3D_ALIGNMENT);
void* Calloc(usize size, u8 alignment = T3D_ALIGNMENT);
void Memset(void* data, usize value, usize size, u8 alignment = T3D_ALIGNMENT);
void Memcpy(void* dest_data, usize dest_size, const void* src_data, usize src_size, u8 alignment = T3D_ALIGNMENT);
void* Moveptr(void* ptr, usize size);

template<typename T>
inline T* MallocT(usize count) {
    return static_cast<T*>(Malloc(sizeof(T) * count));
}

template<typename T>
inline T* ReallocT(T* data, usize count) {
    return static_cast<T*>(Realloc(data, sizeof(T) * count));
}

template<typename T>
inline T* ReallocfT(T* data, usize count) {
    return static_cast<T*>(Reallocf(data, sizeof(T) * count));
}

template<typename T>
inline T* CallocT(usize count) {
    return static_cast<T*>(Calloc(sizeof(T) * count));
}

template<typename T>
inline void MemsetT(T* data, usize value, usize count) {
    Memset(data, value, sizeof(T) * count);
}

template<typename T>
inline void MemcpyT(T* dest_data, usize dest_count, const T* src_data, usize src_count) {
    Memcpy(dest_data, sizeof(T) * dest_count, src_data, sizeof(T) * src_count);
}

template<typename T>
inline T* MoveptrT(T* ptr, usize count) {
    return static_cast<T*>(Moveptr(ptr, sizeof(T) * count));
}

class MemoryBumpAllocator final {

public:
    void Init(usize size);
    void Free();
    void* Alloc(usize size);

private:
    void* m_memory = nullptr;
    usize m_size = 0;
    usize m_used_size = 0;
};

struct MemoryPoolAllocation final {
    bool free = true;
    usize size = 0;
    usize used_size = 0;
    void* data = nullptr;
};

class MemoryPoolAllocator final {

public:
    void Init(usize size, usize alloc_capacity);
    void Free();
    void* Alloc(usize size);
    void Free(void* data);

private:
    void EmplaceAllocation(const MemoryPoolAllocation& alloc);

private:
    void* m_memory = nullptr;
    usize m_size = 0;
    void* m_last_ptr = nullptr;
    void* m_max_ptr = nullptr;
    MemoryPoolAllocation* m_allocs = nullptr;
    usize m_alloc_size = 0;
    usize m_alloc_capacity = 0;
};

struct SystemInfo final {
    usize total_ram;   // Total usable main memory size
    usize free_ram;	   // Available memory size
    usize shared_ram;  // Amount of shared memory
    usize buffer_ram;  // Memory used by buffers
    u16 process_count; // Number of current processes
    usize total_high;  // Total high memory size
    usize free_high;   // Available high memory size
};

SystemInfo GetSystemInfo();

class Memory final {

public:
    MemoryPoolAllocator pool_allocator;

    Memory();
    ~Memory();

};
// TODO(cheerwizard): memory should be initialized statically first, before anything else.
//  it will allow to use memory global variable during static initialization of any MemoryPoolObject.
extern Memory memory;

class MemoryPoolObject {

public:
    void* operator new(usize size) {
        return memory.pool_allocator.Alloc(size);
    }
    void operator delete(void* addr) {
        memory.pool_allocator.Free(addr);
    }
};