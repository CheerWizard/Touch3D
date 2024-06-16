#pragma once

#include <Types.hpp>

class BumpAllocator final {

public:
    BumpAllocator(usize size);
    ~BumpAllocator();

    void* Alloc(usize size);

private:
    void* m_memory = nullptr;
    usize m_used_size = 0;
    usize m_size = 0;
};
