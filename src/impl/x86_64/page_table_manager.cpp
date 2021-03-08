#include "page_table_manager.h"
#include "page_frame_allocator.h"
#include "mem_util.h"

extern Page_Frame_Allocator GlobalAllocator;

struct Page_Map_Index {
    uint16_t PDP_i;
    uint16_t PD_i;
    uint16_t PT_i;
    uint16_t P_i;
};

Page_Map_Index calculate_page_map_index(uint64_t virtual_addr){
    Page_Map_Index index;
    virtual_addr >>= 12;
    index.P_i = virtual_addr & 0x1ff;
    virtual_addr >>= 9;
    index.PT_i = virtual_addr & 0x1ff;
    virtual_addr >>= 9;
    index.PD_i = virtual_addr & 0x1ff;
    virtual_addr >>= 9;
    index.PDP_i = virtual_addr & 0x1ff;
    return index;
}

Page_Table_Manager::Page_Table_Manager(Page_Table* PML4_addr){
    PML4 = PML4_addr;
}

void Page_Table_Manager::map_memory(void* virtual_addr, void* physical_addr){
    Page_Map_Index index = calculate_page_map_index((uint64_t)virtual_addr);
    Page_Table_Entry PDE;

    PDE = PML4->entries[index.PDP_i];
    Page_Table* PDP;
    if (!PDE.get_flag(PT_Flag::Present)){
        PDP = (Page_Table*)GlobalAllocator.request_page();
        memset(PDP, 0, PAGE_SIZE);
        PDE.set_addr((uint64_t)PDP >> 12);
        PDE.set_flag(PT_Flag::Present, true);
        PDE.set_flag(PT_Flag::ReadWrite, true);
        PML4->entries[index.PDP_i] = PDE;
    }
    else
    {
        PDP = (Page_Table*)((uint64_t)PDE.get_addr() << 12);
    }
    
    
    PDE = PDP->entries[index.PD_i];
    Page_Table* PD;
    if (!PDE.get_flag(PT_Flag::Present)){
        PD = (Page_Table*)GlobalAllocator.request_page();
        memset(PD, 0, 0x1000);
        PDE.set_addr((uint64_t)PD >> 12);
        PDE.set_flag(PT_Flag::Present, true);
        PDE.set_flag(PT_Flag::ReadWrite, true);
        PDP->entries[index.PD_i] = PDE;
    }
    else
    {
        PD = (Page_Table*)((uint64_t)PDE.get_addr() << 12);
    }

    PDE = PD->entries[index.PT_i];
    Page_Table* PT;
    if (!PDE.get_flag(PT_Flag::Present)){
        PT = (Page_Table*)GlobalAllocator.request_page();
        memset(PT, 0, 0x1000);
        PDE.set_addr((uint64_t)PT >> 12);
        PDE.set_flag(PT_Flag::Present, true);
        PDE.set_flag(PT_Flag::ReadWrite, true);
        PD->entries[index.PT_i] = PDE;
    }
    else
    {
        PT = (Page_Table*)((uint64_t)PDE.get_addr() << 12);
    }

    PDE = PT->entries[index.P_i];
    PDE.set_addr((uint64_t)physical_addr >> 12);
    PDE.set_flag(PT_Flag::Present, true);
    PDE.set_flag(PT_Flag::ReadWrite, true);
    PT->entries[index.P_i] = PDE;
}

extern Page_Table_Manager GlobalPageManager = Page_Table_Manager();