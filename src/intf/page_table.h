#pragma once
#include <stdint.h>

#define PAGE_SIZE 4096

enum PT_Flag {
    Present = 0,
    ReadWrite = 1,
    UserSuper = 2,
    WriteThrough = 3,
    CacheDisabled = 4,
    Accessed = 5,
    LargerPages = 7,
    Custom0 = 9,
    Custom1 = 10,
    Custom2 = 11,
    NX = 63 // only if supported
};

struct Page_Table_Entry {
    uint64_t value;
    void set_flag(PT_Flag flag, bool enabled);
    bool get_flag(PT_Flag flag);
    void set_addr(uint64_t address);
    uint64_t get_addr();
};

struct Page_Table { 
    Page_Table_Entry entries [PAGE_SIZE / sizeof(Page_Table_Entry)];
}__attribute__((aligned(PAGE_SIZE)));