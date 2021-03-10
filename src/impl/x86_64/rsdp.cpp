#include "rsdp.h"
#include <stddef.h>
#include "text_console.h"
#include "page_frame_allocator.h"
#include "mem_util.h"

extern Text_Console GlobalConsole;
extern Page_Frame_Allocator GlobalAllocator;

RSDP_Descriptor rsdp;

RSDP_Descriptor* find_rspd(multiboot_info_t* mbd){

    uint64_t mmap_addr = mbd->mmap_addr, mmap_length = mbd->mmap_length, total_ram = GlobalAllocator.get_total_RAM();
    uint64_t i = 0, j;
    while(i < mmap_length) {
        multiboot_memory_map_t* entry = (multiboot_memory_map_t*) (mmap_addr + i);
        if(entry->type == MULTIBOOT_MEMORY_RESERVED){
            uint64_t size = ((((uint64_t)entry->length_high) << 32) + entry->length_low);
            uint64_t address = ((((uint64_t)entry->base_addr_high) << 32) + entry->base_addr_low) & 0xFFFFFFFFFFFFFFF0;
            for(j = address; j < address + size - 16 && j < total_ram; j+= 16){
                if(memcmp((uint64_t*)j, "RSD PTR ", 8)) {
                    rsdp = *(RSDP_Descriptor*)j;
                    return &rsdp;
                }
            }
        }
        i += entry->size + 4;
    }

    return (RSDP_Descriptor*)NULL;
}