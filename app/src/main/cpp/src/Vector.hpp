#pragma once

#include <Memory.hpp>

template<typename T>
class Vector : MemoryPoolObject {
    T* m_data = nullptr;
    usize m_size = 0;
    usize m_capacity = 0;
};