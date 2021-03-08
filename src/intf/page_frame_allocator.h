#pragma once

#include <stdint.h>
#include "multiboot.h"
#include "page_table.h"

struct bitmap {
    uint64_t size;
    uint8_t* buffer;
    bool get(uint64_t index);
    bool set(uint64_t index, bool value);
};

class Page_Frame_Allocator {
    public:
        Page_Frame_Allocator(multiboot_info_t* mMap);
        void free_page(void* address);
        void free_pages(void* address, uint64_t pageCount);
        void lock_page(void* address);
        void lock_pages(void* address, uint64_t pageCount);
        void* request_page();
        uint64_t get_free_RAM();
        uint64_t get_used_RAM();
        uint64_t get_reserved_RAM();
        uint64_t get_total_RAM();

    private:
        bitmap pageBitmap;
        void reserve_page(void* address);
        void reserve_pages(void* address, uint64_t pageCount);
        void unreserve_page(void* address);
        void unreserve_pages(void* address, uint64_t pageCount);
        uint64_t freeMemory = 0;
        uint64_t reservedMemory = 0;
        uint64_t usedMemory = 0;
        uint64_t totalMemory = 0;
        uint64_t getMemorySize(multiboot_info_t* mbd);
};
