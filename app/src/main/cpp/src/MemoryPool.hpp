#pragma once

#include <Types.hpp>

class MemoryPool final {

public:
    MemoryPool(usize size);
    ~MemoryPool();

private:
    void* m_memory = nullptr;
    usize m_size = 0;

};