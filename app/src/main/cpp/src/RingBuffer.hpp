#pragma once

#include <Types.hpp>

#include <vector>
#include <mutex>

template<typename T>
class RingBuffer final {

public:
    void Reserve(usize capacity);
    void Resize(usize size);
    bool Push(const T& item);
    bool Pop(T& item);

private:
    std::vector<T> m_vector;
    usize m_tail = 0;
    usize m_head = 0;
    std::mutex m_mutex;
};

template<typename T>
void RingBuffer<T>::Reserve(usize capacity)
{
    m_vector.reserve(capacity);
}

template<typename T>
void RingBuffer<T>::Resize(usize size)
{
    m_vector.resize(size);
}

template<typename T>
bool RingBuffer<T>::Push(const T &item)
{
    bool pushed = false;
    m_mutex.lock();

    usize next = (m_head + 1) % m_vector.size();
    if (next != m_tail)
    {
        m_vector[m_head] = item;
        m_head = next;
        pushed = true;
    }

    m_mutex.unlock();
    return pushed;
}

template<typename T>
bool RingBuffer<T>::Pop(T &item)
{
    bool popped = false;
    m_mutex.lock();

    if (m_tail != m_head)
    {
        item = m_vector[m_tail];
        m_tail = (m_tail + 1) % m_vector.size();
        popped = true;
    }

    m_mutex.unlock();
    return popped;
}
