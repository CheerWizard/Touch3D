#include <Memory.hpp>
#include <unistd.h>
#include <memoryapi.h>

int Brk(void* addr) {
    // Windows doesn't implement this syscall
    return 0;
}

void* Sbrk(intptr_t delta) {
    // Windows doesn't implement this syscall
    return nullptr;
}

void* Mmap(void *addr, usize length, int protection, int flags, int file_desc, long offset) {
    return VirtualAlloc(addr, length, file_desc, protection);
}

int Munmap(void *addr, usize length) {
    return VirtualFree(addr, length, 0);
}