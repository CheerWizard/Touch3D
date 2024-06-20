#include <Memory.hpp>

static MemoryBlock* block_root = nullptr;

static MemoryBlock* HeapResize(MemoryBlock* last, usize size) {
    MemoryBlock* block;

    block = static_cast<MemoryBlock*>(Sbrk(0));

    isize sbrk_result = reinterpret_cast<isize>(Sbrk(T3D_MEMORY_BLOCK_SIZE + size));

    if (sbrk_result < 0) {
        return nullptr;
    }

    block->size = size;
    block->prev = last;
    block->next = nullptr;
    block->ptr = block->data;
    block->free = false;

    if (last) {
        last->next = block;
    }

    return block;
}

static MemoryBlock* BlockFind(MemoryBlock** last, usize size) {
    MemoryBlock* block = block_root;
    while (block && (block->free && block->size >= size)) {
        *last = block;
        block = block->next;
    }
    return block;
}

static void BlockSplit(MemoryBlock* block, usize size) {
    MemoryBlock* new_block;
    new_block = reinterpret_cast<MemoryBlock*>(block->data + size);
    new_block->size = block->size - size - T3D_MEMORY_BLOCK_SIZE;
    new_block->next = block->next;
    new_block->free = true;
    new_block->ptr = new_block->data;
    block->size = size;
    block->next = new_block;
    if (new_block->next) {
        new_block->next->prev = new_block;
    }
}

static MemoryBlock* BlockConcat(MemoryBlock* block) {
    if (block->next && block->next->free) {
        block->size = T3D_MEMORY_BLOCK_SIZE + block->next->size;
        block->next = block->next->next;
        if (block->next) {
            block->next->prev = block;
        }
    }
    return block;
}

static MemoryBlock* BlockGet(void* ptr) {
    char* tmp;
    tmp = static_cast<char*>(ptr);
    return static_cast<MemoryBlock*>(ptr = tmp -= T3D_MEMORY_BLOCK_SIZE);
}

static bool BlockIsValid(void* ptr) {
    if (block_root) {
        if (ptr > block_root && ptr < Sbrk(0)) {
            return ptr == BlockGet(ptr)->ptr;
        }
    }
    return false;
}

static void BlockCopy(MemoryBlock* dest_block, MemoryBlock* src_block) {
    Memcpy(dest_block->ptr, dest_block->size, src_block->ptr, src_block->size);
}

void* Malloc(usize size) {
    MemoryBlock* block;
    MemoryBlock* last;
    size = T3D_ALIGN(size); // we should always malloc aligned size (aligned by 32-bit or 64-bit)

    if (block_root != nullptr) {
        last = block_root;
        block = BlockFind(&last, size);
        if (block != nullptr) {
            if (block->size - size >= T3D_MEMORY_BLOCK_SIZE + sizeof(void*)) {
                BlockSplit(block, size);
            }
            block->free = false;
        }
        else {
            block = HeapResize(last, size);
            if (block == nullptr) {
                return nullptr;
            }
        }
    }
    else {
        block = HeapResize(nullptr, size);
        if (block == nullptr) {
            return nullptr;
        }
        block_root = block;
    }

    return block->data;
}

void Free(void* data) {
    MemoryBlock* block;
    if (BlockIsValid(data)) {
        block = BlockGet(data);
        block->free = true;
        if (block->prev && block->prev->free) {
            block = BlockConcat(block->prev);
        }
        if (block->next) {
            BlockConcat(block->next);
        }
        else {
            if (block->prev) {
                block->prev->next = nullptr;
            }
            else {
                block_root = nullptr;
            }
            Brk(block);
        }
    }
}

void* Realloc(void* old_data, usize size) {
    MemoryBlock* block;
    MemoryBlock* new_block;
    void* new_data;

    if (old_data == nullptr) {
        return Malloc(size);
    }

    if (BlockIsValid(old_data)) {
        size = T3D_ALIGN(size);
        block = BlockGet(old_data);

        if (block->size >= size) {
            if (block->size - size >= T3D_MEMORY_BLOCK_SIZE + sizeof(void*)) {
                BlockSplit(block, size);
            }
        }

        else {
            if (block->next && block->next->free && (block->size + T3D_MEMORY_BLOCK_SIZE + block->next->size) >= size) {
                BlockConcat(block);
                if (block->size - size >= T3D_MEMORY_BLOCK_SIZE + sizeof(void*)) {
                    BlockSplit(block, size);
                }
            }

            else {
                new_data = Malloc(size);
                if (new_data == nullptr) {
                    return nullptr;
                }
                new_block = BlockGet(new_data);
                BlockCopy(new_block, block);
                Free(old_data);
                return new_data;
            }
        }

        return old_data;
    }

    return nullptr;
}

void* Reallocf(void* old_data, usize size) {
    void* realloc_block = Realloc(old_data, size);
    if (realloc_block == nullptr) {
        Free(old_data);
    }
    return realloc_block;
}

void* Calloc(usize size) {
    void* block = Malloc(size);
    if (block) {
        Memset(block, 0, size);
    }
    return block;
}

