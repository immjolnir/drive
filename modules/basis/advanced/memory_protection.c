#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>

/*
mmap, munmap - map or unmap files or devices into memory
       #include <sys/mman.h>

       void *mmap(void *addr, size_t length, int prot, int flags,
                  int fd, off_t offset);
       int munmap(void *addr, size_t length);
*/
int main(int argc, char* argv[]) {
    void* ptr = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        return 1;
    }

    // 修改内存区域的保护属性为只读
    if (mprotect(ptr, 4096, PROT_READ) == -1) {
        perror("mprotect");
        return 1;
    }

    // 尝试写入已变为只读的内存区域
    *((char*)ptr) = 'A';

    munmap(ptr, 4096);
    return 0;
}

// 它要对 只读区域 进行写操作，引起 coredump.
// $ ./build/modules/basis/advanced/memory_protection
// Segmentation fault (core dumped)
