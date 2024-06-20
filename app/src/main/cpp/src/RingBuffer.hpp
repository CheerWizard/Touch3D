#pragma once

#include <Memory.hpp>

template<typename T, usize size>
class RingBuffer : MemoryPoolObject {

public:
    bool Push(const T& item);
    bool Pop(T& item);

private:
    T m_elements[size] = {};
    usize m_tail = 0;
    usize m_head = 0;
};

template<typename T, usize size>
bool RingBuffer<T, size>::Push(const T &item)
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
bool RingBuffer<T, size>::Pop(T &item)
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