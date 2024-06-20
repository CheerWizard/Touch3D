#include <Memory.hpp>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/sysinfo.h>

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

SystemInfo GetSystemInfo() {
    SystemInfo system_info {};
    struct sysinfo sys_info {};
    sysinfo(&sys_info);
    system_info.total_ram = sys_info.totalram;
    system_info.free_ram = sys_info.totalram;
    system_info.shared_ram = sys_info.totalram;
    system_info.buffer_ram = sys_info.totalram;
    system_info.process_count = sys_info.totalram;
    system_info.total_high = sys_info.totalram;
    system_info.free_high = sys_info.totalram;
    return system_info;
}