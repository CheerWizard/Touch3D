#pragma once

#include <Memory.hpp>

class String : MemoryPoolObject {
    char* m_str = nullptr;
    usize m_size = 0;
    usize m_capacity = 0;
};