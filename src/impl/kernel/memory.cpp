#include "memory.h"

uint64_t getMemorySize(multiboot_info_t* mbd) {

    uint64_t totalMemorySize = 0;

    uint64_t mmap_addr = mbd->mmap_addr, mmap_length = mbd->mmap_length;
    uint64_t i = 0;
    while(i < mmap_length) {
        multiboot_memory_map_t* entry = (multiboot_memory_map_t*) (mmap_addr + i);
        totalMemorySize += (((uint64_t)entry->length_high) << 32) + entry->length_low;
        i += entry->size + 4;
    }
    return totalMemorySize;
}