#include <MemoryPool.hpp>

MemoryPool* MemoryPool::s_instance = nullptr;

MemoryPool::MemoryPool() {
    s_instance = this;
}

MemoryPool::~MemoryPool() {
    s_instance = nullptr;
}

MemoryPool& MemoryPool::Get() {
    return *s_instance;
}
