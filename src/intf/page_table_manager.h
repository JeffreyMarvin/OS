#pragma once
#include <stdint.h>
#include "page_table.h"

extern Page_Table page_table_l4;

class Page_Table_Manager {
    public:
        Page_Table_Manager(Page_Table* PML4_addr = &page_table_l4);
        Page_Table* PML4;
        void map_memory(void* virtual_addr, void* physical_addr);
};
