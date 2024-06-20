#include <Memory.hpp>
#include <unistd.h>
#include <sys/mman.h>

int Brk(void* addr) {
    return brk(addr);
}

void* Sbrk(intptr_t delta) {
    return sbrk(delta);
}

void* Mmap(void *addr, usize length, int protection, int flags, int file_desc, long offset) {
    return mmap(addr, length, protection, flags, file_desc, offset);
}

int Munmap(void *addr, usize length) {
    return munmap(addr, length);
}