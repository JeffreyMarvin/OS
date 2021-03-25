#include "page_table.h"

void Page_Table_Entry::set_flag(PT_Flag flag, bool enabled){
    uint64_t bitSelector = (uint64_t)1 << flag;
    value &= ~bitSelector;
    if (enabled){
        value |= bitSelector;
    }
}

bool Page_Table_Entry::get_flag(PT_Flag flag){
    uint64_t bitSelector = (uint64_t)1 << flag;
    return value & bitSelector > 0;
}

uint64_t Page_Table_Entry::get_addr(){
    return (value & 0x000ffffffffff000) >> 12;
}

void Page_Table_Entry::set_addr(uint64_t address){
    address &= 0x000000ffffffffff;
    value &= 0xfff0000000000fff;
    value |= (address << 12);
}
