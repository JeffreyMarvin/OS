#include "page_frame_allocator.h"

extern uint64_t _kernel_start;
extern uint64_t _kernel_end;

bool bitmap::get(uint64_t index){
    if (index > size * 8) return false;
    uint64_t byteIndex = index / 8;
    uint8_t bitIndex = index % 8;
    uint8_t bitIndexer = 0b10000000 >> bitIndex;
    if ((buffer[byteIndex] & bitIndexer) > 0){
        return true;
    }
    return false;
}

bool bitmap::set(uint64_t index, bool value){
    if (index > size * 8) return false;
    uint64_t byteIndex = index / 8;
    uint8_t bitIndex = index % 8;
    uint8_t bitIndexer = 0b10000000 >> bitIndex;
    buffer[byteIndex] &= ~bitIndexer;
    if (value){
        buffer[byteIndex] |= bitIndexer;
    }
    return true;
}

Page_Frame_Allocator::Page_Frame_Allocator(multiboot_info_t* mbd){

    getMemorySize(mbd);
    freeMemory = totalMemory;

    pageBitmap.size = (totalMemory / PAGE_SIZE / 8);
    pageBitmap.buffer = (uint8_t*)(((uint64_t)&_kernel_end / PAGE_SIZE + 1) * PAGE_SIZE);
    for(size_t i = 0; i < pageBitmap.size; i++){
        *(pageBitmap.buffer + i) = 0x00;
    }

    uint64_t mmap_addr = mbd->mmap_addr, mmap_length = mbd->mmap_length;
    uint64_t i = 0;

    while(i < mmap_length) {
        multiboot_memory_map_t* entry = (multiboot_memory_map_t*) (mmap_addr + i);
        uint64_t size = (((uint64_t)entry->length_high) << 32) + entry->length_low;
        uint64_t address = (((uint64_t)entry->base_addr_high) << 32) + entry->base_addr_low;
        switch (entry->type)
        {
        case MULTIBOOT_MEMORY_AVAILABLE:
            break;
        case MULTIBOOT_MEMORY_RESERVED:
        case MULTIBOOT_MEMORY_ACPI_RECLAIMABLE:
        case MULTIBOOT_MEMORY_NVS:
        case MULTIBOOT_MEMORY_BADRAM:
        default:
            reserve_pages((void*)address, size / PAGE_SIZE + 1);
            break;
        }
        i += entry->size + 4;
    }

    lock_pages((void*)(uint64_t)&_kernel_start, ((uint64_t)&_kernel_end - (uint64_t)&_kernel_start) / PAGE_SIZE + 1);
    lock_pages((void*)pageBitmap.buffer, pageBitmap.size / PAGE_SIZE + 1);
}

void Page_Frame_Allocator::free_page(void* address){
    uint64_t index = (uint64_t)address / PAGE_SIZE;
    if (!pageBitmap.get(index)) return;
    pageBitmap.set(index, false);
    freeMemory += PAGE_SIZE;
    usedMemory -= PAGE_SIZE;
}
void Page_Frame_Allocator::free_pages(void* address, uint64_t pageCount){
    for(uint64_t i = 0; i < pageCount; i++){
        free_page((void*)((uint64_t)address + (i * PAGE_SIZE)));
    }
}

void Page_Frame_Allocator::lock_page(void* address){
    uint64_t index = (uint64_t)address / PAGE_SIZE;
    if (pageBitmap.get(index)) return;
    pageBitmap.set(index, true);
    freeMemory -= PAGE_SIZE;
    usedMemory += PAGE_SIZE;
}

void Page_Frame_Allocator::lock_pages(void* address, uint64_t pageCount){
    for(uint64_t i = 0; i < pageCount; i++){
        lock_page((void*)((uint64_t)address + (i * PAGE_SIZE)));
    }
}

void Page_Frame_Allocator::reserve_page(void* address){
    uint64_t index = (uint64_t)address / PAGE_SIZE;
    if (pageBitmap.get(index)) return;
    pageBitmap.set(index, true);
    freeMemory -= PAGE_SIZE;
    reservedMemory += PAGE_SIZE;
}

void Page_Frame_Allocator::reserve_pages(void* address, uint64_t pageCount){
    for(uint64_t i = 0; i < pageCount; i++){
        reserve_page((void*)((uint64_t)address + (i * PAGE_SIZE)));
    }
}

void Page_Frame_Allocator::unreserve_page(void* address){
    uint64_t index = (uint64_t)address / PAGE_SIZE;
    if (!pageBitmap.get(index)) return;
    pageBitmap.set(index, false);
    freeMemory += PAGE_SIZE;
    reservedMemory -= PAGE_SIZE;
}

void Page_Frame_Allocator::unreserve_pages(void* address, uint64_t pageCount){
    for(uint64_t i = 0; i < pageCount; i++){
        unreserve_page((void*)((uint64_t)address + (i * PAGE_SIZE)));
    }
}

void* Page_Frame_Allocator::request_page(){
    return (void*)0;
}

uint64_t Page_Frame_Allocator::get_free_RAM() {return freeMemory;}
uint64_t Page_Frame_Allocator::get_used_RAM() {return usedMemory;}
uint64_t Page_Frame_Allocator::get_reserved_RAM() {return reservedMemory;}
uint64_t Page_Frame_Allocator::get_total_RAM() {return totalMemory;}

uint64_t Page_Frame_Allocator::getMemorySize(multiboot_info_t* mbd) {

    totalMemory = 0;

    uint64_t mmap_addr = mbd->mmap_addr, mmap_length = mbd->mmap_length;
    uint64_t i = 0;
    while(i < mmap_length) {
        multiboot_memory_map_t* entry = (multiboot_memory_map_t*) (mmap_addr + i);
        totalMemory += (((uint64_t)entry->length_high) << 32) + entry->length_low;
        i += entry->size + 4;
    }
    return totalMemory;
}