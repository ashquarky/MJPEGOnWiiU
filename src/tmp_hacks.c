//libwhb doesn't seem to work right :(
#include <stdlib.h>
#include <malloc.h>

void* MEMAllocFromDefaultHeapEx(unsigned int size, unsigned int align) {
    return memalign(align, size);
}

void MEMFreeToDefaultHeap(void* mem) {
    free(mem);
}
