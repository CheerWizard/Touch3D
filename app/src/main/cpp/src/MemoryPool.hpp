#pragma once

class MemoryPool final {

public:
    MemoryPool();
    ~MemoryPool();

public:
    static MemoryPool& Get();

private:
    static MemoryPool* s_instance;

};