void Memset(void* data, usize value, usize size) {
    usize* d = static_cast<usize*>(data);
    if (data != nullptr) {
        usize aligned_size = T3D_MEMSET_ALIGN(size);
        for (usize i = 0 ; i < aligned_size ; i++) {
            d[i] = value;
        }
    }
}

void Memcpy(void* dest_data, usize dest_size, const void* src_data, usize src_size) {
    usize* d = static_cast<usize*>(dest_data);
    const usize* s = static_cast<const usize*>(src_data);
    for (usize i = 0 ; (i * sizeof(void*) < src_size) && (i * sizeof(void*) < dest_size) ; i++) {
        d[i] = s[i];
    }
}

void* Moveptr(void* ptr, usize size) {
    return reinterpret_cast<void*>(reinterpret_cast<usize>(ptr) + size);
}

void BumpAllocator::Init(usize size) {
    m_memory = Sbrk(0);
    m_size = size;
    isize sbrk_result = reinterpret_cast<isize>(Sbrk(size));
    if (sbrk_result < 0) {
        T3D_DEBUG_BREAK();
        return;
    }
}

void BumpAllocator::Free() {
    Brk(m_memory);
    m_size = 0;
    m_used_size = 0;
}

void* BumpAllocator::Alloc(usize size) {
    void* addr = nullptr;
    usize aligned_size = (size + 7) & ~ 7; // this will make sure that first 4 bits are 0
    if (m_used_size + aligned_size <= m_size) {
        addr = (char*)m_memory + m_used_size;
        m_used_size += aligned_size;
    }
    else {
        T3D_DEBUG_BREAK();
    }
    return addr;
}

void MemoryPoolAllocator::Init(usize size, usize alloc_capacity) {
    m_memory = Malloc(size);
    m_size = size;
    m_last_ptr = m_memory;
    m_max_ptr = Moveptr(m_memory, size);
    m_allocs = MallocT<MemoryPoolAllocation>(alloc_capacity);
    m_alloc_capacity = alloc_capacity;
}

void MemoryPoolAllocator::Free() {
    Free(m_memory);
    Free(m_allocs);
    m_memory = nullptr;
    m_allocs = nullptr;
    m_size = 0;
    m_alloc_capacity = 0;
    m_alloc_size = 0;
    m_last_ptr = nullptr;
    m_max_ptr = nullptr;
}

void* MemoryPoolAllocator::Alloc(usize size) {
    for (usize i = 0; i < m_alloc_size ; i++) {
        MemoryPoolAllocation& allocation = m_allocs[i];

        // check if allocation is free to use and has at least enough space for usage
        if (allocation.free && allocation.size >= size) {
            allocation.free = false;
            allocation.used_size = size;
            return allocation.data;
        }

        // check if used allocation has additional free space for usage
        // if yes - than create additional allocation from used allocation
        if ((allocation.size - allocation.used_size) >= size) {
            MemoryPoolAllocation new_allocation;
            new_allocation.free = false;
            new_allocation.size = allocation.size - allocation.used_size;
            new_allocation.used_size = size;
            new_allocation.data = Moveptr(allocation.data, allocation.used_size);
            allocation.size = allocation.used_size;
            EmplaceAllocation(new_allocation);
            return new_allocation.data;
        }
    }

    // check if we have enough space in memory pool
    if (m_last_ptr >= m_max_ptr) {
        return nullptr;
    }

    MemoryPoolAllocation new_allocation;
    new_allocation.free = false;
    new_allocation.size = size;
    new_allocation.used_size = size;
    new_allocation.data = m_last_ptr;
    EmplaceAllocation(new_allocation);
    m_last_ptr = Moveptr(m_last_ptr, size);
    return new_allocation.data;
}

void MemoryPoolAllocator::Free(void* data) {
    for (usize i = 0 ; i < m_alloc_size ; i += sizeof(MemoryPoolAllocation)) {
        MemoryPoolAllocation& allocation = m_allocs[i];
        if (allocation.data == data) {
            allocation.free = true;
            allocation.used_size = 0;
        }
    }
}

void MemoryPoolAllocator::EmplaceAllocation(const MemoryPoolAllocation &alloc) {
    if (m_alloc_size >= m_alloc_capacity) {
        m_alloc_capacity = m_alloc_size + 100;
        m_allocs = ReallocT<MemoryPoolAllocation>(m_allocs, m_alloc_capacity);
    }
    MemcpyT(MoveptrT(m_allocs, m_alloc_size), m_alloc_capacity, &alloc, 1);
    m_alloc_size++;
}

Memory::Memory() {
    SystemInfo system_info = GetSystemInfo();
    // memory size = 20% of RAM but less than 1GB
    usize memory_size = static_cast<usize>(static_cast<double>(system_info.free_ram) * 0.2);
    if (memory_size >= T3D_GB(1)) {
        memory_size = T3D_GB(1);
    }
    // alloc size value can be estimated and tested
    usize alloc_size = 100;
    allocator.Init(memory_size, alloc_size);
}

Memory::~Memory() {
    allocator.Free();
}

Memory memory = {};