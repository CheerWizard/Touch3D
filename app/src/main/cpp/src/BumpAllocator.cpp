#include <BumpAllocator.hpp>
#include <Log.hpp>

#include <malloc.h>

BumpAllocator::BumpAllocator(usize size) : m_size(size), m_used_size(0) {
    m_memory = malloc(size);
}

BumpAllocator::~BumpAllocator() {
    free(m_memory);
}

void* BumpAllocator::Alloc(usize size) {
    void* addr = nullptr;
    usize aligned_size = (size + 7) & ~ 7; // this will make sure that first 4 bits are 0
    if (m_used_size + aligned_size <= m_size) {
        addr = (char*)m_memory + m_used_size;
        m_used_size += aligned_size;
    }
    else {
        LogAssert(false, "Failed to allocate with BumpAllocator!");
    }
    return addr;
}